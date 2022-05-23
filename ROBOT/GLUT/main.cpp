#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include "../freeglut-3.0.0/include/glut.h"
#endif

#include "imageloader.h"

using namespace std;

//-------------------------------------------------------------------
static int window;
static int menu_id;
static int submenu_id;
static int value = 0;
void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		value = num;
	}
	glutPostRedisplay();
}

//--------------------------------------------------------------------------------------------------

#define	PI 3.14159265358979323846f
float v0[3], v1[3];
float mo[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

float clamp(float x, float a, float b) { return x<a ? a : (x<b ? x : b); }
float dot(const float *a, const float *b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
float norm(const float *a) { return sqrtf(dot(a, a)); }
void vassign(float *a, float x, float y, float z) { a[0] = x; a[1] = y; a[2] = z; }
void vassign(float *a, const float *b) { a[0] = b[0]; a[1] = b[1]; a[2] = b[2]; }

void cross(float *a, const float *b, const float *c)
{
	a[0] = b[1] * c[2] - c[1] * b[2];
	a[1] = -b[0] * c[2] + c[0] * b[2];
	a[2] = b[0] * c[1] - c[0] * b[1];
}

void normalize(float *a)
{
	float l = norm(a);
	a[0] /= l; a[1] /= l; a[2] /= l;
}

void mousebutton(int button, int state, int x, int y)
{
	vassign(v0, 2.0*x / 512 - 1, -2.0*y / 512 + 1, 1);
	normalize(v0);
}

void mousemove(int x, int y)
{
	float axis[3], angle;

	vassign(v1, 2.0*x / 512 - 1, -2.0*y / 512 + 1, 1);
	normalize(v1);
	if (dot(v0, v1)>0.999999)
		return;
	cross(axis, v0, v1);
	normalize(axis);
	angle = acosf(clamp(dot(v0, v1), -1, 1));
	vassign(v0, v1);

	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle * 180 / PI, axis[0], axis[1], axis[2]);
	glMultMatrixf(mo);
	glGetFloatv(GL_MODELVIEW_MATRIX, mo);
	glPopMatrix();
	glutPostRedisplay();
}

//-------------------------------------------------------------------

const float BOX_SIZE = 7.0f; //The length of each side of the cube
const float BOX_HEIGHT = BOX_SIZE; //The height of the box off of the ground
const float FLOOR_SIZE = 30.0f; //The length of each side of the floor



float anklef1a[2], anklef1x[2], anklef1y[2], anklef1z[2];	//拇指
float anklef2a[3], anklef2x[3], anklef2y[3], anklef2z[3];	//食指
float anklef3a[3], anklef3x[3], anklef3y[3], anklef3z[3];	//中指
float anklef4a[3], anklef4x[3], anklef4y[3], anklef4z[3];	//無名指
float anklef5a[3], anklef5x[3], anklef5y[3], anklef5z[3];	//小指
float anklef6a[2], anklef6x[2], anklef6y[2], anklef6z[2];//左腿關節
float anklef7a[2], anklef7x[2], anklef7y[2], anklef7z[2];//右腿關節
float anklef8a[6], anklef8x[6], anklef8y[6], anklef8z[6];//右手關節

float anklef1a_2[2], anklef1x_2[2], anklef1y_2[2], anklef1z_2[2];	//拇指
float anklef2a_2[3], anklef2x_2[3], anklef2y_2[3], anklef2z_2[3];	//食指
float anklef3a_2[3], anklef3x_2[3], anklef3y_2[3], anklef3z_2[3];	//中指
float anklef4a_2[3], anklef4x_2[3], anklef4y_2[3], anklef4z_2[3];	//無名指
float anklef5a_2[3], anklef5x_2[3], anklef5y_2[3], anklef5z_2[3];	//小指
float anklef8a_2[6], anklef8x_2[6], anklef8y_2[6], anklef8z_2[6];//左手關節
//	  ^^^轉動量		^^^轉動軸x		^^^轉動軸y		^^^轉動軸z
float neckangle = 0, neckangle_x = 0, neckangle_y = 0, neckangle_z = 0;
float bodyangle[2], bodyangle_x[2], bodyangle_y[2], bodyangle_z[2];
float bodymove_x = 0, bodymove_y = 0, bodymove_z = 0;//身體轉動軸
float move_x = 0, move_y = 0, move_z = 0;


int walk_right_hand_up_0 = 1;
int walk_right_hand_down_0 = 0;
int walk_right_hand_up_1 = 1;
int walk_right_hand_down_1 = 0;
int walk_right_hand_up_2 = 1;
int walk_right_hand_down_2 = 0;

int walk_left_hand_up_0 = 0;
int walk_left_hand_down_0 = 1;
int walk_left_hand_up_1 = 0;
int walk_left_hand_down_1 = 1;
int walk_left_hand_up_2 = 0;
int walk_left_hand_down_2 = 1;

int walk_right_leg_up_0 = 1;
int walk_right_leg_down_0 = 0;
int walk_right_leg_up_1 = 0;
int walk_right_leg_down_1 = 1;
int walk_right_leg_delay_count = 0;


int walk_left_leg_up_0 = 1;
int walk_left_leg_down_0 = 0;
int walk_left_leg_up_1 = 0;
int walk_left_leg_down_1 = 1;
int walk_left_leg_time_balance = 0;

int jump_hand_up_0 = 1;
int jump_hand_down_0 = 0;

int bye_raise_hand = 0;

int pushup_body_up = 0;
int pushup_body_down = 1;
int counter = 0;

int state = 1;

void standby() 
{
	state = 1;

	move_x = 0;
	move_y = 4;
	move_z = 0;

	neckangle = 0;
	neckangle_x = 0;
	neckangle_y = 0;
	neckangle_z = 0;

	bodyangle[0] = 0;
	bodyangle_x[0] = 0;
	bodyangle_y[0] = 0;
	bodyangle_z[0] = 0;

	bodyangle[1] = 0;
	bodyangle_x[1] = 0;
	bodyangle_y[1] = 0;
	bodyangle_z[1] = 0;

	bodymove_x = 0;
	bodymove_y = 0;
	bodymove_z = 0;
	//-----------------------------------------	
	anklef1a[0] = -10;		//母指接掌
	anklef1x[0] = 0;
	anklef1y[0] = 0;
	anklef1z[0] = 1;

	anklef1a[1] = 15;		//母指1
	anklef1x[1] = 0;
	anklef1y[1] = 0;
	anklef1z[1] = 1;


	//-----------------------------------------	
	anklef2a[0] = -5;		//食指接掌
	anklef2x[0] = 0;
	anklef2y[0] = 0;
	anklef2z[0] = 1;

	anklef2a[1] = 10;	//食指1
	anklef2x[1] = 0;
	anklef2y[1] = 0;
	anklef2z[1] = 1;

	anklef2a[2] = 5;	//食指2
	anklef2x[2] = 0;
	anklef2y[2] = 0;
	anklef2z[2] = 1;

	//-----------------------------------------	
	anklef3a[0] = 0;		//中指接掌
	anklef3x[0] = 0;
	anklef3y[0] = 0;
	anklef3z[0] = 1;

	anklef3a[1] = 0;	//中指1
	anklef3x[1] = 0;
	anklef3y[1] = 0;
	anklef3z[1] = 1;

	anklef3a[2] = 0;	//中指2
	anklef3x[2] = 0;
	anklef3y[2] = 0;
	anklef3z[2] = 1;

	//-----------------------------------------	
	anklef4a[0] = 5;		//無名指接掌
	anklef4x[0] = 0;
	anklef4y[0] = 0;
	anklef4z[0] = 1;

	anklef4a[1] = -10;	//無名指1
	anklef4x[1] = 0;
	anklef4y[1] = 0;
	anklef4z[1] = 1;

	anklef4a[2] = -7;	//無名指2
	anklef4x[2] = 0;
	anklef4y[2] = 0;
	anklef4z[2] = 1;

	//-----------------------------------------	
	anklef5a[0] = 15;		//小母指接掌
	anklef5x[0] = 0;
	anklef5y[0] = 0;
	anklef5z[0] = 1;

	anklef5a[1] = -10;	//小母指1
	anklef5x[1] = 0;
	anklef5y[1] = 0;
	anklef5z[1] = 1;

	anklef5a[2] = -8;	//小母指2
	anklef5x[2] = 0;
	anklef5y[2] = 0;
	anklef5z[2] = 1;

	anklef1a_2[0] = -10;		//母指接掌
	anklef1x_2[0] = 0;
	anklef1y_2[0] = 0;
	anklef1z_2[0] = 1;

	anklef1a_2[1] = 15;		//母指1
	anklef1x_2[1] = 0;
	anklef1y_2[1] = 0;
	anklef1z_2[1] = 1;


	//-----------------------------------------	
	anklef2a_2[0] = -5;		//食指接掌
	anklef2x_2[0] = 0;
	anklef2y_2[0] = 0;
	anklef2z_2[0] = 1;

	anklef2a_2[1] = 10;	//食指1
	anklef2x_2[1] = 0;
	anklef2y_2[1] = 0;
	anklef2z_2[1] = 1;

	anklef2a_2[2] = 5;	//食指2
	anklef2x_2[2] = 0;
	anklef2y_2[2] = 0;
	anklef2z_2[2] = 1;

	//-----------------------------------------	
	anklef3a_2[0] = 0;		//中指接掌
	anklef3x_2[0] = 0;
	anklef3y_2[0] = 0;
	anklef3z_2[0] = 1;

	anklef3a_2[1] = 0;	//中指1
	anklef3x_2[1] = 0;
	anklef3y_2[1] = 0;
	anklef3z_2[1] = 1;

	anklef3a_2[2] = 0;	//中指2
	anklef3x_2[2] = 0;
	anklef3y_2[2] = 0;
	anklef3z_2[2] = 1;

	//-----------------------------------------	
	anklef4a_2[0] = 5;		//無名指接掌
	anklef4x_2[0] = 0;
	anklef4y_2[0] = 0;
	anklef4z_2[0] = 1;

	anklef4a_2[1] = -10;	//無名指1
	anklef4x_2[1] = 0;
	anklef4y_2[1] = 0;
	anklef4z_2[1] = 1;

	anklef4a_2[2] = -7;	//無名指2
	anklef4x_2[2] = 0;
	anklef4y_2[2] = 0;
	anklef4z_2[2] = 1;

	//-----------------------------------------	
	anklef5a_2[0] = 15;		//小母指接掌
	anklef5x_2[0] = 0;
	anklef5y_2[0] = 0;
	anklef5z_2[0] = 1;

	anklef5a_2[1] = -10;	//小母指1
	anklef5x_2[1] = 0;
	anklef5y_2[1] = 0;
	anklef5z_2[1] = 1;

	anklef5a_2[2] = -8;	//小母指2
	anklef5x_2[2] = 0;
	anklef5y_2[2] = 0;
	anklef5z_2[2] = 1;
	//-----------------------------------------	
	anklef6a[0] = 0;  //左腿
	anklef6x[0] = 0;
	anklef6y[0] = 0;
	anklef6z[0] = 0;

	anklef6a[1] = 0;
	anklef6x[1] = 0;
	anklef6y[1] = 0;
	anklef6z[1] = 0;
	//-----------------------------------------	
	anklef7a[0] = 0;   //右腿
	anklef7x[0] = 0;
	anklef7y[0] = 0;
	anklef7z[0] = 0;

	anklef7a[1] = 0;
	anklef7x[1] = 0;
	anklef7y[1] = 0;
	anklef7z[1] = 0;
	//-----------------------------------------	
	anklef8a[0] = 0;		//肩關節
	anklef8x[0] = 0;
	anklef8y[0] = 0;
	anklef8z[0] = 0;

	anklef8a[1] = 0;	//肘關節
	anklef8x[1] = 0;
	anklef8y[1] = 0;
	anklef8z[1] = 0;

	anklef8a[2] = 0;	//手腕關節
	anklef8x[2] = 0;
	anklef8y[2] = 0;
	anklef8z[2] = 0;

	anklef8a[3] = 0;	//手腕關節
	anklef8x[3] = 0;
	anklef8y[3] = 0;
	anklef8z[3] = 0;

	anklef8a[4] = 0;	//肩膀關節
	anklef8x[4] = 0;
	anklef8y[4] = 0;
	anklef8z[4] = 0;

	anklef8a[5] = 0;	//手肘關節
	anklef8x[5] = 0;
	anklef8y[5] = 0;
	anklef8z[5] = 0;

	//-----------------------------------------	
	anklef8a_2[0] = 0;		//肩關節
	anklef8x_2[0] = 0;
	anklef8y_2[0] = 0;
	anklef8z_2[0] = 0;

	anklef8a_2[1] = 0;	//肘關節
	anklef8x_2[1] = 0;
	anklef8y_2[1] = 0;
	anklef8z_2[1] = 0;

	anklef8a_2[2] = 0;	//手腕關節
	anklef8x_2[2] = 0;
	anklef8y_2[2] = 0;
	anklef8z_2[2] = 0;

	anklef8a_2[3] = 0;	//手腕關節
	anklef8x_2[3] = 0;
	anklef8y_2[3] = 0;
	anklef8z_2[3] = 0;

	anklef8a_2[4] = 0;	//肩膀關節
	anklef8x_2[4] = 0;
	anklef8y_2[4] = 0;
	anklef8z_2[4] = 0;

	anklef8a_2[5] = 0;	//手肘關節
	anklef8x_2[5] = 0;
	anklef8y_2[5] = 0;
	anklef8z_2[5] = 0;

	//-------------------------control units-------------------------------------
	 walk_right_hand_up_0 = 1;
	 walk_right_hand_down_0 = 0;
	 walk_right_hand_up_1 = 1;
	 walk_right_hand_down_1 = 0;
	 walk_right_hand_up_2 = 1;
	 walk_right_hand_down_2 = 0;

	 walk_left_hand_up_0 = 0;
	 walk_left_hand_down_0 = 1;
	 walk_left_hand_up_1 = 0;
	 walk_left_hand_down_1 = 1;
	 walk_left_hand_up_2 = 0;
	 walk_left_hand_down_2 = 1;

	 walk_right_leg_up_0 = 1;
	 walk_right_leg_down_0 = 0;
	 walk_right_leg_up_1 = 0;
	 walk_right_leg_down_1 = 1;
	 walk_right_leg_delay_count = 0;


	 walk_left_leg_up_0 = 1;
	 walk_left_leg_down_0 = 0;
	 walk_left_leg_up_1 = 0;
	 walk_left_leg_down_1 = 1;
	 walk_left_leg_time_balance = 0;

	 jump_hand_up_0 = 1;
	 jump_hand_down_0 = 0;

	 pushup_body_up = 0;
	 pushup_body_down = 1;

	 bye_raise_hand = 0;

	 counter = 0;
}

//Draws the cube
void drawCube(int textureId) {
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();


	glBegin(GL_QUADS);

	//Top face
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, 1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);

	//Bottom face
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, -1.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);

	//Left face
	glNormal3f(-1.0, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);

	//Right face
	glNormal3f(1.0, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	//Front face
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, BOX_SIZE / 2);

	//Back face
	glNormal3f(0.0, 0.0f, -1.0f);
	glVertex3f(-BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(-BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, BOX_SIZE / 2, -BOX_SIZE / 2);
	glVertex3f(BOX_SIZE / 2, -BOX_SIZE / 2, -BOX_SIZE / 2);

	glEnd();

	glPopMatrix();
}

