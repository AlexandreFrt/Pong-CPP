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
const string GAME_TITLE{ "Pong" };
const unsigned int WINDOW_WIDTH{ 800 };
const unsigned int WINDOW_HEIGHT{ 600 };
const unsigned int FRAME_LIMIT{ 60 };

// Game properties
const Color PLAYER_L_COLOR{ Color::Blue };
const Color PLAYER_R_COLOR{ Color::Red };
const Color BALL_COLOR{ Color::White };
const Player DEFAULT_PLAYER = PlayerRight;
const unsigned int MAX_SCORE{ 10 };
const float RACKET_L_SPEED{ 7.f };
const float RACKET_R_SPEED{ 7.f };
const float DEFAULT_BALL_SPEED{ 7.f };
// With each collision with a racket, the speed of the ball increases by this value
const float BALL_SPEED_INCREASE_VALUE{ 0.15f };

// Sound properties (Volume)
// Racket collision sound
const float RACKET_SOUND_VOLUME{ 10.f };
// Wall collision sound
const float WALL_SOUND_VOLUME{ 10.f };

// Left racket properties
const unsigned int RACKET_L_WIDTH{ 16 };
const unsigned int RACKET_L_HEIGHT{ 80 };
const unsigned int DEFAULT_RACKET_L_POS_X{ 32 };
const unsigned int DEFAULT_RACKET_L_POS_Y{ (WINDOW_HEIGHT / 2) - (RACKET_L_HEIGHT / 2) };
// Minimum and maximum location of the racket
const unsigned int RACKET_L_MIN_POS_Y{ 0 };
const unsigned int RACKET_L_MAX_POS_Y{ WINDOW_HEIGHT - RACKET_L_HEIGHT };

// Right racket properties
const unsigned int RACKET_R_WIDTH{ 16 };
const unsigned int RACKET_R_HEIGHT{ 80 };
const unsigned int DEFAULT_RACKET_R_POS_X{ WINDOW_WIDTH - 32 - RACKET_R_WIDTH };
const unsigned int DEFAULT_RACKET_R_POS_Y{ (WINDOW_HEIGHT / 2) - (RACKET_R_HEIGHT / 2) };
// Minimum and maximum location of the racket
const unsigned int RACKET_R_MIN_POS_Y{ 0 };
const unsigned int RACKET_R_MAX_POS_Y{ WINDOW_HEIGHT - RACKET_R_HEIGHT };

// Ball properties
const float BALL_RADIUS{ 12.f };
// Default location of the ball
const unsigned int DEFAULT_BALL_POS_X{ static_cast<unsigned int>(WINDOW_WIDTH / 2.f - BALL_RADIUS / 2.f) };
const unsigned int DEFAULT_BALL_POS_Y{ static_cast<unsigned int>(WINDOW_HEIGHT / 2.f - BALL_RADIUS) };

// Score properties
const unsigned int SCORE_FONT_SIZE{ 30 };

// Text winner properties
const unsigned int fontSizeWinner{ 54 };
const String TEXT_WINNER_L{ "The Blue player win" };
const String TEXT_WINNER_R{ "The Red player win" };

// Text replay properties
const unsigned int TEXT_REPLAY_FONT_SIZE{ 32 };
const String TEXT_REPLAY{ "Press SPACEBAR to replay" };
const Color TEXT_REPLAY_COLOR{ Color::White };

// Text pause properties
const unsigned int TEXT_PAUSE_FONT_SIZE{ 24 };
const String TEXT_PAUSE{ "PAUSE" };
const Color TEXT_PAUSE_COLOR{ Color::White };

unsigned int collisionCount = 0;
unsigned int scoreL, scoreR = 0;
bool win = false;
bool pause = false;

float currentBallSpeed{ DEFAULT_BALL_SPEED };

Vector2f currentDirection;

// Object init
// Window
RenderWindow* window;

// Texts
Text textScoreL;
Text textScoreR;
Text textWinner;
Text textReplay;
Text textPause;

// Input
Input* input;

// Rackets
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