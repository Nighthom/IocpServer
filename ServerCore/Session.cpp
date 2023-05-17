#include "pch.h"
#include "Session.h"

Session::Session()
{
	m_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(m_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::PreRecv:
		ProcessPreRecv();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	}
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	if (m_connected.exchange(false) == false)
		return;
	
	wprintf(L"Disconnect: %s", cause);
	RegisterDisconnect();
}

bool Session::RegisterConnect()
{
	if (IsConnected())
		return false;

	if (SocketUtils::SetReuseAddress(m_socket, true) == false)
		return false;

	if (SocketUtils::BindAnyAddress(m_socket, 0) == false)
		return false;
	
	m_connectEvent.Init();
	m_connectEvent.owner = shared_from_this();

	DWORD numOfBytes = 0;
	
	if (false == SocketUtils::ConnectEx(m_socket, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr), nullptr, 0, &numOfBytes, &m_connectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_connectEvent.owner = nullptr;
			return false;
		}
	}
	return true;
}

bool Session::RegisterDisconnect()
{
	if (IsConnected() == false)
		return false;

	m_disconnectEvent.Init();
	m_disconnectEvent.owner = shared_from_this();

	if (false == SocketUtils::DisconnectEx(m_socket, &m_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_disconnectEvent.owner = nullptr;
			return false;
		}
	}

	return true;
}

void Session::RegisterPreRecv()
{
	if (IsConnected() == false)
		return;

	m_preRecvEvent.Init();
	m_preRecvEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = nullptr;
	wsaBuf.len = 0;

	DWORD flags = 0;
	if (false == ::WSARecv(m_socket, &wsaBuf, 1, NULL, OUT & flags, &m_preRecvEvent, NULL))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			m_preRecvEvent.owner = nullptr;
		}
	}
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	m_recvEvent.Init();
	m_recvEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(m_recvBuffer);
	wsaBuf.len = sizeof(m_recvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	if (false == ::WSARecv(m_socket, &wsaBuf, 1, OUT &numOfBytes, OUT &flags, &m_recvEvent, NULL))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			m_recvEvent.owner = nullptr;
		}
	}
}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	m_sendEvent.Init();
	m_sendEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(m_sendEvent.sendBuffer);
	wsaBuf.len = sizeof(m_sendEvent.sendBuffer);
	
	DWORD numOfBytes = 0;

	if (false == ::WSASend(m_socket, &wsaBuf, 1, OUT &numOfBytes, 0, &m_sendEvent, NULL))
	{
		int32 errorCode = ::WSAGetLastError();

		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			m_sendEvent.owner = nullptr;
		}
	}

}

void Session::ProcessConnect()
{
	m_connectEvent.owner = nullptr;
	m_connected.store(true);
	OnConnected();

	RegisterPreRecv();
}

void Session::ProcessDisconnect()
{
	m_disconnectEvent.owner = nullptr;
	OnDisconnected();
}

void Session::ProcessPreRecv()
{
	m_preRecvEvent.owner = nullptr;
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	m_recvEvent.owner = nullptr;

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}
	
	OnRecv(m_recvBuffer, numOfBytes);
	RegisterPreRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
	m_sendEvent.owner = nullptr;

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	OnSend(numOfBytes);
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		wprintf(L"Handle Error: %d\n", errorCode);
		break;
	}
}
