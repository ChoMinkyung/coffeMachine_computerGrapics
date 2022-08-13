#include "bmpfuncs.h"
#include "ObjParser.h"
#include "GL\glext.h"
#include <gl\glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
#include <gl\freeglut.h>

#define PI 3.141592

float width, height;

double radius = 25;
double theta = 0, phi = 0;
double upVector = cos(phi * PI / 180);
double cam[3] = { 0,0,0 }; //카메라 위치

float g_pPosition[8] = { -8, -1.5, -4, -1.53, 1, -0.55, 6, 0 };

int g_nX, g_nY; //마우스 선택 위치
int g_nSelect = 0;

int g_nGLWidth = 500, g_nGLHeight = 500;

bool depthM = true;

int shape_select = 0; //그릴 구성품 선택

int mode = 0; // 모드 선택
int body_textrue = 1;//본체 텍스처 선택
int background_textrue = 1;//배경 선택
int state = 0;//시뮬레이션 단계
bool finish[4] = { false }; //시뮬레이션 단계의 마침 여부 표시
bool helpText = true; //도움말 표시 여부

float filterHolder_z = 6;//필터 홀더의 높이
float filter_z = 6;//필터의 높이
float teapot_x = -3;//주전자의 x
float teapot_y = 2;//주전자의 y
float teapot_z = -0.55;//주전자의 높이
float teapot_angle = 0;//주전자의 각도

/* texture mapping set variable */
GLuint texture_filter;
GLuint texture_filter_holder;
GLuint texture_teapot;
GLuint texture_body;
GLuint texture_table;
GLuint texture_glass;
GLuint texName_background[6];
GLuint texName_background1[6];
GLuint texName_background2[6];
GLuint texName_background3[6];
GLuint texName_background4[6];

/* quadric object 생성 */
GLUquadricObj * qobj = gluNewQuadric();

//object bar
ObjParser *body_open_metal;
ObjParser *body_open_green;
ObjParser *body_open_pink;
ObjParser *body_open_yellow;
ObjParser *body_open_green_yellow;

ObjParser *body_close_on_metal;
ObjParser *body_close_on_green;
ObjParser *body_close_on_pink;
ObjParser *body_close_on_yellow;
ObjParser *body_close_on_green_yellow;

ObjParser *body_close_off_metal;
ObjParser *body_close_off_green;
ObjParser *body_close_off_pink;
ObjParser *body_close_off_yellow;
ObjParser *body_close_off_green_yellow;

ObjParser *filter_holder;
ObjParser *filter;
ObjParser *filter_close;
ObjParser *teapot_open;
ObjParser *teapot_close;
ObjParser *table;
ObjParser *glass;

/* 함수 정리 */
void init(void);
void resize(int, int);
void light_setting(void);
void idle(void);

void picking(int, int);
void draw(void);
void draw_string(void *, const char*, float, float, float, float, float);
void draw_text(void);
void draw_helpText(void);

void draw_detail(void);
void draw_obj(ObjParser *);
void draw_obj_with_texture(ObjParser *objParser, GLuint tmp);
void draw_body(int);
void filterTextureMapping(void);
void filterHolderTextureMapping(void);
void teapotTextureMapping(void);
void bodyTextureMapping(void);
void tableTextureMapping(void);
void glassTextureMapping(void);
void draw_coffee(void);

void drawBackground(void);
void background1MapSetting(void);
void background2MapSetting(void);
void background3MapSetting(void);
void background4MapSetting(void);

