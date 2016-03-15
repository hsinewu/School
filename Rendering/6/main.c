#include <gl\glew.h>
#include <gl\glut.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
/*
variable prefix:
b for bullet
e for enemy
p for player
d for direction
*/

GLuint loadBMP_custom(const char * imagepath);	//located at the end

#define CLOCK (10)
#define GAME_ZONE_WIDTH 100
#define GAME_ZONE_HEIGHT 100

//GAME  STATES
#define GS_MENU 0
#define GS_PLAYING 1
#define GS_H2P 2

int game_state = GS_MENU;

//GAME STRUCTUREs, TYPEDEFs
typedef struct player{
	double x,y;
	int alive;
	int hp;
	int mutiShoot;	//zero means shoot one bullet at a time
} t_p;
typedef struct enemy{
	double x,y;
	int alive;		//may be redundant?
	int toBeCollect;
	int mv;			//index of emTable
	int sht;		//index of estTable
} t_e;
typedef struct bullet{
	double x,y;
	double vx,vy;
	int toBeCollect;
} t_b;

typedef struct pbullet{
	double x,y;
	double vx,vy;
	int toBeCollect;
} t_pb;

typedef void (*fptr_em)(t_e*);
typedef void (*fptr_est)(t_e*);

//PLAYER CHARACTER INFOs
t_p p1 = {50,10,1,10,0};			//p1 is initialized here
int pd[2] = {0,0};

//NON-PLAYERs
#define BPOOL_SIZE 200	//muti shoot不要玩過頭,不然就調高size(不打算設最大值)
#define EPOOL_SIZE 30

t_e es[EPOOL_SIZE];		int ehead = 0, etail = 0;

t_b ebs[BPOOL_SIZE];	int head = 0, tail = 0;

t_pb pbs[BPOOL_SIZE];	int phead = 0, ptail = 0;

#define EM_AMOUNT (2)
#define EST_AMOUNT (3)

fptr_em emTable[EM_AMOUNT] ;	//must be set up manually(at game_init_once)
fptr_est estTable[EST_AMOUNT] ;

//INPUT
int keyState[26];

//EFFECT
struct t_powerup{
	double x,y;		//y is used to judge if it is in game zone
}powerup1 = {50,-5};	//也許還會再加什麼,先struct來放

double bloody = 0;
int shield = 0;		int shdEnergy = 100;	double shdAlpha = 0;
int paused = 0;		//do have some side effect on sync, but not a big problem
double bgScroll = 0;
int cheat_player_hp_no_less_than_1 = 0;

//loop counter -.-
int i,j;

GLuint menuBMP = 0;	//I have some problem drawing string, so I used texture instead
GLuint h2pBMP = 0;
GLuint spaceBMP = 0;
//GLuint eyBMP = 0;

//********* test test test ***********//
void enemy_spawn();void garbage_enemy();

void debug_key(unsigned char k){
   switch (k) {
   case '0':
	   printf("x = %d\n", keyState['x'-'a']);
	   break;
      case '1':
		 bgScroll += 1;	
         break;
	  case '2':
		  enemy_spawn();
		  break;
	  case '3':
		  ++p1.mutiShoot;
		  if( p1.mutiShoot > BPOOL_SIZE/6) 
			printf("warning: too many bullets may overflow the buffer\n");
		  break;
	  case '4':
		  printf("h = %3d, t = %3d\teh = %3d, et = %3d\tph = %3d, pt = %3d\n",head,tail,ehead,etail,phead,ptail);
		  break;
	  case '5':
		  garbage_enemy();
		  break;
	  case '6':
		  printf("pux = %f,puy = %f\n",powerup1.x,powerup1.y);
			  break;
	  case '7':
		  break;
	  case '8':
		  shield = 1;
		  break;
	  case '9':
		  printf("energy = %d\n",shdEnergy);
		  break;
   }
}

//********* some helpful function ***********//
double dist(double x1,double y1,double x2,double y2){
	return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}

double radian(double degree){
	return (degree / 180) * 3.14169 ;
}

int myRand(int noGreaterThanMe){
	int tmp = rand()/(double)RAND_MAX * noGreaterThanMe;
	if(noGreaterThanMe == 0) {printf("Don't pass 0 to myRand");return 0;}
	if(tmp >= noGreaterThanMe || tmp < 0 )		//買個保險
		printf("warning, I'm not expecting this random value\n");
	return tmp;
}

