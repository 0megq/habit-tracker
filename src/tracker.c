#include "raylib.h"
#include <stdio.h>
#include "gui.h"

#define MAX_ROWS 10

const CellStyle defaultCellStyle = {
	.fontSize = 16,
	.textHAlign = HALIGN_CENTER,
	.textVAlign = VALIGN_CENTER,
	.cellWidth = 60,
	.cellHeight = 40,
	.borderWidth = 1,
	.borderColor = GOLD,
	.color = LIGHTGRAY,
};

// Program main entry point
int main(void)
{
	// Initialization
	const int screenWidth = 800;
	const int screenHeight = 450;

	char *const text = LoadFileText("habits/testhabit.txt");
	UnloadFileText(text);
	InitWindow(screenWidth, screenHeight, "Habit Tracker");

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawCell("hello", 80, 40, &defaultCellStyle);
		DrawCell("goop", 80 + defaultCellStyle.cellWidth - defaultCellStyle.borderWidth, 40, &defaultCellStyle);
		DrawCell("dooo", 80 + defaultCellStyle.cellWidth - defaultCellStyle.borderWidth, 40 + defaultCellStyle.cellHeight - defaultCellStyle.borderWidth, &defaultCellStyle);
		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}