#include "item.hpp"

Item::Item(sf::FloatRect spawnBound, float accel, int type)
{
    this->init(spawnBound, accel, type);
}
Item::~Item()
{

}
void Item::init(sf::FloatRect spawnBound, float accel, int type)
{


    this->itemPosition = sf::Vector2f(0.f, 0.f);
    //this->itemSprite.setScale(0.5f, 0.5f);
   // spawnBound.left = spawnBound.left + this->itemBounds().width;
    //spawnBound.width = spawnBound.width - this->itemBounds().width*2;
    itemAcceleration = accel;
    this->itemPosition.x = rand() % (int)spawnBound.width + spawnBound.left;
    this->itemPosition.y = rand() % (int)spawnBound.height + spawnBound.top;
    this->itemSpawn.x = spawnBound.left;
    this->itemSpawn.y = spawnBound.top;
    this->itemPosition.y = this->itemPosition.y;
    this->itemSprite.setPosition(this->itemPosition);
    this->itemPosition = sf::Vector2f(0.f, 0.f);

    switch (type)
    {
    //FUEL type
    case 1:
        if(!this->itemTexture.loadFromFile("Images/gem.png"))
        {
            std::cout << "ERROR LOADING fuel.png" << std::endl;
        }
        else
        {
            //std::cout << "THIS IS FUEL" << std::endl;
            this->itemSprite.setScale(0.08f, 0.08f);            
        }
        break;
    //GEM type
    case 2:
        if(!this->itemTexture.loadFromFile("Images/fuel.png"))
        {
            std::cout << "ERROR LOADING gem.png" << std::endl;
        }
        else
        {
            //std::cout << "THIS IS GEM" << std::endl;
            this->itemSprite.setScale(0.25f, 0.25f);            
        }
        break;
    //BACKGROUND type
    case 3:
        if(!this->itemTexture.loadFromFile("Images/background.png"))
        {
            std::cout << "ERROR LOADING background.png" << std::endl;
        }
        else
        {
            //std::cout << "THIS IS BACKGROUND" << std::endl;
            this->itemSpawn.y = -250.f;
            this->itemSprite.setScale(1.f, 1.f);
            this->itemSprite.setPosition(this->itemSpawn);
            this->itemSpawn.y = 0.f;
        }
        break;
    default:
        break;
    }
    this->itemSprite.setTexture(this->itemTexture);

}
void Item::update(float velocityY, sf::FloatRect playerBounds) //for V2 change to INT and add view CenterY
{
    this->itemPosition.y = velocityY * this->itemAcceleration;
    this->itemSprite.move(this->itemPosition);

    /*if(viewCenterY < this->itemSprite.getGlobalBounds().top -650)
    {
        return 1;
    }
    else if(playerBounds.intersects(this->itemSprite.getGlobalBounds()))
    {
        if(type != 4)
        return 2;
        else return 0;
    }
    else
    {
        return 0;
    }*/

}
void Item::render(sf::RenderWindow& window)
{
    window.draw(this->itemSprite);
}
sf::FloatRect Item::itemBounds()
{
    return this->itemSprite.getGlobalBounds();
}