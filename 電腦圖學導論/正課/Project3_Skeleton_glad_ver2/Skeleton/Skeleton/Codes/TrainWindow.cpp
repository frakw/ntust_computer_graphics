/************************************************************************
     File:        TrainWindow.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						this class defines the window in which the project 
						runs - its the outer windows that contain all of 
						the widgets, including the "TrainView" which has the 
						actual OpenGL window in which the train is drawn

						You might want to modify this class to add new widgets
						for controlling	your train

						This takes care of lots of things - including installing 
						itself into the FlTk "idle" loop so that we get periodic 
						updates (if we're running the train).


     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#include <FL/fl.h>
#include <FL/Fl_Box.h>

// for using the real time clock
#include <time.h>

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"



//************************************************************************
//
// * Constructor
//========================================================================
TrainWindow::
TrainWindow(const int x, const int y) 
	: Fl_Double_Window(x,y,800,600,"Train and Roller Coaster")
//========================================================================
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn

		trainView = new TrainView(5,5,590,590);
		trainView->tw = this;
		trainView->m_pTrack = &m_Track;
		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,590);
		widgets->begin();

		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		arcLength = new Fl_Button(730,pty,65,20,"ArcLength");
		togglify(arcLength,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,10);
		speed->value(2);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;

		/*
		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 60, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(1);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when pressed
		worldCam->callback((Fl_Callback*)damageCB,this);
		trainCam = new Fl_Button(670, pty, 60, 20, "Train");
        trainCam->type(FL_RADIO_BUTTON);
        trainCam->value(0);
        trainCam->selection_color((Fl_Color)3);
		trainCam->callback((Fl_Callback*)damageCB,this);
		topCam = new Fl_Button(735, pty, 60, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();
		*/
		cameraBrowser = new Fl_Browser(605, pty, 120, 75, "Camera type");
		cameraBrowser->type(2);		// select
		cameraBrowser->callback((Fl_Callback*)damageCB, this);
		cameraBrowser->add("worldCam");
		cameraBrowser->add("trainCam");
		cameraBrowser->add("topCam");
		cameraBrowser->select(1);

		pty += 110;

		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		splineBrowser = new Fl_Browser(605,pty,120,75,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->callback((Fl_Callback*)damageCB,this);
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->add("Cubic B-Spline");
		splineBrowser->select(2);

		pty += 110;

		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);

		pty+=30;

		// TODO: add widgets for all of your fancier features here
		Fl_Button* _add_car = new Fl_Button(605, pty, 60, 20, "Add Car");
		_add_car->callback((Fl_Callback*)add_car, this);
		Fl_Button* _del_car = new Fl_Button(675, pty, 75, 20, "Delete Car");
		_del_car->callback((Fl_Callback*)del_car, this);
		pty += 30;
		dir_L = new Fl_Button(605, pty, 60, 20, "dir light");
		togglify(dir_L,1);
		point_L = new Fl_Button(675, pty, 75, 20, "point light");
		togglify(point_L);
		pty += 25;
		tension = new Fl_Value_Slider(655, pty, 140, 20, "tension");
		tension->range(0, 1);
		tension->value(0.5);
		tension->align(FL_ALIGN_LEFT);
		tension->type(FL_HORIZONTAL);
		tension->callback((Fl_Callback*)change_tension,this);
		pty += 30;
		physics = new Fl_Button(605, pty, 60, 20, "physics");
		togglify(physics);
#ifdef EXAMPLE_SOLUTION
		makeExampleWidgets(this,pty);
#endif

		// we need to make a little phantom widget to have things resize correctly
		Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		widgets->resizable(resizebox);

		widgets->end();
	}
	end();	// done adding to this widget

	// set up callback on idle
	//Fl::add_idle((void (*)(void*))runButtonCB,this);
	animating = true;
	Fl::add_idle((void (*)(void*))keeping_refresh, this);
}

//************************************************************************
//
// * handy utility to make a button into a toggle
//========================================================================
void TrainWindow::
togglify(Fl_Button* b, int val)
//========================================================================
{
	b->type(FL_TOGGLE_BUTTON);		// toggle
	b->value(val);		// turned off
	b->selection_color((Fl_Color)3); // yellow when pressed	
	b->callback((Fl_Callback*)damageCB,this);
}

//************************************************************************
//
// *
//========================================================================
void TrainWindow::
damageMe()
//========================================================================
{
	if (trainView->selectedCube >= ((int)m_Track.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}

//************************************************************************
//
// * This will get called (approximately) 30 times per second
//   if the run button is pressed
//========================================================================
void TrainWindow::
advanceTrain(float dir)
//========================================================================
{
	//#####################################################################
	// TODO: make this work for your train
	//#####################################################################
	static bool pre_arc_state = arcLength->value();

	if (pre_arc_state != arcLength->value()) {
		if (arcLength->value()) {
			trainView->no_arc_to_arc();
		}
		else {
			m_Track.trainU = trainView->train_i + trainView->train_t;
		}
	}
	
	if (arcLength->value()) {
		m_Track.trainU += 1.0f * speed->value();
		if (physics->value()) m_Track.trainU += trainView->physics_add;
		if (m_Track.trainU > trainView->arc_length) {
			m_Track.trainU -= trainView->arc_length;
		}
		else if (m_Track.trainU < 0) {
			m_Track.trainU += trainView->arc_length;
		}
	}
	else {
		m_Track.trainU += 0.02f * speed->value();
		if (m_Track.trainU > m_Track.points.size()) {
			m_Track.trainU -= m_Track.points.size();
		}
	}
	pre_arc_state = arcLength->value();
	trainView->color_pos++;
#ifdef EXAMPLE_SOLUTION
	// note - we give a little bit more example code here than normal,
	// so you can see how this works

	if (arcLength->value()) {
		float vel = ew.physics->value() ? physicsSpeed(this) : dir * (float)speed->value();
		world.trainU += arclenVtoV(world.trainU, vel, this);
	}
	else {
		world.trainU += dir * ((float)speed->value() * .1f);
	}

	float nct = static_cast<float>(world.points.size());
	if (world.trainU > nct) world.trainU -= nct;
	if (world.trainU < 0) world.trainU += nct;
#endif
}