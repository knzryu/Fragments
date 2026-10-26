#include "Collision.h"
#include   "DxLib.h"

//二つの直方体が衝突しているかを判定する
bool IsCollidingAABB(
    float playerMinX, float playerMaxX,
    float playerMinY, float playerMaxY,
    float playerMinZ, float playerMaxZ,
    float obstacleMinX, float obstacleMaxX,
    float obstacleMinY, float obstacleMaxY,
    float obstacleMinZ, float obstacleMaxZ)
{
    // X軸方向で２つの箱が重なっているか
    bool overlapX =
        playerMinX <= obstacleMaxX && // <= true　か判定　&&だから[かつ]両方とも
        playerMaxX >= obstacleMinX;

    // Y軸で重なっているか
    bool overlapY =
        playerMinY <= obstacleMaxY &&
        playerMaxY >= obstacleMinY;

    // Z軸で重なっているか
    bool overlapZ =
        playerMinZ <= obstacleMaxZ &&
        playerMaxZ >= obstacleMinZ;

    //３方向全てで重なっているなら衝突している
    return overlapX && overlapY && overlapZ;
}