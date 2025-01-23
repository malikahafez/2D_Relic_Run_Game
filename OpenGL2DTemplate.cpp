#include <stdlib.h>
#include <string>
#include <map>
#include <cmath>
#include<iostream>
#include <SFML/Audio.hpp>
#include <glut.h>

//loading sound and music using openAL libraries provided by SFML:
//music
sf::Music music;
//coin sfx
sf::Sound coin;
sf::SoundBuffer sound_Buffer;
//powerup sfx
sf::Sound powerup;
sf::SoundBuffer sound_Buffer2;
//obstacle collision sfx
sf::Sound spike;
sf::SoundBuffer sound_Buffer3;
//jump sfx
sf::Sound jumpSnd;
sf::SoundBuffer sound_Buffer4;
//game end - win sfx
sf::Sound win;
sf::SoundBuffer sound_Buffer5;
//game loss sfx
sf::Sound lose;
sf::SoundBuffer sound_Buffer6;
//duck sfx
sf::Sound duckSnd;
sf::SoundBuffer sound_Buffer7;

bool gameLoss = false;//for playing game lose sound

int times = 75;//total time of game (1:15min) 75sec
int score = 0;//score of player
int hp = 5;//lives decrease after collision with obstacles 5 lives
bool shiny = false;//animation of collectibles
int rot = 0;//animation of shield powerup
int bob = 60;//animation of 2x powerup
int delay = 0;//for delay every time game is reset after collision with obstacles

bool jump = false;//player jumping
int jumpCount = 8;//timed jump
bool duck = false;//player ducking



int numObst = 0;//number of obstacles on screen

int spo = 0;//for distance between obstacles
int spc = 0;//for distance between collectibles

int playerH = 40;//player height changes when jumping
int playerX = 40;//player x position on screen(doesn't change)
int playerD = 1;//player ducking using scale
int playerD2 = 0;//player ducking using translate**
int maxH = 120;//max height of player jump
int run = 1;//for run animation of player

int obstX = 460;//x position of obstacle on screen (decreases over time) start at 460
int obstH = 60;//height of obstacle
int collideX = 460;//for checking collision with obstacles

//for looping of collectibles in cycle1 and cycle2
int cyclespd1 = 31000;
int c1 = 1;
int cyclespd2 = 37000;
int c2 = 1;


int clctX = 460;
int p1X = 460;
int p2X = 460;
int collidecX = 460;//for collision of player with collectibles


//for collectibles 
bool cAppear1 = true;
bool cAppear2 = true;
bool cAppear3 = true;
bool cAppear4 = true;
bool cAppear5 = true;
bool cAppear6 = true;
bool cAppear7 = true;
bool cAppear8 = true;

bool mult = false;//flag if multiplier powerup collected
int multCount = 5;//number of seconds for activation of multiplier
bool shldOn = false;//flag if shield powerup collected
int shldCount = 5;//number of seconds for activation of shield

int clctX1 = 460;//x translation of collectibles
int clctX2 = 560;
int clctX3 = 660;
int clctX4 = 760;
int clctXp1 = 860;//x translation of shield

int clctX5 = 960;
int clctX6 = 1060;
int clctX7 = 1160;
int clctX8 = 1260;
int clctXp2 = 1360;//x translation of multiplier

//translate down off screen if collected
int t1 = 0;//y translation of collectibles
int t2 = 0;
int t3 = 0;
int t4 = 0;
int t5 = 0;
int t6 = 0;
int t7 = 0;
int t8 = 0;
int tp1 = 0;//y translation of shield
int tp2 = 0;//y translation of multiplier

//move obstacles and collectibles every 0.5 sec
int obstspd = 500;
int clctspd = 500;

//animation of background
int sunB = 0;//sun blue value =1 for white =0 for yellow
int timeB = 0;//change color of time when powerup collected

int skyB = 0;//stars blue value =1 for white =0 for yellow
int mSky = 0;//for translation of stars across screen
int starspd = 500;//speed of scrolling of stars

int day = 1;//for changing the background
int sky = 1;//for moving the stars

//background RGB values
double backRed = 1;
double backGreen = 1;
double backBlue = 1;


