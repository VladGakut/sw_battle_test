#pragma once

#include <cstdint>
#include <vector>
#include <optional>

namespace sw::core
{
	class Map
	{
	private:
		uint32_t _width{};
		uint32_t _height{};
		std::vector<std::optional<uint32_t>> _grid; // unitId или nullopt если клетка пуста

	public:
		Map() = default;
		Map(uint32_t width, uint32_t height);

		// Основные методы
		bool isValidPosition(uint32_t x, uint32_t y) const;
		bool isCellOccupied(uint32_t x, uint32_t y) const;
		std::optional<uint32_t> getUnitAt(uint32_t x, uint32_t y) const;
		
		// Управление юнитами на карте
		bool placeUnit(uint32_t unitId, uint32_t x, uint32_t y);
		bool moveUnit(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY);
		bool removeUnit(uint32_t x, uint32_t y);
		
		// Геттеры
		uint32_t getWidth() const { return _width; }
		uint32_t getHeight() const { return _height; }
		
		// Поиск соседних клеток
		std::vector<std::pair<uint32_t, uint32_t>> getAdjacentCells(uint32_t x, uint32_t y) const;
		std::vector<std::pair<uint32_t, uint32_t>> getCellsInRange(uint32_t x, uint32_t y, uint32_t minRange, uint32_t maxRange) const;
	};
}