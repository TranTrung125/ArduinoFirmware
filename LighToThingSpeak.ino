#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <LCD_I2C.h>

//LCD config
#define LCD_I2C_ADDRESS 0x27
LCD_I2C lcd(LCD_I2C_ADDRESS);

//Sensor setting
#define DIGITAL_PIN D2
#define ANALOG_PIN A0
int sensorValue(0);
int sensorState(0);

// Wi-Fi Settings
ESP8266WiFiMulti wifiMulti;
#define SSID01_NAME "CunCon"
#define SSID01_PASS "23456789"
#define SSID02_NAME "Thu Vien Cafe Dong Tay"
#define SSID02_PASS ""
WiFiClient client;

// ThingSpeak Settings
#define CHANNEL_ID        1284769               //Channel ID on Thingspeak.com
#define WRITE_API_KEY     "455ITCJJAYIT4ERB"    // write API key for your ThingSpeak Channel
#define READ_API_KEY      "OKPJ1XPEMFXS9F46"    // read API key for your ThingSpeak Channel
#define SERVER            "api.thingspeak.com"  // host address
#define POSTING_INTERVAL  2000                  // post data every 2 seconds

void setup() {
        Serial.begin(115200);
        lcd.begin();
        lcd.backlight();
        pinMode(ANALOG_PIN, INPUT);
        pinMode(DIGITAL_PIN, INPUT);       
        WiFi.mode(WIFI_STA);
        wifiMulti.addAP(SSID01_NAME, SSID01_PASS);
        wifiMulti.addAP(SSID02_NAME, SSID02_PASS);
        Serial.print("\r\nConnecting Wifi AccessPoint");    
        lcd.print("Connecting......");
        while (wifiMulti.run() != WL_CONNECTED) {
          Serial.print(".");
          delay(300);
        }
        Serial.printf("\r\nWiFi connected. IP address: %s\n",WiFi.localIP().toString().c_str());
        lcd.clear();
        lcd.print("Connected");
        lcd.setCursor(0,1);
        lcd.print(WiFi.localIP());
}

void loop() {
        // wait and then post again
        delay(POSTING_INTERVAL);
        //Read data from sensor
        int sensorValue = analogRead(ANALOG_PIN);
        int sensorState = digitalRead(DIGITAL_PIN);                
        if (client.connect(SERVER, 80)) {
                // Construct API request body
                String body = "field1=" + String(sensorValue) + "&field2=" + String(sensorValue) + "&field3=" + String(sensorState) + "&field4=" + String(sensorValue);
                client.print("POST /update HTTP/1.1\n");
                client.print("Host: api.thingspeak.com\n");
                client.print("Connection: close\n");
                client.printf("X-THINGSPEAKAPIKEY: %s \n", WRITE_API_KEY);
                client.print("Content-Type: application/x-www-form-urlencoded\n");
                client.print("Content-Length: ");
                client.print(body.length());
                client.print("\n\n");
                client.print(body);
                client.print("\n\n");
                Serial.printf("Light: %i Value: %i\r\n", sensorState, sensorValue);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.printf("Light: %i",sensorState);
                lcd.setCursor(0,1);
                lcd.printf("Value: %i",sensorValue);
        }
        client.stop();
}
