#ifndef PLAYER_H  //インクルードガード
#define PLAYER_H
#include "CourseManager.h"

class Player
{
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

    // 横方向オフセットの取得・設定
    float GetLaneOffset() const;
    void SetLaneOffset(float newOffset);

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


    // コース上の位置を更新する
    void UpdateCoursePosition(const CourseManager& courseManager);

    // コース上の進行位置を取得する
    float GetCourseZ() const;

    //deltaTimeに応じて前進する関数の宣言
    //deltaTimeを掛けて１フレームで移動する距離を[経過時間]に応じて計算するため
    // コースに沿った移動
    void MoveForward(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveRight(float deltaTime);


    //前進速度を設定する関数の宣言
    void SetForwardSpeed(float newSpeed);
    void SetSideSpeed(float newSpeed);


    //プレイヤー描画関数、宣言だけ中身の処理は.cppに
    void Draw();

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

    //------------</初期ステータス/>-----------------

        //移動速度   constexprで初期値を定数にする
    static constexpr float INITIAL_FORWARD_SPEED = 60.0f;
    //左右
    static constexpr float INITIAL_SIDE_SPEED = 5.0;

    float forwardSpeed = INITIAL_FORWARD_SPEED;
    float sideSpeed = INITIAL_SIDE_SPEED;

    //プレイヤーの能力値
    int attackPower = 1;
    float bulletSpeed = 30.0f;


    // コース上の進行位置
    float courseZ = 0.0f;

    // コースの進行方向
    float directionX = 0.0f;
    float directionZ = 1.0f;

    // コース中心からの横方向の距離
    float laneOffset = 0.0f;

};

#endif