#include "Gateway.h"
#include <Windows.h>
#include <iphlpapi.h>
#include <iostream>
#include <format>
#include <string>

static const std::string defaultGateway = "{DEFAULT_GATEWAY}";
static bool defaultDHCPEnabled = false;
static const std::string defaultDHCPServer = "{DEFAULT_DHCPSERVER}";
static const std::string ignoreIPAddress = "0.0.0.0";

[[nodiscard]] GatewayStruct GetGateway(void)
{
    IP_ADAPTER_INFO *pAdapterInfo = nullptr;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    DWORD dwRetVal;
    GatewayStruct gatewayStruct;

    std::cout << "[NOTE]: 默认忽略网关(Gateway)为 0.0.0.0 的适配器。" << std::endl;

    pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
    if (pAdapterInfo == nullptr)
    {
        std::cerr << "Error allocating memory for adapter info." << std::endl;
        return GatewayStruct(defaultDHCPServer, defaultDHCPEnabled, defaultDHCPServer);
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
        if (pAdapterInfo == nullptr)
        {
            std::cerr << "Error re - allocating memory for adapter info." << std::endl;
            return GatewayStruct(dwRetVal);
        }
    }

    // 第二次调用获取适配器信息
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        IP_ADAPTER_INFO *pAdapter = pAdapterInfo;
        while (pAdapter != nullptr)
        {
            if (std::strcmp(pAdapter->GatewayList.IpAddress.String, ignoreIPAddress.c_str()))
            {
                gatewayStruct.GatewayList.push_back(pAdapter->GatewayList.IpAddress.String);
                gatewayStruct.DHCPEnabled.push_back((bool)pAdapter->DhcpEnabled);
                gatewayStruct.DHCPServerList.push_back(pAdapter->DhcpServer.IpAddress.String);
            }
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        std::cerr << std::format("GetAdaptersInfo failed with error: {}", dwRetVal) << std::endl;
        if (pAdapterInfo != nullptr)
        {
            HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        }
        return GatewayStruct(dwRetVal);
    }

    if (pAdapterInfo != nullptr)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
    }

    return gatewayStruct;
}

[[nodiscard]] GatewayStruct GetGateway(bool defaultIgnore)
{
    IP_ADAPTER_INFO *pAdapterInfo = nullptr;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    DWORD dwRetVal;
    GatewayStruct gatewayStruct;

    if (defaultIgnore)
    {
        std::cout << "[NOTE]: 默认忽略网关(Gateway)为 0.0.0.0 的适配器。" << std::endl;
    }

    pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
    if (pAdapterInfo == nullptr)
    {
        std::cerr << "Error allocating memory for adapter info." << std::endl;
        return GatewayStruct(defaultDHCPServer, defaultDHCPEnabled, defaultDHCPServer);
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
        if (pAdapterInfo == nullptr)
        {
            std::cerr << "Error re - allocating memory for adapter info." << std::endl;
            return GatewayStruct(dwRetVal);
        }
    }

    // 第二次调用获取适配器信息
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != NO_ERROR)
    {
        std::cerr << std::format("GetAdaptersInfo failed with error: {}", dwRetVal) << std::endl;
        if (pAdapterInfo != nullptr)
        {
            HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        }
        return GatewayStruct(dwRetVal);
    }

    IP_ADAPTER_INFO *pAdapter = pAdapterInfo;
    while (pAdapter != nullptr)
    {
        if (defaultIgnore)
        {
            if (std::strcmp(pAdapter->GatewayList.IpAddress.String, ignoreIPAddress.c_str()))
            {
                gatewayStruct.GatewayList.push_back(pAdapter->GatewayList.IpAddress.String);
                gatewayStruct.DHCPEnabled.push_back((bool)pAdapter->DhcpEnabled);
                gatewayStruct.DHCPServerList.push_back(pAdapter->DhcpServer.IpAddress.String);
            }
        }
        else
        {
            gatewayStruct.GatewayList.push_back(pAdapter->GatewayList.IpAddress.String);
            gatewayStruct.DHCPEnabled.push_back((bool)pAdapter->DhcpEnabled);
            gatewayStruct.DHCPServerList.push_back(pAdapter->DhcpServer.IpAddress.String);
        }
        pAdapter = pAdapter->Next;
    }

    if (pAdapter != nullptr)
    {
        HeapFree(GetProcessHeap(), 0, pAdapter);
    }
    if (pAdapterInfo != nullptr)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
    }

    gatewayStruct.defaultIgnore = false;

    return gatewayStruct;
}