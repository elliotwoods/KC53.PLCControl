#include <Arduino.h>
#include "Module.h"
#include "Controllino.h"
#include "ModbusRTU.h"
#include "ModbusClient.h"


#define MODULE_COUNT 1

#define RS485Serial 3
Modbus modbusMaster(0, RS485Serial, 0);

ModbusClient modbusClients[MODULE_COUNT];
Module modules[MODULE_COUNT];

// A debug LED flashes alternatively on/off every loop
bool ledValue = LOW;

void
setup()
{
	// Fill in configurations for all the individual modbus clients here
	for(uint8_t i=0; i<MODULE_COUNT; i++) {
		modbusClients[i].init(modbusMaster, i + 1);
		modules[i].init(modbusClients[i]
			, CONTROLLINO_A1 + i * 2
			, CONTROLLINO_A2 + i * 2);

		// Everybody starts with go right
		modules[i].setState(Module::State::RunRight);
	}
	

	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(115200);
}

void
updateState()
{
	for(uint8_t i=0; i<MODULE_COUNT; i++) {
		modules[i].updateState();
	}

	// Handle left end wait
	{
		bool allWaitingAtLeft = true;
		for(uint8_t i=0; i<MODULE_COUNT; i++) {
			allWaitingAtLeft &= modules[i].getState() == Module::State::WaitLeft;
		}
		if(allWaitingAtLeft) {
			for(uint8_t i=0; i<MODULE_COUNT; i++) {
				modules[i].setState(Module::State::RunRight);
			}
		}
	}

	// Handle right end wait
	{
		bool allWaitingAtRight = true;
		for(uint8_t i=0; i<MODULE_COUNT; i++) {
			allWaitingAtRight &= modules[i].getState() == Module::State::WaitRight;
		}
		if(allWaitingAtRight) {
			for(uint8_t i=0; i<MODULE_COUNT; i++) {
				modules[i].setState(Module::State::RunLeft);
			}
		}
	}
}

void
sendCommands()
{
	for(uint8_t i=0; i<MODULE_COUNT; i++) {
		modules[i].sendCommands();
	}
}

void
printDebug()
{
	for(uint8_t i=0; i<MODULE_COUNT; i++) {
		char message[100];
		sprintf(message, "[%d] : ", i);
		Serial.print(message);
		modules[i].printStatus();
		Serial.println();
	}
}

void
flashLED()
{
	// LED indicator
	{
		ledValue = !ledValue;
		digitalWrite(LED_BUILTIN, ledValue);
	}
}

void
loop()
{
	updateState();

	sendCommands();

	printDebug();

	delay(100);
}