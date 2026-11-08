#ifndef OBSTACLE_MANAGER_H_
#define OBSTACLE_MANAGER_H_

#include <vector>
#include "Obstacle.h"

class ObstacleManager
{
public:
	//áŠQ•¨‚ğ¶¬‚·‚éŠÖ”‚ÌéŒ¾
    void SpawnObstacle(float courseZ, const CourseManager& courseManager);
	//ƒvƒŒƒCƒ„[‚ÌŒã•û‚É‚ ‚éáŠQ•¨‚ğíœ‚·‚éŠÖ”‚ÌéŒ¾
    void RemovePassedObstacles(float playerZ);
	//áŠQ•¨‚ğ‰Šúó‘Ô‚É–ß‚·ŠÖ”‚ÌéŒ¾
    void Reset();
	//áŠQ•¨‚ÌŒÂ”‚ğæ“¾‚·‚éŠÖ”‚ÌéŒ¾
    int GetCount() const;
	//w’è‚µ‚½”Ô†‚ÌáŠQ•¨‚ğæ“¾‚·‚éŠÖ”‚ÌéŒ¾
    Obstacle& GetObstacle(int index);
	//áŠQ•¨‚ğ‚·‚×‚Ä•`‰æ‚·‚éŠÖ”‚ÌéŒ¾
    void DrawAll();

private:
    std::vector<Obstacle> obstacles;
};

#endif
