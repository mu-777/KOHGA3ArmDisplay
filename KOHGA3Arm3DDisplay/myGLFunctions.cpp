//====================================================================================//
//名前：myGLFunctions.cpp
//説明：OpenGlにより描画する図形と立体の素材
//====================================================================================//

//インクルード
#include "myGLFunctions.h"

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

/*----------------------------------------------------------------------------
名前：draw_cylinder
説明：ワイヤモデルの円柱の描画
引数>
radius：半径[m]
div：分割数
width：円柱の幅（高さ）[m]
戻り値>
void
----------------------------------------------------------------------------*/
void drawCylinder(const float radius, const int div , const float width) {

    float x, z;
    float dth = 2 * (float)M_PI / (float)div;

    glPushMatrix();
    //底面の描写
    glBegin(GL_LINE_LOOP);
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        z = radius * sin(th);
        glVertex3f(x, 0, z);          //描画したい円周上の頂点を並べる
    }
    glEnd();

    //上面の描写
    glBegin(GL_LINE_LOOP);
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        z = radius * sin(th);
        glVertex3f(x, width, z);          //描画したい円周上の頂点を並べる
    }
    glEnd();

    //底面と上面の点を結ぶ
    glBegin(GL_LINES);
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        z = radius * sin(th);
        glVertex3f(x, 0, z);              //底面と上面の点を結ぶ
        glVertex3f(x, width, z);
    }
    glEnd();

}

/*------------------------------------------------------------------------------
名前：drawCylinderSurface
説明：サーフェスモデルの円柱の描画
補足：GL_POLYGONは処理が重いらしいので,GL_TRIANGLE_STRIPを使用
引数>
radius：半径[m]
div：分割数
width：円柱の幅（高さ）[m]
戻り値>
void
-------------------------------------------------------------------------------*/
void drawCylinderSurface(const float radius, const int div , const float width) {

    float x, x2, z, z2;
    float dth = 2 * (float)M_PI / (float)div;

    //底面の描写(表面あり,線はなし
    glBegin(GL_TRIANGLE_STRIP);//処理速度が遅い
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        z = radius * sin(th);
        x2 = radius * cos(th + dth);
        z2 = radius * sin(th + dth);
        glVertex3f(x, 0.0, z);          //描画したい円周上の頂点を並べる
        glVertex3f(x2, 0.0, z2);
        glVertex3f(0.0, 0.0, 0.0);
    }
    glEnd();

    //上面の描写(表面あり．線はなし)
    glBegin(GL_TRIANGLE_STRIP);//処理速度が遅い
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        z = radius * sin(th);
        x2 = radius * cos(th + dth);
        z2 = radius * sin(th + dth);
        glVertex3f(x, width, z);          //描画したい円周上の頂点を並べる
        glVertex3f(x2, width, z2);
        glVertex3f(0.0, width, 0.0);
    }
    glEnd();


    //底面と上面の点を結ぶ
    glBegin(GL_QUADS);
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        z = radius * sin(th);
        x2 = radius * cos(th + dth);
        z2 = radius * sin(th + dth);
        glVertex3f(x, 0, z);              //底面と上面の点を結ぶ
        glVertex3f(x, width, z);
        glVertex3f(x2, width, z2);
        glVertex3f(x2, 0, z2);
    }
    glEnd();

}
/*------------------------------------------------------------------------------
名前：drawBox
説明：箱を描く
引数>
width:
depth:
height:
戻り値>
void
-------------------------------------------------------------------------------*/
void drawBox(double width, double depth, double height) {

    glBegin(GL_QUADS);
    //前
    glNormal3d(0.0, 0.0, -1.0); //法線方向指定
    glVertex3d(width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, depth / 2);
    glVertex3d(width / 2, -height / 2, depth / 2);

    //左
    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d(width / 2, height / 2, depth / 2);
    glVertex3d(width / 2, height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, depth / 2);

    //右
    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(-width / 2, height / 2, -depth / 2);
    glVertex3d(-width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, -depth / 2);

    //後
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(width / 2, height / 2, -depth / 2);
    glVertex3d(-width / 2, height / 2, -depth / 2);
    glVertex3d(-width / 2, -height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, -depth / 2);

    //上
    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d(width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, height / 2, -depth / 2);
    glVertex3d(width / 2, height / 2, -depth / 2);

    //下
    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(width / 2, -height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, -depth / 2);
    glEnd();


}
/*------------------------------------------------------------------------------
名前：drawBoxFlame
説明：箱の枠を描く
引数>
width:
depth:
height:
戻り値>
void
-------------------------------------------------------------------------------*/
void drawBoxFlame(double width, double depth, double height) {

    glBegin(GL_LINE_LOOP);

    //前
    glNormal3d(0.0, 0.0, -1.0); //法線方向指定
    glVertex3d(width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, depth / 2);
    glVertex3d(width / 2, -height / 2, depth / 2);

    glEnd();
    glBegin(GL_LINE_LOOP);

    //左
    glVertex3d(width / 2, height / 2, depth / 2);
    glVertex3d(width / 2, height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, depth / 2);

    glEnd();
    glBegin(GL_LINE_LOOP);

    //右
    glVertex3d(-width / 2, height / 2, -depth / 2);
    glVertex3d(-width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, -depth / 2);

    glEnd();
    glBegin(GL_LINE_LOOP);

    //後
    glVertex3d(width / 2, height / 2, -depth / 2);
    glVertex3d(-width / 2, height / 2, -depth / 2);
    glVertex3d(-width / 2, -height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, -depth / 2);

    glEnd();
    glBegin(GL_LINE_LOOP);

    //上
    glVertex3d(width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, height / 2, depth / 2);
    glVertex3d(-width / 2, height / 2, -depth / 2);
    glVertex3d(width / 2, height / 2, -depth / 2);

    glEnd();
    glBegin(GL_LINE_LOOP);

    //下
    glVertex3d(width / 2, -height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, depth / 2);
    glVertex3d(-width / 2, -height / 2, -depth / 2);
    glVertex3d(width / 2, -height / 2, -depth / 2);

    glEnd();


}
/*------------------------------------------------------------------------------
名前：drawSphere
説明：球の描画
引数>
sphere_size:球の半径[m]
戻り値>
void
-------------------------------------------------------------------------------*/
void drawSphere(double sphere_size) {

    glutSolidSphere(sphere_size, 10, 5);

}

