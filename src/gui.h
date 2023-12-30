#pragma once
#include <stdio.h>
#include "tracker.h"

// Enums
// typedef enum VAlign
// {
// 	VALIGN_TOP,
// 	VALIGN_CENTER,
// 	VALIGN_BOTTOM
// } VAlign;

// typedef enum HAlign
// {
// 	HALIGN_LEFT,
// 	HALIGN_CENTER,
// 	HALIGN_RIGHT
// } HAlign;

typedef enum ButtonStatus
{
	NORMAL,
	HOVERED,
	PRESSED,
	RELEASED,
} ButtonStatus;

// Structs
typedef struct CellStyle
{
	int fontSize;
	// HAlign textHAlign;
	// VAlign textVAlign;
	int borderWidth;
	Color borderColor;
	Color color;
} CellStyle;

typedef struct CellButtonStyle
{
	const CellStyle *cellStyle;
	Color hoveredColor;
	Color pressedColor;
} CellButtonStyle;

// Function Declarations
void DrawCell(const char *text, Vector2 pos, Vector2 size, const CellStyle *style, Color textColor);
ButtonStatus DrawCellButton(const char *text, Vector2 pos, Vector2 size, CellButtonStyle style, Vector2 lastPosPressed, Color textColor);
ButtonStatus DrawButton(Rectangle rec, Color normal, Color hovered, Color pressed, Vector2 lastPosPressed);
ButtonStatus DrawTextButton(const char *text, int fontSize, Color textColor, Rectangle rec, Color normal, Color hovered, Color pressed, Vector2 lastPosPressed);
void DrawTable(char tableData[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t columns, size_t rows, Vector2 pos, Vector2 cellSize, CellButtonStyle style, Vector2 lastPosPressed);