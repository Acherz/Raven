/**
 * Raven - ICMP 网络工具
 * - 子网掩码(IPv4)
 * - 网关 DHCP
 */

#include "subnetMask/subnetMask.h"
#include "Gateway/Gateway.h"
#include <format>
#include <iostream>
#include <vector>
#include <algorithm>
#include<sstream>
#include<unordered_map>

class CommandLineArgs
{
public:
    CommandLineArgs(int argc, const char* argv[]) {
        for (size_t i = 0; i < argc; i++)
        {
            std::string arg = argv[i];
            size_t equalPos = arg.find('=');
            if (equalPos != std::string::npos) {
                std::string key = arg.substr(0, equalPos);
                std::string value = arg.substr(equalPos + 1);
                args[arg] = value;
            } else {
                args[arg] = "";
            }
        }
        
    }

    bool hasArg(const std::string& key) const {
        return args.find(key) != args.end();
    }

    template<typename T>
    T getValue(const std::string& key) const {
        auto it = args.find(key);
        if (it != args.end()) {
            std::istringstream iss(it->second);
            T value;
            iss >> value;
            return value;
        }
        throw std::runtime_error("Argument" + key + "not found");
    }
private:
    std::unordered_map<std::string, std::string> args;
};

void GetSubnetMaskExample()
{
    SubnetMaskStruct sms;
    sms.defaultIgnore = false;
    sms = GetSubnetMask(sms);
    if (sms.AdapterName.empty())
    {
        std::cout << "您当前并未连接任何网络" << std::endl;
    }
    for (size_t i = 0; i < sms.SubnetMask.size(); i++)
    {
        std::cout << std::format("适配器名称: {} 子网掩码: {} IP 地址: {}", sms.AdapterName[i], sms.SubnetMask[i], sms.MachineIPAdress[i]) << std::endl;
    }
}

void GetGatewayExample()
{
    GatewayStruct gs = GetGateway(false);
    if (gs.GatewayList.empty())
    {
        std::cout << "您当前并未连接任何网络" << std::endl;
    }
    for (size_t i = 0; i < gs.GatewayList.size(); i++)
    {
        std::cout << std::format("网关: {}, DHCP 启用(1=true|0=false): {}, DHCP 服务器: {}", gs.GatewayList[i], (int)gs.DHCPEnabled[i], gs.DHCPServerList[i]) << std::endl;
    }
}

int main(int argc, const char *argv[])
{
    CommandLineArgs args(argc, argv);
    if (args.hasArg("gateway")) {
        GetGatewayExample();
    }
    if (args.hasArg("subnetmask")) {
        GetSubnetMaskExample();
    }
}