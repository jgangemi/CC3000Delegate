#ifndef CC3000DELEGATE_H
#define CC3000DELEGATE_H

#ifdef RC_PROXY_DEBUG
#define RCP_DEBUG(string) (Serial.print(string))
#endif

#ifndef RC_PROXY_DEBUG
#define RCP_DEBUG(string)
#endif

#include <Adafruit_CC3000.h>
#include <Client.h>

class CC3000Delegate
{
    private:
        Adafruit_CC3000 *cc3000;

        char *ssid;
        char *key;
        uint8_t secMode;

    public:

        CC3000Delegate(char* ssid, uint8_t csPin = 10, uint8_t irqPin = 3, uint8_t vbatPin = 5, uint8_t spiSpeed = SPI_CLOCK_DIVIDER);
        
        bool connected();
        void disconnect();

        Adafruit_CC3000* getCC3000();
        Client* getClient();
        
        void initialize();
        void setAPCredentials(char *key, uint8_t secMode);

    private:
        void connectToAP();
        void dhcp();
        void hang(const __FlashStringHelper *str);
        
    #ifdef RC_PROXY_DEBUG    
        void validateFirmware();
    #endif
};

#endif
