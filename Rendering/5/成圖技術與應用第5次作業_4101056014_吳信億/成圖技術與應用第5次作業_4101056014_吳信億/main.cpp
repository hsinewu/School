/*
	4101056014 吳信億 第5次作業6/11
 */
#include "stdafx.h"

//notes:放model進去結果都看不到東西，只好用自己拉的代替一下
//notes:有記錄時間了，只要跑的動那動畫速度應該都是一樣的，不會再因為計算速度而變

#include <gl\glew.h>
#include <gl\glut.h>
#include <cmath>
#include <ctime>
#include <cstdio>

#include "Model_OBJ.h"

#define JUMP_MAX 8
#define PI (3.1416)
#define WH 8		//wall height

#define LEFT_HAND 0
#define RIGHT_HAND 1

#define PLAYER_POWER 40
#define MON_LIFE 100

Model_OBJ obj;
//define structure
typedef struct monster{
	double x;
	double z;
	double armAng;
	double ang;	/*face direction, in radian
				ang 	direction(original cam view)
				0		left
				PI/2	forward
				PI		right
				3*PI/2	backward*/
	int hp;
}mon;

//define variables
double camX=0.0,camZ=0.0;
const double camY=3.0;
double jumpAngle=PI;
double camAngle=PI/2;
double range = 5.1;
int useHand = LEFT_HAND;
mon mon_arr[3];
int player_new_attack = 0;
int p1_hp = 100;

int i=0;	//can't define loop variable within for - -

static GLuint texName;

/*
**function relationship**
main
	init
		initSprite
		initLight
		initFog
		initTex
	glutMainLoop

**call backs**
display
	scene
		draw_wall
	sprite
		draw_teamon
	player_shoot
reshape
keyboard
idle
mouse
*/

void initFog(){
   GLfloat fogColor[4] = { .5, .5, .5, 1.0};
   glFogi (GL_FOG_MODE, GL_EXP);
   glFogfv (GL_FOG_COLOR, fogColor);
   glFogf (GL_FOG_DENSITY, 0.15);
   glClearColor(0.5, 0.5, 0.5, 1.0);
}

void initTex(){
   const int w = 64 ,h = 64;
   static GLubyte checkImage[h][w][4];
   int i, j, c;
   
   for (i = 0; i < h; i++) {
      for (j = 0; j < w; j++) {
         c = ( ( ( (i&0x4) ==0 ) ^ ( (j&0x4) ) == 0 ) )*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

}

//set up lights
void initLight() 
{
   GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { .0, -1.0, .0, 1 };

   glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
}
//set up monsters
void initSprite(){
	const double posX[3]={2,4,-6};
	const double posZ[3]={4,8,3};
	const double armAngs[3]={0,60,120};
	for(i=0; i<3; ++i){
		mon_arr[i].x = posX[i];
		mon_arr[i].z = posZ[i];
		mon_arr[i].armAng = armAngs[i];
		mon_arr[i].hp = MON_LIFE;
		mon_arr[i].ang = PI/2;
	}
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnableClientState (GL_VERTEX_ARRAY);
   //glEnableClientState (GL_NORMAL_ARRAY);
   glEnable (GL_DEPTH_TEST);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_FLAT);
   initLight();
   initSprite();
   initFog();
   initTex();
}

void draw_teamon(mon* m){
	const double dArmsAng=120.0;
	const double scale = 0.3;
	const double len = 1.0 * scale;

	glPushMatrix();
		glTranslatef(m->x,1,m->z);	//put monster there

		glPushMatrix();
			glRotatef(-((camAngle-(PI/2)) / PI) * 180,0,1,0);
			//monster life-bar
			glDisable(GL_LIGHTING);
			glColor3f(1,0,0);
			glBegin(GL_QUADS);
				glVertex2f(.5,	1);
				glVertex2f(.5-m->hp /(double)MON_LIFE,	1);
				glVertex2f(.5-m->hp /(double)MON_LIFE,	1.2);
				glVertex2f(.5,	1.2);
			glEnd();
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glRotatef( -(m->ang/PI*180) -90 ,0,1,0);	//face direction >>PI<<
		glScalef(0.5,0.5,0.5);
		obj.Draw();
		/*
		//teapot body
		glRotatef(90,0,1,0);
		glutSolidTeapot(3*scale);
		glRotatef(-90,0,1,0);
		
		glRotatef(m->armAng,1,0,0);	//rotate both arms
		//arm1
		glRotatef(-90,1,0,0);
		glRotatef(-60,0,1,0);
		glTranslatef(0,0,len);
		glutSolidCone(1*scale,6*scale,300,300);
		glTranslatef(0,0,-len);
		
		//arm2
		glRotatef(dArmsAng,0,1,0);
		glTranslatef(0,0,len);
		glutSolidCone(1*scale,6*scale,300,300);*/
		
	glPopMatrix();
}

void draw_wall(int x,int z,int xlen,int zlen){
	GLdouble vertices[] = { x, 0,z,	x+xlen, 0,z,	x+xlen, 0,z+zlen,  x, 0,z+zlen,
						 x,WH,z,	x+xlen,WH,z,	x+xlen,WH,z+zlen,  x,WH,z+zlen
	};
	GLubyte faceNear[] = {0,1,5,4};
	GLubyte faceFar[] = {2,3,7,6};
	GLubyte faceLeft[] = {3,0,4,7};
	GLubyte faceRight[] = {1,2,6,5};
	/*
	GLvoid *idArr[4]={faceNear,faceFar,faceLeft,faceRight};
	GLsizei count[]={4,4,4,4};*/
	/*
	GLint normals[]={0,0,-1, //0,0,-1, 0,0,-1, 0,0,-1, 
					0,0,1, //0,0,1, 0,0,1, 0,0,1, 
					-1,0,0, //-1,0,0, -1,0,0, -1,0,0, 
					1,0,0, //1,0,0, 1,0,0, 1,0,0
	};*/

	//glNormalPointer(GL_INT,0,normals);
	glVertexPointer (3, GL_DOUBLE, 0, vertices);

	glNormal3f(0,0,-1);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceNear);
	glNormal3f(0,0,1);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceFar);
	glNormal3f(-1,0,0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceLeft);
	glNormal3f(1,0,0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceRight);
	
	//glMultiDrawElements(GL_QUADS, count,GL_UNSIGNED_BYTE,idArr,4);

}

