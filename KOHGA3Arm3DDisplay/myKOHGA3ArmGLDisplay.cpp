/*----------------------------------------------------------------------------------------*/
/*!
*   @file myKOHGA3ArmGLDisplay.cpp
*   @brief KOHGA3�̃A�[���̕`��N���X
*   @author Ryosuke MURATA
*   @date 2014/4/28
*   @version 0.0.1
*   @note
*   @attention
*/
/*----------------------------------------------------------------------------------------*/

#include "myKOHGA3ArmGLDisplay.h"
#include <iostream>
#include <Windows.h>

/*----------------------------------------------------------------------------------------*/
//�f�t�H���g�R���X�g���N�^
/*----------------------------------------------------------------------------------------*/
GLForKOHGA3::GLForKOHGA3() {

    ZeroMemory(&currentArmData, sizeof(currentArmData));
    kohga3data.body_width = 140.0;
    kohga3data.body_depth = 500.0;
    kohga3data.body_height = 180.0;
    kohga3data.firstLink_length = 300.0;
    kohga3data.secondLink_length = 400.0;

    upperArmData.baseYaw = UPPERANGLE_BASE_YAW;
    upperArmData.basePitch = UPPERANGLE_BASE_PITCH;
    upperArmData.elbowYaw = UPPERANGLE_ELBOW_YAW;
    upperArmData.elbowPitch = UPPERANGLE_ELBOW_PITCH;
    upperArmData.wristPitch = UPPERANGLE_WRIST_PITCH;
    upperArmData.wristYaw =  UPPERANGLE_WRIST_YAW;

    lowerArmData.baseYaw = LOWERANGLE_BASE_YAW;
    lowerArmData.basePitch = LOWERANGLE_BASE_PITCH;
    lowerArmData.elbowYaw = LOWERANGLE_ELBOW_YAW;
    lowerArmData.elbowPitch = LOWERANGLE_ELBOW_PITCH;
    lowerArmData.wristPitch = LOWERANGLE_WRIST_PITCH;
    lowerArmData.wristYaw =  LOWERANGLE_WRIST_YAW;
}
GLForKOHGA3::GLForKOHGA3(ArmDataStruct _armdata) {

    currentArmData = _armdata;

}

/*----------------------------------------------------------------------------------------*/
//�f�X�g���N�^
/*----------------------------------------------------------------------------------------*/
GLForKOHGA3::~GLForKOHGA3() {

}

void GLForKOHGA3::setArmData(ArmDataStruct _armdata) {

    currentArmData = _armdata;
}