void mouse(int, int, int, int);
void mouseWheel(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void special_keyboard(int, int, int);

void sub_menu_function_shape(int);
void sub_menu_function_bodytexture(int);
void sub_menu_function_background(int);
void main_menu_function(int);

int main(int argc, char ** argv) {

	int shape, background, bodyTexture;

	//Window 초기화
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12181837 조민경 GL Program Mini Project");
	init(); //사용자 초기화 함수

	gluQuadricDrawStyle(qobj, GLU_FILL);

	//gluQuadricNormals(qobj, GLU_NONE);
	//gluQuadricNormals(qobj, GLU_FLAT);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	body_open_metal = new ObjParser("obj/body_open_metal.obj");
	body_open_green = new ObjParser("obj/body_open_green.obj");
	body_open_pink = new ObjParser("obj/body_open_pink.obj");
	body_open_yellow = new ObjParser("obj/body_open_yellow.obj");
	body_open_green_yellow = new ObjParser("obj/body_open_green_yellow.obj");

	body_close_on_metal = new ObjParser("obj/body_close_on_metal.obj");
	body_close_on_green = new ObjParser("obj/body_close_on_green.obj");
	body_close_on_pink = new ObjParser("obj/body_close_on_pink.obj");
	body_close_on_yellow = new ObjParser("obj/body_close_on_yellow.obj");
	body_close_on_green_yellow = new ObjParser("obj/body_close_on_green_yellow.obj");

	body_close_off_metal = new ObjParser("obj/body_close_off_metal.obj");
	body_close_off_green = new ObjParser("obj/body_close_off_green.obj");
	body_close_off_pink = new ObjParser("obj/body_close_off_pink.obj");
	body_close_off_yellow = new ObjParser("obj/body_close_off_yellow.obj");
	body_close_off_green_yellow = new ObjParser("obj/body_close_off_green_yellow.obj");

	filter_holder = new ObjParser("obj/filter_holder.obj");
	filter = new ObjParser("obj/filter.obj");
	filter_close = new ObjParser("obj/filter_close.obj");
	teapot_open = new ObjParser("obj/teapot_open.obj");
	teapot_close = new ObjParser("obj/teapot_close.obj");
	table = new ObjParser("obj/table.obj");
	glass = new ObjParser("obj/glass.obj");


	//shape 바꾸는 submenu
	shape = glutCreateMenu(sub_menu_function_shape);
	glutAddMenuEntry("Filter Holder", 1);
	glutAddMenuEntry("Filter", 2);
	glutAddMenuEntry("Teapot", 3);
	glutAddMenuEntry("Body", 4);

	bodyTexture = glutCreateMenu(sub_menu_function_bodytexture);
	glutAddMenuEntry("Metal", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Pink", 3);
	glutAddMenuEntry("Yellow", 4);
	glutAddMenuEntry("Green&Yellow", 5);

	background = glutCreateMenu(sub_menu_function_background);
	glutAddMenuEntry("Yokohama", 1);
	glutAddMenuEntry("Sky", 2);
	glutAddMenuEntry("Night Sky", 3);
	glutAddMenuEntry("Lycksele", 4);
	glutAddMenuEntry("No Background", 5);

	glutCreateMenu(main_menu_function);

	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Init", 1);
	glutAddSubMenu("Component", shape);
	glutAddSubMenu("Body Texture", bodyTexture);
	glutAddSubMenu("Background", background);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Callback 함수 정의
	glutDisplayFunc(draw); // draw : 실제 그리기 함수
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);// mouse : 마우스 입력 처리
	glutSpecialFunc(special_keyboard); //방향키 사용
	glutMouseWheelFunc(mouseWheel);//마우스 휠 사용 - 헤더파일 추가
	glutMotionFunc(motion); // motion : 마우스 움직임 처리
	glutKeyboardFunc(keyboard); // keyboard : 키보드 입력 처리

	glutIdleFunc(idle);

	//Looping 시작
	glutMainLoop();

	return 0;
}

//사용자 초기화 함수
void init(void) {
	//화면의 기본색으로 black 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* resize 콜백함수 지정 */
	glutReshapeFunc(resize);

	/* light setting */
	light_setting();

	filterTextureMapping();
	filterHolderTextureMapping();
	teapotTextureMapping();
	bodyTextureMapping();
	tableTextureMapping();

	background1MapSetting();
	background2MapSetting();
	background3MapSetting();
	background4MapSetting();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

void light_setting(void) {
	//Light 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_specular[] = { 0.9f, 0.9f, 0.9f, 0.9f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Depth Test Enable
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void idle(void) {
	if (state == 1 && filterHolder_z > 2.05) filterHolder_z -= 0.007;
	if (state == 2 && filter_z > 2.75) filter_z -= 0.007;

	if (state == 4 && teapot_z < -0.1)teapot_z += 0.05;
	if (state == 4 && teapot_z > -0.1 && teapot_x < 0) teapot_x += 0.05;
	if (state == 4 && teapot_z > -0.1 && teapot_x > 0 && teapot_y > 1.65) teapot_y -= 0.05;

	if (state == 6 && teapot_y < 2.2) teapot_y += 0.05;
	if (state == 6 && teapot_y > 2.2 && teapot_x > -3.1) teapot_x -= 0.05;
	if (state == 6 && teapot_y > 2.2 && teapot_x < -3.1 && teapot_z > -0.7) teapot_z -= 0.05;
	glutPostRedisplay();
}

// 문자 출력하는 함수
void draw_string(void *font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);//2D
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10); // 이제 화면의 좌표는  (-5, -5) ~ (5, 5)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void draw_text() {
	char *str = (char *)"MODE : PRODUCT DETAIL";
	char *str2 = (char *)" ";
	char *str3 = (char *)" ";
	char *str4 = (char *)" ";

	switch (mode)
	{
	case 0:
		str = (char *)"MODE : PRODUCT DETAIL";
		break;
	case 1:
		str = (char *)"MODE : SIMULATION";
		if (state == 0) str2 = (char *)"STATE 0";
		if (state == 1) {
			str2 = (char *)"STATE 1 : Insert Filter Holder";
			str3 = (char *)"Waitng...";
			if (finish[0]) str3 = (char *)"Complete!";
		}
		else if (state == 2) {
			str2 = (char *)"STATE 2 : Insert Filter";
			str3 = (char *)"Waitng...";
			if (finish[1]) str3 = (char *)"Complete!";
		}
		else if (state == 3) str2 = (char *)"STATE 3 : Close Body";
		else if (state == 4) {
			str2 = (char *)"STATE 4 : Insert Teapot";
			str3 = (char *)"Waitng...";
			if (finish[2]) str3 = (char *)"Complete!";
		}

		else if (state == 5) str2 = (char *)"STATE 5 : Coffee Extraction Start";
		else if (state == 6) {
			str2 = (char *)"STATE 6 : Coffee Extraction Completed.";
			str3 = (char *)"Waitng...";
			if (finish[3])str3 = (char *)"Press 0 to restart.";
		}
		break;
	case 2:
		str = (char *)"MODE : COMPONENT DETAIL";
		break;
	default:
		break;
	}

	draw_string(GLUT_BITMAP_9_BY_15, str, -9.5, 8.7, 0.2, 0.4, 0.2);
	draw_string(GLUT_BITMAP_9_BY_15, str2, -9.5, 7.7, 0.2, 0.4, 0.2);
	if (finish[0] && state == 1) draw_string(GLUT_BITMAP_9_BY_15, str3, -9.5, 6.7, 1, 0.2, 0.2);
	else if (finish[1] && state == 2) draw_string(GLUT_BITMAP_9_BY_15, str3, -9.5, 6.7, 1, 0.2, 0.2);
	else if (finish[2] && state == 4) draw_string(GLUT_BITMAP_9_BY_15, str3, -9.5, 6.7, 1, 0.2, 0.2);
	else if (finish[3] && state == 6) draw_string(GLUT_BITMAP_9_BY_15, str3, -9.5, 6.7, 1, 0.2, 0.2);
	else draw_string(GLUT_BITMAP_9_BY_15, str3, -9.5, 6.7, 0, 0.4, 1);

	glFlush();
}

void draw_helpText() {
	char *str = (char *)"* HELP (Press F1 to show or turn it off)";
	char *str2 = (char *)"m : Change mode";
	char *str3 = (char *)"Mouse wheel : zoom IN/OUT";
	char *str4 = (char *)"Right click : Init, Show individual components,";
	char *str5 = (char *)"              Change body texture, Change background";
	char *str6 = (char *)"Direction key : Change viewpoint";
	char *str7 = (char *)"[Simulation Mode]";
	char *str8 = (char *)"1 : Move STATE 1";
	char *str9 = (char *)"2 : Move STATE 2";
	char *str10 = (char *)"3 : Move STATE 3";
	char *str11 = (char *)"4 : Move STATE 4";
	char *str12 = (char *)"5 : Move STATE 5";
	char *str13 = (char *)"6 : Move STATE 6";
	char *str14 = (char *)"0 : Move STATE 0";

	draw_string(GLUT_BITMAP_8_BY_13, str, -9.5, 2.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str2, -9.5, 1.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str3, -9.5, 0.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str4, -9.5, -0.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str5, -9.5, -1.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str7, -9.5, -2.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str8, -9.5, -3.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str9, -9.5, -4.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str10, -9.5, -5.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str11, -9.5, -6.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str12, -9.5, -7.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str13, -9.5, -8.5, 1, 0.8, 0);
	draw_string(GLUT_BITMAP_8_BY_13, str14, -9.5, -9.5, 1, 0.8, 0);

	glFlush();
}

void draw_detail(void) {
	glColor3f(1, 1, 1);
	//filter holder 그리기
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0);
	glRotatef(10, 0, 0, 1);
	draw_obj_with_texture(filter_holder, texture_filter_holder);
	glPopMatrix();

	//filter 그리기
	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 0);
	draw_obj_with_texture(filter, texture_filter);
	glPopMatrix();

	//teapot 그리기
	glLoadName(3);
	glPushMatrix();
	glScalef(1.4, 1.4, 1.4);
	glTranslatef(g_pPosition[04], g_pPosition[5], 0);
	draw_obj_with_texture(teapot_open, texture_teapot);
	glPopMatrix();

	//body 그리기
	glLoadName(4);
	glPushMatrix();
	glTranslatef(g_pPosition[6], g_pPosition[7], 0);
	draw_body(0);
	glPopMatrix();
}

