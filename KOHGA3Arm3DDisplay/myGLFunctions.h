//====================================================================================//
//名前：myGLFunctions.h
//説明：OpenGLにより描画する図形と立体の素材
//====================================================================================//
#ifndef MYGLFUNCTIONS_H
#define MYGLFUNCTIONS_H

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

#include <math.h>
#include <string>

//#define GLUT_BUILDING_LIB//これをつけないと#include <GL/glut.h> でコンパイルエラーが出ることがある．
#include "freeglut.h" // GL/glut.hを読み込めば，GL/gl.h, GL/glu.hも読み込まれる



void drawCylinder(const float radius, const int div , const float width); //円柱(ワイヤーモデル)
void drawCylinderSurface(const float radius, const int div , const float width); //円柱(サーフェスモデル)

void drawBox(double width, double depth, double height);//箱
void drawBoxFlame(double width, double depth, double height);//箱

void drawSphere(double sphere_size);//球
void drawCircle(const float radius, const int div); //円
void drawCircleEdge(const float radius, const int div);//円周

void drawGrandAxis(void); //座標軸

void drawGrand(int width, int depth, int interval); //地面
void drawGrand2(double width, double depth, double interval, double power);

void drawGrandRadial(double radius , double degree); //地面（自機からの放射状線）
void drawGrandEqualDistance(double radius , double interval);   //地面（自機周りの等距離線）
void drawLine(double startPoint[], double endPoint[]);
void drawEye(float width, float height);
void drawEye2(double width, double height, double h_gakaku);
void drawObstacle(void);
void drawRailroadLine(double modelSize[3], double startPos[3], double endPos[3]);
void drawRailroadLine(double modelSize[3], double aspect, double h_gakaku, double startPos[6], double endPos[3]);
void drawString(double global_x, double global_y, double global_z, std::string const& str);
void drawString(double screen_x, double screen_y, double width, double height, std::string const& str);
void drawStringBig(double screen_x, double screen_y, double width, double height, std::string const& str);
void drawLayerOnScreen(double width, double height, double alpha);




#endif