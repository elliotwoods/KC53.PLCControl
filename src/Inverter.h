#pragma once
#include "ModbusClient.h"

class Inverter {
public:
	enum class Parameter : ParameterAddress
	{
		ParameterLock = 4
		, FrequencyRef = 5
		, RunCommand = 6
		, Acceleration = 7
		, Deceleration = 8
		, Current = 9
		, Frequency = 10
		, Voltage = 11
		, DCLink = 12
		, Power = 13
		, State = 14
	};

	enum class Direction : uint16_t
	{
		Stop = 1
		, Forwards = 2
		, Reverse = 4
	};

	void init(ModbusClient&);
	void pull();
	void printStatus() const;

	void run(const Direction&);
	void stop();
	void setFrequency(float);
	void setAcceleration(float);
	void setDecceleration(float);

private:
	ModbusClient * modbusClient;

	struct {
		uint16_t Current;
		uint16_t Frequency;
		uint16_t Voltage;
		uint16_t DCLink;
		uint16_t Power;
		uint16_t State;
	} status;
};