#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_X (500)
#define WINDOW_Y (500)
#define WINDOW_NAME "assignment"
#define DEG2RAD(deg) deg * 2.0 * M_PI / 360.0
#define RED 1.0, 0.0, 0.0, 0.0

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);

void draw_square1();
void draw_square2();
void draw_square3();
void draw_polygon(int n);

// グローバル変数
int g_display_mode = 1;

int main(int argc, char *argv[]) {
  /* OpenGLの初期化 */
  init_GL(argc, argv);

  /* このプログラム特有の初期化 */
  init();

  /* コールバック関数の登録 */
  set_callback_functions();

  /* メインループ */
  glutMainLoop(); // 無限ループ。コールバック関数が呼ばれるまでずっと実行される。

  return 0;
}

void init_GL(int argc, char *argv[]) {
  glutInit(&argc, argv);          // OpenGLの初期化
  glutInitDisplayMode(GLUT_RGBA); // ディスプレイモードをRGBAモードに設定
  glutInitWindowSize(WINDOW_X, WINDOW_Y); // ウィンドウサイズを指定
  glutCreateWindow(
      WINDOW_NAME); // ウィンドウを「生成」。まだ「表示」はされない。
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0); // 背景の塗りつぶし色を指定
}

void set_callback_functions() {
  glutDisplayFunc(
      glut_display); // ディスプレイに変化があった時に呼ばれるコールバック関数を登録
  glutKeyboardFunc(
      glut_keyboard); // キーボードに変化があった時に呼び出されるコールバック関数を登録
}

// キーボードに変化があった時に呼び出されるコールバック関数。
void glut_keyboard(unsigned char key, int x, int y) {
  switch (key) {

  case 'q':
  case 'Q':
  case '\033': // Escキーのこと
    exit(0);
  case '1':
    g_display_mode = 1;
    break;
  case '2':
    g_display_mode = 2;
    break;
  case '3':
    g_display_mode = 3;
    break;
  case '4':
    g_display_mode = 4;
    break;
  case '5':
    g_display_mode = 5;
    break;
  case '6':
    g_display_mode = 6;
    break;
  case '7':
    g_display_mode = 7;
    break;
  case '8':
    g_display_mode = 8;
    break;
  }

  glutPostRedisplay(); //
  // 「ディスプレイのコールバック関数を呼んで」と指示する。→
  // 再描画の必要を訴える
}

// ディスプレイに変化があった時に呼び出されるコールバック関数。
// 「ディスプレイに変化があった時」は、glutPostRedisplay() で指示する。
void glut_display() {
  glClear(GL_COLOR_BUFFER_BIT); // 今まで画面に描かれていたものを消す
  if (g_display_mode >= 3) {
    draw_polygon(g_display_mode);
  }
  // switch (g_display_mode) {
  // case 1:
  //   draw_square1();
  //   break;
  // case 2:
  //   draw_square2();
  //   break;
  // case 3:
  //   draw_square3();
  //   break;
  // }

  glFlush(); // ここで画面に描画をする
}

void draw_polygon(int n) {
  glBegin(GL_LINE_LOOP);
  glColor3d(1.0, 0.0, 0.0);
  GLdouble xx, yy;
  int i;
  double deg, offs = 90.0;
  double r = 0.5;
  for (i = 0; i < n; i++) {
    deg = (360.0 / n) * i + offs;
    xx = r * cos(DEG2RAD(deg));
    yy = r * sin(DEG2RAD(deg));
    glVertex2d(xx, yy);
  }
  glEnd();
}

void draw_square1() {
  glBegin(GL_LINE_LOOP);

  glColor3d(1.0, 0.0, 0.0);
  glVertex2d(-0.9, -0.9);
  glVertex2d(0.9, -0.9);
  glVertex2d(0.9, 0.9);
  glVertex2d(-0.9, 0.9);

  glEnd();
}

void draw_square2() {
  glBegin(GL_POLYGON);

  glColor3d(1.0, 0.0, 0.0);
  glVertex2d(-0.9, -0.9);
  glVertex2d(0.9, -0.9);
  glVertex2d(0.9, 0.9);
  glVertex2d(-0.9, 0.9);

  glEnd();
}

void draw_square3() {
  glBegin(GL_POLYGON);

  glColor3d(1.0, 0.0, 0.0);
  glVertex2d(-0.9, -0.9);
  glColor3d(1.0, 1.0, 0.0);
  glVertex2d(0.9, -0.9);
  glColor3d(0.0, 1.0, 1.0);
  glVertex2d(0.9, 0.9);
  glColor3d(0.0, 0.0, 0.0);
  glVertex2d(-0.9, 0.9);

  glEnd();
}
