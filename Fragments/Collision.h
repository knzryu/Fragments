#ifndef COLLISION_H
#define COLLISION_H

//AABB方式２つの直方体が衝突しているかを判定する関数
//bool型、戻り値：衝突しているならtrue,していないならfalse
bool IsCollidingAABB(
    //プレイヤーのX・Y・Z座標の範囲
    float playerMinX, float playerMaxX,
    float playerMinY, float playerMaxY,
    float playerMinZ, float playerMaxZ,

    //障害物のX・Y・Z座標の範囲
    float obstacleMinX, float obstacleMaxX,
    float obstacleMinY, float obstacleMaxY,
    float obstacleMinZ, float obstacleMaxZ);

#endif