/*------------------------------------------------------------------------------
名前：drawCircle
説明：3次元空間のx-y平面に原点(0,0,0)を中心に半径radiusの円を描く
引数>
radius：半径[m]
div: 細かさ（分割数）
戻り値>
void
-------------------------------------------------------------------------------*/
void drawCircle(const float radius, const int div) {

    float x, y;
    float dth = 2 * M_PI / 72;

    glBegin(GL_POLYGON);
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        y = radius * sin(th);
        glVertex3f(x, y, 0.0);          //描画したい円周上の頂点を並べる
    }
    glEnd();

}

/*------------------------------------------------------------------------------
名前：drawCircleEdge
説明：3次元空間のx-y平面に原点(0,0,0)を中心に半径radiusの円の円周を描く
引数>
radius：半径[m]
div: 細かさ（分割数）
戻り値>
void
-------------------------------------------------------------------------------*/
void drawCircleEdge(const float radius, const int div) {

    float x, y;
    float dth = 2 * M_PI / 72;

    glBegin(GL_LINE_LOOP);
    for (float th = 0.0;  th <= 2 * M_PI;  th += dth) {
        x = radius * cos(th);
        y = radius * sin(th);
        glVertex3f(x, y, 0.0);          //描画したい円周上の頂点を並べる
    }
    glEnd();
}



