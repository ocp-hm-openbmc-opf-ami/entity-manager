#pragma once

#include "configuration.hpp"

#include <nlohmann/json.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>

#include <iostream>
#include <set>
#include <vector>

extern std::shared_ptr<sdbusplus::asio::connection> systemBus;
using foundProbeData = std::map<std::string, std::string>;
static foundProbeData foundData;
static std::map<std::string, foundProbeData> mapFoundData;

constexpr const char* fruConn = "xyz.openbmc_project.FruDevice";
constexpr const char* fruIntf = "xyz.openbmc_project.FruDevice";


namespace dbus_interface
{
void tryIfaceInitialize(
    std::shared_ptr<sdbusplus::asio::dbus_interface>& iface);

std::shared_ptr<sdbusplus::asio::dbus_interface> createInterface(
    sdbusplus::asio::object_server& objServer, const std::string& path,
    const std::string& interface, const std::string& parent,
    bool checkNull = false);

template <typename PropertyType>
void addArrayToDbus(const std::string& name, const nlohmann::json& array,
                    sdbusplus::asio::dbus_interface* iface,
                    sdbusplus::asio::PropertyPermission permission,
                    nlohmann::json& systemConfiguration,
                    const std::string& jsonPointerString)
{
    std::vector<PropertyType> values;
    for (const auto& property : array)
    {
        auto ptr = property.get_ptr<const PropertyType*>();
        if (ptr != nullptr)
        {
            values.emplace_back(*ptr);
        }
    }

    if (permission == sdbusplus::asio::PropertyPermission::readOnly)
    {
        iface->register_property(name, values);
    }
    else
    {
        iface->register_property(
            name, values,
            [&systemConfiguration,
             jsonPointerString{std::string(jsonPointerString)}](
                const std::vector<PropertyType>& newVal,
                std::vector<PropertyType>& val) {
                val = newVal;
                if (!configuration::setJsonFromPointer(jsonPointerString, val,
                                                       systemConfiguration))
                {
                    std::cerr << "error setting json field\n";
                    return -1;
                }
                if (!configuration::writeJsonFiles(systemConfiguration))
                {
                    std::cerr << "error setting json file\n";
                    return -1;
                }
                return 1;
            });
    }
}

template <typename PropertyType>
bool persistAssetTag(const PropertyType& newVal,
                     const std::string& jsonPointerString)
{
    std::size_t found = jsonPointerString.find_last_of("/\\");
    std::string jsonPointerPath = jsonPointerString.substr(0, found);

    auto it = mapFoundData.find(jsonPointerPath);
    if (it == mapFoundData.end())
    {
        std::cerr << "Error in finding jsonPointerPath in mapFoundData" << "\n";
        return false;
    }

    foundProbeData& tmpMap = it->second;
    auto foundPath = tmpMap.find("foundPath");
    if (foundPath == tmpMap.end())
    {
        std::cerr << "No prob object data is avaliable in foundProbeData"
                  << "\n";
        return false;
    }

    systemBus->async_method_call(
        [](const boost::system::error_code& ec) {
            if (ec)
            {
                std::cerr << "Error setting AssetTag in FRU interface " << ec
                          << "\n";
            }
        },
        fruConn, foundPath->second, "org.freedesktop.DBus.Properties", "Set",
        fruIntf, "PRODUCT_ASSET_TAG", std::variant<PropertyType>(newVal));
    return true;
}

template <typename PropertyType>
void addProperty(const std::string& name, const PropertyType& value,
                 sdbusplus::asio::dbus_interface* iface,
                 nlohmann::json& systemConfiguration,
                 const std::string& jsonPointerString,
                 sdbusplus::asio::PropertyPermission permission)
{
    if (permission == sdbusplus::asio::PropertyPermission::readOnly)
    {
        iface->register_property(name, value);
        return;
    }
    iface->register_property(
        name, value,
        [name, &systemConfiguration,
         jsonPointerString{std::string(jsonPointerString)}](
            const PropertyType& newVal, PropertyType& val) {
            if (name == "AssetTag")
            {
                if (!persistAssetTag(newVal, jsonPointerString))
                {
                    std::cerr << "error setting AssetTag in FRU interface\n";
                    return -1;
                }
            }
            val = newVal;
            if (!configuration::setJsonFromPointer(jsonPointerString, val,
                                                   systemConfiguration))
            {
                std::cerr << "error setting json field\n";
                return -1;
            }
            if (!configuration::writeJsonFiles(systemConfiguration))
            {
                std::cerr << "error setting json file\n";
                return -1;
            }
            return 1;
        });
}

void createDeleteObjectMethod(
    const std::string& jsonPointerPath,
    const std::shared_ptr<sdbusplus::asio::dbus_interface>& iface,
    sdbusplus::asio::object_server& objServer,
    nlohmann::json& systemConfiguration);

void populateInterfaceFromJson(
    nlohmann::json& systemConfiguration, const std::string& jsonPointerPath,
    std::shared_ptr<sdbusplus::asio::dbus_interface>& iface,
    nlohmann::json& dict, sdbusplus::asio::object_server& objServer,
    sdbusplus::asio::PropertyPermission permission =
        sdbusplus::asio::PropertyPermission::readOnly);

void createAddObjectMethod(
    const std::string& jsonPointerPath, const std::string& path,
    nlohmann::json& systemConfiguration,
    sdbusplus::asio::object_server& objServer, const std::string& board);

std::vector<std::weak_ptr<sdbusplus::asio::dbus_interface>>&
    getDeviceInterfaces(const nlohmann::json& device);

} // namespace dbus_interface
