#pragma once

#include "stm32l4xx_hal.h"
#include <cstdint>

class ILI9341 {
private:
	static constexpr uint16_t DC = GPIO_PIN_8;
	static constexpr uint16_t RESET = GPIO_PIN_9;
	static constexpr uint16_t CS = GPIO_PIN_10;
	static constexpr uint16_t LED = GPIO_PIN_11;

	SPI_HandleTypeDef* hspi_;

	void sendCommand(uint8_t cmd);
	void sendData(uint8_t data);

public:
	ILI9341(SPI_HandleTypeDef* hspi);

	void init();
};
