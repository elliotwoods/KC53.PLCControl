#pragma once
#include <stdint.h>
#include "ModbusRtu.h"

typedef uint16_t ParameterAddress;

class ModbusClient {
public:
	void init(Modbus&, uint8_t modbusAddress);

	bool read(uint16_t&, const ParameterAddress&);
	bool multiRead(uint16_t*, const ParameterAddress& startAddress, uint16_t size);

	bool write(const ParameterAddress&, const uint16_t& value);
	
private:
	bool waitForResponse();
	uint8_t modbusAddress;
	int maxTries = 50;
	int pollDelay = 10;
	Modbus * modbus = nullptr;
};