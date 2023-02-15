#pragma once
#include "resource.hpp"

class Item
{
private:
    sf::Texture itemTexture;
    sf::Sprite itemSprite;
    sf::Vector2f itemPosition;
    sf::Vector2f spriteScale;
    sf::Vector2f itemSpawn;

    float itemAcceleration;


    void init(sf::FloatRect spawnBound, float accel, int type);
    
    

public:
    Item(sf::FloatRect spawnBound, float accel, int type);
    ~Item();
    
    sf::FloatRect itemBounds();
    void update(float velocityY, sf::FloatRect playerBounds);
    void render(sf::RenderWindow& window);

};