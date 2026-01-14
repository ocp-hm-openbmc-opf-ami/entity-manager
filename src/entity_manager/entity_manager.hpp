/*
// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
/// \file entity_manager.hpp

#pragma once

#include "../utils.hpp"

#include <systemd/sd-journal.h>

#include <boost/container/flat_map.hpp>
#include <nlohmann/json.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <phosphor-logging/lg2.hpp>

#include <string>

inline void logDeviceAdded(const nlohmann::json& record)
{
    if (!deviceHasLogging(record))
    {
        return;
    }
    auto findType = record.find("Type");
    auto findAsset =
        record.find("xyz.openbmc_project.Inventory.Decorator.Asset");

    std::string model = "Unknown";
    std::string type = "Unknown";
    std::string sn = "Unknown";
    std::string name = "Unknown";

    if (findType != record.end())
    {
        type = findType->get<std::string>();
    }
    if (findAsset != record.end())
    {
        auto findModel = findAsset->find("Model");
        auto findSn = findAsset->find("SerialNumber");
        if (findModel != findAsset->end())
        {
            model = findModel->get<std::string>();
        }
        if (findSn != findAsset->end())
        {
            const std::string* getSn = findSn->get_ptr<const std::string*>();
            if (getSn != nullptr)
            {
                sn = *getSn;
            }
            else
            {
                sn = findSn->dump();
            }
        }
    }

    auto findName = record.find("Name");
    if (findName != record.end())
    {
        name = findName->get<std::string>();
    }

    std::string severity =
            "xyz.openbmc_project.Logging.Entry.Level.Informational";
        auto bus = sdbusplus::bus::new_default_system();
        sdbusplus::message::message m = bus.new_method_call(
            "xyz.openbmc_project.Logging", "/xyz/openbmc_project/logging",
            "xyz.openbmc_project.Logging.Create", "Create");
        std::string journalMsg = "OpenBMC.0.1.InventoryAdded";

        std::map<std::string, std::string> additionalData;
        additionalData["REDFISH_MESSAGE_ID"] =
            "OpenBMC.0.1.InventoryAdded";
        m.append(journalMsg, severity, additionalData);
        try
        {
            bus.call(m);
        }
        catch (const sdbusplus::exception_t& e)
        {
            lg2::error("Failed to create log entry: {ERROR}", "ERROR",
                       e.what());
        }
}

inline void logDeviceRemoved(const nlohmann::json& record)
{
    if (!deviceHasLogging(record))
    {
        return;
    }
    auto findType = record.find("Type");
    auto findAsset =
        record.find("xyz.openbmc_project.Inventory.Decorator.Asset");

    std::string model = "Unknown";
    std::string type = "Unknown";
    std::string sn = "Unknown";
    std::string name = "Unknown";

    if (findType != record.end())
    {
        type = findType->get<std::string>();
    }
    if (findAsset != record.end())
    {
        auto findModel = findAsset->find("Model");
        auto findSn = findAsset->find("SerialNumber");
        if (findModel != findAsset->end())
        {
            model = findModel->get<std::string>();
        }
        if (findSn != findAsset->end())
        {
            const std::string* getSn = findSn->get_ptr<const std::string*>();
            if (getSn != nullptr)
            {
                sn = *getSn;
            }
            else
            {
                sn = findSn->dump();
            }
        }
    }

    auto findName = record.find("Name");
    if (findName != record.end())
    {
        name = findName->get<std::string>();
    }

    std::string severity =
            "xyz.openbmc_project.Logging.Entry.Level.Informational";
        auto bus = sdbusplus::bus::new_default_system();
        sdbusplus::message::message m = bus.new_method_call(
            "xyz.openbmc_project.Logging", "/xyz/openbmc_project/logging",
            "xyz.openbmc_project.Logging.Create", "Create");
        std::string journalMsg = "OpenBMC.0.1.InventoryRemoved";

        std::map<std::string, std::string> additionalData;
        additionalData["REDFISH_MESSAGE_ID"] =
            "OpenBMC.0.1.InventoryRemoved";
        m.append(journalMsg, severity, additionalData);
        try
        {
            bus.call(m);
        }
        catch (const sdbusplus::exception_t& e)
        {
            lg2::error("Failed to create log entry: {ERROR}", "ERROR",
                       e.what());
        }
}
