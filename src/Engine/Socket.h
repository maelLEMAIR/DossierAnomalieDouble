#ifndef SOCKET_H_DEFINED
#define SOCKET_H_DEFINED

#include <iostream>
#include "SOCKETS.h"
#include <Windows.h>

#include <functional>
#include <map>
#include <string>

namespace Cmd
{
    enum Type {
        NONE,
        CONNECT,
        PING,
        RECEIVE,
        START,
        SHOW_PING,
        STATE_LEVER,
        START_ROOM,
        ID_NEXT_ROOM,
        START_REVEALE_ROOM,
        RESTART,
        END
    };
}

struct WaitingMsg 
{
    int id = 0;
    char* msg = nullptr;
    int size = 0;
    float waitTime = 10;
    float waitProgress = 10;
    float lifeTime = 60;
};

struct MsgData
{
    int cmd;
    const char* data;
    std::string id;
    sockaddr_in from;
};

class Data;

class ProfilData
{
public:
    sockaddr_in ip;
    std::vector<Data> mMsg;
    std::vector<Data> mSecureMsg;
    std::vector<WaitingMsg> m_vWaitingMsg;
    int idMsg = 1; // 0 < idMsg < 256
    int lastIdMsg = 0;
    float timeSinceLastPing = 0.0f;
    bool connectionLost = false;
};

class Socket
{
private:
    std::map<std::string, ProfilData> m_mProfils;

    std::map<int, std::function<void(const char*, std::string, sockaddr_in*)>> m_mFunction;

    CRITICAL_SECTION Profil;
    CRITICAL_SECTION Message;

    int maxProfil = -1; //no limite

    std::vector<MsgData> m_vMsgData;

protected:
    char* mId;

    bool mCanRecv = false;

public:
    SOCKET mSock;
    sockaddr_in mAddr;

    Socket();

    ~Socket();

    virtual void InitADDR(PCSTR path, int port);
    void SetId(char* id) { mId = id; }
    char* GetId() { return mId; }
    void StartReading();

    char* RecFrom(const char* buf = "");
    void SendTo(Data data, std::string to);
    void SendSecurTo(Data data, std::string to);
    void FinalSecureSend();
    void Update(float dt);

    void CheckData(sockaddr_in* from, char data[1024+1]);

    void CreateProfils(std::string id, sockaddr_in addr);
    void DeleteProfils(std::string id);
    bool ProfilIsConnected(char* id);

    void LinkCmdFunc(Cmd::Type cmd, std::function<void(const char*, std::string, sockaddr_in*)> function);

    bool usePing = true;

private:
    void ReadCMD(char* id, char data[1024 + 1], sockaddr_in* from);

    void ConnectPlayer(sockaddr_in* from, char data[1]);
    void ExtractPosPlayer(char data[12]);
    void ReceptionConfirmation(const char* data, std::string id);

    void FinalSend();
    void Send(const char* id, ProfilData& data, bool isSecure = false);
    void UpdateWaitingMsg(float dt);
    void UpdatePing(float _dt);
    void CheckMsg();

    void ReceivePing(std::string _id);
};

#endif