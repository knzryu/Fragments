#include "DxLib.h"
#include "Bullet.h"

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
        VGet(x - size, y - size, z - size),
        VGet(x + size, y + size, z + size),
        GetColor(0, 255, 255),
        GetColor(0, 255, 255),
        TRUE
    );
}