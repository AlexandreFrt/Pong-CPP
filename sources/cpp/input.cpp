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

#include "input.h"

// Constructor
Input::Input()
{
    button.up = false;
    button.down = false;
    button.Z = false;
    button.S = false;
    button.escape = false;
    button.space = false;
}

// Return the button
Input::Button Input::GetButton() const
{
    return button;
}

void Input::InputHandler(const Event& event, RenderWindow& window)
{
    if (event.type == Event::Closed)
    {
        window.close();
    }

    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case Keyboard::Escape:
            if (!button.escape) // Execute action only once per press
            {
                button.escape = true;
            }
            break;
        case Keyboard::Up:
            button.up = true;
            break;
        case Keyboard::Down:
            button.down = true;
            break;
        case Keyboard::Z:
            button.Z = true;
            break;
        case Keyboard::S:
            button.S = true;
            break;
        case Keyboard::Space:
            button.space = true;
            break;
        default:
            break;
        }
    }
    else if (event.type == Event::KeyReleased)
    {
        switch (event.key.code)
        {
        case Keyboard::Escape:
            button.escape = false;
            break;
        case Keyboard::Up:
            button.up = false;
            break;
        case Keyboard::Down:
            button.down = false;
            break;
        case Keyboard::Z:
            button.Z = false;
            break;
        case Keyboard::S:
            button.S = false;
            break;
        case Keyboard::Space:
            button.space = false;
            break;
        default:
            break;
        }
    }
}

void Input::ResetButtons()
{
    // Only reset the escape button and the space bar to allow repeated actions for other buttons
    button.escape = false;
    button.space = false;
}
