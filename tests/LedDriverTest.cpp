#include "CppUTest/TestHarness.h"
#include "LedDriver.h"
#include "RuntimeErrorStub.h"


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
static uint16_t virtualLeds;

TEST_GROUP(LedDriver)
{
   void setup()
   {
      LedDriver_Create(&virtualLeds);
   }

   void teardown()
   {
   }
};

TEST(LedDriver, LedsOffAfterCreate)
{
   uint16_t virtualLeds = 0xffff;
   LedDriver_Create(&virtualLeds);
   UNSIGNED_LONGS_EQUAL(0, virtualLeds);
}

TEST(LedDriver, TurnOnLedOne)
{
   LedDriver_TurnOn(1);
   UNSIGNED_LONGS_EQUAL(1, virtualLeds);
}

TEST(LedDriver, TurnOffLedOne)
{
   LedDriver_TurnOn(1);
   LedDriver_TurnOff(1);
   UNSIGNED_LONGS_EQUAL(0, virtualLeds);
}

TEST(LedDriver, TurnOnMultipleLeds)
{
   LedDriver_TurnOn(8);
   LedDriver_TurnOn(9);
   UNSIGNED_LONGS_EQUAL(0x180, virtualLeds);
}

TEST(LedDriver, TurnOnAllLeds)
{
   LedDriver_TurnAllOn();
   UNSIGNED_LONGS_EQUAL(0xffff, virtualLeds);
}

TEST(LedDriver, TurnOffAnyLed)
{
   LedDriver_TurnAllOn();
   LedDriver_TurnOff(8);
   UNSIGNED_LONGS_EQUAL(0xff7f, virtualLeds);
}

TEST(LedDriver, LedMemoryIsNotReadable)
{
   virtualLeds = 0xffff;
   LedDriver_TurnOn(8);
   UNSIGNED_LONGS_EQUAL(0x80, virtualLeds);
}

TEST(LedDriver, UpperAndLowerBounds)
{
   LedDriver_TurnOn(1);
   LedDriver_TurnOn(16);
   UNSIGNED_LONGS_EQUAL(0x8001, virtualLeds);
}

TEST(LedDriver, OutOfBoundsChangesNothing)
{
   LedDriver_TurnOn(-1);
   LedDriver_TurnOn(0);
   LedDriver_TurnOn(17);
   LedDriver_TurnOn(3141);
   UNSIGNED_LONGS_EQUAL(0, virtualLeds);
}

TEST(LedDriver, OutOfBoundsTurnOffDoesNoHarm)
{
   LedDriver_TurnAllOn();
   LedDriver_TurnOff(-1);
   LedDriver_TurnOff(0);
   LedDriver_TurnOff(17);
   LedDriver_TurnOff(3141);
   UNSIGNED_LONGS_EQUAL(0xffff, virtualLeds);
}

TEST(LedDriver, OutOfBoundsProducesRuntimeError)
{
   LedDriver_TurnOn(-1);
   STRCMP_EQUAL("LED Driver: out-of-bounds LED",
                  RuntimeErrorStub_GetLastError());
   BYTES_EQUAL(-1, RuntimeErrorStub_GetLastParameter());
}