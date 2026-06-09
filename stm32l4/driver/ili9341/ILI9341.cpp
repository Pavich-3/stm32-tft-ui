#include "ILI9341.hpp"

ILI9341::ILI9341(SPI_HandleTypeDef* hspi, ILI9341PinConf dc, ILI9341PinConf reset, ILI9341PinConf cs) :
	hspi_(hspi), dc_(dc), reset_(reset), cs_(cs) {}
