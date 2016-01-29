BM40A0600 Introduction to Computer Graphics
Assignment

Student: Lushin Iakov
Student number: 0458163

----

OpenGL

This assignment is incomplete. It has Exercise 5 Task 1 as it's basis and implements a couple of additional features in it. The main new feature is the light source #3 that changes color in time.

The motivation for the scene is the Christmas and the New Year celebration. The blinking light represents the colored lights of a Christmas tree. Unfortunately, there is no Christmas tree itself.

Only standard components are required for running the program.

Compilation command (on Linux):
gcc -o assignment assignment.c shader.c matlib.c -lGL -lGLEW -lglut -lm

No initial information is required to run the program, just its execution.

Light controls are as follows:
0 - toggles white (very dim) ambient light.
1 - toggles white (dim) directional light.
2 - toggles multicolored point light with specular reflections.
3 - toggles full (brightest) ambient light.
All lights off equals to full (brightest) ambient light.

Camera controls are as follows:
x/X, y/Y, z/Z - moves camera in negative/positive direction of x, y or z axis.
a/A, s/S, d/D - moves point of look in negative/positive direction of x, y or z axis.
i/o           - zoomes in/out.
n/N, f/F      - moves the nearest or farthest border of view towards/from the camera.
p             - toggles perspective/orthogonal projection.

Other controls:
ESC/q - exits the program.

No special steps are required after the program finishes.

Table of items implemented:
Not present.

----

POV-Ray

This part of the assignment is not present.