/*------------------------------------------------------------------------------
名前：drawGrandAxis
説明：座標軸の描画
引数>
void
戻り値>
void
-------------------------------------------------------------------------------*/
void drawGrandAxis(void) {

    // X軸の描画 --------------------------------
    // 描画を赤色に設定
    glColor4f(1.0, 0.0, 0.0, 1.0);
    // 軸の描画
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1000.0, 0.0, 0.0);
    glEnd();

    // Y軸の描画 --------------------------------
    // 描画を緑色に設定
    glColor4f(0.0, 1.0, 0.0, 1.0);
    // 軸の描画
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1000.0, 0.0);
    glEnd();

    // Z軸の描画 --------------------------------
    // 描画を青色に設定
    glColor4f(0.0, 0.0, 1.0, 1.0);
    // 軸の描画
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1000.0);
    glEnd();
}


/*------------------------------------------------------------------------------
名前：drawGrand
説明：地面のグリッド描画
補足：width,depthは偶数であること
引数>
width:横方向（y軸方向）の長さ[m]
depth:縦方向（x軸方向）の長さ[m]
initerval:一マスの長さ[m]
戻り値>
void
-------------------------------------------------------------------------------*/
void drawGrand(int width, int depth, int interval) {


    // 幅，高さの大きさを描画に使うため半分にする
    width = (int)((double)width / 2.0);
    depth = (int)((double)depth / 2.0);

    glBegin(GL_LINES);          // 地面のグリッド

    // y軸と平行なグリッド線を引く(x軸正方向)
    for (int i = 0; i <= width; i += interval) {
        glVertex3d((double)i, (double) - depth, 0.0);
        glVertex3d((double)i, (double)depth, 0.0);
    }

    // y軸と平行なグリッド線を引く(x軸負方向)
    for (int i = -interval; i >= -width; i -= interval) {
        glVertex3d((double)i, (double) - depth, 0.0);
        glVertex3d((double)i, (double)depth, 0.0);
    }

    // x軸と平行なグリッド線を引く(y軸正方向)
    for (int j = 0; j <= depth; j += interval) {
        glVertex3d((double) - width, (double)j, 0.0);
        glVertex3d((double)width, (double)j, 0.0);
    }

    // x軸と平行なグリッド線を引く(y軸負方向)
    for (int j = -interval; j >= -depth; j -= interval) {
        glVertex3f((double) - width, (double)j, 0.0);
        glVertex3f((double)width, (double)j, 0.0);
    }

    glEnd();

}

/*------------------------------------------------------------------------------
名前：draw_grand2
説明：移動体の周りだけ地面のグリッド描画（移動体から遠ざかるほど薄くなる）
引数>
width:横方向（y軸方向）の長さ[m]
depth:縦方向（x軸方向）の長さ[m]
initerval:一マスの長さ[m]
power:透明の成り具合
戻り値>
void
-------------------------------------------------------------------------------*/
void drawGrand2(double width, double depth, double interval, double power) {

    // 幅，高さの大きさを描画に使うため半分にする
    width = width / 2.0;
    depth = depth / 2.0;

    //透明処理の設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_LINES);// 地面のグリッド

    for (double cnt = -depth ; cnt <= depth ; cnt += interval) {
        //x正方向
        for (double cnt2 = 0.0 ; cnt2 <= width ; cnt2 += interval) {
            glColor4f(1.0, 1.0, 1.0, 1.0 - pow(abs(cnt / width), power) - pow(abs(cnt2 / width), power));
            glVertex3d(cnt2, cnt, 0.0);
            glVertex3d(cnt2 + interval, cnt, 0.0);
        }

        //x負方向
        for (double cnt2 = 0.0 ; cnt2 >= -width ; cnt2 -= interval) {
            glColor4f(1.0, 1.0, 1.0, 1.0 - pow(abs(cnt / width), power) - pow(abs(cnt2 / width), power));
            glVertex3d(cnt2, cnt, 0.0);
            glVertex3d(cnt2 - interval, cnt, 0.0);
        }
    }

    for (double cnt = -width ; cnt <= width ; cnt += interval) {
        //y正方向
        for (double cnt2 = 0.0 ; cnt2 <= depth ; cnt2 += interval) {
            glColor4f(1.0, 1.0, 1.0, 1.0 - pow(abs(cnt / depth), power) - pow(abs(cnt2 / depth), power));
            glVertex3d(cnt, cnt2 , 0.0);
            glVertex3d(cnt, cnt2 + interval , 0.0);
        }

        //y負方向
        for (double cnt2 = 0.0 ; cnt2 >= -depth ; cnt2 -= interval) {
            glColor4f(1.0, 1.0, 1.0, 1.0 - pow(abs(cnt / depth), power) - pow(abs(cnt2 / depth), power));
            glVertex3d(cnt, cnt2, 0.0);
            glVertex3d(cnt, cnt2 - interval, 0.0);
        }
    }


    glEnd();

    glDisable(GL_BLEND);

}