int queueIsFull(int h, int t, int sz){
	if( (t + 1) % sz == h)
		return 1;
	return 0;
}

void queueNext(int *iter,int size){
	if(++(*iter) == size) *iter = 0;
}

int outOfGameZone(int x, int y){
	int v = 5;
	if( x < -v || x > GAME_ZONE_WIDTH + v) return 1;
	if( y < -v || y > GAME_ZONE_HEIGHT + v) return 1;
	return 0;
}

void set_bullet(t_b *eb, double x, double y, double vx, double vy, int o){
	eb->x = x;		eb->y = y;
	eb->vx = vx;	eb->vy = vy;
	eb->toBeCollect = o;
}

int request_bullet(t_b **eb){
	if( queueIsFull(head,tail,BPOOL_SIZE) ){
		printf("Buffer is full, enemies shoots too much\n");
		return 0;	//means request fail
	}
	(*eb) = &ebs[tail];
	queueNext(&tail,BPOOL_SIZE);
	return 1;
}

void circle(int size){
	int ii;
	glBegin(GL_POLYGON);
		for(ii = 0; ii< 20; ++ii){
			double arc = radian(360) /20 * ii;
			glVertex2f( size*cos(arc), size*sin(arc) );
		}
	glEnd();
}

//********************  FUNCTION FOR ""GAME LOGIC"" START  ********************//
void game_init_once();
void game_reset();			//used when restart
void enemy_spawn();
void em_close2px_forwarding(t_e* e);
void em_stay_back_leave_px(t_e *e);
void est_forward(t_e *e);
void est_aiming(t_e *e);
void est_aiming_inaccurate(t_e *e);
void update(int t);
void update_player(int t);
void update_enemy(int t);
void update_bullets(int t);
void update_powerup(int t);
void garbage_bullet();
void garbage_pbullet();
void garbage_enemy();

void game_init_once(){
	srand(time(0));
	
	emTable[0] = em_close2px_forwarding;
	emTable[1] = em_stay_back_leave_px;

	estTable[0] = est_forward;
	estTable[1] = est_aiming;estTable[2] = est_aiming;
	//estTable[2] = est_aiming_inaccurate;
	//estTable[0] = est_aiming_inaccurate;
	//estTable[1] = est_aiming_inaccurate;

	menuBMP = loadBMP_custom("./menu.bmp");
	h2pBMP = loadBMP_custom("./h2p.bmp");
	spaceBMP = loadBMP_custom("./space.bmp");
	
	game_reset();
}

void game_reset(){
	for(i=0; i<BPOOL_SIZE; ++i)	//initialize bullet resources(memory)
	{
		ebs[i].toBeCollect = 0;
		pbs[i].toBeCollect = 0;
	}
	for(i=0 ; i<EPOOL_SIZE; ++i){
		es[i].toBeCollect = 0;
	}
	p1.x = 50; p1.y =10; p1.alive = 1; p1.hp = 10; p1.mutiShoot = 0;
	powerup1.x = 50; powerup1.y = -5;
	shdEnergy = 100; shdAlpha = 0;
	
	head = tail =
		ehead = etail =
		phead = ptail =
		bloody =
		paused =
		cheat_player_hp_no_less_than_1 = 0;
}

void enemy_spawn(){
	//to be implemented
	if(queueIsFull(ehead,etail,EPOOL_SIZE)){
		printf("equeue exploded\n");
		return;
	}
	es[etail].alive = 1;
	es[etail].toBeCollect = 0;
	es[etail].x = myRand(95);
	es[etail].y = 95;
	es[etail].mv = myRand(EM_AMOUNT);
	es[etail].sht = myRand(EST_AMOUNT);
	queueNext(&etail,EPOOL_SIZE);
}

void em_close2px_forwarding(t_e* e){
	e->x > p1.x ?
		--e->x : ++e->x;
	--e->y;
}

void em_stay_back_leave_px(t_e *e){
	int ii = 85;
	e->y = (e->y > ii ? e->y - 1 : e->y);			//greater? Yes,go down. : No, stay.

	if(abs(e->x - p1.x) > 15) return;		//Okay, player is far from me now, don't move.
	e->x = (e->x > p1.x ? e->x + 1 : e->x - 1);		//right to p1? Yes,go further : No,turn left
	if(e->x < 0) e->x = 0;		//don't get out of zone
	if(e->x > 100) e->x = 100;
}

