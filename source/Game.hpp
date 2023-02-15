#pragma once

#include "resource.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "item.hpp"

/*
    Game class acts as game engine
    wrapper class
*/

class Game
{
    private:
    //font variables for tutorial
    sf::Font gFont;
    sf::Text gText;
    sf::Text goverText;
    sf::Sprite goverSprite;
    bool textOneShot;
    //Variables
    bool gOver;
    bool deleted;
    
    //window
    sf::RenderWindow* window;
    sf::Event ev;
    sf::VideoMode videoMode;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    //mouse position
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //gamelogic
    int points;
    sf::Vector2f spawnTreshold;

    //Game Objects
    //std::vector<sf::RectangleShape> enemies;
    //sf::RectangleShape enemy;
    //enemy test vector

    //Player game object
    Player* player;
    //Enemy game object
    Enemy* enemy;
    std::vector<Enemy*> enemies;
    bool spawned;
    //Item game object
    Item* item;
    //for V2 change to vector*
    std::vector<Item*> fuel; //may need more fuel than that, then youd have to change the vector pointing to fuel, so it points only to the closest one
    std::vector<Item*> gems; 
    std::vector<Item*> ncages;
    std::vector<Item*> bgs;
    sf::Music gMusic;
    sf::SoundBuffer gmBuffer;

    int type;
    //Player View Camera
    sf::View view;

    //private functions
    void initializeVariables();
    void initializeWindow();
    //void initializeEnemies();

    public:
    //constructors, destructors
        Game();
        ~Game();
    //accessors
    const bool running() const;
    //functions
    void spawnEnemy();
    void spawnItem(sf::FloatRect spawnBound, int type);
    void updateEvents(); 
    void updateEnemies();
    void updateItems();
    void update();
    void renderEnemies();
    void renderItems();
    void render();
    void renderGameOver();
    bool gameOver();

};