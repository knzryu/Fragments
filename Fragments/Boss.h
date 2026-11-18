#pragma once

class Boss
{
private:

	//
	int hp = 50;

	bool active = false;

	//座標
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

public:
    // 出現
    void Spawn(float startX, float startY, float startZ);

    // ダメージを受ける
    void TakeDamage(int damage);

    // 撃破判定
    bool IsDead() const;

    // 出現状態を取得
    bool IsActive() const;

    // HPを取得
    int GetHP() const;

};
