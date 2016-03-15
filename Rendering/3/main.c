/*
	4101056014 吳信億 第3次作業4/24
 */

//note:只有z方向的撞牆可以
#include <gl\glew.h>
#include <gl\glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define JUMP_MAX 8
#define PI 3.1416

double camX=0.0,camZ=0.0;
double jumpAngle=PI;
double camAngle=PI/2;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnableClientState (GL_VERTEX_ARRAY);	//wtf..
}

#define WH 15		//wall height

void draw_wall(int x,int z,int xlen,int zlen){
	GLint vertices[] = { x, 0,z,	x+xlen, 0,z,	x+xlen, 0,z+zlen,  x, 0,z+zlen,
						 x,WH,z,	x+xlen,WH,z,	x+xlen,WH,z+zlen,  x,WH,z+zlen
	};
	GLubyte faceNear[] = {0,1,5,4};
	GLubyte faceFar[] = {2,3,7,6};
	GLubyte faceLeft[] = {0,3,7,4};
	GLubyte faceRight[] = {1,2,6,5};

	GLvoid *idArr[4]={faceNear,faceFar,faceLeft,faceRight};
	GLsizei count[]={4,4,4,4};

	glVertexPointer (3, GL_INT, 0, vertices);
	
	glMultiDrawElements(GL_QUADS, count,GL_UNSIGNED_BYTE,idArr,4);

}

int inRoom(double x,double z){
	if(x>8 || x<-8)
		return 0;
	if(z>8 || z<-8)
		return 0;
	return 1;
}

int wall_test(double x,double z){	//return 0 if it's blocked,1 if not blocked
#define BLOCK_WALL(wx,wz,lx,lz) if(x>wx && x<wx+lx && z>wz && z<wz+lz) return 0;
	BLOCK_WALL(-8,5,9,1)
	BLOCK_WALL(5,7,1,-4)
	BLOCK_WALL(-5,-8,10,1)
	BLOCK_WALL(3,-4,2,-2)
	BLOCK_WALL(-8,-1,1,-5)
	return 1;
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   
   glLoadIdentity ();            
   //printf("%f\n",jumpAngle);
   gluLookAt (camX, JUMP_MAX * sin(jumpAngle), camZ/**/, camX+cos(camAngle), JUMP_MAX * sin(jumpAngle), camZ+sin(camAngle)/**/, 0.0, 1.0, 0.0);
   
   glColor3f (1.0, 1.0, 1.0);
   glutWireCube (16.0);		//my room
   
   glTranslatef(0,-8,0);

   glColor3f(1.0,0.0,0.0);	//red wall
   draw_wall(-8,5,9,1);
   glColor3f(0.0,1.0,0.0);	//green wall
   draw_wall(5,7,1,-4);
   glColor3f(0.0,0.0,1.0);	//blue wall
   draw_wall(-5,-8,10,1);
   glColor3f(1.0,0.0,1.0);	//purple wall
   draw_wall(3,-4,2,-2);
   glColor3f(1.0,1.0,0.0);	//yellow wall
   draw_wall(-8,-1,1,-5);

   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-5.0, 5.0, -5.0, 5.0, 3.0, 10.0);
   glMatrixMode (GL_MODELVIEW);
}

#define LEFTNEAR -5+3*cos(moveAngle),3*sin(moveAngle)
void keyboard(unsigned char key, int x, int y)
{
	double moveAngle = camAngle;	//camAngle won't be modify here, only the value is borrowed
	double tmpX,tmpZ;
	const double mvRate = 0.1;
   switch (key) {
      case 119:		//W,forward
		  //no need modify
         break;
	  case 97:		//A,left
		  moveAngle= (moveAngle +3*PI/2);
		  
         break;
	  case 115:		//S,backward
		  moveAngle= (moveAngle +PI);
         break;
	  case 100:		//D,right
		  moveAngle= (moveAngle +PI/2);
		  
         break;
	  case 32:		//space,jump
		  if( PI- jumpAngle < 1e-6)	//so that no jumping when still falling
			jumpAngle=0.0;
         return;	//since camX,Z sholud not be modify when I press space
   }

   if(moveAngle>2*PI)moveAngle-=2*PI;
   
   tmpX = camX + mvRate*cos(moveAngle);
   tmpZ = camZ + mvRate*sin(moveAngle);
   if(inRoom(tmpX,tmpZ) && wall_test(tmpX+3*cos(camAngle),tmpZ+3*sin(camAngle)) ){	//3 comes from near plane
   camX=tmpX;	camZ=tmpZ;	//apply the movement
   //printf("X:%f, Z:%f\n",tmpX+dx+3*cos(camAngle),tmpZ+3*sin(camAngle));
   //printf("%f\n",cos(camAngle));
   }
}

void idle()
{	
	if(jumpAngle<PI)
		jumpAngle+=0.001;	//modify this if jumping too fast or too slow
	display();
}

void mouse(int button, int state, int x, int y){
		switch(button){
		case GLUT_LEFT_BUTTON:
			//printf("%f",camAngle);
			camAngle-=0.02;
			break;
		case GLUT_RIGHT_BUTTON:
			camAngle+=0.02;
			break;
	}
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glewInit();

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
