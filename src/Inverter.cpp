#include "Inverter.h"

void
Inverter::init(ModbusClient& modbusClient)
{
	this->modbusClient = &modbusClient;
}

void
Inverter::pull()
{
	this->modbusClient->multiRead((uint16_t*)&this->status
		, (ParameterAddress) Parameter::Current
		, 6);
}

void
Inverter::printStatus() const
{
	char message[200];
	sprintf(message, "C : %.2f, F : %.2f, V : %.2f, D : %.2f, P : %.2f, S : %d"
		, (double) this->status.Current / 100.0
		, (double) this->status.Frequency / 100.0
		, (double) this->status.Voltage / 10.0
		, (double) this->status.DCLink / 100.0
		, (double) this->status.Power * 100.0
		, this->status.State);
	Serial.print(message);
}

void
Inverter::unlock()
{
	this->modbusClient->write((ParameterAddress) Parameter::ParameterLock
		, (uint16_t) 1);
}

void
Inverter::run(const Direction& direction)
{
	this->modbusClient->write((ParameterAddress) Parameter::RunCommand
		, (uint16_t) direction);
}

void
Inverter::stop()
{
	this->modbusClient->write((ParameterAddress) Parameter::RunCommand
		, (uint16_t) Direction::Stop);
}

void
Inverter::setFrequency(float frequency)
{
	this->modbusClient->write((ParameterAddress) Parameter::FrequencyRef
		, (uint16_t) (frequency * 100.0f));
}

void
Inverter::setAcceleration(float acceleration)
{
	this->modbusClient->write((ParameterAddress) Parameter::Acceleration
		, (uint16_t) (acceleration * 10.0f));
}

void
Inverter::setDecceleration(float decceleration)
{
	this->modbusClient->write((ParameterAddress) Parameter::Deceleration
		, (uint16_t) (decceleration * 10.0f));
}