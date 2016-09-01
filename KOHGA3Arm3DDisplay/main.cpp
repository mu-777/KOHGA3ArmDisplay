
//------------------------
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "myKOHGA3ArmGLDisplay.h"
#include "udpcomm.h"

//マクロ --------------------------------------------------------------------------------------
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef DEG2RAD
    #define DEG2RAD 0.017453295199432957692             //(M_PI/180.0)
#endif

#ifndef RAD2DEG
    #define RAD2DEG 57.29577951308232876846364344191    //(180.0/M_PI)
#endif

//------------------------
#define WIDTH 640
#define HEIGHT 480

#define ARMSTATE_PORT 7800

//------------------------
typedef struct {
    double distance;    // 視点から物体までの距離 [m]
    double twist;       // 視線回りの回転角度 [rad]
    double elevation;   // 物体を見下ろす角度 [rad]
    double azimuth;     // 鉛直軸回りの回転角度 [rad]
    double s;           // 画面上の位置（横方向）
    double t;           // 画面上の位置（縦方向）
} camera_data;

//------------------------
void display1(void);
void reshape1(int w, int h);
void mouseButton1(int button, int state, int x, int y);
void mouseWheel1(int wheel_num, int direction, int x, int y);
void mouseMotion1(int x, int y);
void keyboard1(unsigned char key, int x, int y);
void init1(void);

void display2(void);
void reshape2(int w, int h);
void mouseButton2(int button, int state, int x, int y);
void mouseWheel2(int wheel_num, int direction, int x, int y);
void mouseMotion2(int x, int y);
void keyboard2(unsigned char key, int x, int y);
void init2(void);

void idle(void);
void end(void);

//------------------------
int g_mButton[2] = {0, 0};
int g_xBegin[2] = {0, 0};
int g_yBegin[2] = {0, 0};
camera_data g_camera[2];
camera_data g_initCamera[2];
int displayID[2];
//------------------------
GLForKOHGA3 arm;
ArmDataStruct currentArmData;
UDPComm armdataReceiveServer;




int main(int argc, char** argv) {


    //アーム関連
    armdataReceiveServer.InitializeAsServer(ARMSTATE_PORT);
    ZeroMemory(&currentArmData, sizeof(currentArmData));
    //コマンド受信
    //while(1){
    //  if(!armdataReceiveServer.CheckReadFlag(3)){
    //      printf("コマンドを送信してください...\r");
    //  }else{
    //      int checkNum = armdataReceiveServer.ReceiveData(&currentArmData, sizeof(currentArmData));
    //      if(checkNum==SOCKET_ERROR){printf("コマンド受信失敗                        \r");}
    //      else{printf("コマンド受信成功！！！                    \r\n"); break;}
    //  }
    //}


    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    displayID[0] = glutCreateWindow("display1");
    glutKeyboardFunc(keyboard1);        // 一般キー
    glutMouseFunc(mouseButton1);        // マウスクリック
    glutMouseWheelFunc(mouseWheel1);    // マウスホイール
    glutMotionFunc(mouseMotion1);   // マウスドラッグ
    //描画関数
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape1);
    glutIdleFunc(idle);             // アイドル：実質メインループ・データの取得など（NULLにすると登録なし）
    init1();

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    displayID[1] = glutCreateWindow("display2");
    glutKeyboardFunc(keyboard2);        // 一般キー
    glutMouseFunc(mouseButton2);        // マウスクリック
    glutMouseWheelFunc(mouseWheel2);    // マウスホイール
    glutMotionFunc(mouseMotion2);   // マウスドラッグ
    //描画関数
    glutDisplayFunc(display2);
    glutReshapeFunc(reshape2);
    glutIdleFunc(idle);             // アイドル：実質メインループ・データの取得など（NULLにすると登録なし）
    init2();


    glutMainLoop();     //イベント待ち無限ループ
    end();


    return 0;
}

void end(void) {

    //UDP終了
    armdataReceiveServer.CloseSocket();


}
void idle(void) {

    // アームの状態を受信
    ArmDataStruct currentArmData_temp;
    if (armdataReceiveServer.CheckReadFlag(1)) {
        int checkNum = armdataReceiveServer.ReceiveData(&currentArmData_temp, sizeof(currentArmData_temp));
        if (checkNum == SOCKET_ERROR) {
            printf("コマンド受信失敗\n");
        } else {
            currentArmData = currentArmData_temp;
        } //受信成功したらcurrentArmDataを更新
    }


    // アームの状態をセット
    arm.setArmData(currentArmData);

    for (int loop = 0; loop < 2; ++loop) {
        glutSetWindow(displayID[loop]);
        glutPostRedisplay(); //再描画 (※display()関数を呼び出す関数 )
    }

}

