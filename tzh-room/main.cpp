
/*****************************************************************************
		FILE : main.cpp (Assignment 1)
		NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
		Student Information
		Student ID:SX2304094
		Student Name:ͯ童子恒
*****************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>

#include <glut.h>
const GLdouble FRUSTDIM = 200.0f;
#define M_PI       3.14159265358979323846
int option = 0, i = 0;
bool enableLight = 1;

/* Do  animation*/
GLfloat board_angle = 0;
GLUquadricObj* quadobj;
GLfloat fanangle = 0, fan1angle = 0, fan2angle = 120, fan3angle = 240;
GLfloat doorangle2 = 0;
GLfloat doorangle = 0;
GLfloat teapotAngle = 0;
GLfloat waterheight = 36;
GLfloat witleboardPosition = -350;

bool light0_flag = true;
bool enableBoard = true;
bool displayer = true;


//light0
GLfloat light0_pos[] = { 0, 500, -500, 1 };
GLfloat light0_dir[] = { 0, -1, 0 };
GLfloat light0_cutoff = 180;
GLfloat light0_amb[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light0_dif[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light0_spc[] = { 1.0, 1.0, 1.0, 1.0 };

struct tm* localTime;
int hour, minute, second;

void gettime() {
	time_t curTime;
	time(&curTime);
	localTime = localtime(&curTime);
	hour = localTime->tm_hour;
	minute = localTime->tm_min;
	second = localTime->tm_sec;
}

void setlight0(bool flag) {
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_dir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light0_cutoff);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spc);

	if (flag)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

void setdisplayerLight(bool flag) {
	// 启用光照
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1); // 使用第二个光源


	GLfloat light_position[] = { 64, -60, witleboardPosition, 0.0 };  // 光源位置，最后一个参数为0表示定向光
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };   // 光源散射颜色

	glLightfv(GL_LIGHT1, GL_POSITION, light_position); // 设置光源位置
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);   // 设置光源散射颜色

	glEnable(GL_LIGHTING);       // 启用光照
	//glEnable(GL_LIGHT0);         // 启用光源
	if (flag)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
}

static GLuint texName[4];//�������
void loadTexture(char* filename, GLuint& texture)
{
	static GLint   ImageWidth;
	static GLint   ImageHeight;
	static GLint   PixelLength;
	static GLubyte* PixelData;

	FILE* pFile = fopen(filename, "rb");
	if (pFile == 0)
		exit(0);
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
	fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

	PixelLength = ImageWidth * 3;
	while (PixelLength % 4 != 0)
		++PixelLength;
	PixelLength *= ImageHeight;

	PixelData = (GLubyte*)malloc(PixelLength);
	if (PixelData == 0)
		exit(0);
	fseek(pFile, 54, SEEK_SET);
	fread(PixelData, PixelLength, 1, pFile);

	fclose(pFile);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);
}


