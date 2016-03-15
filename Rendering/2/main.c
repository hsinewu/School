/*
	4101056014 吳信億 第2次作業4/10
 */

//整個改了很多 結果也碰到很多上次沒碰到的問題 = =||
#include <gl\glew.h>
#include <gl\glut.h>

#include <math.h>
//#include <stdio.h>

#define STILL 0
#define AX 1
#define AZ 2
#define EY 3

#define KEY1 0
#define KEY2 1
#define KEY3 2
#define KEY4 3

int flag = STILL;
int key  = KEY1;

float ang=0;


void draw_e(){
	static GLint eVertex[] ={10,0,	30,0,	30,5,	10,5,/**/30,5,	30,15,	25,15,	25,5/**/
						,25,15,	5,15,	5,10,	25,10/**/,5,10,	5,-10,	10,-10,	10,10/**/
						,10,-10,25,-10,	25,-5,	10,-5};
	
	static GLubyte eindice1 []={0,1,2,3};
	static GLubyte eindice2 []={4,5,6,7};
	static GLubyte eindice3 []={8,9,10,11};
	static GLubyte eindice4 []={12,13,14,15};
	static GLubyte eindice5 []={16,17,18,19};

	static GLint eidMuDraws[5]={0,4,8,12,16};

	static GLsizei count[5]={4,4,4,4,4};
	static GLvoid *indiceArr[5]={eindice1,eindice2,eindice3,eindice4,eindice5};
	int i;

	glVertexPointer (2, GL_INT, 0, eVertex);
	
	glLoadIdentity();
	glTranslatef(100,250,0);
	if(flag==AX){
		glRotatef(ang,1.0f,0.0f,0.0f);
	}
	else if(flag==AZ){
		glRotatef(ang,0.0,0.0f,1.0f);
	}
	else if(flag==EY){
		glLoadIdentity();
		glTranslatef(100,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(1.0f,0.0f,0.0f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
		glDrawArrays(GL_POLYGON,12,4);
		glDrawArrays(GL_POLYGON,16,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, eindice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, eindice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, eindice3);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, eindice4);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, eindice5);
	}else if(key == KEY3){
	//例外發生處
	//glMultiDrawArrays(GL_POLYGON,eidMuDraws,count,5);

	//照講義說的 MultiDrawArrays應該等值於下面這段 可是我用MultiDraw就會發生例外 不解
	for (i=0; i<5; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, eidMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,5);
	}
	
}
void draw_C()
{
	static GLint cVertex[] ={25,35,	5,35,	5,30,	25,30,/**/5,30,	5,-10,	10,-10,	10,30/**/
						,10,-10,	25,-10,	25,-5,	10,-5};

	static GLubyte cindice1 []={0,1,2,3};
	static GLubyte cindice2 []={4,5,6,7};
	static GLubyte cindice3 []={8,9,10,11};

	static GLint cidMuDraws[3]={0,4,8};

	static GLsizei count[3]={4,4,4};
	static GLvoid *indiceArr[3]={cindice1,cindice2,cindice3};
	int i;

	glVertexPointer (2, GL_INT, 0, cVertex);

	glTranslatef(50,0,0);
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(150,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(0.0f,1.0f,0.0f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, cindice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, cindice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, cindice3);
	}else if(key == KEY3){
	//glMultiDrawArrays(GL_POLYGON,eidMuDraws,count,3);
	for (i=0; i<3; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, cidMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,3);
	}
}

void draw_a()
{
	static GLint vertices[] ={20,20,	0,20,	0,15,	20,15,/**/0,15,	0,-10,	5,-10,	5,15/**/
						,0,-10,	10,-10,	10,-5,	0,-5/**/,10,-10,	20,15,	15,20,	5,-5/**/
						,18,25,	23,25,	23,-10,	18,-10};
	
	static GLubyte indice1 []={0,1,2,3};
	static GLubyte indice2 []={4,5,6,7};
	static GLubyte indice3 []={8,9,10,11};
	static GLubyte indice4 []={12,13,14,15};
	static GLubyte indice5 []={16,17,18,19};

	static GLint idMuDraws[5]={0,4,8,12,16};

	static GLsizei count[5]={4,4,4,4,4};
	static GLvoid *indiceArr[5]={indice1,indice2,indice3,indice4,indice5};
	int i;

	glVertexPointer (2, GL_INT, 0, vertices);

	glTranslatef(50,0,0);
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(200,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(0.0f,0.0f,1.0f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
		glDrawArrays(GL_POLYGON,12,4);
		glDrawArrays(GL_POLYGON,16,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice3);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice4);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice5);
	}else if(key == KEY3){
	//glMultiDrawArrays(GL_POLYGON,idMuDraws,count,5);
	for (i=0; i<5; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, idMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,5);
	}
}

