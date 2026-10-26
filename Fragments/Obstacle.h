#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>
//障害物管理class
class Obstacle
{
public:
    //障害物のX座標の最小値・最大値
    float minX, maxX;
    //y
    float minY, maxY;
    //z
    float minZ, maxZ;

    //描画関数中身はｃｐｐ
    void Draw();

};
#endif