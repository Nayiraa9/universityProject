#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <stdbool.h>
// for getch
#include <conio.h>
/**
 * @brief Content struct
 * 
 * @param name
 * @param content
 * @param lines
 * @param line_count
 * 
 */
typedef struct
{
    char* name;
    char* content;
    char** lines;
    int line_count;
} Content;

/**
 * @brief Position struct
 * 
 * @param x
 * @param y
 * 
 */
typedef struct
{
    unsigned int x;
    unsigned int y;
} Position;

/**
 * @brief Mode enum
 * 
 * @param MODE_VISUAL
 * @param MODE_INSERT
 * @param MODE_COMMAND
 * 
 */
typedef enum
{
    MODE_VISUAL,
    MODE_INSERT,
    MODE_COMMAND
} Mode;
// Global variables
Content* buffer;
Mode mode = MODE_VISUAL;

/**
 * @brief Get the window position
 * 
 * @return Position
 */
Position getWindowPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Position p = {csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
    return p;
}
/**
 * @brief Get the current cursor Position
 * 
 * @return Position
 */
Position getCursorPosition()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Position p = {csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y};
    return p;
}
/**
 * @brief Set the cursor position
 * 
 * @param x
 * @param y
 */
void setCursorPosition(unsigned int x, unsigned int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/**
 * @brief Get the number of columns
 * 
 * @return unsigned int
 */
unsigned int getColumns()
{
    Position p = getWindowPosition();
    return p.x;
}
/**
 * @brief Get the number of rows
 * 
 * @return unsigned int
 */
unsigned int getRows()
{
    Position p = getWindowPosition();
    return p.y;
}
/**
 * @brief Clear the screen
 *
 * @return void
 */
void clearScreen()
{
    system("cls");
}
/**
 * @brief Get center position of the screen
 * 
 * @return Position
 */
Position getCenter()
{
    Position size = getWindowPosition();

    unsigned int columns = size.x;
    unsigned int rows = size.y;

    unsigned int x = columns / 2;
    unsigned int y = rows / 2;

    Position p = {x, y};
    return p;
}
/**
 * @brief Print in the center of the screen
 * 
 * @param text
 * 
 * @return void
 */
void printCenter(char* text)
{
    Position center = getCenter();

    setCursorPosition(center.x, center.y);
    printf("%s", text);
}
void printAt(char* text, int row)
{
    int length = strlen(text);
    if (length == 0) return;

    setCursorPosition(0, row);

    printf("%s", text);
}

void printCenterAt(char* text, int row)
{
    int length = strlen(text);
    if (length == 0) return;

    Position center = getCenter();

    int x = center.x - (length / 2);
    setCursorPosition(x, row);

    printf("%s", text);
}

/**
 * @brief Print the welcome message
 * 
 * @return void
 */
void printWelcomeMessage()
{
    Position center = getCenter();

    printCenterAt("Welcome to MyEditor!", center.y);

    printCenterAt("Press Ctrl + Q to quit.", center.y + 1);
}

void changeMode(Mode mode)
{
    switch (mode) {
        case MODE_COMMAND:
            printAt("Command mode", 0);
            break;
        case MODE_VISUAL:
            printAt("Visual mode", 0);
            break;
        case MODE_INSERT:
            printAt("Insert mode", 0);
            break;
        default:
            break;
    }
}

void fileMove(int x, int y)
{
    return;

    Position p = getCursorPosition();
    int columns = getColumns();
    int rows = getRows();

    if (x < 0) x = 0;
    if (x > columns) x = columns;

    if (y < 0) y = 0;
    if (y > rows) y = rows;

    setCursorPosition(x, y);
}

void printContent()
{
    int columns = getColumns();
    int rows = getRows() - 2;
    setCursorPosition(0, 2);

    int line_count = buffer->line_count;
    char** lines = buffer->lines;

    for (int i = 0; i < line_count; i++) {
        char* line = lines[i];
        int length = strlen(line);

        if (length > columns) {
            char* line2 = malloc(sizeof(char)*columns);
            strncpy(line2, line, columns);
            line2[columns] = '\0';
            printf("%s\n", line2);
            free(line2);
        } else {
            printf("%s\n", line);
        }
    }

    for (int i = line_count; i < rows; i++) {
        printf("\n");
    }

    setCursorPosition(0, 2);
}

void printToolbar()
{
    if (buffer == NULL) printCenterAt("Welcome", 0);
    else printCenterAt(buffer->name, 0);
    printf("\n");
}

void printSubToolbar()
{
    printf("\n");
}

// example: skip_string("max base", 3+1) -> "base")
char* skip_string(char* input, int length)
{
    length++;

    int original_length = strlen(input);
    if (length > original_length) return NULL;

    char* output = malloc(sizeof(char)*(original_length - length + 1));
    strcpy(output, input + length);
    return output;
}

int fileExists(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    fclose(file);
    return 1;
}

void openFileContent(char* filename)
{
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }

    if (fileExists(filename) == 0) return;

    FILE* file = fopen(filename, "r");
    if (file == NULL) return;

    buffer = malloc(sizeof(Content));
    buffer->name = filename;
    buffer->line_count = 0;
    buffer->lines = NULL;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int length = strlen(line);
        if (length > 0) {
            if (line[length - 1] == '\n') {
                line[length - 1] = '\0';
            }

            buffer->line_count++;
            buffer->lines = realloc(buffer->lines, sizeof(char*)*buffer->line_count);
            buffer->lines[buffer->line_count - 1] = malloc(sizeof(char)*length);
            strcpy(buffer->lines[buffer->line_count - 1], line);

            printf("%s\n", line);
        }
    }

    fclose(file);
}

void saveContent()
{
    if (buffer == NULL) return;

    FILE* file = fopen(buffer->name, "w");
    if (file == NULL) return;

    int line_count = buffer->line_count;
    char** lines = buffer->lines;

    for (int i = 0; i < line_count; i++) {
        char* line = lines[i];
        fprintf(file, "%s\n", line);
    }

    fclose(file);
}

void clearLine()
{
    printf("\33[2K\r");
}