void picking(int x, int y) {
	GLuint selectBuf[100];
	glSelectBuffer(100, selectBuf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(-1);
	glPushMatrix();
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	draw_detail();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);

	if (hits <= 0)return;

	int stack = selectBuf[0];
	unsigned int zMin = selectBuf[1];
	unsigned int zMax = selectBuf[2];
	g_nSelect = selectBuf[3];

	int index = 3 + stack;
	int i;
	if (depthM) {//zMax가 작은 것이 pick
		for (i = 1; i < hits; i++) {
			stack = selectBuf[index];
			if (zMax > selectBuf[index + 2]) {
				zMax = selectBuf[index + 2];
				g_nSelect = selectBuf[index + 3];
			}
			index += 3 + index;
		}
	}
	else {//zMin이 작은 것이 pick
		for (i = 1; i < hits; i++) {
			stack = selectBuf[index];
			if (zMin < selectBuf[index + 1]) {
				zMin = selectBuf[index + 1];
				g_nSelect = selectBuf[index + 3];
			}
			index += 3 + index;
		}
	}

	printf("select name : %d\n", selectBuf[3]);
	for (int j = 0; j < 8; j++) {
		printf("%d ", selectBuf[j]);
	}
	printf("\n\n");

}

//Display call back 함수
void draw(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//물체의 위치를 지정
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	//시점 변경 가능 - 카메라 위치 좌표, upvector 
	cam[0] = radius * sin(theta *(PI / 180)) *cos(phi*(PI / 180));
	cam[1] = radius * sin(phi*(PI / 180));
	cam[2] = radius * cos(theta*(PI / 180))*cos(phi*(PI / 180));
	upVector = cos(phi * PI / 180);

	draw_text();
	if (helpText) draw_helpText();

	gluLookAt(cam[0], cam[1], cam[2], 0, 0, 0, 0, upVector, 0);

	if (background_textrue < 5) drawBackground();

	if (mode == 0) {
		glColor3f(1.f, 1.f, 1.f);
		//table 그리기
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glScalef(2, 2, 2);
		glTranslatef(0, -1.29, 0);
		draw_obj_with_texture(table, texture_table);
		glPopMatrix();

		draw_detail();
	}

	else if (mode == 1) {
		glColor3f(1.f, 1.f, 1.f);
		//table 그리기
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glScalef(2, 2, 2);
		glTranslatef(0, -1.29, 0);
		draw_obj_with_texture(table, texture_table);
		glPopMatrix();

		//body 그리기
		if (state == 0 || state == 1 || state == 2) {
			glPushMatrix();
			draw_body(0);
			glPopMatrix();
		}

		if (state > 0) {
			//filter holder 그리기
			glPushMatrix();
			glTranslatef(0.05, filterHolder_z, 2.4);
			draw_obj_with_texture(filter_holder, texture_filter_holder);
			glPopMatrix();
			if (filterHolder_z < 2.1) finish[0] = true;
			if (state > 1) {
				//filter 그리기
				glPushMatrix();
				glScalef(0.8, 0.8, 0.8);
				glTranslatef(0, filter_z, 3);
				draw_obj_with_texture(filter_close, texture_filter);
				glPopMatrix();
				if (filter_z < 2.8) finish[1] = true;
				if (state > 2 && state < 5) {
					glPushMatrix();
					draw_body(1);
					glPopMatrix();
				}
				if (state > 3 && state < 6) {
					//teapot 그리기
					glPushMatrix();
					glScalef(1.4, 1.4, 1.4);
					glTranslatef(teapot_x, teapot_z, teapot_y);
					draw_obj_with_texture(teapot_close, texture_teapot);
					glPopMatrix();
					if (teapot_y < 1.7)finish[2] = true;
					if (state == 5) {
						glPushMatrix();
						draw_body(2);
						glPopMatrix();
					}
				}

				if (state == 6) {
					glPushMatrix();
					draw_body(0);
					glPopMatrix();
					if (!finish[3]) {
						//teapot 그리기
						glPushMatrix();
						glTranslatef(teapot_x, teapot_z, teapot_y);
						glRotatef(teapot_angle, 0, 0, 1);
						glScalef(1.4, 1.4, 1.4);
						draw_obj_with_texture(teapot_close, texture_teapot);
						glPopMatrix();
						if (teapot_z < -0.55) finish[3] = true;
					}
					else {
						glPushMatrix();
						glTranslatef(teapot_x, teapot_z, teapot_y);
						glScalef(1.4, 1.4, 1.4);
						draw_obj_with_texture(teapot_open, texture_teapot);
						glPopMatrix();

						glPushMatrix();
						glTranslatef(teapot_x, teapot_z, teapot_y);
						draw_coffee();
						glPopMatrix();
					}

				}
			}
		}
	}

	else if (mode == 2) {
		glColor3f(1.f, 1.f, 1.f);
		glPushMatrix();
		if (shape_select == 1) draw_obj_with_texture(filter_holder, texture_filter_holder);
		else if (shape_select == 2) 	draw_obj_with_texture(filter, texture_filter);
		else if (shape_select == 3) draw_obj_with_texture(teapot_open, texture_teapot);
		else if (shape_select == 4) draw_body(0);
		glPopMatrix();

	}

	glFlush();
	glutSwapBuffers(); //실제로 출력하는 함수
}

