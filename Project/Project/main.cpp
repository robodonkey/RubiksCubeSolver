#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

char topColor;
const int SOLUTION_SIZE = 200;
int cscounter = 0;
int xCoordinate, yCoordinate, solveCounter = 0, solutionIndex = 0, coorIndex = 0;
bool problem = false, validation = false;
int xCoor[9] = {25, 25, 25, 25, 25, 25, 25, 25, 25};
int yCoor[9] = {25, 25, 25, 25, 25, 25, 25, 25, 25};
string solution[SOLUTION_SIZE], shortestSolution[SOLUTION_SIZE], preoptimization[SOLUTION_SIZE]; 
char sideOne[3], sideTwo[3], sideThree[3], sideFour[3];

//Coordinates of the middles on a 2D grid
int lookUpTableMiddles[12][4] = 
{ 
	0, 4, 3, 10,
	1, 3, 3, 1,
	1, 5, 3, 7,
	2, 4, 3, 4,
	4, 0, 4, 11, 
	4, 2, 4, 3,
	4, 5, 4, 6,
	4, 8, 4, 9,
	5, 1, 7, 3,
	5, 4, 6, 4,
	5, 7, 7, 5,
	5, 10, 8, 4 
};

//Coordinates of the corners on a 2D grid
int lookUpTableCorners[8][6] = 
{
	0, 3, 3, 0, 3, 11,
	0, 5, 3, 8, 3, 9,
	2, 3, 3, 2, 3, 3,
	2, 5, 3, 5, 3, 6,
	5, 0, 8, 3, 5, 11,
	5, 2, 5, 3, 6, 3,
	5, 5, 5, 6, 6, 5,
	5, 8, 5, 9, 8, 5,
};

char cube[9][12] = 
{	
	'x', 'x', 'x', 'o', 'o', 'o', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'o', 'o', 'o', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'o', 'o', 'o', 'x', 'x', 'x', 'x', 'x', 'x',
	'w', 'w', 'w', 'b', 'b', 'b', 'y', 'y', 'y', 'g', 'g', 'g',
	'w', 'w', 'w', 'b', 'b', 'b', 'y', 'y', 'y', 'g', 'g', 'g',
	'w', 'w', 'w', 'b', 'b', 'b', 'y', 'y', 'y', 'g', 'g', 'g',
	'x', 'x', 'x', 'r', 'r', 'r', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'r', 'r', 'r', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'r', 'r', 'r', 'x', 'x', 'x', 'x', 'x', 'x',
};
char masterCube[9][12] = 
{	
	'x', 'x', 'x', 'o', 'o', 'o', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'o', 'o', 'o', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'o', 'o', 'o', 'x', 'x', 'x', 'x', 'x', 'x',
	'w', 'w', 'w', 'b', 'b', 'b', 'y', 'y', 'y', 'g', 'g', 'g',
	'w', 'w', 'w', 'b', 'b', 'b', 'y', 'y', 'y', 'g', 'g', 'g',
	'w', 'w', 'w', 'b', 'b', 'b', 'y', 'y', 'y', 'g', 'g', 'g',
	'x', 'x', 'x', 'r', 'r', 'r', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'r', 'r', 'r', 'x', 'x', 'x', 'x', 'x', 'x',
	'x', 'x', 'x', 'r', 'r', 'r', 'x', 'x', 'x', 'x', 'x', 'x',
};

//Rotate right side clockwise
void rightClock()
{
	//------------Get sides------------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[index][5];
	for(int index = 0; index < 3; index++)
		sideTwo[index] = cube[index][5];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[index][9];
	for(int index = 6; index < 9; index++)
		sideFour[index - 6] = cube[index][5];
	//----------Set Sides----------
	for(int index = 0; index < 3; index++)
		cube[3 + index][5] = sideFour[index];
	for(int index = 0; index < 3; index++)
		cube[index][5] = sideOne[index];
	for(int index = 0; index < 3; index++)
		cube[5 - index][9] = sideTwo[index];
	for(int index = 0; index < 3; index++)
		cube[8 - index][5] = sideThree[index];
	//-------------Get Face-----------
	for(int index = 6; index < 9; index++)
		sideOne[index - 6] = cube[3][index];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[index][8];
	for(int index = 6; index < 9; index++)
		sideThree[index - 6] = cube[5][index];
	for(int index = 3; index < 6; index++)
		sideFour[index - 3] = cube[index][6];
	//----------Set Faces--------
	for(int index = 6; index < 9; index++)
		cube[3][index] = sideFour[index - 6];
	for(int index = 3; index < 6; index++)
		cube[index][8] = sideOne[index - 3];
	for(int index = 0; index < 3; index++)
		cube[5][8 - index] = sideTwo[index];
	for(int index = 3; index < 6; index++)
		cube[index][6] = sideThree[index - 3];
}

//Rotate left side clockwise
void leftClock()
{
	//------------Get sides------------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[index][3];
	for(int index = 0; index < 3; index++)
		sideTwo[index] = cube[index][3];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[index][11];
	for(int index = 6; index < 9; index++)
		sideFour[index - 6] = cube[index][3];
	//----------Set Sides----------
	for(int index = 3; index < 6; index++)
		cube[index][11] = sideFour[5 - index];
	for(int index = 6; index < 9; index++)
		cube[index][3] = sideOne[index - 6];
	for(int index = 3; index < 6; index++)
		cube[index][3] = sideTwo[index - 3];
	for(int index = 0; index < 3; index++)
		cube[index][3] = sideThree[2 - index];
	//-------------Get Face-----------
	for(int index = 0; index < 3; index++)
		sideOne[index] = cube[3][index];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[index][2];
	for(int index = 0; index < 3; index++)
		sideThree[index] = cube[5][index];
	for(int index = 3; index < 6; index++)
		sideFour[index - 3] = cube[index][0];
	//----------Set Faces--------
	for(int index = 0; index < 3; index++)
		cube[3][2 - index] = sideFour[index];
	for(int index = 3; index < 6; index++)
		cube[index][2] = sideOne[index - 3];
	for(int index = 0; index < 3; index++)
		cube[5][2 - index] = sideTwo[index];
	for(int index = 3; index < 6; index++)
		cube[index][0] = sideThree[index - 3];
}

//Rotate top face clockwise
void upperClock()
{
	//------------Get sides------------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[index][6];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[6][index];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[index][2];
	for(int index = 3; index < 6; index++)
		sideFour[index - 3] = cube[2][index];
	//----------Set Sides----------
	for(int index = 3; index < 6; index++)
		cube[index][6] = sideFour[index - 3];
	for(int index = 0; index < 3; index++)
		cube[6][5 - index] = sideOne[index];
	for(int index = 3; index < 6; index++)
		cube[index][2] = sideTwo[index - 3];
	for(int index = 3; index < 6; index++)
		cube[2][index] = sideThree[5 - index];
	//-------------Get Face-----------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[3][index];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[index][5];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[5][index];
	for(int index = 3; index < 6; index++)
		sideFour[index - 3] = cube[index][3];
	//----------Set Faces--------
	for(int index = 3; index < 6; index++)
		cube[3][index] = sideFour[5 - index];
	for(int index = 3; index < 6; index++)
		cube[index][5] = sideOne[index - 3];
	for(int index = 3; index < 6; index++)
		cube[5][8 - index] = sideTwo[index - 3];
	for(int index = 3; index < 6; index++)
		cube[index][3] = sideThree[index - 3];
}

