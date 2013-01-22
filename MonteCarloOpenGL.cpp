/**
 *
 * @author praveen_kulkarni
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#define NOP 100000

bool fullscreen = false;
bool mouseDown = false;
bool slicedPolytope = false;
bool hideSlabs = true;
bool showParticles = false;
bool updatedPointColor = false;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

int xslabs = 0;
int yslabs = 0;
int zslabs = 0;
float gapbetweenslabs = 0.03;
float slabstransperency = 0.3;

static float particlesize = 1.0f;

int* sliceplanestatus = new int[6];

GLdouble eqn[6][4]={1.0000	,0.1000	,-0.100	,0.8000	,
					0.3000	,1.0000	,-0.100	,0.3000	,
					0.1000	,0.0000	,0.9000	,0.7000	,
					-1.000	,0.1000	,-0.150	,0.7000	,
					0.2000	,-1.000	,-0.200	,0.6000	,
					-0.400	,0.2000	,-1.000	,0.5000	};

float pointsX[NOP];
float pointsY[NOP];
float pointsZ[NOP];
float pointsC[NOP];

int equationselectedindex = 0;
void equationselected(int value){
	equationselectedindex=value;
}

void updatePointsColor(){
	//printf("\n\n\n.......................\n");
	for(int i=0; i<NOP; i++){
		int flag=1;
		for(int e=0; e<6; e++){
		//int e=1;
			float temp = eqn[e][0]*(pointsX[i])+eqn[e][1]*pointsY[i]+eqn[e][2]*pointsZ[i]+eqn[e][3];
			//printf("\n[%f * %f] + [%f * %f] + [%f * %f] + [%f * %f] .... %f",eqn[e][0], pointsX[i], eqn[e][1], pointsY[i], eqn[e][2], pointsZ[i], eqn[e][3], 0.5f, temp);
			if(temp<0.0f)flag=0;
		}
		if(flag==1){
			pointsC[i]=1.0;
		}
	}
	
}

void resetPointsColor(){
	for(int i=0; i<NOP; i++){
		pointsC[i]=0.0;
	}
}

void drawBox()
{
	float size_box = 2.0f;

	
	///////// menu
	glutCreateMenu(equationselected);
	glutAddMenuEntry("equation 1", 0);
	glutAddMenuEntry("equation 2", 1);
	glutAddMenuEntry("equation 3", 2);
	glutAddMenuEntry("equation 4", 3);
	glutAddMenuEntry("equation 5", 4);
	glutAddMenuEntry("equation 6", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	///////////////////// wire cube
	glColor3f(0.9f,0.1f,0.7f);
	glutWireCube(size_box);

	////////////////// center cube
	glColor4f(0.0,1.0,0.0,1.0);
	glutSolidCube(size_box/15.0f);

	/////////// clip plane
	if(sliceplanestatus[0]==1){
		glClipPlane (GL_CLIP_PLANE0, eqn[0]);
		glEnable (GL_CLIP_PLANE0);
	}
	if(sliceplanestatus[1]==1){
		glClipPlane (GL_CLIP_PLANE1, eqn[1]);
		glEnable (GL_CLIP_PLANE1);
	}
	if(sliceplanestatus[2]==1){
		glClipPlane (GL_CLIP_PLANE2, eqn[2]);
		glEnable (GL_CLIP_PLANE2);
	}
	if(sliceplanestatus[3]==1){
		glClipPlane (GL_CLIP_PLANE3, eqn[3]);
		glEnable (GL_CLIP_PLANE3);
	}
	if(sliceplanestatus[4]==1){
		glClipPlane (GL_CLIP_PLANE4, eqn[4]);
		glEnable (GL_CLIP_PLANE4);
	}
	if(sliceplanestatus[5]==1){
		glClipPlane (GL_CLIP_PLANE5, eqn[5]);
		glEnable (GL_CLIP_PLANE5);
	}
	

	
	


	///////// fill by quads
	///*
	glColor4f(0.0,1.0,1.0,slabstransperency);
	glBegin(GL_QUADS);
	for(float x = -(size_box/2.0f); x<(size_box/2.0f); ){

		if(xslabs==1){
			glVertex3f(x, -(size_box/2.0f),-(size_box/2.0f));
			glVertex3f(x, -(size_box/2.0f),(size_box/2.0f));
			glVertex3f(x, (size_box/2.0f),(size_box/2.0f));
			glVertex3f(x, (size_box/2.0f),-(size_box/2.0f));
		}

		if(yslabs==1){
			glVertex3f( -(size_box/2.0f),x,-(size_box/2.0f));
			glVertex3f( -(size_box/2.0f),x,(size_box/2.0f));
			glVertex3f( (size_box/2.0f),x,(size_box/2.0f));
			glVertex3f( (size_box/2.0f),x,-(size_box/2.0f));
		}

		if(zslabs==1){
			glVertex3f( -(size_box/2.0f),-(size_box/2.0f),x);
			glVertex3f( -(size_box/2.0f),(size_box/2.0f),x);
			glVertex3f( (size_box/2.0f),(size_box/2.0f),x);
			glVertex3f( (size_box/2.0f),-(size_box/2.0f),x);
		}

		x+=gapbetweenslabs;
	}
	glEnd();
	//*/


	////////////// fill by points
	/*
	glPointSize(10.0);
	glEnable(GL_POINT_SMOOTH);
	glColor4f(0.0,1.0,1.0,0.1);
	glBegin(GL_POINTS);	
	for(float x = -(size_box/2.0f); x<(size_box/2.0f); ){
		for(float y = -(size_box/2.0f); y<(size_box/2.0f); ){
			for(float z = -(size_box/2.0f); z<(size_box/2.0f); ){
				glVertex3f(x, y, z);
				z+=0.051f;
			}
			y+=0.051f;
		}
		x+=0.051f;
	}
	glEnd();
	//*///////////


	///////////////////////
	if(sliceplanestatus[0]==1){
		glDisable (GL_CLIP_PLANE0);
	}
	if(sliceplanestatus[1]==1){
		glDisable (GL_CLIP_PLANE1);
	}
	if(sliceplanestatus[2]==1){
		glDisable (GL_CLIP_PLANE2);
	}
	if(sliceplanestatus[3]==1){
		glDisable (GL_CLIP_PLANE3);
	}
	if(sliceplanestatus[4]==1){
		glDisable (GL_CLIP_PLANE4);
	}
	if(sliceplanestatus[5]==1){
		glDisable (GL_CLIP_PLANE5);
	}



	////////////// fill by points
	if(showParticles==true){
		glPointSize(particlesize);
		//glEnable(GL_POINT_SMOOTH);
		//glColor3f(0.0,0.0,0.0);
		//glTranslatef(-1.0f,-1.0f,-1.0f);
		//glScalef(2.0f,2.0f,2.0f);
		glPushMatrix();
		glBegin(GL_POINTS);	
		for(int i=0; i<NOP; i++){
			float c = pointsC[i];
			glColor3f(c,1.0-c,0.0);
			glVertex3f(pointsX[i],pointsY[i],pointsZ[i]);
		}
		glEnd();
		glPopMatrix();
	}
	
}