void drawRectangle(int x, int y, int w, int h) {

	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d((x + w), y);
	glVertex2d((x + w), (y + h));
	glVertex2d(x, (y + h));

	glEnd();

}
void drawCircle(int r) {
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 20, 20);
}
void drawCircleH(int r, int rin) {
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, rin, r, 20, 20);
}
void drawTriangle(int x, int y, double h, int b) {

	glBegin(GL_TRIANGLES);
	glVertex2d(x, y);
	glVertex2d(x, (y + h));
	glVertex2d((x + b), y);
	glEnd();
}
void drawEquilTriangle(int x, int y, int a) {
	int h = (sqrt(3) / 2) * a;
	int b = a / 2;
	drawTriangle(x, y, h, b);
	drawTriangle(x, y, h, -b);

}
void drawHexagon(int x, int y, int w1, int w2, int h) {
	int h1 = h / 2;
	glBegin(GL_POLYGON);
	glVertex2d(x, y);
	glVertex2d((x + w2), (y - h1));
	glVertex2d((x + w2 + w1), (y - h1));
	glVertex2d((x + w2 + w1 + w2), y);
	glVertex2d((x + w2 + w1), (y + h1));
	glVertex2d((x + w2), (y + h1));
	glEnd();
}
void drawEquilTriangleR(int x, int y, int a) {
	//draw equilateral by reflecting triangle
	int h = (sqrt(3) / 2) * a;
	int b = a / 2;
	drawTriangle(x, y, -h, b);
	drawTriangle(x, y, -h, -b);

}
void drawDiamond(int x, int y, int w, int h1, int h2) {
	int w1 = w / 2;
	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d((x - w1), (y + h1));
	glVertex2d(x, (y + h1 + h2));
	glVertex2d((x + w1), (y + h1));
	glEnd();

}
void drawDiamondH(int x, int y, int w, int h1, int h2, int lw) {
	int w1 = w / 2;
	glLineWidth(lw);
	glBegin(GL_LINE_LOOP);
	glVertex2d(x, y);
	glVertex2d((x - w1), (y + h1));
	glVertex2d(x, (y + h1 + h2));
	glVertex2d((x + w1), (y + h1));
	glEnd();

}
void drawStar(int m, int n) {
	glPushMatrix();
	glTranslated(m, n, 0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2d(40, 15);
	glVertex2d(40, 20);

	glVertex2d(30, 15);
	glVertex2d(50, 15);

	glVertex2d(40, 15);
	glVertex2d(35, 6.5);

	glVertex2d(40, 15);
	glVertex2d(45.5, 6.5);
	glEnd();
	glPopMatrix();
}
void LowerTriangles() {
	int m = 10;
	for (int i = 0; i < 13; i++) {
		glPushMatrix();
		glScaled(2, 1, 1);
		glColor3f(0.2, 0.255, 0.769);
		drawEquilTriangleR(m, 40, 20);
		glPopMatrix();
		m += 20;
	}
	//distance of 20 between each triangle

}
void LowerDots() {
	glColor3f(0.851, 0, 0);
	glPointSize(20);
	glBegin(GL_POINTS);
	glVertex2d(0, 10);
	glVertex2d(80, 10);
	glVertex2d(160, 10);
	glVertex2d(240, 10);
	glVertex2d(320, 10);
	glVertex2d(400, 10);
	glVertex2d(480, 10);
	glEnd();
}
//stars
void LowerLines() {
	glColor3f(1, 1, 1);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2d(40, 15);
	glVertex2d(40, 20);

	glVertex2d(30, 15);
	glVertex2d(50, 15);

	glVertex2d(40, 15);
	glVertex2d(35, 6.5);

	glVertex2d(40, 15);
	glVertex2d(45.5, 6.5);
	glEnd();
}
void LowerStars() {
	int m = 0;
	for (int i = 0; i < 6;i++) {
		glPushMatrix();
		glTranslated(m, 0, 0);
		LowerLines();
		glPopMatrix();
		m += 80;
	}

}
//lower border
void LowerBorder() {
	glColor3f(0.851, 0.663, 0.173);
	drawRectangle(0, 0, 1000, 40);
	LowerTriangles();
	LowerDots();
	LowerStars();
}
//upper drawings
void UpperLines() {
	glColor3f(0.349, 0.251, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2d(250, 212);
	glVertex2d(250, 230);
	glVertex2d(250, 212);
	glVertex2d(290, 220);
	glVertex2d(250, 212);
	glVertex2d(210, 220);
	glEnd();
}
void UpperFlowers() {
	glColor3f(0.369, 0.565, 0.871);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(250, 229);
	glVertex2d(230, 245);
	glVertex2d(270, 245);
	glEnd();

	glColor3f(0.988, 0.878, 0.467);
	drawEquilTriangleR(250, 246, 20);
}
//upper border
void UpperBorder() {
	glColor3f(0.922, 0.792, 0.455);
	drawRectangle(0, 210, 1000, 40);
	UpperLines();

	UpperFlowers();
	glPushMatrix();
	glTranslated(-40, -10, 0);
	UpperFlowers();
	glPopMatrix();
	glPushMatrix();
	glTranslated(40, -10, 0);
	UpperFlowers();
	glPopMatrix();

}
//print text
void print(double x, double y, void* font, std::string string) {
	glRasterPos2d(x, y);
	for (int i = 0; i < string.length(); i++) {
		glutBitmapCharacter(font, string[i]);
	}
}
//each health token
void drawHealth() {
	glColor3f(0.880, 0.482, 0.369);
	drawDiamondH(0, 25, 10, 5, 5, 5);
	drawRectangle(-8, 23, 16, 3);
	drawTriangle(0, 13, 12, 5);
	drawTriangle(0, 13, 12, -5);
}
//health bar
void health() {


	glColor3f(0.969, 0.922, 0.792);
	drawHexagon(0, 230, 150, 10, 30);
	int m = 20;//x position of each health token
	for (int i = 0; i < hp; i++) {
		glPushMatrix();
		glTranslated(m, 205, 0);
		drawHealth();
		glPopMatrix();
		m += 30;
	}
}
//time and score at top of screen
void scoreTime() {
	glColor3f(0.969, 0.922, 0.792);
	drawHexagon(380, 230, 100, 10, 30);
	glColor3f(0.349, 0.251, 0);
	print(400, 238, GLUT_BITMAP_HELVETICA_18, "Score");
	print(450, 238, GLUT_BITMAP_HELVETICA_18, "Time");
	int ss = score;
	std::string s1 = std::to_string(ss % 10);
	ss = ss / 10;
	std::string s2 = std::to_string(ss % 10);
	ss = ss / 10;
	std::string s3 = std::to_string(ss % 10);
	ss = ss / 10;
	std::string s4 = std::to_string(ss % 10);
	std::string s = s4 + s3 + s2 + s1;
	print(400.5, 220, GLUT_BITMAP_HELVETICA_18, s);
	glColor3f(0.349, 0.251, timeB);
	int min = times / 60;
	int sec = times % 60;
	std::string t = std::to_string(min) + " : " + ((std::to_string(sec).length() < 2) ? ("0" + std::to_string(sec)) : std::to_string(sec));
	print(450.5, 220, GLUT_BITMAP_HELVETICA_18, t);

}
//score at game end or game loss
void displayScore() {
	int ss = score;
	std::string s1 = std::to_string(ss % 10);
	ss = ss / 10;
	std::string s2 = std::to_string(ss % 10);
	ss = ss / 10;
	std::string s3 = std::to_string(ss % 10);
	ss = ss / 10;
	std::string s4 = std::to_string(ss % 10);
	std::string s = s4 + s3 + s2 + s1;
	print(230, 110, GLUT_BITMAP_TIMES_ROMAN_24, s);
}
//player's hat
void drawHat() {
	glBegin(GL_QUAD_STRIP);
	glVertex2d(0, 2);
	glVertex2d(10, 2);
	glVertex2d(2, 7);
	glVertex2d(7, 7);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2d(-5, 0);
	glVertex2d(15, 0);
	glVertex2d(15, 2);
	glVertex2d(-5, 2);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2d(0, 2);
	glVertex2d(-8, 10);
	glVertex2d(-2, 8);
	glEnd();

}
//player
void drawPlayer() {
	//head
	glPushMatrix();
	glTranslated(0, 30.5 - playerD2, 0);
	glScaled(1.25, 1, 1);
	drawCircleH(4, 3);
	glPopMatrix();
	//hat
	glPushMatrix();
	glTranslated(-5, 30.5 - playerD2, 0);
	drawHat();
	glPopMatrix();
	//neck
	drawEquilTriangle(0, 20 - playerD2, 10);
	//torso
	drawRectangle(-5, 5, 10, 15 - playerD2);
	//limbs
	glBegin(GL_LINES);
	//legs
	glVertex2d(-5 - run, 5);
	glVertex2d(-10 - run, 0);
	glVertex2d(5 - run, 5);
	glVertex2d(10 - run, 0);
	//arms
	glVertex2d(-5 * run, 20 - playerD2);
	glVertex2d(-10 * run, 10 - playerD2);
	glVertex2d(5 * run, 20 - playerD2);
	glVertex2d(15 * run, 25 - playerD2);
	glEnd();
}
//obstacles
void drawObstacle(int x, int y, int h) {
	glColor3f(0.349, 0.251, 0);
	//left leg
	drawRectangle(x, y, 10, h - 15);
	//top
	drawRectangle(x, h - 15, 40, 5);
	//right leg
	drawRectangle(x + 30, y, 10, h - 15);
	//spikes
	glColor3f(0.5, 0.5, 0.5);
	drawEquilTriangle(x + 5, h - 10, 10);
	drawEquilTriangle(x + 15, h - 10, 10);
	drawEquilTriangle(x + 25, h - 10, 10);
	drawEquilTriangle(x + 35, h - 10, 10);
}
//collectibles
void drawCollect(int x, int y) {
	/*glPushMatrix();
	glRotated(45, 0, 0, 1);*/
	glColor3f(1, 0.875, 0.416);
	drawDiamond(x + 0, y + 0, 20, 20, 5);
	glColor3f(1, 0.569, 0.016);
	drawDiamondH(x + 0, y + 5, 15, 15, 3, 2);
	drawDiamondH(x + 0, y + 10, 10, 10, 2, 2);
	//glPopMatrix();
}
void drawShiny1(int x, int y) {
	glBegin(GL_LINES);
	glVertex2d(x + 0, y - 1);
	glVertex2d(x + 0, y - 6);
	glVertex2d(x - 11, y + 20);
	glVertex2d(x - 16, y + 20);
	glVertex2d(x + 11, y + 20);
	glVertex2d(x + 16, y + 20);
	glVertex2d(x + 0, y + 26);
	glVertex2d(x + 0, y + 31);
	glEnd();
}
void drawShiny2(int x, int y) {
	glBegin(GL_LINES);
	glVertex2d(x - 5, y + 8);
	glVertex2d(x - 15, y + 0);
	glVertex2d(x - 5, y + 25);
	glVertex2d(x - 15, y + 33);
	glVertex2d(x + 5, y + 8);
	glVertex2d(x + 15, y + 0);
	glVertex2d(x + 5, y + 25);
	glVertex2d(x + 15, y + 33);

	glEnd();
}
void drawCollectible(int x, int y) {
	drawCollect(x, y);
	drawShiny1(x, y);
	drawShiny2(x, y);
}
//powerup 1
void drawShield(int x, int y) {
	glPushMatrix();
	glTranslated(x, y, 0);
	glRotated(rot, 0, 0, 1);
	glScaled(0.65, 0.65, 1);
	//top
	glColor3f(0.988, 0.878, 0.467);
	glBegin(GL_POLYGON);
	glVertex2d(-5, +0);
	glVertex2d(+10, +0);
	glVertex2d(+10, +15);
	glVertex2d(+2.5, +13);
	glVertex2d(-5, +15);
	glEnd();
	//bottom
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(-5, +0);
	glVertex2d(+10, +0);
	glVertex2d(+6, -5);
	glVertex2d(+0, -5);
	glEnd();
	//star
	glLineWidth(2);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(+2.5, +10);
	glVertex2d(+2.5, +6);
	glVertex2d(-3, +6);
	glVertex2d(+8, +6);
	glVertex2d(+2.5, +6);
	glVertex2d(-1, +0);
	glVertex2d(+2.5, +6);
	glVertex2d(+6, +0);
	glEnd();
	//dots
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2d(-2.5, +12);
	glVertex2d(+7, +12);
	glEnd();
	glPopMatrix();

}
//powerup 2
void drawMult(int x, int y) {
	glPushMatrix();
	glTranslated(x, y, 0);
	glScaled(0.5, 0.65, 1);
	glColor3f(0.988, 0.878, 0.467);
	drawHexagon(-10, +5, 20, 10, 20);
	glColor3f(1, 0, 0);
	drawEquilTriangle(+10, -5, 10);
	drawEquilTriangleR(+10, +15, 10);
	glPushMatrix();
	glTranslated(+15, -5, 0);
	glRotated(90, 0, 0, 1);
	drawEquilTriangle(+10, -10, 10);
	drawEquilTriangleR(+10, +20, 10);
	glPopMatrix();
	glColor3f(0.369, 0.565, 0.871);
	drawRectangle(+0, +0, 20, 10);
	glColor3f(0, 0, 0);
	//2
	glBegin(GL_LINE_STRIP);
	glVertex2d(+10, +8);
	glVertex2d(+15, +8);
	glVertex2d(+15, +6);
	glVertex2d(+10, +6);
	glVertex2d(+10, +3);
	glVertex2d(+15, +3);
	glEnd();
	//x
	glBegin(GL_LINES);
	glVertex2d(+2, +7.5);
	glVertex2d(+7, +3.5);
	glVertex2d(+7, +7.5);
	glVertex2d(+2, +3.5);
	glEnd();
	glPopMatrix();

}
//background
void drawSun() {
	//set sunR and sunG and sunB to 1 when night
	glPushMatrix();
	glTranslated(450, 160, 0);
	glScaled(1.25, 1, 1);
	glColor3f(1, 1, sunB);
	drawCircle(20);
	glPopMatrix();
}
void drawSky() {
	//shapes in sky that move with time and turn white at night skyB = 1
	drawSun();
	glPushMatrix();
	glTranslated(mSky, 0, 0);
	glColor3d(1, 1, skyB);
	drawDiamond(50, 190, 10, 5, 5);
	drawDiamond(250, 190, 10, 5, 5);
	drawDiamond(450, 190, 10, 5, 5);
	drawDiamond(650, 190, 10, 5, 5);
	drawDiamond(150, 180, 10, 5, 5);
	drawDiamond(350, 180, 10, 5, 5);
	drawDiamond(550, 180, 10, 5, 5);
	glPopMatrix();
}



void Display() {

	glClearColor(backRed, backGreen, backBlue, 0.0f);//the background color is changed in the timer function after 15 sec by changing these parameters.
	glClear(GL_COLOR_BUFFER_BIT);
	UpperBorder();
	LowerBorder();
	//if lost all lives - game loss
	if (hp == 0) {
		glColor3f(0.969, 0.922, 0.792);
		drawHexagon(150, 130, 100, 50, 100);
		glColor3f(0, 0, 0);
		print(220, 125, GLUT_BITMAP_TIMES_ROMAN_24, "Game Lost");
		displayScore();
		music.stop();
		gameLoss = true;
	}
	//if time ran out - game end
	else if (times == 0) {
		glColor3f(0.969, 0.922, 0.792);
		drawHexagon(150, 130, 100, 50, 100);
		glColor3f(0, 0, 0);
		print(220, 125, GLUT_BITMAP_TIMES_ROMAN_24, "Game End");
		displayScore();
		music.stop();
	}
	else {
		scoreTime();
		health();
		drawSky();
		//180 obstacles 
		glPushMatrix();
		glTranslated(obstX, 40, 0);
		obstH = 60;
		drawObstacle(0, 0, obstH);
		obstH = 80;
		drawObstacle(100, 0, obstH);
		obstH = 70;
		drawObstacle(200, 0, obstH);
		obstH = 85;
		drawObstacle(300, 0, obstH);
		obstH = 60;
		drawObstacle(400, 0, obstH);
		obstH = 80;
		drawObstacle(500, 0, obstH);
		obstH = 70;
		drawObstacle(600, 0, obstH);
		obstH = 85;
		drawObstacle(700, 0, obstH);
		obstH = 60;
		drawObstacle(800, 0, obstH);
		obstH = 80;
		drawObstacle(900, 0, obstH);
		obstH = 70;
		drawObstacle(1000, 0, obstH);
		obstH = 85;
		drawObstacle(1100, 0, obstH);
		obstH = 60;
		drawObstacle(1200, 0, obstH);
		obstH = 80;
		drawObstacle(1300, 0, obstH);
		obstH = 70;
		drawObstacle(1400, 0, obstH);
		obstH = 85;
		drawObstacle(1500, 0, obstH);
		obstH = 60;
		drawObstacle(1600, 0, obstH);
		obstH = 80;
		drawObstacle(1700, 0, obstH);
		obstH = 70;
		drawObstacle(1800, 0, obstH);
		obstH = 85;
		drawObstacle(1900, 0, obstH);
		obstH = 60;
		drawObstacle(2000, 0, obstH);
		obstH = 80;
		drawObstacle(2100, 0, obstH);
		obstH = 70;
		drawObstacle(2200, 0, obstH);
		obstH = 85;
		drawObstacle(2300, 0, obstH);
		obstH = 60;
		drawObstacle(2400, 0, obstH);
		obstH = 80;
		drawObstacle(2500, 0, obstH);
		obstH = 70;
		drawObstacle(2600, 0, obstH);
		obstH = 85;
		drawObstacle(2700, 0, obstH);
		obstH = 60;
		drawObstacle(2800, 0, obstH);
		obstH = 80;
		drawObstacle(2900, 0, obstH);
		obstH = 70;
		drawObstacle(3000, 0, obstH);
		obstH = 85;
		drawObstacle(3100, 0, obstH);
		obstH = 60;
		drawObstacle(3200, 0, obstH);
		obstH = 80;
		drawObstacle(3300, 0, obstH);
		obstH = 70;
		drawObstacle(3400, 0, obstH);
		obstH = 85;
		drawObstacle(3500, 0, obstH);
		obstH = 60;
		drawObstacle(3600, 0, obstH);
		obstH = 80;
		drawObstacle(3700, 0, obstH);
		obstH = 70;
		drawObstacle(3800, 0, obstH);
		obstH = 85;
		drawObstacle(3900, 0, obstH);
		obstH = 60;
		drawObstacle(4000, 0, obstH);
		obstH = 80;
		drawObstacle(4100, 0, obstH);
		obstH = 70;
		drawObstacle(4200, 0, obstH);
		obstH = 85;
		drawObstacle(4300, 0, obstH);
		obstH = 60;
		drawObstacle(4400, 0, obstH);
		obstH = 80;
		drawObstacle(4500, 0, obstH);
		obstH = 70;
		drawObstacle(4600, 0, obstH);
		obstH = 85;
		drawObstacle(4700, 0, obstH);
		obstH = 60;
		drawObstacle(4800, 0, obstH);
		obstH = 80;
		drawObstacle(4900, 0, obstH);
		obstH = 70;
		drawObstacle(5000, 0, obstH);
		obstH = 85;
		drawObstacle(5100, 0, obstH);
		obstH = 60;
		drawObstacle(5200, 0, obstH);
		obstH = 80;
		drawObstacle(5300, 0, obstH);
		obstH = 70;
		drawObstacle(5400, 0, obstH);
		obstH = 85;
		drawObstacle(5500, 0, obstH);
		obstH = 60;
		drawObstacle(5600, 0, obstH);
		obstH = 80;
		drawObstacle(5700, 0, obstH);
		obstH = 70;
		drawObstacle(5800, 0, obstH);
		obstH = 85;
		drawObstacle(5900, 0, obstH);
		obstH = 60;
		drawObstacle(6000, 0, obstH);
		obstH = 80;
		drawObstacle(6100, 0, obstH);
		obstH = 70;
		drawObstacle(6200, 0, obstH);
		obstH = 85;
		drawObstacle(6300, 0, obstH);
		obstH = 60;
		drawObstacle(6400, 0, obstH);
		obstH = 80;
		drawObstacle(6500, 0, obstH);
		obstH = 70;
		drawObstacle(6600, 0, obstH);
		obstH = 85;
		drawObstacle(6700, 0, obstH);
		obstH = 60;
		drawObstacle(6800, 0, obstH);
		obstH = 80;
		drawObstacle(6900, 0, obstH);
		obstH = 70;
		drawObstacle(7000, 0, obstH);
		obstH = 85;
		drawObstacle(7100, 0, obstH);
		obstH = 60;
		drawObstacle(7200, 0, obstH);
		obstH = 80;
		drawObstacle(7300, 0, obstH);
		obstH = 70;
		drawObstacle(7400, 0, obstH);
		obstH = 85;
		drawObstacle(7500, 0, obstH);
		obstH = 60;
		drawObstacle(7600, 0, obstH);
		obstH = 80;
		drawObstacle(7700, 0, obstH);
		obstH = 70;
		drawObstacle(7800, 0, obstH);
		obstH = 85;
		drawObstacle(7900, 0, obstH);
		obstH = 60;
		drawObstacle(8000, 0, obstH);
		obstH = 80;
		drawObstacle(8100, 0, obstH);
		obstH = 70;
		drawObstacle(8200, 0, obstH);
		obstH = 85;
		drawObstacle(8300, 0, obstH);
		obstH = 60;
		drawObstacle(8400, 0, obstH);
		obstH = 80;
		drawObstacle(8500, 0, obstH);
		obstH = 70;
		drawObstacle(8600, 0, obstH);
		obstH = 85;
		drawObstacle(8700, 0, obstH);
		obstH = 60;
		drawObstacle(8800, 0, obstH);
		obstH = 80;
		drawObstacle(8900, 0, obstH);
		obstH = 70;
		drawObstacle(9000, 0, obstH);
		obstH = 85;
		drawObstacle(9100, 0, obstH);
		obstH = 60;
		drawObstacle(9200, 0, obstH);
		obstH = 80;
		drawObstacle(9300, 0, obstH);
		obstH = 70;
		drawObstacle(9400, 0, obstH);
		obstH = 85;
		drawObstacle(9500, 0, obstH);
		obstH = 60;
		drawObstacle(9600, 0, obstH);
		obstH = 80;
		drawObstacle(9700, 0, obstH);
		obstH = 70;
		drawObstacle(9800, 0, obstH);
		obstH = 85;
		drawObstacle(9900, 0, obstH);
		obstH = 60;
		drawObstacle(10000, 0, obstH);
		obstH = 80;
		drawObstacle(10100, 0, obstH);
		obstH = 70;
		drawObstacle(10200, 0, obstH);
		obstH = 85;
		drawObstacle(10300, 0, obstH);
		obstH = 60;
		drawObstacle(10400, 0, obstH);
		obstH = 80;
		drawObstacle(10500, 0, obstH);
		obstH = 70;
		drawObstacle(10600, 0, obstH);
		obstH = 85;
		drawObstacle(10700, 0, obstH);
		obstH = 60;
		drawObstacle(10800, 0, obstH);
		obstH = 80;
		drawObstacle(10900, 0, obstH);
		obstH = 70;
		drawObstacle(11000, 0, obstH);
		obstH = 85;
		drawObstacle(11100, 0, obstH);
		obstH = 60;
		drawObstacle(11200, 0, obstH);
		obstH = 80;
		drawObstacle(11300, 0, obstH);
		obstH = 70;
		drawObstacle(11400, 0, obstH);
		obstH = 85;
		drawObstacle(11500, 0, obstH);
		obstH = 60;
		drawObstacle(11600, 0, obstH);
		obstH = 80;
		drawObstacle(11700, 0, obstH);
		obstH = 70;
		drawObstacle(11800, 0, obstH);
		obstH = 85;
		drawObstacle(11900, 0, obstH);
		obstH = 60;
		drawObstacle(12000, 0, obstH);
		obstH = 80;
		drawObstacle(12100, 0, obstH);
		obstH = 70;
		drawObstacle(12200, 0, obstH);
		obstH = 85;
		drawObstacle(12300, 0, obstH);
		obstH = 60;
		drawObstacle(12400, 0, obstH);
		obstH = 80;
		drawObstacle(12500, 0, obstH);
		obstH = 70;
		drawObstacle(12600, 0, obstH);
		obstH = 85;
		drawObstacle(12700, 0, obstH);
		obstH = 60;
		drawObstacle(12800, 0, obstH);
		obstH = 80;
		drawObstacle(12900, 0, obstH);
		obstH = 70;
		drawObstacle(13000, 0, obstH);
		obstH = 85;
		drawObstacle(13100, 0, obstH);
		obstH = 60;
		drawObstacle(13200, 0, obstH);
		obstH = 80;
		drawObstacle(13300, 0, obstH);
		obstH = 70;
		drawObstacle(13400, 0, obstH);
		obstH = 85;
		drawObstacle(13500, 0, obstH);
		obstH = 60;
		drawObstacle(13600, 0, obstH);
		obstH = 80;
		drawObstacle(13700, 0, obstH);
		obstH = 70;
		drawObstacle(13800, 0, obstH);
		obstH = 85;
		drawObstacle(13900, 0, obstH);
		obstH = 60;
		drawObstacle(14000, 0, obstH);
		obstH = 80;
		drawObstacle(14100, 0, obstH);
		obstH = 70;
		drawObstacle(14200, 0, obstH);
		obstH = 85;
		drawObstacle(14300, 0, obstH);
		obstH = 60;
		drawObstacle(14400, 0, obstH);
		obstH = 80;
		drawObstacle(14500, 0, obstH);
		obstH = 70;
		drawObstacle(14600, 0, obstH);
		obstH = 85;
		drawObstacle(14700, 0, obstH);
		obstH = 60;
		drawObstacle(14800, 0, obstH);
		obstH = 80;
		drawObstacle(14900, 0, obstH);
		obstH = 70;
		drawObstacle(15000, 0, obstH);
		obstH = 85;
		drawObstacle(15100, 0, obstH);
		obstH = 60;
		drawObstacle(15200, 0, obstH);
		obstH = 80;
		drawObstacle(15300, 0, obstH);
		obstH = 70;
		drawObstacle(15400, 0, obstH);
		obstH = 85;
		drawObstacle(15500, 0, obstH);
		obstH = 60;
		drawObstacle(15600, 0, obstH);
		obstH = 80;
		drawObstacle(15700, 0, obstH);
		obstH = 70;
		drawObstacle(15800, 0, obstH);
		obstH = 85;
		drawObstacle(15900, 0, obstH);
		obstH = 60;
		drawObstacle(16000, 0, obstH);
		obstH = 80;
		drawObstacle(16100, 0, obstH);
		obstH = 70;
		drawObstacle(16200, 0, obstH);
		obstH = 85;
		drawObstacle(16300, 0, obstH);
		obstH = 60;
		drawObstacle(16400, 0, obstH);
		obstH = 80;
		drawObstacle(16500, 0, obstH);
		obstH = 70;
		drawObstacle(16600, 0, obstH);
		obstH = 85;
		drawObstacle(16700, 0, obstH);
		obstH = 60;
		drawObstacle(16800, 0, obstH);
		obstH = 80;
		drawObstacle(16900, 0, obstH);
		obstH = 70;
		drawObstacle(17000, 0, obstH);
		obstH = 85;
		drawObstacle(17100, 0, obstH);
		obstH = 60;
		drawObstacle(17200, 0, obstH);
		obstH = 80;
		drawObstacle(17300, 0, obstH);
		obstH = 70;
		drawObstacle(17400, 0, obstH);
		obstH = 85;
		drawObstacle(17500, 0, obstH);
		obstH = 60;
		drawObstacle(17600, 0, obstH);
		obstH = 80;
		drawObstacle(17700, 0, obstH);
		obstH = 70;
		drawObstacle(17700, 0, obstH);
		obstH = 85;
		drawObstacle(17800, 0, obstH);
		obstH = 60;
		drawObstacle(17900, 0, obstH);
		glPopMatrix();

		//first group of collectibles with p1
				//collect1
		glPushMatrix();
		glTranslated(clctX1, t1, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear1) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);
		}
		glPopMatrix();
		//collect2
		glPushMatrix();
		glTranslated(clctX2, t2, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear2) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);
		}
		glPopMatrix();
		//collect3
		glPushMatrix();
		glTranslated(clctX3, t3, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear3) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);

		}
		glPopMatrix();
		//collect4
		glPushMatrix();
		glTranslated(clctX4, t4, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear4) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);

		}
		glPopMatrix();
		//shield powerup
		glPushMatrix();
		glTranslated(clctXp1, tp1, 0);
		drawShield(20, 130);
		glPopMatrix();

		glPopMatrix();
		//second group of collectibles with p2
		//collect5
		glPushMatrix();
		glTranslated(clctX5, t5, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear5) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);
		}
		glPopMatrix();
		//collect6
		glPushMatrix();
		glTranslated(clctX6, t6, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear6) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);
		}
		glPopMatrix();
		//collect7
		glPushMatrix();
		glTranslated(clctX7, t7, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear7) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);

		}
		glPopMatrix();
		//collect8
		glPushMatrix();
		glTranslated(clctX8, t8, 0);
		glScaled(0.65, 0.65, 1);
		if (cAppear8) {
			drawCollect(30, 190);
			if (shiny == false)
				drawShiny1(30, 190);
			else
				drawShiny2(30, 190);

		}
		glPopMatrix();
		//multiplier powerup
		glPushMatrix();
		glTranslated(clctXp2, tp2, 0);
		glPushMatrix();
		glTranslated(0, bob, 0);
		drawMult(10, 80);
		glPopMatrix();

		glPopMatrix();

		//player
		glPushMatrix();
		glTranslated(playerX, playerH, 0);
		glScaled(1, playerD, 0);
		drawPlayer();
		glPopMatrix();
	}


	glFlush();
}
void key(unsigned char k, int x, int y)//keyboard function for player jumping and ducking
{

	if (k == 'w' && !jump && playerH == 40) {//if the letter w is pressed, then the player will jump
		/*while (playerH < 120) {
			playerH += 10;
		}*/
		playerH = 120;
		jump = true;
		jumpSnd.play();
	}
	if (k == 's' && !duck) {//if the letter s is pressed, then the player will duck
		//playerD = 0.5;
		playerD2 = 8;
		//playerH = 40;
		duck = true;
		duckSnd.play();
	}

	glutPostRedisplay();
}

