#include "DxLib.h"
#include "Bullet.h"
#include <cmath>

//弾移動処理
void Bullet::Update(float deltaTime,const CourseManager& courseManager)
{
    if(!active)
    {
        return;
	}

    // 移動前の位置を保存
    previousCourseZ = courseZ;

	//弾のコース上の位置を更新する
    courseZ += speed * deltaTime;

    // ワールド座標を更新
    UpdatePosition(courseManager);

    
}

void Bullet::UpdatePosition(const CourseManager& courseManager)
{
    CoursePoint center = courseManager.GetPosition(courseZ);
    CoursePoint right = courseManager.GetRight(courseZ);

    x = center.x + right.x * laneOffset;
    z = center.z + right.z * laneOffset;
}

// 描画処理
void Bullet::Draw()
{
    // 弾が無効なら描画しない
    if (!active)
    {
        return;
    }

    // 青白いエネルギー弾
    DrawSphere3D(
        VGet(x, y, z),           // 球の中心座標
        size,                    // 弾の半径
        16,                      // 球の分割数
        GetColor(30, 180, 255),  // 青色の本体
        GetColor(220, 250, 255), // 白に近い反射色
        TRUE                     // 塗りつぶす
    );
}

