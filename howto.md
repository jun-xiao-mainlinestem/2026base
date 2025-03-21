*robot-config.cpp contains all the all the parameter constants (e.g CLAMP_ANGLE). 
smaller number for CLAMP_ANGLE == clamp more.

*Edit autons.cpp to write/revise your own auton functions. 
The drive (chassis) APIs are:
turn_to_heading
drive_distance
drive_with_voltage

*Here are the game pad controls:
(essential)
L1: hold to run the intaked) and release to stop.
R1: press to toggle the clamp/release of mogo with auto-clamp enabled.
R1: hold to release the mogo (disable auto-clamp). When the button is released, clamp down on mogo.

(additional)
Arrow up/down: press and hold to manually adjust the clamp up/down slowly by a small degree.
Arrow left/right: to swing the slapper up/down.
A: switch between drive mode (double arcade, single arcade, & tank drive). Double arcade is the default as it is the easiest way to drive (right stick forward/backward, left stick turn).

(macros)
B: tip over the mogo 
X: corner sweep