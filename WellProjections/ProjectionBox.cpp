#define _SCL_SECURE_NO_WARNINGS
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
		SoSeparator* drawLines(SbVec3f *verLine, int totalpoints, SbColor color, char *txt, float width, SbVec3f verLabel);
		void drawSteps2(int scale, int step, int size, int labelflag, SoSeparator* root, SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2);
		SoSeparator * getBox(int xmax, int ymax, int zmax, int min, int STEP);
		SoSeparator* getOilSymbol(SbVec3f point, int size);
		SoSeparator * getWellSymbol(SbVec3f point, int size);



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
void drawSteps2(int scale, int step, int size, int labelflag, SoSeparator* root, SbColor colorbig, int x1, int y1, int z1, int x2, int y2, int z2)
{
	SbVec3f ver[2];
	char text[10];
	//Major axis
	for (int i = 0; i <= step; i++)
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
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, ver[0]));
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
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(0, i*scale, ZMAX)));
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
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(XMAX, i*scale, 0)));
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
				root->addChild(drawLines(ver, 2, colorbig, labelflag == 0 ? "" : itoa(i*scale, text, 10), 2.2, SbVec3f(XMAX, i*scale, ZMAX)));
			}
		}

	}


}
SoSeparator * getBox(int xmax, int ymax, int zmax, int min, int STEP)
{

	XMAX = xmax;
	YMAX = ymax;
	ZMAX = zmax;

	//root nodes 
	SoSeparator *root = new SoSeparator;

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


	root->addChild(drawLines(xvert, 2, SbColor(1, 0, 0), "X-axis", 4.0, SbVec3f(XMAX / 2, -15, 0)));
	root->addChild(drawLines(yvert, 2, SbColor(0, 1, 0), "Y-axis", 4.0, SbVec3f(-15, YMAX / 2, 0)));
	root->addChild(drawLines(zvert, 2, SbColor(0, 0, 1), "Z-axis", 4.0, SbVec3f(0, -15, ZMAX / 2)));
	root->addChild(drawLines(ver1, 2, SbColor(0, 1, 0), "", 4.0, SbVec3f(-15, YMAX / 2, 0)));
	root->addChild(drawLines(ver9, 2, SbColor(0, 1, 0), "", 4.0, SbVec3f(-15, YMAX / 2, 0)));
	root->addChild(drawLines(ver7, 2, SbColor(0, 1, 0), "", 4.0, SbVec3f(-15, YMAX / 2, 0)));
	root->addChild(drawLines(ver2, 2, SbColor(1, 0, 0), "", 4.0, SbVec3f(XMAX / 2, -5, 0)));
	root->addChild(drawLines(ver3, 2, SbColor(1, 0, 0), "", 4.0, SbVec3f(XMAX / 2, -5, 0)));
	root->addChild(drawLines(ver5, 2, SbColor(1, 0, 0), "", 4.0, SbVec3f(XMAX / 2, -5, 0)));
	root->addChild(drawLines(ver4, 2, SbColor(0, 0, 1), "", 4.0, SbVec3f(0, -5, ZMAX / 2)));
	root->addChild(drawLines(ver6, 2, SbColor(0, 0, 1), "", 4.0, SbVec3f(0, -5, ZMAX / 2)));
	root->addChild(drawLines(ver8, 2, SbColor(0, 0, 1), "", 4.0, SbVec3f(0, -5, ZMAX / 2)));
	//steps
	drawSteps2(XMAX / STEP, STEP, xmax*0.02, 1, root, SbColor(1, 1, 1), 0, 0, 0, XMAX, 0, 0);
	drawSteps2(YMAX / STEP, STEP, ymax*0.02, 1, root, SbColor(1, 1, 1), 0, 0, 0, 0, YMAX, 0);
	drawSteps2(ZMAX / STEP, STEP, zmax*0.02, 1, root, SbColor(1, 1, 1), 0, 0, 0, 0, 0, ZMAX);
	//major

	drawSteps2(ZMAX / STEP, STEP, zmax*0.02, 1, root, SbColor(1, 1, 1), XMAX, YMAX, ZMAX, XMAX, YMAX, 0);
	drawSteps2(XMAX / STEP, STEP, xmax*0.02, 1, root, SbColor(1, 1, 1), XMAX, YMAX, ZMAX, 0, YMAX, ZMAX);
	drawSteps2(YMAX / STEP, STEP, ymax*0.02, 1, root, SbColor(1, 1, 1), 0, YMAX, ZMAX, 0, 0, ZMAX);
	drawSteps2(ZMAX / STEP, STEP, zmax*0.02, 1, root, SbColor(1, 1, 1), 0, YMAX, ZMAX, 0, YMAX, 0);
	drawSteps2(YMAX / STEP, STEP, ymax*0.02, 1, root, SbColor(1, 1, 1), XMAX, YMAX, 0, XMAX, 0, 0);
	drawSteps2(XMAX / STEP, STEP, xmax*0.02, 1, root, SbColor(1, 1, 1), XMAX, YMAX, 0, 0, YMAX, 0);
	drawSteps2(XMAX / STEP, STEP, xmax*0.02, 1, root, SbColor(1, 1, 1), XMAX, 0, ZMAX, 0, 0, ZMAX);
	drawSteps2(ZMAX / STEP, STEP, zmax*0.02, 1, root, SbColor(1, 1, 1), XMAX, 0, ZMAX, XMAX, 0, 0);
	drawSteps2(YMAX / STEP, STEP, ymax*0.02, 1, root, SbColor(1, 1, 1), XMAX, 0, ZMAX, XMAX, YMAX, ZMAX);

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
SoSeparator* getOilSymbol(SbVec3f point, int size)
{
	SoSeparator *node = new SoSeparator;
	SoTransform *trans = new SoTransform;
	SoCylinder *cylinder = new SoCylinder;
	SoMaterial *color = new SoMaterial;
	SbVec3f ver = point;
	ver.operator+=(SbVec3f(0, 4, 0));

	color->ambientColor.setValue(.33, .22, .27);
	color->diffuseColor.setValue(1, 1, 1);
	color->specularColor.setValue(.99, .94, .81);
	color->shininess = .28;

	cylinder->radius = size;
	cylinder->height = 0;
	trans->translation.setValue(ver);
	node->addChild(trans);
	node->addChild(color);
	node->addChild(cylinder);
	return node;
}
SoSeparator *getWellSymbol(SbVec3f point, int size)
{
	SoSeparator *node = new SoSeparator;
	node->addChild(getOilSymbol(point, size));
	float x, y, z;
	size += 4;
	point.getValue(x, y, z);
	SbVec3f v[2] = { SbVec3f(x - size,y,z - size),SbVec3f(x + size,y,z + size) };
	node->addChild(drawLines(v, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	SbVec3f v1[2] = { SbVec3f(x ,y,z - (size + 15)),SbVec3f(x ,y,z + size + 15) };
	node->addChild(drawLines(v1, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	SbVec3f v2[2] = { SbVec3f(x - size,y,z + size),SbVec3f(x + size,y,z - size) };
	node->addChild(drawLines(v2, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	SbVec3f v3[2] = { SbVec3f(x - (size + 15),y,z),SbVec3f(x + size + 15,y,z) };
	node->addChild(drawLines(v3, 2, SbColor(1, 1, 1), "", 4, SbVec3f(0, 0, 0)));
	return node;
}

