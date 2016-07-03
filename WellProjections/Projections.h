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

class Well {// Each well trajectory corresponds to one object of class Well

	protected:
	int nPoints;      // Number of data points
	SbVec3f *points;// 3D coordinate of each data point
	float bigx, bigy, bigz;// Maximum coordinate values for creation of bounded box
	float pipedepth;// Total vertical underground depth of pipes
	float pipelength;//Total length of pipes
public:
	Well() : nPoints(0), points(NULL), bigx(0),bigy(0),bigz(0){}//Constructor
	~Well()//Destructor
	{
		delete points;
	}
	Well(const char *filename);//Constructor to initalize	
	Well(Well *pwell)//Copy Constructor
	{
		this->nPoints = pwell->nPoints;
		this->points = pwell->points;
		this->bigx = pwell->bigx;
		this->bigy = pwell->bigy;
		this->bigz = pwell->bigz;
	}
	public:
		int getPoints(SbVec3f to);// return index of that point from list of points
		SoSeparator *makeExtrusion(SbVec3f to, float scaleFactor);//draw wells trajectory
		SoSeparator *drawWell();// it calls all other functions which makes well diagram
		SoSeparator *getWellSymbol(SbVec3f point, int size);// Draws Gas well Symbol
		float depth();//returns depth
		float pipeLength();//returns length
		float getbigx();//returns bigx
		float getbigy();//returns bigy
		float getbigz();//returns bigz
};
class ProjectionBox :public Well{
private: float min;//By default it is 0
		 int step;// number of steps or marks on each axis
public: ProjectionBox(Well *pWell,float min,float step ) :Well(pWell)
		 {
			 this->min = min;
			 this->step = step;
		 }
		SoSeparator * getBox();//Draws box around the well 
		//Draws steps at particular positon at particular size.
		SoSeparator * drawSteps2(int scale, int size, int labelflag,SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2);
		
};
//A direct fuction that draws lines between points and returns a separator
SoSeparator* drawLines(SbVec3f *verLine, int totalpoints, SbColor color, char *txt, float width, SbVec3f verLabel);
//To rotate whole view in particular axis before rendering.
void rotateView(SoRotationXYZ *myRotXYZ, char axis,char rot);