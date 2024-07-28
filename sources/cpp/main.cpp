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

#include <iostream>
#include <random>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "input.h"
#include "main.h"

using namespace std;

int main()
{
    // Render window
    window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE);
    Event event;
    Texture texture;
    Font font;

    // Load the font
    if (!font.loadFromFile(font_file))
    {
        cout << "FONT LOADING ERROR\n";
        window->close();
    }

    // Load the racket sound effect
    if (!LoadSound(racketBuffer, racketSoundEffect))
    {
        cout << "SOUND LOADING ERROR\n";
    }

    // Load the wall sound effect
    if (!LoadSound(wallBuffer, wallSoundEffect))
    {
        cout << "SOUND LOADING ERROR\n";
    }

    // Set sound properties
    Sound racketSound, wallSound;
    racketSound.setBuffer(racketBuffer);
    wallSound.setBuffer(wallBuffer);

    racketSound.setVolume(RACKET_SOUND_VOLUME);
    wallSound.setVolume(WALL_SOUND_VOLUME);

    // Frame rate limit
    window->setFramerateLimit(FRAME_LIMIT);

    // Init the text
    textScoreL = Text("0", font);
    textScoreR = Text("0", font);

    textWinner = Text("", font);
    textWinner.setCharacterSize(fontSizeWinner);

    textReplay = Text("", font);
    textReplay.setCharacterSize(TEXT_REPLAY_FONT_SIZE);
    textReplay.setFillColor(TEXT_REPLAY_COLOR);

    textPause = Text("", font);
    textPause.setCharacterSize(TEXT_PAUSE_FONT_SIZE);
    textPause.setFillColor(TEXT_PAUSE_COLOR);

    textScoreL.setCharacterSize(SCORE_FONT_SIZE);
    textScoreR.setCharacterSize(SCORE_FONT_SIZE);
    textScoreL.setFillColor(PLAYER_L_COLOR);
    textScoreR.setFillColor(PLAYER_R_COLOR);

    textScoreL.setPosition(14 - textScoreL.getLocalBounds().width / 2.f, WINDOW_HEIGHT / 2.f - textScoreL.getLocalBounds().height / 2.f);
    textScoreR.setPosition(WINDOW_WIDTH - 20 - textScoreR.getLocalBounds().width / 2.f, WINDOW_HEIGHT / 2.f - textScoreR.getLocalBounds().height / 2.f);


    // Init the rackets
    racketL = new Racket(RACKET_L_WIDTH, RACKET_L_HEIGHT, DEFAULT_RACKET_L_POS_X, DEFAULT_RACKET_L_POS_Y, PLAYER_L_COLOR);
    racketR = new Racket(RACKET_R_WIDTH, RACKET_R_HEIGHT, DEFAULT_RACKET_R_POS_X, DEFAULT_RACKET_R_POS_Y, PLAYER_R_COLOR);

    // Init the ball
    ball = new Ball(BALL_RADIUS, DEFAULT_BALL_POS_X, DEFAULT_BALL_POS_Y, BALL_COLOR);

    input = new Input();

    // Throws the ball depending on who starts
    switch (DEFAULT_PLAYER)
    {
    case PlayerLeft:
        currentDirection = Vector2f(-1.f, 0.f);
        break;
    case PlayerRight:
        currentDirection = Vector2f(1.f, 0.f);
        break;
    }

    while (window->isOpen())
    {
        // Events
        while (window->pollEvent(event))
        {
            input->InputHandler(event, *window);
        }

        // Update
        CheckButton();

        // Collision effects
        if (!win && !pause)
        {
            if (Intersect() != LeftWindow && Intersect() != RightWindow)
            {
                ball->Move(currentDirection, currentBallSpeed);
            }

            if (Intersect() == TopRacketL || Intersect() == TopRacketR || Intersect() == BottomRacketL || Intersect() == BottomRacketR)
            {
                collisionCount++;

                racketSound.play();

                if (Intersect() == TopRacketL || Intersect() == BottomRacketL)
                {
                    ball->SetPosition(Vector2f(racketL->GetPosition().x + RACKET_L_WIDTH, ball->GetPosition().y));
                }
                else
                {
                    ball->SetPosition(Vector2f(racketR->GetPosition().x - BALL_RADIUS * 2, ball->GetPosition().y));
                }

                if (Intersect() == TopRacketL || Intersect() == TopRacketR)
                {
                    currentDirection = Vector2f(currentDirection.x * -1.f, 0.5f);
                }
                else
                {
                    currentDirection = Vector2f(currentDirection.x * -1.f, -0.5f);
                }

                // Increase the ball speed
                currentBallSpeed = DEFAULT_BALL_SPEED + static_cast<float>(collisionCount) * BALL_SPEED_INCREASE_VALUE;
            }
            // If there is a collision between the ball and the window then bounce the ball
            else if (Intersect() == TopWindow || Intersect() == BottomWindow)
            {
                wallSound.play();

                if (Intersect() == TopWindow)
                {
                    ball->SetPosition(Vector2f(ball->GetPosition().x, 0));
                }
                else
                {
                    ball->SetPosition(Vector2f(ball->GetPosition().x, WINDOW_HEIGHT - BALL_RADIUS * 2));
                }

                currentDirection = Vector2f(currentDirection.x, currentDirection.y * -1);
            }
            else if (Intersect() == LeftWindow)
            {
                UpdateScore(PlayerRight);
            }
            else if (Intersect() == RightWindow)
            {
                UpdateScore(PlayerLeft);
            }
        }

        // Draw
        window->clear();

        window->draw(textScoreL);
        window->draw(textScoreR);
        window->draw(textWinner);
        window->draw(textReplay);
        window->draw(textPause);

        window->draw(racketL->GetShape());
        window->draw(racketR->GetShape());
        window->draw(ball->GetShape());

        window->display();

        // Reset the escape button after processing it
        input->ResetButtons();
    }
    return 0;
}

