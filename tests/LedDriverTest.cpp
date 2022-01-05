#include "CppUTest/TestHarness.h"
#include "LedDriver.h"
#include "RuntimeErrorStub.h"
#include <stdexcept>


/*
LED driver tests:

X All LEDs are off after the driver is initialized.
X A single LED can be turned on.
X A single LED can be turned off.
X Multiple LEDs can be turned on/off.
X Turn on all LEDs.
X Turn off all LEDs.
Query LED state.
Check boundary values.
X check out-of-bounds values.
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

TEST(LedDriver, TurnOffMultipleLeds)
{
   LedDriver_TurnAllOn();
   LedDriver_TurnOff(8);
   LedDriver_TurnOff(9);
   UNSIGNED_LONGS_EQUAL((~0x180)&0xffff, virtualLeds);
}

TEST(LedDriver, TurnOnAllLeds)
{
   LedDriver_TurnAllOn();
   UNSIGNED_LONGS_EQUAL(0xffff, virtualLeds);
}

TEST(LedDriver, TurnOffAllLeds)
{
   LedDriver_TurnAllOff();
   UNSIGNED_LONGS_EQUAL(0, virtualLeds);
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

IGNORE_TEST(LedDriver, OutOfBoundsChangesNothing)
{
   LedDriver_TurnOn(-1);
   LedDriver_TurnOn(0);
   LedDriver_TurnOn(17);
   LedDriver_TurnOn(3141);
   UNSIGNED_LONGS_EQUAL(0, virtualLeds);
}

IGNORE_TEST(LedDriver, OutOfBoundsTurnOffDoesNoHarm)
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
   CHECK_THROWS(std::out_of_range, LedDriver_TurnOn(-1));
   CHECK_THROWS(std::out_of_range, LedDriver_TurnOff(-1));
}

TEST(LedDriver, IsOn)
{
   CHECK_FALSE(LedDriver_IsOn(11));
   LedDriver_TurnOn(11);
   CHECK(LedDriver_IsOn(11));
}

TEST(LedDriver, IsOff)
{
   CHECK(LedDriver_IsOff(12));
   LedDriver_TurnOn(12);
   CHECK_FALSE(LedDriver_IsOff(12));
}

TEST(LedDriver, OutOfBoundsLedsAreAlwaysOff)
{
   CHECK_FALSE(LedDriver_IsOn(-1));
   CHECK_FALSE(LedDriver_IsOn(19));
   CHECK(LedDriver_IsOff(-1));
   CHECK(LedDriver_IsOff(19));
}
