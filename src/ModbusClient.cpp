#include "ModbusClient.h"

modbus_t modbusQuery;

void ModbusClient::init(Modbus &modbus, uint8_t modbusAddress)
{
	this->modbus = &modbus;
	this->modbusAddress = modbusAddress;
}

bool ModbusClient::read(uint16_t &value, const ParameterAddress &parameterAddress)
{
	return this->multiRead(&value, parameterAddress, 1);
}

bool ModbusClient::multiRead(uint16_t *values, const ParameterAddress &address, uint16_t size)
{
	modbusQuery.u8id = this->modbusAddress;		   // ModbusClient address
	modbusQuery.u8fct = 3;						   // function code (this one is registers read)
	modbusQuery.u16RegAdd = (uint16_t)address - 1; // start address in ModbusClient
	modbusQuery.u16CoilsNo = size;				   // number of elements (coils or registers) to read
	modbusQuery.au16reg = values;				   // pointer to a memory array in the CONTROLLINO

	this->modbus->query(modbusQuery);

	return this->waitForResponse();
}

bool
ModbusClient::write(const ParameterAddress &parameterAddress, const uint16_t & value)
{
	uint16_t values[1] = {value};

	modbusQuery.u8id = this->modbusAddress;			// slave address
	modbusQuery.u8fct = 6;				// function code (this one is write a single register)
	modbusQuery.u16RegAdd = (uint16_t)parameterAddress - 1; // start address in slave
	modbusQuery.u16CoilsNo = 1;			// number of elements (coils or registers) to write
	modbusQuery.au16reg = values;		// pointer to a memory array in the CONTROLLINO

	this->modbus->query(modbusQuery);
	return this->waitForResponse();
}

bool
ModbusClient::waitForResponse()
{
	auto state = this->modbus->getState();
	int tryIndex = 0;
	while (state == COM_WAITING)
	{
		if (tryIndex++ > maxTries)
		{
			return false;
		}
		delay(pollDelay);
		this->modbus->poll();
		state = this->modbus->getState();
	}
	return true;
}