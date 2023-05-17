#pragma once

class IocpObject;
class Session;

enum class EventType : uint8
{
	Accept, 
	Connect, 
	Disconnect, 
	PreRecv,
	Recv, 
	Send, 
};

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);
	
	void Init();
public:
	shared_ptr<IocpObject> owner;
	EventType eventType;
};

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

public:
	shared_ptr<Session> session = nullptr;
};


class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }
};

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) { }
};

class PreRecvEvent : public IocpEvent
{
public:
	PreRecvEvent() : IocpEvent(EventType::PreRecv) { }
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) { }
};

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) { }

	// TEMP sendBuffer
	BYTE sendBuffer[4096] = { 0 };
};