void init(void) // All Setup For OpenGL Goes Here 
{
	quadobj = gluNewQuadric();
	/* Light 0 Settings */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_COLOR_MATERIAL);

	char filename1[] = "floor.bmp";
	loadTexture(filename1, texName[0]);
	char filename2[] = "wall.bmp";
	loadTexture(filename2, texName[1]);
	char filename3[] = "ceiling.bmp"; //TODO
	loadTexture(filename3, texName[2]);
	char filename4[] = "displayer.bmp";
	loadTexture(filename4, texName[3]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void display(void) // Here's Where We Do All The Drawing
{

	// TODO:
	// Place light source here
	setlight0(light0_flag);
	//--------------


	//-----------
	setdisplayerLight(displayer);

	// TODO:
	// Draw walls and objects here
	/* Clear the buffer */
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	glLoadIdentity();
	glPushMatrix();
	/* Draw the wall START */

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//�����Ͳ��ʻ�Ϸ�ʽ
	glBindTexture(GL_TEXTURE_2D, texName[1]);//wall
	/* Back wall */
	glBegin(GL_POLYGON);
	glColor3f(211.0 / 255, 211.0 / 255, 211.0 / 255);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200.0f, 200.0f, -700.0f);
	glTexCoord2f(0.0, 5.0);
	glVertex3f(-200.0f, -200.0f, -700.0f);
	glTexCoord2f(5.0, 5.0);
	glVertex3f(200.0f, -200.0f, -700.0f);
	glTexCoord2f(5.0, 0.0);
	glVertex3f(200.0f, 200.0f, -700.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	/* Top wall */
	glBegin(GL_POLYGON);
	glColor4f(112.0 / 255, 128.0 / 255, 144.0 / 255, 1);

	GLfloat matBrown[] = { 210.0 / 255,180.0 / 255,140.0 / 255,1 };
	GLfloat matWhite[] = { 1, 1, 1, 1 };
	GLfloat matShininess[] = { 20 };
	GLfloat matBlack[] = { 0, 0, 0, 1 };

	GLfloat topwall[] = { 112.0 / 255, 128.0 / 255, 144.0 / 255,1 };
	GLfloat topwallShininess[] = { 2 };

	GLfloat mat_diffuse[] = {0.5, 0.5, 0.5, 1.0}; // 材质的漫反射颜色
	GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0}; // 材质的镜面反射颜色
	GLfloat mat_shininess[] = {50.0}; // 材质的高光系数

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


	glVertex3f(-200.0f, 200.0f, -300.0f);
	glVertex3f(-200.0f, 200.0f, -700.0f);
	glVertex3f(200.0f, 200.0f, -700.0f);
	glVertex3f(200.0f, 200.0f, -300.0f);
	glEnd();

	//left
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//�����Ͳ��ʻ�Ϸ�ʽ
	glBindTexture(GL_TEXTURE_2D, texName[1]);//wall
	glBegin(GL_POLYGON);
	glColor3f(211.0 / 255, 211.0 / 255, 211.0 / 255);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200.0f, 200.0f, -300.0f);
	glTexCoord2f(0.0, 5.0);
	glVertex3f(-200.0f, 200.0f, -700.0f);
	glTexCoord2f(5.0, 5.0);
	glVertex3f(-200.0f, -200.0f, -700.0f);
	glTexCoord2f(5.0, 0.0);
	glVertex3f(-200.0f, -200.0f, -300.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//�����Ͳ��ʻ�Ϸ�ʽ
	glBindTexture(GL_TEXTURE_2D, texName[1]);//wall
	glBegin(GL_POLYGON);
	glBegin(GL_POLYGON);
	//glColor3f(220.0 / 255, 220.0 / 255, 220.0 / 255);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200.0f, 200.0f, -300.0f);
	glTexCoord2f(0.0, 5.0);
	glVertex3f(200.0f, 200.0f, -700.0f);
	glTexCoord2f(5.0, 5.0);
	glVertex3f(200.0f, -200.0f, -700.0f);
	glTexCoord2f(5.0, 0.0);
	glVertex3f(200.0f, -200.0f, -300.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//�����Ͳ��ʻ�Ϸ�ʽ
	glBindTexture(GL_TEXTURE_2D, texName[0]);//floor
	//floor
	glBegin(GL_POLYGON);
	//glColor3f(0.9f, 0.84f, 0.3f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200.0f, -200.0f, -300.0f);
	glTexCoord2f(0.0, 5.0);
	glVertex3f(-200.0f, -200.0f, -700.0f);
	glTexCoord2f(5.0, 5.0);
	glVertex3f(200.0f, -200.0f, -700.0f);
	glTexCoord2f(5.0, 0.0);
	glVertex3f(200.0f, -200.0f, -300.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//light

	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1);
	glTranslatef(-100, 200, -400);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadobj, 4.f, 4.f, 200.f, 100.f, 100.f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100, 200, -600);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadobj, 4.f, 4.f, 200.f, 100.f, 100.f);
	glPopMatrix();

	//door
	glPushMatrix();

	glPushMatrix();
	glTranslatef(198, -75, -360);
	glColor4f(102.0 / 255, 0.0 / 255, 0.0 / 255, 1);
	glRotatef(doorangle, 0, 1, 0);
	glTranslatef(0, 0, 30);
	glScalef(4, 250, 60);
	glutSolidCube(1);
	//drawcuboid(10, 100, 60);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.0 / 255, 0.0 / 255, 0.0 / 255, 1);
	glBegin(GL_POLYGON);
	glVertex3f(200.0f, -200.0f, -300.0f);
	glVertex3f(200.0f, -200.0f, -360.0f);
	glVertex3f(200.0f, 50.0f, -360.0f);
	glVertex3f(200.0f, 50.0f, -300.0f);
	glEnd();
	glPopMatrix();

	//handle
	glPushMatrix();
	glTranslatef(198, -75, -360);
	glColor4f(191.0 / 255, 173.0 / 255, 111.0 / 255, 1);
	glRotatef(doorangle, 0, 1, 0);
	glTranslatef(-9, 0, 51);
	glutSolidSphere(4, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(198, -75, -360);
	glRotatef(doorangle, 0, 1, 0);
	glTranslatef(-5, 0, 51);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadobj, 2.f, 2.f, 3.f, 50.f, 50.f);
	glPopMatrix();

	//sofa
	glPushMatrix();

	GLfloat sofa[] = { 210.0 / 255, 180.0 / 255, 140.0 / 255, 1 };
	GLfloat sofasp[] = { 0.5, 0.5, 0.5,1 };
	GLfloat sofashi[] = { 32 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, sofa);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sofa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sofasp);
	glMaterialfv(GL_FRONT, GL_SHININESS, sofashi);
	//glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

	//low layer
	glTranslatef(-165, -190, -350);
	glColor4f(210.0 / 255, 180.0 / 255, 140.0 / 255, 1);
	glScalef(148, 20, 100);
	glutSolidCube(1);
	//drawcuboid(50,100,26);
	glPopMatrix();


	//dianzi
	glPushMatrix();
	glTranslatef(-166, -170, -350);
	glColor3f(144.0 / 255, 133.0 / 255, 133.0 / 255);
	glScalef(150, 20, 98);
	glutSolidCube(1);
	//drawcuboid(48, 100, 26);
	glPopMatrix();

	//fushou
	glPushMatrix();
	glTranslatef(-165, -170, -390);
	glColor4f(210.0 / 255, 180.0 / 255, 140.0 / 255, 1);
	glScalef(150, 70, 20);
	glutSolidCube(1);
	//drawcuboid(10, 100, 60);
	glPopMatrix();




	//window1
	glPushMatrix();
	glTranslatef(-198, 70, -350);
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glScalef(4, 260, 60);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-196, 140, -350);
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glScalef(4, 6, 60);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glColor4f(176.0 / 255, 196.0 / 255, 222.0 / 255, 0.3f);
	glBegin(GL_POLYGON);
	glVertex3f(-195.0f, 198.0f, -321.0f);
	glVertex3f(-195.0f, 198.0f, -377.0f);
	glVertex3f(-195.0f, -58.0f, -377.0f);
	glVertex3f(-195.0f, -58.0f, -321.0f);
	glEnd();
	glPopMatrix();

	//window2
	glPushMatrix();
	glTranslatef(-198, 70, -600);
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glScalef(4, 260, 60);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-196, 140, -600);
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glScalef(4, 6, 60);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glColor4f(176.0 / 255, 196.0 / 255, 222.0 / 255, 0.3f);
	glBegin(GL_POLYGON);
	glVertex3f(-195.0f, 198.0f, -571.0f);
	glVertex3f(-195.0f, 198.0f, -627.0f);
	glVertex3f(-195.0f, -58.0f, -627.0f);
	glVertex3f(-195.0f, -58.0f, -571.0f);
	glEnd();
	glPopMatrix();

	//window3
	glPushMatrix();
	glTranslatef(-198, 70, -450);
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glScalef(4, 260, 60);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-196, 140, -450);
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glScalef(4, 6, 60);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix();
	glColor4f(176.0 / 255, 196.0 / 255, 222.0 / 255, 0.3f);
	glBegin(GL_POLYGON);
	glVertex3f(-195.0f, 198.0f, -421.0f);
	glVertex3f(-195.0f, 198.0f, -477.0f);
	glVertex3f(-195.0f, -58.0f, -477.0f);
	glVertex3f(-195.0f, -58.0f, -421.0f);
	glEnd();

	//tank desk
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(-197, -167, -405);

	glScalef(6, 66, 4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-147, -167, -405);
	glScalef(6, 66, 4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-147, -167, -485);
	glScalef(6, 66, 4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-197, -167, -485);
	glScalef(6, 66, 4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-172, -131, -445);
	glColor4f(210.0 / 255, 180.0 / 255, 140.0 / 255, 1);
	glScalef(56, 6, 84);
	glutSolidCube(1);
	glPopMatrix();


	//computer desk
	//left leg
	glColor4f(38.0 / 255, 144.0 / 255, 161.0 / 255, 1);
	glPushMatrix();
	glTranslatef(-188, -150, -550);
	glScalef(4, 100, 60);
	glutSolidCube(1);
	glPopMatrix();

	//front leg
	glPushMatrix();
	glTranslatef(-130, -150, -521);
	glScalef(120, 100, 4);
	glutSolidCube(1);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glTranslatef(-72, -150, -550);
	glScalef(4, 100, 60);
	glutSolidCube(1);
	glPopMatrix();

	//desk top
	glPushMatrix();
	glTranslatef(-130, -97, -550);
	glScalef(140, 6, 100);
	glutSolidCube(1);
	glPopMatrix();


	//computer1
	//bottom 
	glColor4f(0 / 255, 0 / 255, 0 / 255, 1);
	glPushMatrix();
	glTranslatef(-160, -93, -528);
	glRotatef(-20, 0, 1, 0);
	glScalef(20, 2, 16);
	glutSolidCube(1);
	glPopMatrix();
	//neck
	glPushMatrix();
	glTranslatef(-160, -84, -523);
	glRotatef(-20, 0, 1, 0);
	glScalef(2, 16, 2);
	glutSolidCube(1);
	glPopMatrix();
	//screen
	glPushMatrix();
	glTranslatef(-160, -64, -526);
	glRotatef(-20, 0, 1, 0);
	glScalef(60, 36, 2);
	glutSolidCube(1);
	glPopMatrix();

	//computer2
	//bottom 
	glColor4f(0 / 255, 0 / 255, 0 / 255, 1);
	glPushMatrix();
	glTranslatef(-100, -93, -528);
	glRotatef(20, 0, 1, 0);
	glScalef(20, 2, 16);
	glutSolidCube(1);
	glPopMatrix();
	//neck
	glPushMatrix();
	glTranslatef(-100, -84, -523);
	glRotatef(20, 0, 1, 0);
	glScalef(2, 16, 2);
	glutSolidCube(1);
	glPopMatrix();
	//screen
	glPushMatrix();
	glTranslatef(-100, -64, -526);
	glRotatef(20, 0, 1, 0);
	glScalef(60, 36, 2);
	glutSolidCube(1);
	glPopMatrix();

	//keyboard
	glPushMatrix();
	glTranslatef(-130, -93, -565);
	glScalef(40, 2, 12);
	glutSolidCube(1);
	glPopMatrix();

	//water
	glColor4f(0.0 / 255, 255.0 / 255, 255.0 / 255, 1);
	glPushMatrix();
	glTranslatef(-172, -129 + waterheight / 2, -445);
	glScalef(46, waterheight, 76);
	glutSolidCube(1);
	glPopMatrix();


	glDepthMask(GL_FALSE); //��Ȼ���������Ϊֻ��
	//tank
	//left
	glColor4f(248.0 / 255, 248.0 / 255, 255.0 / 255, 0.4);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	glPushMatrix();
	glTranslatef(-195, -105, -445);
	glScalef(2, 50, 78);
	glutSolidCube(1);
	glPopMatrix();

	//front
	glPushMatrix();
	glTranslatef(-172, -105, -407);
	glScalef(46, 50, 2);
	glutSolidCube(1);
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslatef(-149, -105, -445);
	glScalef(2, 50, 78);
	glutSolidCube(1);
	glPopMatrix();

	//back
	glPushMatrix();
	glTranslatef(-172, -105, -483);
	glScalef(46, 50, 2);
	glutSolidCube(1);
	glPopMatrix();
	//floor
	glPushMatrix();
	glTranslatef(-172, -129, -445);
	glScalef(48, 2, 78);
	glutSolidCube(1);
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	// TODO:
	// Add animation here

	//cabinet
	//body-back
	glColor4f(0.0 / 255, 204.0 / 255, 204.0 / 255, 1);
	glPushMatrix();
	glTranslatef(180, -50, -698);
	glScalef(40, 300, 4);
	glutSolidCube(1);
	glPopMatrix();

	//body-right
	glPushMatrix();
	glTranslatef(198, -50, -600);
	glScalef(4, 300, 200);
	glutSolidCube(1);
	glPopMatrix();

	//body-front
	glPushMatrix();
	glTranslatef(180, -50, -502);
	glScalef(40, 300, 4);
	glutSolidCube(1);
	glPopMatrix();

	//body-top
	glPushMatrix();
	glTranslatef(180, 98, -600);
	glScalef(40, 4, 200);
	glutSolidCube(1);
	glPopMatrix();

	//body-down
	glPushMatrix();
	glTranslatef(180, -198, -600);
	glScalef(40, 4, 200);
	glutSolidCube(1);
	glPopMatrix();

	//detail-row
	glPushMatrix();
	glTranslatef(180, -82, -600);
	glScalef(40, 4, 200);
	glutSolidCube(1);
	glPopMatrix();

	//detail-row
	glPushMatrix();
	glTranslatef(180, -22, -600);
	glScalef(40, 4, 200);
	glutSolidCube(1);
	glPopMatrix();

	//detail-row
	glPushMatrix();
	glTranslatef(180, 38, -600);
	glScalef(40, 4, 200);
	glutSolidCube(1);
	glPopMatrix();

	
	//detail-door
	glColor4f(0.0 / 255, 51.0 / 255, 51.0 / 255, 1);
	glPushMatrix();
	glTranslatef(161, -140, -665);
	glRotated(doorangle2, 0, 1, 0);
	glScalef(2, 110, 62);
	glutSolidCube(1);
	glPopMatrix();

	glColor4f(153.0 / 255, 102.0 / 255, 0.0 / 255, 1);
	glPushMatrix();
	glTranslatef(161, -140, -633);
	glScalef(2, 110, 1.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(161, -140, -643);
	glRotatef(doorangle2, 0, 1, 0);
	glutSolidSphere(2, 10, 10);
	glPopMatrix();


	//detail-door
	glColor4f(0.0 / 255, 51.0 / 255, 51.0 / 255, 1);
	glPushMatrix();
	glTranslatef(161, -140, -600);
	glRotated(doorangle2, 0, 1, 0);
	glScalef(2, 110, 62);
	glutSolidCube(1);
	glPopMatrix();

	glColor4f(153.0 / 255, 102.0 / 255, 0.0 / 255, 1);
	glPushMatrix();
	glTranslatef(161, -140, -568);
	glScalef(2, 110, 1.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(161, -140, -578);
	glRotatef(doorangle2, 0, 1, 0);
	glutSolidSphere(2, 10, 10);
	glPopMatrix();

	//detail-door
	glPushMatrix();
	glColor4f(0.0 / 255, 51.0 / 255, 51.0 / 255, 1);
	glTranslatef(161, -140, -535);
	glRotated(360-doorangle2, 0, 1, 0);
	glScalef(2, 110, 62);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0.0 / 255, 0.0 / 255, 0.0 / 255, 1);
	glTranslatef(161, -140, -556);
	glTranslatef(161-(-556+535)*sin(doorangle2), -140, -535+(-556+535)*cos(doorangle2));
	glutSolidSphere(2, 10, 10);
	glPopMatrix();

	//detail
	glColor4f(51.0 / 255, 255.0 / 255, 153.0 / 255, 1);
	glPushMatrix();
	glTranslatef(180, -50, -600);
	glScalef(40, 60, 4);
	glutSolidCube(1);
	glPopMatrix();
	//detail

	glColor4f(51.0 / 255, 255.0 / 255, 153.0 / 255, 1);
	glPushMatrix();
	glTranslatef(180, 10, -640);
	glScalef(40, 60, 4);
	glutSolidCube(1);
	glPopMatrix();

	glColor4f(51.0 / 255, 255.0 / 255, 153.0 / 255, 1);
	glPushMatrix();
	glTranslatef(180, 10, -560);
	glScalef(40, 60, 4);
	glutSolidCube(1);
	glPopMatrix();
	//detail
	glColor4f(51.0 / 255, 255.0 / 255, 153.0 / 255, 1);
	glPushMatrix();
	glTranslatef(180, 70, -600);
	glScalef(40, 60, 4);
	glutSolidCube(1);
	glPopMatrix();

	//whiteboard
	//leg-left
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(2, -198, witleboardPosition);
	glScalef(4, 4, 50);
	glutSolidCube(1);
	glPopMatrix();

	//leg-right
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(126, -198, witleboardPosition);
	glScalef(4, 4, 50);
	glutSolidCube(1);
	glPopMatrix();

	//center
	glPushMatrix();
	glTranslatef(64, -198, witleboardPosition);
	glScalef(120, 4, 4);
	glutSolidCube(1);
	glPopMatrix();


	//leg-left
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(2, -125, witleboardPosition);
	glScalef(4, 150, 4);
	glutSolidCube(1);
	glPopMatrix();

	//leg-right
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(126, -125, witleboardPosition);
	glScalef(4, 150, 4);
	glutSolidCube(1);
	glPopMatrix();


	if (displayer == true) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		glBindTexture(GL_TEXTURE_2D, texName[3]);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}//TODO
	//glColor4f(65.0 / 255, 65.0 / 255, 65.0 / 255, 1);
	glPushMatrix();
	glTranslatef(64, -60, witleboardPosition);
	glRotatef(board_angle, 1, 0, 0);
	glScalef(120, 110, 4);
	//glutSolidCube(1);
	// 绘制 Cube 并附加纹理
	glBegin(GL_QUADS);
	// 此处为每个面指定纹理坐标，纹理坐标范围通常在 0 到 1 之间
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5); // 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, -0.5); // 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, -0.5); // 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5); // 左上

		glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5); // 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5); // 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5); // 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, 0.5); // 左上

		glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5); // 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5, 0.5); // 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, 0.5); // 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5); // 左上

		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -0.5, -0.5); // 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5); // 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5); // 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, -0.5); // 左上

		glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5); // 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5, 0.5); // 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5); // 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5); // 左上

		glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, -0.5); // 左下
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5); // 右下
		glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5); // 右上
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, -0.5); // 左上
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	GLfloat black_color[] = { 1.0, 1.0, 1.0, 1.0 };
	//glMaterialfv(GL_FRONT, GL_EMISSION, black_color);


	//coffee table
	//left-leg
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(142, -170, -458);
	glScalef(4, 60, 4);
	glutSolidCube(1);
	glPopMatrix();

	//left-right
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(198, -170, -458);
	glScalef(4, 60, 4);
	glutSolidCube(1);
	glPopMatrix();

	//left-leg
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(142, -170, -382);
	glScalef(4, 60, 4);
	glutSolidCube(1);
	glPopMatrix();

	//left-right
	glColor4f(64.0 / 255, 64.0 / 255, 64.0 / 255, 1);
	glPushMatrix();
	glTranslatef(198, -170, -382);
	glScalef(4, 60, 4);
	glutSolidCube(1);
	glPopMatrix();


	if (enableBoard == false) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName[1]);				// 选择纹理
		glBegin(GL_QUADS);
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0, 0.0);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 1.0, 0.0);		// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(2.141421, 1.0, -1.141421);		// 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f(2.141421, -1.0, -1.141421);		// 纹理和四边形的左下
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	//teapot
	glColor4f(184.0 / 255, 134.0 / 255, 11.0 / 255, 1);
	glPushMatrix();
	glTranslatef(170, -130, -400);
	glScalef(10.f, 10.0f, -10.f);
	glRotated(teapotAngle, 0, 1, 0);
	glutSolidTeapot(1);
	glPopMatrix();


	glDepthMask(GL_FALSE); //��Ȼ���������Ϊֻ��


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	//table1
	glColor4f(255.0 / 255, 255.0 / 255, 255.0 / 255, 0.3);
	glPushMatrix();
	glTranslatef(170, -172, -420);
	glScalef(60, 4, 80);
	glutSolidCube(1);
	glPopMatrix();

	//table2
	glColor4f(255.0 / 255, 255.0 / 255, 255.0 / 255, 0.3);
	glPushMatrix();
	glTranslatef(170, -142, -420);
	glScalef(60, 4, 80);
	glutSolidCube(1);
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);


	//fan
	//ganzi
	glColor4f(211.0 / 255, 211.0 / 255, 211.0 / 255, 1);
	glPushMatrix();
	glTranslatef(0, 180, -500);
	glScalef(6, 40, 6);
	glutSolidCube(1);
	glPopMatrix();

	//tuoqiu
	glColor4f(233.0 / 255, 233.0 / 255, 216.0 / 255, 1);
	glPushMatrix();
	glTranslatef(0, 157, -500);
	glRotatef(fanangle, 0, 1, 0);
	glScalef(15, 6, 15);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	//shanye1
	glColor4f(220.0 / 255, 220.0 / 255, 220.0 / 255, 1);
	glPushMatrix();
	glTranslatef(0, 157, -500);
	glRotatef(fan1angle, 0, 1, 0);
	glTranslatef(0, 0, -30);
	glScalef(20, 4, 60);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 157, -500);
	glRotatef(fan2angle, 0, 1, 0);
	glTranslatef(0, 0, -30);
	glScalef(20, 4, 60);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 157, -500);
	glRotatef(fan3angle, 0, 1, 0);
	glTranslatef(0, 0, -30);
	glScalef(20, 4, 60);
	glutSolidCube(1);
	glPopMatrix();


	//clock
	double clockangle = 0;
	glPushMatrix();
	glTranslatef(199, 160, -400);
	glRotatef(-90, 0, 1, 0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
	for (int n = 1; n <= 360; n++) {
		glVertex3f(30 * cos(clockangle), 30 * sin(clockangle), 0);
		clockangle += M_PI / 360 * 2;
	}
	glEnd();
	glPopMatrix();

	//kedu
	clockangle = 0;
	glPushMatrix();
	glTranslatef(199, 160, -400);
	glRotatef(-90, 0, 1, 0);
	glLineWidth(3);

	glBegin(GL_LINES);
	for (int q = 0; q <= 11; q++) {
		if (q % 3 == 0) {
			glColor4f(0.0 / 255, 128.0 / 255, 0.0 / 255, 1);
		}
		else {
			glColor4f(0.0, 0.1, 0.1, 1);
		}
		glVertex3f((30 - 4) * cos(clockangle), (30 - 4) * sin(clockangle), 0);
		glVertex3f(30 * cos(clockangle), 30 * sin(clockangle), 0);
		clockangle += M_PI / 6;
	}
	glEnd();
	glPopMatrix();

	//shizhen
	glPushMatrix();
	glTranslatef(198, 160, -400);
	glRotatef(-90, 0, 1, 0);
	clockangle = M_PI / 2 - M_PI * 2 / 12 * (hour + minute / 60.0);
	glLineWidth(3);
	glColor3f(1.0, 1.0, 1.0);
	glColor4f(210.0 / 255, 105.0 / 255, 30.0 / 255, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(10 * cos(clockangle), 10 * sin(clockangle), 0);
	glEnd();
	glPopMatrix();
	//fenzhen
	glPushMatrix();
	glTranslatef(198, 160, -400);
	glRotatef(-90, 0, 1, 0);
	clockangle = M_PI / 2 - M_PI * 2 * (minute / 60.0 + second / 60.0 / 60);
	glLineWidth(2);
	glColor3f(1.0, 1.0, 1.0);
	glColor4f(210.0 / 255, 105.0 / 255, 30.0 / 255, 1);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(17 * cos(clockangle), 17 * sin(clockangle), 0);
	glEnd();
	glPopMatrix();

	//miaozhen
	glPushMatrix();
	glTranslatef(198, 160, -400);
	glRotatef(-90, 0, 1, 0);
	clockangle = M_PI / 2 - M_PI * 2 * (second / 60.0);
	glLineWidth(1);
	glColor3f(1.0, 1.0, 1.0);
	glColor4f(210.0 / 255, 105.0 / 255, 30.0 / 255, 1);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(20 * cos(clockangle), 20 * sin(clockangle), 0);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
	glFlush();


}

void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	//	winWidth = w; winHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Frustum Setting
	glFrustum(-FRUSTDIM, FRUSTDIM, -FRUSTDIM, FRUSTDIM, 300., 800.);

	glMatrixMode(GL_MODELVIEW);

}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
	switch (key)
	{
	case '\033':
		exit(0);
		break;
	case 'b':
		option = 1;
		printf("Press key 'b'.\n");
		break;
	case '0':
		option = 0;
		break;
	case '1':
		option = 2;
		break;
	case '2':
		option = 3;
		break;
	case '3':
		option = 4;
		break;
	case 'd':
		option = 5;
		break;
	case 'c':
		option = 6;
		break;
	case 'n':
		option = 7;
		break;
	case 'm':
		option = 8;
		break;
	case 'o':
		option = 9;
		break;
	case 't':
		option = 10;
		break;
	case 'w':
		option = 11;
		break;
	case 's':
		option = 12;
		break;
	case 'f':
		option = 13;
		break;
	case 'k':
		option = 14;
		break;
	case 'l':
		option = 15;
		break;
	}
}