void est_forward(t_e *e){
	t_b *eb = NULL;
	if(	request_bullet(&eb) )
		set_bullet( eb, e->x, e->y -5, 0, -1, 0);
}

void est_aiming(t_e * e){
	t_b *eb = NULL;
	double d = dist(e->x, e->y -10, p1.x, p1.y);

	if(e->y - 5 < p1.y) return;				//It's strange to shoot backward
	if(	request_bullet(&eb) )
		set_bullet( eb, e->x, e->y -5, (p1.x - e->x)/d, (p1.y - (e->y -10))/d, 0);
}

void est_aiming_inaccurate(t_e *e){
	t_b *eb = NULL;
	double d = dist(e->x, e->y -5, p1.x, p1.y);
	int inaccuracy = 30;

	if(e->y - 5 < p1.y) return;
	if(	request_bullet(&eb) ){
		int offset =  myRand(inaccuracy) - inaccuracy/2;	//-15~15
		set_bullet( eb, e->x, e->y -5
				,(p1.x - (e->x + offset ) ) /d		//this is where make it inaccurate
				,(p1.y - (e->y -5))         /d, 0);
	}
}

void player_shoot(/*player is global*/){
	double deg = (p1.mutiShoot) * 15;
	double curDeg = (deg > 150) ? -75 : -deg/2;
	if(deg > 150) deg = 150;
	for( i=0; i<=p1.mutiShoot; ++i){
		if( queueIsFull(phead,ptail,BPOOL_SIZE) ){
			printf("Buffer is full, too many player bullets\n");
			return;
		}
		pbs[ ptail ].x = p1.x;
		pbs[ ptail ].y = p1.y + 4;
		pbs[ ptail ].vx = cos( radian(curDeg+90) );
		pbs[ ptail ].vy = sin( radian(curDeg+90) );
		pbs[ ptail ].toBeCollect = 0;
		queueNext(&ptail,BPOOL_SIZE);
		curDeg += deg/(p1.mutiShoot);
	}
}

//The greater the sync is, the less times it would be performed within an amount of time interval
#define SYNC_SHOOT (50)			//frequency
#define SYNC_PSHOOT (15)
#define SYNC_MOVE (20)		//20
#define SYNC_PMOVE (2)
#define SYNC_BULLET (3)			//speed
#define SYNC_PBULLET (1)
#define SYNC_SPAWN (300)
#define SYNC_POWERUP_MOVE (3)
#define SYNC_POWERUP_SPAWN (300)
#define SYNC_SHD (2)
#define PARAM_BLOODY (0.03)		//fade speed


void update(int sync){
	if(game_state == GS_PLAYING){
		if(!paused){
			bgScroll += 0.2;
			update_player(sync);
			update_enemy(sync);
			update_bullets(sync);
			update_powerup(sync);
			if(shdEnergy < 0) shdEnergy = 0;	//發現碰撞可能誤把能量減到負的,修正它,雖然放這裡怪怪的
		}
	}
}

void update_player(int sync){
	static int pLastShoot = 0;
	static int pLastMove = 0;
	//MOVE
	if( (sync - pLastMove) >= SYNC_PMOVE){
		if(p1.x < -2 && pd[0] == -1 || 
			p1.x > 102 && pd[0] == 1 || 
			p1.y < -2 && pd[1] == -1 ||
			p1.y > 102 && pd[1] == 1)	//so that player can't go out of game zone
			/*do nothing*/ ;
		else{
		p1.x += pd[0];
		p1.y += pd[1];
		pLastMove = sync;
		}
	}
	//ATTACK
	if( (sync - pLastShoot) >= SYNC_PSHOOT && keyState['z'-'a']){
		if( p1.alive )		//no shooting if die
			player_shoot();
		pLastShoot = sync;
	}
	//EFFECT
	if(bloody > 0) bloody -= PARAM_BLOODY;
	if(keyState['x'-'a'] == 1 && shdEnergy == 100) shield = 1;	//##
	if(sync % SYNC_SHD == 0)
		if(shield)
			if(shdEnergy > 0) --shdEnergy;
			else shield = 0;
		else 
			if( shdEnergy < 100) ++shdEnergy;
	if(shield) shdAlpha += 0.1;
}