// Check input
void CheckButton()
{
    if (!pause)
    {
        // Left racket -> Move Up
        if (input->GetButton().Z)
        {
            // Limit the movement based on window
            if (racketL->GetPosition().y > RACKET_L_MIN_POS_Y)
            {
                racketL->Move(1, RACKET_L_SPEED);
            }
        }

        // Left racket -> Move Down
        if (input->GetButton().S)
        {
            // Limit the movement based on window
            if (racketL->GetPosition().y < RACKET_L_MAX_POS_Y)
            {
                racketL->Move(-1, RACKET_L_SPEED);
            }
        }

        // Right racket -> Move Up
        if (input->GetButton().up)
        {
            // Limit the movement based on window
            if (racketR->GetPosition().y > RACKET_R_MIN_POS_Y)
            {
                racketR->Move(1, RACKET_R_SPEED);
            }
        }

        // Right racket -> Move Down
        if (input->GetButton().down)
        {
            // Limit the movement based on window
            if (racketR->GetPosition().y < RACKET_R_MAX_POS_Y)
            {
                racketR->Move(-1, RACKET_R_SPEED);
            }
        }
    }

    // Toggle pause if the space bar is pressed
    if (input->GetButton().escape)
    {
        TogglePause();
    }

    if (input->GetButton().space)
    {
        Replay();
    }
}

// Collision detection
Collision Intersect()
{
    Collision collisionObject = None;

    // Get ball coordinate values
    const float ballMinX = ball->GetPosition().x;
    const float ballMinY = ball->GetPosition().y;

    const float ballMaxX = ballMinX + BALL_RADIUS * 2;
    const float ballMaxY = ballMinY + BALL_RADIUS * 2;

    // Get left racket coordinate values
    const float racketLMinX = racketL->GetPosition().x;
    const float racketLMinY = racketL->GetPosition().y;

    const float racketLMaxX = racketLMinX + RACKET_L_WIDTH;
    const float racketLMaxY = racketLMinY + RACKET_L_HEIGHT;

    // Get right racket coordinate values
    const float racketRMinX = racketR->GetPosition().x;
    const float racketRMinY = racketR->GetPosition().y;

    const float racketRMaxX = racketRMinX + RACKET_R_WIDTH;
    const float racketRMaxY = racketRMinY + RACKET_R_HEIGHT;

    /*
    AABB algorithm (Axis-Aligned Bounding Boxes) in 2D
    */

    // Collision with left racket
    if (ballMinX <= racketLMaxX && ballMaxX >= racketLMinX &&
        ballMinY <= racketLMaxY && ballMaxY >= racketLMinY)
    {
        const float racketMiddleY = racketLMinY + RACKET_L_HEIGHT / 2.0f;

        if (ballMaxY >= racketMiddleY)
        {
            collisionObject = BottomRacketL;
        }
        else
        {
            collisionObject = TopRacketL;
        }
    }
    // Collision with right racket
    else if (ballMinX <= racketRMaxX && ballMaxX >= racketRMinX &&
        ballMinY <= racketRMaxY && ballMaxY >= racketRMinY)
    {
        const float racketMiddleY = racketRMinY + RACKET_R_HEIGHT / 2.0f;

        if (ballMaxY >= racketMiddleY)
        {
            collisionObject = BottomRacketR;
        }
        else
        {
            collisionObject = TopRacketR;
        }
    }

    // Collision between the ball and the top of the window
    else if (ballMinY <= 0)
    {
        collisionObject = TopWindow;
    }

    // Collision between the ball and the bottom of the window
    else if (ballMaxY >= WINDOW_HEIGHT)
    {
        collisionObject = BottomWindow;
    }

    // Collision between the ball and the right of the window
    else if (ballMaxX >= WINDOW_WIDTH)
    {
        collisionObject = RightWindow;
    }

    // Collision between the ball and the left of the window
    else if (ballMinX <= 0)
    {
        collisionObject = LeftWindow;
    }

    return collisionObject;
}

