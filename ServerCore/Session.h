#pragma once

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;

	enum
	{
		RECV_BUFFER_SIZE = 4096,
	};
	// ����/�Ҹ�
public:
	Session();
	virtual ~Session();
	
	// IocpObject override �Լ�
public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes);

	// �ܺο��� ����� Interface
public:
	void SetAddr(NetAddress addr) { m_addr = addr; }

	SOCKET GetSocket() { return m_socket; }
	NetAddress GetAddr() { return m_addr; }

	bool Connect();
	void Disconnect(const WCHAR* cause);

public:
	bool IsConnected() { return m_connected; }

	// �۾� Register
private:
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterPreRecv();
	void RegisterRecv();
	void RegisterSend();

	// Register�� �۾��� ���� �� ������ �Լ�
private:
	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessPreRecv();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

private:
	void HandleError(int32 errorCode);

	// ������ �ڵ�
private:
	virtual void OnConnected() { }
	virtual void OnDisconnected() { }
	virtual void OnRecv(BYTE* recvBuffer, int32 numOfBytes) { }
	virtual void OnSend(int32 numOfBytes) { }

	// TEMP recvBuffer(���߿� recvBuffer ���� ���� ����)
private:
	BYTE m_recvBuffer[RECV_BUFFER_SIZE] = { 0 };

	// ���� ���ϰ� ������ �ּ�, �������
private:
	SOCKET m_socket = INVALID_SOCKET;
	NetAddress m_addr = NetAddress();
	atomic<bool> m_connected = false;

	// Event ��� ����
private:
	ConnectEvent m_connectEvent;
	DisconnectEvent m_disconnectEvent;
	PreRecvEvent m_preRecvEvent;
	RecvEvent m_recvEvent;
	SendEvent m_sendEvent;
};

