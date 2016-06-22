#define _SCL_SECURE_NO_WARNINGS
#include<windows.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoCoordinate3.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/VRMLnodes/SoVRMLExtrusion.h>

#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoPointSet.h>

#include <Inventor/nodes/SoSeparator.h>
#include <math.h> // for M_PI


float bigx = 0, bigy = 0, bigz = 0;
SoSeparator* drawLines(SbVec3f *verLine, int totalpoints, SbColor color, char *txt, float width, SbVec3f verLabel);
		void drawSteps2(int scale, int step, int size, int labelflag, SoSeparator* root, SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2);
		SoSeparator * getBox(int xmax, int ymax, int zmax, int min, int STEP);
		SoSeparator* getOilSymbol(SbVec3f point, int size);
		SoSeparator * getWellSymbol(SbVec3f point, int size);

class WellBore {

public:
	WellBore() : nPoints(0), points(NULL), dataMin(0),
		dataMax(1), data0(NULL), data1(NULL) {}

	~WellBore()
	{
		delete points;
		delete data0;
		delete data1;
	}

	int nPoints;      // Number of data points
	SbVec3f *points;  // 3D coordinate of each data point
	float    dataMin; // Smallest data value
	float    dataMax; // Largest data value
	float   *data0;   // First set of data values
	float   *data1;   // (Optional) Second set of data values
};

WellBore *
readWellBore(const char *filename)
{
	WellBore *pWell = new WellBore;

	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "*** Unable to open data file '%s'.\n", filename);
		return NULL;
	}

	int nPoints = 0;

	const int BUFLEN = 256;
	char buffer[BUFLEN];
	char *p;

	while ((p = fgets(buffer, BUFLEN, fp))) {
		while (*p == ' ') p++; // skip whitespace
		if (*p == '#' || *p == '\0' || *p == '\n') // Comment or end-of-line
			continue;
		if (!strncmp(p, "nVertices", 9)) {
			p += 9;
			sscanf(p, "%d", &nPoints);
		}
		else if (!strncmp(p, "Coords", 6)) {
			pWell->nPoints = nPoints;
			pWell->points = new SbVec3f[nPoints];
			for (int i = 0; i < nPoints; ++i) {
				float x, y, z;
				p = fgets(buffer, BUFLEN, fp);
				sscanf(p, "%g %g %g", &x, &y, &z);
				pWell->points[i].setValue(x, y, z);
				if (x > bigx)bigx = x;
				if (y > bigy)bigy = y;
				if (z > bigz)bigz = z;
			}
		}
		else if (!strncmp(p, "Data", 4)) {
			pWell->data0 = new float[nPoints];
			for (int i = 0; i < nPoints; ++i) {
				float val;
				p = fgets(buffer, BUFLEN, fp);
				sscanf(p, "%g", &val);
				pWell->data0[i] = val;
			}
		}
	}

	// We happen to know the range for the data in this example
	pWell->dataMin = 0;
	pWell->dataMax = 255;

	fclose(fp);
	printf("--- Loaded data file '%s': %d points.\n",
		filename, nPoints);

	return pWell;
}


int getPoints(WellBore *pWell, SbVec3f to)
{
	int index;
	for (int i = 0; i < pWell->nPoints; i++)
	{
		float x1, y1, z1;
		float x2, y2, z2;
		pWell->points[i].getValue(x1, y1, z1);
		to.getValue(x2, y2, z2);
		if (x1 == x2 && y1 == y2 && z1 == z2)
			return i;
	}

	return 0;
}


SoSeparator *
makeExtrusion(WellBore *pWell, SbVec3f to, float scaleFactor, SbColor color)
{
	// Group the attribute nodes and extrusion
	SoSeparator *pSep = new SoSeparator;
	int   numSides = 12;
	int nPoints = getPoints(pWell, to);
	SbVec3f *points = new SbVec3f[nPoints];
	memcpy(points, pWell->points, nPoints*sizeof(pWell->points[0]));
	// Extrusion will be considered "solid" to enable back-face culling.
	// Also set crease angle to "smooth" surface for more than 4 sides.
	SoShapeHints *pHints = new SoShapeHints;
	pHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	pHints->shapeType = SoShapeHints::CONVEX;
	pHints->creaseAngle = (float)(M_PI / 2.1);
	pSep->addChild(pHints);
	SoVRMLExtrusion *pExt = new SoVRMLExtrusion;
	//SoExtrusion *pExt = new SoExtrusion;
	// Compute x and z coordinates around circle
	numSides = 1000;
	int   side;
	float theta = 0.0f;
	float dTheta = (float)(2.0 * M_PI / (double)numSides);
	const float eps = 1e-6f;
	pExt->crossSection.setNum(numSides + 1);
	for (side = 0; side < numSides; side++) {
		float x = 0.5f * sin(theta);
		float z = 0.5f * cos(theta);
		if (fabs(x) < eps) x = 0;
		if (fabs(z) < eps) z = 0;
		pExt->crossSection.set1Value(side, SbVec2f(x, z));
		theta += dTheta;
	}
	SoMaterial *bronze = new SoMaterial;
	bronze->ambientColor.setValue(.33, .22, .27);
	bronze->diffuseColor.setValue(.78, .57, .11);
	bronze->specularColor.setValue(.99, .94, .81);
	bronze->shininess = .28;
	pSep->addChild(bronze);
	pExt->crossSection.set1Value(numSides, SbVec2f(0, 0.5f)); // close loop
															  // Coordinates of well bore define the spine

	pExt->spine.setValues(0, nPoints, points);
	// Get data range


	pExt->scale.setValue(SbVec2f(scaleFactor, scaleFactor));
	pSep->addChild(pExt);
	return pSep;
}



/////////////////////////////////////////////////////////////

int
main(int, char ** argv)
{

	HWND window = SoWin::init(argv[0]);
	if (window == NULL) exit(1);

	SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);

	
	SoSeparator * root = new SoSeparator;
	root->ref();

	// Try to read the files
	char *DataDir0 = "data0.txt";
	char *DataDir1 = "data1.txt";
	WellBore *pWell0 = readWellBore(DataDir0);
	WellBore *pWell1 = readWellBore(DataDir1);
	if (!pWell0 || !pWell1) {
		fprintf(stderr, "Exiting -- unable to load data or colormap\n");
		return 0;
	}

	// Simulate having a second data set on the second bore
	pWell1->data1 = new float[pWell1->nPoints];
	memcpy(pWell1->data1, pWell1->data0, pWell1->nPoints * sizeof(float));

	// First bore only has one data set, so default bore will have
	// diameter of 1.  Scale it by 8 to be more visible.
	
	root->addChild(getOilSymbol(pWell1->points[0], 30));
	root->addChild(makeExtrusion(pWell1, pWell1->points[49], 12, SbColor(1, 0, 0)));
	root->addChild(makeExtrusion(pWell1, pWell1->points[30], 20, SbColor(0, 1, 0)));
	root->addChild(makeExtrusion(pWell1, pWell1->points[10], 25, SbColor(0, 1, 1)));
	root->addChild(getWellSymbol(pWell0->points[0], 30));
	root->addChild(makeExtrusion(pWell0, pWell0->points[30], 30, SbColor(1, 1, 0)));
	root->addChild(getBox(bigx + 20, bigy + 20, bigz + 20, 0, 10));

	viewer->setSceneGraph(root);
	viewer->show();

	SoWin::show(window);
	SoWin::mainLoop();
	delete viewer;
	root->unref();
	return 0;
}



