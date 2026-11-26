#ifndef OBSTACLE_MANAGER_H_
#define OBSTACLE_MANAGER_H_

#include <vector>
#include "Obstacle.h"

#include <vector>
#include "Obstacle.h"

class ObstacleManager
{
private:
    // “Ç‚İ‚ñ‚¾áŠQ•¨ƒ‚ƒfƒ‹‚Ìƒnƒ“ƒhƒ‹
    int modelHandle = -1;

    // ŠÇ—‚µ‚Ä‚¢‚éáŠQ•¨‚Ì”z—ñ
    std::vector<Obstacle> obstacles;

public:

    // áŠQ•¨ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
    bool LoadModel();

    // áŠQ•¨ƒ‚ƒfƒ‹‚Ì‰ğ•ú
    void ReleaseModel();

    // áŠQ•¨‚ğ¶¬
    void SpawnObstacle(
        float courseZ,
        const CourseManager& courseManager
    );

    // ƒvƒŒƒCƒ„[‚ÌŒã•û‚É‚ ‚éáŠQ•¨‚ğíœ
    void RemovePassedObstacles(float playerZ);

    // áŠQ•¨‚ğ‰Šúó‘Ô‚É–ß‚·
    void Reset();

    // áŠQ•¨‚ÌŒÂ”‚ğæ“¾
    int GetCount() const;

    // w’è‚µ‚½”Ô†‚ÌáŠQ•¨‚ğæ“¾
    Obstacle& GetObstacle(int index);

    // áŠQ•¨‚ğ‚·‚×‚Ä•`‰æ
    void DrawAll();
};

#endif
