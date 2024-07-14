# How it works ?

This project was made with Visual Studio 2022.
You can find all he settings in the main.h file.
For instance, if you want to change the speed of the rackets, you need to change these values.

racketL : Left racket
racketR : Right racket

```cpp
const float racketLSpeed{ 7 };
const float racketRSpeed{ 7 };
```

# Other settings

## Window settings

```cpp
const string game_name{ "Pong" };
const unsigned int window_width{ 800 };
const unsigned int window_height{ 600 };
const unsigned int frame_limit{ 60 };
```

**Important :** the frame limit influences game speed.

## Game settings

```cpp
const Color playerLColor{ Color::Blue };
const Color playerRColor{ Color::Red };
const Color ballColor{ Color::White };
const Player defaultPlayer = PlayerRight;
const unsigned int maxScore{ 10 };
const float racketLSpeed{ 7 };
const float racketRSpeed{ 7 };
const float defaultBallSpeed{ 11 };
// With each collision with a racket, the speed of the ball increases by this value
const float ballSpeedIncrease{ 0.15f };
```

## Volume
```cpp
// Racket collision sound
const float racketSoundVolume{ 10.f };
// Wall collision sound
const float wallSoundVolume{ 10.f };
```

## Racket properties
```cpp
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
```

## Ball properties
```cpp
const float ballRadius{ 12 };
// Default location of the ball
const unsigned int ballPosX{ static_cast<unsigned int>(window_width / 2.f - ballRadius / 2.f) };
const unsigned int ballPosY{ static_cast<unsigned int>(window_height / 2.f - ballRadius) };
```