#ifndef PLAYER_H  //インクルードガード
#define PLAYER_H

class Player
{
public:
    //プレイヤー座標
    float x = 0.0f;
    float z = 0.0f;

    //前進するときの移動速度
    float forwardSpeed = 10.0f;
    //左右するときの移動速度
    float sideSpeed = 10.0f;

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