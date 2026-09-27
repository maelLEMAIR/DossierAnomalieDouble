#ifndef SOCKET_CPP_DEFINED
#define SOCKET_CPP_DEFINED

#include "Socket.h"
#include "Data.h"

#define MAGIC_NUMBER 0xFF71FE3F

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI GetMSG(LPVOID lpParam)
{
    Socket* sock = (Socket*)lpParam;

    while (true)
    {
        sock->RecFrom();
    }

    return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Socket::Socket()
{
    mSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (mSock == INVALID_SOCKET)
    {
        std::cout << "Erreur creation socket : " << SOCKETS::GetError() << std::endl;
        return;
    }
    else
        std::cout << "Creation du socket" << std::endl;

    

    InitializeCriticalSection(&Profil);
    InitializeCriticalSection(&Message);

    LinkCmdFunc(Cmd::RECEIVE, [this](const char* data, std::string id, sockaddr_in* _from) {ReceptionConfirmation(data, id); });
    LinkCmdFunc(Cmd::PING, [this](const char* data, std::string id, sockaddr_in* _from) {ReceivePing(id); });
}

Socket::~Socket()
{
    SOCKETS::CloseSocket(mSock);
    DeleteCriticalSection(&Profil);
    DeleteCriticalSection(&Message);
}

void Socket::InitADDR(PCSTR path, int port)
{
    if (inet_pton(AF_INET, path, &mAddr.sin_addr) <= 0)
    {
        std::cout << "Fail to create addr" << std::endl;
        return;
    }
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(port);
}

void Socket::StartReading()
{
    HANDLE thread = CreateThread(NULL, NULL, GetMSG, this, NULL, NULL);
}

char* Socket::RecFrom(const char* buf)
{
    if (!mCanRecv) return 0;

    char data[1024 + 1];

    int received = 0;
    sockaddr_in from;
    int fromlen = sizeof(from);
    //std::cout << "start receive" << std::endl;
    received = recvfrom(mSock, data, 1024, 0, (sockaddr*)&from, &fromlen);
    if (received == SOCKET_ERROR) {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }
    else
    {
        //std::cout << "receive data" << std::endl;
        data[received] = 0;
        CheckData(&from, data);
    }
}

void Socket::SendTo(Data data, std::string to)
{
    EnterCriticalSection(&Profil);
    if (m_mProfils.contains(to))
        m_mProfils[to].mMsg.push_back(data);
    else
        std::cout << "Try to send to a profil not initialize" << std::endl;
    LeaveCriticalSection(&Profil);
}

void Socket::SendSecurTo(Data data, std::string to)
{
    EnterCriticalSection(&Profil);
    if (m_mProfils.contains(to))
        m_mProfils[to].mSecureMsg.push_back(data);
    else
        std::cout << "Try to secur send to a profil not initialize" << std::endl;
    LeaveCriticalSection(&Profil);
}

void Socket::Update(float dt)
{
    if (usePing)
    {
        for (auto& [id, profil] : m_mProfils)
        {
            Data ping;
            ping.Init(Cmd::PING);

            SendTo(ping, id);
        }
    }

    EnterCriticalSection(&Profil);
    FinalSend();
    FinalSecureSend();
    UpdateWaitingMsg(dt);
    if (usePing)
        UpdatePing(dt);
    LeaveCriticalSection(&Profil);
    CheckMsg();
}

void Socket::FinalSend()
{
    for (auto& [key, data] : m_mProfils)
    {
        const char* id = key.c_str();
        Send(id, data);
        data.mMsg.clear();
    }

    if (!mCanRecv)
        mCanRecv = true;
}

void Socket::FinalSecureSend()
{
    for (auto& [key, data] : m_mProfils)
    {
        const char* id = key.c_str();
        Send(id, data, true);
        data.mSecureMsg.clear();
    }

    //clear les securmsg

    if (!mCanRecv)
        mCanRecv = true;
}

void Socket::Send(const char* key, ProfilData& data, bool isSecure)
{
    size_t var_len = 0;

    std::vector<Data> msg;

    if (isSecure)
        msg = data.mSecureMsg;
    else
        msg = data.mMsg;

    for (Data tdata : msg)
    {
        var_len += tdata.GetSize();
    }

    if (var_len == 0) return;

    uint32_t magic = MAGIC_NUMBER;

    int sizeHeaders = 4 + 12 + 1 + 1 + 1;

    size_t buf_len = var_len + sizeHeaders;

    std::vector<char*> buffers;

    float nbB = (float)buf_len / 1024.f;

    for (int i = 0; i < nbB; i++)
    {
        char* buf = new char[buf_len];
        buffers.push_back(buf);
    }

    size_t offset = 0;
    char* currentBuf = buffers[0];
    int currentIndex = 0;

    for (char* buff : buffers)
    {
        memcpy(buff + offset, &magic, 4);
    }
    offset += 4;

    //ip
    for (char* buff : buffers)
    {
        memcpy(buff + offset, &*mId, 12);
    }
    offset += 12;

    //id message
    int startId = data.idMsg;
    for (char* buff : buffers)
    {
        int id = data.idMsg;

        data.idMsg++;

        if (data.idMsg == 256)
            data.idMsg = 1;

        memcpy(buff + offset, &id, 1);
    }
    offset++;

    for (char* buff : buffers)
    {
        memcpy(buff + offset, &isSecure, 1);
    }
    offset++;

    for (Data tdata : msg)
    {
        if (offset + tdata.GetSize() >= 1023)
        {
            currentBuf = buffers[currentIndex + 1];
            currentIndex++;
            offset = sizeHeaders - 1;
        }

        memcpy(currentBuf + offset, tdata.GetByte(), tdata.GetSize());
        offset += tdata.GetSize();
    }

    for (char* buf : buffers)
    {
        //add cmd end before send
        int cmd = Cmd::END;
        memcpy(buf + buf_len - 1, &cmd, 1);

        int send = sendto(mSock, buf, (int)buf_len, 0, (sockaddr*)&m_mProfils[key], sizeof(m_mProfils[key]));
        if (send == SOCKET_ERROR)
            std::cout << "Failed to sent" << std::endl;

        if (isSecure)
        {
            data.m_vWaitingMsg.push_back(WaitingMsg(startId, buf, (int)buf_len));
            startId++;

            if (startId == 256)
                startId = 1;
        }
        else
            delete[] buf;
    }
}

void Socket::UpdateWaitingMsg(float dt)
{
    for (auto& [key, data] : m_mProfils)
    {
        for (WaitingMsg& msg : data.m_vWaitingMsg)
        {
            msg.waitProgress -= dt;
            msg.lifeTime -= dt;

            if (msg.waitProgress <= 0)
            {
                int send = sendto(mSock, msg.msg, msg.size, 0, (sockaddr*)&m_mProfils[key], sizeof(m_mProfils[key]));
                if (send == SOCKET_ERROR)
                    std::cout << "Failed to resend" << '\n';

                msg.waitProgress = msg.waitTime;
            }

            if (msg.lifeTime <= 0)
            {
                std::cout << "Failed to send/receive an importante message : Lost of connexion" << '\n';
            }
        }
    }
}

void Socket::UpdatePing(float _dt)
{
    for (auto& [id, profil] : m_mProfils)
    {
        profil.timeSinceLastPing += _dt;

        if (profil.timeSinceLastPing >= 10.0f)
            profil.connectionLost = true;
    }
}

void Socket::CheckMsg()
{
    EnterCriticalSection(&Message);

    for (MsgData msgData : m_vMsgData)
    {
        if (m_mFunction.contains(msgData.cmd))
        {
            m_mFunction[msgData.cmd](msgData.data, msgData.id, &msgData.from);
        }
        else
            std::cout << "Receive a cmd but there is no function link to it" << '\n';

        delete msgData.data;
    }

    m_vMsgData.clear();

    LeaveCriticalSection(&Message);
}

void Socket::ReceivePing(std::string _id)
{
    if (m_mProfils.contains(_id))
    {
        m_mProfils[_id].timeSinceLastPing = 0.0f;
    }
}

void Socket::CheckData(sockaddr_in* from, char data[1024 + 1])
{
    int offset = 0;

    //Check magic number
    char magic_number[4];

    memcpy(magic_number, data, 4);

    char test[4];
    uint32_t magic = MAGIC_NUMBER;
    memcpy(test, &magic, 4);
    offset += 4;

    if (*magic_number != *test) return;

    //check id of from
    char id[13];
    memcpy(&id, data + offset, 12);
    id[12] = 0;
    offset += 12;

    //check idMsg
    int idMsg = 0;
    memcpy(&idMsg, data + offset, 1);
    offset++;

    bool isSecure = false;
    memcpy(&isSecure, data + offset, 1);
    offset++;

    if (m_mProfils.contains(id) == false)
    {
        CreateProfils(id, *from);
    }

    if (isSecure)
    {
        std::cout << "Receive secure msg" << '\n';

        //Send confirmation
        Data d;
        d.Init(Cmd::RECEIVE, { &idMsg }, { Type::TYPE_INT1 });

        SendTo(d, id);
    }
    EnterCriticalSection(&Profil);
    int diff = idMsg - m_mProfils[id].lastIdMsg;
    m_mProfils[id].lastIdMsg = idMsg;
    LeaveCriticalSection(&Profil);

    if (diff >= 0 && diff < 128)
        ReadCMD(id, data, from);
    //else
        //std::cout << std::endl; //debug
}

void Socket::CreateProfils(std::string id, sockaddr_in addr)
{
    EnterCriticalSection(&Profil);
    if (m_mProfils.size() < maxProfil || maxProfil == -1)
        m_mProfils[id].ip = addr;
    else
        std::cout << "Failed to create new profil because the max amount of profil have been created" << '\n';
    LeaveCriticalSection(&Profil);
}

void Socket::DeleteProfils(std::string id)
{
    EnterCriticalSection(&Profil);
    m_mProfils.erase(id);
    LeaveCriticalSection(&Profil);
}

bool Socket::ProfilIsConnected(char* id)
{
    if (m_mProfils.contains(id))
    {
        return !m_mProfils[id].connectionLost;
    }
}

void Socket::LinkCmdFunc(Cmd::Type cmd, std::function<void(const char*, std::string, sockaddr_in*)> function)
{
    m_mFunction[cmd] = function;
}

void Socket::ReadCMD(char* id, char data[1024 + 1], sockaddr_in* from)
{
    int offset = 18;

    int cmd = 0;
    int size = 0;

    memcpy(&cmd, data + offset, 1);
    offset++;
    memcpy(&size, data + offset, 4);
    offset += 4;

    std::vector<MsgData> temp;

    while (cmd < Cmd::END) //to avoid crash if problem with offset
    {
        char* value = nullptr;
        if (size != 0)
        {
            value = new char[size];
            memcpy(value, data + offset, size);
            offset += size;
        }

        MsgData msgData;
        msgData.cmd = cmd;
        msgData.data = value;
        msgData.id = id;
        msgData.from = *from;

        temp.push_back(msgData);

        std::cout << "cmd add : " << cmd << std::endl;

        memcpy(&cmd, data + offset, 1);
        offset++;
        memcpy(&size, data + offset, 4);
        offset += 4;
    }

    EnterCriticalSection(&Message);
    for (MsgData msgData : temp)
    {
        m_vMsgData.push_back(msgData);
    }
    LeaveCriticalSection(&Message);
}

void Socket::ConnectPlayer(sockaddr_in* from, char data[1])
{
    int id = 0;

    memcpy(&id, data, 1);
}

void Socket::ExtractPosPlayer(char data[12])
{
}

void Socket::ReceptionConfirmation(const char* data, std::string id)
{
    int idMsg = 0;
    memcpy(&idMsg, data, 1);

    int i = 0;
    EnterCriticalSection(&Profil);
    for (const WaitingMsg& msg : m_mProfils[id].m_vWaitingMsg)
    {
        if (msg.id == idMsg)
        {
            m_mProfils[id].m_vWaitingMsg.erase(m_mProfils[id].m_vWaitingMsg.begin() + i);
            LeaveCriticalSection(&Profil);
            return;
        }

        i++;
    }
    LeaveCriticalSection(&Profil);
}

#endif