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

#include "ball.h"

// Constructor
Ball::Ball(float radius, int posX, int posY, Color color)
{
	circle = CircleShape(radius);
	circle.setPosition(static_cast<float>(posX), static_cast<float>(posY));
	circle.setFillColor(color);
}

// Move function
void Ball::Move(Vector2f direction, float speed)
{
    if (canMove)
    {
        circle.setPosition(circle.getPosition().x + direction.x * speed, circle.getPosition().y + direction.y * -1 * speed);
        currentDirection = direction;
    }
}

// Return the circle
CircleShape Ball::GetShape()
{
	return circle;
}

Vector2f Ball::GetPosition()
{
	return circle.getPosition();
}

void Ball::SetPosition(Vector2f position)
{
    canMove = false;
    circle.setPosition(position);
    canMove = true;
}
