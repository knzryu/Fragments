#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 20.0f;

    int hp = 3;

    void Draw();

    // “G‚Ì‰Šú‰»
    void Initialize(float startX, float startY, float startZ);


    //ó‚¯‚½ƒ_ƒ[ƒW•ª‚¾‚¯HP‚ğŒ¸‚ç‚·ŠÖ”‚ÌéŒ¾
    void TakeDamage(int damage);
};
#endif