int inRoom(double x,double z){
	//return 1;
	if(x>8 || x<-8)
		return 0;
	if(z>8 || z<-8)
		return 0;
	return 1;
}

int wall_test(double x,double z){	//return 0 if it's blocked,1 if not blocked
#define BLOCK_WALL(wx,wz,lx,lz) if(x>wx && x<wx+lx && z>wz && z<wz+lz) return 0;

	BLOCK_WALL(-8,5,9,1)
	BLOCK_WALL(5,3,1,4)
	BLOCK_WALL(-5,-8,10,1)
	BLOCK_WALL(3,-6,2,2)
	BLOCK_WALL(-8,-6,1,5)
	return 1;
}

//draw scene
void scene(){	//for room and walls
   GLfloat mat_r[] = { 1.0, .0, .0, 1.0 };
   GLfloat mat_g[] = { .0, 1.0, .0, 1.0 };
   GLfloat mat_p[] = { .0, .0, 1.0, 1.0 };
   GLfloat mat_b[] = { 1.0, .0, 1.0, 1.0 };
   GLfloat mat_y[] = { 1.0, 1.0, .0, 1.0 };

   //glColor3f (1.0, 1.0, 1.0);
   glPushMatrix();
	   glTranslatef(0,8,0);
	   
	   glutWireCube (16.0);		//my room
	   glTranslatef(0,-8,0);
	   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_r);
	   //glColor3f(1.0,0.0,0.0);	//red wall
	   draw_wall(-8,5,9,1);
	   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_g);
	   //glColor3f(0.0,1.0,0.0);	//green wall
	   draw_wall(5,3,1,4);
	   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_p);
	   //glColor3f(1.0,0.0,1.0);	//purple wall
	   draw_wall(3,-6,2,2);
	   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_b);
	   //glColor3f(0.0,0.0,1.0);	//blue wall
	   draw_wall(-5,-8,10,1);
	   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_y);
	   //glColor3f(1.0,1.0,0.0);	//yellow wall
	   draw_wall(-8,-1,1,-5);
   glPopMatrix();
   //texture floor
   glEnable(GL_TEXTURE_2D);
	   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	   glBindTexture(GL_TEXTURE_2D, texName);
	   glBegin(GL_QUADS);
		   glTexCoord2f(0.0, 0.0); glVertex3f(-8.0, 0,-8.0);
		   glTexCoord2f(0.0, 1.0); glVertex3f(-8.0, 0,8.0);
		   glTexCoord2f(1.0, 1.0); glVertex3f(8.0, 0,8.0);
		   glTexCoord2f(1.0, 0.0); glVertex3f(8.0, 0,-8.0);
	   glEnd();
   glDisable(GL_TEXTURE_2D);
}

int got_hit(mon m){
	return 0;
}

