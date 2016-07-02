/*
	A simple menu driven program to read data file and display its corresponding projection.
*/
#include<windows.h>
#include<iostream>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h>
#include <math.h> // for M_PI
#include"Projections.h"
using namespace std;

int main(int, char ** argv)
{
	HWND window = SoWin::init(argv[0]);
	if (window == NULL) exit(1);
	
	SoSeparator * root = new SoSeparator;// The root node, all nodes will be child of this node.
	root->ref();// root count increased to 1
	SoRotationXYZ *myRotXYZ = new SoRotationXYZ; // For rotation.
	root->addChild(myRotXYZ);// adding nodes to scenegraph
	int choice=1;
	char FileName[50];
	SoSeparator *temp;
	while (choice)//A loop to control menu, Enter 0 for exit
	{
		cout << "\n1. Read coordinates file";
		cout << "\n2. Remove casing";// Not yet programmend
		cout << "\n3. Rotate Camera";
		cout << "\n4. Import IV";
		cout << "\n5. Export IV";
		cout << "\n Choose option:";
		cin >> choice;
		if (choice == 1)
		{
			cout << "\nEnter file name:";
			scanf_s("%s", FileName, 50);//Reading file name from console
			Well *pWell = new Well(FileName);// Creating an object of Well
			cout << "\nAboslute Depth" << pWell->depth();//Calcluating Depth
			cout << "\nReal length of pipe" << pWell->pipeLength()<<endl;//Calcluating total pipe length
			temp = pWell->drawWell();// Well is constructed and its node is returned
			temp->setName(FileName);//Well node is named
			root->addChild(temp);//Newly constructed well is addded to scenegraph
			ProjectionBox *box = new ProjectionBox(pWell, 0, 10);// A bounding box inside which all trajectories will be shown
			root->addChild(box->getBox()); //A bounding box node is added in scenegraph
		}
		if (choice == 2)//YET TO PROGRAMME , IT IS TO REMOVE CASING NODES FROM GRAPH
		{
			/*char name[30];
			cout << "\nEnter casing number:";
			cin >> name;
			SoSeparator *temp;
			cout << endl<<name;
			temp = (SoSeparator*)SoNode::getByName(SbName(name));
			temp->removeChild(temp);*/
		}
		if (choice == 3)//Rotation of diagram at certain angle.
		{
			char axis,rot;
			cout << "Rotation in x,y,z?:";
			cin >> axis;
			cout << "+ or - rotation?:";
			cin >> rot;
			rotateView(myRotXYZ, axis, rot);

		}
		if (choice == 4)// Construct whole diagram directly from an .IV file.
		{
			SoInput myInput;
			cout << "\nEnter file name:";
			scanf_s("%s", FileName, 50);
			if (!myInput.openFile(FileName))
				return 1;
			SoSeparator *fileContents = SoDB::readAll(&myInput);//Reading the .iv file.
			if (fileContents == NULL)
				return 1;
			root->addChild(fileContents);// applying it to root node.
		}
		if (choice == 5)//Save current visulatiztion as .iv file which can be opened in any CAD, CAMD software too.
		{
			cout << "\nEnter file name:";
			scanf_s("%s", FileName, 50);
			SoWriteAction myAction;
			myAction.getOutput()->openFile(FileName);
			myAction.getOutput()->setBinary(FALSE);//Data saved as text
			myAction.apply(root);
			myAction.getOutput()->closeFile();
		}
	}
	
	
	
	//root->addChild(getBox(pWell1->bigx + 20, pWell1->bigy , pWell1->bigz + 20, 0, 10));
	SoWinExaminerViewer *myRenderArea = new SoWinExaminerViewer(window);
	myRenderArea->setSceneGraph(root);
	myRenderArea->setTitle("Wells");
	
	myRenderArea->show();
	SoWin::show(window);
	SoWin::mainLoop();

	//system("pause>NULL");
	delete myRenderArea;
	root->unref();
	return 0;
}