#pragma once

// Enums
typedef enum VAlign
{
	VALIGN_TOP,
	VALIGN_CENTER,
	VALIGN_BOTTOM
} VAlign;

typedef enum HAlign
{
	HALIGN_LEFT,
	HALIGN_CENTER,
	HALIGN_RIGHT
} HAlign;

// Structs
typedef struct CellStyle
{
	int fontSize;
	HAlign textHAlign;
	VAlign textVAlign;
	int cellHeight;
	int cellWidth;
	int borderWidth;
	Color borderColor;
	Color color;
} CellStyle;

// Function Declarations
void DrawCell(const char *text, int posX, int posY, const CellStyle *style);