/*------------------------------------------------------------------------------
名前：draw_grand_radial
説明：自機からのxy平面上の放射状の線
補足：デフォルトでは原点周りのxy平面上なので，3次元空間に描くときは，まず並進，回転行列でモデルビューを変更してから，この円を描く
引数>
radius:表示する半径[m]
degree:放射状線間の角度[degree]
location：自機の位置姿勢情報
戻り値>
void
-------------------------------------------------------------------------------*/
void drawGrandRadial(double radius , double degree) {
    //  glLineWidth(1.0);   //線幅の決定

    for (double theta = 0.0 ; theta <= 2.0 * M_PI ; theta += (degree * DEG2RAD)) {
        glBegin(GL_LINES);
        glVertex3f(0.0 , 0.0 , 0.0);        //スタート座標
        glVertex3f(0.0 + radius * cos(theta) , 0.0 + radius * sin(theta) , 0.0); //ゴール座標
        glEnd();
    }
}

/*------------------------------------------------------------------------------
名前：drawGrandEqualDistance
説明：自機からのxy平面上の等距離線
補足：デフォルトでは原点周りのxy平面上なので，3次元空間に描くときは，まず並進，回転行列でモデルビューを変更してから，この円を描く
引数>
radius:表示する半径[m]
initerval:各線間の長さ[m]
戻り値>
void
-------------------------------------------------------------------------------*/
void drawGrandEqualDistance(double radius , double interval) {
    //  glLineWidth(1.0);   //線幅の決定

    double delta = 5 * DEG2RAD;
    for (double r = interval ; r <= radius ; r += interval) {
        glBegin(GL_LINE_LOOP);
        for (double theta = 0.0 ; theta <= 2.0 * M_PI ; theta += delta) {
            glVertex3f(0.0 + r * cos(theta) , 0.0 + r * sin(theta) , 0.0); //繋げたい点を並べる
        }
        glEnd();
    }
}

/*------------------------------------------------------------------------------
名前：drawLine
説明：直線の描画
補足：
引数>
startPoint[3]:始点
endPoint[3]:終点
戻り値>
void
-------------------------------------------------------------------------------*/
void drawLine(double startPoint[], double endPoint[]) {

    glBegin(GL_LINES);
    glVertex3f(startPoint[0], startPoint[1], startPoint[2]);
    glVertex3f(endPoint[0], endPoint[1], endPoint[2]);
    glEnd();


}


