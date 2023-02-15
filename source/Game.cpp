#include "Game.hpp"

void Game::initializeVariables()
{
    // Initialize the variables
    this->window = nullptr;
    this->player = nullptr;
    this->item = nullptr;
    this->enemy = nullptr;
    this->deleted = false;
    //gamelogic
    this->points = 0;
    this->gOver = false;
    //setup sounds
    gMusic.openFromFile("sound/music.wav");
    gMusic.setLoop(true);
    gMusic.setVolume(5.f);
    //setup tutorial text
    this->textOneShot = false;
    this->gFont.loadFromFile("Fonts/comicz.ttf");
    this->gText.setFont(this->gFont);
    this->gText.setString("Hello and welcome to my FlyHard Ripoff\nUse A and D to move left/right and SPACE \nto accelerate\nCollect fuel to stay airborne,\nbut watch out not to hit anything while youre \nup there, or it might slow you down\nOnce your speed reaches 0, it is game over\nthis message will self destruct once you hit SPACE");
    this->goverText.setFont(this->gFont);

    //this->goverText.setPosition(this->window->getSize().x/3, this->window->getSize().y/2);

    this->initializeWindow();
    

    //init Player
    this->player = new Player(*this->window, this->bgSprite.getGlobalBounds(),this->view);
    this->spawnTreshold = sf::Vector2f(this->player->spawnBounds().left+this->player->spawnBounds().width/2, this->player->spawnBounds().top);
    sf::FloatRect sest(this->bgSprite.getGlobalBounds().left, 0, this->bgSprite.getGlobalBounds().width, this->bgSprite.getGlobalBounds().height);
    this->spawnItem(sest, BACKGROUND);
}
void Game::initializeWindow()
{
    // Initialize the window
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode, "RipOff-Hard", sf::Style::Titlebar | sf::Style::Close);
    if(!bgTexture.loadFromFile("Images/Vertical.png"))
    {
        std::cout << "ERROR::GAME::INITIALIZEWINDOW::FAILED TO LOAD BACKGROUND TEXTURE" << std::endl;
    }
    this->bgSprite.setTexture(this->bgTexture);
    this->bgSprite.setPosition(-(this->bgSprite.getGlobalBounds().width/3.f), -this->bgSprite.getGlobalBounds().height + this->videoMode.height);
    this->view.reset(sf::FloatRect(0.f, 0.f, this->window->getSize().x, this->window->getSize().y));
    this->view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    this->window->setFramerateLimit(61);
}
//Constructors / Destructors
Game::Game()
{
        this->initializeVariables();
}
Game::~Game()
{
    delete this->window;
    delete this->player;
    delete this->item;
    delete this->enemy;
}
//Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}
bool Game::gameOver()
{
    return this->gOver;
}
//Functions
void Game::updateEvents()
{
    // Update the events
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
            {
                this->window->close();
            }
            break;
            
        }
    }
}
void Game::updateEnemies()
{
    if(this->enemies.size() > 0)
    {
        for(int i = 0; i < this->enemies.size(); i++)
        {
            int state = this->enemies[i]->updateEnemies(this->bgSprite.getGlobalBounds().left, this->bgSprite.getGlobalBounds().left + this->bgSprite.getGlobalBounds().width, this->player->getvelocityY(), this->view.getCenter().y-300, this->player->playerBounds());
            if(state == 1)
               { //out of bounds
                this->enemies.erase(enemies.begin() + i);
                this->deleted = true;
               }
            else if(state == 2)
               { //player collision
                this->enemies.erase(enemies.begin() + i);
                this->deleted = true;
                //probably could add a bonk sound when colliding... hmm...
                this->player->bump(0.1f);
               }
            if(this->deleted) i--; this->deleted = false;
        }
    }
    else
    {
        //spawns enemy if theres none left(either due to total annihilation or player collision)
        this->spawnEnemy();
    }
}
/// TODO: move to item class(pass view and player bounds)
void Game::updateItems()
{
    

    bool respawn = (this->spawnTreshold.y > this->view.getCenter().y - 650.f);
    
    if(this->gems.size() > 0)
    {
        for(int i = 0; i < this->gems.size(); i++)
        {
            this->gems[i]->update(this->player->getvelocityY(), this->player->playerBounds());
            if(this->view.getCenter().y - 300.f < this->gems[i]->itemBounds().top - 800.f)
            {
                this->gems.erase(gems.begin() + i);
                this->deleted = true;
            }
            else if(this->player->playerBounds().intersects(this->gems[i]->itemBounds()))
            {
                this->gems.erase(gems.begin() + i);
                this->deleted = true;
                //player gets bonus points for hitting these
                this->player->loadsaMoney(500);
            }

            if(this->deleted) i--;
            this->deleted = false;

        }
    }
    if(respawn)
        this->spawnItem(this->player->spawnBounds(), GEM);
    
    //update fuel Item
    if(this->fuel.size() > 0)
    {
        for(int i = 0; i < this->fuel.size(); i++)
        {
            this->fuel[i]->update(this->player->getvelocityY(), this->player->playerBounds());
            if(this->view.getCenter().y - 300.f < this->fuel[i]->itemBounds().top - 800.f)
            {
                this->fuel.erase(fuel.begin() + i);
                this->deleted = true;
            }
            else if(this->player->playerBounds().intersects(this->fuel[i]->itemBounds()))
            {
                this->fuel.erase(fuel.begin() + i);
                this->deleted = true;
                //replenish players fuel tank by some amount
                this->player->reFuel(0.33f);
            }

            if(this->deleted) i--;
            this->deleted = false;
        }
    }
    if(respawn)
        this->spawnItem(this->player->spawnBounds(), FUEL);
    //update bgs Items
    if(this->bgs.size() > 0)
    {
        for (int i = 0; i < this->bgs.size(); i++)
        {
            this->bgs[i]->update(this->player->getvelocityY(), this->player->playerBounds());
            if(this->view.getCenter().y - 300.f < this->bgs[i]->itemBounds().top - 800.f)
            {
                //std::cout << "itembounds: " << this->bgs[i]->itemBounds().top-650 << " View center -300: " << this->view.getCenter().y-300 << std::endl;
                this->bgs.erase(bgs.begin() + i);
                this->deleted = true;
            }

            if(this->deleted) i--;
            this->deleted = false;
        }
    }
    if(respawn)
    {
        this->spawnTreshold.y = this->player->spawnBounds().top;
    }
}