void keyUp(unsigned char k, int x, int y)//keyboard up function when releasing jump and duck
{
	if (k == 'w') {//if the letter w is released, then the player will go down again;
		//handled in timer function**
	}
	if (k == 's') {//if the letter s is released, then the player will go up again
		playerD2 = 0;
		duck = false;
	}
	glutPostRedisplay();//redisplay to update the screen with the changed
}


void spe(int k, int x, int y)// keyboard special key function for player jumping and ducking
{

	if (k == GLUT_KEY_UP && !jump && playerH == 40) {//if the up arrow is pressed,  then the player will jump
		playerH = 120;
		jump = true;
		jumpSnd.play();
	}
	if (k == GLUT_KEY_DOWN && !duck) {//if the down arrow is pressed, then the player will duck
		//playerD = 0.5;
		playerD2 = 8;
		duck = true;
		duckSnd.play();
	}

	glutPostRedisplay();
}
void speUp(int k, int x, int y)// keyboard special key function for jump and duck release
{
	if (k == GLUT_KEY_UP) {//if the up arrow is released, then the player will go back down
		//handled in timer function**
	}
	if (k == GLUT_KEY_DOWN) {//if the down arrow is released, then the player will go back up
		playerD2 = 0;
		duck = false;
	}

	glutPostRedisplay();
}