void init1(void) {

    ZeroMemory(&g_camera[0], sizeof(g_camera[0]));
    g_initCamera[0].s = 0.0;
    g_initCamera[0].t = 300.0;
    g_initCamera[0].distance = 1700.0;
    g_initCamera[0].azimuth = 0.0;
    g_initCamera[0].elevation = -3.0 * DEG2RAD;
    g_initCamera[0].twist = 0.0;
    g_camera[0] = g_initCamera[0];


    glClearColor(0.0, 0.0, 1.0, 1.0);//背景色指定

    // 投影変換行列スタックを操作対象とする
    glMatrixMode(GL_PROJECTION);
}

void display1(void) {

    glClear(GL_COLOR_BUFFER_BIT |   //カラーバッファの初期化
            GL_DEPTH_BUFFER_BIT);   //デプスバッファの初期化

    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();   //モデルビュー変換行列の初期化

    //gluLookAt(15.0+g_camera.s, 15.0, 15.0,    //この座標から
    //          0.0, 0.0, 0.0,  //この点を見る
    //          0.0, 0.0, 1.0); //上はこの方向

    glPushMatrix();
    //------------
    glTranslated(g_camera[0].s, -g_camera[0].t, -g_camera[0].distance);// 視点から物体までの距離
    glRotated(-g_camera[0].twist * RAD2DEG, 0.0, 0.0, 1.0); // 視線回りの回転角度
    glRotated(-g_camera[0].elevation * RAD2DEG, 1.0, 0.0, 0.0); // 物体を見下ろす角度
    glRotated(-g_camera[0].azimuth * RAD2DEG, 0.0, 1.0, 0.0); // 鉛直軸回りの回転角度
    //------------

    glPushMatrix();
    arm.drawArm();  //アーム描画
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();  //バッファのスワップ（ダブルバッファモードでの描画コマンド(参考:http://www.ics.kagoshima-u.ac.jp/edu/ProgramingJava/UsingImage/s5.html)）

}



void reshape1(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100000000000000.0);
    glMatrixMode(GL_MODELVIEW);
}


void mouseButton1(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {

            case GLUT_LEFT_BUTTON:
                g_mButton[0] = button;
                break;
            case GLUT_MIDDLE_BUTTON:
                g_mButton[0] = button;
                break;
            case GLUT_RIGHT_BUTTON:
                g_mButton[0] = button;
                g_camera[0] = g_initCamera[0];//初期視点へ戻す
                break;
        }
        // ドラッグ開始点のx, y座標値を取得
        g_xBegin[0] = x;
        g_yBegin[0] = y;
    }
}

void mouseWheel1(int wheel_num, int direction, int x, int y) {

    int wheelScroll = 0;

    if (direction == 1) {
        wheelScroll = 5;
    } else if (direction == -1) {
        wheelScroll = -5;
    } else {
        wheelScroll = 0;
    }


    //マウスホイールのスクロール量に応じてズーム
    //　ホイールが動いたときに呼び出されるこの関数内で更新グローバル変数を更新する必要がある！
    g_camera[0].distance -= (double)wheelScroll * 10.0;

    // 結果を描画に反映
    glutPostRedisplay();
}

void mouseMotion1(int x, int y) {
    //変数
    int xDisp, yDisp;

    // マウス移動距離の計算
    xDisp = x - g_xBegin[0];
    yDisp = y - g_yBegin[0];

    switch (g_mButton[0]) {
        // 左ボタンのドラッグで物体と視点の角度を変える
        case GLUT_LEFT_BUTTON:
            g_camera[0].azimuth -= ((double) xDisp / 2.0) * DEG2RAD;
            g_camera[0].elevation -= ((double) yDisp / 2.0) * DEG2RAD;
            break;

        // 中央ボタンのドラッグで画面2次元平面の移動
        case GLUT_MIDDLE_BUTTON:
            g_camera[0].s += (double) xDisp / 1.0;
            g_camera[0].t += (double) yDisp / 1.0;
            break;

        // 右ボタンのドラッグはなにもなし
        case GLUT_RIGHT_BUTTON:
            break;
    }

    // 次のステップのマウスの出発点の更新
    g_xBegin[0] = x;
    g_yBegin[0] = y;



    // 描画に反映
    glutPostRedisplay();
}


