#ifndef BULLET_H
#define BULLET_H

class Bullet
{
public:
    //’e‚ÌÀ•W
    float x = 0.0f;
    float y = 1.0f;
    float z = 0.0f;

    //’e‚ªZ•ûŒü‚Éi‚Ş‘¬‚³
    float speed = 30.0f;

    //’e‚ª—LŒø‚©‚Ç‚¤‚©boolŒ^
    //trueF—LŒøAfalseF–³Œø
    bool active = true;

    //’e‚ğˆÚ“®‚³‚¹‚éŠÖ”‚ÌéŒ¾
    void Update(float deltaTime);

    //•`‰æ
    void Draw();
};

#endif