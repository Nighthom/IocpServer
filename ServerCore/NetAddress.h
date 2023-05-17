#pragma once

class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(wstring ip, uint16 port);

	SOCKADDR_IN& GetSockAddr() { return m_sockAddr; }
	wstring GetIpAddress();
	uint16 GetPort() { return m_sockAddr.sin_port; }

public:
	static IN_ADDR Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN m_sockAddr;
};

