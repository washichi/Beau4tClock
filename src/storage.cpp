#include "storage.h"
#include <EEPROM.h>
#include "globals.h"

void storage_init()
{
    if (!LittleFS.begin())
    {
        Serial.println(F("An Error has occurred while mounting LittleFS"));
        return;
    }

    // eeprom
    EEPROM.begin(512);

    int address = 0;
    int initializeFlag = 0;

    Serial.println(config.provider);

    EEPROM.get(address, initializeFlag);
    if (initializeFlag != 1)
    {
        Serial.println(F("EEPROM not initialized, writing default config to EEPROM."));
        initializeFlag = 1;

        EEPROM.put(address, initializeFlag);
        address += sizeof(initializeFlag);

        EEPROM.put(address, config.provider);
        address += sizeof(config.provider);

        EEPROM.put(address, config.spot);
        address += sizeof(config.spot);

        EEPROM.put(address, config.lat);
        address += sizeof(config.lat);

        EEPROM.put(address, config.lon);
        address += sizeof(config.lon);

        EEPROM.put(address, config.brightnessPercentage);
        address += sizeof(config.brightnessPercentage);

        EEPROM.commit();
    }

    address = 0;
    EEPROM.get(address, initializeFlag);
    address += sizeof(initializeFlag);
    if (initializeFlag != 1)
    {
        Serial.println(F("EEPROM config failed to initialize ..."));
    }

    EEPROM.get(address, config.provider);
    address += sizeof(config.provider);

    EEPROM.get(address, config.spot);
    address += sizeof(config.spot);

    EEPROM.get(address, config.lat);
    address += sizeof(config.lat);

    EEPROM.get(address, config.lon);
    address += sizeof(config.lon);

    EEPROM.get(address, config.brightnessPercentage);
    address += sizeof(config.brightnessPercentage);

    Serial.println(F("Config loaded:"));

}