#ifndef PLAYER_H  //インクルードガード
#define PLAYER_H

class Player
{
private:

    //3Dモデル
    int modelHandle = -1;

    int attachIndex = -1;
    float animTime = 0.0f;
    float animTotalTime = 0.0f;
    float animSpeed = 0.03f;

    //プレイヤーの座標
	float x = 0.0f;
    float z = 0.0f;
    
    //移動速度
    float forwardSpeed = 10.0f;
    //左右
	float sideSpeed = 5.0f;

    //プレイヤーの能力値
    int attackPower = 1;
    float bulletSpeed = 30.0f;

public:

    //モデルの解放
    void Finalize();

    void UpdateAnimation(float deltaTime);

    //座標の取得
    float GetX() const;
	float GetZ() const;

	//座標の設定
	void SetX(float newX);
	void SetZ(float newZ);

    //攻撃力の取得・設定
    int GetAttackPower()const;
    void SetAttackPower(int newPower);

    //弾速の取得・設定
    float GetBulletSpeed()const;
    void SetBulletSpeed(float newSpeed);

    //初期化
	void Initialize();

    //リセット
    void Reset();


    //deltaTimeに応じて前進する関数の宣言
    //deltaTimeを掛けて１フレームで移動する距離を[経過時間]に応じて計算するため
    void MoveForward(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveRight(float deltaTime);

    //前進速度を設定する関数の宣言
    void SetForwardSpeed(float newSpeed);
    void SetSideSpeed(float newSpeed);


    //プレイヤー描画関数、宣言だけ中身の処理は.cppに
    void Draw();
};

#endif