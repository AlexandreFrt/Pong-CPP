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
    window = new RenderWindow(VideoMode(window_width, window_height), game_name);
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

    racketSound.setVolume(racketSoundVolume);
    wallSound.setVolume(wallSoundVolume);

    // Frame rate limit
    window->setFramerateLimit(frame_limit);

    // Init the text
    textScoreL = Text("0", font);
    textScoreR = Text("0", font);

    textWinner = Text("", font);
    textWinner.setCharacterSize(fontSizeWinner);

    textReplay = Text("", font);
    textReplay.setCharacterSize(fontSizeTextReplay);
    textReplay.setFillColor(textReplayColor);

    textPause = Text("", font);
    textPause.setCharacterSize(fontSizeTextPause);
    textPause.setFillColor(textPauseColor);

    textScoreL.setCharacterSize(fontSize);
    textScoreR.setCharacterSize(fontSize);
    textScoreL.setFillColor(playerLColor);
    textScoreR.setFillColor(playerRColor);

    textScoreL.setPosition(14 - textScoreL.getLocalBounds().width / 2.f, window_height / 2.f - textScoreL.getLocalBounds().height / 2.f);
    textScoreR.setPosition(window_width - 20 - textScoreR.getLocalBounds().width / 2.f, window_height / 2.f - textScoreR.getLocalBounds().height / 2.f);


    // Init the rackets
    racketL = new Racket(racketLWidth, racketLHeight, racketLPosX, racketLPosY, playerLColor);
    racketR = new Racket(racketRWidth, racketRHeight, racketRPosX, racketRPosY, playerRColor);

    // Init the ball
    ball = new Ball(ballRadius, ballPosX, ballPosY, ballColor);

    input = new Input();

    // Throws the ball depending on who starts
    switch (defaultPlayer)
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
                ball->Move(currentDirection, ballSpeed);
            }

            if (Intersect() == TopRacketL || Intersect() == TopRacketR || Intersect() == BottomRacketL || Intersect() == BottomRacketR)
            {
                collisionCount++;

                racketSound.play();

                if (Intersect() == TopRacketL || Intersect() == BottomRacketL)
                {
                    ball->SetPosition(Vector2f(racketL->GetPosition().x + racketLWidth, ball->GetPosition().y));
                }
                else
                {
                    ball->SetPosition(Vector2f(racketR->GetPosition().x - ballRadius * 2, ball->GetPosition().y));
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
                ballSpeed = defaultBallSpeed + static_cast<float>(collisionCount) * ballSpeedIncrease;
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
                    ball->SetPosition(Vector2f(ball->GetPosition().x, window_height - ballRadius * 2));
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
            if (racketL->GetPosition().y > racketLMinPosY)
            {
                racketL->Move(1, racketLSpeed);
            }
        }

        // Left racket -> Move Down
        if (input->GetButton().S)
        {
            // Limit the movement based on window
            if (racketL->GetPosition().y < racketLMaxPosY)
            {
                racketL->Move(-1, racketLSpeed);
            }
        }

        // Right racket -> Move Up
        if (input->GetButton().up)
        {
            // Limit the movement based on window
            if (racketR->GetPosition().y > racketRMinPosY)
            {
                racketR->Move(1, racketRSpeed);
            }
        }

        // Right racket -> Move Down
        if (input->GetButton().down)
        {
            // Limit the movement based on window
            if (racketR->GetPosition().y < racketRMaxPosY)
            {
                racketR->Move(-1, racketRSpeed);
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
    const int ballMinX = static_cast<int>(ball->GetPosition().x);
    const int ballMinY = static_cast<int>(ball->GetPosition().y);

    const float ballMaxX = ballMinX + ballRadius * 2;
    const float ballMaxY = ballMinY + ballRadius * 2;

    // Get left racket coordinate values
    const int racketLMinX = static_cast<int>(racketL->GetPosition().x);
    const int racketLMinY = static_cast<int>(racketL->GetPosition().y);

    const unsigned int racketLMaxX = racketLMinX + racketLWidth;
    const unsigned int racketLMaxY = racketLMinY + racketLHeight;

    // Get right racket coordinate values
    const int racketRMinX = static_cast<int>(racketR->GetPosition().x);
    const int racketRMinY = static_cast<int>(racketR->GetPosition().y);

    const unsigned int racketRMaxX = racketRMinX + racketRWidth;
    const int racketRMaxY = racketRMinY + racketRHeight;



    /*
    AABB algorithm (Axis-Aligned Bounding Boxes) in 2D
     */

     // Collision with left racket
    if (ballMinX <= racketLMaxX && ballMaxX >= racketLMinX &&
        ballMinY <= racketLMaxY && ballMaxY >= racketLMinY)
    {
        const float racketMiddleY = racketLMinY + racketLHeight / 2.0f;

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
        const float racketMiddleY = racketRMinY + racketRHeight / 2.0f;

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
    else if (ballMaxY >= window_height)
    {
        collisionObject = BottomWindow;
    }

    // Collision between the ball and the right of the window
    else if (ballMaxX >= window_width)
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
        switch (defaultPlayer)
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
        switch (defaultPlayer)
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
    textScoreL.setPosition(14 - textScoreL.getLocalBounds().width / 2, window_height / 2.f - textScoreL.getLocalBounds().height / 2.f);
    textScoreR.setPosition(window_width - 20 - textScoreR.getLocalBounds().width / 2, window_height / 2.f - textScoreR.getLocalBounds().height / 2.f);

    // Resets the racket to start with the rackets in the middle of the screen
    racketL = new Racket(racketLWidth, racketLHeight, racketLPosX, racketLPosY, playerLColor);
    racketR = new Racket(racketRWidth, racketRHeight, racketRPosX, racketRPosY, playerRColor);

    // Check if there is a winner
    Winner();

    // Reset the ball
    ball = new Ball(ballRadius, ballPosX, ballPosY, ballColor);
    ballSpeed = defaultBallSpeed;
    collisionCount = 0;

    // Pause during 1.5 second
    sleep(Time(seconds(1.5f)));
}

// Check if there is a winner
void Winner()
{
    // If a player has a score higher than the max score, the game is over
    if (scoreL >= maxScore)
    {
        win = true;
        textWinner.setFillColor(playerLColor);
        SetText(textWinner, winnerL);
        SetText(textReplay, strReplay);
    }
    else if (scoreR >= maxScore)
    {
        win = true;
        textWinner.setFillColor(playerRColor);
        SetText(textWinner, winnerR);
        SetText(textReplay, strReplay);
    }

    textWinner.setPosition(window_width / 2.f - textWinner.getLocalBounds().width / 2.f, 24);
    textReplay.setPosition(window_width / 2.f - textReplay.getLocalBounds().width / 2.f, 96);
}

// Toggle pause function
void TogglePause()
{
    pause = !pause;
    SetText(textPause, pause ? strPause : "");
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
        textScoreL.setPosition(14 - textScoreL.getLocalBounds().width / 2, window_height / 2.f - textScoreL.getLocalBounds().height / 2.f);
        textScoreR.setPosition(window_width - 20 - textScoreR.getLocalBounds().width / 2, window_height / 2.f - textScoreR.getLocalBounds().height / 2.f);

        ball = new Ball(ballRadius, ballPosX, ballPosY, ballColor);

        racketL = new Racket(racketLWidth, racketLHeight, racketLPosX, racketLPosY, playerLColor);
        racketR = new Racket(racketRWidth, racketRHeight, racketRPosX, racketRPosY, playerRColor);

        switch (defaultPlayer)
        {
        case PlayerLeft:
            currentDirection = Vector2f(-1.f, 0.f);
            break;
        case PlayerRight:
            currentDirection = Vector2f(1.f, 0.f);
        }
	}
}