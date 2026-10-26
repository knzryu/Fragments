#include "DxLib.h"
#include "Obstacle.h"

void Obstacle::Draw()
{
    DrawCube3D(
        VGet(minX, minY, minZ),
        VGet(maxX, maxY, maxZ),
        GetColor(0, 255, 0),
        GetColor(0, 255, 0),
        TRUE
    );
}
