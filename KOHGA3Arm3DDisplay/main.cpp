
//------------------------
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "myKOHGA3ArmGLDisplay.h"
#include "udpcomm.h"

//�}�N�� --------------------------------------------------------------------------------------
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
    double distance;    // ���_���畨�̂܂ł̋��� [m]
    double twist;       // �������̉�]�p�x [rad]
    double elevation;   // ���̂������낷�p�x [rad]
    double azimuth;     // ���������̉�]�p�x [rad]
    double s;           // ��ʏ�̈ʒu�i�������j
    double t;           // ��ʏ�̈ʒu�i�c�����j
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


    //�A�[���֘A
    armdataReceiveServer.InitializeAsServer(ARMSTATE_PORT);
    ZeroMemory(&currentArmData, sizeof(currentArmData));
    //�R�}���h��M
    //while(1){
    //  if(!armdataReceiveServer.CheckReadFlag(3)){
    //      printf("�R�}���h�𑗐M���Ă�������...\r");
    //  }else{
    //      int checkNum = armdataReceiveServer.ReceiveData(&currentArmData, sizeof(currentArmData));
    //      if(checkNum==SOCKET_ERROR){printf("�R�}���h��M���s                        \r");}
    //      else{printf("�R�}���h��M�����I�I�I                    \r\n"); break;}
    //  }
    //}


    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    displayID[0] = glutCreateWindow("display1");
    glutKeyboardFunc(keyboard1);        // ��ʃL�[
    glutMouseFunc(mouseButton1);        // �}�E�X�N���b�N
    glutMouseWheelFunc(mouseWheel1);    // �}�E�X�z�C�[��
    glutMotionFunc(mouseMotion1);   // �}�E�X�h���b�O
    //�`��֐�
    glutDisplayFunc(display1);
    glutReshapeFunc(reshape1);
    glutIdleFunc(idle);             // �A�C�h���F�������C�����[�v�E�f�[�^�̎擾�ȂǁiNULL�ɂ���Ɠo�^�Ȃ��j
    init1();

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    displayID[1] = glutCreateWindow("display2");
    glutKeyboardFunc(keyboard2);        // ��ʃL�[
    glutMouseFunc(mouseButton2);        // �}�E�X�N���b�N
    glutMouseWheelFunc(mouseWheel2);    // �}�E�X�z�C�[��
    glutMotionFunc(mouseMotion2);   // �}�E�X�h���b�O
    //�`��֐�
    glutDisplayFunc(display2);
    glutReshapeFunc(reshape2);
    glutIdleFunc(idle);             // �A�C�h���F�������C�����[�v�E�f�[�^�̎擾�ȂǁiNULL�ɂ���Ɠo�^�Ȃ��j
    init2();


    glutMainLoop();     //�C�x���g�҂��������[�v
    end();


    return 0;
}

