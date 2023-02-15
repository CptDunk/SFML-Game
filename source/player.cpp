#include "player.hpp"

Player::Player(sf::RenderWindow& window, sf::FloatRect bgBounds, sf::View& view)
{
    this->window = &window;
    this->view = &view;
    this->bgBounds = bgBounds;
    this->initPlayer();
}
void Player::initPlayer()
{
    rotationZ = 0.f;
     // Initialize the player
    if(!this->playerTexture.loadFromFile("Images/player.png"))
    {
        std::cout << "ERROR::GAME::INITIALIZEPLAYER::FAILED TO LOAD PLAYER TEXTURE" << std::endl;
    }
    else
    {
    this->start = false;
    this->movement.x = 0.f;
    this->movement.y = 0.f;
    this->maxSpeed = 1.f;
    this->velocityY = 0.f;
    this->drag = 0.0002f;
    this->acceleration = 0.f;
    this->fuelTank = 1.f;
    this->points = 0;
    this->playerSprite.setTexture(this->playerTexture);
    this->playerSprite.setScale(0.75f, 0.75f);
    this->playerSprite.setOrigin(this->playerSprite.getLocalBounds().width/2, this->playerSprite.getLocalBounds().height/2);
    this->playerSprite.setPosition(this->window->getSize().x/2, this->window->getSize().y/3*2);
    //this->playerSprite.setPosition(this->window->getSize().x/2 - this->playerSprite.getGlobalBounds().width/2, this->window->getSize().y - this->playerSprite.getGlobalBounds().height);
    std::cout << "Player initialized" << std::endl;
    spawnBound.width = this->bgBounds.width;
    spawnBound.height = 900.f;
    spawnBound.left = this->bgBounds.left;
    spawnBound.top = -2700 + this->view->getCenter().y;
    

    //Initialize the text
    this->font.loadFromFile("fonts/comicz.ttf");
    this->pText.setFont(this->font);
    //Initialize HUD fuel bar
    this->fuelBar.setSize(sf::Vector2f(200.f, 20.f));
    this->fuelBar.setFillColor(sf::Color::Green);
    this->fuelBar.setPosition(this->view->getCenter().x - this->window->getSize().x * 0.495f, this->view->getCenter().y + this->window->getSize().y * 0.31f);
    this->fuelBar.setOutlineColor(sf::Color::Black);
    this->fuelBar.setOutlineThickness(2.f);


    }
}
void Player::updatePlayer()
{

     //smooth movement of player

     //old code movement
    //direction = sf::Vector2i(sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A), sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W));
    this->spawnBound.top = - 2700 + this->view->getCenter().y;
    this->direction.y = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    
    if(!this->start && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        this->start = true;
        this->velocityY += 0.0001f;
    }
    if(this->start)
    {
        this->direction.x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    }
    //this->fuelTank = lerp(this->fuelTank, 0, 0.001f*this->direction.y);
    this->fuelTank -= 0.0005f*this->direction.y;
    if(this->fuelTank < 0.00f) this->fuelTank = 0.00f;
    this->acceleration = lerp(this->acceleration, 0.0005f*this->direction.y, 0.01f);
    if(direction.y == 0 && velocityY > 0 || this->fuelTank == 0) velocityY -= drag;
    else velocityY += (this->direction.y * this->acceleration); 

    if(velocityY < 0.0001f) velocityY = 0.f;
    
    this->movement.x = lerp(this->movement.x, direction.x * 3.5f, 0.2f);

    //check maxspeed
    if(this->velocityY >= this->maxSpeed)
    {
        this->velocityY = this->maxSpeed;
    }
    
    //move the player
    //float rotationZ = this->playerSprite.getRotation();
    rotationZ = lerp(rotationZ, 15*direction.x, 0.08f);
    
    this->playerSprite.move(this->movement.x, -velocityY);
    this->playerSprite.setRotation(rotationZ);
    //std::cout << "Player X: " << float(int(velocityY*100))/100 << " " <<  std::endl;


    //Scroll the view - Y Axis
    if(this->playerSprite.getPosition().y < this->window->getSize().y / 3 * 2)
    { this->position.y = this->playerSprite.getPosition().y - 100; }
    else
    { this->position.y = this->window->getSize().y / 2; }
    
    //Scroll the view - X Axis
    sf::FloatRect playerBounds = this->playerSprite.getGlobalBounds();

    //Player position on screen relative to the center bottom of the screen
    float playerX = this->playerSprite.getPosition().x - this->window->getSize().x/2; ;

    if(this->bgBounds.width/2 - playerX < 400.f) 
        position.x = (this->bgBounds.width/6)*3;
    else if((-(this->bgBounds.width/2)) + std::abs(playerX) > -400.f)
        position.x = -(this->bgBounds.width/6);
    else
        position.x = this->playerSprite.getPosition().x; //+ this->playerSprite.getGlobalBounds().width/2;

    //Bounding box collision
    if(playerBounds.left <= bgBounds.left)
    {
        this->playerSprite.setPosition(bgBounds.left + playerBounds.width/2, this->playerSprite.getPosition().y);
    }
    else if(playerBounds.left + playerBounds.width >= bgBounds.left + bgBounds.width)
    {
        this->playerSprite.setPosition(bgBounds.left + bgBounds.width - playerBounds.width/2, this->playerSprite.getPosition().y);
    }
    if(playerBounds.top <= bgBounds.top)
    {
        this->playerSprite.setPosition(this->playerSprite.getPosition().x, bgBounds.top);
    }
    else if(playerBounds.top + playerBounds.height >= bgBounds.top + bgBounds.height)
    {
        this->playerSprite.setPosition(this->playerSprite.getPosition().x, bgBounds.top + bgBounds.height - playerBounds.height/2);
    }

    this->view->setCenter(this->position.x, this->position.y);

    this->pText.setPosition(this->view->getCenter().x - this->window->getSize().x * 0.50f, this->view->getCenter().y + this->window->getSize().y * 0.36f);
    this->pText.setString("Altitude: " + std::to_string((int)abs(this->playerSprite.getPosition().y-400)*3) + "\nSpeed: " + std::to_string((int)(this->velocityY*500)));
    this->fuelBar.setPosition(this->view->getCenter().x - this->window->getSize().x * 0.495f, this->view->getCenter().y + this->window->getSize().y * 0.31f);
    this->fuelBar.setSize(sf::Vector2f(200.f * this->fuelTank, 20.f));
    
    //this one did the problem with moved X axis the position X is still 0 hence why you have -400 to 400 pixels instead of 0 to 800 
    //##Changed to screen width/2 for now

}

void Player::renderPlayer()
{
    this->window->draw(this->playerSprite);

    //render HUD
    this->window->draw(this->pText);
    this->window->draw(this->fuelBar);

}
sf::FloatRect Player::playerBounds()
{
    return this->playerSprite.getGlobalBounds();
}
sf::FloatRect Player::spawnBounds()
{
    return this->spawnBound;
}
float Player::getvelocityY()
{
    return this->velocityY;
}
sf::Vector2f Player::getPos()
{
    return this->playerSprite.getPosition();
}
int Player::getPoints()
{
    return this->points;
}
void Player::reFuel(float fuel)
{
    this->fuelTank += fuel;
    if(this->fuelTank > 1.f)
    {
        this->fuelTank = 1.f;
    }
}
void Player::bump(float bump)
{
    this->velocityY -= bump;
    points -= 100;
    if (this->velocityY < 0.f) this->velocityY = 0.f;
}
void Player::loadsaMoney(int points)
{
    this->points += points;
}
bool Player::playing()
{
    return this->start;
}
Player::~Player()
{
    delete this->window;
    delete this->view;
}