//Rotate front face clockwise
void frontClock()
{
	//------------Get sides------------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[5][index];
	for(int index = 6; index < 9; index++)
		sideTwo[index - 6] = cube[5][index];
	for(int index = 9; index < 12; index++)
		sideThree[index - 9] = cube[5][index];
	for(int index = 0; index < 3; index++)
		sideFour[index] = cube[5][index];
	//----------Set Sides----------
	for(int index = 3; index < 6; index++)
		cube[5][index] = sideFour[index - 3];
	for(int index = 6; index < 9; index++)
		cube[5][index] = sideOne[index - 6];
	for(int index = 9; index < 12; index++)
		cube[5][index] = sideTwo[index - 9];
	for(int index = 0; index < 3; index++)
		cube[5][index] = sideThree[index];
	//-------------Get Face-----------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[6][index];
	for(int index = 6; index < 9; index++)
		sideTwo[index - 6] = cube[index][5];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[8][index];
	for(int index = 6; index < 9; index++)
		sideFour[index - 6] = cube[index][3];
	//----------Set Faces--------
	for(int index = 3; index < 6; index++)
		cube[6][index] = sideFour[5 - index];
	for(int index = 6; index < 9; index++)
		cube[index][5] = sideOne[index - 6];
	for(int index = 3; index < 6; index++)
		cube[8][8 - index] = sideTwo[index - 3];
	for(int index = 6; index < 9; index++)
		cube[index][3] = sideThree[index - 6];
}

//Rotate the back face clockwise
void backClock()
{
	//------------Get sides------------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[3][index];
	for(int index = 6; index < 9; index++)
		sideTwo[index - 6] = cube[3][index];
	for(int index = 9; index < 12; index++)
		sideThree[index - 9] = cube[3][index];
	for(int index = 0; index < 3; index++)
		sideFour[index] = cube[3][index];
	//----------Set Sides----------
	for(int index = 9; index < 12; index++)
		cube[3][index] = sideFour[index - 9];
	for(int index = 0; index < 3; index++)
		cube[3][index] = sideOne[index];
	for(int index = 3; index < 6; index++)
		cube[3][index] = sideTwo[index - 3];
	for(int index = 6; index < 9; index++)
		cube[3][index] = sideThree[index - 6];
	//-------------Get Face-----------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[0][index];
	for(int index = 0; index < 3; index++)
		sideTwo[index] = cube[index][5];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[2][index];
	for(int index = 0; index < 3; index++)
		sideFour[index] = cube[index][3];
	//----------Set Faces--------
	for(int index = 3; index < 6; index++)
		cube[0][index] = sideFour[5 - index];
	for(int index = 0; index < 3; index++)
		cube[index][5] = sideOne[index];
	for(int index = 3; index < 6; index++)
		cube[2][8 - index] = sideTwo[index - 3];
	for(int index = 0; index < 3; index++)
		cube[index][3] = sideThree[index];
}

//Rotate the bottom face clockwise
void downClock()
{
	//-------------Get Face-----------
	for(int index = 9; index < 12; index++)
		sideOne[index - 9] = cube[3][index];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[index][11];
	for(int index = 11; index >= 9; index--)
		sideThree[11 - index] = cube[5][index];
	for(int index = 5; index >= 3; index--)
		sideFour[5 - index] = cube[index][9];
	//----------Set Faces--------
	for(int index = 9; index < 12; index++)
		cube[3][index] = sideFour[index - 9];
	for(int index = 3; index < 6; index++)
		cube[index][11] = sideOne[index - 3];
	for(int index = 9; index < 12; index++)
		cube[5][index] = sideTwo[11 - index];
	for(int index = 3; index < 6; index++)
		cube[index][9] = sideThree[5 - index];
	//------------Get sides------------
	for(int index = 3; index < 6; index++)
		sideOne[index - 3] = cube[0][index];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[index][8];
	for(int index = 3; index < 6; index++)
		sideThree[index - 3] = cube[8][index];
	for(int index = 3; index < 6; index++)
		sideFour[index - 3] = cube[index][0];
	//----------Set Sides----------
	for(int index = 3; index < 6; index++)
		cube[0][index] = sideTwo[index - 3];
	for(int index = 3; index < 6; index++)
		cube[index][8] = sideThree[5 - index];
	for(int index = 3; index < 6; index++)
		cube[8][index] = sideFour[index - 3];
	for(int index = 3; index < 6; index++)
		cube[index][0] = sideOne[5 - index];
}

//Rotating all faces counter clockwise is simply to rotate clockwise 3 times
void rightCounter()
{
	rightClock();
	rightClock();
	rightClock();
}
void leftCounter()
{
	leftClock();
	leftClock();
	leftClock();
}
void upperCounter()
{
	upperClock();
	upperClock();
	upperClock();
}
void frontCounter()
{
	frontClock();
	frontClock();
	frontClock();
}
void backCounter()
{
	backClock();
	backClock();
	backClock();
}
void downCounter()
{
	downClock();
	downClock();
	downClock();
}

//This function turns the entire cube to the right.
void turnCubeRight()
{
	frontCounter();
	backClock();
	//------------Get sides------------
	for(int index = 0; index < 3; index++)
		sideOne[index] = cube[4][index];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[4][index];
	for(int index = 6; index < 9; index++)
		sideThree[index - 6] = cube[4][index];
	for(int index = 9; index < 12; index++)
		sideFour[index - 9] = cube[4][index];
	//----------Set Sides----------
	for(int index = 0; index < 3; index++)
		cube[4][index] = sideFour[index];
	for(int index = 3; index < 6; index++)
		cube[4][index] = sideOne[index - 3];
	for(int index = 6; index < 9; index++)
		cube[4][index] = sideTwo[index - 6];
	for(int index = 9; index < 12; index++)
		cube[4][index] = sideThree[index - 9];
}

//turns the entire cube to the front
void turnCubeFront()
{
	leftClock();
	rightCounter();
	//------------Get sides------------
	for(int index = 0; index < 3; index++)
		sideOne[index] = cube[index][4];
	for(int index = 3; index < 6; index++)
		sideTwo[index - 3] = cube[index][4];
	for(int index = 6; index < 9; index++)
		sideThree[index - 6] = cube[index][4];
	for(int index = 3; index < 6; index++)
		sideFour[index - 3] = cube[index][10];
	//----------Set Sides----------
	for(int index = 0; index < 3; index++)
		cube[index][4] = sideFour[2 - index];
	for(int index = 3; index < 6; index++)
		cube[index][4] = sideOne[index - 3];
	for(int index = 6; index < 9; index++)
		cube[index][4] = sideTwo[index - 6];
	for(int index = 3; index < 6; index++)
		cube[index][10] = sideThree[5 - index];
}

//Do 200 random moves
void random()
{
	srand((unsigned short)time(0));
	for(int index = 0; index < 200; index++)
	{
		switch(rand()%12)
		{
		case 0: upperClock();    break;
		case 1: leftClock();     break;
		case 2: frontClock();    break;
		case 3: rightClock();    break;
		case 4: backClock();     break;
		case 5: downClock();     break;
		case 6: upperCounter();  break;
		case 7: leftCounter();   break;
		case 8: frontCounter();  break;
		case 9: rightCounter();  break;
		case 10: backCounter();  break;
		case 11: downCounter();  break;
		}
	}
}

//Pick a color for output
void colorOfText(unsigned short color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon,color);
}

//Select an appropriate color for letters in the array
void determineColor(string square)
{
	char thisSquare = square[0];
	enum Color{DARK_BLUE=1, GREEN, GREY, DARK_RED, DEEP_PURPLE, BROWN, LIGHT_GREY, DARK_GREY, BLUE, LIME_GREEN, TEAL, RED, PURPLE, YELLOW, WHITE, B_B};
	switch(thisSquare)
	{
	case 'x':
		colorOfText(DEEP_PURPLE);
		break;
	case 'g':
		colorOfText(LIME_GREEN);
		break;
	case 'r':
		colorOfText(RED);
		break;
	case 'b':
		colorOfText(BLUE);
		break;
	case 'y':
		colorOfText(YELLOW);
		break;
	case 'w':
		colorOfText(WHITE);
		break;
	case 'o':
		colorOfText(BROWN);
		break;
	}
}

//Output the cube to the console
void printCube()
{
	enum Color{DARK_BLUE=1, GREEN, GREY, DARK_RED, DEEP_PURPLE, BROWN, LIGHT_GREY, DARK_GREY, BLUE, LIME_GREEN, TEAL, RED, PURPLE, YELLOW, WHITE, B_B};

	//	system("cls"); //Clear Console

	colorOfText(YELLOW);
	cout << "      Rubik's Cube" << endl;

	string cubeCurrent = "x";
	for (int row = 0; row < 9; row++)
	{
		for(int column = 0; column < 12; column++)
		{
			cubeCurrent = cube[row][column];
			determineColor(cubeCurrent);
			cout << cubeCurrent << " ";
		}
		cout << endl;
	}
	cout << endl;
	colorOfText(LIGHT_GREY);
}

