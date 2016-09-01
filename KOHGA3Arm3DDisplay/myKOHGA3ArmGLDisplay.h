//====================================================================================//
//���O�FmyKOHGA3ArmGLDisplay.h
//�����F
//====================================================================================//
#ifndef MYKOHGA3ARMGLDISPLAY_H
#define MYKOHGA3ARMGLDISPLAY_H

#include "myGLFunctions.h"

//�A�[���̊e�֐ߊp�x�̃��~�b�g
#define UPPERANGLE_BASE_YAW      70 /*! ���_�l�ł�80�܂ŉ� */
#define LOWERANGLE_BASE_YAW     -70 /*! ���_�l�ł�-80�܂ŉ� */
#define UPPERANGLE_BASE_PITCH    50 /*! */
#define LOWERANGLE_BASE_PITCH   -70 /*! */
#define UPPERANGLE_ELBOW_YAW     90 /*!checked ���_�l�ł�120�܂ŉ� */
#define LOWERANGLE_ELBOW_YAW    -90 /*!checked ���_�l�ł�-120�܂ŉ� */
#define UPPERANGLE_ELBOW_PITCH   60 /*!checked */
#define LOWERANGLE_ELBOW_PITCH  -10 /*!checked */
#define UPPERANGLE_LINEAR       640 /*! */
#define LOWERANGLE_LINEAR       40  /*! */
#define UPPERANGLE_WRIST_YAW     90/*!checked ���_�l�ł�150�܂ŉ� */
#define LOWERANGLE_WRIST_YAW    -90/*!checked ���_�l�ł�-150�܂ŉ� */
#define UPPERANGLE_WRIST_PITCH   90/*!checked ���_�l�ł�150�܂ŉ� */
#define LOWERANGLE_WRIST_PITCH  -90/*!checked ���_�l�ł�-150�܂ŉ� */



typedef struct {
    double baseYaw;
    double basePitch;
    double elbowYaw;
    double elbowPitch;
    double linear;
    double wristYaw;
    double wristPitch;
} ArmDataStruct;

typedef struct {
    double body_width;
    double body_depth;
    double body_height;
    double firstLink_length;
    double secondLink_length;
} KOHGA3Data; //[mm]

class GLForKOHGA3 {
private:
    ArmDataStruct currentArmData;
    ArmDataStruct upperArmData;
    ArmDataStruct lowerArmData;
    KOHGA3Data kohga3data;

    void drawBody();
    void drawFirstLinks();
    void drawFirstLink();
    void drawSecondLink();
    void drawWrist();

public:
    GLForKOHGA3();
    GLForKOHGA3(ArmDataStruct _armdata);
    ~GLForKOHGA3();

    void setArmData(ArmDataStruct _armdata);
    void drawArm();


};
#endif