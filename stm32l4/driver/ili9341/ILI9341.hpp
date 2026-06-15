#pragma once

#include "stm32l4xx_hal.h"
#include <cstdint>

struct ILI9341PinConf {
	uint16_t pin;
	GPIO_TypeDef* port;
};

class ILI9341 {
private:
	static constexpr uint16_t rows = 320;
	static constexpr uint16_t cols = 240;
	static constexpr uint32_t maxDMATransmit = 32767;
	static constexpr uint16_t maxIter = 128;
	static constexpr uint32_t pixelNumbers = static_cast<uint32_t>(rows) * static_cast<uint32_t>(cols);

	uint16_t lineBuf[cols] = {0};

	SPI_HandleTypeDef* hspi_;
	ILI9341PinConf dc_;
	ILI9341PinConf reset_;
	ILI9341PinConf cs_;

	void sendCommand(uint8_t cmd);
	void sendData(uint8_t data);
	void writeBlock(uint16_t color, uint32_t count);
	void writeDMA(const uint16_t* buf, uint32_t count);
	void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

public:
	ILI9341(SPI_HandleTypeDef* hspi, ILI9341PinConf dc, ILI9341PinConf reset, ILI9341PinConf cs);

	void init();
	void drawPixel(uint16_t x, uint16_t y, uint16_t color);
	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	void drawCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color);
	void drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color);
	void drawString(uint16_t x, uint16_t y, const char* str, uint16_t color);
	void drawImage(const uint16_t* img, uint16_t w, uint16_t h);
	void drawMandelbrot(float centerX, float centerY, float scale);
	void fillScreen(uint16_t color);
};