//draw monsters
void sprite(){
   const double step = 0.05;
   double tmpX,tmpZ;
   double tmpAng;
   GLfloat mat_w[] = { 1.0, 1.0, 1.0, 1.0 };
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_w);
   for(i=0; i<3; ++i){
	   if( mon_arr[i].hp <= 0) continue;	//skip it when dead (no drawing, no calculating)
	   mon_arr[i].ang += rand()/32767.0 -0.5;	//todo
	   tmpAng=mon_arr[i].ang;
	   //mon_arr[i].ang = tmpAng = PI;	//testing
	   //printf("ang = %f sin = %f cos = %f\n",tmpAng,cos(tmpAng ),sin(tmpAng));
	   tmpX = mon_arr[i].x + cos(tmpAng ) * step;
	   tmpZ = mon_arr[i].z + sin(tmpAng ) * step;

	   if( inRoom(tmpX,tmpZ) ){
		   mon_arr[i].x = tmpX;
		   mon_arr[i].z = tmpZ;
	   }
	   mon_arr[i].armAng+=30;

	   draw_teamon(&mon_arr[i]);		//display the monster
	   if( got_hit(mon_arr[i]) )
		   p1_hp -=15;
   }
}

int hit(mon m){
	return 1;
}

void player_shoot(){
	const double speed = 0.1;
	const double camTheta = -((camAngle-(PI/2)) / PI) * 180;
	const double deltaHands = (useHand == LEFT_HAND) ? 2 : -2;
	
	if( range > 5)
		return;
	range += speed;
	
	glPushMatrix();
		//改一改發現上次好像把translat跟rotate放反了
		glTranslatef(camX,0,camZ );		//put it in front of me
		glRotatef(camTheta,0,1,0);		//cam angle
		glTranslatef(deltaHands,3,range);	//ajust the position
		glutSolidSphere(0.3,60,30);
	glPopMatrix();

	if(!player_new_attack) return;	//make sure the damage is counted once in a movement(animation)
									//or the monster would lost their life every time the player arm is drawn

	for (i = 0; i<3; ++i)
		if(hit(mon_arr[i])){
			mon_arr[i].hp -= PLAYER_POWER;
			player_new_attack = 0;	//no more damage till next attack
		}
}

void headLight(){
	static int state = 0;
	state = state ^ 1;
	state ?
		glEnable(GL_LIGHT1):
		glDisable(GL_LIGHT1);
}


void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();
   gluLookAt (	camX,				camY+ JUMP_MAX * sin(jumpAngle),	camZ/**/,
				camX+cos(camAngle),	camY+ JUMP_MAX * sin(jumpAngle),	camZ+sin(camAngle)/**/,
				0.0,				1.0,								0.0);
   
   scene();
   sprite();
   player_shoot();
   glFlush ();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum(-0.01,0.01,-0.01,0.01,0.01,20);
   glMatrixMode (GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
	double moveAngle = camAngle;	//camAngle won't be modify here, only the value is borrowed
	double tmpX,tmpZ;
	const double mvRate = 0.1;
   switch (key) {
      case 'W':
	  case 'w':
		  //no need modify
         break;
	  case 'A':
	  case 'a':
		  moveAngle= (moveAngle +3*PI/2);
         break;
	  case 'S':
	  case 's':
		  moveAngle= (moveAngle +PI);
         break;
	  case 'D':
	  case 'd':
		  moveAngle= (moveAngle +PI/2);
         break;
	  case 'F':
	  case 'f':
		  {static int f = 0;
		  f = !f;
		  f ?
			  glEnable(GL_FOG):
			  glDisable(GL_FOG);
		  return;}
	  case 32:		//space,jump
		  if( PI- jumpAngle < 1e-6)	//so that no jumping when still falling
			jumpAngle=0.0;
         return;	//since camX,Z sholud not be modify when I press space
	  case '1':
		  if(range > 5)
		  {
			  range = 0;
			  useHand = LEFT_HAND;
			  player_new_attack = 1;
		  }
		  return;
	  case '2':
		  if(range > 5)
		  {
			  range = 0;
			  useHand = RIGHT_HAND;
			  player_new_attack = 1;
		  }
		  return;
	  case 'L':
	  case 'l':
		  headLight();
		  return;
   }

   if(moveAngle>2*PI)moveAngle-=2*PI;
   
   tmpX = camX + mvRate*cos(moveAngle);
   tmpZ = camZ + mvRate*sin(moveAngle);
   if(inRoom(tmpX,tmpZ) && wall_test(tmpX,tmpZ) ){
   camX=tmpX;	camZ=tmpZ;	//apply the movement
   }
}

void idle()
{	
	static int t = 0;
	if( clock()-t >10){
		t = clock();
		if(jumpAngle<PI)
			jumpAngle+=0.1;
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y){
		switch(button){
		case GLUT_LEFT_BUTTON:
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
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   obj.Load("cubes.obj");
   //換成c++下面這行就掛了,LINK2019
   //glewInit();

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
