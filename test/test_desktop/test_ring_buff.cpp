#include <Arduino.h>
#include <unity.h>
#include "ringBuff.h"
extern RingBuffer_t buffAct;
extern SemaphoreHandle_t buffMutex;

void setUp()
{
    memset(&buffAct, 0, sizeof(RingBuffer_t));
    buffAct.head= 0 ;
    buffAct.tail= 0 ;

    buffMutex = xSemaphoreCreateMutex();
}

void tearDown()
{
    vSemaphoreDelete(buffMutex);
}
// TEST:  testing and retriving action
void test_add_get_action()
{
    setUp();
    TEST_ASSERT_TRUE(AddAction(Forward));
    TEST_ASSERT_EQUAL(Forward, GetAction());
    // tearDown();
}

// TEST: Adding multiple actions and retrieving them in FIFO oerder

void test_fifo_order()
{
    setUp();

    TEST_ASSERT_TRUE(AddAction(Forward));
    TEST_ASSERT_TRUE(AddAction(Left));
    TEST_ASSERT_TRUE(AddAction(Right));

    TEST_ASSERT_EQUAL(Forward, GetAction());
    TEST_ASSERT_EQUAL(Left, GetAction());
    TEST_ASSERT_EQUAL(Right, GetAction());
    // tearDown();
}

// TEST: Buffer shoudl handle overflow
void test_buffer_overflow()
{
    setUp();

    for (int i = 0; i < RING_BUFFER_SIZE -1 ; i++)
    {
        TEST_ASSERT_TRUE(AddAction(Forward));
        Serial.printf("Count %d\n\r",i);
    }
    TEST_ASSERT_FALSE(AddAction(Forward));
    // tearDown();
}

// TEST: Buffer underflow
void test_buffer_underflow()
{
    setUp();

    TEST_ASSERT_EQUAL(IDL, GetAction());
    // tearDown();
}

void setup()
{
    UNITY_BEGIN();
    Serial.begin(115200); 
    RUN_TEST(test_add_get_action);
    RUN_TEST(test_fifo_order);
    RUN_TEST(test_buffer_overflow);
    RUN_TEST(test_buffer_underflow);

    UNITY_END();
}
void loop()
{
}