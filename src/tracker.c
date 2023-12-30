#include "raylib.h"
#include <stdio.h>
#include "gui.h"
#include "tracker.h"

// Constants
const CellStyle defaultCellStyle = {
	.fontSize = 14,
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
static char *GetHabitText(const char name[MAX_HABIT_NAME_LENGTH], const char date[MAX_HABIT_DATE_LENGTH], const char data[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t columns, size_t rows);

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

	char *text = LoadFileText("habits/savedhabit.txt");

	char habitName[MAX_HABIT_NAME_LENGTH] = {0};
	char date[MAX_HABIT_DATE_LENGTH] = {0};
	char habitData[MAX_ENTRIES][MAX_ENTRY_LENGTH] = {0};
	size_t columns, rows;
	ParseHabitText(text, habitName, date, habitData, &columns, &rows);
	UnloadFileText(text);

	Vector2 lastPosPressed = {0};
	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			lastPosPressed = GetMousePosition();
		}
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText(habitName, 20, 20, 20, DARKGRAY);
		DrawText(date, 24, 40, 16, DARKGRAY);

		DrawTable(habitData, columns, rows, (Vector2){70, 70}, defaultCellSize, defaultButtonStyle, lastPosPressed);

		if (RELEASED == DrawTextButton("Save Data", 12, BLACK, (Rectangle){200, 20, 80, 40}, SKYBLUE, BLUE, DARKBLUE, lastPosPressed))
		{
			char *temp = GetHabitText(habitName, date, habitData, columns, rows);
			SaveFileText("habits/savedhabit.txt", temp);
		}
		if (RELEASED == DrawTextButton("Load Data", 12, BLACK, (Rectangle){300, 20, 80, 40}, GREEN, LIME, DARKGREEN, lastPosPressed))
		{
			text = LoadFileText("habits/savedhabit.txt");
			ParseHabitText(text, habitName, date, habitData, &columns, &rows);
			UnloadFileText(text);
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

static char *GetHabitText(const char name[MAX_HABIT_NAME_LENGTH], const char date[MAX_HABIT_DATE_LENGTH], const char data[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t columns, size_t rows)
{
	if (name == NULL || date == NULL || data == NULL)
		return NULL;
	// Create a variable to store the text
	// We add 2 to account for '\n' and ','. We add 4 to account for two '\n' after the name, the single '\n' after the date, and '\0'.
	static char textBuffer[MAX_ENTRIES * (MAX_ENTRY_LENGTH + 2) + MAX_HABIT_NAME_LENGTH + MAX_HABIT_DATE_LENGTH + 4] = {};
	char *textStep = textBuffer;
	// Add the name
	textStep += TextCopy(textStep, name);
	// Add two '\n'
	*textStep++ = '\n';
	*textStep++ = '\n';
	// Add the date
	textStep += TextCopy(textStep, date);
	// Add one '\n'
	*textStep++ = '\n';

	size_t currentEntry = 0;
	size_t colCounter = 0;
	size_t rowCounter = 0;
	// Add entries:
	// Check that an entry exists
	while (data[currentEntry][0] != '\0')
	{
		// Add the entry
		textStep += TextCopy(textStep, data[currentEntry]);
		// Add a ','
		*textStep++ = ',';
		// Add a '\n' if it is the last column
		if (++colCounter >= columns)
		{
			*textStep++ = '\n';
			rowCounter++;
			colCounter = 0;
			// Keep going until rows * columns entries have been added
			// If there are more entries then send a warning message
			if (rowCounter >= rows)
			{
				if (data[currentEntry + 1][0] != '\0')
					TraceLog(LOG_WARNING, "Too many entries for given columns and rows");
				break;
			}
		}

		currentEntry++;
	}
	// If there are not enough entries then send a warning message
	if (rowCounter < rows)
		TraceLog(LOG_WARNING, "Expected %llu more rows and %llu more columns", rows - rowCounter + 1, columns - colCounter);
	// Add second '\n' indicating end of data
	*textStep++ = '\n';

	// Add \0
	*textStep = '\0';

	return textBuffer;
}

static void ParseHabitText(const char *text, char name[MAX_HABIT_NAME_LENGTH], char date[MAX_HABIT_DATE_LENGTH], char data[MAX_ENTRIES][MAX_ENTRY_LENGTH], size_t *columns, size_t *rows)
{
	*columns = *rows = 0;

	bool nameSet, dateSet, dataSet, columnsSet;
	nameSet = dateSet = dataSet = columnsSet = false;

	size_t currentEntry = 0;
	size_t entryIndex = 0;

	size_t colCounter = 0;
	if (text == NULL)
	{
		TraceLog(LOG_ERROR, "ParseHabitText: Pointer to NULL");
		return;
	}

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
					TraceLog(LOG_ERROR, "ParseHabitText: Expected new line, but got %c", text[1]);
					text--;
				}
				text++;
			}
			else
				*name++ = *text;
		}
		else if (!dateSet)
		{
			if (*text == '\n')
			{
				*date = '\0';
				dateSet = true;
				while (text[1] == '\n')
				{
					TraceLog(LOG_ERROR, "ParseHabitText: Expected start of entry, but got new line");
					text++;
				}
			}
			else
				*date++ = *text;
		}
		else if (!dataSet)
		{
			if (*text == ',')
			{
				colCounter++;
				data[currentEntry++][entryIndex] = '\0';
				entryIndex = 0;
			}
			else if (*text == '\n')
			{
				{
					(*rows)++;
					if (entryIndex != 0)
					{
						TraceLog(LOG_WARNING, "ParseHabitText: Expected ',' to end last entry of row, but got new line instead", *columns, colCounter);
						data[currentEntry++][entryIndex] = '\0';
						entryIndex = 0;
						colCounter++;
					}
					if (!columnsSet)
					{
						columnsSet = true;
						*columns = colCounter;
					}
					if (colCounter != *columns)
					{
						TraceLog(LOG_ERROR, "ParseHabitText: Expected %llu columns before new line, but got %llu columns", *columns, colCounter);
						while (colCounter < *columns) // Add extra entries with INVALID string until we have enough
						{

							TextCopy(data[currentEntry++], INVALID);
							colCounter++;
						}
					}
					colCounter -= *columns;
				}					 // In case we have too many columns they will count towards the next row.
				if (text[1] == '\n') // The data for this habit and date is done
				{
					while (colCounter != 0)
					{
						(*rows)++;
						if (entryIndex != 0)
						{
							TraceLog(LOG_WARNING, "ParseHabitText: Expected ',' to end last entry of row, but got new line instead", *columns, colCounter);
							data[currentEntry++][entryIndex] = '\0';
							entryIndex = 0;
							colCounter++;
						}
						if (!columnsSet)
						{
							columnsSet = true;
							*columns = colCounter;
						}
						if (colCounter != *columns)
						{
							TraceLog(LOG_ERROR, "ParseHabitText: Expected %llu columns before new line, but got %llu columns", *columns, colCounter);
							while (colCounter < *columns) // Add extra entries with INVALID string until we have enough
							{

								TextCopy(data[currentEntry++], INVALID);
								colCounter++;
							}
						}
						colCounter -= *columns;
					}
					data[currentEntry][entryIndex] = '\0'; // Empty string indicates end of entries
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
	if (!nameSet)
		TraceLog(LOG_ERROR, "Got to end of habit file, before setting name");
	if (!dateSet)
		TraceLog(LOG_ERROR, "Got to end of habit file, before setting date");
	if (!dataSet)
		TraceLog(LOG_ERROR, "Got to end of habit file, before setting all data");
}