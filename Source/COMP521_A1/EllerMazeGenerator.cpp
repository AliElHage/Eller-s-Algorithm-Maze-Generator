// Fill out your copyright notice in the Description page of Project Settings.

#include "EllerMazeGenerator.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include <iostream>

long MazeCell::NextId = 0;

MazeCell::MazeCell(long Id)
{
	CellId = Id;
	UniqueId = NextId++;
}

void MazeCell::SetId(std::map<long, std::vector<MazeCell*>>& CellsMap, long Id)
{
	// Set the Id the cell belongs to and update all the other cell's Ids if they
	// belong to the same set.
	if (Id != CellId)
	{
		if (CellsMap.count(CellId) > 0)
		{
			std::vector<MazeCell*>& currentSet = CellsMap[CellId];
			for (int i = currentSet.size() - 1; i >= 0; i--)
			{
				if (currentSet[i]->GetUniqueId() == GetUniqueId())
				{
					currentSet.erase(currentSet.begin() + i);

					if (CellsMap.count(Id) == 0)
					{
						CellsMap[Id] = std::vector<MazeCell*>();
					}

					CellsMap[Id].push_back(this);
				}
			}
		}

		CellId = Id;
	}
}

EllerMazeGenerator::EllerMazeGenerator(int Width)
{
	MazeWidth = Width;
	MazeRows = std::vector<std::vector<MazeCell*>>();
	CellsById = std::map<long, std::vector<MazeCell*>>();
}

EllerMazeGenerator::~EllerMazeGenerator()
{
}

void EllerMazeGenerator::AddCellToIdMap(MazeCell* Cell)
{
	// We add the new cell to a map that keeps track of the cells per Id.
	int key = Cell->GetSetId();
	if (CellsById.count(key) == 0)
	{
		CellsById[key] = std::vector<MazeCell*>();
	}

	CellsById[key].push_back(Cell);
}

std::vector<MazeCell*> EllerMazeGenerator::GenerateRow(bool bIsLast = false)
{
	std::vector<MazeCell*> Row;

	// If it's the first row, generate isolated cells
	if (MazeRows.size() == 0)
	{
		Row = std::vector<MazeCell*>();
		for (int i = 0; i < MazeWidth; i++)
		{
			MazeCell* mazeCell = new MazeCell(i);
			AddCellToIdMap(mazeCell);
			Row.push_back(mazeCell);
		}
	}
	// Otherwise, extend the cells without a bottom wall and generate the others by giving them
	// a set ID currently not in use.
	else
	{
		std::vector<MazeCell*> PreviousRow = MazeRows[MazeRows.size() - 1];
		Row = std::vector<MazeCell*>();
		for (int i = 0; i < MazeWidth; i++)
		{
			MazeCell* mazeCell;
			if (!PreviousRow[i]->bHasWallBelow)
			{
				mazeCell = new MazeCell(PreviousRow[i]->GetSetId());
				AddCellToIdMap(mazeCell);
				Row.push_back(mazeCell);
			}
			else
			{
				int emptySetId = 0;
				while (CellsById.count(emptySetId) == 0 || CellsById[emptySetId].size() != 0)
				{
					emptySetId++;
				}

				mazeCell = new MazeCell(emptySetId);
				AddCellToIdMap(mazeCell);
				Row.push_back(mazeCell);
			}
		}
	}

	// Randomly decide whether or not to join two adjacent cells from different sets
	for (int i = 0; i < MazeWidth - 1; i++)
	{
		if (Row[i]->GetSetId() != Row[i + 1]->GetSetId())
		{
			bool bShouldJoin = UKismetMathLibrary::RandomBool() || bIsLast;
			if (bShouldJoin)
			{
				Row[i]->bHasWallRight = false;

				int cellId = Row[i + 1]->GetSetId();
				Row[i + 1]->SetId(CellsById, Row[i]->GetSetId());

				for (int j = CellsById[cellId].size() - 1; j >= 0; j--)
				{
					CellsById[cellId][j]->SetId(CellsById, Row[i]->GetSetId());
				}
			}
		}
	}

	// Randomly open lower walls while making sure that each set is extended at least once. Start
	// by constructing a map of all cells of the same set in the row.
	std::map<long, std::vector<MazeCell*>> rowCellsBySetId = std::map<long, std::vector<MazeCell*>>();
	for (int i = 0; i < Row.size(); i++)
	{
		long currentId = Row[i]->GetSetId();
		if (rowCellsBySetId.count(currentId) == 0)
		{
			rowCellsBySetId[currentId] = std::vector<MazeCell*>();
		}

		rowCellsBySetId[currentId].push_back(Row[i]);
	}

	// For each set in the row, guarantee that it extends once.
	for (auto &elem : rowCellsBySetId)
	{
		int numberInRow = elem.second.size();
		int guaranteedExtensionIndex = UKismetMathLibrary::RandomIntegerInRange(0, numberInRow - 1);
		elem.second[guaranteedExtensionIndex]->bHasWallBelow = false || bIsLast;
	}

	// Reiterate through the row and randomly choose whether or not to extend each wall except those
	// that have been already extended (see above).
	for (int i = 0; i < Row.size(); i++)
	{
		bool bShouldExtend = UKismetMathLibrary::RandomBool();
		if (Row[i]->bHasWallBelow)
		{
			Row[i]->bHasWallBelow = bShouldExtend || bIsLast;
		}
	}

	MazeRows.push_back(Row);

	return Row;
}

FString EllerMazeGenerator::RowToString(std::vector<MazeCell*> Row)
{
	// This is for debugging purposes
	FString cellRow = FString("|");
	FString lowerWallRow = FString("|");

	for (int i = 0; i < Row.size(); i++)
	{
		cellRow += FString::FromInt(Row[i]->GetSetId());
		if (Row[i]->bHasWallRight)
		{
			cellRow += "|";
		}
		else
		{
			cellRow += " ";
		}
		
		if (Row[i]->bHasWallBelow)
		{
			lowerWallRow += "-";
		}
		else
		{
			lowerWallRow += " ";
		}

		lowerWallRow += "|";
	}

	return cellRow + "\n" + lowerWallRow;
}

FString EllerMazeGenerator::CellsMapToString()
{
	// This is for debugging purposes
	FString mapContents = FString("\n");
	for (auto elem : CellsById)
	{
		mapContents += "key: ";
		mapContents += FString::FromInt(elem.first);
		mapContents += "; value(s): ";
		for (int i = 0; i < elem.second.size(); i++)
		{
			mapContents += FString::FromInt(elem.second[i]->GetSetId());
		}

		mapContents += "\n";
	}

	return mapContents;
}

FString EllerMazeGenerator::MazeToString()
{
	// This is for debugging purposes
	FString mazeString = FString(LINE_TERMINATOR);
	for (int i = 0; i < MazeRows.size(); i++)
	{
		mazeString += RowToString(MazeRows[i]);
		mazeString += "\n";
	}

	return mazeString;
}
