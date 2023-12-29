#include "raylib.h"
#include <stdio.h>
#include "gui.h"
#include "tracker.h"

// Constants
const CellStyle defaultCellStyle = {
	.fontSize = 16,
	// .textHAlign = HALIGN_CENTER,
	// .textVAlign = VALIGN_CENTER,
	.borderWidth = 1,
	.borderColor = DARKGRAY,
	.color = SKYBLUE,
};

const CellButtonStyle defaultButtonStyle = {
	.cellStyle = &defaultCellStyle,
	.hoveredColor = (Color){200, 200, 200, 100},
	.pressedColor = (Color){80, 80, 80, 100},
};

const Vector2 defaultCellSize = {80, 40};

// Static declarations
static void ParseHabitText(const char *text, char name[MAX_HABIT_NAME_LENGTH], char date[MAX_HABIT_DATE_LENGTH], char data[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t *columns, size_t *rows);

// Program main entry point
int main(void)
{
	// Initialization
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Habit Tracker");

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	SetTraceLogLevel(LOG_DEBUG);

	char *const text = LoadFileText("habits/testhabit.txt");

	char habitName[MAX_HABIT_NAME_LENGTH] = {};
	char date[MAX_HABIT_DATE_LENGTH] = {};
	char habitData[MAX_ENTRIES][MAX_ENTRY_LENGTH] = {};
	size_t columns, rows;
	columns = rows = 0;
	ParseHabitText(text, habitName, date, habitData, &columns, &rows);

	UnloadFileText(text);
	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText(habitName, 20, 20, 20, DARKGRAY);
		DrawTable(habitData, columns, rows, (Vector2){70, 70}, defaultCellSize, defaultButtonStyle);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

static void ParseHabitText(const char *text, char name[MAX_HABIT_NAME_LENGTH], char date[MAX_HABIT_DATE_LENGTH], char data[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t *columns, size_t *rows)
{
	bool nameSet, dateSet, dataSet, columnsSet;
	nameSet = dateSet = dataSet = columnsSet = false;

	size_t currentEntry = 0;
	size_t entryIndex = 0;

	size_t colCounter = 0;

	while (*text != '\0')
	{
		if (!nameSet)
		{
			if (*text == '\n')
			{
				*name = '\0';
				nameSet = true;
				if (text[1] != '\n')
				{
					TraceLog(LOG_ERROR, "ParseHabitText Error: Expected new line, but got %c", text[1]);
					return;
				}
				text++;
			}
			*name++ = *text;
		}
		else if (!dateSet)
		{
			if (*text == '\n')
			{
				*date = '\0';
				dateSet = true;
				if (text[1] == '\n')
				{
					TraceLog(LOG_ERROR, "ParseHabitText Error: Expected start of entry, but got new line");
					return;
				}
			}
			*date++ = *text;
		}
		else if (!dataSet)
		{
			if (*text == ',')
			{
				colCounter++;
				data[currentEntry][entryIndex] = '\0';
				entryIndex = 0;
				currentEntry++;
			}
			else if (*text == '\n')
			{
				(*rows)++;
				if (!columnsSet)
				{
					columnsSet = true;
					*columns = colCounter;
				}
				if (colCounter != *columns)
				{
					TraceLog(LOG_ERROR, "ParseHabitText Error: Expected %llu columns before new line, but got %llu columns", columns, colCounter);
					return;
				}
				colCounter = 0;
				if (text[1] == '\n')
				{
					dataSet = true;
				}
			}
			else
			{
				data[currentEntry][entryIndex++] = *text;
			}
		}
		text++;
	}
}