void idle()
{
	if (option == 1)
	{
		if (board_angle >= 360)
			board_angle = 0;
		board_angle++;
		printf("%f\n", board_angle);
		option = 0;
	}
	else if (option == 2)
	{
		if (fanangle >= 360)
			fanangle = 0;
		if (fan1angle >= 360)
			fan1angle = 0;
		if (fan2angle >= 360)
			fan2angle = 0;
		if (fan3angle >= 360)
			fan3angle = 0;
		fanangle += 2;
		fan1angle += 2;
		fan2angle += 2;
		fan3angle += 2;
	}
	else if (option == 3)
	{
		if (fanangle >= 360)
			fanangle = 0;
		if (fan1angle >= 360)
			fan1angle = 0;
		if (fan2angle >= 360)
			fan2angle = 0;
		if (fan3angle >= 360)
			fan3angle = 0;
		fanangle += 4;
		fan1angle += 4;
		fan2angle += 4;
		fan3angle += 4;
	}
	else if (option == 4)
	{
		if (fanangle >= 360)
			fanangle = 0;
		if (fan1angle >= 360)
			fan1angle = 0;
		if (fan2angle >= 360)
			fan2angle = 0;
		if (fan3angle >= 360)
			fan3angle = 0;
		fanangle += 6;
		fan1angle += 6;
		fan2angle += 6;
		fan3angle += 6;
	}
	else if (option == 5)
	{
		if (doorangle <= -90)
		{
			doorangle = -90;
			option = 0;
		}
		doorangle--;
	}
	else if (option == 6)
	{
		if (doorangle >= -1)
		{
			doorangle = -1;
			option = 0;
		}

		doorangle++;
	}
	else if (option == 7)
	{
		if (waterheight <= 0)
			option = 0;
		waterheight -= 0.5;
	}
	else if (option == 8)
	{
		if (waterheight >= 36)
			option = 0;
		waterheight += 0.5;
	}
	else if (option == 9)
	{
		light0_flag = !light0_flag;
		option = 0;
	}
	else if (option == 10)
	{
		if (fan3angle >= 360)
			fan3angle = 0;
		teapotAngle++;
	}
	else if (option == 11)
	{
		if (witleboardPosition >= -600)
			witleboardPosition -= 10;
		option = 0;
	}
	else if (option == 12) 
	{
		if (witleboardPosition <= -350)
			witleboardPosition += 10;
		option = 0;
	}
	else if (option == 13)
	{
		displayer = !displayer;
		option = 0;
	}
	else if (option == 14) 
	{
		if (doorangle2 <= -90)
		{
			doorangle2 = -90;
			option = 0;
		}
		doorangle2--;
	}
	else if (option == 15)
	{
		if (doorangle2 >= -1) 
		{
			doorangle2 = -1;
			option = 0;
		}
		doorangle2++;
	}
	gettime();
	Sleep(10);
	glutPostRedisplay();
}


void main(int argc, char** argv)
{
	/* Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	/* Create a window with title specified */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 1: Tzh's 3D Room");

	init();	/* not GLUT call, initialize several parameters */

	/* Register different CALLBACK function for GLUT to response
	 * with different events, e.g. window sizing, mouse click or
	 * keyboard stroke*/
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	getchar();
}