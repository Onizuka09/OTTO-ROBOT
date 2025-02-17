#ifndef RING_BUFF 
#define RING_BUFF
#define RING_BUFFER_SIZE 255

typedef enum
{
  Forward = 0,
  Backword,
  Right,
  Left,
  IDL = -1
} Actions_e;

typedef struct
{
  Actions_e buffer[RING_BUFFER_SIZE];
  int head; // Points to the next item to remove
  int tail; // Points to the next free slot
} RingBuffer_t;


bool AddAction(Actions_e act);
Actions_e GetAction();
void initMutext(); 

#endif 