/************************************************************************
	 File:        MazeWindow.cpp
	 Author:
				  Stephen Chenney, schenney@cs.wisc.edu
	 Modifier
				  Yu-Chi Lai, yu-chi@cs.wisc.edu
	 Comment:
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
						Class header file for the MazeWindow class. The MazeWindow is
						the window in which the viewer's view of the maze is displayed.
	 Platform:    Visio Studio.Net 2003 (converted to 2005)
*************************************************************************/
#include "MazeWindow.h"
#include <Fl/math.h>
#include <Fl/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <array>
#include "all_glm.h"

float global::aspect = 0.0f;

//*************************************************************************
//
// * Constructor
//=========================================================================
MazeWindow::
MazeWindow(int x, int y, int width, int height, const char* label, Maze* m)
	: Fl_Gl_Window(x, y, width, height, label)
	//=========================================================================
{
	maze = m;

	// The mouse button isn't down and there is no key pressed.
	down = false;
	z_key = 0;
	global::aspect = w() / h();
}


//*************************************************************************
//
// * Set the maze. Also causes a redraw.
//=========================================================================
void MazeWindow::
Set_Maze(Maze* m)
//=========================================================================
{
	// Change the maze
	maze = m;
	// Force a redraw
	redraw();
}

//*************************************************************************
//
// * draw() method invoked whenever the view changes or the window
//   otherwise needs to be redrawn.
//=========================================================================
void MazeWindow::
draw(void)
//=========================================================================
{
	float   focal_length = 0.0f;

	if (!valid()) {
		// The OpenGL context may have been changed
		// Set up the viewport to fill the window.

		glViewport(0, 0, w(), h());

		// We are using orthogonal viewing for 2D. This puts 0,0 in the
		// middle of the screen, and makes the image size in view space
		// the same size as the window.
		gluOrtho2D(-w() * 0.5, w() * 0.5, -h() * 0.5, h() * 0.5);

		// Sets the clear color to black.
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);


	// Draw the "floor". It is an infinite plane perpendicular to
	// vertical, so we know it projects to cover the entire bottom
	// half of the screen. Walls of the maze will be drawn over the top
	// of it.
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-w() * 0.5f, -h() * 0.5f);
	glVertex2f(w() * 0.5f, -h() * 0.5f);
	glVertex2f(w() * 0.5f, 0.0);
	glVertex2f(-w() * 0.5f, 0.0);

	// Draw the ceiling. It will project to the entire top half
	// of the window.
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex2f(w() * 0.5f, h() * 0.5f);
	glVertex2f(-w() * 0.5f, h() * 0.5f);
	glVertex2f(-w() * 0.5f, 0.0);
	glVertex2f(w() * 0.5f, 0.0);
	glEnd();
	if (maze) {
		maze->Draw_View(focal_length);
	}

}


//*************************************************************************
//
// *
//=========================================================================
bool MazeWindow::
Drag(float dt)
//=========================================================================
{
	float   x_move, y_move, z_move;

	if (down) {
		int	dx = x_down - x_last;
		int   dy = y_down - y_last;
		float dist;

		// Set the viewing direction based on horizontal mouse motion.
		maze->Set_View_Dir(d_down + 360.0f * dx / (float)w());

		// Set the viewer's linear motion based on a speed (derived from
		// vertical mouse motion), the elapsed time and the viewing direction.
		dist = 50.0f * dt * dy / (float)h();//½ÕÆF±Ó«×
		x_move = dist * (float)cos(Maze::To_Radians(maze->viewer_dir));
		y_move = dist * (float)sin(Maze::To_Radians(maze->viewer_dir));
	}
	else {
		x_move = 0.0;
		y_move = 0.0;
	}

	// Update the z posn
	z_move = z_key * 0.1f;
	z_key = 0;

	// Tell the maze how much the view has moved. It may restrict the motion
	// if it tries to go through walls.
	maze->Move_View_Posn(x_move, y_move, z_move);

	return true;
}


//*************************************************************************
//
// *
//=========================================================================
bool MazeWindow::
Update(float dt)
//=========================================================================
{
	// Update the view

	if (down || z_key) // Only do anything if the mouse button is down.
		return Drag(dt);

	// Nothing changed, so no need for a redraw.
	return false;
}


//*************************************************************************
//
// *
//=========================================================================
int MazeWindow::
handle(int event)
//=========================================================================
{
	if (!maze)
		return Fl_Gl_Window::handle(event);

	// Event handling routine.
	switch (event) {
	case FL_PUSH:
		down = true;
		x_last = x_down = Fl::event_x();
		y_last = y_down = Fl::event_y();
		d_down = maze->viewer_dir;
		return 1;
	case FL_DRAG:
		x_last = Fl::event_x();
		y_last = Fl::event_y();
		return 1;
	case FL_RELEASE:
		down = false;
		return 1;
	case FL_KEYBOARD:
		/*
		if ( Fl::event_key() == FL_Up )	{
			z_key = 1;
			return 1;
		}
		if ( Fl::event_key() == FL_Down ){
			z_key = -1;
			return 1;
		}
		*/
		return Fl_Gl_Window::handle(event);
	case FL_FOCUS:
	case FL_UNFOCUS:
		return 1;
	}

	// Pass any other event types on the superclass.
	return Fl_Gl_Window::handle(event);
}