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
/// \file devices.hpp

#pragma once
#include <boost/container/flat_map.hpp>

namespace devices
{

struct CmpStr
{
    bool operator()(const char* a, const char* b) const
    {
        return std::strcmp(a, b) < 0;
    }
};

// I2C device drivers may create a /hwmon subdirectory. For example the tmp75
// driver creates a /sys/bus/i2c/devices/<busnum>-<i2caddr>/hwmon
// directory. The sensor code relies on the presence of the /hwmon
// subdirectory to collect sensor readings. Initialization of this subdir is
// not reliable. I2C devices flagged with hasHWMonDir are tested for correct
// initialization, and when a failure is detected the device is deleted, and
// then recreated. The default is to retry 5 times before moving to the next
// device.

// Devices such as I2C EEPROMs do not generate this file structure. These
// kinds of devices are flagged using the noHWMonDir enumeration. The
// expectation is they are created correctly on the first attempt.

// This enumeration class exists to reduce copy/paste errors. It is easy to
// overlook the trailing parameter in the ExportTemplate structure when it is
// a simple boolean.
enum class createsHWMon : bool
{
    noHWMonDir,
    hasHWMonDir
};

struct ExportTemplate
{
    ExportTemplate(const char* params, const char* bus, const char* constructor,
                   const char* destructor, createsHWMon hasHWMonDir) :
        parameters(params), busPath(bus), add(constructor), remove(destructor),
        hasHWMonDir(hasHWMonDir) {};
    const char* parameters;
    const char* busPath;
    const char* add;
    const char* remove;
    createsHWMon hasHWMonDir;
};

const boost::container::flat_map<const char*, ExportTemplate, CmpStr>
    exportTemplates{
        {{"EEPROM_24C01",
          ExportTemplate("24c01 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C02",
          ExportTemplate("24c02 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C04",
          ExportTemplate("24c04 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C08",
          ExportTemplate("24c08 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C16",
          ExportTemplate("24c16 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C32",
          ExportTemplate("24c32 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C64",
          ExportTemplate("24c64 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C128",
          ExportTemplate("24c128 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM_24C256",
          ExportTemplate("24c256 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"ADS1015",
          ExportTemplate("ads1015 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"ADS7828",
          ExportTemplate("ads7828 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"DPS310",
          ExportTemplate("dps310 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"SI7020",
          ExportTemplate("si7020 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EEPROM",
          ExportTemplate("eeprom $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"EMC1412",
          ExportTemplate("emc1412 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"EMC1413",
          ExportTemplate("emc1413 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"EMC1414",
          ExportTemplate("emc1414 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},

         {"Gpio", ExportTemplate("$Index", "/sys/class/gpio", "export",
                                 "unexport", createsHWMon::noHWMonDir)},

         {"IPSPS1",
          ExportTemplate("ipsps1 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"MAX31725",
          ExportTemplate("max31725 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"MAX31730",
          ExportTemplate("max31730 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},

         {"MAX34440",
          ExportTemplate("max34440 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"PCA9537",
          ExportTemplate("pca9537 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"MAX6654",
          ExportTemplate("max6654 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"NCT6779",
          ExportTemplate("nct6779 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"PCA9542Mux",
          ExportTemplate("pca9542 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9543Mux",
          ExportTemplate("pca9543 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9544Mux",
          ExportTemplate("pca9544 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9545Mux",
          ExportTemplate("pca9545 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9546Mux",
          ExportTemplate("pca9546 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9547Mux",
          ExportTemplate("pca9547 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9548Mux",
          ExportTemplate("pca9548 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9846Mux",
          ExportTemplate("pca9846 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9847Mux",
          ExportTemplate("pca9847 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9848Mux",
          ExportTemplate("pca9848 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PCA9849Mux",
          ExportTemplate("pca9849 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"SIC450",
          ExportTemplate("sic450 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"Q50SN12072",
          ExportTemplate("q50sn12072 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"XDPE12284",
          ExportTemplate("xdpe12284 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"LM95234",
          ExportTemplate("lm95234 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP112",
          ExportTemplate("tmp112 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP175",
          ExportTemplate("tmp175 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP421",
          ExportTemplate("tmp421 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP432",
          ExportTemplate("tmp432 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP441",
          ExportTemplate("tmp441 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP461",
          ExportTemplate("tmp461 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP1075",
          ExportTemplate("tmp1075 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"MAX16550",
          ExportTemplate("max16550 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"LM75A", ExportTemplate("lm75a $Address",
                                  "/sys/bus/i2c/devices/i2c-$Bus", "new_device",
                                  "delete_device", createsHWMon::hasHWMonDir)},
         {"LM25066",
          ExportTemplate("lm25066 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP75", ExportTemplate("tmp75 $Address",
                                  "/sys/bus/i2c/devices/i2c-$Bus", "new_device",
                                  "delete_device", createsHWMon::hasHWMonDir)},
         {"W83773G",
          ExportTemplate("w83773g $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"RAA228000",
          ExportTemplate("raa228000 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"DPS800",
          ExportTemplate("dps800 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"MAX31790",
          ExportTemplate("max31790 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"PIC32", ExportTemplate("pic32 $Address",
                                  "/sys/bus/i2c/devices/i2c-$Bus", "new_device",
                                  "delete_device", createsHWMon::hasHWMonDir)},
         {"INA226",
          ExportTemplate("ina226 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"JC42", ExportTemplate("jc42 $Address",
                                 "/sys/bus/i2c/devices/i2c-$Bus", "new_device",
                                 "delete_device", createsHWMon::hasHWMonDir)},
         {"ADM1293",
          ExportTemplate("adm1293 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"INA219",
          ExportTemplate("ina219 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"RAA229126",
          ExportTemplate("raa229126 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"RAA229620",
          ExportTemplate("raa229620 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"RAA229621",
          ExportTemplate("raa229621 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP432",

          ExportTemplate("tmp432 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP461",
          ExportTemplate("tmp461 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"TMP1075",
          ExportTemplate("tmp1075 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"MAX16550",
          ExportTemplate("max16550 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)},
         {"HDC1080",
          ExportTemplate("hdc1080 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"ADS112C04",
          ExportTemplate("ads112c04 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"LM83", ExportTemplate("lm83 $Address",
                                 "/sys/bus/i2c/devices/i2c-$Bus", "new_device",
                                 "delete_device", createsHWMon::noHWMonDir)},
         {"TPS25990",
          ExportTemplate("tps25990 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::noHWMonDir)},
         {"PIC32",
          ExportTemplate("pic32 $Address", "/sys/bus/i2c/devices/i2c-$Bus",
                         "new_device", "delete_device",
                         createsHWMon::hasHWMonDir)}}};
} // namespace devices
