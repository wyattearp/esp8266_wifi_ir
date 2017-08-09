#include <FS.h>
#include <ESP8266WiFi.h>

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include "config_item.h"

#define CONFIG_FILE "/config.bin"

bool g_shouldSaveConfig = false;
bool g_readyToRoll = false;

char g_multicast_addr[] = "239.0.0.100";
char g_multicast_port[] = "44100";

void saveConfigCallback()
{
    Serial.println("Saving configuration");
    g_shouldSaveConfig = true;
}

void setup()
{
    File config_file;
    size_t config_file_size;
    char* buffer = NULL;
    WiFiManager wifi_manager;
    config_data* conf = NULL;

    Serial.begin(115200);
    Serial.println("ESP8266 wifi IR starting up...");
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    // items we'll need for multicast to work right
    WiFiManagerParameter multicast_address("multicast_addr", "Multicast Address", g_multicast_addr, sizeof("255.255.255.255"));
    WiFiManagerParameter multicast_port("multicast_port", "Multicast Port", g_multicast_port, sizeof("65535"));
    //set config save notify callback
    wifi_manager.setSaveConfigCallback(saveConfigCallback);
    // custom parameters
    wifi_manager.addParameter(&multicast_address);
    wifi_manager.addParameter(&multicast_port);

    if (!SPIFFS.begin()) {
        goto error_path;
    }

    Serial.println("Mounted SPIFFS filesystem");
    if (!(SPIFFS.exists(CONFIG_FILE))) {
        goto no_config_loaded;
    }

    if (!(config_file = SPIFFS.open(CONFIG_FILE, "r")) ) {
        Serial.println("Unable to open file even though it's there.");
        //TODO: remove file;
        goto no_config_loaded;
    }

    Serial.println("Configuration opened...");
    config_file_size = config_file.size();
    buffer = (char*)malloc(sizeof(char) * config_file_size);
    if (!buffer) {
        Serial.println("Unable to allocate space");
        goto no_config_loaded;
    }

no_config_loaded:
    // startup up the wifi / portal
    wifi_manager.autoConnect();

    // inform the rest of the sytem we're ready to begin processing
    g_readyToRoll = true;

error_path:
    if (buffer) {
        free(buffer);
    }

    if (g_readyToRoll) {
        Serial.println("Everything looks good ... let's kick this pig!");
    } else {
        Serial.println("Something is horribly wrong");
    }
}

void loop()
{
    if (!g_readyToRoll) {
        Serial.println("Abandon all hope!");
        digitalWrite(0, HIGH);
        delay(500);
        digitalWrite(0, LOW);
        delay(500);
    }
}