void Game::update()
{
        // Update the game
        this->updateEvents();
        if(!this->gOver)
        {
        this->player->updatePlayer();
        this->spawnTreshold.y += this->player->getvelocityY() * 3.85f;
        if(this->player->playing() && this->gMusic.getStatus() != sf::Music::Playing)
        gMusic.play();

        this->updateEnemies();
        this->updateItems();

        if (this->player->getPos().y-300 < -5400)
    {
        this->points = this->player->getPoints();
        this->points += 1000;

        this->goverText.setString("Game Over\nYou reached the geostationary orbit\nScore: " + std::to_string(this->points) + "\nPress ESC to exit");
        if(!this->bgTexture.loadFromFile("images/spaceSloth.jpg"))
        {
            std::cout << "Error loading spaceSloth.jpg" << std::endl;
        }
        else
        {
            this->goverSprite.setTexture(this->bgTexture);
            this->goverSprite.getGlobalBounds() = sf::FloatRect(0, 0, this->bgTexture.getSize().x, this->bgTexture.getSize().y);
            this->goverSprite.setOrigin(this->goverSprite.getGlobalBounds().left + this->goverSprite.getGlobalBounds().width/2, this->goverSprite.getGlobalBounds().top + this->goverSprite.getGlobalBounds().height/2);
            this->goverSprite.setPosition(this->window->getSize().x/2, this->window->getSize().y/2);
            this->goverSprite.setScale(1.4f, 2.f);
            this->goverText.setFillColor(sf::Color::Black);
        }

        this->gOver = true;
        this->gMusic.stop();
        //yippee
    }
    else if(this->player->getvelocityY() == 0 && this->player->playing())
    {
        this->points = this->player->getPoints();
        this->points -= 800;

        this->goverText.setString("Game Over\nYou failed to reach the stars!\nScore: " + std::to_string(this->points) + "\nPress ESC to exit");
        if(!this->bgTexture.loadFromFile("images/crashed.jpg"))
        {
            std::cout << "Error loading crashed.jpg" << std::endl;
        }
        else
        {
            this->goverSprite.setTexture(this->bgTexture);
            this->goverSprite.getGlobalBounds() = sf::FloatRect(0, 0, this->bgTexture.getSize().x, this->bgTexture.getSize().y);
            this->goverSprite.setOrigin(this->goverSprite.getGlobalBounds().left + this->goverSprite.getGlobalBounds().width/2, this->goverSprite.getGlobalBounds().top + this->goverSprite.getGlobalBounds().height/2);
            this->goverSprite.setPosition(this->window->getSize().x/2, this->window->getSize().y/2);
            this->goverSprite.setScale(1.4f, 2.f);

        }
       
        this->gOver = true;
        this->gMusic.stop();
        //boowomp
    }


        }
    
}
void Game::render()
{

    // Set the view
    this->window->setView(this->view);
    

    // Render the game
    this->window->clear(sf::Color::White);
    this->window->draw(this->bgSprite);

    //render stuff
    this->renderItems();
    this->renderEnemies();
    if(!this->player->playing())
    this->window->draw(this->gText);
    //this->window->draw(this->playerSprite);
    //this->enemy->renderEnemies();
    this->player->renderPlayer();

    //show rendered stuff

    this->window->display();

}
void Game::spawnEnemy()
{
    for(int i = 0; i < 15; i++)
    {
        this->enemy = new Enemy(this->player->spawnBounds());
        this->enemies.push_back(this->enemy);
    } 
}
void Game::spawnItem(sf::FloatRect spawnBounds, int type)
{
    switch (type)
    {
    case 1:
    for(int i = 0; i < 2; i++)
    {
        this->item = new Item(spawnBounds, 3.85f, GEM);
        this->gems.push_back(this->item);
    }
        break;
    case 2:
    for(int i = 0; i < 2; i++)
    {
        this->item = new Item(spawnBounds, 3.85f, FUEL);
        this->fuel.push_back(this->item);
    }
        break;
    case 3:
        this->item = new Item(spawnBounds, 8.5f, BACKGROUND);
        this->bgs.push_back(this->item);
        break;
    default:
        break;
    }
}
void Game::renderItems()
{
    // Render the items
    //i could apply the same method as in ItemUpdate here... maybe
    if(this->fuel.size() > 0)
    for(auto &i : this->fuel)
    {
        i->render(*this->window);
    }
    if(this->gems.size() > 0)
    for(auto &i : this->gems)
    {
        i->render(*this->window);
    } 
    if(this->ncages.size() > 0)
    for(auto &i : this->ncages)
    {
        i->render(*this->window);
    } 
    if(this->bgs.size() > 0)
    for(auto &i : this->bgs)
    {
        i->render(*this->window);
    } 

}
void Game::renderGameOver()
{
    this->window->setView(this->window->getDefaultView());
    this->window->clear(sf::Color(255, 155, 55, 255));
    //this->view.reset(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    this->window->draw(this->goverSprite);
    this->window->draw(this->goverText);
    this->window->display();

}
void Game::renderEnemies()
{
    // Render the enemies
    if(this->enemies.size() > 0)
    for(auto &e : this->enemies)
    {
        e->renderEnemies(*this->window);
    } 
}