void findAndEliminateCornerCoordinates(int index)
{
	if( (yCoor[index] == 0 && xCoor[index] == 3) ||	(yCoor[index] == 0 && xCoor[index] == 5) ||
		(yCoor[index] == 2 && xCoor[index] == 3) ||	(yCoor[index] == 2 && xCoor[index] == 5) ||
		(yCoor[index] == 3 && xCoor[index] == 0) ||	(yCoor[index] == 3 && xCoor[index] == 2) ||
		(yCoor[index] == 3 && xCoor[index] == 3) ||	(yCoor[index] == 3 && xCoor[index] == 5) ||
		(yCoor[index] == 3 && xCoor[index] == 6) ||	(yCoor[index] == 3 && xCoor[index] == 8) ||
		(yCoor[index] == 3 && xCoor[index] == 9) ||	(yCoor[index] == 3 && xCoor[index] == 11) || 
		(yCoor[index] == 5 && xCoor[index] == 0) ||	(yCoor[index] == 5 && xCoor[index] == 2) ||
		(yCoor[index] == 5 && xCoor[index] == 3) ||	(yCoor[index] == 5 && xCoor[index] == 5) ||
		(yCoor[index] == 5 && xCoor[index] == 6) ||	(yCoor[index] == 5 && xCoor[index] == 8) ||
		(yCoor[index] == 5 && xCoor[index] == 9) ||	(yCoor[index] == 5 && xCoor[index] == 11) ||
		(yCoor[index] == 6 && xCoor[index] == 3) ||	(yCoor[index] == 6 && xCoor[index] == 5) ||
		(yCoor[index] == 8 && xCoor[index] == 3) ||	(yCoor[index] == 8 && xCoor[index] == 5) ||
		(yCoor[index] == 1 && xCoor[index] == 4) ||	(yCoor[index] == 4 && xCoor[index] == 1) ||
		(yCoor[index] == 4 && xCoor[index] == 4) ||	(yCoor[index] == 4 && xCoor[index] == 7) ||
		(yCoor[index] == 4 && xCoor[index] == 10) ||(yCoor[index] == 7 && xCoor[index] == 4) )
	{
		xCoor[index] = 15;
		yCoor[index] = 15;
	}
}

void findAndEliminateMiddleCoordinates(int index)
{
	if( (yCoor[index] == 0 && xCoor[index] == 4) ||	(yCoor[index] == 1 && xCoor[index] == 3) ||
		(yCoor[index] == 1 && xCoor[index] == 5) ||	(yCoor[index] == 2 && xCoor[index] == 4) ||
		(yCoor[index] == 3 && xCoor[index] == 1) ||	(yCoor[index] == 3 && xCoor[index] == 4) ||
		(yCoor[index] == 3 && xCoor[index] == 7) ||	(yCoor[index] == 3 && xCoor[index] == 10) ||
		(yCoor[index] == 4 && xCoor[index] == 0) ||	(yCoor[index] == 4 && xCoor[index] == 2) ||
		(yCoor[index] == 4 && xCoor[index] == 3) ||	(yCoor[index] == 4 && xCoor[index] == 5) ||
		(yCoor[index] == 4 && xCoor[index] == 6) ||	(yCoor[index] == 4 && xCoor[index] == 8) ||
		(yCoor[index] == 4 && xCoor[index] == 9) ||	(yCoor[index] == 4 && xCoor[index] == 11) ||
		(yCoor[index] == 5 && xCoor[index] == 1) ||	(yCoor[index] == 5 && xCoor[index] == 4) ||
		(yCoor[index] == 5 && xCoor[index] == 7) ||	(yCoor[index] == 5 && xCoor[index] == 10) ||
		(yCoor[index] == 6 && xCoor[index] == 4) ||	(yCoor[index] == 7 && xCoor[index] == 3) ||
		(yCoor[index] == 7 && xCoor[index] == 5) ||	(yCoor[index] == 8 && xCoor[index] == 4) ||
		(yCoor[index] == 1 && xCoor[index] == 4) ||	(yCoor[index] == 4 && xCoor[index] == 1) ||
		(yCoor[index] == 4 && xCoor[index] == 4) ||	(yCoor[index] == 4 && xCoor[index] == 7) ||
		(yCoor[index] == 4 && xCoor[index] == 10) ||(yCoor[index] == 7 && xCoor[index] == 4) )
	{
		xCoor[index] = 15;
		yCoor[index] = 15;
	}
}

//This function will find the coordinates of a cell that is specified by the parameters
void findCells(char color, string whatLookingFor)
{
	//Set the coordinates of each cell at 20,20, which is a bad state if left.
	for(coorIndex = 0; coorIndex < 9; coorIndex++)
	{
		xCoor[coorIndex] = 20;
		yCoor[coorIndex] = 20;
	}

	//Find the coordinates of all 9 colors on the cube
	coorIndex = 0;
	for(int row = 0; row < 9; row++)
	{
		for(int column = 0; column < 12; column++)
		{
			if(cube[row][column] == color)
			{
				xCoor[coorIndex] = column;
				yCoor[coorIndex] = row;
				coorIndex++;
			}
		}
	}
	if(whatLookingFor.compare("middle") == 0)
	{
		for(int index = 0; index < 9; index++)
		{
			findAndEliminateCornerCoordinates(index);
		}	
	}
	if(whatLookingFor.compare("corner") == 0)
	{
		for(int index = 0; index < 9; index++)
		{
			findAndEliminateMiddleCoordinates(index);
		}	
	}
}

//Function to flip the cube around as needed
void flipCube(int toBeUpFace)
{
	switch(toBeUpFace)
	{
	case 0:
		break;
	case 1:
		turnCubeFront();
		turnCubeFront();
		turnCubeFront();
		break;
	case 2:
		turnCubeRight();		
		turnCubeRight();		
		turnCubeRight();		
		break;
	case 3:
		turnCubeFront();
		break;
	case 4:
		turnCubeRight();		
		break;
	case 5:
		turnCubeRight();		
		turnCubeRight();		
		break;
	}
}

//Rotate the faces according to the cube solution
void rotateFaces(string faces)
{
	char faceturn;
	for (int index = 0; index < (int)faces.length(); index++)
	{
		faceturn=faces[index];
		//If the program is not valid or has not gone above solution size, then add it to the end of the solution.
		if(solutionIndex < SOLUTION_SIZE && validation != true)
		{
			solution[solutionIndex] = faceturn;
			solutionIndex++;
		}

		//do the appropriate action to the cube
		switch (faceturn)
		{
		case 'U': upperClock();    break;
		case 'L': leftClock();     break;
		case 'F': frontClock();    break;
		case 'R': rightClock();    break;
		case 'B': backClock();     break;
		case 'D': downClock();     break;
		case 'u': upperCounter();  break;
		case 'l': leftCounter();   break;
		case 'f': frontCounter();  break;
		case 'r': rightCounter();  break;
		case 'b': backCounter();   break;
		case 'd': downCounter();   break;
		case 'w': printCube();     break;
		case 'x':                  break;
		case '0': flipCube(0);     break;
		case '1': flipCube(1);     break;
		case '2': flipCube(2);     break;
		case '3': flipCube(3);     break;
		case '4': flipCube(4);     break;
		case '5': flipCube(5);     break;
		}
	}
}