void GLForKOHGA3::drawArm() {


    //�P�ʂ�[mm]
    drawGrandAxis();

    //---
    glPushMatrix();
    glRotated(-90.0, 1.0, 0.0, 0.0);
    glLineWidth(2.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawGrand(1000, 1000, 100);
    glPopMatrix();


    glPushMatrix();
    //�{�f�B
    drawBody();
    glPopMatrix();

    glPushMatrix();
    //UPPER/LOWER
    glPushMatrix();
    glRotated(90.0, 0.0, 0.0, 1.0);
    glRotated(upperArmData.baseYaw, 1.0, 0.0, 0.0);
    glTranslated(0.0, kohga3data.firstLink_length / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, 3.0, kohga3data.firstLink_length);
    glPopMatrix();
    glPushMatrix();
    glRotated(90.0, 0.0, 0.0, 1.0);
    glRotated(lowerArmData.baseYaw, 1.0, 0.0, 0.0);
    glTranslated(0.0, kohga3data.firstLink_length / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, 3.0, kohga3data.firstLink_length);
    glPopMatrix();

    //���{���[
    glRotated(currentArmData.baseYaw, 0.0, 1.0, 0.0);
    //��1�����N
    drawFirstLinks();


    glPushMatrix();
    glRotated(upperArmData.elbowYaw, 0.0, 1.0, 0.0);
    glTranslated(-kohga3data.secondLink_length / 2.0, 0.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, kohga3data.secondLink_length, 3.0);
    glPopMatrix();
    glPushMatrix();
    glRotated(lowerArmData.elbowYaw, 0.0, 1.0, 0.0);
    glTranslated(-kohga3data.secondLink_length / 2.0, 0.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, kohga3data.secondLink_length, 3.0);
    glPopMatrix();


    //�I���[
    glRotated(currentArmData.elbowYaw, 0.0, -1.0, 0.0);
    //��2�����N
    drawSecondLink();
    //���
    drawWrist();
    glPopMatrix();


    glPopMatrix();


}


void GLForKOHGA3::drawBody() {

    glTranslated(0.0, -kohga3data.body_height / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(0.0, 0.0, 0.0);//��
    drawBox(kohga3data.body_width, kohga3data.body_depth, kohga3data.body_height);
    glLineWidth(5.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(kohga3data.body_width, kohga3data.body_depth, kohga3data.body_height);
}

void GLForKOHGA3::drawFirstLinks() {

    //�I�e�[�u���T�C�Y
    double table_d = 80.0;
    double table_w = 60.0;

    //���s�����N
    glPushMatrix();
    glTranslated(table_w / 2.0, 10.0, table_d / 2.0);
    drawFirstLink();
    glPopMatrix();
    glPushMatrix();
    glTranslated(table_w / 2.0, 10.0, -table_d / 2.0);
    drawFirstLink();
    glPopMatrix();
    glPushMatrix();
    glTranslated(-table_w / 2.0, 10.0, table_d / 2.0);
    drawFirstLink();
    glPopMatrix();
    glPushMatrix();
    glTranslated(-table_w / 2.0, 10.0, -table_d / 2.0);
    drawFirstLink();
    glPopMatrix();

    //UPPER/LOWER
    glPushMatrix();
    glRotated(upperArmData.basePitch, 0.0, 0.0, 1.0);
    glTranslated(0.0, kohga3data.firstLink_length / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, 3.0, kohga3data.firstLink_length);
    glPopMatrix();
    glPushMatrix();
    glRotated(lowerArmData.basePitch, 0.0, 0.0, 1.0);
    glTranslated(0.0, kohga3data.firstLink_length / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, 3.0, kohga3data.firstLink_length);
    glPopMatrix();

    //���{���[�e�[�u��
    glTranslated(0.0, 10.0, 0.0);
    glColor3d(0.0, 1.0, 1.0);//���F
    drawBox(table_w + 10, table_d + 10, 10.0);
    glLineWidth(5.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(table_w + 10, table_d + 10, 10.0);

    //�I���[�e�[�u��
    glTranslated(0.0, 0.0, 0.0);
    glTranslated(-kohga3data.firstLink_length * sin(currentArmData.basePitch * DEG2RAD),
                 kohga3data.firstLink_length * cos(currentArmData.basePitch * DEG2RAD),
                 0.0);

    glColor3d(0.0, 1.0, 1.0);//���F
    drawBox(table_w + 10, table_d + 10, 10.0);
    glLineWidth(5.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(table_w + 10, table_d + 10, 10.0);



}


void GLForKOHGA3::drawFirstLink() {

    glRotated(currentArmData.basePitch, 0.0, 0.0, 1.0);
    glTranslated(0.0, kohga3data.firstLink_length / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);

    glColor3d(0.0, 1.0, 0.0);//��
    drawBox(3.0, 15.0, kohga3data.firstLink_length);
    glLineWidth(5.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(3.0, 15.0, kohga3data.firstLink_length);

}




void GLForKOHGA3::drawSecondLink() {

    //�I�e�[�u���̔�
    glTranslated(0.0, 20.0, 0.0);
    glColor4d(0.0, 0.0, 0.0, 0.5);//��
    drawBox(50.0, 30.0, 30.0);
    glLineWidth(5.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(50.0, 30.0, 30.0);


    //UPPER/LOWER
    glPushMatrix();
    glRotated(upperArmData.elbowPitch, 0.0, 0.0, -1.0);
    glTranslated(-kohga3data.secondLink_length / 2.0, 0.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, kohga3data.secondLink_length, 3.0);
    glPopMatrix();
    glPushMatrix();
    glRotated(lowerArmData.elbowPitch, 0.0, 0.0, -1.0);
    glTranslated(-kohga3data.secondLink_length / 2.0, 0.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(3.0, kohga3data.secondLink_length, 3.0);
    glPopMatrix();


    //�����N
    double initsecondLink_length = 400.0;
    if (currentArmData.linear <= 300.0) {
        kohga3data.secondLink_length = initsecondLink_length + 100.0 * (currentArmData.linear / 300.0);
    }
    if (currentArmData.linear >= 360.0) {
        kohga3data.secondLink_length = initsecondLink_length + 100.0 + 100.0 * ((currentArmData.linear - 360.0) / 300.0);
    }

    glRotated(currentArmData.elbowPitch, 0.0, 0.0, -1.0);
    glPushMatrix();
    glTranslated(-initsecondLink_length / 2.0, 0.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(1.0, 0.0, 0.0);//red
    drawBox(8.0, initsecondLink_length, 15.0);
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(8.0, initsecondLink_length, 15.0);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-kohga3data.secondLink_length / 2.0, 0.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    if (currentArmData.linear < LOWERANGLE_LINEAR || currentArmData.linear > UPPERANGLE_LINEAR) {
        glColor3d(1.0, 0.0, 0.0);   //red
    } else {
        glColor3d(0.0, 1.0, 0.0);   //green
    }
    drawBox(5.0, kohga3data.secondLink_length, 10.0);
    glLineWidth(2.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(5.0, kohga3data.secondLink_length, 10.0);
    glPopMatrix();
}


void GLForKOHGA3::drawWrist() {

    glTranslated(-kohga3data.secondLink_length, 0.0, 0.0);
    glRotated(currentArmData.wristPitch, 0.0, 0.0, 1.0);
    glRotated(currentArmData.wristYaw, 0.0, -1.0, 0.0);
    glTranslated(-20.0, 0.0, 0.0);

    glColor4d(0.0, 0.0, 0.0, 0.5);//��
    drawBox(50.0, 30.0, 30.0);
    glLineWidth(5.0);   //���̑���
    glColor3d(1.0, 1.0, 1.0);//��
    drawBoxFlame(50.0, 30.0, 30.0);

}


