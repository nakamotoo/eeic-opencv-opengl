#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "test4"

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);

void draw_pyramid();
void draw_plane();

// 繧ｰ繝ｭ繝ｼ繝舌Ν螟画焚
double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_angle3 = 0.0;
double g_angle4 = 3.141592 / 4;
double g_distance = 20.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;

int main(int argc, char *argv[]) {
  /* OpenGL縺ｮ蛻晄悄蛹� */
  init_GL(argc, argv);

  /* 縺薙�繝励Ο繧ｰ繝ｩ繝�迚ｹ譛峨�蛻晄悄蛹� */
  init();

  /* 繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ縺ｮ逋ｻ骭ｲ */
  set_callback_functions();

  /* 繝｡繧､繝ｳ繝ｫ繝ｼ繝� */
  glutMainLoop();

  return 0;
}

void init_GL(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_X, WINDOW_Y);
  glutCreateWindow(WINDOW_NAME);
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0); // 閭梧勹縺ｮ蝪励ｊ縺､縺ｶ縺苓牡繧呈欠螳�
}

void set_callback_functions() {
  glutDisplayFunc(glut_display);
  glutKeyboardFunc(glut_keyboard);
  glutMouseFunc(glut_mouse);
  glutMotionFunc(glut_motion);
  glutPassiveMotionFunc(glut_motion);
}

void glut_keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    exit(0);
  }
  glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_UP) {
      g_isLeftButtonOn = false;
    } else if (state == GLUT_DOWN) {
      g_isLeftButtonOn = true;
    }
  } else if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_UP) {
      g_isRightButtonOn = false;
    } else if (state == GLUT_DOWN) {
      g_isRightButtonOn = true;
    }
  }
}

void glut_motion(int x, int y) {
  static int px = -1, py = -1;

  if (g_isLeftButtonOn == true) {
    if (px >= 0 && py >= 0) {
      g_angle1 += (double)-(x - px) / 20;
      g_angle2 += (double)(y - py) / 20;
    }
    px = x;
    py = y;
  } else if (g_isRightButtonOn == true) {
    if (px >= 0 && py >= 0) {
      g_angle3 += (double)(x - px) / 20;
    }
    px = x;
    py = y;
  } else {
    px = -1;
    py = -1;
  }
  glutPostRedisplay();
}

void glut_display() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, 1.0, 0.1, 100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (cos(g_angle2) > 0) {
    gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),
              g_distance * sin(g_angle2),
              g_distance * cos(g_angle2) * cos(g_angle1), 0.0, 0.0, 0.0, 0.0,
              1.0, 0.0);
  } else {
    gluLookAt(g_distance * cos(g_angle2) * sin(g_angle1),
              g_distance * sin(g_angle2),
              g_distance * cos(g_angle2) * cos(g_angle1), 0.0, 0.0, 0.0, 0.0,
              -1.0, 0.0);
  }

  /* hatena */
  GLfloat lightpos[] = {5 * cosf((float)g_angle4) * sinf((float)g_angle3),
                        5 * sinf((float)g_angle4),
                        5 * cosf((float)g_angle4) * cosf((float)g_angle3),
                        1.0f};
  GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};

  GLfloat lightpos1[] = {5 * cosf((float)g_angle4) * sinf((float)g_angle3),
                         5 * sinf((float)g_angle4),
                         -5 * cosf((float)g_angle4) * cosf((float)g_angle3),
                         1.0f};
  GLfloat diffuse1[] = {.0, .0, 1.0, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  /* hatena */
  glPushMatrix();
  /* hatena */
  glTranslatef(5 * cosf((float)g_angle4) * sinf((float)g_angle3),
               5 * sinf((float)g_angle4),
               5 * cosf((float)g_angle4) * cosf((float)g_angle3));
  /* hatena */
  glutSolidSphere(0.2, 50, 50);
  /* hatena */
  glPopMatrix();

  glPushMatrix();
  /* hatena */
  glTranslatef(5 * cosf((float)g_angle4) * sinf((float)g_angle3),
               5 * sinf((float)g_angle4),
               -5 * cosf((float)g_angle4) * cosf((float)g_angle3));
  /* hatena */
  glutSolidSphere(0.2, 50, 50);
  /* hatena */
  glPopMatrix();

  /* hatena */
  glEnable(GL_LIGHTING);
  /* hatena */
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  /* hatena */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  /* hatena */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

  /* hatena */
  glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
  /* hatena */
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);

  glPushMatrix();
  glTranslatef(0.0, -2.0, 0.0);
  draw_plane();
  glPopMatrix();

  glPushMatrix();
  glScalef(1.0, 2.0, 1.0);
  draw_pyramid();
  glPopMatrix();

  glFlush();

  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glutSwapBuffers();
}

void draw_pyramid() {
  GLdouble pointO[] = {0.0, 1.0, 0.0};
  GLdouble pointA[] = {1.5, -1.0, 1.5};
  GLdouble pointB[] = {-1.5, -1.0, 1.5};
  GLdouble pointC[] = {-1.5, -1.0, -1.5};
  GLdouble pointD[] = {1.5, -1.0, -1.5};

  GLfloat facecolor1[] = {1.0f, 0.0f, 0.0f, 0.8f};
  GLfloat facecolor2[] = {1.0f, 1.0f, 0.0f, 0.8f};
  GLfloat facecolor3[] = {0.0f, 1.0f, 1.0f, 0.8f};
  GLfloat facecolor4[] = {1.0f, 0.0f, 1.0f, 0.8f};
  GLfloat facecolor5[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

  /* hatena */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor1);
  glNormal3d(0.0, 0.6, 0.8);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointA);
  glVertex3dv(pointB);
  glEnd();

  /* hatena */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor2);
  glNormal3d(-0.8, 0.6, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointB);
  glVertex3dv(pointC);
  glEnd();

  /* hatena */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor3);
  glNormal3d(0.0, 0.6, -0.8);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointC);
  glVertex3dv(pointD);
  glEnd();

  /* hatena */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor4);
  glNormal3d(0.8, 0.6, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointD);
  glVertex3dv(pointA);
  glEnd();

  /* hatena */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor5);
  glNormal3d(0.0, -1.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex3dv(pointA);
  glVertex3dv(pointB);
  glVertex3dv(pointC);
  glVertex3dv(pointD);
  glEnd();
}

void draw_plane() {
  GLfloat facecolor[] = {0.9f, 0.9f, 0.9f, 1.0f};
  /* hatena */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, facecolor);
  glNormal3d(0.0, 1.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3d(4.0, 0.0, 4.0);
  glVertex3d(4.0, 0.0, -4.0);
  glVertex3d(-4.0, 0.0, -4.0);
  glVertex3d(-4.0, 0.0, 4.0);
  glEnd();
}