#pragma once

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	bool StartAccept(NetAddress netAddress);
	void CloseSocket();

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes) override;

public:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET m_socket = INVALID_SOCKET;
	Vector<AcceptEvent*> m_acceptEvents;
	
};

