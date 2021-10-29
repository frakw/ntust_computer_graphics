/************************************************************************
     File:        Track.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     Container for the "World"

						This provides a container for all of the "stuff" 
						in the world.

						It could have been all global variables, or it could 
						have just been
						contained in the window. The advantage of doing it 
						this way is that
						we might have multiple windows looking at the same 
						world. But, I don't	think we'll actually do that.

						See the readme for commentary on code style

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/
#pragma once

#include <vector>

using std::vector; // avoid having to say std::vector all of the time

// make use of other data structures from this project
#include "ControlPoint.H"

class CTrack {
	public:		
		// Constructor
		CTrack();

	public:
		// when we want to clear the control points, we really "reset" them 
		// to have 4 default positions (since we should never have fewer
		// than 4 points)
		void resetPoints();


		// read and write to files
		void readPoints(const char* filename);
		void writePoints(const char* filename);

	public:
		// rather than have generic objects, we make a special case for these few
		// objects that we know that all implementations are going to need and that
		// we're going to have to handle specially
		vector<ControlPoint> points;

		//###################################################################
		// TODO: you might want to do this differently
		//###################################################################
		// the state of the train - basically, all I need to remember is where
		// it is in parameter space
		float trainU = 0.0f;
};