//Draws the floor
void drawFloor() {


	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glColor4f(0.5, 0.5, 0.5, 0.7f);
	glVertex3f(-FLOOR_SIZE, -2, FLOOR_SIZE);

	glVertex3f(-FLOOR_SIZE, -2, -FLOOR_SIZE);

	glVertex3f(FLOOR_SIZE, -2, -FLOOR_SIZE);

	glVertex3f(FLOOR_SIZE, -2, FLOOR_SIZE);

	glEnd();
}

GLuint _textureId;
GLuint _textureId2;
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width, image->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image->pixels);
	return textureId;
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Image* image = loadBMP("face.bmp");
	Image* image2 = loadBMP("muscle2.bmp");
	_textureId = loadTexture(image);
	_textureId2 = loadTexture(image2);
	delete image;
	delete image2;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (float)w / (float)h, 1.0, 200.0);
}

void drawleftleg()
{
	glPushMatrix();
	glRotatef(anklef6a[0], anklef6x[0], anklef6y[0], anklef6z[0]);
	glPushMatrix();  //屁股關節
	glScalef(0.15, 0.15, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, -0.37, 0);
	glPushMatrix();//大腿
	glScalef(0.15, 0.3, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, -0.42, 0);
	glRotatef(anklef6a[1], anklef6x[1], anklef6y[1], anklef6z[1]);
	glPushMatrix();  //膝蓋關節
	glScalef(0.15, 0.15, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0.0, -0.47, 0);
	glPushMatrix();  //小腿
	glScalef(0.15, 0.4, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, -0.6, 0);
	glPushMatrix();//腳
	glScalef(0.08, 0.08, 0.15);
	drawCube(_textureId);
	glPopMatrix();

	glPopMatrix();//pop整隻腳
}
void drawrightleg()
{
	glPushMatrix();
	glRotatef(anklef7a[0], anklef7x[0], anklef7y[0], anklef7z[0]);
	glPushMatrix();  //屁股關節
	glScalef(0.15, 0.15, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, -0.37, 0);
	glPushMatrix();//大腿
	glScalef(0.15, 0.3, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, -0.42, 0);
	glRotatef(anklef7a[1], anklef7x[1], anklef7y[1], anklef7z[1]);
	glPushMatrix();  //膝蓋關節
	glScalef(0.15, 0.15, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0.0, -0.47, 0);
	glPushMatrix();  //小腿
	glScalef(0.15, 0.4, 0.15);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, -0.6, 0);
	glPushMatrix();//腳
	glScalef(0.08, 0.08, 0.15);
	drawCube(_textureId);
	glPopMatrix();

	glPopMatrix();//pop整隻腳
}

