
#include "Game.hpp"
#include <SFML/Audio.hpp>


int main()
{
    //init srand
    bool restart = false;
    srand(static_cast<unsigned>(time(NULL)));
   
   //restart game
    //init gameObject
    Game game;
    //game loop
    while(game.running())
    {
        //Update
        game.update();
        //Render
        if(game.gameOver())
        {
            game.renderGameOver();
        }
        else
        {
            game.render();
        }
    }
    //the end

    return 1;
}