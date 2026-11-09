#ifndef BULLET_MANAGER_H 
#define BULLET_MANAGER_H

#include <vector>
#include "Bullet.h"
#include "CourseManager.h"

class BulletManager
{
private:
    // 弾を管理する配列
    std::vector<Bullet> bullets;

public:
    // 弾の初期化
    void Initialize();

    // 弾の生成
    void SpawnBullet(float courseZ, float laneOffset, float y,const CourseManager& courseManager);

    // すべての弾を更新
    void UpdateAll(float deltaTime,const CourseManager& courseManager);

    // すべての弾を描画
    void DrawAll();

    // 弾の数を取得
    int GetCount() const;

    // 弾を取得
    Bullet& GetBullet(int index);

    // 無効な弾を削除
    void RemoveInactiveBullets();

    // 弾を全削除
    void Clear();
};

#endif