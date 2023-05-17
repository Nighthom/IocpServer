#include "pch.h"
#include "IocpCore.h"

IocpCore::IocpCore()
{
	m_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(m_iocpHandle == INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(m_iocpHandle);
}

bool IocpCore::Register(shared_ptr<IocpObject> iocpObject)
{
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), m_iocpHandle, 0, 0);
}

bool IocpCore::Dispatch()
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(m_iocpHandle, OUT &numOfBytes, OUT &key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), INFINITE))
	{
		shared_ptr<IocpObject> iocpObject = iocpEvent->owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errorCode = ::WSAGetLastError();
		switch (errorCode)
		{
		default:
			shared_ptr<IocpObject> iocpObject = iocpEvent->owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}
	return true;
}


