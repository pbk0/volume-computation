volume-computation
==================

This project uses monte carlo simulation to estimate volume of polytope

Find more details on 
http://www.spikingneurons.com/projects_volume-computation


Video:
http://www.youtube.com/watch?v=UPeNyzkmttw


/**
 *
 * @author praveen_kulkarni
 */

####################################################################
#1 check out the files and execute follow instructions
sudo apt-get install freeglut3 freeglut3-dev
sudo apt-get install binutils-gold
g++ -lGL -lglut -lm -lGLU MonteCarloOpenGL.cpp -o test
./test

####################################################################
#2 controls

F1              - for full screen
Esc             - to exit application
p               - display points
h               - toggle display of hyperplanes
1               - enable hyperplane 1
2               - enable hyperplane 2
3               - enable hyperplane 3
4               - enable hyperplane 4
5               - enable hyperplane 5
6               - enable hyperplane 6

right click     - select which plane equation parameters to control
(after selecting plane equation by right click control its 
parameters with following keys)
q
w
e
r
Shift+q
Shift+w
Shift+e
Shift+r

u               - update points (the points within polytope 
                  will be red)     

####################################################################
#3
So the points within polytope (red ones) divided by total points 
is the approximate volume of polytope by monte carlo simulation
