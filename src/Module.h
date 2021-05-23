#pragma once
#include <stdint.h>
#include "Inverter.h"
#include "ModbusClient.h"

class Module {
public:
	enum class State : uint32_t {
		Stop = 0
		, RunRight = 1
		, WaitRight = 2
		, RunLeft = 3
		, WaitLeft = 4
	};

	void init(ModbusClient&, uint8_t endstopLeft, uint8_t endstopRight);
	void updateState();
	void sendCommands();

	void stop();

	const State & getState() const;
	void setState(const State&);

	void printStatus() const;
private:
	Inverter inverter;
	State state = State::Stop;

	uint8_t endstopLeft;
	uint8_t endstopRight;
};