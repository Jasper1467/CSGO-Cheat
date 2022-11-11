#pragma once
#include "bitbuf.h"
#include "INetMessage.h"

#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define MAX_FLOWS 2

#define CONNECTION_PROBLEM_TIME 4.0f

#define FLOW_INTERVAL 0.25f
#define FLOW_AVG 0.75f

#define NET_FRAMES_BACKUP 128
#define NET_FRAMES_MASK (NET_FRAMES_BACKUP - 1)

#define NET_RATE_MIN 16000.f
#define NET_RATE_MAX 786432.f

class INetChannelInfo
{
public:
    enum
    {
        GENERIC = 0x0,
        LOCALPLAYER = 0x1,
        OTHERPLAYERS = 0x2,
        ENTITIES = 0x3,
        SOUNDS = 0x4,
        EVENTS = 0x5,
        TEMPENTS = 0x6,
        USERMESSAGES = 0x7,
        ENTMESSAGES = 0x8,
        VOICE = 0x9,
        STRINGTABLE = 0xA,
        MOVE = 0xB,
        STRINGCMD = 0xC,
        SIGNON = 0xD,
        PAINTMAP = 0xE,
        ENCRYPTED = 0xF,
        TOTAL = 0x10,
    };

    virtual const char* GetName(void) const = 0;	// get channel name
    virtual const char* GetAddress(void) const = 0; // get channel IP address as string
    virtual float		GetTime(void) const = 0;	// current net time
    virtual float		GetTimeConnected(void) const = 0;	// get connection time in seconds
    virtual int			GetBufferSize(void) const = 0;	// netchannel packet history size
    virtual int			GetDataRate(void) const = 0; // send data rate in byte/sec

    virtual bool		IsLoopback(void) const = 0;	// true if loopback channel
    virtual bool		IsTimingOut(void) const = 0;	// true if timing out
    virtual bool		IsPlayback(void) const = 0;	// true if demo playback

    virtual float		GetLatency(int flow) const = 0;	 // current latency (RTT), more accurate but jittering
    virtual float		GetAvgLatency(int flow) const = 0; // average packet latency in seconds
    virtual float		GetAvgLoss(int flow) const = 0;	 // avg packet loss[0..1]
    virtual float		GetAvgChoke(int flow) const = 0;	 // avg packet choke[0..1]
    virtual float		GetAvgData(int flow) const = 0;	 // data flow in bytes/sec
    virtual float		GetAvgPackets(int flow) const = 0; // avg packets/sec
    virtual int			GetTotalData(int flow) const = 0;	 // total flow in/out in bytes
    virtual int			GetSequenceNr(int flow) const = 0;	// last send seq number
    virtual bool		IsValidPacket(int flow, int frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
    virtual float		GetPacketTime(int flow, int frame_number) const = 0; // time when packet was send
    virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0; // group size of this packet
    virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;  // TCP progress if transmitting
    virtual float		GetTimeSinceLastReceived(void) const = 0;	// get time since last recieved packet in seconds
    virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
    virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
    virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

    virtual float		GetTimeoutSeconds() const = 0;
};

struct ns_address;
class IDemoRecorder;
class INetChannel : public INetChannelInfo
{
public:
    virtual	~INetChannel(void) {};

    virtual void	SetDataRate(float rate) = 0;

    virtual bool	RegisterMessage(INetMessageBinder* msg) = 0;
    virtual bool	UnregisterMessage(INetMessageBinder* msg) = 0;

    virtual bool	StartStreaming(unsigned int challengeNr) = 0;
    virtual void	ResetStreaming(void) = 0;
    virtual void	SetTimeout(float seconds, bool bForceExact = false) = 0;
    virtual void	SetDemoRecorder(IDemoRecorder* recorder) = 0;
    virtual void	SetChallengeNr(unsigned int chnr) = 0;

    virtual void	Reset(void) = 0;
    virtual void	Clear(void) = 0;
    virtual void	Shutdown(const char* reason) = 0;

    virtual void	ProcessPlayback(void) = 0;
    virtual bool	ProcessStream(void) = 0;
    virtual void	ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;

    virtual bool	SendNetMsg(INetMessage& msg, bool bForceReliable = false, bool bVoice = false) = 0;
    virtual bool	SendData(bf_write& msg, bool bReliable = true) = 0;
    virtual bool	SendFile(const char* filename, unsigned int transferID, bool bIsReplayDemoFile) = 0;
    virtual void	DenyFile(const char* filename, unsigned int transferID, bool bIsReplayDemoFile) = 0;
    virtual void	RequestFile_OLD(const char* filename, unsigned int transferID) = 0;	// get rid of this function when we version the 
    virtual void	SetChoked(void) = 0;
    virtual int		SendDatagram(bf_write* data) = 0;
    virtual bool	Transmit(bool onlyReliable = false) = 0;

