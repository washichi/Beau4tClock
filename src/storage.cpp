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

    Serial.println(F(" XXXXXXXXXXXX NO EEPROM R/W for now, reading not correct, so config will be garbage XXXXXXXXXXXX")); //@todo
    return;

    // eeprom
    EEPROM.begin(512);

    int address = 0;
    int initializeFlag = 0;



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

        EEPROM.put(address, config.dayBrightness);
        address += sizeof(config.dayBrightness);

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

    EEPROM.get(address, config.dayBrightness);
    address += sizeof(config.dayBrightness);

    Serial.println(F("Config loaded FROM EEPROM."));

}