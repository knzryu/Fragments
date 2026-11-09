#include "DxLib.h"
#include "Bullet.h"

//弾移動処理
void Bullet::Update(float deltaTime,const CourseManager& courseManager)
{
    if(!active)
    {
        return;
	}
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

//描画処理
void Bullet::Draw()
{
    //弾が無効なら、ここで関数を終了する
    if (!active)
    {
        return;
    }

    //立方体の弾
    DrawCube3D(
        VGet(x - 0.15f, y - 0.15f, z - 0.15f),
        VGet(x + 0.15f, y + 0.15f, z + 0.15f),
        GetColor(255, 255, 0),
        GetColor(255, 255, 0),
        TRUE
    );
}