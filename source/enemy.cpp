#include "enemy.hpp"

Enemy::Enemy(sf::FloatRect spawnBound)
{
   /*
    this->window = &window;
    this->view = &view;
    this->spawnBound = spawnBound;
    */
    this->initEnemy(spawnBound);
}

Enemy::~Enemy()
{
    //Delete enemy
    //delete this->window;
}

void Enemy::initEnemy(sf::FloatRect spawnBound)
{
    //Probably load texture here
        //-Height dependend
    //Set texture to sprite
    //Set position of enemy(get random position of passed reference to player spawn Bounds)
    sf::Vector2f spawnPosition;
    spawnBound.left = spawnBound.left + this->enemyBounds().width;
    spawnBound.width = spawnBound.width - this->enemyBounds().width*2;
    spawnPosition.x = rand() % (int)spawnBound.width + (int)spawnBound.left;
    spawnPosition.y = rand() % (int)spawnBound.height + (int)spawnBound.top;
    //std::cout << (int)spawnBound.width << " " << (int)spawnBound.left << std::endl;
    this->enemyAcceleration = 3.85f;
    this->enemySprite.setPosition(spawnPosition);
    this->enemyPosition = sf::Vector2f(0.f, 0.f);
    this->enemyVelocity = sf::Vector2f(0.f, 0.f);
    this->enemyTexture.loadFromFile("Images/enemy.png");
    this->enemySprite.setTexture(this->enemyTexture);
    this->enemySprite.setScale(0.25f, 0.25f);

    //std::cout << "enemy initialized" << "x position: " << spawnPosition.x << "  y position: " << spawnPosition.y << std::endl;

}
sf::FloatRect Enemy::enemyBounds()
{
    return this->enemySprite.getGlobalBounds();
}
int Enemy::updateEnemies(float bgBoundXMin, float bgBoundXMax, float velocityY, float viewCenterY, sf::FloatRect playerBounds)
{
    //check if enemy is out of view or collided with

    //update movement of enemy
    this->enemyPosition.x = 2.f * flip;
    if(this->enemyBounds().left + this->enemyBounds().width >= bgBoundXMax) { flip = -1; }
    else if(this->enemyBounds().left <= bgBoundXMin) { flip = 1; }

    this->enemyPosition.y = velocityY * this->enemyAcceleration;
    this->enemySprite.move(this->enemyPosition);

    if(viewCenterY < this->enemySprite.getGlobalBounds().top -800)
            {
                return 1;
            }
            if(playerBounds.intersects(this->enemySprite.getGlobalBounds()))
            {
                return 2;
            }
            else
            {
                return 0;
            }
}
void Enemy::renderEnemies(sf::RenderWindow& window)
{
    window.draw(this->enemySprite);
}