/************************************************************************
     File:        CallBacks.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     Header file to define callback functions.
						define the callbacks for the TrainWindow

						these are little functions that get called when the 
						various widgets
						get accessed (or the fltk timer ticks). these 
						functions are used 
						when TrainWindow sets itself up.

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/
#pragma once

#include <time.h>
#include <math.h>

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_File_Chooser.H>
#include <Fl/math.h>
#pragma warning(pop)

//***************************************************************************
//
// * Reset the control points back to their base setup
//===========================================================================
void resetCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	tw->m_Track.resetPoints();
	tw->trainView->selectedCube = -1;
	tw->m_Track.trainU = 0;
	tw->damageMe();
}

//***************************************************************************
//
// * any time something changes, you need to force a redraw
//===========================================================================
void damageCB(Fl_Widget*, TrainWindow* tw)
{
	tw->damageMe();
}

//***************************************************************************
//
// * Callback that adds a new point to the spline
// idea: add the point AFTER the selected point
//===========================================================================
void addPointCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	// get the number of points
	size_t npts = tw->m_Track.points.size();
	// the number for the new point
	size_t newidx = (tw->trainView->selectedCube>=0) ? tw->trainView->selectedCube : 0;

	// pick a reasonable location
	size_t previdx = (newidx + npts -1) % npts;
	Pnt3f npos = (tw->m_Track.points[previdx].pos + tw->m_Track.points[newidx].pos) * .5f;

	tw->m_Track.points.insert(tw->m_Track.points.begin() + newidx,npos);

	// make it so that the train doesn't move - unless its affected by this control point
	// it should stay between the same points
	if (ceil(tw->m_Track.trainU) > ((float)newidx)) {
		tw->m_Track.trainU += 1;
		if (tw->m_Track.trainU >= npts) tw->m_Track.trainU -= npts;
	}

	tw->damageMe();
}

//***************************************************************************
//
// * Callback that deletes a point from the spline
//===========================================================================
void deletePointCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	if (tw->m_Track.points.size() > 4) {
		if (tw->trainView->selectedCube >= 0) {
			tw->m_Track.points.erase(tw->m_Track.points.begin() + tw->trainView->selectedCube);
		} else
			tw->m_Track.points.pop_back();
	}
	tw->damageMe();
}
//***************************************************************************
//
// * Advancing the train
//===========================================================================
void forwCB(Fl_Widget*, TrainWindow* tw)
{
	tw->advanceTrain(2);
	tw->damageMe();
}
//***************************************************************************
//
// * Reverse the movement of the train
//===========================================================================
void backCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	tw->advanceTrain(-2);
	tw->damageMe();
}



static unsigned long lastRedraw = 0;
//***************************************************************************
//
// * Callback for idling - if things are sitting, this gets called
// if the run button is pushed, then we need to make the train go.
// This is taken from the old "RunButton" demo.
// another nice problem to have - most likely, we'll be too fast
// don't draw more than 30 times per second
//===========================================================================
void runButtonCB(TrainWindow* tw)
//===========================================================================
{
	if (tw->runButton->value()) {	// only advance time if appropriate
		if (clock() - lastRedraw > CLOCKS_PER_SEC/30) {
			lastRedraw = clock();
			tw->advanceTrain();
			tw->damageMe();
		}
	}
}

//***************************************************************************
//
// * Load the control points from the files
//===========================================================================
void loadCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	const char* fname = 
		fl_file_chooser("Pick a Track File","*.txt","TrackFiles/track.txt");
	if (fname) {
		tw->m_Track.readPoints(fname);
		tw->damageMe();
	}
}
//***************************************************************************
//
// * Save the control points
//===========================================================================
void saveCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	const char* fname = 
		fl_input("File name for save (should be *.txt)","TrackFiles/");
	if (fname)
		tw->m_Track.writePoints(fname);
}

//***************************************************************************
//
// * Rotate the selected control point about x axis
//===========================================================================
void rollx(TrainWindow* tw, float dir)
{
	int s = tw->trainView->selectedCube;
	if (s >= 0) {
		Pnt3f old = tw->m_Track.points[s].orient;
		float si = sin(((float)M_PI_4) * dir);
		float co = cos(((float)M_PI_4) * dir);
		tw->m_Track.points[s].orient.y = co * old.y - si * old.z;
		tw->m_Track.points[s].orient.z = si * old.y + co * old.z;
	}
	tw->damageMe();
} 

//***************************************************************************
//
// * Rotate the selected control point about x axis by one more degree
//===========================================================================
void rpxCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollx(tw,1);
}
//***************************************************************************
//
// * Rotate the selected control point  about x axis by less one degree
//===========================================================================
void rmxCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollx(tw,-1);
}

//***************************************************************************
//
// * Rotate the selected control point  about z axis
//===========================================================================
void rollz(TrainWindow* tw, float dir)
//===========================================================================
{
	int s = tw->trainView->selectedCube;
	if (s >= 0) {

		Pnt3f old = tw->m_Track.points[s].orient;

		float si = sin(((float)M_PI_4) * dir);
		float co = cos(((float)M_PI_4) * dir);

		tw->m_Track.points[s].orient.y = co * old.y - si * old.x;
		tw->m_Track.points[s].orient.x = si * old.y + co * old.x;
	}

	tw->damageMe();
}

//***************************************************************************
//
// * Rotate the selected control point  about the z axis one more degree
//===========================================================================
void rpzCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollz(tw,1);
}

//***************************************************************************
//
// *  Rotate the selected control point  about the z axis one less degree
//===========================================================================
void rmzCB(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	rollz(tw, -1);
}

void add_car(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	tw->trainView->car_amount++;
	tw->damageMe();
}
void del_car(Fl_Widget*, TrainWindow* tw)
//===========================================================================
{
	if (tw->trainView->car_amount > 1)
		tw->trainView->car_amount--;
	tw->damageMe();
}

void change_tension(Fl_Widget*, TrainWindow* tw) {
	tw->damageMe();
}



void keeping_refresh(TrainWindow* tw)
{
	if (clock() - lastRedraw > CLOCKS_PER_SEC / 30 && tw->animating) {
		lastRedraw = clock();
		tw->time += 0.01f;
		tw->damageMe();
		if (std::numeric_limits<float>::max() - tw->time <= 0.01f) {
			tw->time = 0.0f;
		}

		if (tw->runButton->value()) {	// only advance time if appropriate
			lastRedraw = clock();
			tw->advanceTrain();
		}
	}
}