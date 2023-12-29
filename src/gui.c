#include "raylib.h"
#include "gui.h"
#include "tracker.h"
#include <stdio.h>
#include <string.h>

// Static constants
const char checked[] = "x";
const char unchecked[] = " ";

// Static variables
static Vector2 lastPressedPos = {0};

void DrawCell(const char *text, Vector2 pos, Vector2 size, const CellStyle *style)
{
	// Draw Cell Rectangle
	DrawRectangleV(pos, size, style->color);
	DrawRectangleLinesEx((Rectangle){pos.x, pos.y, size.x, size.y}, style->borderWidth, style->borderColor);

	// Draw Cell Text
	int textWidth = MeasureText(text, style->fontSize);
	int textX = pos.x + (size.x - textWidth) / 2;
	int textY = pos.y + (size.y - style->fontSize) / 2;
	DrawText(text, textX, textY, style->fontSize, BLACK);
}

ButtonStatus DrawCellButton(const char *text, Vector2 pos, Vector2 size, CellButtonStyle style, Vector2 lastPosPressed)
{
	// Draw Cell
	DrawCell(text, pos, size, style.cellStyle);

	// Draw Button
	Rectangle cellRec = (Rectangle){pos.x, pos.y, size.x, size.y};
	return DrawButton(cellRec, BLANK, style.hoveredColor, style.pressedColor, lastPosPressed);
}

ButtonStatus DrawButton(Rectangle rec, Color normal, Color hovered, Color pressed, Vector2 lastPosPressed)
{
	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		bool wasPressedLast = CheckCollisionPointRec(lastPosPressed, rec);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && wasPressedLast)
		{
			DrawRectangleRec(rec, pressed);
			return PRESSED;
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && wasPressedLast)
		{
			return RELEASED;
		}
		DrawRectangleRec(rec, hovered);
		return HOVERED;
	}
	DrawRectangleRec(rec, normal);
	return NORMAL;
}

void DrawTable(char tableData[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t columns, size_t rows, Vector2 pos, Vector2 cellSize, CellButtonStyle style)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		lastPressedPos = GetMousePosition();
	}
	for (int col = 0; col < columns; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			int index = col + row * columns;
			char *text = tableData[index];
			Vector2 cellPos = (Vector2){pos.x + col * cellSize.x, pos.y + row * cellSize.y};
			if (TextIsEqual(text, unchecked) || TextIsEqual(text, checked))
			{
				if (RELEASED == DrawCellButton(text, cellPos, cellSize, style, lastPressedPos))
				{
					TextCopy(text, TextIsEqual(text, unchecked) ? checked : unchecked);
				}
			}
			else
			{
				DrawCell(text, cellPos, cellSize, style.cellStyle);
			}
		}
	}
}