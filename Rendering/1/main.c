/*
	4101056014 吳信億 第1次作業3/26
 */
#include <gl\glew.h>
#include <gl\glut.h>

#include <math.h>
//#include <stdio.h>

#define P(x,y) glVertex3f( ( (float)x )/10 -0.25f,(float)y/10,0.0);//move 0.25f left so 'm' is centered, remember that x and y are scaled
#define T glTranslatef(0.5f,0.5f,0.0f);

#define STILL 0
#define AX 1
#define AZ 2
#define EY 3

int flag = STILL;

float ang=0;

void draw_e(){
	//Modify each local baseX in these brother methods can move the letter
	float baseX=0.0;
	glTranslatef(0.5f,0.5f,0.5f);//center the string, all subsequent draws share this 
	switch(flag)
	{
	case STILL:
		break;
	case AX:
		glRotatef(ang,1.0f,0.0f,0.0f);
		break;
	case AZ:
		glRotatef(ang,0.0,0.0f,1.0f);
		break;
	case EY:
		//do this latter
		
		break;
	}
	if(flag==EY){
		glTranslatef(-0.21f,0.0f,0.0f);
		baseX=2.5f-0.3f;				//so that after preprocessing, P macro place it at orgin
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	//Beginning strokes
	glBegin(GL_POLYGON);
		glColor3f(1.0f,0.0f,0.0f);
		P(0.2 +baseX,0.0);
		P(0.6 +baseX,0.0);
		P(0.6 +baseX,0.1);
		P(0.2 +baseX,0.1);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.6 +baseX,0.1);
		P(0.6 +baseX,0.3);
		P(0.5 +baseX,0.3);
		P(0.5 +baseX,0.1);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.5 +baseX,0.3);
		P(0.1 +baseX,0.3);
		P(0.1 +baseX,0.2);
		P(0.5 +baseX,0.2);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.1 +baseX,0.2);
		P(0.1 +baseX,-0.2);
		P(0.2 +baseX,-0.2);
		P(0.2 +baseX,0.2);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.2 +baseX,-0.2);
		P(0.5 +baseX,-0.2);
		P(0.5 +baseX,-0.1);
		P(0.2 +baseX,-0.1);
	glEnd();
	//End strokes
}
void draw_C()
{
	float baseX=0.75;
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(0.5f-0.21f+0.075f,0.5f,0.0f);
		baseX=2.5f-0.4f;
		glRotatef(ang,0.0,1.0f,0.0f);
	}//printf("%f",baseX/10 -0.25f);

	glBegin(GL_POLYGON);
		glColor3f(0.0f,1.0f,0.0f);
		P(0.5 +baseX,0.7);
		P(0.1 +baseX,0.7);
		P(0.1 +baseX,0.6);
		P(0.5 +baseX,0.6);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.1 +baseX,0.6);
		P(0.1 +baseX,-0.2);
		P(0.2 +baseX,-0.2);
		P(0.2 +baseX,0.6);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.2 +baseX,-0.2);
		P(0.5 +baseX,-0.2);
		P(0.5 +baseX,-0.1);
		P(0.2 +baseX,-0.1);
	glEnd();
}

void draw_a()
{
	float baseX=1.5;
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(0.5f-0.21f+0.15f,0.5f,0.0f);
		baseX=2.5f-0.2f;
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glBegin(GL_POLYGON);
		glColor3f(0.0f,0.0f,1.0f);
		P(0.4 +baseX,0.4);
		P(0.0 +baseX,0.4);
		P(0.0 +baseX,0.3);
		P(0.4 +baseX,0.3);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.0 +baseX,0.3);
		P(0.0 +baseX,-0.2);
		P(0.1 +baseX,-0.2);
		P(0.1 +baseX,0.3);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.0 +baseX,-0.2);
		P(0.2 +baseX,-0.2);
		P(0.2 +baseX,-0.1);
		P(0.0 +baseX,-0.1);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.2 +baseX,-0.2);
		P(0.4 +baseX,0.3);
		P(0.3 +baseX,0.4);
		P(0.1 +baseX,-0.1);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.35 +baseX,0.5);
		P(0.45 +baseX,0.5);
		P(0.45 +baseX,-0.2);
		P(0.35 +baseX,-0.2);
	glEnd();
}