void end(void) {

    //UDP�I��
    armdataReceiveServer.CloseSocket();


}
void idle(void) {

    // �A�[���̏�Ԃ���M
    ArmDataStruct currentArmData_temp;
    if (armdataReceiveServer.CheckReadFlag(1)) {
        int checkNum = armdataReceiveServer.ReceiveData(&currentArmData_temp, sizeof(currentArmData_temp));
        if (checkNum == SOCKET_ERROR) {
            printf("�R�}���h��M���s\n");
        } else {
            currentArmData = currentArmData_temp;
        } //��M����������currentArmData���X�V
    }


    // �A�[���̏�Ԃ��Z�b�g
    arm.setArmData(currentArmData);

    for (int loop = 0; loop < 2; ++loop) {
        glutSetWindow(displayID[loop]);
        glutPostRedisplay(); //�ĕ`�� (��display()�֐����Ăяo���֐� )
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


    glClearColor(0.0, 0.0, 1.0, 1.0);//�w�i�F�w��

    // ���e�ϊ��s��X�^�b�N�𑀍�ΏۂƂ���
    glMatrixMode(GL_PROJECTION);
}

void display1(void) {

    glClear(GL_COLOR_BUFFER_BIT |   //�J���[�o�b�t�@�̏�����
            GL_DEPTH_BUFFER_BIT);   //�f�v�X�o�b�t�@�̏�����

    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();   //���f���r���[�ϊ��s��̏�����

    //gluLookAt(15.0+g_camera.s, 15.0, 15.0,    //���̍��W����
    //          0.0, 0.0, 0.0,  //���̓_������
    //          0.0, 0.0, 1.0); //��͂��̕���

    glPushMatrix();
    //------------
    glTranslated(g_camera[0].s, -g_camera[0].t, -g_camera[0].distance);// ���_���畨�̂܂ł̋���
    glRotated(-g_camera[0].twist * RAD2DEG, 0.0, 0.0, 1.0); // �������̉�]�p�x
    glRotated(-g_camera[0].elevation * RAD2DEG, 1.0, 0.0, 0.0); // ���̂������낷�p�x
    glRotated(-g_camera[0].azimuth * RAD2DEG, 0.0, 1.0, 0.0); // ���������̉�]�p�x
    //------------

    glPushMatrix();
    arm.drawArm();  //�A�[���`��
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();  //�o�b�t�@�̃X���b�v�i�_�u���o�b�t�@���[�h�ł̕`��R�}���h(�Q�l:http://www.ics.kagoshima-u.ac.jp/edu/ProgramingJava/UsingImage/s5.html)�j

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
                g_camera[0] = g_initCamera[0];//�������_�֖߂�
                break;
        }
        // �h���b�O�J�n�_��x, y���W�l���擾
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


    //�}�E�X�z�C�[���̃X�N���[���ʂɉ����ăY�[��
    //�@�z�C�[�����������Ƃ��ɌĂяo����邱�̊֐����ōX�V�O���[�o���ϐ����X�V����K�v������I
    g_camera[0].distance -= (double)wheelScroll * 10.0;

    // ���ʂ�`��ɔ��f
    glutPostRedisplay();
}

void mouseMotion1(int x, int y) {
    //�ϐ�
    int xDisp, yDisp;

    // �}�E�X�ړ������̌v�Z
    xDisp = x - g_xBegin[0];
    yDisp = y - g_yBegin[0];

    switch (g_mButton[0]) {
        // ���{�^���̃h���b�O�ŕ��̂Ǝ��_�̊p�x��ς���
        case GLUT_LEFT_BUTTON:
            g_camera[0].azimuth -= ((double) xDisp / 2.0) * DEG2RAD;
            g_camera[0].elevation -= ((double) yDisp / 2.0) * DEG2RAD;
            break;

        // �����{�^���̃h���b�O�ŉ��2�������ʂ̈ړ�
        case GLUT_MIDDLE_BUTTON:
            g_camera[0].s += (double) xDisp / 1.0;
            g_camera[0].t += (double) yDisp / 1.0;
            break;

        // �E�{�^���̃h���b�O�͂Ȃɂ��Ȃ�
        case GLUT_RIGHT_BUTTON:
            break;
    }

    // ���̃X�e�b�v�̃}�E�X�̏o���_�̍X�V
    g_xBegin[0] = x;
    g_yBegin[0] = y;



    // �`��ɔ��f
    glutPostRedisplay();
}


void keyboard1(unsigned char key, int x, int y) {

    //if(&keyboardInput==NULL){memset(&keyboardInput, 0.0, sizeof(keyboardInput));}

    //switch(key){

    //case 'x'://x������0.01�i�񂾂Ƃ���
    //  keyboardInput.x += 0.01;
    //  break;

    //case 'y'://y������0.01�i�񂾂Ƃ���
    //  keyboardInput.y += 0.01;
    //  break;

    //case 'z'://z������0.01�i�񂾂Ƃ���
    //  keyboardInput.z += 0.01;
    //  break;
    //}

    // �`��ɔ��f
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



    glClearColor(0.0, 0.0, 1.0, 1.0);//�w�i�F�w��

    // ���e�ϊ��s��X�^�b�N�𑀍�ΏۂƂ���
    glMatrixMode(GL_PROJECTION);
}