    virtual const ns_address& GetRemoteAddress(void) const = 0;
    virtual INetChannelHandler* GetMsgHandler(void) const = 0;
    virtual int				GetDropNumber(void) const = 0;
    virtual int				GetSocket(void) const = 0;
    virtual unsigned int	GetChallengeNr(void) const = 0;
    virtual void			GetSequenceData(int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck) = 0;
    virtual void			SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;

    virtual void	UpdateMessageStats(int msggroup, int bits) = 0;
    virtual bool	CanPacket(void) const = 0;
    virtual bool	IsOverflowed(void) const = 0;
    virtual bool	IsTimedOut(void) const = 0;
    virtual bool	HasPendingReliableData(void) = 0;

    virtual void	SetFileTransmissionMode(bool bBackgroundMode) = 0;
    virtual void	SetCompressionMode(bool bUseCompression) = 0;
    virtual unsigned int RequestFile(const char* filename, bool bIsReplayDemoFile) = 0;
    virtual float	GetTimeSinceLastReceived(void) const = 0;	// get time since last received packet in seconds

    virtual void	SetMaxBufferSize(bool bReliable, int nBytes, bool bVoice = false) = 0;

    virtual bool	IsNull() const = 0;
    virtual int		GetNumBitsWritten(bool bReliable) = 0;
    virtual void	SetInterpolationAmount(float flInterpolationAmount) = 0;
    virtual void	SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation, float flFrameStartTimeStdDeviation) = 0;

    // Max # of payload bytes before we must split/fragment the packet
    virtual void	SetMaxRoutablePayloadSize(int nSplitSize) = 0;
    virtual int		GetMaxRoutablePayloadSize() = 0;

    // For routing messages to a different handler
    virtual bool	SetActiveChannel(INetChannel* pNewChannel) = 0;
    virtual void	AttachSplitPlayer(int nSplitPlayerSlot, INetChannel* pChannel) = 0;
    virtual void	DetachSplitPlayer(int nSplitPlayerSlot) = 0;

    virtual bool	IsRemoteDisconnected() const = 0;

    virtual bool	WasLastMessageReliable() const = 0; // True if the last (or currently processing) message was sent via the reliable channel

    virtual const unsigned char* GetChannelEncryptionKey() const = 0;	// Returns a buffer with channel encryption key data (network layer determines the buffer size)

    virtual bool	EnqueueVeryLargeAsyncTransfer(INetMessage& msg) = 0;	// Enqueues a message for a large async transfer
};

enum PacketFlags_e
{
    PACKET_FLAG_RELIABLE = 0x1,
};

struct netpacket_t
{
    char pad[52];
    bf_read m_Message;
    char pad1[35];
    int m_nSize;
    int m_nWireSize;
};

class CNetChan : public INetChannel
{
public:
    struct netframe_header_t
    {
        float m_flTime;
        int m_nSize;
        __int16 m_nChoked;
        bool m_bValid;
        float m_flLatency;
    };

    struct netframe_t
    {
        int m_nDropped;
        float m_flAvgLatency;
        float m_flInterpolationAmount;
        unsigned __int16 m_nMsgGroups[INetChannelInfo::TOTAL];
    };

    struct netflow_t
    {
        float m_flNextCompute;
        float m_flAvgBytesPerSecond;
        float m_flAvgPacketsPerSecond;
        float m_flAvgLoss;
        float m_flAvgChoke;
        float m_flAvgLatency;
        float m_flLatency;
        int m_nTotalPackets;
        int m_nTotalBytes;
        int m_nCurrentIndex;
        netframe_header_t m_FrameHeaders[NET_FRAMES_BACKUP];
        netframe_t m_Frames[NET_FRAMES_BACKUP];
        netframe_t* m_pCurrentFrame;
    };

    char pad[20];
    bool m_bProcessingMessages;
    bool m_bShouldDelete;
    bool m_bStopProcessing;
    int m_nOutSequenceNr;
    int m_nInSequenceNr;
    int m_nOutSequenceNrAck;
    int m_nOutReliableState;
    int m_nInReliableState;
    int m_nChokedPackets;
    char pad0[108];
    int m_nSocket;
    char padqzdqzd[4];
    bool m_bWasLastMessageReliable;
    char pad1[100];
    float m_flLastReceived;
    double m_dbConnectTime;
    int m_nRate;
    double m_dbClearTime;
    char pad2[872];
    unsigned int m_nFileRequestCounter;
    char pad3[5];
    bool m_bCompress;
    bool m_bStreamActive;
    netflow_t m_DataFlow[MAX_FLOWS];
    char padqzdzqd[104];
    float m_flTimeOut;
    char pad4[32];
    float m_flRemoteFrameTime;
    float m_flRemoteFrameTimeStdDeviation;
    float m_flRemoteFrameStartTimeStdDeviation;
    int m_nMaxRoutablePayloadSize;
    char pad5[2052];
    int m_nSplitPacketSequence;
    INetChannel* m_pActiveChannel;
};