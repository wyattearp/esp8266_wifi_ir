#include <FS.h>
#include <ESP8266WiFi.h>

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#define CONFIG_FILE "/config.json"

bool g_shouldSaveConfig = false;
bool g_readyToRoll = false;

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

    Serial.begin(115200);
    Serial.println("ESP8266 wifi IR starting up...");
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

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

    // items we'll need for multicast to work right
    //WiFiManagerParameter multicast_address("multicast_address", "Multicast Address", )

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