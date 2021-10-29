/************************************************************************
     File:        CallBacks.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						Programming Assignment 3 - Trains and Roller Coasters

						The "main file" and entry point for the train project
						this basically opens up the window and starts up the world

	  Note:			this code is meant to serve as both the example solution, 
						as well as the starting point for student projects. 
						Therefore, some (questionable)
						design tradeoffs have been made - please see the 
						ReadMe-Initial.txt file

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#include "stdio.h"
#include "TrainWindow.H"

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl.h>
#include <ctime>
#pragma warning(pop)


int main(int, char**)
{
	srand(time(NULL));
	printf("CS559 Train Assignment\n");

	TrainWindow tw;
	tw.show();

	Fl::run();
}