void update_enemy(int sync){
	if(sync % SYNC_SPAWN == 0 && !queueIsFull(ehead,etail,EPOOL_SIZE) )
		enemy_spawn();
	//for each existing enemy
	for( i=0; i != etail; queueNext(&i,EPOOL_SIZE)){
		t_e *e = &es[i];
		if(e->alive){
			//MOVE
			if(sync % SYNC_MOVE == 0)
				emTable[e->mv](e);
			//ATTACK
			if(sync % SYNC_SHOOT == 0 )
				estTable[e->sht](e);
			//COLISSION
			if(shield){
				if( dist( e->x,e->y,p1.x,p1.y ) < 10 ){
					e->toBeCollect = 1;  e->alive = 0; shdEnergy -= 60;
				}
			}
			else
				if(dist( e->x,e->y,p1.x,p1.y) < 8){
					if( --p1.hp == 0) {p1.alive = 0; paused = 1;}
					bloody = 1.5;
					e->toBeCollect = 1;
					e->alive = 0;
					if(cheat_player_hp_no_less_than_1 && p1.hp == 0){
						p1.hp = 1; p1.alive = 1; paused = 0;
					}
				}
		}
		if(outOfGameZone(e->x,e->y)){
			e->alive = 0;
			e->toBeCollect = 1;
		}
	}
}

void update_bullets(int sync){
	if(sync % SYNC_BULLET == 0)
		for( i=head; i != tail; queueNext(&i,BPOOL_SIZE)){	//for enemy bullets
			t_b *eb = &ebs[i];
			if(eb->toBeCollect) continue;	//ignore bullets not yet collected
			eb->x += eb->vx;
			eb->y += eb->vy;
			//player got hit?
			if(shield){
				if( dist(eb->x, eb->y, p1.x, p1.y) < 7 ){
					eb->toBeCollect = 1;
					shdEnergy -= 10;
				}
			}
			else
				if( dist(eb->x, eb->y, p1.x, p1.y) < 5 ){
					if(--p1.hp == 0)   {	p1.alive = 0; paused = 1;  }
					eb->toBeCollect = 1;
					bloody = 1.5;	//blend the player wtih red later
									//I set it greater than 1 so that the player remains all red for a while(alpha value greater than 1 is truncated)
					if( cheat_player_hp_no_less_than_1 && p1.hp == 0 ) {  p1.hp = 1; p1.alive = 1; paused =0; }
				}
			if( outOfGameZone(eb->x, eb->y) ){
				eb->toBeCollect = 1;
			}
		}
	if(sync % SYNC_PBULLET == 0){
		for(i = phead; i != ptail; queueNext(&i,BPOOL_SIZE) ){
			t_pb *pb = &pbs[i];
			//if(pb->toBeCollect) continue;
			pb->x += pb->vx;
			pb->y += pb->vy;
			//enemy got hit?
			for( j = ehead; j != etail; queueNext(&j,EPOOL_SIZE)){
				t_e *e = &es[j];
				if(e->toBeCollect) continue;
				if( dist(pb->x, pb->y, e->x, e->y) < 5 ){
					e->alive = 0;
					e->toBeCollect = 1;
					pb->toBeCollect = 1;	//tag it
				}
			}
			if( outOfGameZone(pb->x, pb->y) ){
				pb->toBeCollect = 1;
				
			}
		}
	}
	garbage_pbullet();
	garbage_bullet();
	garbage_enemy();	//I put it here cuz this is where enemy set to  dead(possibly)
}

void update_powerup(int sync){
	if( powerup1.y <= -5 && sync % SYNC_POWERUP_SPAWN == 0) {
			powerup1.x = myRand(100);
			powerup1.y = 105;	//put at top
		}
	else{
		if(sync % SYNC_POWERUP_MOVE == 0){
			--powerup1.y;
			if( dist( powerup1.x, powerup1.y, p1.x, p1.y ) < 6){
				++p1.mutiShoot;
				powerup1.y = -10;	// no more toBeCollect -.-
			}
		}
	}
}

