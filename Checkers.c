/*
11/15/2017

Checkers Game
	-Two player game
	-Double,Triple... jump is available in any direction
	-When checker reaches opposite side, becomes a king
	-standard jump rules

Honor Code: I have neither given or received, nor have I tolerated
			others' use of unauthorized aid.
			--Kostadin Pendev, Antuane Thompson, Jacob Gehring	

*/


#include <windows.h>  //must be included to work in windows.
#include <math.h>     //provides sin, cos, etc.
#include <stdlib.h>   //provides several useful functions.
#include <stdio.h>
#include <D:\ECE251\glut.h>  //connects to glut.h (must be changed to match correct path)

void checkerSelection(void);
void renderScene(void);
void checkerMovement(void);
void mouse(int button, int state, int x, int y);
void checkerJump(void);

int mouseX; //Global variable for the X position of the click
int mouseY; //Global variable for the Y position of the click
int playerTurn=1; //stores which player is next
int checkerCurrentlySelected; //The Checker Currently Selected
int moveMade=0;
int jumped=0;
int start=0;

int* pmoveMade=&moveMade;
int* pcheckerCurrentlySelected=&checkerCurrentlySelected;
int* pPlayerTurn=&playerTurn;
int* pMouseX=&mouseX; //pointer to X position of the click
int* pMouseY=&mouseY; //pointer to Y position of the click
int* pJumped=&jumped; //point jumped
int* pStart=&start;

struct Checker{
	int enable;
	int color; //0 for red, 1 for blue, 3 for green/highlighted
	float posX; //X Position
	float posY; //Y Positions
	int king; //0 for regular, 1 for King
};
struct Checker Checker[24];