void draw_coffee(void) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glPushMatrix();
	glColor3f(0.2, 0, 0);
	glRotatef(-90, 1, 0, 0); //세우기
	gluCylinder(qobj, 1.0f, 1.0f, 0.5f, 30, 4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2, 0, 0);
	glTranslatef(0, 0.5, 0);
	glRotatef(-90, 1, 0, 0); //세우기
	gluDisk(qobj, 0.0f, 1.0f, 30, 4);
	glPopMatrix();
}

//배경 그려주는 함수
void drawBackground(void) {
	glColor3f(1.0, 1.0, 1.0); //white로 컬러를 설정해주어야 texture색상이 제대로 적용된다.
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //polygon의 원래 색상은 무시하고 texture로 덮는다.
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//polygon의 원래 색상과 texture 색상을 곱하여 덮음, texture가 입혀진 표면에 광원효과 설정 가능
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //texture의 색상으로 덮어 씌운다.
	float g_nSize = 50.0f;

	if (background_textrue == 1) glBindTexture(GL_TEXTURE_2D, texName_background1[0]);
	else if (background_textrue == 2) glBindTexture(GL_TEXTURE_2D, texName_background2[0]);
	else if (background_textrue == 3) glBindTexture(GL_TEXTURE_2D, texName_background3[0]);
	else if (background_textrue == 4) glBindTexture(GL_TEXTURE_2D, texName_background4[0]);

	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0, 0); // -x 축
	glTexCoord2f(1, 1); glVertex3f(-g_nSize, g_nSize, -g_nSize);
	glTexCoord2f(1, 0); glVertex3f(-g_nSize, -g_nSize, -g_nSize);
	glTexCoord2f(0, 0); glVertex3f(-g_nSize, -g_nSize, g_nSize);
	glTexCoord2f(0, 1); glVertex3f(-g_nSize, g_nSize, g_nSize);
	glEnd();

	if (background_textrue == 1) glBindTexture(GL_TEXTURE_2D, texName_background1[1]);
	else if (background_textrue == 2) glBindTexture(GL_TEXTURE_2D, texName_background2[1]);
	else if (background_textrue == 3) glBindTexture(GL_TEXTURE_2D, texName_background3[1]);
	else if (background_textrue == 4) glBindTexture(GL_TEXTURE_2D, texName_background4[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0); // x 축
	glTexCoord2f(1, 1);  glVertex3f(g_nSize, g_nSize, g_nSize);
	glTexCoord2f(1, 0);  glVertex3f(g_nSize, -g_nSize, g_nSize);
	glTexCoord2f(0, 0); glVertex3f(g_nSize, -g_nSize, -g_nSize);
	glTexCoord2f(0, 1); glVertex3f(g_nSize, g_nSize, -g_nSize);
	glEnd();

	if (background_textrue == 1) glBindTexture(GL_TEXTURE_2D, texName_background1[2]);
	else if (background_textrue == 2) glBindTexture(GL_TEXTURE_2D, texName_background2[2]);
	else if (background_textrue == 3) glBindTexture(GL_TEXTURE_2D, texName_background3[2]);
	else if (background_textrue == 4) glBindTexture(GL_TEXTURE_2D, texName_background4[2]);

	glBegin(GL_QUADS);
	glNormal3f(0, -1.0, 0); // -y 축
	glTexCoord2f(0, 1); glVertex3f(-g_nSize, -g_nSize, -g_nSize);
	glTexCoord2f(1, 1); glVertex3f(g_nSize, -g_nSize, -g_nSize);
	glTexCoord2f(1, 0); glVertex3f(g_nSize, -g_nSize, g_nSize);
	glTexCoord2f(0, 0); glVertex3f(-g_nSize, -g_nSize, g_nSize);
	glEnd();

	if (background_textrue == 1) glBindTexture(GL_TEXTURE_2D, texName_background1[3]);
	else if (background_textrue == 2) glBindTexture(GL_TEXTURE_2D, texName_background2[3]);
	else if (background_textrue == 3) glBindTexture(GL_TEXTURE_2D, texName_background3[3]);
	else if (background_textrue == 4) glBindTexture(GL_TEXTURE_2D, texName_background4[3]);

	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0); // y 축
	glTexCoord2f(0, 1); glVertex3f(-g_nSize, g_nSize, g_nSize);
	glTexCoord2f(1, 1); glVertex3f(g_nSize, g_nSize, g_nSize);
	glTexCoord2f(1, 0); glVertex3f(g_nSize, g_nSize, -g_nSize);
	glTexCoord2f(0, 0); glVertex3f(-g_nSize, g_nSize, -g_nSize);
	glEnd();

	if (background_textrue == 1) glBindTexture(GL_TEXTURE_2D, texName_background1[4]);
	else if (background_textrue == 2) glBindTexture(GL_TEXTURE_2D, texName_background2[4]);
	else if (background_textrue == 3) glBindTexture(GL_TEXTURE_2D, texName_background3[4]);
	else if (background_textrue == 4) glBindTexture(GL_TEXTURE_2D, texName_background4[4]);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0); // -z 축
	glTexCoord2f(0, 1);  glVertex3f(g_nSize, g_nSize, g_nSize);
	glTexCoord2f(1, 1);  glVertex3f(-g_nSize, g_nSize, g_nSize);
	glTexCoord2f(1, 0); glVertex3f(-g_nSize, -g_nSize, g_nSize);
	glTexCoord2f(0, 0); glVertex3f(g_nSize, -g_nSize, g_nSize);
	glEnd();

	if (background_textrue == 1) glBindTexture(GL_TEXTURE_2D, texName_background1[5]);
	else if (background_textrue == 2) glBindTexture(GL_TEXTURE_2D, texName_background2[5]);
	else if (background_textrue == 3) glBindTexture(GL_TEXTURE_2D, texName_background3[5]);
	else if (background_textrue == 4) glBindTexture(GL_TEXTURE_2D, texName_background4[5]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0); // z 축
	glTexCoord2f(1, 1);  glVertex3f(g_nSize, g_nSize, -g_nSize);
	glTexCoord2f(0, 1); glVertex3f(-g_nSize, g_nSize, -g_nSize);
	glTexCoord2f(0, 0);  glVertex3f(-g_nSize, -g_nSize, -g_nSize);
	glTexCoord2f(1, 0); glVertex3f(g_nSize, -g_nSize, -g_nSize);
	glEnd();
}