void draw_m()
{
	float baseX=2.25;
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(0.5f-0.21f+0.225f,0.5f,0.0f);
		baseX=2.5f-0.4f;
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glBegin(GL_POLYGON);
		glColor3f(1.0f,1.0f,1.0f);
		P(0.0 +baseX,0.5);
		P(0.1 +baseX,0.5);
		P(0.1 +baseX,-0.2);
		P(0.0 +baseX,-0.2);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.1 +baseX,0.4);
		P(0.6 +baseX,0.4);
		P(0.6 +baseX,0.3);
		P(0.1 +baseX,0.3);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.3 +baseX,0.4);
		P(0.4 +baseX,0.4);
		P(0.4 +baseX,-0.2);
		P(0.3 +baseX,-0.2);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.6 +baseX,0.4);
		P(0.7 +baseX,0.4);
		P(0.7 +baseX,-0.2);
		P(0.6 +baseX,-0.2);
	glEnd();
}

void draw_p()
{
	float baseX=3.25;
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(0.5f-0.21f+0.325f,0.5f,0.0f);
		baseX=2.5f-0.2f;
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glBegin(GL_POLYGON);
		glColor3f(0.0f,1.0f,1.0f);
		P(0.0 +baseX,0.5);
		P(0.1 +baseX,0.5);
		P(0.1 +baseX,-0.3);
		P(0.0 +baseX,-0.3);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.1 +baseX,0.4);
		P(0.4 +baseX,0.4);
		P(0.4 +baseX,0.3);
		P(0.1 +baseX,0.3);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.3 +baseX,0.4);
		P(0.4 +baseX,0.4);
		P(0.4 +baseX,0.2);
		P(0.3 +baseX,0.2);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.4 +baseX,0.2);
		P(0.0 +baseX,0.2);
		P(0.0 +baseX,0.1);
		P(0.4 +baseX,0.1);
	glEnd();
}

void draw_u()
{
	float baseX=4.0;
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(0.5f-0.21f+0.40f,0.5f,0.0f);
		baseX=2.5f-0.2f;
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glBegin(GL_POLYGON);
		glColor3f(1.0f,0.0f,1.0f);
		P(0.0 +baseX,0.5);
		P(0.1 +baseX,0.5);
		P(0.1 +baseX,-0.2);
		P(0.0 +baseX,-0.2);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.1 +baseX,-0.2);
		P(0.4 +baseX,-0.2);
		P(0.4 +baseX,-0.1);
		P(0.1 +baseX,-0.1);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.4 +baseX,0.4);
		P(0.5 +baseX,0.4);
		P(0.5 +baseX,-0.2);
		P(0.4 +baseX,-0.2);
	glEnd();
}

void draw_s()
{
	float baseX=5.0;
	float r=((float)rand())/32768,g=((float)rand())/32768,b=(float)rand()/32768; //don't know why when placing this after following if statement, the program crashed
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(0.5f-0.21f+0.5f,0.5f,0.0f);
		baseX=2.5f;
		glRotatef(ang,0.0,1.0f,0.0f);
	}

	
	//printf("%f %f %f",r,g,b);
	
	glBegin(GL_POLYGON);
		glColor3f(r,g,b);
		P(0.0 +baseX,0.5);
		P(0.14 +baseX,0.36);
		P(0.0 +baseX,0.27);
		P(-0.28 +baseX,0.27);
	glEnd();
	glBegin(GL_POLYGON);
		P(-0.28 +baseX,0.27);
		P(0.0 +baseX,0.27);
		P(0.28 +baseX,0.03);
		P(0.0 +baseX,0.03);
	glEnd();
	glBegin(GL_POLYGON);
		P(0.28 +baseX,0.03);
		P(0.0 +baseX,0.03);
		P(-0.14 +baseX,-0.06);
		P(0.0 +baseX,-0.2);
	glEnd();
}

void display(void)
{
/* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

/* draw white polygon (rectangle) with corners at
 * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)  
 */
   glColor3f (1.0, 1.0, 1.0);
   draw_e();
   draw_C();
   draw_a();
   draw_m();
   draw_p();
   draw_u();
   draw_s();
   glutSwapBuffers();
 // glFlush ();
}

void mouse(int button, int state, int x, int y)
{
	switch(button){
		case GLUT_LEFT_BUTTON:
			//rotate whole word by z
			//if statement to ensure angle won'n be reset when direction isn't really changed
			if(flag!=AZ){
				flag=AZ;ang=0.0;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			//rotate each letters by y
			if(flag!=EY){
			flag=EY;ang=0.0;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			//rotate whole word by x
			if(flag!=AX){
			flag=AX;ang=0.0;
			}
			break;
	}
}

void idle()
{
	ang+=0.1;
	if(ang>360) ang=0;
	//printf("ang = %d\n",ang);
	//glLoadIdentity();
	
	
	display();
}

void init (void) 
{
/* select clearing color 	*/
   glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

/* 
 * Declare initial window size, position, and display mode
 * (single buffer and RGBA).  Open window with "hello"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics.
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("hello");
   init ();
   
   glutDisplayFunc(display); 
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