/************************************************************
名前： drawEye
説明： 視点位置の描画
引数>
width：画面の横[pixel]
heigh:画面の縦[pixel]
戻り値>
void
**************************************************************/
void drawEye(float width, float height) {

    // アスペクト比の計算
    float aspect = (float) width / (float) height;

    glPushMatrix();             // 座標系の保存




    //原点の描画******************************************************
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3d(0.0, 0.0, 0.0);
    glEnd();

    //四角錐部の描画***************************************************
    glBegin(GL_LINES);

    //四角錐の側面の4辺
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.17320503, 0.100, -0.075);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.17320503, 0.100, 0.075);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.17320503, -0.100, 0.075);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.17320503, -0.100, -0.075);

    //四角錐の底面の4辺
    glVertex3d(0.17320503, 0.100, -0.075);
    glVertex3d(0.17320503, 0.100, 0.075);

    glVertex3d(0.17320503, 0.100, 0.075);
    glVertex3d(0.17320503, -0.100, 0.075);

    glVertex3d(0.17320503, -0.100, 0.075);
    glVertex3d(0.17320503, -0.100, -0.075);

    glVertex3d(0.17320503, -0.100, -0.075);
    glVertex3d(0.17320503, 0.100, -0.075);
    glEnd();

    glPopMatrix();              // 座標系の復元

}
/************************************************************
名前： drawEye2
説明： 視点位置の描画
        distance=0.17320503(?)

引数>
width：画面の横[pixel]
heigh:画面の縦[pixel]
戻り値>
void
**************************************************************/
void drawEye2(double width, double height, double h_gakaku) {

    // アスペクト比の計算
    double aspect = width / height;
    double distance = (width) / tan(h_gakaku * DEG2RAD);
    double y, z;

    y = 0.1;
    z = y / aspect;

    glPushMatrix();             // 座標系の保存

    // 線の太さと色を設定
    glLineWidth(3.0);
    glColor3f(1.0, 0.0, 0.0);


    //原点の描画******************************************************
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3d(0.0, 0.0, 0.0);
    glEnd();


    //四角錐部の描画***************************************************
    glBegin(GL_LINES);

    //四角錐の側面の4辺
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(distance, y, -z);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(distance, y, z);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(distance, -y, z);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(distance, -y, -z);

    //四角錐の底面の4辺
    glVertex3d(distance, y, -z);
    glVertex3d(distance, y, z);

    glVertex3d(distance, y, z);
    glVertex3d(distance, -y, z);

    glVertex3d(distance, -y, z);
    glVertex3d(distance, -y, -z);

    glVertex3d(distance, -y, -z);
    glVertex3d(distance, y, -z);
    glEnd();

    glPopMatrix();              // 座標系の復元

}

