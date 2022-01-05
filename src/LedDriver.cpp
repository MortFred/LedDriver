#include "LedDriver.h"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

enum {ALL_LEDS_ON = ~0, ALL_LEDS_OFF = ~ALL_LEDS_ON};
enum {FIRST_LED = 1, LAST_LED = 16};

static uint16_t * ledsAddress;
static uint16_t ledsImage;


// ***** Helper functions *****
static uint16_t convertLedNumberToBit(int ledNumber)
{
	return 1 << (ledNumber - 1);	
}

static void updateHardware()
{
	*ledsAddress = ledsImage;
}

static bool isLedOutOfBounds(int ledNumber)
{
	return (ledNumber < FIRST_LED || ledNumber > LAST_LED);
}

static void setLedImageBit(int ledNumber)
{
	ledsImage |= convertLedNumberToBit(ledNumber);
}

static void clearLedImageBit(int ledNumber)
{
	ledsImage &= ~convertLedNumberToBit(ledNumber);
}

// ****************************


void LedDriver_Create(uint16_t * address)
{
	ledsAddress = address;
	ledsImage = ALL_LEDS_OFF;
	updateHardware();
}

void LedDriver_Destroy(void)
{

}

void LedDriver_TurnOn(int ledNumber)
{
	if (isLedOutOfBounds(ledNumber))
		throw out_of_range("LED Driver: out-of-bounds LED: " + to_string(ledNumber));

	setLedImageBit(ledNumber);
	updateHardware();
}

void LedDriver_TurnOff(int ledNumber)
{
	if (isLedOutOfBounds(ledNumber))
		throw out_of_range("LED Driver: out-of-bounds LED: " + to_string(ledNumber));
	
	clearLedImageBit(ledNumber);
	updateHardware();
}

void LedDriver_TurnAllOn(void)
{	
	ledsImage = ALL_LEDS_ON;
	updateHardware();
}

void LedDriver_TurnAllOff(void)
{
	ledsImage = ALL_LEDS_OFF;
	updateHardware();
}

bool LedDriver_IsOn(int ledNumber)
{
	if (isLedOutOfBounds(ledNumber))
		return false;
	
	return ledsImage & (convertLedNumberToBit(ledNumber));
}

bool LedDriver_IsOff(int ledNumber)
{	
	return !LedDriver_IsOn(ledNumber);
}


