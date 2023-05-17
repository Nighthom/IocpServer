#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : m_sockAddr(sockAddr)
{
}

NetAddress::NetAddress(wstring ip, uint16 port)
{
	::memset(&m_sockAddr, 0, sizeof(m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = ::htons(port);
	m_sockAddr.sin_addr = Ip2Address(ip.data());
}

wstring NetAddress::GetIpAddress()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &m_sockAddr.sin_addr, buffer, sizeof(buffer) / sizeof(WCHAR));
	return wstring(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}
