#include<iostream>
#include<windows.h>
#include<string>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/VRMLnodes/SoVRMLExtrusion.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/Win/events/SoEvent.h>
#include <Inventor/Win/events/SoEvents.h>

#include <math.h>// for M_PI
#include"Projections.h"
float Well::getbigx()
{
	return bigx;
}
float Well::getbigy()
{
	return bigy;
}
float Well::getbigz()
{
	return bigz;
}
Well::Well(const char *filename)
{
	//Well *pWell = new Well;
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "*** Unable to open data file '%s'.\n", filename);
		
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
			this->nPoints = nPoints;
			points = new SbVec3f[nPoints];
			for (int i = 0; i < nPoints; ++i) {
				float x, y, z;
				p = fgets(buffer, BUFLEN, fp);
				sscanf(p, "%g %g %g", &x, &y, &z);
				points[i].setValue(x, y, z);
				if (x > bigx)bigx = x;
				if (y > bigy)bigy = y;
				if (z > bigz)bigz = z;
			}
		}
		}
	
	fclose(fp);
	
}


int Well:: getPoints( SbVec3f to)
{
	int index;
	for (int i = 0; i < nPoints; i++)
	{
		float x1, y1, z1;
		float x2, y2, z2;
		points[i].getValue(x1, y1, z1);
		to.getValue(x2, y2, z2);
		if (x1 == x2 && y1 == y2 && z1 == z2)
			return i;
	}

	return 0;
}


SoSeparator * Well::
makeExtrusion( SbVec3f to, float scaleFactor)
{
	// Group the attribute nodes and extrusion
	SoSeparator *pSep = new SoSeparator;
	int   numSides;
	int noPoints = getPoints(to);
	SbVec3f *temppoints = new SbVec3f[noPoints];
	memcpy(temppoints, points, noPoints*sizeof(points[0]));
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
	numSides = 500;
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
	pExt->crossSection.set1Value(numSides, SbVec2f(0, 0.5f)); 
	pExt->spine.setValues(0, noPoints, temppoints);
	pExt->scale.setNum(nPoints);
	pExt->scale.set1Value(0, SbVec2f(scaleFactor+10, scaleFactor+10));
	for (int i = 1; i <nPoints-2; ++i) 
	{
		pExt->scale.set1Value(i, SbVec2f(scaleFactor, scaleFactor));
	}
	pExt->scale.set1Value(nPoints - 2, SbVec2f(scaleFactor + 4, scaleFactor + 4));
	pSep->addChild(pExt);
	return pSep;
}
using namespace std;
SoSeparator* Well:: drawWell()
{
	SoSeparator * root = new SoSeparator;
	int totalcasing;
	std::cout << "\n\nTotal points read:";
	std::cout <<nPoints << std::endl;
	std::cout << "\n\nTotal number of casing:";
	std::cin >> totalcasing;
	float bigsize=0;
	for (int i = 0; i < totalcasing; i++)
	{
		float size;
		int index;
		std::cout << "Casing " << i + 1 << " Enter size and point number:";
	//again:
		std::cin >> size >> index;
		//if (index >= pWell->nPoints)
		//	goto again;
		SoSeparator *temp = makeExtrusion(points[index], size);
		std::string nm = "casing"+std::to_string(i+1);
		temp->setName(SbName(nm.c_str()));
		root->addChild(temp);
		if (size > bigsize)bigsize = size;
	}
	int welltype;
	std::cout << "\n\n1.Oil Well\n2.Gas Well";
	std::cout << "\n Choose option:";
	std::cin >> welltype;
	if (welltype == 2)
		root->addChild(getWellSymbol(points[0], bigsize));
	/*char name[30];
	cout << "\nEnter casing number:";
	cin >> name;
	SoSeparator *temp;
	cout << endl << name;
	temp = (SoSeparator*)SoNode::getByName(SbName(name));
	root->removeChild(temp);*/
	return root;
}
float Well::depth()
{
	float x1,y1,z1,x2, y2,z2;
	points[0].getValue(x1, y1,z1 );
	points[nPoints-1].getValue(x2, y2, z2);
	this->pipedepth=(abs(y2 - y1));
	return pipedepth;
}
float Well::pipeLength()
{
	float x1, y1, z1, x2, y2, z2;
	float realLength = 0;
	for (int i = 0; i < nPoints-1; i++)
	{
		points[i].getValue(x1, y1, z1);
		points[i+1].getValue(x2, y2, z2);
		realLength+=sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
	}
	this->pipelength = realLength;
	return realLength;
}
SoSeparator* Well:: getWellSymbol(SbVec3f point, int size)
{
	
	SoSeparator *node = new SoSeparator;
	float x, y, z;
	size += 2;
	point.getValue(x, y, z);
	SbVec3f v[2] = { SbVec3f(x - size,y,z - size),SbVec3f(x + size,y,z + size) };
	node->addChild(drawLines(v, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	SbVec3f v1[2] = { SbVec3f(x ,y,z - (size + 7)),SbVec3f(x ,y,z + size + 7) };
	node->addChild(drawLines(v1, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	SbVec3f v2[2] = { SbVec3f(x - size,y,z + size),SbVec3f(x + size,y,z - size) };
	node->addChild(drawLines(v2, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	SbVec3f v3[2] = { SbVec3f(x - (size + 7),y,z),SbVec3f(x + size + 7,y,z) };
	node->addChild(drawLines(v3, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	return node;
}