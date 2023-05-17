#pragma once

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;

	enum
	{
		RECV_BUFFER_SIZE = 4096,
	};
	// 생성/소멸
public:
	Session();
	virtual ~Session();
	
	// IocpObject override 함수
public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes);

	// 외부에서 사용할 Interface
public:
	void SetAddr(NetAddress addr) { m_addr = addr; }

	SOCKET GetSocket() { return m_socket; }
	NetAddress GetAddr() { return m_addr; }

	bool Connect();
	void Disconnect(const WCHAR* cause);

public:
	bool IsConnected() { return m_connected; }

	// 작업 Register
private:
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterPreRecv();
	void RegisterRecv();
	void RegisterSend();

	// Register한 작업이 끝난 뒤 수행할 함수
private:
	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessPreRecv();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

private:
	void HandleError(int32 errorCode);

	// 컨텐츠 코드
private:
	virtual void OnConnected() { }
	virtual void OnDisconnected() { }
	virtual void OnRecv(BYTE* recvBuffer, int32 numOfBytes) { }
	virtual void OnSend(int32 numOfBytes) { }

	// TEMP recvBuffer(나중에 recvBuffer 따로 만들 예정)
private:
	BYTE m_recvBuffer[RECV_BUFFER_SIZE] = { 0 };

	// 현재 소켓과 소켓의 주소, 연결상태
private:
	SOCKET m_socket = INVALID_SOCKET;
	NetAddress m_addr = NetAddress();
	atomic<bool> m_connected = false;

	// Event 목록 관리
private:
	ConnectEvent m_connectEvent;
	DisconnectEvent m_disconnectEvent;
	PreRecvEvent m_preRecvEvent;
	RecvEvent m_recvEvent;
	SendEvent m_sendEvent;
};

