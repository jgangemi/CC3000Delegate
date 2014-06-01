#include "CC3000Delegate.h"

const unsigned long PROGMEM connectTimeout = 15L * 1000L;
const unsigned long PROGMEM dhcpTimeout = 60L * 1000L;
const unsigned long PROGMEM responseTimeout = 15L * 1000L;

CC3000Delegate::CC3000Delegate(char* ssid, uint8_t csPin, uint8_t irqPin, uint8_t vbatPin, uint8_t spiSpeed)
{
    this->ssid = ssid;
    this->cc3000 = new Adafruit_CC3000(csPin, irqPin, vbatPin, spiSpeed);
}

void CC3000Delegate::initialize()
{
    RCP_DEBUG("initializing cc3000...");

    if (!cc3000->begin())
    {
        hang(F("\ninitialization failed, check wiring!"));
    }

    RCP_DEBUG("ok\n");

#ifdef RC_PROXY_DEBUG    
    validateFirmware();
#endif

    RCP_DEBUG("deleting old connection profiles...");
    if (!cc3000->deleteProfiles())
    {
        hang(F("\nfailed to delete old connection profiles!"));
    }

    RCP_DEBUG("ok\n");
    
    connectToAP();
    dhcp();
}

Adafruit_CC3000* CC3000Delegate::getCC3000()
{
    return this->cc3000;
}

Client* CC3000Delegate::getClient()
{   
    return new Adafruit_CC3000_Client();
}

bool CC3000Delegate::connected()
{
    return cc3000->checkConnected();
}

void CC3000Delegate::setAPCredentials(char *key, uint8_t secMode)
{
    this->key = key;
    this->secMode = secMode;
}

void CC3000Delegate::disconnect()
{
    cc3000->disconnect();
}

// private

void CC3000Delegate::connectToAP()
{
    RCP_DEBUG("attempting connection to ");
    RCP_DEBUG(ssid);
    RCP_DEBUG("...");
    
    if (key == NULL || secMode == 0)
    {
        if (cc3000->connectOpen(ssid))
        {
            RCP_DEBUG("connected!\n");
            return;
        }
    }
    else
    {
        if (cc3000->connectToAP(ssid, key, secMode))
        {
            RCP_DEBUG("connected\n");
            return;
        }
    }

    hang(F("failed to connect to access point!"));
}

void CC3000Delegate::dhcp()
{
    unsigned long t = millis();
    
    while (!cc3000->checkDHCP())
    {
        if ((millis() - t) > dhcpTimeout)
        {
            hang(F("failed to obtain dhcp lease"));
        }
        
        delay(100);
    }
    
    uint32_t ipAddress, other;
    
    if(!cc3000->getIPAddress(&ipAddress, &other, &other, &other, &other))
    {
        hang(F("failed to obtain ip address information"));
    }
    
    RCP_DEBUG("using ip address: ");

    // need to filter this out manually...
#ifdef RC_PROXY_DEBUG    
    cc3000->printIPdotsRev(ipAddress);
#endif

    RCP_DEBUG("\n");
}

void CC3000Delegate::hang(const __FlashStringHelper *str)
{
    Serial.println(str);
    for(;;);
}

#ifdef RC_PROXY_DEBUG

void CC3000Delegate::validateFirmware()
{
    uint8_t  major, minor;
    uint16_t firmware = 0;

    if (!cc3000->getFirmwareVersion(&major, &minor))
    {
        hang(F("unable to retrieve the firmware version!"));
    }

    // bleh...
    RCP_DEBUG("firmware version : ");
    RCP_DEBUG(major);
    RCP_DEBUG(".");
    RCP_DEBUG(minor);
    RCP_DEBUG("\n");

    firmware = ((uint16_t)major << 8) | minor;

    if ((firmware != 0x113) && (firmware != 0x118))
    {
        hang(F("wrong firmware revision detected!"));
    }
}

#endif
