#include "pch.h"
#include "Listener.h"
#include "Session.h"
#include <memory>

Listener::~Listener()
{
	SocketUtils::Close(m_socket);
	for (AcceptEvent* acceptEvent : m_acceptEvents)
	{
		delete acceptEvent;
	}

	m_acceptEvents.clear();
}

bool Listener::StartAccept(NetAddress netAddress)
{
	m_socket = SocketUtils::CreateSocket();

	if (m_socket == INVALID_SOCKET)
		return false;

	if (g_iocpCore->Register(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetLinger(m_socket, 0, 0) == false)
		return false;

	if (SocketUtils::SetReuseAddress(m_socket, true) == false)
		return false;

	if (SocketUtils::Bind(m_socket, netAddress) == false)
		return false;

	if (SocketUtils::Listen(m_socket) == false)
		return false;

	const int32 acceptCount = 5;

	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = xnew<AcceptEvent>();
		acceptEvent->owner = shared_from_this();
		m_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}
	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(m_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(m_socket);
}


void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = MakeShared<Session>();

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	if (SocketUtils::AcceptEx(m_socket, session->GetSocket(), session->m_recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT &bytesReceived, acceptEvent))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = acceptEvent->session;

	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), m_socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddr;
	int32 addrLen = sizeof(sockAddr);

	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddr), &addrLen))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetAddr(NetAddress(sockAddr));
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}
