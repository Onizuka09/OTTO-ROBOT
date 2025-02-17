
#include <WiFiServer.h>
#include <WiFiClient.h>

#include <Arduino.h>
#include <ESP32Servo.h>
#include <OscillatorESP.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include "OttoControl.h"

// Define servo pins
#define PIN_LG 25 // Left Leg
#define PIN_RG 27 // Right Leg

#define PIN_LF 26 // Left Foot
#define PIN_RF 14 // Right Foot

const char *ssid = "NETGEAR30";             // "TT_23C0";        //"TP-Link_AP_70CA";
const char *password = "Freeways20222023#"; //  "i6q4k3b21r"; //"97800903";
// Oscillator leftLeg, rightLeg, leftFoot, rightFoot;
OTTO otto;

void setup()
{

    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Attempting to connect to SSID: ");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("...");
    }
    Serial.println("");
    ArduinoOTA.onStart([]()
                       { Serial.println("Start OTA"); });

    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd OTA"); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       { Serial.printf("Error[%u]: ", error); });

    ArduinoOTA.begin();
    // ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);
    Serial.println(WiFi.localIP());

    otto.begin();
    delay(1000);
}

void loop()
{
    ArduinoOTA.handle();


    otto.stop(); // Walk 1 step at a time
    // otto.walk(1, 500,  1);

    // int A[4] = {30, 30, 20, 25};
    // int O[4] = {0, 0, 1, 2};
    // double phase_diff[4] = {0, 0, DEG2RAD(1 * -90), DEG2RAD(1 * -90)};

    // leftFoot.SetO(O[3]);
    // leftFoot.SetA(A[3]);
    // leftFoot.SetT(500);
    // leftFoot.SetPh(phase_diff[3]);
    // leftFoot.refresh();
    // rightFoot.SetO(O[2]);
    // rightFoot.SetA(A[2]);
    // rightFoot.SetT(500);
    // rightFoot.SetPh(phase_diff[2]);
    // rightFoot.refresh();

    delay(50);
}