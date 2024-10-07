#include "subnetMask.h"
#include <Windows.h>
#include <iphlpapi.h>
#include <iostream>
#include <format>

static const std::string defaultAdapterName = "{DEFAULT_ADAPTER}";
static const std::string defaultSubnetMask = "{DEFAULT_SUBNETMASK}";
static const std::string defaultMachineIP = "{MACHINE_IPADRESS}";
static const std::string ignoreMask = "0.0.0.0";

#pragma comment(lib, "Iphlpapi.lib")

[[nodiscard]]
SubnetMaskStruct
GetSubnetMask(void)
{
    SubnetMaskStruct subnetMaskStruct;
    IP_ADAPTER_INFO *pAdapterInfo = nullptr;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    DWORD dwRetVal;

    std::cout << "[NOTE]: 默认忽略子网掩码为 0.0.0.0 的适配器" << std::endl;

    pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
    if (pAdapterInfo == nullptr)
    {
        std::cerr << "Error allocating memory for adapter info." << std::endl;

        return SubnetMaskStruct(defaultAdapterName, defaultSubnetMask, defaultMachineIP);
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
        if (pAdapterInfo == nullptr)
        {
            std::cerr << "Error re - allocating memory for adapter info." << std::endl;
            return SubnetMaskStruct(defaultAdapterName, defaultSubnetMask, defaultMachineIP);
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        IP_ADAPTER_INFO *pAdapter = pAdapterInfo;
        while (pAdapter != nullptr)
        {
            if (strcmp(pAdapter->IpAddressList.IpMask.String, ignoreMask.c_str()))
            {
                subnetMaskStruct.AdapterName.push_back(pAdapter->AdapterName);
                subnetMaskStruct.SubnetMask.push_back(pAdapter->IpAddressList.IpMask.String);
                subnetMaskStruct.MachineIPAdress.push_back(pAdapter->IpAddressList.IpAddress.String);
            }

            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwRetVal,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL);
        std::cerr << std::format("GetAdaptersInfo failed with erorr: {}", dwRetVal) << std::endl;
        std::cerr << std::format("Error Message: {}", (LPCTSTR)lpMsgBuf) << std::endl;
        if (pAdapterInfo != nullptr)
        {
            HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        }
        return SubnetMaskStruct(dwRetVal);
    }

    if (pAdapterInfo != nullptr)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
    }

    subnetMaskStruct.ItemCount = subnetMaskStruct.AdapterName.size();

    return subnetMaskStruct;
}

[[nodiscard]]
SubnetMaskStruct GetSubnetMask(SubnetMaskStruct &__subnetMaskStruct)
{
    SubnetMaskStruct subnetMaskStruct;
    IP_ADAPTER_INFO *pAdapterInfo = nullptr;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    DWORD dwRetVal;

    if (__subnetMaskStruct.defaultIgnore)
    {
        std::cout << "[NOTE]: 配置忽略子网掩码为 0.0.0.0 的适配器" << std::endl;
    }

    pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
    if (pAdapterInfo == nullptr)
    {
        std::cerr << "Error allocating memory for adapter info." << std::endl;

        return SubnetMaskStruct(defaultAdapterName, defaultSubnetMask, defaultMachineIP);
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);
        if (pAdapterInfo == nullptr)
        {
            std::cerr << "Error re - allocating memory for adapter info." << std::endl;
            return SubnetMaskStruct(defaultAdapterName, defaultSubnetMask, defaultMachineIP);
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != NO_ERROR)
    {
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwRetVal,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL);
        std::cerr << std::format("GetAdaptersInfo failed with erorr: {}", dwRetVal) << std::endl;
        std::cerr << std::format("Error Message: {}", (LPCTSTR)lpMsgBuf) << std::endl;
        if (pAdapterInfo != nullptr)
        {
            HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        }
        return SubnetMaskStruct(dwRetVal);
    }

    IP_ADAPTER_INFO *pAdapter = pAdapterInfo;
    while (pAdapter != nullptr)
    {
        if (!__subnetMaskStruct.defaultIgnore)
        {
            subnetMaskStruct.AdapterName.push_back(pAdapter->AdapterName);
            subnetMaskStruct.SubnetMask.push_back(pAdapter->IpAddressList.IpMask.String);
            subnetMaskStruct.MachineIPAdress.push_back(pAdapter->IpAddressList.IpAddress.String);
        }
        else
        {
            if (strcmp(pAdapter->IpAddressList.IpMask.String, ignoreMask.c_str()))
            {
                subnetMaskStruct.AdapterName.push_back(pAdapter->AdapterName);
                subnetMaskStruct.SubnetMask.push_back(pAdapter->IpAddressList.IpMask.String);
                subnetMaskStruct.MachineIPAdress.push_back(pAdapter->IpAddressList.IpAddress.String);
            }
        }

        pAdapter = pAdapter->Next;
    }

    if (pAdapter != nullptr) {
        HeapFree(GetProcessHeap(), 0, pAdapter);
    }

    if (pAdapterInfo != nullptr)
    {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
    }

    subnetMaskStruct.ItemCount = subnetMaskStruct.AdapterName.size();

    return subnetMaskStruct;
}