#include "raylib.h"
#include "gui.h"
#include "tracker.h"
#include <stdio.h>
#include <string.h>

void DrawCell(const char *text, Vector2 pos, Vector2 size, const CellStyle *style, Color textColor)
{
	// Draw Cell Rectangle
	DrawRectangleV(pos, size, style->color);
	DrawRectangleLinesEx((Rectangle){pos.x, pos.y, size.x, size.y}, style->borderWidth, style->borderColor);

	// Draw Cell Text
	int textWidth = MeasureText(text, style->fontSize);
	int textX = pos.x + (size.x - textWidth) / 2;
	int textY = pos.y + (size.y - style->fontSize) / 2;
	DrawText(text, textX, textY, style->fontSize, textColor);
}

ButtonStatus DrawCellButton(const char *text, Vector2 pos, Vector2 size, CellButtonStyle style, Vector2 lastPosPressed, Color textColor)
{
	// Draw Cell
	DrawCell(text, pos, size, style.cellStyle, textColor);

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

ButtonStatus DrawTextButton(const char *text, int fontSize, Color textColor, Rectangle rec, Color normal, Color hovered, Color pressed, Vector2 lastPosPressed)
{
	ButtonStatus status = DrawButton(rec, normal, hovered, pressed, lastPosPressed);
	int textWidth = MeasureText(text, fontSize);
	int posX = rec.x + rec.width / 2 - textWidth / 2;
	int posY = rec.y + rec.height / 2 - fontSize / 2;
	DrawText(text, posX, posY, fontSize, textColor);
	return status;
}

void DrawTable(char tableData[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t columns, size_t rows, Vector2 pos, Vector2 cellSize, CellButtonStyle style, Vector2 lastPosPressed)
{
	for (int col = 0; col < columns; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			int index = col + row * columns;
			char *text = tableData[index];
			Vector2 cellPos = (Vector2){pos.x + col * cellSize.x, pos.y + row * cellSize.y};
			if (TextIsEqual(text, UNCHECKED) || TextIsEqual(text, CHECKED))
			{
				if (RELEASED == DrawCellButton(text, cellPos, cellSize, style, lastPosPressed, BLACK))
				{
					TextCopy(text, TextIsEqual(text, UNCHECKED) ? CHECKED : UNCHECKED);
				}
			}
			else if (TextIsEqual(text, INVALID))
			{
				DrawCell(text, cellPos, cellSize, style.cellStyle, RED);
			}
			else
			{
				DrawCell(text, cellPos, cellSize, style.cellStyle, BLACK);
			}
		}
	}
}