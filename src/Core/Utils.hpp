#ifndef UTILS_HPP_INCLUDED
#define UTILS_HPP_INCLUDED

#include "define.h"

inline XMFLOAT3 ToColor(XMINT3 _color)
{
    XMFLOAT3 color;
    color.x = Clamp((float)_color.x, 0, 255)/255.0f;
    color.y = Clamp((float)_color.y, 0, 255)/255.0f;
    color.z = Clamp((float)_color.z, 0, 255)/255.0f;
    return color;
}

inline XMFLOAT3 ToColor(int _r, int _g, int _b)
{
    XMFLOAT3 color;
    color.x = Clamp((float)_r, 0, 255)/255.0f;
    color.y = Clamp((float)_g, 0, 255)/255.0f;
    color.z = Clamp((float)_b, 0, 255)/255.0f;
    return color;
}

inline bool FindTheWord(String const& _seqWord, String const& _word)
{
    if (_word.empty()) return true;
    if (_seqWord.length() < _word.length()) return false;

    for (size_t i = 0; i < _seqWord.length(); ++i)
    {
        if (_seqWord[i] != _word[0]) continue;
        for (size_t j = 0; j < _word.length(); ++j)
        {
            if (i + j >= _seqWord.length()) return false;
            if (_seqWord[i + j] != _word[j]) break;
            if (j == _word.length() - 1) return true;
        }
    }
    return false;
}

#endif
