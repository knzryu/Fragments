#pragma once

class Boss
{
private:

	
	int hp = 50;

	bool active = false;

	//座標
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

    // 中ボスのモデルハンドル
    int modelHandle = -1;

    // 待機アニメーションのモデルハンドル
    int idleAnimModelHandle = -1;

    // 待機アニメーションの接続番号
    int idleAttachIndex = -1;

public:

    // モデルとアニメーションの読み込み
    bool LoadModel();

    // モデルとアニメーションの解放
    void ReleaseModel();

    // 中ボスの更新
    void Update(float deltaTime);

    // 中ボスの描画
    void Draw();


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
