#include <GL/glut.h>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "test5"
#define TEXTURE_HEIGHT (512)
#define TEXTURE_WIDTH (512)

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void draw_pyramid();
void set_texture();

// 繧ｰ繝ｭ繝ｼ繝舌Ν螟画焚
double g_angle1 = 0.0;
double g_angle2 = -3.141592 / 6;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
GLuint g_TextureHandles[3] = {0, 0, 0};

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
  /* hatena */
  glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
  /* hatena */
  glGenTextures(3, g_TextureHandles);

  for (int i = 0; i < 3; i++) {
    /* hatena */
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
    /* hatena */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    /* hatena */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);
    /* hatena */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /* hatena */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }

  /* hatena */
  set_texture();
}

void set_callback_functions() {
  glutDisplayFunc(glut_display);
  glutKeyboardFunc(glut_keyboard);
  glutMouseFunc(glut_mouse);
  glutMotionFunc(glut_motion);
  glutPassiveMotionFunc(glut_motion);
  glutIdleFunc(glut_idle);
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
  }

  if (button == GLUT_RIGHT_BUTTON) {
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
      g_distance += (double)(y - py) / 20;
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

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  draw_pyramid();
  glFlush();
  glDisable(GL_DEPTH_TEST);

  glutSwapBuffers();
}

void glut_idle() {
  static int counter = 0;

  if (counter == 0) {
    /* hatena */
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[0]);
  } else if (counter == 100) {
    /* hatena */
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[1]);
  } else if (counter == 200) {
    /* hatena */
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[2]);
  }

  counter++;
  if (counter > 300)
    counter = 0;

  glutPostRedisplay();
}

void draw_pyramid() {
  GLdouble pointO[] = {0.0, 1.0, 0.0};
  GLdouble pointA[] = {1.5, -1.0, 1.5};
  GLdouble pointB[] = {-1.5, -1.0, 1.5};
  GLdouble pointC[] = {-1.5, -1.0, -1.5};
  GLdouble pointD[] = {1.5, -1.0, -1.5};

  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointA);
  glVertex3dv(pointB);
  glEnd();

  glColor3d(1.0, 1.0, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointB);
  glVertex3dv(pointC);
  glEnd();

  glColor3d(0.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointC);
  glVertex3dv(pointD);
  glEnd();

  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointD);
  glVertex3dv(pointA);
  glEnd();

  glColor3d(1.0, 1.0, 1.0);

  /* hatena */
  glEnable(GL_TEXTURE_2D);

  glBegin(GL_POLYGON);
  /* hatena */
  glTexCoord2d(1.0, 0.0);
  glVertex3dv(pointA);
  /* hatena */
  glTexCoord2d(0.0, 0.0);
  glVertex3dv(pointB);
  /* hatena */
  glTexCoord2d(0.0, 1.0);
  glVertex3dv(pointC);
  /* hatena */
  glTexCoord2d(1.0, 1.0);
  glVertex3dv(pointD);
  glEnd();

  /* hatena */
  glDisable(GL_TEXTURE_2D);
}

void set_texture() {
  const char *inputFileNames[3] = {"apple.jpg", "fruits.jpg", "chicky_512.png"};
  for (int i = 0; i < 3; i++) {
    /* hatena */
    cv::Mat input = cv::imread(inputFileNames[i], 1);
    // BGR -> RGB縺ｮ螟画鋤
    /* hatena */
    cv::cvtColor(input, input, CV_BGR2RGB);
    glBindTexture(GL_TEXTURE_2D, g_TextureHandles[i]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, (TEXTURE_WIDTH - input.cols) / 2,
                    (TEXTURE_HEIGHT - input.rows) / 2, input.cols, input.rows,
                    GL_RGB, GL_UNSIGNED_BYTE, input.data);
  }
}