void draw_m()
{
	static GLint vertices[] ={0,25,	5,25,	5,-10,	0,-10,/**/5,20,	30,20,	30,15,	5,15/**/
						,15,20,	20,20,	20,-10,	15,-10/**/,30,20,	35,20,	35,-10,	30,-10};
	
	static GLubyte indice1 []={0,1,2,3};
	static GLubyte indice2 []={4,5,6,7};
	static GLubyte indice3 []={8,9,10,11};
	static GLubyte indice4 []={12,13,14,15};

	static GLint idMuDraws[4]={0,4,8,12};

	static GLsizei count[4]={4,4,4,4};
	static GLvoid *indiceArr[4]={indice1,indice2,indice3,indice4};
	int i;

	glVertexPointer (2, GL_INT, 0, vertices);

	glTranslatef(50,0,0);
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(250,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(1.0f,1.0f,1.0f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
		glDrawArrays(GL_POLYGON,12,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice3);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice4);
	}else if(key == KEY3){
	//glMultiDrawArrays(GL_POLYGON,idMuDraws,count,5);
	for (i=0; i<4; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, idMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,5);
	}
}

void draw_p()
{
	static GLint vertices[] ={0,25,	5,25,	5,-15,	0,-15,/**/5,20,	20,20,	20,15,	5,15/**/
						,15,20,	20,20,	20,10,	15,10/**/,20,10,	0,10,	0,5,	20,5};
	
	static GLubyte indice1 []={0,1,2,3};
	static GLubyte indice2 []={4,5,6,7};
	static GLubyte indice3 []={8,9,10,11};
	static GLubyte indice4 []={12,13,14,15};

	static GLint idMuDraws[4]={0,4,8,12};

	static GLsizei count[4]={4,4,4,4};
	static GLvoid *indiceArr[4]={indice1,indice2,indice3,indice4};
	int i;

	glVertexPointer (2, GL_INT, 0, vertices);

	glTranslatef(50,0,0);
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(300,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(0.0f,1.0f,1.0f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
		glDrawArrays(GL_POLYGON,12,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice3);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice4);
	}else if(key == KEY3){
	//glMultiDrawArrays(GL_POLYGON,idMuDraws,count,4);
	for (i=0; i<4; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, idMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,4);
	}
}

void draw_u()
{
	static GLint vertices[] ={0,25,	5,25,	5,-10,	0,-10,/**/5,-10,	20,-10,	20,-5,	5,-5/**/
						,20,20,	25,20,	25,-10,	20,-10};
	
	static GLubyte indice1 []={0,1,2,3};
	static GLubyte indice2 []={4,5,6,7};
	static GLubyte indice3 []={8,9,10,11};

	static GLint idMuDraws[3]={0,4,8};

	static GLsizei count[3]={4,4,4};
	static GLvoid *indiceArr[4]={indice1,indice2,indice3};
	int i;

	glVertexPointer (2, GL_INT, 0, vertices);

	glTranslatef(50,0,0);
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(350,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(1.0f,0.0f,1.0f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice3);
	}else if(key == KEY3){
	//glMultiDrawArrays(GL_POLYGON,idMuDraws,count,3);
	for (i=0; i<3; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, idMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,3);
	}
}

void draw_s()
{
	static GLint vertices[] ={0,25,	7,18,	0,13,	-14,13,/**/-14,13,	0,13,	14,2,	0,2/**/
						,14,2,	0,2,	7,-4,	-7,-10};
	
	static GLubyte indice1 []={0,1,2,3};
	static GLubyte indice2 []={4,5,6,7};
	static GLubyte indice3 []={8,9,10,11};

	static GLint idMuDraws[3]={0,4,8};

	static GLsizei count[3]={4,4,4};
	static GLvoid *indiceArr[4]={indice1,indice2,indice3};
	int i;

	glVertexPointer (2, GL_INT, 0, vertices);

	glTranslatef(50,0,0);
	if(flag==EY){
		glLoadIdentity();
		glTranslatef(400,250,0);
		glRotatef(ang,0.0,1.0f,0.0f);
	}
	glColor3f(1.0f,0.5f,0.5f);

	if(key == KEY1){
		glDrawArrays(GL_POLYGON,0,4);
		glDrawArrays(GL_POLYGON,4,4);
		glDrawArrays(GL_POLYGON,8,4);
	}else if(key == KEY2){
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice1);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice2);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, indice3);
	}else if(key == KEY3){
	//glMultiDrawArrays(GL_POLYGON,idMuDraws,count,3);
	for (i=0; i<3; i++)	if(count[i]>0)
		glDrawArrays(GL_POLYGON, idMuDraws[i], count[i]);

	}else if(key == KEY4){
	//glMultiDrawElements(GL_POLYGON, count,GL_UNSIGNED_BYTE,indiceArr,3);
	}
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

void reshape(int w,int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
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

void keyboard(unsigned char k, int x, int y)
{
	//printf("%d",k);
	switch(k){
	case 49:
		key=KEY1;
		//printf("KEY!");
		break;
	case 50:
		key=KEY2;
		break;
	case 51:
		key=KEY3;
		break;
	case 52:
		key=KEY4;
		break;
	default:
		;
	}
}

void idle()
{
	ang+=0.05f;
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
   
    
   glEnableClientState (GL_VERTEX_ARRAY);


   
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
   glutKeyboardFunc(keyboard);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}