void drawrighthand()
{
	glPushMatrix();
	glTranslatef(-3, 1, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(anklef8a[0], anklef8x[0], anklef8y[0], anklef8z[0]);
	

	glPushMatrix();  //肩關節
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肩關節

	glTranslatef(-1.1, 0, 0);

	glPushMatrix();  //上臂
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1, 0.35, 0.35);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop上臂

	glTranslatef(-1.2, 0, 0);
	glRotatef(anklef8a[1], anklef8x[1], anklef8y[1], anklef8z[1]);

	glPushMatrix();  //肘關節
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肘關節

	glTranslatef(-1.2, 0, 0);

	glPushMatrix();  //前臂
	glColor3f(1.0, 1.0, 1.0);
	//glColor3ub(219, 156, 53);
	glScalef(1, 0.35, 0.35);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop前臂

	glTranslatef(-1.2, 0, 0);
	glRotatef(anklef8a[2], anklef8x[2], anklef8y[2], anklef8z[2]);
	glRotatef(anklef8a[5], anklef8x[5], anklef8y[5], anklef8z[5]);
	glRotatef(anklef8a[3], anklef8x[3], anklef8y[3], anklef8z[3]);

	glPushMatrix();  //手腕關節
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肘關節

	glTranslatef(-0.6, 0, 0);

	glPushMatrix();  //手掌
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肘關節

	glTranslatef(3.7, -3, -5);

	//222222222222
	glPushMatrix();//食指接掌關節
				   //------------------------------------------------------------食指
	glTranslatef(-4, 3.3, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef2a[0], anklef2x[0], anklef2y[0], anklef2z[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//食指節3

	glPushMatrix();
	glScalef(1.2, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//食指中關節
	glRotatef(anklef2a[1], anklef2x[1], anklef2y[1], anklef2z[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.0, 0, 0.0);//食指節2

	glPushMatrix();
	glScalef(1, 0.6, 1);
	glColor3f(1.0, 0, 0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0.0);//食指上關節
	glRotatef(anklef2a[2], anklef2x[2], anklef2y[2], anklef2z[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //食指節3

	glPushMatrix();
	glRotatef(20.0, 0, 1, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glColor3f(0, 0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop食指



				  //111111111111

	glPushMatrix();//母指接掌關節
				   //------------------------------------------------------------母指
	glTranslatef(-3.7, 3.5, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef1a[0], anklef1x[0], anklef1y[0], anklef1z[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//母指節1

	glPushMatrix();
	glScalef(1.0, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.5, 0, 0);//母指中關節
	glRotatef(anklef1a[1], anklef1x[1], anklef1y[1], anklef1z[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();


	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //母指節2

	glPushMatrix();
	glRotatef(20.0, 0, 1, 1.0);
	glScalef(1.0, 0.8, 1.0);
	glColor3f(1.0, 0, 0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop拇指

				  //33333333333333

	glPushMatrix();//中指接掌關節
				   //------------------------------------------------------------中指
	glTranslatef(-4.2, 3, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef3a[0], anklef3x[0], anklef3y[0], anklef3z[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//中指節1

	glPushMatrix();
	glScalef(1.5, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4.5, 0, 0);//中指中關節
	glRotatef(anklef3a[1], anklef3x[1], anklef3y[1], anklef3z[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.0, 0, 0.0);//中指節2

	glPushMatrix();
	glScalef(1, 0.6, 1);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0.0);//中指上關節
	glRotatef(anklef3a[2], anklef3x[2], anklef3y[2], anklef3z[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //中指節3

	glPushMatrix();
	glRotatef(10.0, 0, 1, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop中指



				  //444444444444

	glPushMatrix();//無名指接掌關節
				   //------------------------------------------------------------無名指
	glTranslatef(-4.1, 2.7, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef4a[0], anklef4x[0], anklef4y[0], anklef4z[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//無名指節1

	glPushMatrix();
	glScalef(1.5, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);

	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4.5, 0, 0);//無名指中關節
	glRotatef(anklef4a[1], anklef4x[1], anklef4y[1], anklef4z[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.0, 0, 0.0);//無名指節2

	glPushMatrix();
	glScalef(1, 0.6, 1);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0.0);//無名指上關節
	glRotatef(anklef4a[2], anklef4x[2], anklef4y[2], anklef4z[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //無名指節3

	glPushMatrix();
	glRotatef(10.0, 0, 1, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop無名指

				  //55555555555

	glPushMatrix();//小母指接掌關節
				   //------------------------------------------------------------小母指
	glTranslatef(-3.9, 2.5, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef5a[0], anklef5x[0], anklef5y[0], anklef5z[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.5, 0, 0);//小母指節1

	glPushMatrix();
	glScalef(0.7, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0);//小母指中關節
	glRotatef(anklef5a[1], anklef5x[1], anklef5y[1], anklef5z[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.0, 0, 0.0);//小母指節2

	glPushMatrix();
	glScalef(0.7, 0.6, 1);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0, 0.0);//小母指上關節
	glRotatef(anklef5a[2], anklef5x[2], anklef5y[2], anklef5z[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.3, 0.0, 0.0); //小母指節3

	glPushMatrix();
	glRotatef(20.0, 0, 1, 1.0);
	glScalef(0.8, 0.6, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop小拇指

	glPopMatrix();
}
void drawlefthand()
{
	glPushMatrix();
	glTranslatef(-3, 1, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(anklef8a_2[0], anklef8x_2[0], anklef8y_2[0], anklef8z_2[0]);

	glPushMatrix();  //肩關節
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肩關節

	glTranslatef(-1.1, 0, 0);

	glPushMatrix();  //上臂
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1, 0.35, 0.35);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop上臂

	glTranslatef(-1.2, 0, 0);
	glRotatef(anklef8a_2[1], anklef8x_2[1], anklef8y_2[1], anklef8z_2[1]);

	glPushMatrix();  //肘關節
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肘關節

	glTranslatef(-1.2, 0, 0);

	glPushMatrix();  //前臂
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1, 0.35, 0.35);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop前臂

	glTranslatef(-1.2, 0, 0);
	glRotatef(anklef8a_2[2], anklef8x_2[2], anklef8y_2[2], anklef8z_2[2]);
	glRotatef(anklef8a_2[5], anklef8x_2[5], anklef8y_2[5], anklef8z_2[5]);
	glRotatef(anklef8a_2[3], anklef8x_2[3], anklef8y_2[3], anklef8z_2[3]);

	glPushMatrix();  //手腕關節
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.3, 0.3, 0.3);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肘關節

	glTranslatef(-0.6, 0, 0);

	glPushMatrix();  //手掌
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();//pop肘關節

	glTranslatef(3.7, -3, -5);

	//222222222222
	glPushMatrix();//食指接掌關節
				   //------------------------------------------------------------食指
	glTranslatef(-4, 3.3, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef2a_2[0], anklef2x_2[0], anklef2y_2[0], anklef2z_2[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//食指節3

	glPushMatrix();
	glScalef(1.2, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//食指中關節
	glRotatef(anklef2a_2[1], anklef2x_2[1], anklef2y_2[1], anklef2z_2[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.0, 0, 0.0);//食指節2

	glPushMatrix();
	glScalef(1, 0.6, 1);
	glColor3f(1.0, 0, 0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0.0);//食指上關節
	glRotatef(anklef2a_2[2], anklef2x_2[2], anklef2y_2[2], anklef2z_2[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //食指節3

	glPushMatrix();
	glRotatef(20.0, 0, 1, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glColor3f(0, 0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop食指



				  //111111111111

	glPushMatrix();//母指接掌關節
				   //------------------------------------------------------------母指
	glTranslatef(-3.7, 3.5, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef1a_2[0], anklef1x_2[0], anklef1y_2[0], anklef1z_2[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//母指節1

	glPushMatrix();
	glScalef(1.0, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.5, 0, 0);//母指中關節
	glRotatef(anklef1a_2[1], anklef1x_2[1], anklef1y_2[1], anklef1z_2[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();


	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //母指節2

	glPushMatrix();
	glRotatef(20.0, 0, 1, 1.0);
	glScalef(1.0, 0.8, 1.0);
	glColor3f(1.0, 0, 0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop拇指

				  //33333333333333

	glPushMatrix();//中指接掌關節
				   //------------------------------------------------------------中指
	glTranslatef(-4.2, 3, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef3a_2[0], anklef3x_2[0], anklef3y_2[0], anklef3z_2[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//中指節1

	glPushMatrix();
	glScalef(1.5, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4.5, 0, 0);//中指中關節
	glRotatef(anklef3a_2[1], anklef3x_2[1], anklef3y_2[1], anklef3z_2[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.0, 0, 0.0);//中指節2

	glPushMatrix();
	glScalef(1, 0.6, 1);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0.0);//中指上關節
	glRotatef(anklef3a_2[2], anklef3x_2[2], anklef3y_2[2], anklef3z_2[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //中指節3

	glPushMatrix();
	glRotatef(10.0, 0, 1, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop中指



				  //444444444444

	glPushMatrix();//無名指接掌關節
				   //------------------------------------------------------------無名指
	glTranslatef(-4.1, 2.7, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef4a_2[0], anklef4x_2[0], anklef4y_2[0], anklef4z_2[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4, 0, 0);//無名指節1

	glPushMatrix();
	glScalef(1.5, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-4.5, 0, 0);//無名指中關節
	glRotatef(anklef4a_2[1], anklef4x_2[1], anklef4y_2[1], anklef4z_2[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.0, 0, 0.0);//無名指節2

	glPushMatrix();
	glScalef(1, 0.6, 1);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0.0);//無名指上關節
	glRotatef(anklef4a_2[2], anklef4x_2[2], anklef4y_2[2], anklef4z_2[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0.0, 0.0); //無名指節3

	glPushMatrix();
	glRotatef(10.0, 0, 1, 1.0);
	glScalef(1.0, 0.6, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop無名指

				  //55555555555

	glPushMatrix();//小母指接掌關節
				   //------------------------------------------------------------小母指
	glTranslatef(-3.9, 2.5, 5);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(anklef5a_2[0], anklef5x_2[0], anklef5y_2[0], anklef5z_2[0]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3.5, 0, 0);//小母指節1

	glPushMatrix();
	glScalef(0.7, 0.6, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(2.5, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-3, 0, 0);//小母指中關節
	glRotatef(anklef5a_2[1], anklef5x_2[1], anklef5y_2[1], anklef5z_2[1]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.0, 0, 0.0);//小母指節2

	glPushMatrix();
	glScalef(0.7, 0.6, 1);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(2.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.5, 0, 0.0);//小母指上關節
	glRotatef(anklef5a_2[2], anklef5x_2[2], anklef5y_2[2], anklef5z_2[2]);

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	//-----------------------------
	glTranslatef(-2.3, 0.0, 0.0); //小母指節3

	glPushMatrix();
	glRotatef(20.0, 0, 1, 1.0);
	glScalef(0.8, 0.6, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.7, 30.0, 30.0);
	glPopMatrix();

	glPopMatrix();//pop小拇指

	glPopMatrix();
}


void drawbody()
{
	

	glTranslatef(move_x, move_y, move_z);
	glRotatef(bodyangle[1], bodyangle_x[1], bodyangle_y[1], bodyangle_z[1]);
	glRotatef(bodyangle[0], bodyangle_x[0], bodyangle_y[0], bodyangle_z[0]);
	glPushMatrix();
	glTranslatef(bodymove_x, bodymove_y, bodymove_z);
	glPushMatrix();  //身體
	glScalef(0.6, 0.5, 0.5);
	drawCube(_textureId2);
	glPopMatrix();

	

	glPushMatrix();
	glTranslatef(0, 2.1, 0);
	glRotatef(neckangle, neckangle_x, neckangle_y, neckangle_z);
	glPushMatrix();  //脖子關節
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.8, 30.0, 30.0);
	glPopMatrix();

	glTranslatef(0, 2.2, 0);
	glPushMatrix();  //頭

	//-------hair/////////
	glPushMatrix();
	
	glColor3f(1, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 1);
	glutSolidCone(0.6, 3, 30, 30);
	glTranslatef(0, -1, 0);
	glutSolidCone(0.6, 3, 30, 30);
	glTranslatef(0, 2, 0);
	glutSolidCone(0.6, 3, 30, 30);

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	glRotatef(-30, 1, 0, 0);
	glutSolidCone(0.4, 3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glRotatef(-60, 1, 0, 0);
	glutSolidCone(0.4, 3, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -2.2, 0);
	glRotatef(30, 1, 0, 0);
	glutSolidCone(0.4, 3, 30, 30);
	glPopMatrix();

	glPopMatrix();
	//-------hair/////////

	glScalef(0.8, 0.5, 0.5);
	drawCube(_textureId);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();  //右手
	glScalef(0.8, 0.8, 0.8);
	glRotatef(anklef8a[4], anklef8x[4], anklef8y[4], anklef8z[4]);
	drawrighthand();
	glPopMatrix();

	glPushMatrix();  //左手
	glScalef(-0.8, 0.8, 0.8);
	glRotatef(anklef8a_2[4], anklef8x_2[4], anklef8y_2[4], anklef8z_2[4]);
	drawlefthand();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -1.9, 0);
	glPushMatrix();//右腿
	glScalef(1.3, 1.3, 1.3);
	drawleftleg();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, -1.9, 0);
	glPushMatrix();//左腿
	glScalef(1.3, 1.3, 1.3);
	drawrightleg();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

}



//---------------------------------------------------------------
void walk()
{
	//--------righthand----------------------
	anklef8x[0] = 0;
	anklef8y[0] = 0;
	anklef8z[0] = 1;

	anklef8x[1] = 0;
	anklef8y[1] = 0;
	anklef8z[1] = 1;
	if (anklef8a[0] >= -60 && walk_right_hand_up_0 == 1)
	{
		anklef8a[0] --;
		if (anklef8a[0] == -60)
		{
			walk_right_hand_up_0 = 0;
			walk_right_hand_down_0 = 1;
		}
	}
	if (anklef8a[0] <= 60 && walk_right_hand_down_0 == 1)
	{
		anklef8a[0] ++;
		if (anklef8a[0] == 60)
		{
			walk_right_hand_up_0 = 1;
			walk_right_hand_down_0 = 0;
		}
	}
	//前臂
	if (anklef8a[1] >= -60 && walk_right_hand_up_1 == 1)
	{
		anklef8a[1] --;
	}
	//--------righthand----------------------
	//--------lefthand-----------------------
	anklef8x_2[0] = 0;
	anklef8y_2[0] = 0;
	anklef8z_2[0] = 1;

	anklef8x_2[1] = 0;
	anklef8y_2[1] = 0;
	anklef8z_2[1] = 1;
	if (anklef8a_2[0] <= 60 && walk_left_hand_down_0 == 1)
	{
		anklef8a_2[0] ++;
		if (anklef8a_2[0] == 60)
		{
			walk_left_hand_up_0 = 1;
			walk_left_hand_down_0 = 0;
		}
	}
	if (anklef8a_2[0] >= -60 && walk_left_hand_up_0 == 1)
	{
		anklef8a_2[0] --;
		if (anklef8a_2[0] == -60)
		{
			walk_left_hand_up_0 = 0;
			walk_left_hand_down_0 = 1;
		}
	}
	//前臂
	if (anklef8a_2[1] >= -60 && walk_left_hand_down_1 == 1)
	{
		anklef8a_2[1] --;
	}


	//--------lefthand-----------------------
	//--------leftleg------------------------
	anklef6x[0] = 1;
	anklef6y[0] = 0;
	anklef6z[0] = 0;

	anklef6x[1] = 1;
	anklef6y[1] = 0;
	anklef6z[1] = 0;
	if (walk_left_leg_time_balance == 1)
	{
		//大腿
		if (anklef6a[0] >= -60 && walk_left_leg_up_0 == 1)
		{
			anklef6a[0] -= 0.5;
			if (anklef6a[0] < -60)
			{
				walk_left_leg_up_0 = 0;
				walk_left_leg_down_0 = 1;
			}
		}
		if (anklef6a[0] <= 0 && walk_left_leg_down_0 == 1)
		{
			anklef6a[0] += 0.5;
			if (anklef6a[0] > 0)
			{
				walk_left_leg_up_0 = 1;
				walk_left_leg_down_0 = 0;
			}
		}
		//小腿
		if (anklef6a[1] <= 60 && walk_left_leg_down_1 == 1)
		{
			anklef6a[1] += 0.5;
			if (anklef6a[1] > 60)
			{
				walk_left_leg_up_1 = 1;
				walk_left_leg_down_1 = 0;
			}
		}
		if (anklef6a[1] >= 0 && walk_left_leg_up_1 == 1)
		{
			anklef6a[1] -= 0.5;
			if (anklef6a[1] < 0)
			{
				walk_left_leg_up_1 = 0;
				walk_left_leg_down_1 = 1;
			}
		}
	}
	else if (walk_left_leg_time_balance == 0)
	{
		//大腿
		if (anklef6a[0] >= -60 && walk_left_leg_up_0 == 1)
		{
			anklef6a[0] --;
			if (anklef6a[0] == -60)
			{
				walk_left_leg_up_0 = 0;
				walk_left_leg_down_0 = 1;
				walk_left_leg_time_balance = 1;
			}
		}
		//小腿
		if (anklef6a[1] <= 60 && walk_left_leg_down_1 == 1)
		{
			anklef6a[1] ++;
			if (anklef6a[1] == 60)
			{
				walk_left_leg_up_1 = 1;
				walk_left_leg_down_1 = 0;
			}
		}
	}


	//--------leftleg------------------------
	//--------rightleg-----------------------
	anklef7x[0] = 1;
	anklef7y[0] = 0;
	anklef7z[0] = 0;

	anklef7x[1] = 1;
	anklef7y[1] = 0;
	anklef7z[1] = 0;
	if (walk_right_leg_delay_count <= 60)
	{
		walk_right_leg_delay_count++;
	}
	if (walk_right_leg_delay_count == 61)
	{
		//大腿
		if (anklef7a[0] >= -60 && walk_right_leg_up_0 == 1)
		{
			anklef7a[0] -= 0.5;
			if (anklef7a[0] < -60)
			{
				walk_right_leg_up_0 = 0;
				walk_right_leg_down_0 = 1;
			}
		}
		if (anklef7a[0] <= 0 && walk_right_leg_down_0 == 1)
		{
			anklef7a[0] += 0.5;
			if (anklef7a[0] > 0)
			{
				walk_right_leg_up_0 = 1;
				walk_right_leg_down_0 = 0;
			}
		}
		//小腿
		if (anklef7a[1] <= 60 && walk_right_leg_down_1 == 1)
		{
			anklef7a[1] += 0.5;
			if (anklef7a[1] > 60)
			{
				walk_right_leg_up_1 = 1;
				walk_right_leg_down_1 = 0;
			}
		}
		if (anklef7a[1] >= 0 && walk_right_leg_up_1 == 1)
		{
			anklef7a[1] -= 0.5;
			if (anklef7a[1] < 0)
			{
				walk_right_leg_up_1 = 0;
				walk_right_leg_down_1 = 1;
			}
		}
	}
	//--------rightleg-----------------------
	
}
//---------------------------------------------------------------

void jump()
{
	//--------righthand----------------------
	anklef8x[0] = 0;
	anklef8y[0] = 1;
	anklef8z[0] = 0;

	anklef8x[1] = 0;
	anklef8y[1] = 1;
	anklef8z[1] = 0;

	anklef8x[2] = 1;
	anklef8y[2] = 0;
	anklef8z[2] = 0;
	
	if (anklef8a[0] >= -120 && jump_hand_up_0 == 1)
	{
		anklef8a[0] --;
		if (anklef8a[0] == -120)
		{
			jump_hand_up_0 = 0;
			jump_hand_down_0 = 1;
		}
	}
	if (anklef8a[0] <= 0 && jump_hand_down_0 == 1)
	{
		anklef8a[0] ++;
		if (anklef8a[0] == 0)
		{
			jump_hand_up_0 = 1;
			jump_hand_down_0 = 0;
		}
	}
	//前臂
	if (anklef8a[1] >= -30 && jump_hand_up_0 == 1)
	{
		anklef8a[1] --;
	}
	if (anklef8a[1] <= 0 && jump_hand_down_0 == 1)
	{
		anklef8a[1] ++;
	}
	//手掌
	if (anklef8a[2] >= -160)
	{
		anklef8a[2] --;
	}
	//--------righthand----------------------
	//--------lefthand-----------------------
	anklef8x_2[0] = 0;
	anklef8y_2[0] = 1;
	anklef8z_2[0] = 0;

	anklef8x_2[1] = 0;
	anklef8y_2[1] = 1;
	anklef8z_2[1] = 0;

	anklef8x_2[2] = 1;
	anklef8y_2[2] = 0;
	anklef8z_2[2] = 0;

	if (anklef8a_2[0] >= -120 && jump_hand_up_0 == 1)
	{
		anklef8a_2[0] --;
		if (anklef8a_2[0] == -120)
		{
			jump_hand_up_0 = 0;
			jump_hand_down_0 = 1;
		}
	}
	if (anklef8a_2[0] <= 0 && jump_hand_down_0 == 1)
	{
		anklef8a_2[0] ++;
		if (anklef8a_2[0] == 0)
		{
			jump_hand_up_0 = 1;
			jump_hand_down_0 = 0;
		}
	}
	//前臂
	if (anklef8a_2[1] >= -30 && jump_hand_up_0 == 1)
	{
		anklef8a_2[1] --;
	}
	if (anklef8a_2[1] <= 0 && jump_hand_down_0 == 1)
	{
		anklef8a_2[1] ++;
	}
	//手掌
	if (anklef8a_2[2] >= -160)
	{
		anklef8a_2[2] --;
	}
	//--------lefthand-----------------------
	//--------leftleg------------------------
	anklef6x[0] = 0;
	anklef6y[0] = 0;
	anklef6z[0] = 1;

	anklef6x[1] = 0;
	anklef6y[1] = 0;
	anklef6z[1] = 1;
	//大腿
	if (anklef6a[0] <= 60 && walk_left_leg_up_0 == 1)
	{
		anklef6a[0] += 0.5;
		if (anklef6a[0] > 60)
		{
			walk_left_leg_up_0 = 0;
			walk_left_leg_down_0 = 1;
		}
	}
	if (anklef6a[0] >= 0 && walk_left_leg_down_0 == 1)
	{
		anklef6a[0] -= 0.5;
		if (anklef6a[0] < 0)
		{
			walk_left_leg_up_0 = 1;
			walk_left_leg_down_0 = 0;
		}
	}
	//--------leftleg------------------------
	//--------rightleg-----------------------
	anklef7x[0] = 0;
	anklef7y[0] = 0;
	anklef7z[0] = 1;

	anklef7x[1] = 0;
	anklef7y[1] = 0;
	anklef7z[1] = 1;
	//大腿
	if (anklef7a[0] >= -60 && walk_right_leg_up_0 == 1)
	{
		anklef7a[0] -= 0.5;
		if (anklef7a[0] < -60)
		{
			walk_right_leg_up_0 = 0;
			walk_right_leg_down_0 = 1;
		}
	}
	if (anklef7a[0] <= 0 && walk_right_leg_down_0 == 1)
	{
		anklef7a[0] += 0.5;
		if (anklef7a[0] > 0)
		{
			walk_right_leg_up_0 = 1;
			walk_right_leg_down_0 = 0;
		}
	}
	//--------rightleg-----------------------
	//--------move---------------------------
	if (jump_hand_up_0 == 1)
	{
		move_y += 0.05;
	}
	else if (jump_hand_down_0 == 1)
	{
		move_y -= 0.05;
	}
	//--------move---------------------------
}


//---------------------------------------------------------------
void ballet()
{

	neckangle = -45.0;
	neckangle_x = 0;
	neckangle_y = 1.0;
	neckangle_z = 0;

	anklef8x[0] = 0;
	anklef8y[0] = 1;
	anklef8z[0] = 0;

	anklef8x_2[0] = 0;
	anklef8y_2[0] = -1;
	anklef8z_2[0] = 0;


	anklef6x[0] = 0;
	anklef6y[0] = 0;
	anklef6z[0] = 1;

	bodyangle_x[0] = 0;
	bodyangle_y[0] = -1;
	bodyangle_z[0] = 0;

	bodymove_x = 1;
	if (anklef8a[0] >= -135)
	{
		anklef8a[0] = anklef8a[0] - 3;
	}

	if (anklef8a_2[0] <= 45)
	{
		anklef8a_2[0]++;
	}

	if (anklef6a[0] <= 30)
	{
		anklef6a[0]++;
	}

	if (bodyangle[0] <= 360)
	{
		bodyangle[0]++;
	}
	else
	{
		bodyangle[0] = 0;
	}
}
//---------------------------------------------------------------
void bye()
{
	anklef8x[0] = 0;
	anklef8y[0] = 1;
	anklef8z[0] = 0;

	anklef8x[1] = 0;
	anklef8y[1] = 1;
	anklef8z[1] = 0;
	
	anklef8x[2] = 1;
	anklef8y[2] = 0;
	anklef8z[2] = 0;
	
	//--------raisehand----------------------
	if (anklef8a[0] >= -110 && bye_raise_hand == 0)
	{
		anklef8a[0] --;
		if (anklef8a[0] == -110)
		{
			bye_raise_hand = 1;
		}
	}
	if (anklef8a[2] >= -90 && bye_raise_hand == 0)
	{
		anklef8a[2] --;
	}
	//--------raisehand----------------------
	if (anklef8a[0] >= -130 && bye_raise_hand == 1 && walk_right_hand_up_0 == 1)
	{
		anklef8a[0] --;
		if (anklef8a[0] == -130)
		{
			walk_right_hand_up_0 = 0;
			walk_right_hand_down_0 = 1;
		}
	}
	if (anklef8a[0] <= -90 && walk_right_hand_down_0 == 1)
	{
		anklef8a[0] ++;
		if (anklef8a[0] == -90)
		{
			walk_right_hand_up_0 = 1;
			walk_right_hand_down_0 = 0;
		}
	}
	//前臂
	if (anklef8a[1] >= -20 && bye_raise_hand == 1 && walk_right_hand_up_0 == 1)
	{
		anklef8a[1] -= 0.6;
		
	}
	if (anklef8a[1] <= 0 && walk_right_hand_down_0 == 1)
	{
		anklef8a[1] += 0.6;
	
	}
	
	
}
//---------------------------------------------------------------
void monster()
{

	bodyangle[0] = 60;
	bodyangle_x[0] = 0;
	bodyangle_y[0] = 1;
	bodyangle_z[0] = 0;

	anklef8x[0] = 0;
	anklef8y[0] = 0;
	anklef8z[0] = 1;

	anklef8a[2] = 90;
	anklef8x[2] = 1;
	anklef8y[2] = 0;
	anklef8z[2] = 0;

	anklef8x[3] = 0;
	anklef8y[3] = 1;
	anklef8z[3] = 0;

	anklef8x_2[0] = 0;
	anklef8y_2[0] = 0;
	anklef8z_2[0] = 1;

	anklef8a_2[2] = 90;
	anklef8x_2[2] = 1;
	anklef8y_2[2] = 0;
	anklef8z_2[2] = 0;

	anklef8x_2[3] = 0;
	anklef8y_2[3] = 1;
	anklef8z_2[3] = 0;

	anklef6x[0] = 1;
	anklef6y[0] = 0;
	anklef6z[0] = 0;

	anklef6x[1] = 1;
	anklef6y[1] = 0;
	anklef6z[1] = 0;

	anklef7x[0] = 1;
	anklef7y[0] = 0;
	anklef7z[0] = 0;

	anklef7x[1] = 1;
	anklef7y[1] = 0;
	anklef7z[1] = 0;

	if (pushup_body_down == 1)
	{
		bodymove_y -= 0.031;
		pushup_body_down = 1;
		if (bodymove_y <= -1.35)
		{
			pushup_body_down = 0;
		}
	}
	if (pushup_body_down == 0)
	{
		bodymove_y += 0.031;
		if (bodymove_y >= 0.0)
		{
			pushup_body_down = 1;
		}
	}



	if (anklef8a[0] >= -135 && walk_right_hand_up_0 == 1)
	{
		anklef8a[0] = anklef8a[0] - 3;
		if (anklef8a[0] == -135)
		{
			walk_right_hand_up_0 = 0;
			walk_right_hand_down_0 = 1;
		}
	}
	if (anklef8a[0] <= 0 && walk_right_hand_down_0 == 1)
	{
		anklef8a[0] = anklef8a[0] + 3;
		if (anklef8a[0] == 0)
		{
			walk_right_hand_up_0 = 1;
			walk_right_hand_down_0 = 0;
		}
	}

	if (anklef8a_2[0] >= -135 && walk_left_hand_up_0 == 0)
	{
		anklef8a_2[0] = anklef8a_2[0] - 3;
		if (anklef8a_2[0] == -135)
		{
			walk_left_hand_up_0 = 1;
			walk_left_hand_down_0 = 0;
		}
	}
	if (anklef8a_2[0] <= 0 && walk_left_hand_down_0 == 0)
	{
		anklef8a_2[0] = anklef8a_2[0] + 3;
		if (anklef8a_2[0] == 0)
		{
			walk_left_hand_up_0 = 0;
			walk_left_hand_down_0 = 1;
		}
	}

	if (anklef8a[3] <= 45 && walk_right_hand_down_1 == 0)
	{
		anklef8a[3] ++;
		if (anklef8a[3] == 45)
		{
			walk_right_hand_down_1 = 1;
			walk_right_hand_up_1 = 0;
		}
	}
	if (anklef8a[3] >= 0 && walk_right_hand_up_1 == 0)
	{
		anklef8a[3] --;
		if (anklef8a[3] == 0)
		{
			walk_right_hand_up_1 = 1;
			walk_right_hand_down_1 = 0;
		}
	}

	if (anklef8a_2[3] <= 45 && walk_left_hand_down_1 == 1)
	{
		anklef8a_2[3] ++;
		if (anklef8a_2[3] == 45)
		{
			walk_left_hand_down_1 = 0;
			walk_left_hand_up_1 = 1;
		}
	}
	if (anklef8a_2[3] >= 0 && walk_left_hand_up_1 == 1)
	{
		anklef8a_2[3] --;
		if (anklef8a_2[3] == 0)
		{
			walk_left_hand_up_1 = 0;
			walk_left_hand_down_1 = 1;
		}
	}

	if (anklef7a[0] >= -45 && walk_right_leg_up_0 == 1)
	{
		anklef7a[0] --;
		if (anklef7a[0] == -45)
		{
			walk_right_leg_up_0 = 0;
			walk_right_leg_down_0 = 1;
		}
	}
	if (anklef7a[0] <= 0 && walk_right_leg_down_0 == 1)
	{
		anklef7a[0] ++;
		if (anklef7a[0] == 0)
		{
			walk_right_leg_up_0 = 1;
			walk_right_leg_down_0 = 0;
		}
	}

	if (anklef6a[0] >= -45 && walk_left_leg_down_0 == 0)
	{
		anklef6a[0] --;
		if (anklef6a[0] == -45)
		{
			walk_left_leg_down_0 = 1;
			walk_left_leg_up_0 = 0;
		}
	}
	if (anklef6a[0] <= 0 && walk_left_leg_up_0 == 0)
	{
		anklef6a[0]++;
		if (anklef6a[0] == 0)
		{
			walk_left_leg_down_0 = 0;
			walk_left_leg_up_0 = 1;
		}
	}

	if (anklef7a[1] <= 90 && walk_right_leg_down_1 == 1)
	{
		anklef7a[1] = anklef7a[1] + 2;
		if (anklef7a[1] == 90)
		{
			walk_right_leg_down_1 = 0;
			walk_right_leg_up_1 = 1;
		}
	}
	if (anklef7a[1] >= 0 && walk_right_leg_up_1 == 1)
	{
		anklef7a[1] = anklef7a[1] - 2;
		if (anklef7a[1] == 0)
		{
			walk_right_leg_up_1 = 0;
			walk_right_leg_down_1 = 1;
		}
	}

	if (anklef6a[1] <= 90 && walk_left_leg_down_1 == 1)
	{
		anklef6a[1] = anklef6a[1] + 2;
		if (anklef6a[1] == 90)
		{
			walk_left_leg_down_1 = 0;
			walk_left_leg_up_1 = 1;
		}
	}
	if (anklef6a[1] >= 0 && walk_left_leg_up_1 == 1)
	{
		anklef6a[1] = anklef6a[1] - 2;
		if (anklef6a[1] == 0)
		{
			walk_left_leg_up_1 = 0;
			walk_left_leg_down_1 = 1;
		}
	}

}
//---------------------------------------------------------------
void pushup()
{


	bodyangle[1] = 50;
	bodyangle_x[1] = 1;
	bodyangle_y[1] = 0;
	bodyangle_z[1] = 0;

	bodyangle_x[0] = 1;
	bodyangle_y[0] = 0;
	bodyangle_z[0] = 0;
	move_y = 0;
	bodymove_y = 4;

	anklef8x[0] = 0;
	anklef8y[0] = 0;
	anklef8z[0] = 1;

	anklef8x[1] = 0;
	anklef8y[1] = 0;
	anklef8z[1] = -1;

	anklef8a[2] = 90;
	anklef8x[2] = 1;
	anklef8y[2] = 0;
	anklef8z[2] = 0;

	anklef8x[3] = 0;
	anklef8y[3] = 1;
	anklef8z[3] = 0;

	anklef8a[5] = -90;
	anklef8x[5] = 0;
	anklef8y[5] = 1;
	anklef8z[5] = 0;

	anklef8a[4] = -50;
	anklef8x[4] = 1;
	anklef8y[4] = 0;
	anklef8z[4] = 0;

	anklef8x_2[0] = 0;
	anklef8y_2[0] = 0;
	anklef8z_2[0] = 1;

	anklef8x_2[1] = 0;
	anklef8y_2[1] = 0;
	anklef8z_2[1] = -1;

	anklef8a_2[2] = 90;
	anklef8x_2[2] = 1;
	anklef8y_2[2] = 0;
	anklef8z_2[2] = 0;

	anklef8x_2[3] = 0;
	anklef8y_2[3] = 1;
	anklef8z_2[3] = 0;

	anklef8a_2[5] = -90;
	anklef8x_2[5] = 0;
	anklef8y_2[5] = 1;
	anklef8z_2[5] = 0;

	anklef8a_2[4] = -50;
	anklef8x_2[4] = 1;
	anklef8y_2[4] = 0;
	anklef8z_2[4] = 0;

	if (bodyangle[0] <= 25 && pushup_body_down == 1)
	{
		bodyangle[0] = bodyangle[0] + 0.5;
		if (bodyangle[0] == 25)
		{
			pushup_body_down = 0;
			pushup_body_up = 1;
		}
	}
	if (bodyangle[0] >= 0 && pushup_body_up == 1)
	{
		bodyangle[0] = bodyangle[0] - 0.5;
		if (bodyangle[0] == 0)
		{
			pushup_body_up = 0;
			pushup_body_down = 1;
		}
	}

	if (anklef8a[0] <= 25 && walk_right_hand_down_0 == 0)
	{
		anklef8a[0] = anklef8a[0] + 0.5;
		if (anklef8a[0] == 25)
		{
			walk_right_hand_up_0 = 0;
			walk_right_hand_down_0 = 1;
		}
	}
	if (anklef8a[0] >= 0 && walk_right_hand_up_0 == 0)
	{
		anklef8a[0] = anklef8a[0] - 0.5;
		if (anklef8a[0] == 0)
		{
			walk_right_hand_up_0 = 1;
			walk_right_hand_down_0 = 0;
		}
	}


	if (anklef8a_2[0] <= 25 && walk_left_hand_down_0 == 1)
	{
		anklef8a_2[0] = anklef8a_2[0] + 0.5;
		if (anklef8a_2[0] == 25)
		{
			walk_left_hand_up_0 = 1;
			walk_left_hand_down_0 = 0;
		}
	}
	if (anklef8a_2[0] >= 0 && walk_left_hand_up_0 == 1)
	{
		anklef8a_2[0] = anklef8a_2[0] - 0.5;
		if (anklef8a_2[0] == 0)
		{
			walk_left_hand_up_0 = 0;
			walk_left_hand_down_0 = 1;
		}
	}


	if (anklef8a[1] <= 75 && walk_right_hand_up_1 == 1)
	{
		anklef8a[1] = anklef8a[1] + 1.5;
		if (anklef8a[1] == 75)
		{
			walk_right_hand_down_1 = 1;
			walk_right_hand_up_1 = 0;
		}
	}
	if (anklef8a[1] >= 0 && walk_right_hand_down_1 == 1)
	{
		anklef8a[1] = anklef8a[1] - 1.5;
		if (anklef8a[1] == 0)
		{
			walk_right_hand_up_1 = 1;
			walk_right_hand_down_1 = 0;
		}
	}

	if (anklef8a_2[1] <= 75 && walk_left_hand_up_1 == 0)
	{
		anklef8a_2[1] = anklef8a_2[1] + 1.5;
		if (anklef8a_2[1] == 75)
		{
			walk_left_hand_down_1 = 0;
			walk_left_hand_up_1 = 1;
		}
	}
	if (anklef8a_2[1] >= 0 && walk_left_hand_down_1 == 0)
	{
		anklef8a_2[1] = anklef8a_2[1] - 1.5;
		if (anklef8a_2[1] == 0)
		{
			walk_left_hand_up_1 = 0;
			walk_left_hand_down_1 = 1;
		}
	}


	if (anklef8a[3] <= 25 && walk_right_hand_down_2 == 0)//右關節
	{
		anklef8a[3] = anklef8a[3] + 0.5;
		if (anklef8a[3] == 25)
		{
			walk_right_hand_up_2 = 0;
			walk_right_hand_down_2 = 1;
		}
	}
	if (anklef8a[3] >= 0 && walk_right_hand_up_2 == 0)
	{
		anklef8a[3] = anklef8a[3] - 0.5;
		if (anklef8a[3] == 0)
		{
			walk_right_hand_up_2 = 1;
			walk_right_hand_down_2 = 0;
		}
	}


	if (anklef8a_2[3] <= 25 && walk_left_hand_down_2 == 1)//左關節
	{
		anklef8a_2[3] = anklef8a_2[3] + 0.5;
		if (anklef8a_2[3] == 25)
		{
			walk_left_hand_up_2 = 1;
			walk_left_hand_down_2 = 0;
		}
	}
	if (anklef8a_2[3] >= 0 && walk_left_hand_up_2 == 1)
	{
		anklef8a_2[3] = anklef8a_2[3] - 0.5;
		if (anklef8a_2[3] == 0)
		{
			walk_left_hand_up_2 = 0;
			walk_left_hand_down_2 = 1;
		}
	}

}
//---------------------------------------------------------------



void display(void)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, double(viewport[2]) / viewport[3], 0.1, 100);

	// Clear Screen And Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 10, 50, 0, 0, 0, 0, 1, 0);
	glTranslatef(0, 0, 0);

	glMultMatrixf(mo);
	glPushMatrix();
	drawbody();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -3, 0);
	glScalef(1, -1, 1);
	drawbody();
	glPopMatrix();

	glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
	glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
	glDisable(GL_DEPTH_TEST); //Disable depth testing
	glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
									//Make pixels in the stencil buffer be set to 1 when the stencil test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//Set all of the pixels covered by the floor to be 1 in the stencil buffer
	drawFloor();


	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
	glEnable(GL_DEPTH_TEST); //Enable depth testing
							 //Make the stencil test pass only when the pixel is 1 in the stencil buffer
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
	glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer

								//Blend the floor onto the screen
	glEnable(GL_BLEND);
	glColor4f(0.5, 0.5, 0.5, 0.7f);
	drawFloor();
	glDisable(GL_BLEND);

	glutSwapBuffers();
	glutPostRedisplay();
}

void createMenu(void) {
	submenu_id = glutCreateMenu(menu);
	glutAddMenuEntry("standby", 1);
	glutAddMenuEntry("walk", 2);
	glutAddMenuEntry("jump", 3);
	glutAddMenuEntry("ballet", 4);
	glutAddMenuEntry("bye", 5);
	glutAddMenuEntry("pushup", 6);
	glutAddMenuEntry("monster", 7);
	menu_id = glutCreateMenu(menu);
	glutAddSubMenu("Pose", submenu_id);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void timer(int t)
{
	switch (value)
	{
	case 1:
		standby();
		break;
	case 2:
		walk();
		break;
	case 3:
		jump();
		break;
	case 4:
		ballet();
		break;
	case 5:
		bye();
		break;
	case 6:
		pushup();
		break;
	case 7:
		monster();
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(20, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_ACCUM | GLUT_SINGLE);
	glutInitWindowSize(800, 800);
	standby();
	window=glutCreateWindow("robot");
	initRendering();
	createMenu();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemove);

	glutTimerFunc(20, timer, 0);
	glutMainLoop();
	return EXIT_SUCCESS;
}
