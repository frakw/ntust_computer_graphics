/************************************************************************
     File:        TrainView.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						The TrainView is the window that actually shows the 
						train. Its a
						GL display canvas (Fl_Gl_Window).  It is held within 
						a TrainWindow
						that is the outer window with all the widgets. 
						The TrainView needs 
						to be aware of the window - since it might need to 
						check the widgets to see how to draw

	  Note:        we need to have pointers to this, but maybe not know 
						about it (beware circular references)

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#pragma once

// Preclarify for preventing the compiler error
class TrainWindow;
class CTrack;


//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################


// The TrainView also needs to handle its user interface events.
// since this is an FLTk Window, we need to include FlTk Headers
// the pragmas turn off the warnings from FlTk - our code should have no warnings,
// but their code doesn't meet that standard
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_Gl_Window.h>
#pragma warning(pop)
class Shader;
class Model;
class ParticleGenerator;
class Texture;

// this uses the old ArcBall Code
#include "Utilities/ArcBallCam.H"
#include "Utilities/Pnt3f.H"
//#include "particle.h"
#include "camera.h"
class TrainView : public Fl_Gl_Window
{
	public:
		// note that we keep the "standard widget" constructor arguments
		TrainView(int x, int y, int w, int h, const char* l = 0);

		// overrides of important window things
		virtual int handle(int);
		virtual void draw();

		// all of the actual drawing happens in this routine
		// it has to be encapsulated, since we draw differently if
		// we're drawing shadows (no colors, for example)
		void drawStuff(bool doingShadows=false);

		void draw_track(bool);
		void draw_train(bool);
		void draw_first_sleeper(bool);
		void no_arc_to_arc();
		void draw_pillar(float);
		void draw_wheel(Pnt3f center, Pnt3f forward, Pnt3f cross, Pnt3f up, float r, float thickness, int color_pos, bool doingShadows);
		// setup the projection - assuming that the projection stack has been
		// cleared for you
		void setProjection();

		// Reset the Arc ball control
		void resetArcball();

		// pick a point (for when the mouse goes down)
		void doPick();
		Pnt3f GMT(const Pnt3f& p0, const Pnt3f& p1, const Pnt3f& p2, const Pnt3f& p3, int matrix_type, float t);

		void draw_tunnel();
	public:
		ArcBallCam		arcball;			// keep an ArcBall for the UI
		int				selectedCube;  // simple - just remember which cube is selected

		TrainWindow*	tw;				// The parent of this display window
		CTrack*			m_pTrack;		// The track of the entire scene

		Camera camera();

		Pnt3f tunnel_pos = Pnt3f(0,0,0);
		int tunnel_tex = -1;
		int terrain_tex = -1;
		Model* tunnel = nullptr;
		Model* steve = nullptr;
		Model* terrain = nullptr;
		Shader* for_model = nullptr;
		Shader* for_model_texture = nullptr;

		ParticleGenerator* Particles = nullptr;//記得設null
		Shader* particle_shader = nullptr;//記得設null
		Texture* particle_texture = nullptr;//記得設null

		float DIVIDE_LINE = 1000.0f;
		float t_time = 0.0f;
		float arc_length = 0.0f;
		float sleeper_length = 3.0f;
		float sleeper_width = 5.0f;
		float space_length = 6.0f;
		int train_i = 0;
		float train_t = 0.0f;
		float rail_width = 2.50f;

		int car_amount = 1;
		float car_to_car_distance = 4.0f;

		float physics_add = 1.0f;
		int color_pos = 0;
};


