# ESP8266 Adapter for LED Floods
This is the code that backs the array of lights. Each light has been modified to use the [Adafruit Huzzah](https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/pinouts) and have a connection to the IR receiver that would normally be used. Rather than add another part that shoots IR signals at the receiver, we just wire directly into where the receiver would be and send the signals.

This should be considered an incomplete work in progress until stated otherwise :)