void garbage_bullet(){
	if(ebs[head].toBeCollect == 1)
		while( ebs[++head].toBeCollect == 1 && head < BPOOL_SIZE);
	if(head == BPOOL_SIZE) head = 0;
}

void garbage_pbullet(){
	if(pbs[phead].toBeCollect == 1)
		while( pbs[++phead].toBeCollect == 1 && phead < BPOOL_SIZE);
	if(phead == BPOOL_SIZE) phead = 0;
}
void garbage_enemy(){
	if(es[ehead].toBeCollect == 1)
		while( es[++ehead].toBeCollect == 1 && ehead < EPOOL_SIZE);
	if(ehead == EPOOL_SIZE) ehead = 0;
}

//********************  FUNCTION FOR ""GAME LOGIC"" END  ********************//

//********************  FUNCTION FOR ""DRAWING"" START  ********************//
void bullet();
void bullet_player();
void player(double x, double y);
void enemy(double x, double y);
void enemies();
void powerup();
void display(void);
void display_playing();
void display_menu();
void display_h2p();

void bullet(){
	if(head == tail)
		return;
	glColor3f (1.0, 1.0, 1.0);
	for( i=head; i != tail; queueNext(&i,BPOOL_SIZE)){
		if(ebs[i].toBeCollect) continue;	//ignore, go for next one
		glPushMatrix();
			glTranslatef(ebs[i].x, ebs[i].y, 0);
			circle(1);
			/*glBegin(GL_POLYGON);
				for(j = 0; j< 20; ++j){
					double arc = radian(360) /20 * j;
					int size = 1;
					glVertex2f( size*cos(arc), size*sin(arc) );
				}
			glEnd();*/
		glPopMatrix();
	}
}

void bullet_player(){
	if(phead == ptail)
		return;
	glColor3f (.7, .7, .7);

	for(i = phead; i != ptail; queueNext(&i,BPOOL_SIZE) ){
		glPushMatrix();
			glTranslatef(pbs[i].x, pbs[i].y, 0);
			circle(1);
		glPopMatrix();
	}
	
}

void player(double x, double y){
	glColor3f (.4, .2, .8);
	glPushMatrix();
		glTranslatef(x,y,0);
		//body
		circle(4);
		//bloody effect
		if(bloody > 0.01){
			glEnable(GL_BLEND);
			glColor4f( 1, 0, 0, bloody );
			circle(4);
			glDisable(GL_BLEND);
		}
		if(shield){
			glEnable(GL_BLEND);
			glColor4f( .55, .85, .85, 0.5 + 0.2*sin(shdAlpha) );
			circle(6);
			glDisable(GL_BLEND);
		}
		//life bar
		glColor3f(	p1.hp == 10 ? 0 : 1,	p1.hp/10.0,	0);
		glBegin(GL_QUADS);
			glVertex2f( -5,			 7);
			glVertex2f( -5 + p1.hp,	 7);
			glVertex2f( -5 + p1.hp,	 8);
			glVertex2f( -5,			 8);
		glEnd();
		//energy bar
		glColor4f( .55, .85, .85, 1 );
		glBegin(GL_QUADS);
			glVertex2f( -5,			 6);
			glVertex2f( -5 + 10*(shdEnergy/100.0),	 6);
			glVertex2f( -5 + 10*(shdEnergy/100.0),	 7);
			glVertex2f( -5,			 7);
		glEnd();
	glPopMatrix();
}

void enemy(double x, double y){
	int ii = 0;
	glColor3f (.4, .8, .2);
	glPushMatrix();
		glTranslatef(x,y,0);
		//body
		circle(4);
	glPopMatrix();
}

void enemies(){
	for( i=ehead ; i != etail; queueNext(&i,EPOOL_SIZE) ){ //lol
		t_e *e = &es[i];
		if(e->alive)
			enemy(e->x,e->y);	//draw it
	}
}

void powerup(){
	//if(powerup1.y < -5) return;
	glColor3f( 0, .5, .5);
	
	glPushMatrix();
		glTranslatef( powerup1.x, powerup1.y, 0);
		circle(2);
		glColor3f( 1, 1, .0);
		glBegin(GL_QUADS);
		glVertex2f( -1.5, -0.2);
		glVertex2f( -1.5,  .2);
		glVertex2f(  1.5,  .2);
		glVertex2f(  1.5, -0.2);

		glVertex2f( -0.2, -1.5);
		glVertex2f( -0.2,  1.5);
		glVertex2f(  0.2,  1.5);
		glVertex2f(  0.2, -1.5);
		glEnd();
	glPopMatrix();
}

