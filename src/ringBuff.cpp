#include "ringBuff.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>



RingBuffer_t buffAct = {{IDL}, 0, 0};
SemaphoreHandle_t buffMutex;

void initMutext(){ 
    memset(&buffAct, 0, sizeof(RingBuffer_t));
    buffAct.head= 0 ;
    buffAct.tail= 0 ;

    buffMutex = xSemaphoreCreateMutex();

} 

bool AddAction(Actions_e act)
{
  // lock the shared resouces to insert data
  xSemaphoreTake(buffMutex, portMAX_DELAY); // Lock

  int nextHead = (buffAct.head + 1) % RING_BUFFER_SIZE; // Next position
  if (nextHead == buffAct.tail)
  {
    // Buffer is full, drop command
    Serial.println("Buffer Full! Dropping action. ");
    Serial.println(nextHead); 
    xSemaphoreGive(buffMutex);
    return false;
  }

  buffAct.buffer[buffAct.head] = act;
  buffAct.head = nextHead;

  xSemaphoreGive(buffMutex); // Unlock
  return true;
}


Actions_e GetAction()
{
  xSemaphoreTake(buffMutex, portMAX_DELAY); // Lock

  if (buffAct.head == buffAct.tail) { 
    xSemaphoreGive(buffMutex); // Unlock
    return IDL;  // Buffer is empty
  }

  Actions_e action = buffAct.buffer[buffAct.tail];
  buffAct.tail = (buffAct.tail + 1) % RING_BUFFER_SIZE; // Move tail forward

  xSemaphoreGive(buffMutex); // Unlock
  return action;
}
