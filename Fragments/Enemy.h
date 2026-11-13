#ifndef ENEMY_H
#define ENEMY_H

#include "CourseManager.h"

class Enemy
{
private:
    //HPはEnemy自身が管理
    int hp = 3;

public:
    // ワールド座標
    float x = 0.0f;
    float y = 0.0f;
    float z = 20.0f;

    // コース上の位置
    float courseZ = 20.0f;
    float laneOffset = 0.0f;

    void Draw();

    // 敵の初期化
    void Initialize(float startX, float startY, float startZ);

    // コース情報からワールド座標を更新
    void UpdatePosition(const CourseManager& courseManager);


    //HPを取得
    int GetHP() const;
    //敵が倒されているか確認
    bool IsDead() const;
    // 受けたダメージ分だけHPを減らす
    void TakeDamage(int damage);
};
#endif