#include<windows.h>
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
#include <Inventor/nodes/SoRotationXYZ.h>
#include <math.h> // for M_PI

class Well {

	protected:
	int nPoints;      // Number of data points
	SbVec3f *points;// 3D coordinate of each data point
	float bigx, bigy, bigz;
	float pipedepth;
	float pipelength;
public:
	Well() : nPoints(0), points(NULL), bigx(0),bigy(0),bigz(0){}
	~Well()
	{
		delete points;
	}
	Well(const char *filename);
	Well(Well *pwell)
	{
		this->nPoints = pwell->nPoints;
		this->points = pwell->points;
		this->bigx = pwell->bigx;
		this->bigy = pwell->bigy;
		this->bigz = pwell->bigz;
	}
	public:
		int getPoints(SbVec3f to);
		SoSeparator *makeExtrusion(SbVec3f to, float scaleFactor);
		SoSeparator *drawWell();
		SoSeparator *getWellSymbol(SbVec3f point, int size);
		float depth();
		float pipeLength();
		float getbigx();
		float getbigy();
		float getbigz();
};
class ProjectionBox :public Well{
private: float min;
		 int step;
public: ProjectionBox(Well *pWell,float min,float step ) :Well(pWell)
		 {
			 
			 this->min = min;
			 this->step = step;
		 }
		SoSeparator * getBox();
		SoSeparator * drawSteps2(int scale, int size, int labelflag,SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2);
		
};

SoSeparator* drawLines(SbVec3f *verLine, int totalpoints, SbColor color, char *txt, float width, SbVec3f verLabel);
void rotateView(SoRotationXYZ *myRotXYZ, char axis,char rot);