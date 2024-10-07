#pragma once

#include <vector>
#include <string>
#include <Windows.h>

struct SubnetMaskStruct
{
    std::vector<std::string> AdapterName;     // 适配器名称
    std::vector<std::string> SubnetMask;      // 子网掩码
    std::vector<std::string> MachineIPAdress; // 本机 IP 地址
    DWORD dwRetVal;                           // 返回值
    unsigned int ItemCount;                   // 适配器、子网掩码、IP地址已获取的数目
    bool defaultIgnore = true;
    SubnetMaskStruct(const std::string &__AdapterName, const std::string &__SubnetMask, const std::string &__MachineIPAddress)
    {
        AdapterName.push_back(__AdapterName);
        SubnetMask.push_back(__SubnetMask);
        MachineIPAdress.push_back(__MachineIPAddress);
    }

    SubnetMaskStruct(DWORD __dwRetVal)
    {
        dwRetVal = __dwRetVal;
    }

    SubnetMaskStruct() = default; // 默认构造函数
};

/**
 * 函数名称：
 * GetSubnetMask(void)
 *
 * 作用：
 * 获取当前网络的子网掩码(WinAPI).
 *
 * 默认返回值：
 * {DEFAULT_ADAPTER}
 * {DEFAULT_SUBNETMASK}
 * {MACHINE_IPADRESS}
 */
[[nodiscard]] SubnetMaskStruct GetSubnetMask(void);
[[nodiscard]] SubnetMaskStruct GetSubnetMask(SubnetMaskStruct& __subnetMaskStruct);