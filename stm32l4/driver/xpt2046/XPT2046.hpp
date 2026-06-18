#pragma once

#include "stm32l4xx_hal.h"
#include <cstdint>

struct XPT2046PinConf {
	uint16_t pin;
	GPIO_TypeDef* port;
};

struct TouchPoint {
	uint16_t x;
	uint16_t y;
};

class XPT2046 {
private:
	uint8_t commands[6] = {0xD0, 0, 0, 0x90, 0, 0};
	uint8_t buf_[6] = { 0 };

	SPI_HandleTypeDef* hspi_;
	XPT2046PinConf cs_;
	XPT2046PinConf irq_;

	void sendData(uint16_t adcX, uint16_t adcY);

public:
	XPT2046(SPI_HandleTypeDef* hspi, XPT2046PinConf cs, XPT2046PinConf irq);

	TouchPoint readCoordinates();
};