// Update the score if a player scores
void UpdateScore(Player player)
{
    switch (player)
    {
    case PlayerLeft:
        scoreL++;
        SetText(textScoreL, to_string(scoreL));
        break;
    case PlayerRight:
        scoreR++;
        SetText(textScoreR, to_string(scoreR));
        break;
    }

    // If the total score is even then throws the ball to the default player
    if ((scoreR + scoreL) % 2 == 0)
    {
        switch (DEFAULT_PLAYER)
        {
        case PlayerLeft:
            currentDirection = Vector2f(-1.f, 0.f);
            break;
        case PlayerRight:
            currentDirection = Vector2f(1.f, 0.f);
            break;
        }
    }
    else
    {
        switch (DEFAULT_PLAYER)
        {
        case PlayerLeft:
            currentDirection = Vector2f(1.f, 0.f);
            break;
        case PlayerRight:
            currentDirection = Vector2f(-1.f, 0.f);
            break;
        }
    }

    // Update score text
    textScoreL.setPosition(14 - textScoreL.getLocalBounds().width / 2, WINDOW_HEIGHT / 2.f - textScoreL.getLocalBounds().height / 2.f);
    textScoreR.setPosition(WINDOW_WIDTH - 20 - textScoreR.getLocalBounds().width / 2, WINDOW_HEIGHT / 2.f - textScoreR.getLocalBounds().height / 2.f);

    // Resets the racket to start with the rackets in the middle of the screen
    racketL = new Racket(RACKET_L_WIDTH, RACKET_L_HEIGHT, DEFAULT_RACKET_L_POS_X, DEFAULT_RACKET_L_POS_Y, PLAYER_L_COLOR);
    racketR = new Racket(RACKET_R_WIDTH, RACKET_R_HEIGHT, DEFAULT_RACKET_R_POS_X, DEFAULT_RACKET_R_POS_Y, PLAYER_R_COLOR);

    // Check if there is a winner
    Winner();

    // Reset the ball
    ball = new Ball(BALL_RADIUS, DEFAULT_BALL_POS_X, DEFAULT_BALL_POS_Y, BALL_COLOR);
    currentBallSpeed = DEFAULT_BALL_SPEED;
    collisionCount = 0;

    // Pause during 1.5 second
    sleep(Time(seconds(1.5f)));
}

// Check if there is a winner
void Winner()
{
    // If a player has a score higher than the max score, the game is over
    if (scoreL >= MAX_SCORE)
    {
        win = true;
        textWinner.setFillColor(PLAYER_L_COLOR);
        SetText(textWinner, TEXT_WINNER_L);
        SetText(textReplay, TEXT_REPLAY);
    }
    else if (scoreR >= MAX_SCORE)
    {
        win = true;
        textWinner.setFillColor(PLAYER_R_COLOR);
        SetText(textWinner, TEXT_WINNER_R);
        SetText(textReplay, TEXT_REPLAY);
    }

    textWinner.setPosition(WINDOW_WIDTH / 2.f - textWinner.getLocalBounds().width / 2.f, 24);
    textReplay.setPosition(WINDOW_WIDTH / 2.f - textReplay.getLocalBounds().width / 2.f, 96);
}

// Toggle pause function
void TogglePause()
{
    pause = !pause;
    SetText(textPause, pause ? TEXT_PAUSE : "");
}

// Replay if the game is over and the space bar is pressed
void Replay()
{
    if (win)
    {
        win = false;
        scoreL = 0;
        scoreR = 0;
        SetText(textWinner, "");
        SetText(textReplay, "");
        SetText(textScoreL, "0");
        SetText(textScoreR, "0");
        textScoreL.setPosition(14 - textScoreL.getLocalBounds().width / 2, WINDOW_HEIGHT / 2.f - textScoreL.getLocalBounds().height / 2.f);
        textScoreR.setPosition(WINDOW_WIDTH - 20 - textScoreR.getLocalBounds().width / 2, WINDOW_HEIGHT / 2.f - textScoreR.getLocalBounds().height / 2.f);

        ball = new Ball(BALL_RADIUS, DEFAULT_BALL_POS_X, DEFAULT_BALL_POS_Y, BALL_COLOR);

        racketL = new Racket(RACKET_L_WIDTH, RACKET_L_HEIGHT, DEFAULT_RACKET_L_POS_X, DEFAULT_RACKET_L_POS_Y, PLAYER_L_COLOR);
        racketR = new Racket(RACKET_R_WIDTH, RACKET_R_HEIGHT, DEFAULT_RACKET_R_POS_X, DEFAULT_RACKET_R_POS_Y, PLAYER_R_COLOR);

        switch (DEFAULT_PLAYER)
        {
        case PlayerLeft:
            currentDirection = Vector2f(-1.f, 0.f);
            break;
        case PlayerRight:
            currentDirection = Vector2f(1.f, 0.f);
        }
	}
}