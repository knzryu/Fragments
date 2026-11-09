#include "BulletManager.h"

// 弾の初期化 
void BulletManager::Initialize()
{
    Clear();
}

// 弾の生成
void BulletManager::SpawnBullet(float courseZ, float laneOffset, float y,const CourseManager& courseManager)
{
    Bullet bullet;

	bullet.courseZ = courseZ;
	bullet.laneOffset = laneOffset;
	bullet.y = y;

	bullet.UpdatePosition(courseManager);

    bullets.push_back(bullet);
}

// すべての弾を更新
void BulletManager::UpdateAll(float deltaTime,const CourseManager& courseManager)
{
    for (int i = 0; i < static_cast<int>(bullets.size()); i++)
    {
        bullets[i].Update(deltaTime, courseManager);
    }
}

// すべての弾を描画
void BulletManager::DrawAll()
{
    for (int i = 0; i < static_cast<int>(bullets.size()); i++)
    {
        bullets[i].Draw();
    }
}

// 弾の数を取得 
int BulletManager::GetCount() const
{
    return static_cast<int>(bullets.size());
}

// 弾を取得 
Bullet& BulletManager::GetBullet(int index)
{
    return bullets[index];
}

// 無効な弾を削除
void BulletManager::RemoveInactiveBullets()
{
    for (int i = static_cast<int>(bullets.size()) - 1; i >= 0; i--)
    {
        if (!bullets[i].active)
        {
            bullets.erase(bullets.begin() + i);
        }
    }
}

// 弾を全削除
void BulletManager::Clear()
{
    bullets.clear();
}