/*-----------------------------------------------------------
名前:drawObstacle
説明:障害物の描画
引数＞
void    //URG or Kinectの3次元データになる予定
戻り値＞
void

とりあえずここに書いておく

------------------------------------------------------------*/
void drawObstacle(void) {

    glColor4f(1.0f, 0.0f, 1.0f, 1.0f);  //描画色の設定

    glEnable(GL_BLEND);         //ブレンドの有効化
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//ブレンドの仕方

    double height = 0.1;
    double inf_r = 100.0;

    //実験用
    //仮想的なURGからのデータ
    //ケース1:生データのコピペ
    double dataset1[181] = {3.495, 3.495, 3.586, 3.571, 3.592, 3.999, 3.916, 2.834, 2.753, 2.777, 2.576, 2.508, 2.438, 2.378, 2.302, 2.256, 2.194, 2.143, 2.099, 2.046, 2.003, 3.963, 3.929, 3.929, 3.929, 3.932, 3.941, 3.942, 3.943, 3.945, 3.951, 3.956, 3.966, 3.968, 3.97, 3.975, 3.978, 3.983, 3.985, 3.994, 3.001, 3.002, 3.012, 2.02, 2.021, 2.026, 2.03, 2.047, 2.057, 2.057, 2.061, 2.064, 2.076, 2.081, 2.092, 2.098, 2.106, 2.113, 2.117, 2.129, 2.131, 2.139, 2.142, 2.16, 2.167, 2.17, 2.187, 2.188, 2.2, 2.215, 2.225, 2.226, 2.23, 2.254, 2.255, 2.263, 2.285, 2.295, 2.301, 2.317, 2.337, 2.347, 2.36, 2.362, 2.38, 2.389, 2.404, 2.411, 2.422, 2.439, 2.439, 2.456, 2.463, 2.471, 3.243, 3.232, 3.231, 3.229, 3.207, 3.196, 3.195, 3.192, 3.192, 3.176, 3.176, 3.176, 3.219, 3.238, 3.282, 3.306, 3.337, 3.373, 3.397, 3.435, 3.468, 3.513, 3.553, 3.583, 3.637, 3.657, 3.698, 3.745, 3.757, 3.757, 3.757, 3.749, 3.74, 3.729, 3.717, 3.709, 3.691, 3.688, 3.677, 3.656, 3.653, 3.64, 3.634, 3.63, 3.613, 3.601, 3.588, 3.584, 3.572, 3.561, 3.556, 3.554, 3.542, 3.533, 3.526, 3.519, 3.516, 3.496, 3.493, 3.483, 3.477, 3.469, 3.468, 3.461, 3.46, 3.46, 3.441, 3.431, 3.43, 3.422, 3.42, 3.417, 3.41, 3.408, 3.408, 3.408, 3.436, 3.443, 3.472, 3.48, 3.404, 3.403, 3.403, 3.404, 3.438, 3.451, 3.481};
    for (int i = 0; i <= 180; i++) {
        dataset1[i] = dataset1[i] * 18;   //スケール合わせ
    }

    glPushMatrix();
    glRotated(-90.0, 0.0, 0.0, 1.0);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0 ; i <= 180 ; i++) {
        glVertex3d(dataset1[i]*cos(i * DEG2RAD) , dataset1[i]*sin(i * DEG2RAD) , -height); //仮想Urgデータ1
        glVertex3d(dataset1[i]*cos(i * DEG2RAD) , dataset1[i]*sin(i * DEG2RAD) , height);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (int i = 0 ; i <= 180 ; i++) {
        glVertex3d(dataset1[i]*cos(i * DEG2RAD) , dataset1[i]*sin(i * DEG2RAD) , -height); //仮想Urgデータ1
        glVertex3d(dataset1[i]*cos(i * DEG2RAD) , dataset1[i]*sin(i * DEG2RAD) , height);
        glVertex3d(inf_r * cos(i * DEG2RAD) , inf_r * sin(i * DEG2RAD) , -height);
        glVertex3d(inf_r * cos(i * DEG2RAD) , inf_r * sin(i * DEG2RAD) , height);
    }
    glEnd();

    glPopMatrix();

    //ケース2:勝手に空間を作る

    //double width = 10;//空間の幅
    //glPushMatrix();
    //glRotated(-90.0, 0.0, 0.0, 1.0);
    //glBegin(GL_QUAD_STRIP);//
    //for(int i=0 ; i<45 ; i++ ){
    //  glVertex3d(abs(width/cos(i*DEG2RAD))*cos(i*DEG2RAD) , abs(width/cos(i*DEG2RAD))*sin(i*DEG2RAD) , -height);  //仮想Urgデータ1
    //  glVertex3d(abs(width/cos(i*DEG2RAD))*cos(i*DEG2RAD) , abs(width/cos(i*DEG2RAD))*sin(i*DEG2RAD) , height);
    //}

    //for(int i=45 ; i<=75 ; i++ ){
    //  glVertex3d(abs(width*0.8/(sin(i*DEG2RAD)+0.5*cos(i*DEG2RAD)))*cos(i*DEG2RAD) , abs(width*0.8/(sin(i*DEG2RAD)+0.5*cos(i*DEG2RAD)))*sin(i*DEG2RAD) , -height);    //仮想Urgデータ1
    //  glVertex3d(abs(width*0.8/(sin(i*DEG2RAD)+0.5*cos(i*DEG2RAD)))*cos(i*DEG2RAD) , abs(width*0.8/(sin(i*DEG2RAD)+0.5*cos(i*DEG2RAD)))*sin(i*DEG2RAD) , height);
    //}

    //for(int i=60 ; i<=135 ; i++ ){
    //  glVertex3d(abs(width/sin(i*DEG2RAD))*cos(i*DEG2RAD) , abs(width/sin(i*DEG2RAD))*sin(i*DEG2RAD) , -height);  //仮想Urgデータ1
    //  glVertex3d(abs(width/sin(i*DEG2RAD))*cos(i*DEG2RAD) , abs(width/sin(i*DEG2RAD))*sin(i*DEG2RAD) , height);
    //}

    //for(int i=135 ; i<=180 ; i++ ){
    //  glVertex3d(abs(width/cos(i*DEG2RAD))*cos(i*DEG2RAD) , abs(width/cos(i*DEG2RAD))*sin(i*DEG2RAD) , -height);  //仮想Urgデータ1
    //  glVertex3d(abs(width/cos(i*DEG2RAD))*cos(i*DEG2RAD) , abs(width/cos(i*DEG2RAD))*sin(i*DEG2RAD) , height);
    //}
    //glEnd();
    //glPopMatrix();


    glDisable(GL_BLEND);

}


