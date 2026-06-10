#include "ILI9341.hpp"
#include <cmath>

ILI9341::ILI9341(SPI_HandleTypeDef* hspi, ILI9341PinConf dc, ILI9341PinConf reset, ILI9341PinConf cs) :
	hspi_(hspi), dc_(dc), reset_(reset), cs_(cs) {}

void ILI9341::sendCommand(uint8_t cmd) {
	HAL_GPIO_WritePin(dc_.port, dc_.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_, &cmd, 1, 1000);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_SET);
}

void ILI9341::sendData(uint8_t data) {
	HAL_GPIO_WritePin(dc_.port, dc_.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi_, &data, 1, 1000);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_SET);
}

void ILI9341::init() {
	HAL_GPIO_WritePin(reset_.port, reset_.pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(reset_.port, reset_.pin, GPIO_PIN_SET);
	HAL_Delay(120);

	sendCommand(0x01);  // Software Reset

	sendCommand(0xEF);
	sendData(0x03);
	sendData(0x80);
	sendData(0x02);

	sendCommand(0xCF);
	sendData(0x00);
	sendData(0xC1);
	sendData(0x30);

	sendCommand(0xED);
	sendData(0x64);
	sendData(0x03);
	sendData(0x12);
	sendData(0x81);

	sendCommand(0xE8);
	sendData(0x85);
	sendData(0x00);
	sendData(0x78);

	sendCommand(0xCB);
	sendData(0x39);
	sendData(0x2C);
	sendData(0x00);
	sendData(0x34);
	sendData(0x02);

	sendCommand(0xF7);
	sendData(0x20);

	sendCommand(0xEA);
	sendData(0x00);
	sendData(0x00);

	sendCommand(0xC0);  // Power Control 1
	sendData(0x23);

	sendCommand(0xC1);  // Power Control 2
	sendData(0x10);

	sendCommand(0xC5);  // VCOM Control 1
	sendData(0x3E);
	sendData(0x28);

	sendCommand(0xC7);  // VCOM Control 2
	sendData(0x86);

	sendCommand(0x36);  // Memory Access Control
	sendData(0x08);

	sendCommand(0x3A);  // Pixel Format — 16 bit
	sendData(0x55);

	sendCommand(0xB1);  // Frame Rate
	sendData(0x00);
	sendData(0x18);

	sendCommand(0xB6);  // Display Function Control
	sendData(0x08);
	sendData(0x82);
	sendData(0x27);

	sendCommand(0xF2);  // Gamma disable
	sendData(0x00);

	sendCommand(0x26);  // Gamma curve
	sendData(0x01);

	sendCommand(0x11);  // Sleep Out
	HAL_Delay(120);

	sendCommand(0x29);  // Display ON
}

void ILI9341::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	uint8_t x_high = (x >> 8) & 0xFF;
	uint8_t x_low = x & 0xFF;
	uint8_t y_high = (y >> 8) & 0xFF;
	uint8_t y_low = y & 0xFF;

	sendCommand(0x2A);
	sendData(x_high);
	sendData(x_low);
	sendData(x_high);
	sendData(x_low);

	sendCommand(0x2B);
	sendData(y_high);
	sendData(y_low);
	sendData(y_high);
	sendData(y_low);

	sendCommand(0x2C);
	writeBlock(color, 1);
}

void ILI9341::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t dx = std::abs(x1 - x0);
	uint16_t dy = std::abs(y1 - y0);
	int8_t sx = (x0 < x1) ? 1 : -1;
	int8_t sy = (y0 < y1) ? 1 : -1;


	int16_t err = dx - dy;
	while (true) {
		drawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		int16_t err2 = 2 * err;

		if (err2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (err2 < -dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void ILI9341::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	uint16_t x1 = x + w;
	uint16_t y1 = y + h;

	drawLine(x, y,  x1, y, color);
	drawLine(x, y, x, y1, color);
	drawLine(x, y1, x1, y1, color);
	drawLine(x1, y, x1, y1, color);
}

void ILI9341::writeBlock(uint16_t color, uint32_t count) {
	HAL_GPIO_WritePin(dc_.port, dc_.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_RESET);
	uint8_t msb = (color >> 8) & 0xFF;
	uint8_t lsb = color & 0xFF;
	for (uint32_t i = 0; i < count; ++i) {
		HAL_SPI_Transmit(hspi_, &msb, 1, 1000);
		HAL_SPI_Transmit(hspi_, &lsb, 1, 1000);
	}
	HAL_GPIO_WritePin(cs_.port, cs_.pin, GPIO_PIN_SET);
}

void ILI9341::fillScreen(uint16_t color) {
	sendCommand(0x2A);
	sendData(0);
	sendData(0);
	sendData(0);
	sendData(0xEF);

	sendCommand(0x2B);
	sendData(0);
	sendData(0);
	sendData(0x01);
	sendData(0x3F);

	sendCommand(0x2C);
	writeBlock(color, pixelNumbers);
}