void display(void){
   
   glClear (GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   //look below
   switch (game_state)
   {
		case GS_MENU:
			display_menu();
			break;
		case GS_PLAYING:
			display_playing();
			break;
		case GS_H2P:
			display_h2p();
			break;
   }
   //look above
   glutSwapBuffers();
   glFlush ();
}

void display_menu(){

	/*glColor3f(0,1,1);
	glBegin(GL_QUADS);
		glVertex2f( 5, 5);
		glVertex2f( 5, 95);
		glVertex2f( 95, 95);
		glVertex2f( 95, 5);
	glEnd();*/
	if(menuBMP){
		glEnable(GL_TEXTURE_2D);
		   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		   glBindTexture(GL_TEXTURE_2D, menuBMP);
		   glBegin(GL_QUADS);
			   glTexCoord2f(0.0, 0.0); glVertex2f(	.0,		.0	);
			   glTexCoord2f(0.0, 1.0); glVertex2f(	.0,		100.0);
			   glTexCoord2f(1.0, 1.0); glVertex2f(	100.0,	100.0);
			   glTexCoord2f(1.0, 0.0); glVertex2f(	100.0,	.0	);
		   glEnd();
	    glDisable(GL_TEXTURE_2D);
	}else   printf("no texture bind\n");
	
}

void display_playing(){
   //background
   if(spaceBMP){//##
		glEnable(GL_TEXTURE_2D);
		   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		   glBindTexture(GL_TEXTURE_2D, spaceBMP);
		   glBegin(GL_QUADS);
			   glTexCoord2f(0.0, 0.0 );						glVertex2f(	.0,		-bgScroll);
			   glTexCoord2f(0.0, 2.0 /*+ 2*bgScroll/100*/);	glVertex2f(	.0,		200.0 - bgScroll);
			   glTexCoord2f(1.0, 2.0 /*+ 2*bgScroll/100*/);	glVertex2f(	100.0,	200.0 - bgScroll);
			   glTexCoord2f(1.0, 0.0 );						glVertex2f(	100.0,	-bgScroll);
		   glEnd();
	    glDisable(GL_TEXTURE_2D);
		if(bgScroll > 100) bgScroll -= 100;
	}else   printf("no texture bind\n");
   
   if(p1.alive)
      player(p1.x, p1.y);
   enemies();
   bullet();
   bullet_player();
   powerup();
   
   //mask when paused
   if(paused){
	   glEnable(GL_BLEND);
	   glColor4f(0,0,0,0.5);
	   glBegin(GL_QUADS);
			glVertex2f(0,0);
			glVertex2f(100,0);
			glVertex2f(100,100);
			glVertex2f(0,100);
		glEnd();
		glDisable(GL_BLEND);
   }

}

void display_h2p(){
	/*glColor3f(1,0,1);
	glBegin(GL_QUADS);
		glVertex2f( 5, 5);
		glVertex2f( 5, 95);
		glVertex2f( 95, 95);
		glVertex2f( 95, 5);
	glEnd();*/
	if(h2pBMP){
		glEnable(GL_TEXTURE_2D);
		   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		   glBindTexture(GL_TEXTURE_2D, h2pBMP);
		   glBegin(GL_QUADS);
			   glTexCoord2f(0.0, 0.0); glVertex2f(	.0,		.0	);
			   glTexCoord2f(0.0, 1.0); glVertex2f(	.0,		100.0);
			   glTexCoord2f(1.0, 1.0); glVertex2f(	100.0,	100.0);
			   glTexCoord2f(1.0, 0.0); glVertex2f(	100.0,	.0	);
		   glEnd();
	    glDisable(GL_TEXTURE_2D);
	}else   printf("no texture bind\n");
}

//unused yet
/*
void drawStringAt(int top, int left, char *c){
	printf("draw string\n");
	glPushMatrix();
		glTranslatef(top,left,0);
		glColor3f(1 ,0.5,0.5);
		glScalef(5,5,5);
		while( (*c) != 0){
			printf("d:%c\t",*c);
			glutStrokeCharacter( GLUT_STROKE_ROMAN, *c);
			++c;
		}
	glPopMatrix();
}
*/

//********************  FUNCTION FOR ""DRAWING"" END  ********************//

//********************  IMPORTANT CALL-BACK ""KEYBOARD"" START  ********************//
void key_playing(unsigned char k){
   switch (k) {
	  case 'h':
		  cheat_player_hp_no_less_than_1 =
			  cheat_player_hp_no_less_than_1 ? 0 : 1;
		  break;
	  case 'r':
		  game_reset();
		  break;
	  case 'p':
		  if(p1.alive == 0) break;
		  paused = paused ? 0 : 1;
          break;
   }
}

void key_menu(unsigned char k){
   switch(k){
		case 13:		//enter
		case 's':
			game_state = GS_PLAYING;
			game_reset();
			break;
		case 'h':
			game_state = GS_H2P;
			break;
   }
}

void keyboard(unsigned char key, int x, int y)
{
   if( isalpha(key) ){
	   key = tolower(key);
	   keyState[ key - 'a'] = 1;
   }
   debug_key(key);
   switch(game_state){
		case GS_PLAYING:
			key_playing(key);
			break;
		case GS_MENU:
			key_menu(key);
			break;
   }
   switch(key){
		case 27:		//esc
			game_state = GS_MENU;
			break;
   }

}

void keyUp(unsigned char key, int x, int y){
	if( !isalpha(key) ) return;
	key = tolower(key);
	keyState[key - 'a'] = 0;     
}

void spec(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			pd[1] = 1;//py++;
			break;	
		case GLUT_KEY_DOWN:
			pd[1] = -1;//py--;
			break;
		case GLUT_KEY_LEFT:
			pd[0] = -1;//px--;
			break;
		case GLUT_KEY_RIGHT:
			pd[0] = 1;//px++;
			break;
	}
}