bool init()
{

	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	glEnable(GL_BLEND);// enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// needed for blending

	return true;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//gluLookAt(
	//	0.0f, 0.0f, 3.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f);

	glTranslatef(0,0,-5.0);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	/////////////// zoom
	glPushMatrix();
	//glTranslatef(0,0,-5.0);
	drawBox();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	if (!mouseDown)
	{
		//xrot += 0.3f;
		//yrot += 0.4f;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	/////////// applying slicing plane
	case 97:
		if(slicedPolytope==true){
			sliceplanestatus[0]=0; sliceplanestatus[1]=0; sliceplanestatus[2]=0; sliceplanestatus[3]=0; sliceplanestatus[4]=0; sliceplanestatus[5]=0;
			slicedPolytope=false;
		}else{
			sliceplanestatus[0]=1; sliceplanestatus[1]=1; sliceplanestatus[2]=1; sliceplanestatus[3]=1; sliceplanestatus[4]=1; sliceplanestatus[5]=1;
			slicedPolytope=true;
		}
		break;


	case 49:
		if(sliceplanestatus[0]==1){
			sliceplanestatus[0]=0;
		}else{
			sliceplanestatus[0]=1; 
		}
		break;
	case 50:
		if(sliceplanestatus[1]==1){
			sliceplanestatus[1]=0;
		}else{
			sliceplanestatus[1]=1; 
		}
		break;
	case 51:
		if(sliceplanestatus[2]==1){
			sliceplanestatus[2]=0;
		}else{
			sliceplanestatus[2]=1; 
		}
		break;
	case 52:
		if(sliceplanestatus[3]==1){
			sliceplanestatus[3]=0;
		}else{
			sliceplanestatus[3]=1; 
		}
		break;
	case 53:
		if(sliceplanestatus[4]==1){
			sliceplanestatus[4]=0;
		}else{
			sliceplanestatus[4]=1; 
		}
		break;
	case 54:
		if(sliceplanestatus[5]==1){
			sliceplanestatus[5]=0;
		}else{
			sliceplanestatus[5]=1; 
		}
		break;

	////////////// adjusting slicing plane equationselectedindex
	case 113:
		eqn[equationselectedindex][0]-=0.1; break;
	case 119:
		eqn[equationselectedindex][1]-=0.1; break;
	case 101:
		eqn[equationselectedindex][2]-=0.1; break;
	case 114:
		eqn[equationselectedindex][3]-=0.1; break;
	case 81:
		eqn[equationselectedindex][0]+=0.1; break;
	case 87:
		eqn[equationselectedindex][1]+=0.1; break;
	case 69:
		eqn[equationselectedindex][2]+=0.1; break;
	case 82:
		eqn[equationselectedindex][3]+=0.1; break;

	////////// gapbetweenslabs
	case 60:
		gapbetweenslabs-=0.01; break;
	case 62:
		gapbetweenslabs+=0.01; break;

	////////////// slabs transperency
	case 44:
		slabstransperency-=0.05; break;
	case 46:
		slabstransperency+=0.05; break;

	/////////////// Particles
	case 112 : 
		if(showParticles==true){
			showParticles=false;
		}else{
			showParticles=true;
		}
		break;
	case 91 :
		particlesize-=0.1; break;
	case 93 :
		particlesize+=0.1; break;

	/////////////// slabs
	case 104 :
		if(hideSlabs==true){
			xslabs=1; yslabs=1; zslabs=1; 
			hideSlabs=false;
		}else{
			xslabs=0; yslabs=0; zslabs=0; 
			hideSlabs=true;
		}
		break;

	case 120 :
		if(xslabs==1){
			xslabs=0;
		}else{
			xslabs=1; 
		}
		break;
	case 121 :
		if(yslabs==1){
			yslabs=0;
		}else{
			yslabs=1; 
		}
		break;
	case 122 :
		if(zslabs==1){
			zslabs=0;
		}else{
			zslabs=1; 
		}
		break;

	/////////// update and reset
	case 117 : 
		if(updatedPointColor==false){
			updatedPointColor=true;
			updatePointsColor();
		}else{
			updatedPointColor=false;
			resetPointsColor();
		}
		break;


	/////////// escape
	case 27 : 
		exit(1); break;
	}
}

void specialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else
		{
			glutReshapeWindow(500, 500);
			glutPositionWindow(50, 50);
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

		glutPostRedisplay();
	}
}


void populatePoints(){
	srand(time(NULL));
	for(int i=0; i<NOP; i++){
		pointsX[i]=2.0f*((float)rand()/(float)RAND_MAX)-1.0f;
		pointsY[i]=2.0f*((float)rand()/(float)RAND_MAX)-1.0f;
		pointsZ[i]=2.0f*((float)rand()/(float)RAND_MAX)-1.0f;
		pointsC[i]=0.0f;
	}

}


int main(int argc, char *argv[])
{
	populatePoints();

	glutInit(&argc, argv);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	
	glutCreateWindow("Polytope");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	if (!init())
		return 1;

	glutMainLoop();

	return 0;
}
