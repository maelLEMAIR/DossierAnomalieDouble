#ifndef INPUT_SYSTEM_CPP_INCLUDED
#define INPUT_SYSTEM_CPP_INCLUDED

#include "InputSystem.h"


UnorderedMap<UINT8, INT32> InputSystem::s_keyboardMap{
    { BACKSPACE,       VK_BACK     },
    { TAB,             VK_TAB      },
    { RETURN,          VK_RETURN   },
    { PAUSE,           VK_PAUSE    },
    { CAPSLOCK,        VK_CAPITAL  },
    { ESCAPE,          VK_ESCAPE   },
    { SPACE,           VK_SPACE    },
    { PAGE_UP,         VK_PRIOR    },
    { PAGE_DOWN,       VK_NEXT     },
    { END,             VK_END      },
    { HOME,            VK_HOME     },
    { LEFT,            VK_LEFT     },
    { UP,              VK_UP       },
    { RIGHT,           VK_RIGHT    },
    { DOWN,            VK_DOWN     },
    { INSERT,          VK_INSERT   },
    { DELETE_,         VK_DELETE   },
    { LWINDOW,         VK_LWIN     },
    { RWINDOW,         VK_RWIN     },
    { NUMPAD0,         VK_NUMPAD0  },
    { NUMPAD1,         VK_NUMPAD1  },
    { NUMPAD2,         VK_NUMPAD2  },
    { NUMPAD3,         VK_NUMPAD3  },
    { NUMPAD4,         VK_NUMPAD4  },
    { NUMPAD5,         VK_NUMPAD5  },
    { NUMPAD6,         VK_NUMPAD6  },
    { NUMPAD7,         VK_NUMPAD7  },
    { NUMPAD8,         VK_NUMPAD8  },
    { NUMPAD9,         VK_NUMPAD9  },
    { NUMPAD_MULTIPLY, VK_MULTIPLY },
    { NUMPAD_ADD,      VK_ADD      },
    { NUMPAD_SUBTRACT, VK_SUBTRACT },
    { NUMPAD_DECIMAL,  VK_DECIMAL  },
    { NUMPAD_DIVIDE,   VK_DIVIDE   },
    { F1,              VK_F1       },
    { F2,              VK_F2       },
    { F3,              VK_F3       },
    { F4,              VK_F4       },
    { F5,              VK_F5       },
    { F6,              VK_F6       },
    { F7,              VK_F7       },
    { F8,              VK_F8       },
    { F9,              VK_F9       },
    { F10,             VK_F10      },
    { F11,             VK_F11      },
    { F12,             VK_F12      },
    { NUMLOCK,         VK_NUMLOCK  },
    { SCROLL_LOCK,     VK_SCROLL   },
    { LSHIFT,          VK_LSHIFT   },
    { RSHIFT,          VK_RSHIFT   },
    { LCONTROL,        VK_LCONTROL },
    { RCONTROL,        VK_RCONTROL },
    { LALT,            VK_LMENU    },
    { RALT,            VK_RMENU    },
    { A,              'A'          },
    { B,              'B'          },
    { C,              'C'          },
    { D,              'D'          },
    { E,              'E'          },
    { F,              'F'          },
    { G,              'G'          },
    { H,              'H'          },
    { I,              'I'          },
    { J,              'J'          },
    { K,              'K'          },
    { L,              'L'          },
    { M,              'M'          },
    { N,              'N'          },
    { O,              'O'          },
    { P,              'P'          },
    { Q,              'Q'          },
    { R,              'R'          },
    { S,              'S'          },
    { T,              'T'          },
    { U,              'U'          },
    { V,              'V'          },
    { W,              'W'          },
    { X,              'X'          },
    { Y,              'Y'          },
    { Z,              'Z'          },
    { _0,             '0'          },
    { _1,             '1'          },
    { _2,             '2'          },
    { _3,             '3'          },
    { _4,             '4'          },
    { _5,             '5'          },
    { _6,             '6'          },
    { _7,             '7'          },
    { _8,             '8'          },
    { _9,             '9'          },
    { ²,              VK_OEM_7     },
};

UnorderedMap<UINT8, INT32> InputSystem::s_mouseMap{
    { LEFT_MOUSE,   VK_LBUTTON  },
    { RIGHT_MOUSE,  VK_RBUTTON  },
    { MIDDLE_MOUSE, VK_MBUTTON  }
};

String InputSystem::s_typedChars = "";

void InputSystem::OnInit()
{
    s_pHWND = EngineManager::GetWindow()->GetHWND();
    s_typedChars = "";
}

void InputSystem::HandleInput()
{
    for (const Pair<unsigned char, int> input : s_keyboardMap)
    {
        unsigned char inputKey = input.first;
        int indexKey = input.second;

        bool isKeyDown = (GetAsyncKeyState(indexKey) & 0x8000) != 0;
        InputState currentState = s_keyboardStates[inputKey];

        if (isKeyDown)
        {
            if (currentState == DOWN_STATE || currentState == PRESSED_STATE)
                s_keyboardStates[inputKey] = PRESSED_STATE;
            else
                s_keyboardStates[inputKey] = DOWN_STATE;
        }
        else
        {
            if (currentState == DOWN_STATE || currentState == PRESSED_STATE)
                s_keyboardStates[inputKey] = UP_STATE;
            else
                s_keyboardStates[inputKey] = NONE;
        }
    }

    for (const Pair<unsigned char, int> input : s_mouseMap)
    {
        unsigned char inputButton = input.first;
        int indexButton = input.second;

        bool isButtonDown = (GetAsyncKeyState(indexButton) & 0x8000) != 0;
        InputState currentState = s_mouseStates[inputButton];

        if (isButtonDown)
        {
            if (currentState == DOWN_STATE || currentState == PRESSED_STATE)
                s_mouseStates[inputButton] = PRESSED_STATE;
            else
                s_mouseStates[inputButton] = DOWN_STATE;
        }
        else
        {
            if (currentState == DOWN_STATE || currentState == PRESSED_STATE)
                s_mouseStates[inputButton] = UP_STATE;
            else
                s_mouseStates[inputButton] = NONE;
        }
    }
}