//change from day to night
void Skytime(int val) {
	if (day % 2 == 0) {
		sunB = 1;
		backRed = 0.5;
		backGreen = 0.5;
		skyB = 1;

	}
	else {
		sunB = 0;
		backRed = 1;
		backGreen = 1;
		skyB = 0;


	}

	day += 1;
	glutPostRedisplay();
	glutTimerFunc(15000, Skytime, 0);
}
//move stars across screen
void Startime(int val) {

	if (sky < 18) {
		mSky -= 10;
	}

	else {
		mSky = 40;
		sky = 0;
		mSky -= 10;
	}

	sky += 1;
	glutPostRedisplay();
	glutTimerFunc(starspd, Startime, 0);
}
//increase speed of game over time
void speedT(int val) {
	obstspd *= 0.75;
	clctspd *= 0.75;
	starspd *= 0.75;
	glutPostRedisplay();
	//increase speed every 5 sec
	glutTimerFunc(5000, speedT, 0);
}
//animate collectibles and powerups
void animate(int val) {
	if (shiny == true) {
		shiny = false;
		rot += 10;
		bob += 5;
		run *= -1;
	}
	else {
		shiny = true;
		rot -= 10;
		bob -= 5;
		run *= -1;
	}

	glutPostRedisplay();
	glutTimerFunc(200, animate, 0);
}
//count time down
void countDown(int val) {
	if (times > 0)
		times -= 1;

	glutPostRedisplay();
	glutTimerFunc(1000, countDown, 0);
}
//move obstacles across screen
void moveObstacles(int val) {
	obstX -= 5;
	collideX -= 5;
	if (obstX == -(spo * 100) - 45) {
		spo += 1;
		numObst += 1;
		collideX = obstX + spo * 100;
	}

	//updating health - collision detection
	if ((playerX >= collideX && playerX <= collideX + 40) && (!jump) && (!duck) && hp > 0 && !shldOn) {

		hp -= 1;
		obstX = 460;
		collideX = obstX;
		obstspd = 500;
		starspd = 500;
		clctX = 460 + 5 * delay;
		p1X = 460 + 5 * delay;
		p2X = 460 + 5 * delay;
		clctspd = 500;
		delay += 1;
		cyclespd1 = 31000;
		cyclespd2 = 37000;
		c1 = 0;
		c2 = 0;
		spc = 0;
		collidecX = 460;
		clctX1 = 460;//x translation of collectibles
		clctX2 = 560;
		clctX3 = 660;
		clctX4 = 760;
		clctXp1 = 860;//x translation of shield
		t1 = 0;//y translation of collectibles
		t2 = 0;
		t3 = 0;
		t4 = 0;
		tp1 = 0;//y translation of shield

		clctX5 = 960;//x translation of collectibles
		clctX6 = 1060;
		clctX7 = 1160;
		clctX8 = 1260;
		clctXp2 = 1360;//x translation of multiplier
		t5 = 0;//y translation of collectibles
		t6 = 0;
		t7 = 0;
		t8 = 0;
		tp2 = 0;//y translation of multiplier
		mult = false;
		multCount = 5;
		shldOn = false;
		shldCount = false;
		spike.play();
	}

	glutPostRedisplay();
	glutTimerFunc(obstspd, moveObstacles, 0);
}
//move collectibles and powerups across screen
void moveCollect(int val) {
	clctX1 -= 5;
	clctX2 -= 5;
	clctX3 -= 5;
	clctX4 -= 5;
	clctX5 -= 5;
	clctX6 -= 5;
	clctX7 -= 5;
	clctX8 -= 5;
	clctXp1 -= 5;
	clctXp2 -= 5;
	collidecX -= 5;
	clctX -= 5;
	p1X -= 5;
	p2X -= 5;


	if (clctX == -(spc * 100) - 45) {
		spc += 1;
		collidecX = clctX + spc * 100;
	}
	//updating score - collision detection
	if (((clctX1 + 1 >= playerX && clctX1 <= playerX) || (clctX2 + 1 >= playerX && clctX2 <= playerX)
		|| (clctX3 + 1 >= playerX && clctX3 <= playerX) || (clctX4 + 1 >= playerX && clctX4 <= playerX)
		|| (clctX5 + 1 >= playerX && clctX5 <= playerX) || (clctX6 + 1 >= playerX && clctX6 <= playerX)
		|| (clctX7 + 1 >= playerX && clctX7 <= playerX) || (clctX8 + 1 >= playerX && clctX8 <= playerX)
		|| (clctXp1 + 1 >= playerX && clctXp1 <= playerX) || (clctXp2 + 1 >= playerX && clctXp2 <= playerX)) && jump)
	{

		if (mult)//if multiplier effect on, get double points
			score += 20;
		else
			score += 10;

		if (clctX1 + 1 >= playerX && clctX1 <= playerX) {
			t1 = -300;
			coin.play();
		}
		else if (clctX2 + 1 >= playerX && clctX2 <= playerX) {
			t2 = -300;
			coin.play();
		}
		else if (clctX3 + 1 >= playerX && clctX3 <= playerX) {
			t3 = -300;
			coin.play();
		}
		else if (clctX4 + 1 >= playerX && clctX4 <= playerX) {
			t4 = -300;
			coin.play();
		}
		else if (clctX5 + 1 >= playerX && clctX5 <= playerX) {
			t5 = -300;
			coin.play();
		}
		else if (clctX6 + 1 >= playerX && clctX6 <= playerX) {
			t6 = -300;
			coin.play();
		}
		else if (clctX7 + 1 >= playerX && clctX7 <= playerX) {
			t7 = -300;
			coin.play();
		}
		else if (clctX8 + 1 >= playerX && clctX8 <= playerX) {
			t8 = -300;
			coin.play();
		}
		else if (clctXp1 + 1 >= playerX && clctXp1 <= playerX) {//shield powerup
			tp1 = -300;
			shldOn = true;
			powerup.play();
		}
		else if (clctXp2 + 1 >= playerX && clctXp2 <= playerX) {//multiplier powerup
			tp2 = -300;
			mult = true;
			powerup.play();

		}
	}

	glutPostRedisplay();
	glutTimerFunc(clctspd, moveCollect, 0);
}
//loop first group of collectibles on screen
void cycle1(int val) {

	if (c1 == 0)
		c1 += 1;
	else {
		clctX1 = 460;//x translation of collectibles
		clctX2 = 560;
		clctX3 = 660;
		clctX4 = 760;
		clctXp1 = 860;//x translation of shield

		t1 = 0;//y translation of collectibles
		t2 = 0;
		t3 = 0;
		t4 = 0;
		tp1 = 0;//y translation of shield
		c1 += 1;
	}
	if (c1 == 2)
		cyclespd1 = 11000;
	else if (c1 == 3)
		cyclespd1 = 8000;
	else if (c1 == 4)
		cyclespd1 = 4500;
	else if (c1 == 5)
		cyclespd1 = 4000;
	else if (c1 == 6)
		cyclespd1 = 3500;
	else if (c1 == 7)
		cyclespd1 = 3000;
	else if (c1 == 8)
		cyclespd1 = 4000;
	else if (c1 == 9)
		cyclespd1 = 3500;
	else if (c1 == 10)
		cyclespd1 = 3000;
	else if (c1 == 11)
		cyclespd1 = 2000;
	else if (c1 == 12)
		cyclespd1 = 2000;
	else if (c1 == 13)
		cyclespd1 = 3000;
	else if (c1 == 14)
		cyclespd1 = 2000;
	else if (c1 == 15)
		cyclespd1 = 3000;
	else if (c1 == 16)
		cyclespd1 = 2000;
	else if (c1 == 17)
		cyclespd1 = 3000;
	else if (c1 == 18)
		cyclespd1 = 2000;

	glutPostRedisplay();
	glutTimerFunc(cyclespd1, cycle1, 0);
}
//loop second group of collectibles on screen
void cycle2(int val) {

	if (c2 == 0)
		c2 += 1;
	else {
		clctX5 = 560;//x translation of collectibles
		clctX6 = 660;
		clctX7 = 760;
		clctX8 = 860;
		clctXp2 = 960;//x translation of multiplier

		t5 = 0;//y translation of collectibles
		t6 = 0;
		t7 = 0;
		t8 = 0;
		tp2 = 0;//y translation of multiplier
		c2 += 1;
	}
	if (c2 == 2)
		cyclespd2 = 9000;
	else if (c2 == 3)
		cyclespd2 = 10000;
	else if (c2 == 4)
		cyclespd2 = 5000;
	else if (c2 == 5)
		cyclespd2 = 4500;
	else if (c2 == 6)
		cyclespd2 = 4000;
	else if (c2 == 7)
		cyclespd2 = 8000;
	else if (c2 == 8)
		cyclespd2 = 5500;
	else if (c2 == 9)
		cyclespd2 = 3500;
	else if (c2 == 10)
		cyclespd2 = 3000;
	else if (c2 == 11)
		cyclespd2 = 2500;
	else if (c2 == 12)
		cyclespd2 = 2000;
	else if (c2 == 13)
		cyclespd2 = 2000;
	else if (c2 == 14)
		cyclespd2 = 1500;
	else if (c2 == 15)
		cyclespd2 = 1500;
	else if (c2 == 16)
		cyclespd2 = 1000;
	else if (c2 == 17)
		cyclespd2 = 500;
	else if (c2 == 18)
		cyclespd2 = 250;

	glutPostRedisplay();
	glutTimerFunc(cyclespd2, cycle2, 0);
}
//deactivate multiplier powerup after 5 sec
void multiplier(int val) {
	if (mult && multCount > 0) {
		multCount -= 1;
		timeB = 1;
	}
	else if (multCount == 0) {
		mult = false;
		multCount = 5;
		timeB = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, multiplier, 0);
}
//deactivate shield powerup after 5 sec
void shield(int val) {
	if (shldOn && shldCount > 0) {
		shldCount -= 1;
		timeB = 1;
	}
	else if (shldCount == 0) {
		shldOn = false;
		shldCount = 5;
		timeB = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(1000, shield, 0);
}
void GameEnd(int val) {//play game end sound
	if (times == 0)
		win.play();

	glutPostRedisplay();
	glutTimerFunc(5000, GameEnd, 0);
}
void GameLose(int val) {//play game lose sound
	if (hp == 0)
		lose.play();
	glutPostRedisplay();
	glutTimerFunc(5000, GameLose, 0);
}
void jumping(int val) {//handle jumping motion
	if (jump && jumpCount > 0) {
		jumpCount -= 1;
		playerH = 120;
	}
	else if (jumpCount == 0 && playerH > 40) {
		jump = false;
		playerH -= 20;
	}
	else
		jumpCount = 8;


	glutPostRedisplay();
	glutTimerFunc(200, jumping, 0);
}





void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(1000, 600);
	//actual window size is 500x250****
	glutInitWindowPosition(150, 60);

	glutCreateWindow("Relic Run");
	glutDisplayFunc(Display);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutKeyboardFunc(key);			//call the keyboard function for jumping and ducking
	glutKeyboardUpFunc(keyUp);		//call the keyboard up function for jumping and ducking release

	glutSpecialFunc(spe);			//call the keyboard special keys function for jumping and ducking
	glutSpecialUpFunc(speUp);		//call the keyboard special keys up function for jumping and ducking release



	glutTimerFunc(200, Skytime, 0);		//change colour of sky 
	glutTimerFunc(200, Startime, 0);	//move stars
	glutTimerFunc(200, speedT, 0);		//increase speed
	glutTimerFunc(200, animate, 0);		//animate collectibles and powerups
	glutTimerFunc(1000, countDown, 0);	//count down time
	glutTimerFunc(500, moveObstacles, 0);	//move obstacles
	glutTimerFunc(500, moveCollect, 0);		//move collectibles and powerups
	glutTimerFunc(31950, cycle1, 0);		//reload collectibles and shield
	glutTimerFunc(37000, cycle2, 0);		//reload collectibles and multiplier
	glutTimerFunc(1000, multiplier, 0);		//disable multiplier powerup after 5 sec
	glutTimerFunc(1000, shield, 0);			//disable shield powerup after 5 sec
	glutTimerFunc(5500, GameEnd, 0);		//play sound upon game end (time runs out)
	glutTimerFunc(5000, GameLose, 0);		//play sound upon game loss (health ran out)
	glutTimerFunc(500, jumping, 0);			//timed jumps - can't jump until back on ground - can't fly



	//loading sound and music using openAL libraries provided by SFML:

	//background music
	music.openFromFile("8-bit Desert Tune.ogg");
	music.setVolume(50.0f);
	music.play();

	//coin collection sound effect
	sound_Buffer.loadFromFile("coin-recieved-230517.wav");
	coin.setBuffer(sound_Buffer);
	coin.setVolume(100.0f);

	//powerup collection sound effect
	sound_Buffer2.loadFromFile("badge-coin-win-14675.wav");
	powerup.setBuffer(sound_Buffer2);
	powerup.setVolume(100.0f);

	//obstacle collision sound effect
	sound_Buffer3.loadFromFile("Spike-Trap-Sound-Effect.wav");
	spike.setBuffer(sound_Buffer3);
	spike.setVolume(300.0f);

	//jump sound effect
	sound_Buffer4.loadFromFile("jump-up-245782.wav");
	jumpSnd.setBuffer(sound_Buffer4);
	jumpSnd.setVolume(100.0f);

	//game end sound effect
	sound_Buffer5.loadFromFile("8-bit-video-game-win-level-sound-version-1-145827.wav");
	win.setBuffer(sound_Buffer5);
	win.setVolume(100.0f);

	//game lose sound effect
	sound_Buffer6.loadFromFile("8-bit-video-game-lose-sound-version-1-145828.wav");
	lose.setBuffer(sound_Buffer6);
	lose.setVolume(100.0f);

	//duck sound effect
	sound_Buffer7.loadFromFile("woosh-230554.wav");
	duckSnd.setBuffer(sound_Buffer7);
	duckSnd.setVolume(100.0f);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 500, 0.0, 250);

	glutMainLoop();//don't call any method after this line as it will not be reached.
}
