#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include<windows.h>
#include <Inventor/Win/SoWin.h>
#include<Inventor/nodes/SoTransform.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include<Inventor/nodes/SoLineSet.h>
#include<Inventor\nodes\SoText2.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCylinder.h>
#include<Inventor\nodes\SoPointSet.h>
#include<Inventor\actions\SoWriteAction.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include"Projections.h"
#include<string>

int XMAX, YMAX, ZMAX;
const unsigned short
	patterns[5] = {
	0xFFFF, // solid
	0x000F, // dash 
	0x1111, // dot
	0x041F, // dash-dot
	0x111F, // dash-dot-dot
};

SoSeparator* drawLines(SbVec3f *verLine, int totalpoints, SbColor color, char *txt, float width, SbVec3f verLabel)
{
	SoSeparator *step = new SoSeparator;
	SoCoordinate3 *coord = new SoCoordinate3();
	SoLineSet *line = new SoLineSet();
	//label
	SoText2 *label = new SoText2;
	label->string.setValue(txt);
	SoTransform *labelTransform = new SoTransform;
	labelTransform->translation.setValue(verLabel);
	//color
	SoMaterial *linecolor = new SoMaterial;
	linecolor->diffuseColor.setValue(color);
	step->addChild(linecolor);
	//line properties
	SoDrawStyle *linePro = new SoDrawStyle;
	linePro->lineWidth = width;

	//line
	coord->point.setValues(0, totalpoints, verLine);
	line->numVertices.set1Value(0, totalpoints);
	step->addChild(linePro);
	step->addChild(coord);
	step->addChild(line);
	step->addChild(labelTransform);
	step->addChild(label);
	return step;
}
SoSeparator * ProjectionBox::drawSteps2(int scale, int size, int labelflag, SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2)
{
	SbVec3f ver[2];
	char text[10];
	//Major axis
	float count=YMAX;
	SoSeparator *root=new SoSeparator;
	for (int i = 0; i <= step; i++,count=(step-i)*scale)
	{
		if (x1 == 0 && y1 == 0 && z1 == 0)
		{
			if (x2 == XMAX && y2 == 0 && z2 == 0)
			{
				ver[0] = SbVec3f(i*scale, size, 0);
				ver[1] = SbVec3f(i*scale, -size, 0);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, ver[0]));
			}
			if (x2 == 0 && y2 == YMAX && z2 == 0)
			{
				ver[0] = SbVec3f(size, i*scale, 0);
				ver[1] = SbVec3f(-size, i*scale, 0);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(count, text, 10), 2.2, ver[0]));
			}
			if (x2 == 0 && y2 == 0 && z2 == ZMAX)
			{
				ver[0] = SbVec3f(size, 0, i*scale);
				ver[1] = SbVec3f(-size, 0, i*scale);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, ver[0]));
			}

		}
		if (x1 == XMAX && y1 == YMAX && z1 == ZMAX)
		{
			if (x2 == XMAX && y2 == YMAX && z2 == 0)
			{
				ver[0] = SbVec3f(XMAX, YMAX, i*scale);
				ver[1] = SbVec3f(XMAX, YMAX - size, i*scale);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(XMAX, YMAX, i*scale)));
			}
			if (x2 == 0 && y2 == YMAX && z2 == ZMAX)
			{


				ver[0] = SbVec3f(i*scale, YMAX, ZMAX);
				ver[1] = SbVec3f(i*scale, YMAX - size, ZMAX);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(i*scale, YMAX, ZMAX)));
			}
		}

		if (x1 == 0 && y1 == YMAX && z1 == ZMAX)
		{
			if (x2 == 0 && y2 == 0 && z2 == ZMAX)
			{
				ver[0] = SbVec3f(0, i*scale, ZMAX);
				ver[1] = SbVec3f(0, i*scale, ZMAX - size);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(count, text, 10), 2.2, SbVec3f(0, i*scale, ZMAX)));
			}
			if (x2 == 0 && y2 == YMAX && z2 == 0)
			{
				ver[0] = SbVec3f(0, YMAX, i*scale);
				ver[1] = SbVec3f(size, YMAX, i*scale);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(0, YMAX, i*scale)));
			}
		}
		if (x1 == XMAX && y1 == YMAX && z1 == 0)
		{
			if (x2 == XMAX && y2 == 0 && z2 == 0)
			{
				ver[0] = SbVec3f(XMAX, i*scale, 0);
				ver[1] = SbVec3f(XMAX - size, i*scale, 0);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(count, text, 10), 2.2, SbVec3f(XMAX, i*scale, 0)));
			}
			if (x2 == 0 && y2 == YMAX && z2 == 0)
			{
				ver[0] = SbVec3f(i*scale, YMAX, 0);
				ver[1] = SbVec3f(i*scale, YMAX - size, 0);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(i*scale, YMAX, 0)));
			}
		}
		if (x1 == XMAX && y1 == 0 && z1 == ZMAX)
		{
			if (x2 == 0 && y2 == 0 && z2 == ZMAX)
			{
				ver[0] = SbVec3f(i*scale, 0, ZMAX);
				ver[1] = SbVec3f(i*scale, size, ZMAX);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(i*scale, 0, ZMAX)));
			}
			if (x2 == XMAX && y2 == 0 && z2 == 0)
			{
				ver[0] = SbVec3f(XMAX, 0, i*scale);
				ver[1] = SbVec3f(XMAX, size, i*scale);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(XMAX, 0, i*scale)));
			}
			if (x2 == XMAX && y2 == YMAX && z2 == ZMAX)
			{
				ver[0] = SbVec3f(XMAX, i*scale, ZMAX);
				ver[1] = SbVec3f(XMAX, i*scale, ZMAX - size);
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(count, text, 10), 2.2, SbVec3f(XMAX, i*scale, ZMAX)));
			}
		}

	}

	return root;
}
SoSeparator * ProjectionBox::getBox()
{

	XMAX = bigx + 30;
	YMAX = bigy + 30;
	ZMAX = bigz + 60;

	//root nodes 
	SoSeparator *root = new SoSeparator;
	SoTransform *trans = new SoTransform;
	float x, y, z;
	points[0].getValue(x, y, z);
	x = 0; z = 0; y = -(YMAX - y);
	trans->translation.setValue(x, y, z);
	root->addChild(trans);
	float color[3] = { 1,0,0 };
	//Y- axis co-ordinates
	SbVec3f yvert[2] = { SbVec3f(0,YMAX,0),SbVec3f(0,0,0) };
	//X- axis co-ordinate
	SbVec3f xvert[2] = { SbVec3f(XMAX,0,0),SbVec3f(0,0,0) };
	//Z- axis co-ordinate
	SbVec3f zvert[2] = { SbVec3f(0,0,ZMAX),SbVec3f(0,0,0) };
	//X-Y axis co-ordinate
	SbVec3f ver1[2] = { SbVec3f(0,0,ZMAX),SbVec3f(0,YMAX,ZMAX) };
	SbVec3f ver2[2] = { SbVec3f(0,YMAX,ZMAX),SbVec3f(XMAX,YMAX,ZMAX) };
	SbVec3f ver3[2] = { SbVec3f(0,YMAX,0),SbVec3f(XMAX,YMAX,0) };
	SbVec3f ver4[2] = { SbVec3f(0,YMAX,ZMAX),SbVec3f(0,YMAX,0) };
	SbVec3f ver5[2] = { SbVec3f(0,0,ZMAX),SbVec3f(XMAX,0,ZMAX) };
	SbVec3f ver6[2] = { SbVec3f(XMAX,0,ZMAX),SbVec3f(XMAX,0,0) };
	SbVec3f ver7[2] = { SbVec3f(XMAX,0,0),SbVec3f(XMAX,YMAX,0) };
	SbVec3f ver8[2] = { SbVec3f(XMAX,YMAX,0),SbVec3f(XMAX,YMAX,ZMAX) };
	SbVec3f ver9[2] = { SbVec3f(XMAX,YMAX,ZMAX),SbVec3f(XMAX,0,ZMAX) };

	root->ref();

	//add a seperator
	//set a name of the sep
	
	SoSeparator *temp;
	temp = drawLines(xvert, 2, SbColor(1, 0, 0), "", 4.0, SbVec3f(XMAX / 2, -15, 0));
	temp->setName("axis1");
	root->addChild(temp);
	
	temp = drawLines(yvert, 2, SbColor(0, 1, 0), "Y-axis", 4.0, SbVec3f(-15, YMAX / 2, 0));
	temp->setName("axis2");
	root->addChild(temp);

	temp = drawLines(zvert, 2, SbColor(0, 0, 1), "", 4.0, SbVec3f(0, -15, ZMAX / 2));
	temp->setName("axis3");
	root->addChild(temp);

	temp=drawLines(ver1, 2, SbColor(0, 1, 0), "", 4.0, SbVec3f(-15, YMAX / 2, 0));
	temp->setName("axis4");
	root->addChild(temp);

	temp=drawLines(ver9, 2, SbColor(0, 1, 0), "", 4.0, SbVec3f(-15, YMAX / 2, 0));
	temp->setName("axis5");
	root->addChild(temp);

	temp=drawLines(ver7, 2, SbColor(0, 1, 0), "", 4.0, SbVec3f(-15, YMAX / 2, 0));
	temp->setName("axis6");
	root->addChild(temp);

	temp=drawLines(ver2, 2, SbColor(1, 0, 0), "", 4.0, SbVec3f(XMAX / 2, -5, 0));
	temp->setName("axis7");
	root->addChild(temp);

	temp=drawLines(ver3, 2, SbColor(1, 0, 0), "X-axis", 4.0, SbVec3f(XMAX / 2, YMAX+15, -15));
	temp->setName("axis8");
	root->addChild(temp);

	temp=drawLines(ver5, 2, SbColor(1, 0, 0), "", 4.0, SbVec3f(XMAX / 2, -5, 0));
	temp->setName("axis9");
	root->addChild(temp);

	temp=drawLines(ver4, 2, SbColor(0, 0, 1), "Z-axis", 4.0, SbVec3f(-30, YMAX + 15, ZMAX / 2));
	temp->setName("axis10");
	root->addChild(temp);

	temp=drawLines(ver6, 2, SbColor(0, 0, 1), "", 4.0, SbVec3f(0, -5, ZMAX / 2));
	temp->setName("axis11");
	root->addChild(temp);

	temp=drawLines(ver8, 2, SbColor(0, 0, 1), "", 4.0, SbVec3f(0, -5, ZMAX / 2));
	temp->setName("axis12");
	root->addChild(temp);

	//steps
	//drawSteps2(int scale, int size, int labelflag, SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2)
	temp= drawSteps2(XMAX /  step, XMAX*0.02, 1,  SbColor(1, 1, 1), 0, 0, 0, XMAX, 0, 0);
	temp->setName("step1");
	root->addChild(temp);

	temp=drawSteps2(YMAX /  step, YMAX*0.02, 1,  SbColor(1, 1, 1), 0, 0, 0, 0, YMAX, 0);
	temp->setName("step2");
	root->addChild(temp);

	temp=drawSteps2(ZMAX /  step, ZMAX*0.02, 1,  SbColor(1, 1, 1), 0, 0, 0, 0, 0, ZMAX);
	temp->setName("step3");
	root->addChild(temp);
	//major

	temp=drawSteps2(ZMAX /  step, ZMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, YMAX, ZMAX, XMAX, YMAX, 0);
	temp->setName("step4");
	root->addChild(temp); 
	
	temp=drawSteps2(XMAX /  step, XMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, YMAX, ZMAX, 0, YMAX, ZMAX);
	temp->setName("step5");
	root->addChild(temp); 
	
	temp=drawSteps2(YMAX /  step, YMAX*0.02, 1,  SbColor(1, 1, 1), 0, YMAX, ZMAX, 0, 0, ZMAX);
	temp->setName("step6");
	root->addChild(temp); 
	
	temp=drawSteps2(ZMAX /  step, ZMAX*0.02, 1,  SbColor(1, 1, 1), 0, YMAX, ZMAX, 0, YMAX, 0);
	temp->setName("step7");
	root->addChild(temp);

	temp=drawSteps2(YMAX /  step, YMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, YMAX, 0, XMAX, 0, 0);
	temp->setName("step8");
	root->addChild(temp);

	temp=drawSteps2(XMAX /  step, XMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, YMAX, 0, 0, YMAX, 0);
	temp->setName("step9");
	root->addChild(temp); 
	
	temp=drawSteps2(XMAX /  step, XMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, 0, ZMAX, 0, 0, ZMAX);
	temp->setName("step10");
	root->addChild(temp); 
	
	temp=drawSteps2(ZMAX /  step, ZMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, 0, ZMAX, XMAX, 0, 0);
	temp->setName("step11");
	root->addChild(temp); 
	
	temp=drawSteps2(YMAX /  step, YMAX*0.02, 1,  SbColor(1, 1, 1), XMAX, 0, ZMAX, XMAX, YMAX, ZMAX);
	temp->setName("step12");
	root->addChild(temp);
	////Minor
	//drawSteps2(1, ZMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, YMAX, ZMAX, XMAX, YMAX, 0);
	//drawSteps2(1, XMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, YMAX, ZMAX, 0, YMAX, ZMAX);
	//drawSteps2(1, YMAX, 2, 0, root, SbColor(1, 1, 1), 0, YMAX, ZMAX, 0, 0, ZMAX);
	//drawSteps2(1, ZMAX, 2, 0, root, SbColor(1, 1, 1), 0, YMAX, ZMAX, 0, YMAX, 0);
	//drawSteps2(1, YMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, YMAX, 0, XMAX, 0, 0);
	//drawSteps2(1, XMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, YMAX, 0, 0, YMAX, 0);
	//drawSteps2(1, XMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, 0, ZMAX, 0, 0, ZMAX);
	//drawSteps2(1, ZMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, 0, ZMAX, XMAX, 0, 0);
	//drawSteps2(1, YMAX, 2, 0, root, SbColor(1, 1, 1), XMAX, 0, ZMAX, XMAX, YMAX, ZMAX);
	return root;
}


void rotateView(SoRotationXYZ *myRotXYZ, char axis, char rot)
{
	float rotation;
	if (rot == '+')
		rotation = M_PI / 8;
	else
		rotation = -(M_PI / 8);
	if (axis == 'x')
	{
		myRotXYZ->axis = SoRotationXYZ::X; // rotate about X axis
		myRotXYZ->angle.setValue(rotation);
	}
	if (axis == 'y')
	{
		myRotXYZ->axis = SoRotationXYZ::Y; // rotate about Y axis
		myRotXYZ->angle.setValue(rotation);
	}
	if (axis == 'z')
	{
		myRotXYZ->axis = SoRotationXYZ::Z; // rotate about Z axis
		myRotXYZ->angle.setValue(rotation);
	}
}