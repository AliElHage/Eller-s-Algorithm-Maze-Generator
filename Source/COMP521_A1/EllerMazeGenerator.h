// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeGeneratorHelper.h"
#include <vector>
#include <map>

struct MazeCell
{
private:
	static long NextId;
	long UniqueId;
	long CellId;

public:
	bool bHasWallBelow = true;
	bool bHasWallRight = true;

	MazeCell(long Id);

	void SetId(std::map<long, std::vector<MazeCell*>>& CellsMap, long Id);
	long GetSetId() { return CellId; }
	long GetUniqueId() { return UniqueId; }
};

class COMP521_A1_API EllerMazeGenerator
{
private:
	// The width of the maze in number of cells
	int MazeWidth = 8;

	// The list of all maze rows generated
	//TArray<TArray<MazeCell*>> MazeRows;
	std::vector<std::vector<MazeCell*>> MazeRows;

	// A map that keeps track of the cells comprising each set
	//TMap<long, TArray<MazeCell*>> CellsById;
	std::map<long, std::vector<MazeCell*>> CellsById;

	void AddCellToIdMap(MazeCell* Cell);

public:
	EllerMazeGenerator(int Width);
	~EllerMazeGenerator();

	std::vector<MazeCell*> GenerateRow(bool bIsLast);

	FString RowToString(std::vector<MazeCell*> Row);

	FString MazeToString();

	FString CellsMapToString();
};
