#pragma once

#include "stm32l4xx_hal.h"
#include <cstdint>

struct ILI9341PinConf {
	uint16_t pin;
	GPIO_TypeDef* port;
};

class ILI9341 {
private:
	static constexpr uint32_t pixelNumbers = 240 * 320;

	SPI_HandleTypeDef* hspi_;
	ILI9341PinConf dc_;
	ILI9341PinConf reset_;
	ILI9341PinConf cs_;

	void sendCommand(uint8_t cmd);
	void sendData(uint8_t data);
	void writeBlock(uint16_t color, uint32_t count);

public:
	ILI9341(SPI_HandleTypeDef* hspi, ILI9341PinConf dc, ILI9341PinConf reset, ILI9341PinConf cs);

	void init();
	void drawPixel(uint16_t x, uint16_t y, uint16_t color);
	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	void fillScreen(uint16_t color);
};
