#include "Module.h"
#include <Arduino.h>

void
Module::init(ModbusClient& modbusClient, uint8_t endstopLeft, uint8_t endstopRight)
{
	this->inverter.init(modbusClient);
	this->endstopLeft = endstopLeft;
	this->endstopRight = endstopRight;

	pinMode(this->endstopLeft, INPUT);
	pinMode(this->endstopRight, INPUT);
}

void
Module::updateState()
{
	if(digitalRead(this->endstopLeft) && this->state == State::RunLeft) {
		this->setState(State::WaitLeft);
	}
	if(digitalRead(this->endstopRight) && this->state == State::RunRight) {
		this->setState(State::WaitRight);
	}
}

void
Module::sendCommands()
{
	switch(this->state) {
		case State::Stop:
			this->inverter.stop();
			break;
		case State::RunRight:
			this->inverter.run(Inverter::Direction::Reverse);
			break;
		case State::WaitRight:
			this->inverter.stop();
			break;
		case State::RunLeft:
			this->inverter.run(Inverter::Direction::Forwards);
			break;
		case State::WaitLeft:
			this->inverter.stop();
			break;
	}

	// Get the debug info from the inverter
	this->inverter.pull();
}

void
Module::stop()
{
	this->state = State::Stop;
}

const Module::State &
Module::getState() const
{
	return this->state;
}

void
Module::setState(const State& state)
{
	this->state = state;
}

void
Module::printStatus() const
{
	Serial.print("State : ");
	Serial.print((uint32_t) this->state);
	Serial.print(",");
	this->inverter.printStatus();
}