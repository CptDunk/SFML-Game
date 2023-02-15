#pragma once
#include "resource.hpp"


/*
    Enemy Class
    -Should be checking player position relative to world space
        -If conditions met, spawn enemies randomly on a spawn rectangle
*/
class Enemy
{
    private:
    //Variables
        //enemy variables
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    sf::Vector2f enemyPosition;
    sf::Vector2f enemyVelocity;
    float enemyAcceleration;
    int flip = 1;

    void initEnemy(sf::FloatRect spawnBound);
    public:
    Enemy(sf::FloatRect spawnBound);
    ~Enemy();
    sf::FloatRect enemyBounds();
    int updateEnemies(float bgBoundXMin, float bgBoundXMax, float velocityY, float viewCenterY, sf::FloatRect playerBounds);
    void renderEnemies(sf::RenderWindow& window);

};