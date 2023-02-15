#pragma once
#include "resource.hpp"

/*
    Player class
    -TODO: give player a "spawn rectangle" on which enemies and items will spawn once requirements are met
*/
class Player
{
    private:
        //Variables
        sf::Texture playerTexture;
        sf::Sprite playerSprite;
        sf::Vector2i direction;
        sf::FloatRect spawnBound;

        //Game arguments
        sf::RenderWindow* window;
        sf::View* view;
        sf::FloatRect bgBounds;
        //Player Movement
        sf::Vector2f movement;
        //HUD components
        sf::Font font;
        sf::Text pText;
        sf::RectangleShape fuelBar;
        sf::RectangleShape fuelBarOutline;
        sf::RectangleShape fuelBarFill;

        float acceleration;
        float drag;
        float maxSpeed;
        float velocityY;
        bool start;
        float rotationZ;
        int points;
        float fuelTank;
        //Player View Camera
        sf::Vector2i position;
        //Functions
        void initPlayer();


    public:
        Player(sf::RenderWindow& window, sf::FloatRect bgBounds, sf::View& view);
        ~Player();
        void updatePlayer();
        void renderPlayer();
        sf::FloatRect playerBounds();
        sf::FloatRect spawnBounds();
        float getvelocityY();
        sf::Vector2f getPos();
        void bump(float velocityY);
        void reFuel(float fuelAmount);
        void loadsaMoney(int points);
        int getPoints();
        bool playing();
};