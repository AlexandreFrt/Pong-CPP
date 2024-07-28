# Image of the game

![Image of the game](image.png "Image of the game")

# How to play ?

## Controls
**Left racket :**\
Move up : <kbd>Z</kbd>\
Move down : <kbd>S</kbd>

**Right racket :**\
Move up : <kbd>&uarr;</kbd>\
Move down : <kbd>&darr;</kbd>

Toggle pause : <kbd>Escape</kbd>

# How it works ?
> [!IMPORTANT]
> This project was made with Visual Studio 2022.
> You can find all he settings in the main.h file.\
> Therefore, you can change all the settings.

<br/>
For instance, if you want to change the speed of the rackets, you need to change these values.

```cpp
const float racketLSpeed{ 7.f };
const float racketRSpeed{ 7.f };
```

> [!NOTE]
> racketL : Left racket\
> racketR : Right racket


## Collision calculation
I used the Axis-Aligned Bounding Box (AABB) algorithm to calculate the intersections between the balls and the rackets or borders in 2D.

The boolean function : $f(A,B) = (A_{\text{minX}} \leq B_{\text{maxX}} \land A_{\text{maxX}} \geq B_{\text{minX}}) \land (A_{\text{minY}} \leq B_{\text{maxY}} \land A_{\text{maxY}} \geq B_{\text{minY}})$

![Collision calculation](https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection/screen_shot_2015-10-16_at_15.11.21.png "Collision calculation")\
![Collision calculation](https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection/aabb_test.png "Collision calculation")

```cpp
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
```

<br/>

> [!NOTE]
> If that interests you, you can read the article below.\
> https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection

# Main settings

## Window settings

```cpp
// Window properties
const string GAME_TITLE{ "Pong" };
const unsigned int WINDOW_WIDTH{ 800 };
const unsigned int WINDOW_HEIGHT{ 600 };
const unsigned int FRAME_LIMIT{ 60 };
```

> [!WARNING]  
> The frame limit influences game speed.

## Game settings

```cpp
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
```

## Volume
```cpp
// Sound properties (Volume)
// Racket collision sound
const float RACKET_SOUND_VOLUME{ 10.f };
// Wall collision sound
const float WALL_SOUND_VOLUME{ 10.f };
```

> [!NOTE]
> The volume is clamped between 0 and 100. There is no danger in changing it.

## Racket properties
```cpp
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
```

## Ball properties
```cpp
// Ball properties
const float BALL_RADIUS{ 12.f };
// Default location of the ball
const unsigned int DEFAULT_BALL_POS_X{ static_cast<unsigned int>(WINDOW_WIDTH / 2.f - BALL_RADIUS / 2.f) };
const unsigned int DEFAULT_BALL_POS_Y{ static_cast<unsigned int>(WINDOW_HEIGHT / 2.f - BALL_RADIUS) };
```
