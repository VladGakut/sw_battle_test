#include "Map.hpp"
#include <algorithm>
#include <cmath>

namespace sw::core
{
	Map::Map(uint32_t width, uint32_t height) 
		: _width(width), _height(height)
	{
		_grid.resize(width * height);
	}

	bool Map::isValidPosition(uint32_t x, uint32_t y) const
	{
		return x < _width && y < _height;
	}

	bool Map::isCellOccupied(uint32_t x, uint32_t y) const
	{
		if (!isValidPosition(x, y))
			return false;
		
		size_t index = y * _width + x;
		return _grid[index].has_value();
	}

	std::optional<uint32_t> Map::getUnitAt(uint32_t x, uint32_t y) const
	{
		if (!isValidPosition(x, y))
			return std::nullopt;
		
		size_t index = y * _width + x;
		return _grid[index];
	}

	bool Map::placeUnit(uint32_t unitId, uint32_t x, uint32_t y)
	{
		if (!isValidPosition(x, y) || isCellOccupied(x, y))
			return false;
		
		size_t index = y * _width + x;
		_grid[index] = unitId;
		return true;
	}

	bool Map::moveUnit(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY)
	{
		// Проверяем, что исходная клетка содержит нужный юнит
		if (!isValidPosition(fromX, fromY) || getUnitAt(fromX, fromY) != unitId)
			return false;
		
		// Проверяем, что целевая клетка свободна
		if (!isValidPosition(toX, toY) || isCellOccupied(toX, toY))
			return false;
		
		// Освобождаем исходную клетку
		size_t fromIndex = fromY * _width + fromX;
		_grid[fromIndex] = std::nullopt;
		
		// Занимаем целевую клетку
		size_t toIndex = toY * _width + toX;
		_grid[toIndex] = unitId;
		
		return true;
	}

	bool Map::removeUnit(uint32_t x, uint32_t y)
	{
		if (!isValidPosition(x, y) || !isCellOccupied(x, y))
			return false;
		
		size_t index = y * _width + x;
		_grid[index] = std::nullopt;
		return true;
	}

	std::vector<std::pair<uint32_t, uint32_t>> Map::getAdjacentCells(uint32_t x, uint32_t y) const
	{
		std::vector<std::pair<uint32_t, uint32_t>> adjacent;
		
		// Проверяем все 8 соседних клеток
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				if (dx == 0 && dy == 0) continue; // Пропускаем саму клетку
				
				uint32_t newX = x + dx;
				uint32_t newY = y + dy;
				
				if (isValidPosition(newX, newY))
				{
					adjacent.emplace_back(newX, newY);
				}
			}
		}
		
		return adjacent;
	}

	std::vector<std::pair<uint32_t, uint32_t>> Map::getCellsInRange(uint32_t x, uint32_t y, uint32_t minRange, uint32_t maxRange) const
	{
		std::vector<std::pair<uint32_t, uint32_t>> cells;
		
		for (uint32_t targetX = 0; targetX < _width; ++targetX)
		{
			for (uint32_t targetY = 0; targetY < _height; ++targetY)
			{
				// Пропускаем саму клетку
				if (targetX == x && targetY == y)
					continue;
				
				// Вычисляем расстояние (манхэттенское расстояние)
				uint32_t distance = std::abs(static_cast<int>(targetX) - static_cast<int>(x)) + 
								   std::abs(static_cast<int>(targetY) - static_cast<int>(y));
				
				if (distance >= minRange && distance <= maxRange)
				{
					cells.emplace_back(targetX, targetY);
				}
			}
		}
		
		return cells;
	}
}