/*-----------------------------------------------------------
名前:drawRailroadLine
説明:機体CGの前に延長線を描画する
引数＞
void
戻り値＞
void
------------------------------------------------------------*/
void drawRailroadLine(double modelSize[3], double startPos[3], double endPos[3]) {

    double model_x = modelSize[0];  //D:奥行き
    double model_y = modelSize[1];//W:幅
    double model_z = modelSize[2];  //H:高さ

    double spos_x = startPos[0];
    double spos_y = startPos[1];
    double spos_z = startPos[2];

    double epos_x = endPos[0];
    double epos_y = endPos[1];
    double epos_z = endPos[2];


    glPushMatrix();
    glTranslated(spos_x, spos_y, spos_z);

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , model_y / 2 , model_z / 2); //左上
    glVertex3d(model_x / 2 + epos_x , model_y / 2 + epos_y , model_z / 2 + epos_z);

    glEnd();

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , model_y / 2 , -model_z / 2); //左下
    glVertex3d(model_x / 2 + epos_x , model_y / 2 + epos_y, -model_z / 2 + epos_z);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , -model_y / 2 , model_z / 2); //右上
    glVertex3d(model_x / 2 + epos_x , -model_y / 2 + epos_y, model_z / 2 + epos_z);
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , -model_y / 2 , -model_z / 2); //右下
    glVertex3d(model_x / 2 + epos_x , -model_y / 2 + epos_y, -model_z / 2 + epos_z);
    glEnd();

    glPopMatrix();



}

/*-----------------------------------------------------------
名前:drawRailroadLine
説明:機体CGの前に延長線を描画する
引数＞
void
戻り値＞
void
------------------------------------------------------------*/
void drawRailroadLine(double modelSize[3], double aspect, double h_gakaku, double startPos[3], double endPos[3]) {


    double model_x = modelSize[0];  //D:奥行き
    double model_y = modelSize[1];//W:幅
    double model_z = modelSize[2];  //H:高さ

    double v_gakaku = 2.0 * atan(abs(tan(h_gakaku * DEG2RAD / 2.0)) * (1 / aspect)) * RAD2DEG;

    double spos_x = startPos[0];
    double spos_y = startPos[1];
    double spos_z = startPos[2];

    double epos_x = endPos[0];
    double epos_y = endPos[1];
    double epos_z = endPos[2];

    double inf = 1000.0;

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , model_y / 2 , model_z / 2); //左上
    glVertex3d(model_x / 2 + inf , model_y / 2 + inf * tan(h_gakaku * DEG2RAD / 2) , model_z / 2 + inf * tan(v_gakaku * DEG2RAD / 2));

    glEnd();

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , model_y / 2 , -model_z / 2); //左下
    glVertex3d(model_x / 2 + inf , model_y / 2  + inf * tan(h_gakaku * DEG2RAD / 2) , -model_z / 2 -  inf * tan(v_gakaku * DEG2RAD / 2));
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , -model_y / 2 , model_z / 2); //右上
    glVertex3d(model_x / 2 + inf , -model_y / 2  - inf * tan(h_gakaku * DEG2RAD / 2) , model_z / 2  + inf * tan(v_gakaku * DEG2RAD / 2));
    glEnd();

    glBegin(GL_LINES);
    glVertex3d(model_x / 2 , -model_y / 2 , -model_z / 2); //右下
    glVertex3d(model_x / 2 + inf , -model_y / 2 - inf * tan(h_gakaku * DEG2RAD / 2), -model_z / 2  - inf * tan(v_gakaku * DEG2RAD / 2));
    glEnd();

    glPopMatrix();



}
/*-----------------------------------------------------------
名前:drawString
説明:英数字の描画
        日本語は不可．書くとスルーされる←要修正！！
        文字の向きは視点に対してちゃんと見えるようになる
            （逆に，文字をナナメから見ることはできない）
引数＞
global_x,global_y,global_z: 現在の3次元空間座標系における描画位置
screen_x,screen_y:  画面中心原点の2次元画面座標系における描画位置（横がx，上がy）
width, height: 描画ウィンドウのサイズ
str: 描画する英数字

戻り値＞
void
参考:http://d.hatena.ne.jp/osyo-manga/20110827/1314417606
------------------------------------------------------------*/
void drawString(double global_x, double global_y, double global_z, std::string const& str) {
    glRasterPos3f(global_x, global_y, global_z);
    int strsize = str.length() + 1;
    char* ch = new char[strsize];
	strcpy_s(ch, strsize, str.c_str());


    for (int i = 0; i < strsize; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[i]);
    };
}

