/*
	MIT License
	
	Copyright (c) 2024 Alexandre Foret (https://www.alexandreforet.pro)
	
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#include "ball.h"
#include "input.h"
#include "racket.h"
#include "utils.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

// Enums
enum Collision { TopRacketL, BottomRacketL, RacketL, RacketR, TopRacketR, BottomRacketR, TopWindow, BottomWindow, LeftWindow, RightWindow, None };
enum RacketDirection { Up, Down };
enum BallDirection { Left, Right };
enum Player { PlayerLeft, PlayerRight };

// Window properties
RenderWindow* window;
const string game_name{ "Pong" };
const unsigned int window_width{ 800 };
const unsigned int window_height{ 600 };
const unsigned int frame_limit{ 60 };

// Game properties
const Color playerLColor{ Color::Blue };
const Color playerRColor{ Color::Red };
const Color ballColor{ Color::White };
const Player defaultPlayer = PlayerRight;
const unsigned int maxScore{ 10 };
const float racketLSpeed{ 7.f };
const float racketRSpeed{ 7.f };
const float defaultBallSpeed{ 7.f };
// With each collision with a racket, the speed of the ball increases by this value
const float ballSpeedIncrease{ 0.15f };

// Sound properties (Volume)
// Racket collision sound
const float racketSoundVolume{ 10.f };
// Wall collision sound
const float wallSoundVolume{ 10.f };

// Left racket properties
const unsigned int racketLWidth{ 16 };
const unsigned int racketLHeight{ 80 };
const unsigned int racketLPosX{ 32 };
const unsigned int racketLPosY{ (window_height / 2) - (racketLHeight / 2) };
// Minimum and maximum location of the racket
const unsigned int racketLMinPosY{ 0 };
const unsigned int racketLMaxPosY{ window_height - racketLHeight };

// Right racket properties
const unsigned int racketRWidth{ 16 };
const unsigned int racketRHeight{ 80 };
const unsigned int racketRPosX{ window_width - 32 - racketRWidth };
const unsigned int racketRPosY{ (window_height / 2) - (racketRHeight / 2) };
// Minimum and maximum location of the racket
const unsigned int racketRMinPosY{ 0 };
const unsigned int racketRMaxPosY{ window_height - racketRHeight };

// Ball properties
const float ballRadius{ 12.f };
// Default location of the ball
const unsigned int ballPosX{ static_cast<unsigned int>(window_width / 2.f - ballRadius / 2.f) };
const unsigned int ballPosY{ static_cast<unsigned int>(window_height / 2.f - ballRadius) };

// Score properties
const unsigned int fontSize{ 30 };
const Vector2f scoreLPos = Vector2f(0, window_height / 2.f - fontSize / 2.f);
const Vector2f scoreRPos = Vector2f(window_width - fontSize + 20, window_height / 2.f - fontSize / 2.f);

// Text winner properties
const unsigned int fontSizeWinner{ 54 };
const String winnerL{ "The Blue player win" };
const String winnerR{ "The Red player win" };

// Text replay properties
const unsigned int fontSizeTextReplay{ 32 };
const String strReplay{ "Press SPACEBAR to replay" };
const Color textReplayColor{ Color::White };

// Text pause properties
const unsigned int fontSizeTextPause{ 24 };
const String strPause{ "PAUSE" };
const Color textPauseColor{ Color::White };

unsigned int collisionCount = 0;
unsigned int scoreL, scoreR = 0;
bool win = false;
bool pause = false;

float ballSpeed{ defaultBallSpeed };

Vector2f currentDirection;

// Object init
// Text
Text textScoreL;
Text textScoreR;
Text textWinner;
Text textReplay;
Text textPause;

// Input
Input* input;

// Racket
Racket* racketL;
Racket* racketR;

// Ball
Ball* ball;

// Sound buffers
SoundBuffer racketBuffer;
SoundBuffer wallBuffer;

// Functions
void CheckButton();
Collision Intersect();
void UpdateScore(Player player);
void Winner();
void TogglePause();
void Replay();

// Asset locations
const string assets_dir{ "assets/" };
const string font_file{ assets_dir + "CodeNewRoman.otf" };
const string racketSoundEffect{ assets_dir + "PaddleSound.wav" };
const string wallSoundEffect{ assets_dir + "WallSound.wav" };