void InputSystem::Update(float _dt)
{
    HandleInput();

    if (IsKeyDown(InputKeyboard::BACKSPACE))
    {
        if (s_typedChars.length() > 0)
        {
            s_typedChars.pop_back();
        }
    }

    if (IsKeyDown(InputKeyboard::SPACE))
    {
        s_typedChars += ' ';
    }

    if (IsKeyDown(InputKeyboard::NUMPAD_DECIMAL))
    {
        s_typedChars += '.';
    }

    // A - Z
    for (int i = InputKeyboard::A; i <= InputKeyboard::Z; ++i)
    {
        if (IsKeyDown(static_cast<InputKeyboard>(i)))
        {
            bool shiftDown = false;

            if (IsKeyDown(InputKeyboard::LSHIFT)) shiftDown = true;
            if (IsKeyPressed(InputKeyboard::LSHIFT)) shiftDown = true;
            if (IsKeyDown(InputKeyboard::RSHIFT)) shiftDown = true;
            if (IsKeyPressed(InputKeyboard::RSHIFT)) shiftDown = true;

            char c = 'a';
            if (shiftDown)
            {
                c = 'A' + (i - InputKeyboard::A);
            }
            else
            {
                c = 'a' + (i - InputKeyboard::A);
            }
            s_typedChars += c;
        }
    }

    // 0 - 9 
    for (int i = InputKeyboard::_0; i <= InputKeyboard::_9; ++i)
    {
        if (IsKeyDown(static_cast<InputKeyboard>(i)))
        {
            char c = '0' + (i - InputKeyboard::_0);
            s_typedChars += c;
        }
    }

    // 0 - 9 (NUMPAD)
    for (int i = InputKeyboard::NUMPAD0; i <= InputKeyboard::NUMPAD9; ++i)
    {
        if (IsKeyDown(static_cast<InputKeyboard>(i)))
        {
            char c = '0' + (i - InputKeyboard::NUMPAD0);
            s_typedChars += c;
        }
    }
}

bool InputSystem::IsKeyPressed(InputKeyboard key)
{
    return s_keyboardStates[key] == PRESSED_STATE;
}

bool InputSystem::IsKeyUp(InputKeyboard key)
{
    return s_keyboardStates[key] == UP_STATE;
}

bool InputSystem::IsKeyDown(InputKeyboard key)
{
    return s_keyboardStates[key] == DOWN_STATE;
}

bool InputSystem::IsMouseButtonPressed(InputMouse key)
{
    return s_mouseStates[key] == PRESSED_STATE;
}

bool InputSystem::IsMouseButtonUp(InputMouse key)
{
    return s_mouseStates[key] == UP_STATE;
}

bool InputSystem::IsMouseButtonDown(InputMouse key)
{
    return s_mouseStates[key] == DOWN_STATE;
}

XMINT2 InputSystem::GetMousePosition()
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(s_pHWND, &p);
    return { p.x, p.y };
}

XMINT2 InputSystem::GetMousePositionCenter()
{
    XMINT2 pos = GetMousePosition();
    pos.x -= EngineManager::GetWindow()->GetWidth() / 2;
    pos.y -= EngineManager::GetWindow()->GetHeight() / 2;

    return pos;
}

void InputSystem::SetMousePosition(XMINT2 const& coordinates)
{
    POINT p{ coordinates.x, coordinates.y };
    ClientToScreen(s_pHWND, &p);
    SetCursorPos(p.x, p.y);
}

void InputSystem::LockMouseCursor()
{
    if (s_pHWND == nullptr) return;
    s_cursorLocked = true;

    if (s_cursorLocked == false || s_pHWND == nullptr) return;

    RECT clientRect;
    if (GetClientRect(s_pHWND, &clientRect) == false) return;

    POINT topLeft = { clientRect.left, clientRect.top };
    POINT bottomRight = { clientRect.right, clientRect.bottom };

    ClientToScreen(s_pHWND, &topLeft);
    ClientToScreen(s_pHWND, &bottomRight);

    RECT const clipRect = { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
    ClipCursor(&clipRect);
}

void InputSystem::UnlockMouseCursor()
{
    s_cursorLocked = false;
    ClipCursor(nullptr);
}

void InputSystem::ShowMouseCursor()
{
    if (s_cursorVisible) return;
    s_cursorVisible = true;

    while (s_cursorVisibilityCount < 0)
        s_cursorVisibilityCount = ShowCursor(TRUE);
}

void InputSystem::HideMouseCursor()
{
    if (s_cursorVisible == false) return;
    s_cursorVisible = false;

    while (s_cursorVisibilityCount >= 0)
        s_cursorVisibilityCount = ShowCursor(FALSE);
}

const String InputSystem::GetTypedChar()
{
    return s_typedChars;
}

void InputSystem::ClearTypedChars()
{
    s_typedChars.clear();
}

#endif