void drawString(double screen_x, double screen_y, double width, double height, std::string const& str) {

    glPushMatrix(); //各種行列を退避
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION); //2Dの並行投影を設定
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glRasterPos3f(screen_x + width / 2, screen_y + height / 2, 0.0);
    int strsize = str.length() + 1;
    char* ch = new char[strsize];
	strcpy_s(ch, strsize, str.c_str());

    for (int i = 0; i < strsize; i++) {
        //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch[i]);
        glutStrokeCharacter(GLUT_STROKE_ROMAN , ch[i]);
    };
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();//もとの状態にもどる

}

/*-----------------------------------------------------------
名前:drawStringBig
説明:大きい英数字の描画
        日本語は不可．書くとスルーされる←要修正！！
        文字の向きは視点に対してちゃんと見えるようになる
            （逆に，文字をナナメから見ることはできない）
引数＞
global_x,global_y,global_z: 現在の3次元空間座標系における描画位置
screen_x,screen_y:  画面中心原点の2次元画面座標系における描画位置（横がx，上がy）
width, height: 描画ウィンドウのサイズ
str: 描画する英数字

戻り値＞
void
参考:http://d.hatena.ne.jp/osyo-manga/20110827/1314417606
------------------------------------------------------------*/
void drawStringBig(double screen_x, double screen_y, double width, double height, std::string const& str) {

    glPushMatrix(); //各種行列を退避


    glLoadIdentity();
    glMatrixMode(GL_PROJECTION); //2Dの並行投影を設定
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);


    glTranslated(screen_x + width / 2, screen_y + height / 2, 0.0);
    glScaled(0.2, 0.2, 0.2);
    int strsize = str.length() + 1;
    char* ch = new char[strsize];
	strcpy_s(ch, strsize, str.c_str());

    for (int i = 0; i < strsize; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , ch[i]);
    };
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();//もとの状態にもどる

}

/*-----------------------------------------------------------
名前:drawLayerOnScreen
説明:画面いっぱいに薄い赤のレイヤーを表示
引数＞
width:  画面横幅
height: 画面縦幅
alpha:  透明度
戻り値＞
void
------------------------------------------------------------*/
void drawLayerOnScreen(double width, double height, double alpha) {

    glPushMatrix(); //各種行列を退避


    glLoadIdentity();
    glMatrixMode(GL_PROJECTION); //2Dの並行投影を設定

    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glEnable(GL_BLEND);         //ブレンドの有効化
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//ブレンドの仕方
    glColor4d(1.0, 0.0, 0.0, alpha);    //描画色の設定

    glBegin(GL_QUADS);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(width, 0.0, 0.0);
    glVertex3d(width, height, 0);
    glVertex3d(0.0, height, 0);
    glEnd();

    glDisable(GL_BLEND);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();//もとの状態にもどる

}
