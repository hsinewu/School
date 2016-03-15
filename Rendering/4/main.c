/*
	4101056014 吳信億 第4次作業5/15
 */

//note:animation speed depend on the machine
//note:怪死掉會讓計算變快,影響animation
#include <gl\glew.h>
#include <gl\glut.h>
#include <math.h>
//#include <time.h>	//嘆
#include <stdio.h>

#define JUMP_MAX 8
#define PI (3.1416)
#define WH 8		//wall height

#define LEFT_HAND 0
#define RIGHT_HAND 1

#define PLAYER_POWER 40
#define MON_LIFE 100
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
double handsTheta = 90.1;
int useHand = LEFT_HAND;
mon mon_arr[3];
int player_new_attack = 0;
int p1_hp = 100;

int i=0;	//can't define loop variable within for - -

/*
**function relationship**
main
	init
		initSprite
		initLight
	glutMainLoop

**call backs**
display
	scene
		draw_wall
	sprite
		draw_teamon
	player_hands
reshape
keyboard
idle
mouse
*/

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
   initLight();
   initSprite();
}

void draw_teamon(mon* obj){
	const double dArmsAng=120.0;
	const double scale = 0.3;
	const double len = 1.0 * scale;
	//printf("dir = %f\n",direction);
	glPushMatrix();
		glTranslatef(obj->x,1,obj->z);	//put monster there

		glPushMatrix();
			glRotatef(-((camAngle-(PI/2)) / PI) * 180,0,1,0);
			//monster life-bar
			glDisable(GL_LIGHTING);
			glColor3f(1,0,0);
			glBegin(GL_QUADS);
				glVertex2f(.5,	1);
				glVertex2f(.5-obj->hp /(double)MON_LIFE,	1);
				glVertex2f(.5-obj->hp /(double)MON_LIFE,	1.2);
				glVertex2f(.5,	1.2);
			glEnd();
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glRotatef( -(obj->ang/PI*180) -90 ,0,1,0);	//face direction >>PI<<
		//teapot body
		glRotatef(90,0,1,0);
		glutSolidTeapot(3*scale);
		glRotatef(-90,0,1,0);
		
		glRotatef(obj->armAng,1,0,0);	//rotate both arms
		//arm1
		glRotatef(-90,1,0,0);
		glRotatef(-60,0,1,0);
		glTranslatef(0,0,len);
		glutSolidCone(1*scale,6*scale,300,300);
		glTranslatef(0,0,-len);
		
		//arm2
		glRotatef(dArmsAng,0,1,0);
		glTranslatef(0,0,len);
		glutSolidCone(1*scale,6*scale,300,300);
		
	glPopMatrix();
}

void draw_wall(int x,int z,int xlen,int zlen){

	GLint vertices[] = { x, 0,z,	x+xlen, 0,z,	x+xlen, 0,z+zlen,  x, 0,z+zlen,
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
	glVertexPointer (3, GL_INT, 0, vertices);

	glNormal3f(0,0,-1);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceNear);
	glNormal3f(0,0,1);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceFar);
	glNormal3f(-1,0,0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceLeft);
	glNormal3f(1,0,0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, faceRight);
	//嘗試使用normalPointer,發現解讀方式不如預期只好退化回draw
	//glMultiDrawElements(GL_QUADS, count,GL_UNSIGNED_BYTE,idArr,4);

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

void player_hands()
{
	const double rotRate = 5;			//modify this if arm rotating too slow
	const double camTheta = -((camAngle-(PI/2)) / PI) * 180;
	const double deltaHands = (useHand == LEFT_HAND) ? 2 : -2;
	
	if( handsTheta > 90)	//最多彎個90度,然後直接消失
		return;
	handsTheta += rotRate;
	
	glPushMatrix();
		glRotatef(camTheta,0,1,0);		//cam angle
		glTranslatef(camX,0,camZ);		//put it in front of me
		glTranslatef(deltaHands,3,2);	//ajust the position
		glRotatef(handsTheta,1,0,0);
		glPushMatrix();
			//wrist
			glTranslatef(0,0.8,-0.8);
			glTranslatef(0,0,0.8);
			glRotatef(handsTheta,1,0,0);
			glTranslatef(0,0,-0.8);
			glScalef(1,1,4);
			glutSolidSphere(0.3,60,30);
		glPopMatrix();
		glScalef(1,4,1);
		glutSolidSphere(0.3,60,30);	//part that near to shoulder
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
	//printf("state = %d\n",state);
	state ?
		glEnable(GL_LIGHT1),	printf("headlight on\n"):
		(glDisable(GL_LIGHT1),	printf("headlight off\n"));
	//
}


void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //printf("%f\t",rand()/32767.0 -0.5);
   glLoadIdentity ();
   gluLookAt (	camX,				camY+ JUMP_MAX * sin(jumpAngle),	camZ/**/,
				camX+cos(camAngle),	camY+ JUMP_MAX * sin(jumpAngle),	camZ+sin(camAngle)/**/,
				0.0,				1.0,								0.0);
   
   scene();
   sprite();
   player_hands();
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
	printf("%d\n",key);
   switch (key) {
	   //之前 前後左右老是會突然失靈,原來是大小寫(按到caps lock)的問題
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
	  case 32:		//space,jump
		  if( PI- jumpAngle < 1e-6)	//so that no jumping when still falling
			jumpAngle=0.0;
         return;	//since camX,Z sholud not be modify when I press space
	  case '1':
		  if(handsTheta > 90)
		  {
			  handsTheta = 0;
			  useHand = LEFT_HAND;
			  player_new_attack = 1;
		  }
		  return;
	  case '2':
		  if(handsTheta > 90)
		  {
			  handsTheta = 0;
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
	if(jumpAngle<PI)
		jumpAngle+=0.1;			//modify this if jumping too fast or too slow
	display();
	
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