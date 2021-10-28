(:toc:)

!!CS559 Train Project - Framework Code

written October 2008, by Mike Gleicher

This provides a framework for the CS559 Train Project. A simple system
that provides all of the basic features so that students can add their
own train and other features.

This code also serves as the basis for the example solution. When the
macro "EXAMPLE_SOLUTION" is defined, a few extra files are
included. These files are not provided to the students.

The source code for the framework is available in
pub:train/TrainSource2008.zip

The executables for the framework and the example solution is
available in pub:train/TrainExe.zip. You can also find the executables
in p:/course/cs559-gleicher/public/bin/Train2008.

We ''strongly'' recommend that you start by playing with the example
solution to get an idea of what the assignment is about.

!!! Files

Most of the files are in the "TrainFiles" directory. A few utility
files are in the "Utilities" directory that muse be placed in the same
directory as the TrainFiles directory. (the zip file should get this
right)

There is a "TrackFiles" directory inside of the TrainFiles directory
that contains sample track layouts.

!!! Getting Started and Learning your way Around

First, run the example solution and try it out. Look at the
ReadMe-UI.txt file for help in using it.

Then open up the solution and have a look. If you search for "TODO:"
you will see places in the code that we expect you to want to add your
own things.

You don't need to understand everything - you can do the entire
assignment just by adding things where we day "TODO". In fact, you can
see all of the places where the Example Solution (which is relatively
complete) had to add things to the code.

Some Hints:
* If you want to add widgets to the control panel, add them in
  TrainWindow.H first, then add them in TrainWindow's constructor.
  (there are TODO). 
* You probably want to give your widget a callback. If it just changes
  the state, the damageCB is a good option (it forces a redraw).
* The control points keep an extra 3 variables for an orientation. You
  might only want the position of the control points. But notice what
  the example solution does with them. The code tries to keep the
  orientations normalized.
* The world is set with Y going up (so the groundplane is XZ) and the
  ground has size 100x100 (to give you some scale).
* Change as little of the framework as possible. You can see exactly
  where the ExampleSolution connects to the framework. 
* The ExampleSolution tries to keep the files separate from the
  framework (so we can keep it from the students). So most things are
  function calls (or in the case of the extra widgets in TrainWindow,
  a class). But you can put things right into the framework.
