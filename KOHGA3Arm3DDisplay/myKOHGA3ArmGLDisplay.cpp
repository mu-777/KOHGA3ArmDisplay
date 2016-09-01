/*----------------------------------------------------------------------------------------*/
/*!
*   @file myKOHGA3ArmGLDisplay.cpp
*   @brief KOHGA3のアームの描画クラス
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
//デフォルトコンストラクタ
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
//デストラクタ
/*----------------------------------------------------------------------------------------*/
GLForKOHGA3::~GLForKOHGA3() {

}

void GLForKOHGA3::setArmData(ArmDataStruct _armdata) {

    currentArmData = _armdata;
}

void GLForKOHGA3::drawArm() {


    //単位は[mm]
    drawGrandAxis();

    //---
    glPushMatrix();
    glRotated(-90.0, 1.0, 0.0, 0.0);
    glLineWidth(2.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawGrand(1000, 1000, 100);
    glPopMatrix();


    glPushMatrix();
    //ボディ
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

    //根本ヨー
    glRotated(currentArmData.baseYaw, 0.0, 1.0, 0.0);
    //第1リンク
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


    //肘ヨー
    glRotated(currentArmData.elbowYaw, 0.0, -1.0, 0.0);
    //第2リンク
    drawSecondLink();
    //手首
    drawWrist();
    glPopMatrix();


    glPopMatrix();


}


void GLForKOHGA3::drawBody() {

    glTranslated(0.0, -kohga3data.body_height / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);
    glColor3d(0.0, 0.0, 0.0);//黒
    drawBox(kohga3data.body_width, kohga3data.body_depth, kohga3data.body_height);
    glLineWidth(5.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawBoxFlame(kohga3data.body_width, kohga3data.body_depth, kohga3data.body_height);
}

void GLForKOHGA3::drawFirstLinks() {

    //肘テーブルサイズ
    double table_d = 80.0;
    double table_w = 60.0;

    //平行リンク
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

    //根本ヨーテーブル
    glTranslated(0.0, 10.0, 0.0);
    glColor3d(0.0, 1.0, 1.0);//水色
    drawBox(table_w + 10, table_d + 10, 10.0);
    glLineWidth(5.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawBoxFlame(table_w + 10, table_d + 10, 10.0);

    //肘ヨーテーブル
    glTranslated(0.0, 0.0, 0.0);
    glTranslated(-kohga3data.firstLink_length * sin(currentArmData.basePitch * DEG2RAD),
                 kohga3data.firstLink_length * cos(currentArmData.basePitch * DEG2RAD),
                 0.0);

    glColor3d(0.0, 1.0, 1.0);//水色
    drawBox(table_w + 10, table_d + 10, 10.0);
    glLineWidth(5.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawBoxFlame(table_w + 10, table_d + 10, 10.0);



}


void GLForKOHGA3::drawFirstLink() {

    glRotated(currentArmData.basePitch, 0.0, 0.0, 1.0);
    glTranslated(0.0, kohga3data.firstLink_length / 2.0, 0.0);
    glRotated(90.0, 0.0, 1.0, 0.0);

    glColor3d(0.0, 1.0, 0.0);//緑
    drawBox(3.0, 15.0, kohga3data.firstLink_length);
    glLineWidth(5.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawBoxFlame(3.0, 15.0, kohga3data.firstLink_length);

}




void GLForKOHGA3::drawSecondLink() {

    //肘テーブルの箱
    glTranslated(0.0, 20.0, 0.0);
    glColor4d(0.0, 0.0, 0.0, 0.5);//黒
    drawBox(50.0, 30.0, 30.0);
    glLineWidth(5.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
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


    //リンク
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
    glColor3d(1.0, 1.0, 1.0);//白
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
    glLineWidth(2.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawBoxFlame(5.0, kohga3data.secondLink_length, 10.0);
    glPopMatrix();
}


void GLForKOHGA3::drawWrist() {

    glTranslated(-kohga3data.secondLink_length, 0.0, 0.0);
    glRotated(currentArmData.wristPitch, 0.0, 0.0, 1.0);
    glRotated(currentArmData.wristYaw, 0.0, -1.0, 0.0);
    glTranslated(-20.0, 0.0, 0.0);

    glColor4d(0.0, 0.0, 0.0, 0.5);//黒
    drawBox(50.0, 30.0, 30.0);
    glLineWidth(5.0);   //線の太さ
    glColor3d(1.0, 1.0, 1.0);//白
    drawBoxFlame(50.0, 30.0, 30.0);

}