//creates all of the chackers and places them
void initializeCheckers(void){
	int i = 0;
	float X = 150; //starting x reference 
	float Y = 50; //starting y reference 
	
	for(i=0; i <24; i++){
		Checker[i].enable = 1; //checker is still on the board
		Checker[i].king = 0;   //not a king
		if(i <12){
			Checker[i].color = 1; //blue
		}
		else{
			Checker[i].color = 0; //red
		}
	}
	
	//first row of blue
	for(i=0; i <4; i++){
		Checker[i].posX = X;
		Checker[i].posY = Y;
		X = X + 200;
	}
	
	X = 50;
	Y = 150;
	//second row of blue
	for(i=4; i <8; i++){
		Checker[i].posX = X;
		Checker[i].posY = Y;
		X = X + 200;
	}
	
	X = 150;
	Y = 250;
	//Third row of blue
	for(i=8; i <12; i++){
		Checker[i].posX = X;
		Checker[i].posY = Y;
		X = X + 200;
	}
	
	X = 50;
	Y = 550;
	//first row of red
	for(i=12; i <16; i++){
		Checker[i].posX = X;
		Checker[i].posY = Y;
		X = X + 200;
	}
	
	X = 150;
	Y = 650;
	//second row of red
	for(i=16; i <20; i++){
		Checker[i].posX = X;
		Checker[i].posY = Y;
		X = X + 200;
	}
	X = 50;
	Y = 750;
	//third row of red
	for(i=20; i <24; i++){
		Checker[i].posX = X;
		Checker[i].posY = Y;
		X = X + 200;
	}
}
//set up the view
void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 10000.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
void mouse(int button, int state, int x, int y) { //function that stores the mouse click location
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{	if(*pStart==0){
			*pStart=1;
			renderScene();
		}
		*pMouseX=x; //dereference the global variable mouseX
		*pMouseY=800-y; //dereference the global variable mouseY offset by 800
		printf("%d %d\n", *pMouseX, *pMouseY);  //This is the line that prints it to the screen.
		int tmpval = *pmoveMade; //Creates a Temporary value in order to avoid error from global variable
		if(tmpval == 0){
			checkerSelection(); // Goes to this function after the click is performed to see which checker was selected 
		}
		if(tmpval == 1){
			checkerJump(); //goes to this function after user clicks on desired location
		}
	}
}
void checkerJump(void){
	int xLeftBound[2]; //hold the left bound of each square
	int xRightBound[2]; //hold the right bound of each square
	int yTopBound[2]; //hold the top bound of each square
	int yBotBound[2]; //hold the bottom bound of each square
	int xBackLeftBound[2];
	int xBackRightBound[2];
	int yBackTopBound[2];
	int yBackBotBound[2];
	int i;
	int left = 0;
	int right = 0;
	int backLeft = 0;
	int backRight = 0;
	int killLeft;
	int killRight;
	int killBackRight;
	int killBackLeft;
	int done = 0;
	int player = *pPlayerTurn;
	int current = *pcheckerCurrentlySelected;
	if (player%2!=0 && current>=0 && current<12 && Checker[current].king==0){ //if it is blues' turn and a blue checker is selected and checker is not a king
		printf("blue checker selected\n");
		xRightBound[0]=Checker[current].posX-50; //creates the right bound of one square
		yTopBound[0]=Checker[current].posY+150; //creates the top bound of one square
		xLeftBound[0]=Checker[current].posX-150; //creates the left bound of one square
		yBotBound[0]=Checker[current].posY+50; //creates the bottom bound of one square
		xRightBound[1]=Checker[current].posX+150; //creates the right bound of the second square
		yTopBound[1]=Checker[current].posY+150; //creates the top bound of the second square
		xLeftBound[1]=Checker[current].posX+50; //creates the left bound of the second square
		yBotBound[1]=Checker[current].posY+50; //creates the bottom bound of the second square
		
		//checks if there is a red checker that can be jumped (two diagonal boxes)
		for(i=12; i<24; i++){ 
			if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
				left = 1;
				killLeft = i;
			}
			if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
				right = 1;
				killRight = i;
			}
		}
		
		xRightBound[0]-=100; //creates the right bound of one square
		yTopBound[0]+=100; //creates the top bound of one square
		xLeftBound[0]-=100; //creates the left bound of one square
		yBotBound[0]+=100; //creates the bottom bound of one square
		xRightBound[1]+=100; //creates the right bound of the second square
		yTopBound[1]+=100; //creates the top bound of the second square
		xLeftBound[1]+=100; //creates the left bound of the second square
		yBotBound[1]+=100; //creates the bottom bound of the second square
		
		//if the square is off the board
		if(xRightBound[1]>800 || yTopBound[1]>800){
			right = 0;
		}
		//if the square is off the board
		if(xLeftBound[0]<0 || yTopBound[0]>800){
			left = 0;
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to 
		if(right==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
					right = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to 
		if(left==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
					left = 0;
				}
			}
		}
		
		//if no checker is present, user can jump
		if(right==1){
			if(*pMouseX < xRightBound[1] && *pMouseX > xLeftBound[1] && *pMouseY < yTopBound[1] && *pMouseY > yBotBound[1]){
				Checker[current].posX +=200;
				Checker[current].posY +=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made 
				}else{
					*pmoveMade = 0;  //move has not been made
				}
				Checker[killRight].enable = 0; //checker is removed
				Checker[killRight].posX = 0; //Moves checker out of the way (X)
				Checker[killRight].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump
		if(left==1){
			if(*pMouseX < xRightBound[0] && *pMouseX > xLeftBound[0] && *pMouseY < yTopBound[0] && *pMouseY > yBotBound[0]){
				Checker[current].posX -=200;
				Checker[current].posY +=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killLeft].enable = 0; //checker is removed
				Checker[killLeft].posX = 0; //Moves checker out of the way (X)
				Checker[killLeft].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		if(left==0 && right==0 && *pJumped==0){ //no jumps available, normal movement
			checkerMovement();
			*pmoveMade = 0;

		}
		
		//player jumped and no more jumps available, next player turn
		else if(left==0 && right==0 && *pJumped==1){ 
			*pPlayerTurn+=1; //next players turn
			checkerSelection(); //go to checker selection
			Checker[current].color = 1; //change color
			*pmoveMade = 0;
			*pJumped = 0;
			renderScene();
		}
	}
	if (player%2==0 && current>=12 && current<24 && Checker[current].king==0){ //if it red's turn and a red checker is selected and checker is not a king
		printf("red checker selected\n");
		xRightBound[0]=Checker[current].posX-50; //creates the right bound of one square
		yTopBound[0]=Checker[current].posY-50; //creates the top bound of one square
		xLeftBound[0]=Checker[current].posX-150; //creates the left bound of one square
		yBotBound[0]=Checker[current].posY-150; //creates the bottom bound of one square
		xRightBound[1]=Checker[current].posX+150; //creates the right bound of the second square
		yTopBound[1]=Checker[current].posY-50; //creates the top bound of the second square
		xLeftBound[1]=Checker[current].posX+50; //creates the left bound of the second square
		yBotBound[1]=Checker[current].posY-150; //creates the bottom bound of the second square
		
		//blue checker available to be jumped?
		for(i=0; i<12; i++){
			if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
				left = 1;
				killLeft = i;
			}
			if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
				right = 1;
				killRight = i;
			}
		}
		
		xRightBound[0]-=100; //creates the right bound of one square
		yTopBound[0]-=100; //creates the top bound of one square
		xLeftBound[0]-=100; //creates the left bound of one square
		yBotBound[0]-=100; //creates the bottom bound of one square
		xRightBound[1]+=100; //creates the right bound of the second square
		yTopBound[1]-=100; //creates the top bound of the second square
		xLeftBound[1]+=100; //creates the left bound of the second square
		yBotBound[1]-=100; //creates the bottom bound of the second square
		
		//if off the board
		if(xRightBound[1]>800 || yBotBound[1]<0){
			right = 0;
		}
		if(xLeftBound[0]<0 || yBotBound[0]<0){
			left = 0;
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to
		if(right==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
					right = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to
		if(left==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
					left = 0;
				}
			}
		}
		
		//if the user can jump
		if(right==1){
			if(*pMouseX < xRightBound[1] && *pMouseX > xLeftBound[1] && *pMouseY < yTopBound[1] && *pMouseY > yBotBound[1]){
				Checker[current].posX +=200;
				Checker[current].posY -=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move not been made
				}
				Checker[killRight].enable = 0; //this checker has been jumped
				Checker[killRight].posX = 0; //Moves checker out of the way (X)
				Checker[killRight].posY = 0; //Moves checker out of the way (Y)
				Checker[current].color = 0;
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped==0){
				checkerMovement();
			}
		}
		if(left==1){
			if(*pMouseX < xRightBound[0] && *pMouseX > xLeftBound[0] && *pMouseY < yTopBound[0] && *pMouseY > yBotBound[0]){
				Checker[current].posX -=200;
				Checker[current].posY -=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killLeft].enable = 0; //this checker has been jumped
				Checker[killLeft].posX = 0; //Moves checker out of the way (X)
				Checker[killLeft].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped==0){
				checkerMovement();
			}
		}
		if(left==0 && right==0 && *pJumped==0){ //no jumps available, normal movement
			checkerMovement();
			*pmoveMade = 0;
		}
		else if(left==0 && right==0 && *pJumped==1){ //user has jumped and no more jumps available, reset for next turn
			*pPlayerTurn+=1; //next players turn
			Checker[current].color = 0; //reset color
			*pJumped = 0; 
			*pmoveMade = 0;
			renderScene();
			checkerSelection();
		}
	}
	if (player%2!=0 && current>=0 && current<12 && Checker[current].king==1){ //if it is blues' turn and a blue checker is selected and checker is a king
		printf("blue king checker selected\n");
		xRightBound[0]=Checker[current].posX-50; //creates the right bound of one square
		yTopBound[0]=Checker[current].posY+150; //creates the top bound of one square
		xLeftBound[0]=Checker[current].posX-150; //creates the left bound of one square
		yBotBound[0]=Checker[current].posY+50; //creates the bottom bound of one square
		xRightBound[1]=Checker[current].posX+150; //creates the right bound of the second square
		yTopBound[1]=Checker[current].posY+150; //creates the top bound of the second square
		xLeftBound[1]=Checker[current].posX+50; //creates the left bound of the second square
		yBotBound[1]=Checker[current].posY+50; //creates the bottom bound of the second square
		xBackRightBound[0]=Checker[current].posX-50; //creates the right bound of one square
		yBackTopBound[0]=Checker[current].posY-50; //creates the top bound of one square
		xBackLeftBound[0]=Checker[current].posX-150; //creates the left bound of one square
		yBackBotBound[0]=Checker[current].posY-150; //creates the bottom bound of one square
		xBackRightBound[1]=Checker[current].posX+150; //creates the right bound of the second square
		yBackTopBound[1]=Checker[current].posY-50; //creates the top bound of the second square
		xBackLeftBound[1]=Checker[current].posX+50; //creates the left bound of the second square
		yBackBotBound[1]=Checker[current].posY-150; //creates the bottom bound of the second square
		
		//check if there is a checker that can be jumped from all corners of current checker
		for(i=12; i<24; i++){
			if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
				left = 1;
				killLeft = i;
			}
			if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
				right = 1;
				killRight = i;
			}
			if(Checker[i].posX < xBackRightBound[1] && Checker[i].posX > xBackLeftBound[1] && Checker[i].posY < yBackTopBound[1] && Checker[i].posY > yBackBotBound[1]){
				backRight = 1;
				killBackRight = i;
			}
			if(Checker[i].posX < xBackRightBound[0] && Checker[i].posX > xBackLeftBound[0] && Checker[i].posY < yBackTopBound[0] && Checker[i].posY > yBackBotBound[0]){
				backLeft = 1;
				killBackLeft = i;
			}
		}
		xRightBound[0]-=100; //creates the right bound of one square
		yTopBound[0]+=100; //creates the top bound of one square
		xLeftBound[0]-=100; //creates the left bound of one square
		yBotBound[0]+=100; //creates the bottom bound of one square
		xRightBound[1]+=100; //creates the right bound of the second square
		yTopBound[1]+=100; //creates the top bound of the second square
		xLeftBound[1]+=100; //creates the left bound of the second square
		yBotBound[1]+=100; //creates the bottom bound of the second square
		xBackRightBound[0]-=100; //creates the right bound of one square
		yBackTopBound[0]-=100; //creates the top bound of one square
		xBackLeftBound[0]-=100; //creates the left bound of one square
		yBackBotBound[0]-=100; //creates the bottom bound of one square
		xBackRightBound[1]+=100; //creates the right bound of the second square
		yBackTopBound[1]-=100; //creates the top bound of the second square
		xBackLeftBound[1]+=100; //creates the left bound of the second square
		yBackBotBound[1]-=100; //creates the bottom bound of the second square
		
		//if off the board
		if(xRightBound[1]>800 || yTopBound[1]>800){
			right = 0;
		}
		if(xLeftBound[0]<0 || yTopBound[0]>800){
			left = 0;
		}
		if(xBackRightBound[1]>800 || yBackBotBound[1]<0){
			backRight = 0;
		}
		if(xBackLeftBound[0]<0 || yBackBotBound[0]<0){
			backLeft = 0;
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to//checks if there is a checker present in the box that the user is trying to jump to
		if(right==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
					right = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to
		if(left==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
					left = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to
		if(backRight==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xBackRightBound[1] && Checker[i].posX > xBackLeftBound[1] && Checker[i].posY < yBackTopBound[1] && Checker[i].posY > yBackBotBound[1]){
					backRight = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to
		if(backLeft==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xBackRightBound[0] && Checker[i].posX > xBackLeftBound[0] && Checker[i].posY < yBackTopBound[0] && Checker[i].posY > yBackBotBound[0]){
					backLeft = 0;
				}
			}
		}
		
		//if no checker is present, user can jump 
		if(right==1){
			if(*pMouseX < xRightBound[1] && *pMouseX > xLeftBound[1] && *pMouseY < yTopBound[1] && *pMouseY > yBotBound[1]){
				Checker[current].posX +=200;
				Checker[current].posY +=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killRight].enable = 0; //checker is removed
				Checker[killRight].posX = 0; //Moves checker out of the way (X)
				Checker[killRight].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump 
		if(left==1){
			if(*pMouseX < xRightBound[0] && *pMouseX > xLeftBound[0] && *pMouseY < yTopBound[0] && *pMouseY > yBotBound[0]){
				Checker[current].posX -=200;
				Checker[current].posY +=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killLeft].enable = 0; //checker is removed
				Checker[killLeft].posX = 0; //Moves checker out of the way (X)
				Checker[killLeft].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump 
		if(backRight==1){
			if(*pMouseX < xBackRightBound[1] && *pMouseX > xBackLeftBound[1] && *pMouseY < yBackTopBound[1] && *pMouseY > yBackBotBound[1]){
				Checker[current].posX +=200;
				Checker[current].posY -=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{ 
					*pmoveMade = 0; //move has not been made
				}
				Checker[killBackRight].enable = 0; //checker is removed
				Checker[killBackRight].posX = 0; //Moves checker out of the way (X)
				Checker[killBackRight].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump 
		if(backLeft==1){
			if(*pMouseX < xBackRightBound[0] && *pMouseX > xBackLeftBound[0] && *pMouseY < yBackTopBound[0] && *pMouseY > yBackBotBound[0]){
				Checker[current].posX -=200;
				Checker[current].posY -=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killBackLeft].enable = 0; //checker is removed
				Checker[killBackLeft].posX = 0; //Moves checker out of the way (X)
				Checker[killBackLeft].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		if(left==0 && right==0 && backLeft==0 && backRight==0 && *pJumped==0){ //no jumps available, normal movement
			checkerMovement();
			*pmoveMade = 0;
		}
		else if(left==0 && right==0 &&backLeft==0 && backRight==0 && *pJumped==1){ //jump has been made and no more jumps available, next players turn
			*pPlayerTurn+=1; //next players turn
			checkerSelection(); //checker selection
			Checker[current].color = 1;
			*pmoveMade = 0;
			*pJumped = 0;
			renderScene();
		}
	}
	
	if (player%2==0 && current>=12 && current<24 && Checker[current].king==1){ //if it is red's turn and a red checker is selected and checker is a king
		printf("red king checker selected\n");
		xRightBound[0]=Checker[current].posX-50; //creates the right bound of one square
		yTopBound[0]=Checker[current].posY+150; //creates the top bound of one square
		xLeftBound[0]=Checker[current].posX-150; //creates the left bound of one square
		yBotBound[0]=Checker[current].posY+50; //creates the bottom bound of one square
		xRightBound[1]=Checker[current].posX+150; //creates the right bound of the second square
		yTopBound[1]=Checker[current].posY+150; //creates the top bound of the second square
		xLeftBound[1]=Checker[current].posX+50; //creates the left bound of the second square
		yBotBound[1]=Checker[current].posY+50; //creates the bottom bound of the second square
		xBackRightBound[0]=Checker[current].posX-50; //creates the right bound of one square
		yBackTopBound[0]=Checker[current].posY-50; //creates the top bound of one square
		xBackLeftBound[0]=Checker[current].posX-150; //creates the left bound of one square
		yBackBotBound[0]=Checker[current].posY-150; //creates the bottom bound of one square
		xBackRightBound[1]=Checker[current].posX+150; //creates the right bound of the second square
		yBackTopBound[1]=Checker[current].posY-50; //creates the top bound of the second square
		xBackLeftBound[1]=Checker[current].posX+50; //creates the left bound of the second square
		yBackBotBound[1]=Checker[current].posY-150; //creates the bottom bound of the second square
		
		//checks if there is a blue checker that can be jumped
		for(i=0; i<12; i++){
			if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
				left = 1;
				killLeft = i;
			}
			if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
				right = 1;
				killRight = i;
			}
			if(Checker[i].posX < xBackRightBound[1] && Checker[i].posX > xBackLeftBound[1] && Checker[i].posY < yBackTopBound[1] && Checker[i].posY > yBackBotBound[1]){
				backRight = 1;
				killBackRight = i;
			}
			if(Checker[i].posX < xBackRightBound[0] && Checker[i].posX > xBackLeftBound[0] && Checker[i].posY < yBackTopBound[0] && Checker[i].posY > yBackBotBound[0]){
				backLeft = 1;
				killBackLeft = i;
			}
		}
		
		xRightBound[0]-=100; //creates the right bound of one square
		yTopBound[0]+=100; //creates the top bound of one square
		xLeftBound[0]-=100; //creates the left bound of one square
		yBotBound[0]+=100; //creates the bottom bound of one square
		xRightBound[1]+=100; //creates the right bound of the second square
		yTopBound[1]+=100; //creates the top bound of the second square
		xLeftBound[1]+=100; //creates the left bound of the second square
		yBotBound[1]+=100; //creates the bottom bound of the second square
		xBackRightBound[0]-=100; //creates the right bound of one square
		yBackTopBound[0]-=100; //creates the top bound of one square
		xBackLeftBound[0]-=100; //creates the left bound of one square
		yBackBotBound[0]-=100; //creates the bottom bound of one square
		xBackRightBound[1]+=100; //creates the right bound of the second square
		yBackTopBound[1]-=100; //creates the top bound of the second square
		xBackLeftBound[1]+=100; //creates the left bound of the second square
		yBackBotBound[1]-=100; //creates the bottom bound of the second square
		
		//if off the board
		if(xRightBound[1]>800 || yTopBound[1]>800){
			right = 0;
		}
		if(xLeftBound[0]<0 || yTopBound[0]>800){
			left = 0;
		}
		if(xBackRightBound[1]>800 || yBackBotBound[1]<0){
			backRight = 0;
		}
		if(xBackLeftBound[0]<0 || yBackBotBound[0]<0){
			backLeft = 0;
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to//checks if there is a checker present in the box that the user is trying to jump to
		if(right==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[1] && Checker[i].posX > xLeftBound[1] && Checker[i].posY < yTopBound[1] && Checker[i].posY > yBotBound[1]){
					right = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to//checks if there is a checker present in the box that the user is trying to jump to
		if(left==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xRightBound[0] && Checker[i].posX > xLeftBound[0] && Checker[i].posY < yTopBound[0] && Checker[i].posY > yBotBound[0]){
					left = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to//checks if there is a checker present in the box that the user is trying to jump to
		if(backRight==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xBackRightBound[1] && Checker[i].posX > xBackLeftBound[1] && Checker[i].posY < yBackTopBound[1] && Checker[i].posY > yBackBotBound[1]){
					backRight = 0;
				}
			}
		}
		
		//checks if there is a checker present in the box that the user is trying to jump to//checks if there is a checker present in the box that the user is trying to jump to
		if(backLeft==1){
			for(i=0; i<24; i++){
				if(Checker[i].posX < xBackRightBound[0] && Checker[i].posX > xBackLeftBound[0] && Checker[i].posY < yBackTopBound[0] && Checker[i].posY > yBackBotBound[0]){
					backLeft = 0;
				}
			}
		}
		
		//if no checker is present, user can jump
		if(right==1){
			if(*pMouseX < xRightBound[1] && *pMouseX > xLeftBound[1] && *pMouseY < yTopBound[1] && *pMouseY > yBotBound[1]){
				Checker[current].posX +=200;
				Checker[current].posY +=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killRight].enable = 0; //checker has been jumped
				Checker[killRight].posX = 0; //Moves checker out of the way (X)
				Checker[killRight].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump
		if(left==1){
			if(*pMouseX < xRightBound[0] && *pMouseX > xLeftBound[0] && *pMouseY < yTopBound[0] && *pMouseY > yBotBound[0]){
				Checker[current].posX -=200;
				Checker[current].posY +=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killLeft].enable = 0; //checker has been jumped
				Checker[killLeft].posX = 0; //Moves checker out of the way (X)
				Checker[killLeft].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump
		if(backRight==1){
			if(*pMouseX < xBackRightBound[1] && *pMouseX > xBackLeftBound[1] && *pMouseY < yBackTopBound[1] && *pMouseY > yBackBotBound[1]){
				Checker[current].posX +=200;
				Checker[current].posY -=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killBackRight].enable = 0; //checker has been jumped
				Checker[killBackRight].posX = 0; //Moves checker out of the way (X)
				Checker[killBackRight].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		//if no checker is present, user can jump
		if(backLeft==1){
			if(*pMouseX < xBackRightBound[0] && *pMouseX > xBackLeftBound[0] && *pMouseY < yBackTopBound[0] && *pMouseY > yBackBotBound[0]){
				Checker[current].posX -=200;
				Checker[current].posY -=200;
				if(*pJumped == 0){
					*pmoveMade = 1; //move has been made
				}else{
					*pmoveMade = 0; //move has not been made
				}
				Checker[killBackLeft].enable = 0; //checker has been jumped
				Checker[killBackLeft].posX = 0; //Moves checker out of the way (X)
				Checker[killBackLeft].posY = 0; //Moves checker out of the way (Y)
				*pJumped = 1; //there has been a jump
				renderScene(); //re-render
				checkerJump(); //another jump available?
			}
			else if(*pJumped == 0){
				checkerMovement();
			}
		}
		
		if(left==0 && right==0 && backLeft==0 && backRight==0 && *pJumped==0){ //no jumps available, normal movement
			checkerMovement();
			*pmoveMade = 0;
		}
		
		else if(left==0 && right==0 &&backLeft==0 && backRight==0 && *pJumped==1){ //there has been a jump and no more jumps available, reset everything for the next player 
			*pPlayerTurn+=1; //next players turn
			checkerSelection(); //checker selection
			Checker[current].color = 0;
			*pmoveMade = 0;
			*pJumped = 0;
			renderScene();
		}
	}
}
void checkerSelection(void) {
	int i=0; //counter
	float radius; //radius used to test if user clicked on checker
	if (*pPlayerTurn%2!=0){
		for(i=0; i <12; i++){ //goes through each blue checker
			if(Checker[i].enable == 1){
				radius= sqrt( pow(*pMouseX-Checker[i].posX, 2)+pow(*pMouseY-Checker[i].posY, 2)); //calculates the radius of the click from the center of the checker
				if (radius<50) //if radius is less than 50, that is the checker user clicked on 
				{
				printf("Checker#%d\n", i); //prints checker selected
				*pcheckerCurrentlySelected=i; //stores the selected checker
				Checker[i].color = 2; //changes the color of the checker to green to show it has been selected
				renderScene(); //rerenders the scene to update the changes in the graphics
				*pmoveMade=1; //indicates that a checker has been selected
				}
			}
			if(Checker[i].posY == 750){
				Checker[i].king=1;
			}
		}
	}
	if (*pPlayerTurn%2==0){
		for(i=12; i <24; i++){ //goes through each red checker
			if(Checker[i].enable == 1){
				radius= sqrt( pow(*pMouseX-Checker[i].posX, 2)+pow(*pMouseY-Checker[i].posY, 2));//calculates the radius of the click from the center of the checker
				if (radius<50) //if radius is less than 50, that is the checker user clicked on
				{
				printf("Checker#%d\n", i); //prints checker selected
				*pcheckerCurrentlySelected=i; //stores the selected checker
				Checker[i].color = 2; //changes the color of the checker to green to show it has been selected
				renderScene(); //rerenders the scene to update the changes in the graphics
				*pmoveMade=1; //indicates that a checker has been selected
				}
			}
			if(Checker[i].posY == 50){
				Checker[i].king=1;
			}
		}
		
	}
}
void checkerMovement(void) {
	int xLeftBound[4]; //hold the left bound of each square
	int xRightBound[4]; //hold the right bound of each square
	int yTopBound[4]; //hold the top bound of each square
	int yBotBound[4]; //hold the bottom bound of each square
	int i=0; //counter
	int spotTaken=0; //indicates if spot chosen is occupied or not (0 is not occupied, 1 is occupied)
	int testX;
	int testY;
	int testZ;
	int testA;
		if (*pPlayerTurn%2!=0 && *pcheckerCurrentlySelected>=0 && *pcheckerCurrentlySelected<12 && Checker[*pcheckerCurrentlySelected].king==0){ //if it is blues' turn and a blue checker is selected and checker is not a king
			printf("blue checker selected\n");
			xRightBound[0]=Checker[*pcheckerCurrentlySelected].posX-50; //creates the right bound of one square
			yTopBound[0]=Checker[*pcheckerCurrentlySelected].posY+150; //creates the top bound of one square
			xLeftBound[0]=Checker[*pcheckerCurrentlySelected].posX-150; //creates the left bound of one square
			yBotBound[0]=Checker[*pcheckerCurrentlySelected].posY+50; //creates the bottom bound of one square
			xRightBound[1]=Checker[*pcheckerCurrentlySelected].posX+150; //creates the right bound of the second square
			yTopBound[1]=Checker[*pcheckerCurrentlySelected].posY+150; //creates the top bound of the second square
			xLeftBound[1]=Checker[*pcheckerCurrentlySelected].posX+50; //creates the left bound of the second square
			yBotBound[1]=Checker[*pcheckerCurrentlySelected].posY+50; //creates the bottom bound of the second square
			if (xLeftBound[0]<*pMouseX && *pMouseX< xRightBound[0] && yBotBound[0]<*pMouseY && *pMouseY<yTopBound[0]){
				testX=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testX-=100; //X value of center of top left box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY+=100; //Y value of center of top left box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){ //checks to see if there is a checker in the selected box and if that checker has been jumped
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0; 
					} 
				} 
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX -=100; //moves checker to top left
					Checker[*pcheckerCurrentlySelected].posY +=100; //moves checker to top left
					Checker[*pcheckerCurrentlySelected].color = 1; //make checker blue again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Blue moved left\n");
					printf("Turn=%d\n",*pPlayerTurn);
					if(Checker[*pcheckerCurrentlySelected].posY==750){
						Checker[*pcheckerCurrentlySelected].king=1;
					}
				}
			}
			else if (xLeftBound[1]<*pMouseX && *pMouseX< xRightBound[1] && yBotBound[1]<*pMouseY && *pMouseY<yTopBound[1]){
				testX=Checker[*pcheckerCurrentlySelected].posX;//X value of current checker selected
				testX+=100; //X value of center of top left box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY+=100; //Y value of center of top left box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					} 
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");				
					Checker[*pcheckerCurrentlySelected].posX +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].posY +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].color = 1; //make checker blue again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Blue moved right\n");
					printf("Turn=%d\n",*pPlayerTurn);
					if(Checker[*pcheckerCurrentlySelected].posY==750){
						Checker[*pcheckerCurrentlySelected].king=1;
					}
				}
			}
			else{ //if a nonvaild box is selected for movement
				Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
				renderScene();
				checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
				printf("Invalid Selection, Checker Deselected\n");
				*pmoveMade = 0;
			}
		}
		if (*pPlayerTurn%2!=0 && *pcheckerCurrentlySelected>=0 && *pcheckerCurrentlySelected<12 && Checker[*pcheckerCurrentlySelected].king==1){ //if it is blues' turn and a blue checker is selected and checker is a king
			printf("blue king checker selected\n");
			xRightBound[0]=Checker[*pcheckerCurrentlySelected].posX-50; //creates the right bound of one square
			yTopBound[0]=Checker[*pcheckerCurrentlySelected].posY+150; //creates the top bound of one square
			xLeftBound[0]=Checker[*pcheckerCurrentlySelected].posX-150; //creates the left bound of one square
			yBotBound[0]=Checker[*pcheckerCurrentlySelected].posY+50; //creates the bottom bound of one square
			xRightBound[1]=Checker[*pcheckerCurrentlySelected].posX+150; //creates the right bound of the second square
			yTopBound[1]=Checker[*pcheckerCurrentlySelected].posY+150; //creates the top bound of the second square
			xLeftBound[1]=Checker[*pcheckerCurrentlySelected].posX+50; //creates the left bound of the second square
			yBotBound[1]=Checker[*pcheckerCurrentlySelected].posY+50; //creates the bottom bound of the second square
			xRightBound[2]=Checker[*pcheckerCurrentlySelected].posX-50;
			yTopBound[2]=Checker[*pcheckerCurrentlySelected].posY-50;
			xLeftBound[2]=Checker[*pcheckerCurrentlySelected].posX-150;
			yBotBound[2]=Checker[*pcheckerCurrentlySelected].posY-150;
			xRightBound[3]=Checker[*pcheckerCurrentlySelected].posX+150;
			yTopBound[3]=Checker[*pcheckerCurrentlySelected].posY-50;
			xLeftBound[3]=Checker[*pcheckerCurrentlySelected].posX+50;
			yBotBound[3]=Checker[*pcheckerCurrentlySelected].posY-150;
			if (xLeftBound[0]<*pMouseX && *pMouseX< xRightBound[0] && yBotBound[0]<*pMouseY && *pMouseY<yTopBound[0]){
				testX=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testX-=100; //X value of center of top left box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY+=100; //Y value of center of top left box
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){ //checks to see if there is a checker in the selected box and if that checker has been jumped
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0; 
					} 
				} 
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX -=100; //moves checker to top left
					Checker[*pcheckerCurrentlySelected].posY +=100; //moves checker to top left
					Checker[*pcheckerCurrentlySelected].color = 1; //make checker blue again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Blue moved left\n");
					printf("Turn=%d\n",*pPlayerTurn);
				
				}
			}
			else if (xLeftBound[1]<*pMouseX && *pMouseX< xRightBound[1] && yBotBound[1]<*pMouseY && *pMouseY<yTopBound[1]){
				testX=Checker[*pcheckerCurrentlySelected].posX;//X value of current checker selected
				testX+=100; //X value of center of top left box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY+=100; //Y value of center of top left box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					} 
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");				
					Checker[*pcheckerCurrentlySelected].posX +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].posY +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].color = 1; //make checker blue again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Blue moved right\n");
					printf("Turn=%d\n",*pPlayerTurn);
				}
			}
			if (xLeftBound[2]<*pMouseX && *pMouseX< xRightBound[2] && yBotBound[2]<*pMouseY && *pMouseY<yTopBound[2]){
				testZ=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testZ-=100; //X value of center of bottom left box
				testA=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testA-=100; //Y value of center of bottom left box
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testZ && Checker[i].posY == testA){ //checks to see if there is a checker in the selected box and if that checker has been jumped
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0; 
					} 
				} 
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX -=100; //moves checker to bottom left
					Checker[*pcheckerCurrentlySelected].posY -=100; //moves checker to bottom left
					Checker[*pcheckerCurrentlySelected].color = 1; //make checker blue again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Blue moved left\n");
					printf("Turn=%d\n",*pPlayerTurn);
				
				}
			}
			else if (xLeftBound[3]<*pMouseX && *pMouseX< xRightBound[3] && yBotBound[3]<*pMouseY && *pMouseY<yTopBound[3]){
				testZ=Checker[*pcheckerCurrentlySelected].posX;//X value of current checker selected
				testZ+=100; //X value of center of bottom right box
				testA=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testA-=100; //Y value of center of bottom right box
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testZ && Checker[i].posY == testA){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					} 
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");				
					Checker[*pcheckerCurrentlySelected].posX +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].posY -=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].color = 1; //make checker blue again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Blue moved right\n");
					printf("Turn=%d\n",*pPlayerTurn);
				}
			}
			else{ //if a nonvaild box is selected for movement
				Checker[*pcheckerCurrentlySelected].color = 1; //changes color back to blue
				renderScene();
				checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
				printf("Invalid Selection, Checker Deselected\n");
				*pmoveMade = 0;
			}
		}
		if (*pPlayerTurn%2==0 && *pcheckerCurrentlySelected>=12 && *pcheckerCurrentlySelected<24 && Checker[*pcheckerCurrentlySelected].king==0){//if it is red's turn and a red checker is selected and checker is not a king
			printf("red checker selected\n");
			xRightBound[0]=Checker[*pcheckerCurrentlySelected].posX-50; //creates the right bound of one square
			yTopBound[0]=Checker[*pcheckerCurrentlySelected].posY-50; //creates the top bound of one square
			xLeftBound[0]=Checker[*pcheckerCurrentlySelected].posX-150; //creates the left bound of one square
			yBotBound[0]=Checker[*pcheckerCurrentlySelected].posY-150; //creates the bottom bound of one square
			xRightBound[1]=Checker[*pcheckerCurrentlySelected].posX+150; //creates the right bound of the second square
			yTopBound[1]=Checker[*pcheckerCurrentlySelected].posY-50; //creates the top bound of the second square
			xLeftBound[1]=Checker[*pcheckerCurrentlySelected].posX+50; //creates the left bound of the second square
			yBotBound[1]=Checker[*pcheckerCurrentlySelected].posY-150; //creates the bottom bound of the second square
			if (xLeftBound[0]<*pMouseX && *pMouseX< xRightBound[0] && yBotBound[0]<*pMouseY && *pMouseY<yTopBound[0]){
				testX=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testX-=100; //X value of center of bottom left box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY-=100; //Y value of center of bottom left box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					}
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX -=100; //moves checker to bottom left
					Checker[*pcheckerCurrentlySelected].posY -=100; //moves checker to bottom left
					Checker[*pcheckerCurrentlySelected].color = 0; //make checker red again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Red moved left\n");
					printf("Turn=%d\n",*pPlayerTurn);
					if(Checker[*pcheckerCurrentlySelected].posY==50){
					Checker[*pcheckerCurrentlySelected].king=1;
					}
				}
			}
			else if (xLeftBound[1]<*pMouseX && *pMouseX< xRightBound[1] && yBotBound[1]<*pMouseY && *pMouseY<yTopBound[1]){
				testX=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testX+=100; //X value of center of bottom right box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY-=100; //Y value of center of bottom right box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					}
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX +=100; //moves checker to bottom right
					Checker[*pcheckerCurrentlySelected].posY -=100; //moves checker to bottom right
					Checker[*pcheckerCurrentlySelected].color = 0; //make checker red again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Red moved right\n");
					printf("Turn=%d\n",*pPlayerTurn);
					if(Checker[*pcheckerCurrentlySelected].posY==50){
					Checker[*pcheckerCurrentlySelected].king=1;
				}
				}
			}
			else{ //if a nonvaild box is selected for movement
				Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
				renderScene();
				checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
				printf("Invalid Selection, Checker Deselected\n");
				*pmoveMade = 0;
			}
		}
		if (*pPlayerTurn%2==0 && *pcheckerCurrentlySelected>=12 && *pcheckerCurrentlySelected<24 && Checker[*pcheckerCurrentlySelected].king==1){//if it is red's turn and a red checker is selected and checker is a king
			printf("red king checker selected\n");
			xRightBound[0]=Checker[*pcheckerCurrentlySelected].posX-50; //creates the right bound of one square
			yTopBound[0]=Checker[*pcheckerCurrentlySelected].posY-50; //creates the top bound of one square
			xLeftBound[0]=Checker[*pcheckerCurrentlySelected].posX-150; //creates the left bound of one square
			yBotBound[0]=Checker[*pcheckerCurrentlySelected].posY-150; //creates the bottom bound of one square
			xRightBound[1]=Checker[*pcheckerCurrentlySelected].posX+150; //creates the right bound of the second square
			yTopBound[1]=Checker[*pcheckerCurrentlySelected].posY-50; //creates the top bound of the second square
			xLeftBound[1]=Checker[*pcheckerCurrentlySelected].posX+50; //creates the left bound of the second square
			yBotBound[1]=Checker[*pcheckerCurrentlySelected].posY-150; //creates the bottom bound of the second square
			xRightBound[2]=Checker[*pcheckerCurrentlySelected].posX-50; //creates the right bound of one square
			yTopBound[2]=Checker[*pcheckerCurrentlySelected].posY+150; //creates the top bound of one square
			xLeftBound[2]=Checker[*pcheckerCurrentlySelected].posX-150; //creates the left bound of one square
			yBotBound[2]=Checker[*pcheckerCurrentlySelected].posY+50; //creates the bottom bound of one square
			xRightBound[3]=Checker[*pcheckerCurrentlySelected].posX+150; //creates the right bound of the second square
			yTopBound[3]=Checker[*pcheckerCurrentlySelected].posY+150; //creates the top bound of the second square
			xLeftBound[3]=Checker[*pcheckerCurrentlySelected].posX+50; //creates the left bound of the second square
			yBotBound[3]=Checker[*pcheckerCurrentlySelected].posY+50; //creates the bottom bound of the second square
			if (xLeftBound[0]<*pMouseX && *pMouseX< xRightBound[0] && yBotBound[0]<*pMouseY && *pMouseY<yTopBound[0]){
				testX=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testX-=100; //X value of center of bottom left box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY-=100; //Y value of center of bottom left box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					}
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX -=100; //moves checker to bottom left
					Checker[*pcheckerCurrentlySelected].posY -=100; //moves checker to bottom left
					Checker[*pcheckerCurrentlySelected].color = 0; //make checker red again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Red moved left\n");
					printf("Turn=%d\n",*pPlayerTurn);
					
				}
			}
			else if (xLeftBound[1]<*pMouseX && *pMouseX< xRightBound[1] && yBotBound[1]<*pMouseY && *pMouseY<yTopBound[1]){
				testX=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testX+=100; //X value of center of bottom right box
				testY=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testY-=100; //Y value of center of bottom right box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testX && Checker[i].posY == testY){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					}
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX +=100; //moves checker to bottom right
					Checker[*pcheckerCurrentlySelected].posY -=100; //moves checker to bottom right
					Checker[*pcheckerCurrentlySelected].color = 0; //make checker red again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Red moved right\n");
					printf("Turn=%d\n",*pPlayerTurn);
				}
			}
			if (xLeftBound[2]<*pMouseX && *pMouseX< xRightBound[2] && yBotBound[2]<*pMouseY && *pMouseY<yTopBound[2]){
				testZ=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testZ-=100; //X value of center of top left box
				testA=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testA+=100; //Y value of center of top left box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testZ && Checker[i].posY == testA){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					}
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX -=100; //moves checker to top left
					Checker[*pcheckerCurrentlySelected].posY +=100; //moves checker to top left
					Checker[*pcheckerCurrentlySelected].color = 0; //make checker red again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Red moved left\n");
					printf("Turn=%d\n",*pPlayerTurn);
					
				}
			}
			else if (xLeftBound[3]<*pMouseX && *pMouseX< xRightBound[3] && yBotBound[3]<*pMouseY && *pMouseY<yTopBound[3]){
				testZ=Checker[*pcheckerCurrentlySelected].posX; //X value of current checker selected
				testZ+=100; //X value of center of top right box
				testA=Checker[*pcheckerCurrentlySelected].posY; //Y value of current checker selected
				testA+=100; //Y value of center of top right box
				
				for(i=0; i <24; i++){
					//printf("%f=%i? %f=%i?\n", Checker[i].posX,testX, Checker[i].posY, testY);
					if(Checker[i].enable==1 && Checker[i].posX == testZ && Checker[i].posY == testA){
						spotTaken=1; //spot is taken
						printf("Spot Taken Invalid Move\n");
						Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
						renderScene();
						checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
						*pmoveMade = 0;
					}
				}
				if (spotTaken==0){ //if spot is not taken, move
					printf("Spot Not Taken- Valid Move\n");
					Checker[*pcheckerCurrentlySelected].posX +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].posY +=100; //moves checker to top right
					Checker[*pcheckerCurrentlySelected].color = 0; //make checker red again
					renderScene();
					*pmoveMade=0;
					*pPlayerTurn+=1; //next player's turn
					printf("Red moved right\n");
					printf("Turn=%d\n",*pPlayerTurn);
				}
			}
			else{ //if a nonvaild box is selected for movement
				Checker[*pcheckerCurrentlySelected].color = 0; //changes color back to red
				renderScene();
				checkerSelection(); // Goes to this function after the click is performed to see which checker was selected
				printf("Invalid Selection, Checker Deselected\n");
				*pmoveMade = 0;
			}
		}
}
void renderScene(void) {
    //You may change the following line to adjust the background:
	glClearColor(0.0f, 0.0f, 1.0f, 0); //Set background to blue
	// Clear the screen. DO NOT CHANGE.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	// Reset transformations. DO NOT CHANGE.
	glLoadIdentity(); 
	// Set the camera view.  DO NOT CHANGE.
	gluLookAt(0.0f, 0.0f, 965.0f, //Location of your eye
			  0.0f, 0.0f, 0.0f, //Location you are looking at
			  0.0f, 1.0f, 0.0f); //Direction of "up"

	
	int red = 0;
	int blue = 0;
	int temp = 0;
	int i = 0;
	int row = 0;
    int col = 0;
	float size=100.0f;
	if(*pStart==0){
		blue++;
		red++;
		glPushMatrix();
		glTranslatef(-400.0f ,-400.0f, 50.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);		         // Draw a Quadrangle
	      glVertex3f(0,0, 0.0f); // Bottom Left
	      glVertex3f(0,800, 0.0f); // Top Left
	      glVertex3f(800,800, 0.0f); // Top Right
	      glVertex3f(800,0, 0.0f); //Bottom Right
        glEnd();
		glPopMatrix();
		char *string="Do NOT resize window!"; //String to be printed
		char *charPtr;
		glColor3f(0,0,1); //Print it in blue
		glRasterPos3f(-50.0,0,51); //Start a little left of center
		for (charPtr=string; *charPtr; charPtr++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
		}
		string="Blue Starts!"; //String to be printed
		*charPtr;
		glColor3f(0,0,1); //Print it in blue
		glRasterPos3f(-50.0,-50,51); //Start a little left of center
		for (charPtr=string; *charPtr; charPtr++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
		}
		string="Click anywhere to begin."; //String to be printed
		*charPtr;
		glColor3f(0,0,1); //Print it in red
		glRasterPos3f(-50.0,-100,51); //Start a little left of center
		for (charPtr=string; *charPtr; charPtr++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
		}
	}
	if(temp!=1 && *pStart==1){
		glPushMatrix();
		glTranslatef(-400.0f, -400.0f,0.0f);
		for (row = 1; row <=8 ; row++) {
		for(col=1; col<=8; col++)
			{
			if((row + col)%2 == 0) // if row+col is even
			{
			glColor3f(1.0f,1.0f,1.0f);// black
			}
			else
			{
			glColor3f(0.0f, 0.0f, 0.0f);// white
			}	
			glBegin(GL_QUADS);		         // Draw a Quadrangle
			glVertex3f((row-1)*size, (col-1)*size, 0.0f); // Bottom Left
			glVertex3f((row-1)*size, col*size, 0.0f); // Top Left
			glVertex3f( row*size, col*size, 0.0f); // Top Right
			glVertex3f(row*size, (col-1)*size, 0.0f); //Bottom Right
			glEnd();					     // Finished Square (Quadrangle)
		}
		}
		for(i=0; i<12; i++){
			if(Checker[i].enable==1){
				blue++;
			}
			if(Checker[i].king==1){
				glColor3f(1,1,1); //Print it in white
				glRasterPos3f(Checker[i].posX,Checker[i].posY,50);//Change to center of Checker
				char *string="K"; //String to be printed
				char *charPtr;
				for (charPtr=string; *charPtr; charPtr++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
				}
			}
		}
		for(i=12; i<24; i++){
			if(Checker[i].enable==1){
				red++;
			}
			if(Checker[i].king==1){
				glColor3f(1,1,1); //Print it in white
				glRasterPos3f(Checker[i].posX,Checker[i].posY,50);//Change to center of Checker
				char *string="K"; //String to be printed
				char *charPtr;
				for (charPtr=string; *charPtr; charPtr++)
				{
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
				}
			}
		}
		glPopMatrix();
		for(i=0; i<24; i++){
			if(Checker[i].enable == 1){
				glPushMatrix();
				glTranslatef(-400.0f ,-400.0f, 0.0f);
				if(Checker[i].color == 0){
					glColor3f(1.0f,0.0f,0.0f);//red
					}
				else if(Checker[i].color == 1){
					glColor3f(0.0f,0.0f,1.0f);//blue
					}
				else if(Checker[i].color == 2){
					glColor3f(0.0f,1.0f,0.0f);//green
				}
				glTranslatef(Checker[i].posX ,Checker[i].posY, 0.0f);
				glutSolidSphere(50.0f,10,10);
				glPopMatrix();
			}
		}
	}
	if(blue==0){
		temp = 1;
		glPushMatrix();
		glTranslatef(-400.0f ,-400.0f, 50.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);		         // Draw a Quadrangle
	      glVertex3f(0,0, 0.0f); // Bottom Left
	      glVertex3f(0,800, 0.0f); // Top Left
	      glVertex3f(800,800, 0.0f); // Top Right
	      glVertex3f(800,0, 0.0f); //Bottom Right
        glEnd();
		glPopMatrix();
		char *string="Red Wins!"; //String to be printed
		char *charPtr;
		glColor3f(1,0,0); //Print it in red
		glRasterPos3f(-50.0,0,51); //Start a little left of center
		for (charPtr=string; *charPtr; charPtr++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
		}
	}
	if(red==0){
		temp = 1;
		glPushMatrix();
		glTranslatef(-400.0f ,-400.0f, 50.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);		         // Draw a Quadrangle
	      glVertex3f(0,0, 0.0f); // Bottom Left
	      glVertex3f(0,800, 0.0f); // Top Left
	      glVertex3f(800,800, 0.0f); // Top Right
	      glVertex3f(800,0, 0.0f); //Bottom Right
        glEnd();
		glPopMatrix();
		char *string="Blue Wins!"; //String to be printed
		char *charPtr;
		glColor3f(0,0,1); //Print it in red
		glRasterPos3f(-50.0,0,51); //Start a little left of center
		for (charPtr=string; *charPtr; charPtr++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *charPtr);//Draw each character, automatically updating raster position.
		}
	}
    glutSwapBuffers();
} 
int main(int argc, char **argv) {
	initializeCheckers();
	// init GLUT and create window
	glutInit(&argc, argv); //Allows input arguments to initialize OpenGL.
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0); //Create window at this location.
	glutInitWindowSize(800,800); //Default window size.
	glutCreateWindow("Checkers");  //Window title.
	// OpenGL init
	glEnable(GL_DEPTH_TEST);  //Enables objects to appear in 3D
	// register callbacks
	glutDisplayFunc(renderScene);  //When you need to display, call renderScene().
	glutReshapeFunc(changeSize);   //When you need to reshape, call changeSize().
	glutMouseFunc(mouse);
	glutMainLoop();  //Enter an infinite loop to draw the picture.
	// enter GLUT event processing cycle
	return 1;
}