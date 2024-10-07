#pragma once

#include <vector>
#include <string>
#include <Windows.h>

struct GatewayStruct
{
    // 可选项
    std::vector<std::string> ApapterList;
    bool defaultIgnore = true;

    // 必选项
    std::vector<std::string> GatewayList;
    std::vector<bool> DHCPEnabled;
    std::vector<std::string> DHCPServerList;

    // 仅用于错误处理
    DWORD dwRetVal;

    GatewayStruct() = default;
    GatewayStruct(const std::string &__Gateway, bool __DHCPEnabled, const std::string &__DHCPServer)
    {
        GatewayList.push_back(__Gateway);
        DHCPEnabled.push_back(__DHCPEnabled);
        DHCPServerList.push_back(__DHCPServer);
    }
    GatewayStruct(DWORD __dwRetVal)
    {
        dwRetVal = __dwRetVal;
    }
};

[[nodiscard]] GatewayStruct GetGateway(void);
[[nodiscard]] GatewayStruct GetGateway(bool defaultIgnore);
