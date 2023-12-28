#include "raylib.h"
#include "gui.h"

void DrawCell(const char *text, int posX, int posY, const CellStyle *style)
{
	// Draw Cell Rectangle
	DrawRectangle(posX, posY, style->cellWidth, style->cellHeight, style->color);
	DrawRectangleLinesEx((Rectangle){posX, posY, style->cellWidth, style->cellHeight}, style->borderWidth, style->borderColor);

	// Draw Cell Text
	int textWidth = MeasureText(text, style->fontSize);
	int textX = posX + (style->cellWidth - textWidth) / 2;
	int textY = posY + (style->cellHeight - style->fontSize) / 2;
	DrawText(text, textX, textY, style->fontSize, BLACK);
}