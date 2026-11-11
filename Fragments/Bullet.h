#ifndef BULLET_H
#define BULLET_H

#include "CourseManager.h"

class Bullet
{
public:
    //弾の座標
    float x = 0.0f;
    float y = 1.0f;
    float z = 0.0f;

	//コース上の位置
	float courseZ = 0.0f;
	float laneOffset = 0.0f;


    //弾がZ方向に進む速さ
    float speed = 80.0f;

    //弾が有効かどうかbool型
    //true：有効、false：無効
    bool active = true;

    //

    //弾を移動させる関数の宣言
    void Update(float deltaTime,const CourseManager & coursManager);

    // コース上の位置からワールド座標を計算
    void UpdatePosition(const CourseManager& courseManager);

    //描画
    void Draw();

private:
    
};

#endif