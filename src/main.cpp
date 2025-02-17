#include <Arduino.h>
#include <WiFi.h>

#include <ESP32Servo.h>
#include <BluetoothSerial.h>
#include "ringBuff.h"
#include "OttoControl.h"
OTTO otto;

BluetoothSerial SerialBT;

void BLTask(void *parameters);

void setup()
{
  SerialBT.begin("OTTO"); // Bluetooth device name
  Serial.begin(115200);
  TaskHandle_t BLTaskHandler;
  otto.begin();

  xTaskCreate(BLTask,           // Function to run as a thread
              "Bluetooth Task", // Name of the task
              4096,             // Stack size in words
              NULL,             // Task input parameter
              3,                // Task priority
              &BLTaskHandler    // Task handle (optional)
  );
  initMutext();
}
Actions_e tmp;
void loop()
{
  tmp = GetAction();
  if (tmp != IDL)
  {
    Serial.printf("Executing Action: %d\n\r", tmp);
  }
  switch (tmp)
  {
  case Forward:
    otto.walk(1, 500, 1);
    break;
  case Backword:
    otto.walk(1, 500, -1);
    break;
  case Left:
    otto.turn(1, 500, -1); // Turn left by 1 step
    break;
  case Right:
    otto.turn(1, 500, 1); // Turn left by 1 step
    break;
  default:
    otto.stop();
    break;
  }
  delay(50);
}
void BLTask(void *parameters)
{
  Serial.println("Running BL task NOW...");

  char cmd = ' ';
  while (1)
  {
    if (SerialBT.available())
    {
      cmd = SerialBT.read();
      if (cmd == '\n' || cmd == '\r')
      {
        continue;
      }

      // Serial.printf("Received CMD: %c\n\r", cmd);

      switch (cmd)
      {
      case 'F':
        Serial.println("Forward");
        AddAction(Forward);
        break;
      case 'B':
        Serial.println("Backward");
        AddAction(Backword);
        break;
      case 'R':
        Serial.println("Right");
        AddAction(Right);
        break;
      case 'L':
        Serial.println("Left");
        AddAction(Left);
        break;
      default:
        // Serial.printf("Error: Invalid Command %c\n\r", cmd);
        cmd = 0;
        break;
      }
    }

    vTaskDelay(20 / portTICK_PERIOD_MS); // Proper FreeRTOS delay
  }
}
