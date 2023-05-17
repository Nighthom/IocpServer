#pragma once
#include "IocpEvent.h"

// ������, ���� Ŭ������ ����� ����� ����
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes) abstract;
};

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

public:
	bool Register(shared_ptr<IocpObject> iocpObject);
	bool Dispatch();

private:
	HANDLE m_iocpHandle;
};