void specUp(int key, int x, int y){
   switch(key)
   {
		case(GLUT_KEY_UP):                                  
        case(GLUT_KEY_DOWN):
			pd[1] = 0;
            break;
        case(GLUT_KEY_LEFT):
        case(GLUT_KEY_RIGHT):
            pd[0] = 0;
            break;
     }	
}

//********************  IMPORTANT CALL-BACK ""KEYBOARD"" END  ********************//

void idle()
{	
	static clock_t lastUpdate = 0;
	static int acc = 0;	//accumulator
	clock_t now = clock();
	if(now - lastUpdate > CLOCK){
		update(++acc);
		lastUpdate = now;
		glutPostRedisplay();
	}	
}

void reshape (int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if (w <= h) 
      gluOrtho2D (0.0, 100.0, 0.0, 100.0*(GLfloat)h/(GLfloat)w);
   else 
      gluOrtho2D (0.0, 100.0*(GLfloat)w/(GLfloat)h, 0.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y){
		switch(button){
		case GLUT_LEFT_BUTTON:
			break;
	}
}

//********************  REMAINS FUNCTIONs START  ********************//

void init (void) 
{
   glClearColor (1.0, 1.0, 1.0, 0.0);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, GAME_ZONE_WIDTH, 0.0, GAME_ZONE_HEIGHT, -1.0, 1.0);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("4101056014");

   init ();
   glewInit();
   game_init_once();

   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutSpecialFunc(spec);
   glutSpecialUpFunc(specUp);

   glutKeyboardUpFunc(keyUp);

   glutMainLoop();
   return 0;   /* ANSI C requires main to return int. */
}

//This function is ^C + ^V with little modification by me
//The data is not released, cheese
GLuint loadBMP_custom(const char * imagepath){
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;unsigned char * newData;
	GLuint textureID;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file) {printf("Image could not be opened\n"); return 0;}

	if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
	   return 0;
	}

	if ( header[0]!='B' || header[1]!='M' ){
	   printf("Not a correct BMP file\n");
	   return 0;
	}

	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)   imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)     dataPos=54; // The BMP header is done that way

	// Create a buffer
	{
		int bufferSize = sizeof(unsigned char) * imageSize;
		data = (unsigned char*) malloc( bufferSize );//new unsigned char [imageSize];
	}
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture

	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	printf("load %s successful, tid = %d\n", imagepath, textureID);
	return textureID;
}