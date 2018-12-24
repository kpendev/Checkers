11/15/2017
This is my first programing project (completed in a small group).

Objective: Create the game of checkers in C.

Features:
	-Two player game
	-Double,Triple... jumps available in any legal direction
	-When checker reaches opposite side, becomes king
	-Standard jump rules
	-Can deselect a selected checker by clicking on a invalid move spot
	
Notes:
	- the glut32.dll must be in the same folder as the executable to run
	- if trying to recompile, the directory of the glut.h file may have to be changed (line 19)
	
Bugs: the only bug present is that at times a selected checker keeps the color of a selected
	  checker even though it is not selected (does not affect game functionality)