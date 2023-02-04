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
