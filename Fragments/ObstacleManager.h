#ifndef OBSTACLE_MANAGER_H_
#define OBSTACLE_MANAGER_H_

#include <vector>
#include"Obstacle.h"

class ObstacleManager
{
public:
    void SpawnObstacle(float spawnZ);
    void RemovePassedObstacles(float playerZ);



private:
    std::vector<Obstacle> obstacles;
};

#endif 

