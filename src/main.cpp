#include <ESP8266WiFi.h>

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


void setup()
{
    Serial.begin(115200);
    Serial.println("ESP8266 wifi IR starting up...");
    WiFiManager wifiManager;
    wifiManager.autoConnect();

}

void loop()
{
  digitalWrite(0, HIGH);

  delay(1000);

  digitalWrite(0, LOW);

  delay(1000);
}