!! How the TrainUI works for the framework code and example solution

October, 2008 - By Mike Gleicher

!!! Camera Controls

* Right Mouse = ArcBall (rotates world)
* ALT-RightMouse = Pan (translate view)
* double click Right Mouse = reset view
* Mousewheel = Zoom In / Zoom Out

!!! Moving Points

The program provides a "Mouse Pole" user interface.

Use the Left Mouse to select and move (with dragging).

When dragging, things move parallel to the ground plane. Unless the
CTRL key is held down, in which case things move perpindicular to the
ground plane.

!!! Some Features of the Sample Solution

The sample solution was written for simplicity, not for
efficiency. This might be a problem for you if you don't have a good
computer with a decent graphics card, it might not run fast enough. 

Some silly things you might notice:
* The parallel rails are flat ribbons. And they Z-Fight with the tops
  of the rail ties. I was going to make them thicker ribbons, but I
  didn't get to it. 
* The wheels on the cars are "trucked" (they swivel to stay on the
  track), but the center of the car is also locked onto the track and
  the car rotates around that point. You won't notice this unless the
  curve is really sharp.
* The wheels on the engine are fixed, so they go off the track in a
  sharp turn.
* Physics conserves energy - and that's it. The amount of energy in
  the system is set to be sufficient that the train doesn't stop at
  the highest point, so a tall track will have the train going really
  fast at the bottom. There is no friction or anything like that, just
  potential energy and kinetic energy transfering back and forth like
  in a pendulum.
* Save and Load is relative to where the executable is run from (so if
  you run in visual studio, its relative to the project/solution
  file).
* The interface for rotating the control points is terrible. But it
  let me make some tracks. I actually made the more complicated ones
  in excel.
* The "NoAL" buttons let you see what happens if you implement fancy
  tracks or multiple cars without arc length.