//Function to check over all of the cube for all colors
//This function allows a user to enter 1-3 layers to check over for correctness.
bool checkCube(int layers)
{
	bool topFace = false;
	bool side1 = false;
	bool side2 = false;
	bool side3 = false;
	bool side4 = false;
	bool bottomFace = false;
	char faceColor = cube[4][4];
	if (cube[3][3] == faceColor && cube[3][4] == faceColor && cube[3][5] == faceColor && cube[4][3] == faceColor &&
		cube[4][5] == faceColor && cube[5][3] == faceColor && cube[5][4] == faceColor && cube[5][5] == faceColor)
		topFace = true;

	faceColor = cube[1][4]; //-Top------------Change Sides-------------- 
	if (cube[2][3] == faceColor && cube[2][4] == faceColor && cube[2][5] == faceColor && layers >= 1)
		side1 = true;
	else if(layers >= 1)
		side1 = false;
	if (cube[1][3] == faceColor && cube[1][5] == faceColor && layers >= 2 && side1 == true)
		side1 = true;
	else if(layers >= 2)
		side1 = false;
	if (cube[0][4] == faceColor && cube[0][3] == faceColor && cube[0][5] == faceColor && layers >= 3 && side1 == true)
		side1 = true;
	else if(layers >= 3)
		side1 = false;

	faceColor = cube[4][1]; //-Left-----------Change Sides--------------
	if (cube[3][2] == faceColor && cube[4][2] == faceColor && cube[5][2] == faceColor && layers >= 1)
		side2 = true;
	else if(layers >= 1)
		side1 = false;
	if (cube[3][1] == faceColor && cube[5][1] == faceColor && layers >= 2 && side2 == true)
		side2 = true;
	else if (layers >= 2)
		side2 = false;
	if (cube[4][0] == faceColor && cube[3][0] == faceColor && cube[5][0] == faceColor && layers >= 3 && side2 == true)
		side2 = true;
	else if(layers >= 3)
		side2 = false;

	faceColor = cube[7][4]; //-Bottom---------Change Sides--------------
	if (cube[6][3] == faceColor && cube[6][4] == faceColor && cube[6][5] == faceColor && layers >= 1)
		side3 = true;
	else if(layers >= 1)
		side1 = false;
	if(cube[7][3] == faceColor && cube[7][5] == faceColor && layers >= 2 && side3 == true)
		side3 = true;
	else if (layers >= 2)
		side3 = false;
	if(cube[8][4] == faceColor && cube[8][3] == faceColor && cube[8][5] == faceColor && layers >= 3 && side3 == true)
		side3 = true;
	else if(layers >= 3)
		side3 = false;

	faceColor = cube[4][7]; //-Right----------Change Sides--------------
	if (cube[3][6] == faceColor && cube[4][6] == faceColor && cube[5][6] == faceColor && layers >= 1)
		side4 = true;
	else if(layers >= 1)
		side1 = false;
	if (cube[3][7] == faceColor && cube[5][7] == faceColor && layers >= 2 && side4 == true)
		side4 = true;
	else if (layers >= 2)
		side4 = false;
	if (cube[4][8] == faceColor && cube[3][8] == faceColor && cube[5][8] == faceColor && layers >= 3 && side4 == true)
		side4 = true;
	else if(layers >= 3)
		side4 = false;

	faceColor = cube[4][10]; //----------------Change Sides--------------
	if (layers >= 3 &&		 //Only check the 3rd layer if it is actually specified to check
		cube[3][9] == faceColor && cube[3][10] == faceColor && cube[3][11] == faceColor && 
		cube[4][9] == faceColor && cube[4][10] == faceColor && cube[4][11] == faceColor && 
		cube[5][9] == faceColor && cube[5][10] == faceColor && cube[5][11] == faceColor)
		bottomFace = true;
	else if (layers >= 3) //Looking at the 3rd layer, but the face colors are not correct
		bottomFace = false;
	else if (layers <= 2) //Default state if checking 1 or 2 layers
		bottomFace = true;

	if(topFace == true && side1 == true && side2 == true && side3 == true && side4 == true && bottomFace == true)
		return true;
	else
		return false;
}

//function to solve the top side middles
void solveTopMiddle()
{
	solveCounter = 0;
	char topOrientation[4] = {'h', 'h', 'h', 'h'}; //What middles should be on what side from the top layer
	char correspMiddles[4] = {'h', 'h', 'h', 'h'}; //What middles actually are on the top layer
	char thisXCoor[4] = {'h', 'h', 'h', 'h'}; //X-Coordinates of the 4 needed pieces
	char thisYCoor[4] = {'h', 'h', 'h', 'h'}; //Y-coordinates of the 4 needed pieces
	int focus = 0;
	int middlesCounter = 0;
	bool skip = false;
	switch(topColor)
	{
	case 'b': topOrientation[0] = 'y'; topOrientation[1] = 'r'; topOrientation[2] = 'w'; topOrientation[3] = 'o'; break;
	case 'w': topOrientation[0] = 'b'; topOrientation[1] = 'r'; topOrientation[2] = 'g'; topOrientation[3] = 'o'; break;
	case 'r': topOrientation[0] = 'y'; topOrientation[1] = 'g'; topOrientation[2] = 'w'; topOrientation[3] = 'b'; break;
	case 'o': topOrientation[0] = 'y'; topOrientation[1] = 'b'; topOrientation[2] = 'w'; topOrientation[3] = 'g'; break;
	case 'y': topOrientation[0] = 'g'; topOrientation[1] = 'r'; topOrientation[2] = 'b'; topOrientation[3] = 'o'; break;
	case 'g': topOrientation[0] = 'w'; topOrientation[1] = 'r'; topOrientation[2] = 'y'; topOrientation[3] = 'o'; break;
	}
	while(solveCounter!= 4)
	{
		findCells(topColor, "middle");
		middlesCounter = 0;
		for(int counter = 0; counter < 9; counter++)
		{
			if(xCoor[counter] != 15)
			{
				thisXCoor[middlesCounter] = xCoor[counter];
				thisYCoor[middlesCounter] = yCoor[counter];
				for(int index = 0; index < 12; index++)
				{
					if(yCoor[counter] == lookUpTableMiddles[index][0] && xCoor[counter] == lookUpTableMiddles[index][1])
					{
						correspMiddles[middlesCounter] = cube[ lookUpTableMiddles[index][2] ][ lookUpTableMiddles[index][3] ];
						middlesCounter++;
						index = 12;
					}
					else if(yCoor[counter] == lookUpTableMiddles[index][2] && xCoor[counter] == lookUpTableMiddles[index][3])
					{
						correspMiddles[middlesCounter] = cube[ lookUpTableMiddles[index][0] ][ lookUpTableMiddles[index][1] ];
						middlesCounter++;
						index = 12;
					}
				} //end for
			} //end if xCoor != 15
		}//end for
		for(int index = 0; index < 4; index++)
		{
			if(correspMiddles[index] == topOrientation[solveCounter])
				focus = index;
		}
		switch(thisYCoor[focus])
		{
		case 0:
			if(thisXCoor[focus] == 4)
				rotateFaces("BUru");
			break;
		case 1:
			switch(thisXCoor[focus])
			{
			case 3:
				rotateFaces("uL");
				skip = true;
				break;
			case 5:
				rotateFaces("Uru");
				break;
			}
			break;
		case 2:
			if(thisXCoor[focus] == 4)
				rotateFaces("bUru");
			break;
		case 3:
			switch(thisXCoor[focus])
			{
			case 1:
				rotateFaces("b");
				break;
			case 4:
				break;
			case 7:
				rotateFaces("B");
				break;
			case 10:
				rotateFaces("BB");
				break;
			}
			break;
		case 4:
			switch(thisXCoor[focus])
			{
			case 0:
				rotateFaces("uLUb");
				break;
			case 2:
				rotateFaces("lb");
				break;
			case 3:
				skip = true;
				break;
			case 5:
				rotateFaces("rURu");
				break;
			case 6:
				rotateFaces("RB");
				break;
			case 8:
				rotateFaces("UruB");
				break;
			case 9:
				rotateFaces("URRu");
				break;
			case 11:
				rotateFaces("uLL");
				skip = true;
				break;
			}
			break;
		case 5:
			switch(thisXCoor[focus])
			{
			case 1:
				rotateFaces("UUFU");
				skip = true;
				break;
			case 4:
				rotateFaces("FUUfU");
				skip = true;
				break;
			case 7:
				rotateFaces("UUfU");
				skip = true;
				break;
			case 10:
				rotateFaces("UUFFU");
				skip = true;
				break;
			}
			break;
		case 6:
			if(thisXCoor[focus] == 4)
				rotateFaces("FURu");
			break;
		case 7:
			switch(thisXCoor[focus])
			{
			case 3:
				rotateFaces("ul");
				skip = true;
				break;
			case 5:
				rotateFaces("URu");
				break;
			}
			break;
		case 8:
			if(thisXCoor[focus] == 4)
			{
				rotateFaces("UUFUl");
				skip = true;
			}
			break;
		} //end switch
		if(skip == false)
			rotateFaces("u");
		solveCounter++;
		skip = false;
	}
}