void keyboard1(unsigned char key, int x, int y) {

    //if(&keyboardInput==NULL){memset(&keyboardInput, 0.0, sizeof(keyboardInput));}

    //switch(key){

    //case 'x'://x方向に0.01進んだとする
    //  keyboardInput.x += 0.01;
    //  break;

    //case 'y'://y方向に0.01進んだとする
    //  keyboardInput.y += 0.01;
    //  break;

    //case 'z'://z方向に0.01進んだとする
    //  keyboardInput.z += 0.01;
    //  break;
    //}

    // 描画に反映
    glutPostRedisplay();
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void init2(void) {

    ZeroMemory(&g_camera[1], sizeof(g_camera[1]));
    g_initCamera[1].s = 0.0;
    g_initCamera[1].t = 0.0;
    g_initCamera[1].distance = 1700.0;
    g_initCamera[1].azimuth = 0.0;
    g_initCamera[1].elevation = -90.0 * DEG2RAD;
    g_initCamera[1].twist = 0.0;
    g_camera[1] = g_initCamera[1];



    glClearColor(0.0, 0.0, 1.0, 1.0);//背景色指定

    // 投影変換行列スタックを操作対象とする
    glMatrixMode(GL_PROJECTION);
}

void display2(void) {

    glClear(GL_COLOR_BUFFER_BIT |   //カラーバッファの初期化
            GL_DEPTH_BUFFER_BIT);   //デプスバッファの初期化

    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();   //モデルビュー変換行列の初期化

    //gluLookAt(15.0+g_camera.s, 15.0, 15.0,    //この座標から
    //          0.0, 0.0, 0.0,  //この点を見る
    //          0.0, 0.0, 1.0); //上はこの方向

    glPushMatrix();
    //------------
    glTranslated(g_camera[1].s, -g_camera[1].t, -g_camera[1].distance);// 視点から物体までの距離
    glRotated(-g_camera[1].twist * RAD2DEG, 0.0, 0.0, 1.0); // 視線回りの回転角度
    glRotated(-g_camera[1].elevation * RAD2DEG, 1.0, 0.0, 0.0); // 物体を見下ろす角度
    glRotated(-g_camera[1].azimuth * RAD2DEG, 0.0, 1.0, 0.0); // 鉛直軸回りの回転角度
    //------------

    glPushMatrix();
    arm.drawArm();  //アーム描画
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();  //バッファのスワップ（ダブルバッファモードでの描画コマンド(参考:http://www.ics.kagoshima-u.ac.jp/edu/ProgramingJava/UsingImage/s5.html)）

}



void reshape2(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100000000000000.0);
    glMatrixMode(GL_MODELVIEW);
}


void mouseButton2(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        switch (button) {

            case GLUT_LEFT_BUTTON:
                g_mButton[1] = button;
                break;
            case GLUT_MIDDLE_BUTTON:
                g_mButton[1] = button;
                break;
            case GLUT_RIGHT_BUTTON:
                g_mButton[1] = button;
                g_camera[1] = g_initCamera[1];//初期視点へ戻す
                break;
        }
        // ドラッグ開始点のx, y座標値を取得
        g_xBegin[1] = x;
        g_yBegin[1] = y;
    }
}

void mouseWheel2(int wheel_num, int direction, int x, int y) {

    int wheelScroll = 0;

    if (direction == 1) {
        wheelScroll = 5;
    } else if (direction == -1) {
        wheelScroll = -5;
    } else {
        wheelScroll = 0;
    }


    //マウスホイールのスクロール量に応じてズーム
    //　ホイールが動いたときに呼び出されるこの関数内で更新グローバル変数を更新する必要がある！
    g_camera[1].distance -= (double)wheelScroll * 10.0;

    // 結果を描画に反映
    glutPostRedisplay();
}

void mouseMotion2(int x, int y) {
    //変数
    int xDisp, yDisp;

    // マウス移動距離の計算
    xDisp = x - g_xBegin[1];
    yDisp = y - g_yBegin[1];

    switch (g_mButton[1]) {
        // 左ボタンのドラッグで物体と視点の角度を変える
        case GLUT_LEFT_BUTTON:
            g_camera[1].azimuth -= ((double) xDisp / 2.0) * DEG2RAD;
            g_camera[1].elevation -= ((double) yDisp / 2.0) * DEG2RAD;
            break;

        // 中央ボタンのドラッグで画面2次元平面の移動
        case GLUT_MIDDLE_BUTTON:
            g_camera[1].s += (double) xDisp / 1.0;
            g_camera[1].t += (double) yDisp / 1.0;
            break;

        // 右ボタンのドラッグはなにもなし
        case GLUT_RIGHT_BUTTON:
            break;
    }

    // 次のステップのマウスの出発点の更新
    g_xBegin[1] = x;
    g_yBegin[1] = y;



    // 描画に反映
    glutPostRedisplay();
}


void keyboard2(unsigned char key, int x, int y) {

    //if(&keyboardInput==NULL){memset(&keyboardInput, 0.0, sizeof(keyboardInput));}

    //switch(key){

    //case 'x'://x方向に0.01進んだとする
    //  keyboardInput.x += 0.01;
    //  break;

    //case 'y'://y方向に0.01進んだとする
    //  keyboardInput.y += 0.01;
    //  break;

    //case 'z'://z方向に0.01進んだとする
    //  keyboardInput.z += 0.01;
    //  break;
    //}

    // 描画に反映
    glutPostRedisplay();
}
