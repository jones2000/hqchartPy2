#pragma once
#include <vector>

#ifdef _WIN32
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#else
#define IFRSIZE ((int)(size*sizeof(struct ifreq)))
#endif

std::vector<std::string> GetMAC()
{
	unsigned char  null_card[6] = { 0 }, j = 0;
	unsigned char  cur_card[6] = { 0 };
	char szMAC[32] = { 0 };
	std::vector<std::string> aryMAC;
#ifdef _WIN32
	unsigned long ulAdapterInfoSize(0);
	GetAdaptersInfo(NULL, &ulAdapterInfoSize);
	if (ulAdapterInfoSize)
	{
		IP_ADAPTER_INFO*pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
		IP_ADAPTER_INFO*pAdapterInfoBkp = pAdapterInfo;
		IP_ADDR_STRING* pIPAddr = NULL;
		if (GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_SUCCESS)
		{
			do	//遍历所有适配器
			{
				if (pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET || pAdapterInfo->Type == IF_TYPE_IEEE80211)//判断是否为以太网接口
				{
					if (pAdapterInfo->AddressLength == 6 && memcmp(pAdapterInfo->Address, null_card, 6))
					{
						pIPAddr = &(pAdapterInfo->IpAddressList);
						std::string strIP = pIPAddr->IpAddress.String;
						sprintf_s(szMAC, "%02X-%02X-%02X-%02X-%02X-%02X", pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2], pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
						aryMAC.push_back(szMAC);
					}
				}
				pAdapterInfo = pAdapterInfo->Next;
			} while (pAdapterInfo);
		}
		delete pAdapterInfoBkp;
	}
#else
	int  sockfd, size = 1;
	struct ifconf ifc;
	struct sockaddr_in sa;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0) return(aryMAC);
	ifc.ifc_req = NULL;
	do
	{
		++size;
		if (!(ifc.ifc_req = (ifreq*)realloc(ifc.ifc_req, IFRSIZE))) return(aryMAC);
		ifc.ifc_len = IFRSIZE;
		if (ioctl(sockfd, SIOCGIFCONF, &ifc)) return(aryMAC);
	} while (IFRSIZE <= ifc.ifc_len);
	struct ifreq *ifr = ifc.ifc_req;
	for (; (char*)ifr < (char*)ifc.ifc_req + ifc.ifc_len; ++ifr)
	{
		if (ifr->ifr_addr.sa_data == (ifr + 1)->ifr_addr.sa_data) continue;
		if (ioctl(sockfd, SIOCGIFFLAGS, ifr)) continue;
		if (!ioctl(sockfd, SIOCGIFHWADDR, ifr))
		{
			switch (ifr->ifr_hwaddr.sa_family)
			{
			case ARPHRD_NETROM:
			case ARPHRD_ETHER:
			case ARPHRD_PPP:
			case ARPHRD_EETHER:
			case ARPHRD_IEEE802:
				break;
			default:
				continue;
			}
			if (memcmp(&ifr->ifr_addr.sa_data, null_card, 6))
				memcpy(cur_card, &ifr->ifr_addr.sa_data, 6);
				sprintf(szMAC, "%02X-%02X-%02X-%02X-%02X-%02X", cur_card[0], cur_card[1], cur_card[2], cur_card[3], cur_card[4], cur_card[5]);
				aryMAC.push_back(szMAC);
		}
	}
	close(sockfd);
#endif
	return aryMAC;
}