//Solve the corners on the top. This function assumes that all of the top middles are correct
void solveTopCorner()
{
	solveCounter = 0;
	char oneColor[4] = {'h', 'h', 'h', 'h'};
	char twoColor[4] = {'h', 'h', 'h', 'h'};
	int thisXCoor[4] = {15, 15, 15, 15};
	int thisYCoor[4] = {15, 15, 15, 15};
	int focus, colorCounter;
	bool skip = false;
	while(solveCounter != 4)
	{
		findCells(topColor, "corner");
		colorCounter = 0;
		focus = 15;
		for(int counter = 0; counter < 9; counter++)
		{
			if(xCoor[counter] != 15)
			{
				thisXCoor[colorCounter] = xCoor[counter];
				thisYCoor[colorCounter] = yCoor[counter];
				for(int index = 0; index < 8; index++)
				{
					if(lookUpTableCorners[index][0] == yCoor[counter] && lookUpTableCorners[index][1] == xCoor[counter])
					{
						oneColor[colorCounter] = cube[lookUpTableCorners[index][2]][lookUpTableCorners[index][3]];
						twoColor[colorCounter] = cube[lookUpTableCorners[index][4]][lookUpTableCorners[index][5]];
						colorCounter++;
						index = 15;
					}
					else if(lookUpTableCorners[index][2] == yCoor[counter] && lookUpTableCorners[index][3] == xCoor[counter])
					{
						oneColor[colorCounter] = cube[lookUpTableCorners[index][0]][lookUpTableCorners[index][1]];
						twoColor[colorCounter] = cube[lookUpTableCorners[index][4]][lookUpTableCorners[index][5]];
						colorCounter++;
						index = 15;
					}
					else if(lookUpTableCorners[index][4] == yCoor[counter] && lookUpTableCorners[index][5] == xCoor[counter])
					{
						oneColor[colorCounter] = cube[lookUpTableCorners[index][0]][lookUpTableCorners[index][1]];
						twoColor[colorCounter] = cube[lookUpTableCorners[index][2]][lookUpTableCorners[index][3]];
						colorCounter++;
						index = 15;
					}
				} //end for(i(8))
			}// end if xCoor != 15
		}//end for(counter(9))
		for(int index = 0; index < 4; index++)
		{
			if((oneColor[index] == cube[2][4] && twoColor[index] == cube[4][2]) || (oneColor[index] == cube[4][2] && twoColor[index] == cube[2][4]))
				focus = index;
		}
		if(focus < 4)
		{
			switch(thisYCoor[focus])
			{
			case 0:
				switch(thisXCoor[focus])
				{
				case 3:
					rotateFaces("dlDL");
					break;
				case 5:
					rotateFaces("DDBdb");
					break;
				}
				break;
			case 2:
				switch(thisXCoor[focus])
				{
				case 3:
					rotateFaces("BDDblDDL");
					break;
				case 5:
					rotateFaces("bdBBddb");
					break;
				}
				break;
			case 3:
				switch(thisXCoor[focus])
				{
				case 0:
					rotateFaces("DBdb");
					break;
				case 2:
					rotateFaces("ldLdBddb");
					break;
				case 3:
					break;
				case 5:
					rotateFaces("bdBlDDL");
					break;
				case 6:
					rotateFaces("RDrdlDL");
					break;
				case 8:
					rotateFaces("lDL");
					break;
				case 9:
					rotateFaces("DBdblddL");
					break;
				case 11:
					rotateFaces("lDLBDDb");
					break;
				}
				break;
			case 5:
				switch(thisXCoor[focus])
				{
				case 0:
					rotateFaces("DlddL");
					break;
				case 2:
					rotateFaces("LDllDDL");
					break;
				case 3:
					rotateFaces("fDDFlDL");
					break;
				case 5:
					rotateFaces("rdRDlddL");
					break;
				case 6:
					rotateFaces("rdRBdb");
					break;
				case 8:
					rotateFaces("Bddb");
					break;
				case 9:
					rotateFaces("DDlDLBddb");
					break;
				case 11:
					rotateFaces("dlDLBddb");
					break;
				}
				break;
			case 6:
				switch(thisXCoor[focus])
				{
				case 3:
					rotateFaces("fBdbF");
					break;
				case 5:
					rotateFaces("FDflDL");
					break;
				}
				break;
			case 8:
				switch(thisXCoor[focus])
				{
				case 3:
					rotateFaces("Bdb");
					break;
				case 5:
					rotateFaces("lDDL");
					break;
				}
				break;
			}
			if(solveCounter != 3)
				rotateFaces("u");
			solveCounter++;
		}
	}
}