void background1MapSetting(void) {
	glGenTextures(6, texName_background1);
	int width, height, channels, tmp;

	tmp = 1;

	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName_background1[i]);
		char buf[100];
		if (i == 0)sprintf(buf, "background/%d/negx.bmp", tmp);
		else if (i == 1)	sprintf(buf, "background/%d/posx.bmp", tmp);
		else if (i == 2)	sprintf(buf, "background/%d/negy.bmp", tmp);
		else if (i == 3)	sprintf(buf, "background/%d/posy.bmp", tmp);
		else if (i == 4)	sprintf(buf, "background/%d/negz.bmp", tmp);
		else if (i == 5)	sprintf(buf, "background/%d/posz.bmp", tmp);

		buf[strlen(buf)] = 0;
		uchar *img = readImageData(buf, &width, &height, &channels);

		glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL */ 3, width, height, 0, /* TEXEL CHANNEL */GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void background2MapSetting(void) {
	glGenTextures(6, texName_background2);
	int width, height, channels, tmp;

	tmp = 2;
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName_background2[i]);
		char buf[100];
		if (i == 0)sprintf(buf, "background/%d/negx.bmp", tmp);
		else if (i == 1)	sprintf(buf, "background/%d/posx.bmp", tmp);
		else if (i == 2)	sprintf(buf, "background/%d/negy.bmp", tmp);
		else if (i == 3)	sprintf(buf, "background/%d/posy.bmp", tmp);
		else if (i == 4)	sprintf(buf, "background/%d/negz.bmp", tmp);
		else if (i == 5)	sprintf(buf, "background/%d/posz.bmp", tmp);

		buf[strlen(buf)] = 0;
		uchar *img = readImageData(buf, &width, &height, &channels);

		glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL */ 3, width, height, 0, /* TEXEL CHANNEL */GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void background3MapSetting(void) {
	glGenTextures(6, texName_background3);
	int width, height, channels, tmp;

	tmp = 3;

	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName_background3[i]);
		char buf[100];
		if (i == 0)sprintf(buf, "background/%d/negx.bmp", tmp);
		else if (i == 1)	sprintf(buf, "background/%d/posx.bmp", tmp);
		else if (i == 2)	sprintf(buf, "background/%d/negy.bmp", tmp);
		else if (i == 3)	sprintf(buf, "background/%d/posy.bmp", tmp);
		else if (i == 4)	sprintf(buf, "background/%d/negz.bmp", tmp);
		else if (i == 5)	sprintf(buf, "background/%d/posz.bmp", tmp);

		buf[strlen(buf)] = 0;
		uchar *img = readImageData(buf, &width, &height, &channels);

		glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL */ 3, width, height, 0, /* TEXEL CHANNEL */GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void background4MapSetting(void) {
	glGenTextures(6, texName_background4);
	int width, height, channels, tmp;

	tmp = 4;

	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName_background4[i]);
		char buf[100];
		if (i == 0)sprintf(buf, "background/%d/negx.bmp", tmp);
		else if (i == 1)	sprintf(buf, "background/%d/posx.bmp", tmp);
		else if (i == 2)	sprintf(buf, "background/%d/negy.bmp", tmp);
		else if (i == 3)	sprintf(buf, "background/%d/posy.bmp", tmp);
		else if (i == 4)	sprintf(buf, "background/%d/negz.bmp", tmp);
		else if (i == 5)	sprintf(buf, "background/%d/posz.bmp", tmp);

		buf[strlen(buf)] = 0;
		uchar *img = readImageData(buf, &width, &height, &channels);

		glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL */ 3, width, height, 0, /* TEXEL CHANNEL */GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void draw_obj(ObjParser *objParser)
{
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

void filterHolderTextureMapping() {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("obj_image/black.bmp", &imgWidth, &imgHeight, &channels);;

	int texNum = 1;
	glGenTextures(texNum, &texture_filter_holder);
	glBindTexture(GL_TEXTURE_2D, texture_filter_holder);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void filterTextureMapping() {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("obj_image/filter_texture_2.bmp", &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &texture_filter);
	glBindTexture(GL_TEXTURE_2D, texture_filter);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void teapotTextureMapping() {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("obj_image/teapot_texture.bmp", &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &texture_teapot);
	glBindTexture(GL_TEXTURE_2D, texture_teapot);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void bodyTextureMapping() {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("obj_image/main_body_texture.bmp", &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &texture_body);
	glBindTexture(GL_TEXTURE_2D, texture_body);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void tableTextureMapping(void) {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("obj_image/table_texture.bmp", &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &texture_table);
	glBindTexture(GL_TEXTURE_2D, texture_table);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void glassTextureMapping(void) {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData("obj_image/white.bmp", &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &texture_glass);
	glBindTexture(GL_TEXTURE_2D, texture_glass);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//GL_REPEAT 둘중 하나 선택
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
}

void draw_obj_with_texture(ObjParser *objParser, GLuint tmp)
{
	glDisable(GL_BLEND);
	//glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable
	glBindTexture(GL_TEXTURE_2D, tmp);

	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
	glEnable(GL_BLEND);
}

//선택된 body texture에 따라 본체를 그려주는 함수
void draw_body(int n) {
	if (n == 0) {
		if (body_textrue == 1)draw_obj_with_texture(body_open_metal, texture_body);
		else if (body_textrue == 2) draw_obj_with_texture(body_open_green, texture_body);
		else if (body_textrue == 3) draw_obj_with_texture(body_open_pink, texture_body);
		else if (body_textrue == 4) draw_obj_with_texture(body_open_yellow, texture_body);
		else if (body_textrue == 5) draw_obj_with_texture(body_open_green_yellow, texture_body);
	}
	else if (n == 1) {
		if (body_textrue == 1)draw_obj_with_texture(body_close_off_metal, texture_body);
		else if (body_textrue == 2) draw_obj_with_texture(body_close_off_green, texture_body);
		else if (body_textrue == 3) draw_obj_with_texture(body_close_off_pink, texture_body);
		else if (body_textrue == 4) draw_obj_with_texture(body_close_off_yellow, texture_body);
		else if (body_textrue == 5) draw_obj_with_texture(body_close_off_green_yellow, texture_body);
	}
	else if (n == 2) {
		if (body_textrue == 1)draw_obj_with_texture(body_close_on_metal, texture_body);
		else if (body_textrue == 2) draw_obj_with_texture(body_close_on_green, texture_body);
		else if (body_textrue == 3) draw_obj_with_texture(body_close_on_pink, texture_body);
		else if (body_textrue == 4) draw_obj_with_texture(body_close_on_yellow, texture_body);
		else if (body_textrue == 5) draw_obj_with_texture(body_close_on_green_yellow, texture_body);
	}
}

void mouse(int button, int state, int x, int y) {
	//인자들을 해석해서 원하는 기능을 구현

	if (mode == 0) {
		printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, state, x, y);
		y = g_nGLHeight - y;
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			picking(x, y);
			g_nX = x;
			g_nY = y;
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			g_nSelect = 0;
		}
	}
}

void mouseWheel(int button, int dir, int x, int y) {
	//인자들을 해석해서 원하는 기능을 구현
	//printf("Mouse button is clicked! (%d, %d, %d, %d)\n", button, dir, x, y);

	if (dir > 0) {//휠 위로
		if (radius > 1)radius -= 1;
	}
	else {//휠 아래로
		if (radius < 50)radius += 1;
	}
}

void motion(int x, int y) {
	//인자들을 해석해서 원하는 기능을 구현
	if (mode == 0) {
		printf("Mouse is moving! (%d, %d)\n", x, y);
		y = g_nGLHeight - y;
		if (g_nSelect > 0) {
			g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
			g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
			g_nX = x;
			g_nY = y;
			glutPostRedisplay();
		}
	}
}

void keyboard(unsigned char key, int x, int y) {
	//인자들을 해석해서 원하는 기능을 구현
	printf("You pressed %c\n", key);

	if (key == 'm') {
		if (mode == 0) {
			printf("MODE : 시뮬레이션\n");
			state = 0;
			filterHolder_z = 6;
			filter_z = 6;
			teapot_x = -3;
			teapot_y = 2;
			teapot_z = -0.55;
			teapot_angle = 0;
			mode = 1;

		}
		else {
			printf("MODE : 제품 상세\n");
			mode = 0;
		}

		glFlush();
		glutPostRedisplay();

	}
	if (mode == 1) {
		if (key == '1' && state == 0)state = 1;
		if (key == '2' && state == 1 && finish[0] == true) state = 2;
		if (key == '3' && state == 2 && finish[1] == true)state = 3;
		if (key == '4' && state == 3)state = 4;
		if (key == '5' && state == 4 && finish[2] == true)state = 5;
		if (key == '6' && state == 5)state = 6;
		if (key == '0' && state == 6) {
			state = 0;
			filterHolder_z = 6;
			filter_z = 6;
			teapot_x = -3;
			teapot_y = 2;
			finish[0] = false;
			finish[1] = false;
			finish[2] = false;
		}

		glFlush();
		glutPostRedisplay();
	}
}

//상하좌우 방향키 사용
void special_keyboard(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		phi -= 5;
		if (phi < 0) phi = 355;
	}
	else if (key == GLUT_KEY_DOWN) {
		phi += 5;
		if (phi > 360) phi = 5;
	}
	else if (key == GLUT_KEY_LEFT) {
		theta += 5;
		if (theta > 360) theta = 5;
	}
	else if (key == GLUT_KEY_RIGHT) {
		theta -= 5;
		if (theta < 0) theta = 355;
	}
	else if (key == GLUT_KEY_F1) {
		if (helpText) helpText = false;
		else helpText = true;
	}
}

void sub_menu_function_shape(int option) {
	printf("MODE : 개별 구성품 상세 (%d)\n", option);
	if (option == 1) {//filter holder
		mode = 2;
		shape_select = 1;
		glFlush();
		glutPostRedisplay();
	}
	if (option == 2) {//filter
		mode = 2;
		shape_select = 2;
		glFlush();
		glutPostRedisplay();
	}
	if (option == 3) {//teapot
		mode = 2;
		shape_select = 3;
		glFlush();
		glutPostRedisplay();
	}
	if (option == 4) {//body
		mode = 2;
		shape_select = 4;
		glFlush();
		glutPostRedisplay();
	}
}

void sub_menu_function_bodytexture(int option) {
	if (option == 1) {//metal
		body_textrue = 1;
		printf("본체 texture : metal");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 2) {//green
		body_textrue = 2;
		printf("본체 texture : green");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 3) {//pink
		body_textrue = 3;
		printf("본체 texture : pink");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 4) {//yellow
		body_textrue = 4;
		printf("본체 texture : yellow");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 5) {//green&yellow
		body_textrue = 5;
		printf("본체 texture : green&yellow");
		glFlush();
		glutPostRedisplay();
	}
}

void sub_menu_function_background(int option) {
	if (option == 1) {//
		background_textrue = 1;
		printf("배경 : Yokohama\n");
		glFlush();
		glutPostRedisplay();

	}
	if (option == 2) {//sky
		background_textrue = 2;
		printf("배경 : Sky\n");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 3) {//sky night
		background_textrue = 3;
		printf("배경 : Night Sky\n");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 4) {//
		background_textrue = 4;
		printf("배경 : Lycksele\n");
		glFlush();
		glutPostRedisplay();
	}
	if (option == 5) {//no background
		background_textrue = 5;
		printf("배경 : No Background\n");
		glFlush();
		glutPostRedisplay();
	}
}

void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	if (option == 999) {
		exit(0);
	}
	else if (option == 1) { //init
		radius = 25;
		theta = 0, phi = 0;
		shape_select = 0;
		mode = 0;
		body_textrue = 1;
		background_textrue = 1;
		state = 0;

		g_pPosition[0] = -8;
		g_pPosition[1] = -1.5;
		g_pPosition[2] = -4;
		g_pPosition[3] = -1.53;
		g_pPosition[4] = 1;
		g_pPosition[5] = -0.55;
		g_pPosition[6] = 6;
		g_pPosition[7] = 0;

		g_nSelect = 0;

		g_nGLWidth = 500;
		g_nGLHeight = 500;

		depthM = true;

		finish[0] = false; //시뮬레이션 단계의 마침 여부 표시
		finish[1] = false;
		finish[2] = false;
		finish[3] = false;
		helpText = true; //도움말 표시 여부

		filterHolder_z = 6;
		filter_z = 6;
		teapot_x = -3;
		teapot_y = 2;
		teapot_z = -0.55;
		teapot_angle = 0;

		glFlush();
		glutPostRedisplay();
	}
}