void display2(void) {

    glClear(GL_COLOR_BUFFER_BIT |   //�J���[�o�b�t�@�̏�����
            GL_DEPTH_BUFFER_BIT);   //�f�v�X�o�b�t�@�̏�����

    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();   //���f���r���[�ϊ��s��̏�����

    //gluLookAt(15.0+g_camera.s, 15.0, 15.0,    //���̍��W����
    //          0.0, 0.0, 0.0,  //���̓_������
    //          0.0, 0.0, 1.0); //��͂��̕���

    glPushMatrix();
    //------------
    glTranslated(g_camera[1].s, -g_camera[1].t, -g_camera[1].distance);// ���_���畨�̂܂ł̋���
    glRotated(-g_camera[1].twist * RAD2DEG, 0.0, 0.0, 1.0); // �������̉�]�p�x
    glRotated(-g_camera[1].elevation * RAD2DEG, 1.0, 0.0, 0.0); // ���̂������낷�p�x
    glRotated(-g_camera[1].azimuth * RAD2DEG, 0.0, 1.0, 0.0); // ���������̉�]�p�x
    //------------

    glPushMatrix();
    arm.drawArm();  //�A�[���`��
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();  //�o�b�t�@�̃X���b�v�i�_�u���o�b�t�@���[�h�ł̕`��R�}���h(�Q�l:http://www.ics.kagoshima-u.ac.jp/edu/ProgramingJava/UsingImage/s5.html)�j

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
                g_camera[1] = g_initCamera[1];//�������_�֖߂�
                break;
        }
        // �h���b�O�J�n�_��x, y���W�l���擾
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


    //�}�E�X�z�C�[���̃X�N���[���ʂɉ����ăY�[��
    //�@�z�C�[�����������Ƃ��ɌĂяo����邱�̊֐����ōX�V�O���[�o���ϐ����X�V����K�v������I
    g_camera[1].distance -= (double)wheelScroll * 10.0;

    // ���ʂ�`��ɔ��f
    glutPostRedisplay();
}

void mouseMotion2(int x, int y) {
    //�ϐ�
    int xDisp, yDisp;

    // �}�E�X�ړ������̌v�Z
    xDisp = x - g_xBegin[1];
    yDisp = y - g_yBegin[1];

    switch (g_mButton[1]) {
        // ���{�^���̃h���b�O�ŕ��̂Ǝ��_�̊p�x��ς���
        case GLUT_LEFT_BUTTON:
            g_camera[1].azimuth -= ((double) xDisp / 2.0) * DEG2RAD;
            g_camera[1].elevation -= ((double) yDisp / 2.0) * DEG2RAD;
            break;

        // �����{�^���̃h���b�O�ŉ��2�������ʂ̈ړ�
        case GLUT_MIDDLE_BUTTON:
            g_camera[1].s += (double) xDisp / 1.0;
            g_camera[1].t += (double) yDisp / 1.0;
            break;

        // �E�{�^���̃h���b�O�͂Ȃɂ��Ȃ�
        case GLUT_RIGHT_BUTTON:
            break;
    }

    // ���̃X�e�b�v�̃}�E�X�̏o���_�̍X�V
    g_xBegin[1] = x;
    g_yBegin[1] = y;



    // �`��ɔ��f
    glutPostRedisplay();
}


void keyboard2(unsigned char key, int x, int y) {

    //if(&keyboardInput==NULL){memset(&keyboardInput, 0.0, sizeof(keyboardInput));}

    //switch(key){

    //case 'x'://x������0.01�i�񂾂Ƃ���
    //  keyboardInput.x += 0.01;
    //  break;

    //case 'y'://y������0.01�i�񂾂Ƃ���
    //  keyboardInput.y += 0.01;
    //  break;

    //case 'z'://z������0.01�i�񂾂Ƃ���
    //  keyboardInput.z += 0.01;
    //  break;
    //}

    // �`��ɔ��f
    glutPostRedisplay();
}
