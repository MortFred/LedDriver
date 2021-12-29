#include "CppUTest/TestHarness.h"

/*
LED driver tests:

All LEDs are off after the driver is initialized.
A single LED can be turned on.
A single LED can be turned off.
Multiple LEDs can be turned on/off.
Turn on all LEDs.
Turn off all LEDs.
Query LED state.
Check boundary values.
check out-of-bounds values.
*/ 

TEST_GROUP(LedDriver)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(LedDriver, LedsOffAfterCreate)
{
   // FAIL("Start here!");

}