#pragma once
#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//non-essential
#include <SFML/Network.hpp>


const int GEM = 1;
const int FUEL = 2;
//const int NCAGE = 3;
const int BACKGROUND = 3;

static float lerp(float a, float b, float f){ return a + f * (b - a); }