//Function to solve the middle layer. This function assumes that the top layer is solved correctopLefty.
void solveMiddleLayer()
{
	solveCounter = 0;
	int rotateCounter = 0;
	string left = "k";
	string right = "h";
	bool topLeft = false;
	bool topRight = false;
	bool bottomRight = false;
	bool bottomLeft = false;
	bool topLeftLoop = false;
	bool trLoop = false;
	bool brLoop = false;
	bool blLoop = false;
	if(cube[4][1] == cube[3][1] && cube[1][4] == cube[1][3]) //top left middle
	{
		solveCounter++;
		topLeft = true;
	}
	if(cube[4][7] == cube[3][7] && cube[1][4] == cube[1][5]) //top right middle
	{
		solveCounter++;
		topRight = true;
	}
	if(cube[4][7] == cube[5][7] && cube[7][4] == cube[7][5]) //bottom right middle
	{
		solveCounter++;
		bottomRight = true;
	}
	if(cube[4][1] == cube[5][1] && cube[7][4] == cube[7][3]) //bottom left middle
	{
		solveCounter++;
		bottomLeft = true;
	}
	while(solveCounter != 4)
	{
		rotateCounter = 0;
		if((cube[4][1] != cube[3][1] || cube[1][4] != cube[1][3]) && (topLeft == false) && (topLeftLoop == false)) //top left middle
		{
			while(rotateCounter < 4)
			{
				if(cube[4][0] == cube[1][4] && cube[4][11] == cube[4][1])
					break;
				if(cube[4][0] == cube[4][1] && cube[4][11] == cube[1][4])
					break;
				rotateFaces("D");
				rotateCounter++;
			}
			if(cube[4][0] == cube[1][4] && cube[4][11] == cube[4][1])
			{
				rotateFaces("ddlDLDBdb");
				solveCounter++;
				topLeft = true;
				trLoop = false;
				brLoop = false;
				blLoop = false;
			}
			else if(cube[4][0] == cube[4][1] && cube[4][11] == cube[1][4])
			{
				rotateFaces("DBdbdlDL");
				solveCounter++;
				topLeft = true;
				trLoop = false;
				brLoop = false;
				blLoop = false;
			}
			else
				topLeftLoop = true;
		}

		else if((cube[4][7] != cube[3][7] || cube[1][4] != cube[1][5]) && (topRight == false) && (trLoop == false)) //top right middle
		{
			while(rotateCounter < 4)
			{
				if(cube[0][4] == cube[4][7] && cube[3][10] == cube[1][4])
					break;
				if(cube[0][4] == cube[1][4] && cube[3][10] == cube[4][7])
					break;
				rotateFaces("D");
				rotateCounter++;
			}
			if(cube[0][4] == cube[4][7] && cube[3][10] == cube[1][4])
			{
				rotateFaces("ddbDBDRdr");
				solveCounter++;
				topRight = true;
				topLeftLoop = false;
				brLoop = false;
				blLoop = false;
			}
			else if(cube[0][4] == cube[1][4] && cube[3][10] == cube[4][7])
			{
				rotateFaces("DRdrdbDB");
				solveCounter++;
				topRight = true;
				topLeftLoop = false;
				brLoop = false;
				blLoop = false;
			}
			else
				trLoop = true;
		}
		else if((cube[4][7] != cube[5][7] || cube[7][4] != cube[7][5]) && (bottomRight == false) && (brLoop == false)) //bottom right middle
		{
			while(rotateCounter < 4)
			{
				if(cube[4][8] == cube[7][4] && cube[4][9] == cube[4][7])
					break;
				if(cube[4][8] == cube[4][7] && cube[4][9] == cube[7][4])
					break;
				rotateFaces("D");
				rotateCounter++;
			}
			if(cube[4][8] == cube[7][4] && cube[4][9] == cube[4][7])
			{
				rotateFaces("ddrDRDFdf");
				solveCounter++;
				bottomRight = true;
				topLeftLoop = false;
				trLoop = false;
				blLoop = false;
			}
			else if(cube[4][8] == cube[4][7] && cube[4][9] == cube[7][4])
			{
				rotateFaces("DFdfdrDR");
				solveCounter++;
				bottomRight = true;
				topLeftLoop = false;
				trLoop = false;
				blLoop = false;
			}
			else
				brLoop = true;
		}

		else if((cube[4][1] != cube[5][1] || cube[7][4] != cube[7][3]) && (bottomLeft == false) && (blLoop == false))//bottom left middle
		{
			while(rotateCounter < 4)
			{
				if(cube[8][4] == cube[4][1] && cube[5][10] == cube[7][4])
					break;
				if(cube[8][4] == cube[7][4] && cube[5][10] == cube[4][1])
					break;
				rotateFaces("D");
				rotateCounter++;
			}
			if(cube[8][4] == cube[4][1] && cube[5][10] == cube[7][4])
			{
				rotateFaces("ddfDFDLdl");
				solveCounter++;
				bottomLeft = true;
				topLeftLoop = false;
				trLoop = false;
				brLoop = false;
			}
			else if(cube[8][4] == cube[7][4] && cube[5][10] == cube[4][1])
			{
				rotateFaces("DLdldfDF");
				solveCounter++;
				bottomLeft = true;
				topLeftLoop = false;
				trLoop = false;
				brLoop = false;
			}
			else
				blLoop = true;
		}
		else if(cube[4][1] == cube[3][1] && cube[1][4] == cube[1][3] &&
			cube[4][7] == cube[3][7] && cube[1][4] == cube[1][5] &&
			cube[4][7] == cube[5][7] && cube[7][4] == cube[7][5] &&
			cube[4][1] == cube[5][1] && cube[7][4] == cube[7][3])
		{
			solveCounter = 4;
			break;
		}
		else
		{
			if(topLeft != true)
				rotateFaces("lDLDBdb");
			else if(topRight != true)
				rotateFaces("bDBDRdr");
			else if(bottomRight != true)
				rotateFaces("rDRDFdf");
			else if(bottomLeft != true)
				rotateFaces("fDFDLdl");
			topLeftLoop = false;
			trLoop = false;
			brLoop = false;
			blLoop = false;
		}
	}
}

//This function orients the middles on the bottom. This assumes that the top 2 layers are correct.
void orientBottomMiddle()
{
	int solveCounter = 0;
	bool left = false, right = false, top = false, bottom = false, adjacent = false, across = false;
	char bottomColor = cube[4][4];
	if(cube[3][4] == bottomColor)
	{
		top = true;
		solveCounter++;
	}
	if(cube[4][3] == bottomColor)
	{
		left = true;
		solveCounter++;
	}
	if(cube[4][5] == bottomColor)
	{
		right = true;
		solveCounter++;
	}
	if(cube[5][4] == bottomColor)
	{
		bottom = true;
		solveCounter++;
	}
	if(solveCounter == 0)
	{
		rotateFaces("FRUruf");
		solveCounter = 2;
		if(cube[3][4] == bottomColor)
			top = true;
		if(cube[4][3] == bottomColor)
			left = true;
		if(cube[4][5] == bottomColor)
			right = true;
		if(cube[5][4] == bottomColor)
			bottom = true;
	}
	if(solveCounter == 2)
	{
		if((left == true && (top == true || bottom == true)) || 
			(right == true && (top == true || bottom == true)))
			adjacent = true;
		else
			across = true;
		if(adjacent) //Adjacent
		{
			if(left) //left
			{
				if(top) //left-top
					rotateFaces("FRUruRUruf");
				else //left-bottom
					rotateFaces("RBUbuBUbur");
			}
			else //right
			{
				if(top) //right-top
					rotateFaces("LFUfuFUful");
				else //right-bottom
					rotateFaces("BLUluLUlub");
			}
		}
		if(across) //across
		{
			if(left)
				rotateFaces("FRUruf");
			else
				rotateFaces("LFUful");
		}
	}
}

//This function solves the middle bottoms. 
void solveBottomMiddle()
{
	int solveCounter = 0, rotateCounter = 0;
	bool left = false, right = false, top = false, bottom = false, adjacent = false, across = false;
	if(cube[1][4] == cube[2][4])
	{
		top = true;
		solveCounter++;
	}
	if(cube[4][1] == cube[4][2])
	{
		left = true;
		solveCounter++;
	}
	if(cube[4][7] == cube[4][6])
	{
		right = true;
		solveCounter++;
	}
	if(cube[7][4] == cube[6][4])
	{
		bottom = true;
		solveCounter++;
	}
	while(solveCounter < 2 || rotateCounter >= 4)
	{
		rotateFaces("U");
		solveCounter = 0;
		top = false;
		left = false;
		right = false;
		bottom = false;
		if(cube[1][4] == cube[2][4])
		{
			top = true;
			solveCounter++;
		}
		if(cube[4][1] == cube[4][2])
		{
			left = true;
			solveCounter++;
		}
		if(cube[4][7] == cube[4][6])
		{
			right = true;
			solveCounter++;
		}
		if(cube[7][4] == cube[6][4])
		{
			bottom = true;
			solveCounter++;
		}
		rotateCounter++;
	}
	if(((left == true && right == true) || (top == true && bottom == true)) && solveCounter == 0) //sides across from each other
	{
		rotateFaces("RUrURUUr");
		solveCounter = 2;
		if(cube[1][4] == cube[2][4])
			top = true;
		if(cube[4][1] == cube[4][2])
			left = true;
		if(cube[4][7] == cube[4][6])
			right = true;
		if(cube[7][4] == cube[6][4])
			bottom = true;
	}
	if(solveCounter == 2)
	{
		if( (left == true && (top == true || bottom == true)) || 
			(right == true && (top == true || bottom == true)))
			adjacent = true;
		else
			across = true;
		if(across) //across
		{
			rotateFaces("RUrURUUr");
			adjacent = true;
			top = false;
			left = false;
			right = false;
			bottom = false;
			if(cube[1][4] == cube[2][4])
				top = true;
			if(cube[4][1] == cube[4][2])
				left = true;
			if(cube[4][7] == cube[4][6])
				right = true;
			if(cube[7][4] == cube[6][4])
				bottom = true;
		}
		if(adjacent) //Adjacent
		{
			if(left) //left
			{
				if(top) //left-top
					rotateFaces("BUbUBUUb");
				else //left-bottom
					rotateFaces("LUlULUUl");
			}
			else //right
			{
				if(top) //right-top
					rotateFaces("RUrURUUr");
				else //right-bottom
					rotateFaces("FUfUFUUf");
			}
		}
	}
	while(cube[1][4] != cube[2][4])
	{
		rotateFaces("U");
	}
}

