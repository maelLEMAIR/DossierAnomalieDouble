#ifndef DATA_H_DEFINED
#define DATA_H_DEFINED

#include "Socket.h"

enum Type {
    TYPE_INT1,
    TYPE_INT4,
    TYPE_XMFLOAT3,
    TYPE_XMFLOAT4,
    TYPE_BOOL,
    TYPE_FLOAT,
    TYPE_PTR
};

class Data 
{
private:
    Cmd::Type mCmd;
    std::vector<Type> m_vType;
    const char* mByte;
    int mSize;

public:
    void Init(Cmd::Type cmd, std::vector<void*> data = {}, std::vector<Type> type = {});

    const char* GetByte() { return mByte; }
    int GetSize() { return mSize; }
};

#endif