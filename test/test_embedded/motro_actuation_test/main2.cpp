#include <WiFiServer.h>
#include <WiFiClient.h>

#include <Arduino.h>
#include <ESP32Servo.h>
#include <OscillatorESP.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

// Define servo pins
#define PIN_LG 25 // Left Leg
#define PIN_RG 27 // Right Leg

#define PIN_LF 26 // Left Foot
#define PIN_RF 14 // Right Foot

const char *ssid = "NETGEAR30"; // "TT_23C0";        //"TP-Link_AP_70CA";
const char *password ="Freeways20222023#"; //  "i6q4k3b21r"; //"97800903";
Oscillator leftLeg, rightLeg, leftFoot, rightFoot;
class OTTO
{
private:
    const int neutralPosition = 90; // Center position
    const int stepDelay = 100;      // Delay for smooth movement
    bool restState = true;

public:
    void begin()
    {
        // Attach servos to their GPIO pins
        attachServos();

        // Move servos to neutral position
        resetPosition();
        delay(1000); // Allow servos to stabilize
    }

    void attachServos()
    {
        leftLeg.attach(PIN_LG);
        rightLeg.attach(PIN_RG);
        leftFoot.attach(PIN_LF);
        rightFoot.attach(PIN_RF);
    }

    void resetPosition()
    {
        leftLeg.SetPosition(neutralPosition);
        rightLeg.SetPosition(neutralPosition);
        leftFoot.SetPosition(neutralPosition);
        rightFoot.SetPosition(neutralPosition);
    }

    void setRestState(bool state)
    {
        restState = state;
    }

    bool getRestState()
    {
        return restState;
    }

    void walk(float steps, int T, int dir)
    {

        //-- Oscillator parameters for walking
        //-- Hip sevos are in phase
        //-- Feet servos are in phase
        //-- Hip and feet are 90 degrees out of phase
        //--      -90 : Walk forward
        //--       90 : Walk backward
        //-- Feet servos also have the same offset (for tiptoe a little bit)
        // int A[4] = {20, 20, 10, 10};
        // int O[4] = {0, 0, 0, 4};
        int A[4] = {30, 30, 20, 25};
        int O[4] = {0, 0, 1, 2};
        double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

        //-- Let's oscillate the servos!
        _execute(A, O, T, phase_diff, steps);
    }

    void _execute(int A[4], int O[4], int T, double phase_diff[4], float steps = 1.0)
    {

        if (getRestState() == true)
        {
            setRestState(false);
        }

        int cycles = (int)steps;

        //-- Execute complete cycles
        if (cycles >= 1)
            for (int i = 0; i < cycles; i++)
                oscillateServos(A, O, T, phase_diff);

        //-- Execute the final not complete cycle
        oscillateServos(A, O, T, phase_diff, (float)steps - cycles);
    }

    void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle = 1)
    {
        // Assign oscillation parameters to each servo
        rightLeg.SetO(O[0]);
        rightLeg.SetA(A[0]);
        rightLeg.SetT(T);
        rightLeg.SetPh(phase_diff[0]);

        leftLeg.SetO(O[1]);
        leftLeg.SetA(A[1]);
        leftLeg.SetT(T);
        leftLeg.SetPh(phase_diff[1]);

        rightFoot.SetO(O[2]);
        rightFoot.SetA(A[2]);
        rightFoot.SetT(T);
        rightFoot.SetPh(phase_diff[2]);

        leftFoot.SetO(O[3]);
        leftFoot.SetA(A[3]);
        leftFoot.SetT(T);
        leftFoot.SetPh(phase_diff[3]);

        double ref = millis();
        for (double x = ref; x <= T * cycle + ref; x = millis())
        {
            leftLeg.refresh();
            rightLeg.refresh();
            leftFoot.refresh();
            rightFoot.refresh();
        }
    }

    void turn(float steps, int T, int dir)
    {

        //-- Same coordination than for walking (see Otto::walk)
        //-- The Amplitudes of the hip's oscillators are not igual
        //-- When the right hip servo amplitude is higher, the steps taken by
        //--   the right leg are bigger than the left. So, the robot describes an
        //--   left arc
        int A[4] = {0, 0, 20, 10};
        int O[4] = {0, 0, 4, 4};
        double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)};

        if (dir == 1)
        {
            A[0] = 40; //-- Left hip servo
            A[1] = 20; //-- Right hip servo
        }
        else
        {
            A[0] = 10;
            A[1] = 40;
        }

        //-- Let's oscillate the servos!
        _execute(A, O, T, phase_diff, steps);
    }
    void moonwalker(float steps, int T, int h, int dir)
    {

        //-- This motion is similar to that of the caterpillar robots: A travelling
        //-- wave moving from one side to another
        //-- The two Otto's feet are equivalent to a minimal configuration. It is known
        //-- that 2 servos can move like a worm if they are 120 degrees out of phase
        //-- In the example of Otto, the two feet are mirrored so that we have:
        //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
        //--  is 60 degrees.
        //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
        //-   offset so that the robot tiptoe lightly
        int h1 = 10 ; 
        int h2 = 15 ; 
        int A[4] = {0, 0, h1, h2};
        int O[4] = {0, 0,  1, 2 };
        int phi = -dir * 90;
        double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};

        //-- Let's oscillate the servos!
        _execute(A, O, T, phase_diff, steps);
    }
};

// Create robot instance
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
unsigned long previousMillis = 0;
const int movementInterval = 500; // Adjust timing for each movement step
int stepCount = 0;
int currentAction = 0; // 0: walk, 1: turn left, 2: turn right

void loop()
{
    ArduinoOTA.handle();

    // unsigned long currentMillis = millis();

    // if (currentMillis - previousMillis >= movementInterval)
    // {
    //     previousMillis = currentMillis;

    //     switch (currentAction)
    //     {
    //     case 0:                    // Walking forward
    //         otto.walk(10, 500, 1); // Walk 1 step at a time

    //         currentAction = 1;

    //         break;

    //     case 1:                    // Turn left
    //         otto.turn(10, 500, 1); // Turn left by 1 step

    //         currentAction = 2;

    //         break;

    //     case 2:                     // Turn right
    //         otto.turn(10, 500, -1); // Turn right by 1 step

    //         currentAction = 0;

    //         break;
    //     }
    // }
    otto.walk(1, 700, 1); // Walk 1 step at a time
    // otto.moonwalker(1, 500, 30 , 1);

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
