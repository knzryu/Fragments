#include "DxLib.h"
#include "Obstacle.h"



//è·äQï®ÇÃï`âÊä÷êî
void Obstacle::Draw()
{
    //SetUseLighting(FALSE);

    DrawCube3D(
        VGet(minX, minY, minZ),
        VGet(maxX, maxY, maxZ),
        GetColor(0, 255, 0),
        GetColor(0, 255, 0),
        TRUE
    );
}