//This function puts all of the corners in the correct spot. It assumes all of the previous functions worked correctopLefty.
void positionBottomCorners()
{
	char side1[] = {cube[2][3], cube[2][5], cube[6][5], cube[6][3]};
	char side2[] = {cube[3][2], cube[3][6], cube[5][6], cube[5][2]};
	char side3[] = {cube[3][3], cube[3][5], cube[5][5], cube[5][3]};
	char pattern[] = {cube[4][1], cube[1][4], cube[4][7], cube[7][4], cube[4][1]};
	char bottomColor = cube[4][4], oneSide = pattern[0], twoSide = pattern[1];
	bool topLeft = false, topRight = false, bottomRight = false, bottomLeft = false;
	solveCounter = 0;
	if( (side1[0] == oneSide || side1[0] == twoSide || side1[0] == bottomColor) &&
		(side2[0] == twoSide || side2[0] == bottomColor || side2[0] == oneSide) &&
		(side3[0] == bottomColor || side3[0] == oneSide || side3[0] == twoSide) )
	{
		topLeft = true;
		solveCounter++;
	}
	oneSide = pattern[1];
	twoSide = pattern[2];
	if( (side1[1] == oneSide || side1[1] == twoSide || side1[1] == bottomColor) &&
		(side2[1] == twoSide || side2[1] == bottomColor || side2[1] == oneSide) &&
		(side3[1] == bottomColor || side3[1] == oneSide || side3[1] == twoSide) )
	{
		topRight = true;
		solveCounter++;
	}
	oneSide = pattern[2];
	twoSide = pattern[3];
	if( (side1[2] == oneSide || side1[2] == twoSide || side1[2] == bottomColor) &&
		(side2[2] == twoSide || side2[2] == bottomColor || side2[2] == oneSide) &&
		(side3[2] == bottomColor || side3[2] == oneSide || side3[2] == twoSide) )
	{
		bottomRight = true;
		solveCounter++;
	}
	oneSide = pattern[3];
	twoSide = pattern[4];
	if( (side1[3] == oneSide || side1[3] == twoSide || side1[3] == bottomColor) &&
		(side2[3] == twoSide || side2[3] == bottomColor || side2[3] == oneSide) &&
		(side3[3] == bottomColor || side3[3] == oneSide || side3[3] == twoSide) )
	{
		bottomLeft = true;
		solveCounter++;
	}
	if(solveCounter == 0)
	{
		rotateFaces("URulUruL");
		topLeft = false;
		topRight = false;
		bottomRight = false;
		bottomLeft = false;
		side1[0] = cube[2][3]; //update sides because of shifting faces, this is out of date then
		side1[1] = cube[2][5];
		side1[2] = cube[6][5];
		side1[3] = cube[6][3];
		side2[0] = cube[3][2];
		side2[1] = cube[3][6];
		side2[2] = cube[5][6];
		side2[3] = cube[5][2];
		side3[0] = cube[3][3];
		side3[1] = cube[3][5];
		side3[2] = cube[5][5];
		side3[3] = cube[5][3];
		oneSide = pattern[0];
		twoSide = pattern[1];
		if( (side1[0] == oneSide || side1[0] == twoSide || side1[0] == bottomColor) &&
			(side2[0] == twoSide || side2[0] == bottomColor || side2[0] == oneSide) &&
			(side3[0] == bottomColor || side3[0] == oneSide || side3[0] == twoSide) )
		{
			topLeft = true;
			solveCounter++;
		}
		oneSide = pattern[1];
		twoSide = pattern[2];
		if( (side1[1] == oneSide || side1[1] == twoSide || side1[1] == bottomColor) &&
			(side2[1] == twoSide || side2[1] == bottomColor || side2[1] == oneSide) &&
			(side3[1] == bottomColor || side3[1] == oneSide || side3[1] == twoSide) )
		{
			topRight = true;
			solveCounter++;
		}
		oneSide = pattern[2];
		twoSide = pattern[3];
		if( (side1[2] == oneSide || side1[2] == twoSide || side1[2] == bottomColor) &&
			(side2[2] == twoSide || side2[2] == bottomColor || side2[2] == oneSide) &&
			(side3[2] == bottomColor || side3[2] == oneSide || side3[2] == twoSide) )
		{
			bottomRight = true;
			solveCounter++;
		}
		oneSide = pattern[3];
		twoSide = pattern[4];
		if( (side1[3] == oneSide || side1[3] == twoSide || side1[3] == bottomColor) &&
			(side2[3] == twoSide || side2[3] == bottomColor || side2[3] == oneSide) &&
			(side3[3] == bottomColor || side3[3] == oneSide || side3[3] == twoSide) )
		{
			bottomLeft = true;
			solveCounter++;
		}
	}
	while(solveCounter != 4)
	{
		if(topLeft)
			rotateFaces("ULurUluR");
		else if(topRight)
			rotateFaces("UBufUbuF");
		else if(bottomRight)
			rotateFaces("URulUruL");
		else if(bottomLeft)
			rotateFaces("UFubUfuB");
		solveCounter = 0;
		topLeft = false;
		topRight = false;
		bottomRight = false;
		bottomLeft = false;
		side1[0] = cube[2][3]; //update sides because of shifting faces, this is out of date then
		side1[1] = cube[2][5];
		side1[2] = cube[6][5];
		side1[3] = cube[6][3];
		side2[0] = cube[3][2];
		side2[1] = cube[3][6];
		side2[2] = cube[5][6];
		side2[3] = cube[5][2];
		side3[0] = cube[3][3];
		side3[1] = cube[3][5];
		side3[2] = cube[5][5];
		side3[3] = cube[5][3];
		oneSide = pattern[0];
		twoSide = pattern[1];
		if( (side1[0] == oneSide || side1[0] == twoSide || side1[0] == bottomColor) &&
			(side2[0] == twoSide || side2[0] == bottomColor || side2[0] == oneSide) &&
			(side3[0] == bottomColor || side3[0] == oneSide || side3[0] == twoSide) )
		{
			topLeft = true;
			solveCounter++;
		}
		oneSide = pattern[1];
		twoSide = pattern[2];
		if( (side1[1] == oneSide || side1[1] == twoSide || side1[1] == bottomColor) &&
			(side2[1] == twoSide || side2[1] == bottomColor || side2[1] == oneSide) &&
			(side3[1] == bottomColor || side3[1] == oneSide || side3[1] == twoSide) )
		{
			topRight = true;
			solveCounter++;
		}
		oneSide = pattern[2];
		twoSide = pattern[3];
		if( (side1[2] == oneSide || side1[2] == twoSide || side1[2] == bottomColor) &&
			(side2[2] == twoSide || side2[2] == bottomColor || side2[2] == oneSide) &&
			(side3[2] == bottomColor || side3[2] == oneSide || side3[2] == twoSide) )
		{
			bottomRight = true;
			solveCounter++;
		}
		oneSide = pattern[3];
		twoSide = pattern[4];
		if( (side1[3] == oneSide || side1[3] == twoSide || side1[3] == bottomColor) &&
			(side2[3] == twoSide || side2[3] == bottomColor || side2[3] == oneSide) &&
			(side3[3] == bottomColor || side3[3] == oneSide || side3[3] == twoSide) )
		{
			bottomLeft = true;
			solveCounter++;
		}
	}
}

//This function solves all of the bottom corners to the correct orientations
void solveBottomCorners()
{
	for(int index = 0; index < 4; index++)
	{
		while(cube[5][5] != cube[4][4])
		{
			rotateFaces("rdRD");
		}
		if(index != 3)
			rotateFaces("u");
	}
	while(cube[6][4] != cube[7][4])
	{
		rotateFaces("u");
	}
	rotateFaces("5");
	while(cube[6][4] != cube[7][4])
	{
		rotateFaces("u");
	}
}

//This function copies the master to the cube
void setCubeFromMaster()
{
	for(int row = 0; row < 9; row++)
		for(int column = 0; column < 12; column++)
			cube[row][column] = masterCube[row][column];
}

//Sets the master from the cube
void setMasterFromCube()
{
	for(int row = 0; row < 9; row++)
		for(int column = 0; column < 12; column++)
			masterCube[row][column] = cube[row][column];
}

//This function updates the shortest solution from the current solution.
void updateShortest()
{
	for(int index = 0; index < SOLUTION_SIZE; index++)
		shortestSolution[index] = "x";
	for(int index = 0; index < SOLUTION_SIZE; index++)
		shortestSolution[index] = solution[index];
}

//This function will take a solution and validate it using the step by step string created as the solution was created.
bool validateSolution()
{
	validation = true;
	solutionIndex = 0;
	setCubeFromMaster();
	rotateFaces(solution[solutionIndex]);
	solutionIndex+=2;
	for(solutionIndex; solutionIndex < SOLUTION_SIZE; solutionIndex++)
		rotateFaces(solution[solutionIndex]);
	validation = false;
	if(checkCube(3))
		return true;
	else
		return false;
}

//This function will push the entire solution back a slot. This is used in conjunction with optimize to get a shorter solution.
void shiftSolutionBack()
{
	string temp;
	for(int index = 0; index < SOLUTION_SIZE-1; index++)
	{
		if(solution[index] == "x")
		{
			temp = solution[index];
			solution[index] = solution[index+1];
			solution[index+1] = temp;
		}
	}
}

//This function optimizes the solution. It looks for useless moves that undoes itself, or looks for three of the same move
//which would be faster as one of the opposite move
void optimize()
{
	int cost = 0;
	int prevCost = 1;
	char tempChar;
	string tempString;
	for(int index = 0; index < SOLUTION_SIZE; index++)
		if(solution[index] != "x")
			cost++;
	while(cost != prevCost) //get rid of the 4 rotations around.  this is does nothing overall, so get rid of it.
	{
		prevCost = cost;
		cost = 0;
		for(int index = 0; index < SOLUTION_SIZE - 3; index++)
		{
			if((solution[index] == solution[index+1]) && (solution[index] == solution[index+2]) && (solution[index] == solution[index+3]))
			{
				solution[index] = "x";
				solution[index+1] = "x";
				solution[index+2] = "x";
				solution[index+3] = "x";
			}
		}
		for(int index = 0; index < SOLUTION_SIZE; index++)
			if(solution[index] != "x")
				cost++;
	}
	shiftSolutionBack();
	prevCost = 1;
	while(cost != prevCost) //change the 3 rotations around one direction to just 1 in the other direction, since it's equivalent.
	{
		prevCost = cost;
		cost = 0;
		for(int index = 0; index < SOLUTION_SIZE - 2; index++)
		{
			if((solution[index] == solution[index+1]) && (solution[index] == solution[index+2]))
			{
				tempString = solution[index];
				tempChar = tempString[0];
				switch(tempChar)
				{
				case 'U': solution[index] = "u";  break;
				case 'L': solution[index] = "l";  break;
				case 'F': solution[index] = "f";  break;
				case 'R': solution[index] = "r";  break;
				case 'B': solution[index] = "b";  break;
				case 'D': solution[index] = "d";  break;
				case 'u': solution[index] = "U";  break;
				case 'l': solution[index] = "L";  break;
				case 'f': solution[index] = "F";  break;
				case 'r': solution[index] = "R";  break;
				case 'b': solution[index] = "B";  break;
				case 'd': solution[index] = "D";  break;
				}
				solution[index+1] = "x";
				solution[index+2] = "x";
			}
		}
		for(int index = 0; index < SOLUTION_SIZE; index++)
			if(solution[index] != "x")
				cost++;
	}
	shiftSolutionBack();
	prevCost = 1;
	while(cost != prevCost) //change the movement in 1 direction and the equivalent movement in the other direction to x's.
	{
		prevCost = cost;
		cost = 0;
		for(int index = 0; index < SOLUTION_SIZE - 1; index++)
		{
			tempString = solution[index];
			tempChar = tempString[0];
			switch(tempChar)
			{
			case 'U': 
				if(solution[index+1] == "u")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'L': 
				if(solution[index+1] == "l")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'F': 
				if(solution[index+1] == "f")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'R': 
				if(solution[index+1] == "r")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'B': 
				if(solution[index+1] == "b")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'D': 
				if(solution[index+1] == "d")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'u': 
				if(solution[index+1] == "U")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'l': 
				if(solution[index+1] == "L")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'f': 
				if(solution[index+1] == "F")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'r': 
				if(solution[index+1] == "R")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'b': 
				if(solution[index+1] == "B")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			case 'd': 
				if(solution[index+1] == "D")
				{
					solution[index] = "x";
					solution[index+1] = "x";
				}
				break;
			}
		}
		for(int index = 0; index < SOLUTION_SIZE; index++)
			if(solution[index] != "x")
				cost++;
	}
	shiftSolutionBack();
	for(int index = 0; index < SOLUTION_SIZE; index++)
	{
		if(solution[index] == "x")
		{
			solutionIndex = index+1;
			index = SOLUTION_SIZE;
		}
	}
}

void solve()
{
	cout << "\nI'm thinking..." << endl;
	setMasterFromCube();
	long long startTime = GetTickCount();
	int shortestMoveCount = 65535;
	string shortestColor = "k";
	for(int index = 0; index < 6; index++)
	{
		setCubeFromMaster();
		for(int j = 0; j < SOLUTION_SIZE; j++)
		{
			solution[j] = "x";
			preoptimization[j] = "x";
		}
		solutionIndex = 0;
		switch(index)
		{
		case 0: rotateFaces("0");  break;
		case 1: rotateFaces("1");  break;
		case 2: rotateFaces("2");  break;
		case 3: rotateFaces("3");  break;
		case 4: rotateFaces("4");  break;
		case 5: rotateFaces("5");  break;
		}
		if(!checkCube(3))
		{
			topColor = cube[4][4];
			solution[1] = topColor;
			solutionIndex++;
			if(!checkCube(1))
			{
				solveTopMiddle();
				solveTopCorner();
			}
			else
				problem = true;
			if(!checkCube(2) && checkCube(1))
			{
				solveMiddleLayer();
			}
			else
				problem = true;
			if(!checkCube(3) && checkCube(2))
			{
				rotateFaces("5");
				orientBottomMiddle();
				solveBottomMiddle();
				positionBottomCorners();
				solveBottomCorners();
			}
		} //end if to solve
		for(int index = 0; index < SOLUTION_SIZE; index++)
			preoptimization[index] = solution[index];
		optimize();
		int cost = 0;
		for(int index = 0; index < SOLUTION_SIZE; index++)
			if(solution[index] != "x")
				cost++;
		printCube();
		if(validateSolution())
			cout << solution[1] << " took " << cost << " moves to do." << endl;
		else
			cout << "  !!Not Valid!!" << endl;
		if((cost <= shortestMoveCount) && checkCube(3))
		{
			updateShortest();
			shortestMoveCount = cost;
			shortestColor = cube[4][4];
		}
	}
	long long endTime = GetTickCount() - startTime;
	cout << "DONE!\n" << endl;
	cout << "Solution took " << shortestMoveCount << " moves to do in " << endTime << " milliseconds. \nQuickest color is: " << shortestColor << endl;
}

//This function looks at what the user has input, and trues to do that action
void performOutput(string input)
{
	if(input.compare("R") == 0)
		rightClock();
	else if(input.compare("r") == 0)
		rightCounter();
	else if(input.compare("L") == 0)
		leftClock();
	else if(input.compare("l") == 0)
		leftCounter();
	else if(input.compare("U") == 0)
		upperClock();
	else if(input.compare("u") == 0)
		upperCounter();
	else if(input.compare("F") == 0)
		frontClock();
	else if(input.compare("f") == 0)
		frontCounter();
	else if(input.compare("B") == 0)
		backClock();
	else if(input.compare("b") == 0)
		backCounter();
	else if(input.compare("D") == 0)
		downClock();
	else if(input.compare("d") == 0)
		downCounter();
	else if(input.compare("random") == 0)
		random();
	else if(input.compare("solve") == 0)
		solve();
	else if(input.compare("flip-r") == 0)
		flipCube(4);
	else if(input.compare("flip-f") == 0)
		flipCube(3);
	else if(input.compare("set") == 0)
		setMasterFromCube();
}

//Main function for program. Will run until the user has entered "quit" on the console.
int main()
{
	string input = "k";
	while(input.compare("quit") != 0)
	{
		for(solutionIndex = 0; solutionIndex < SOLUTION_SIZE; solutionIndex++)
			solution[solutionIndex] = "x";
		solutionIndex = 0;
		printCube();
		cout << ">";
		cin >> input;
		performOutput(input);
	}
	return 0;
}