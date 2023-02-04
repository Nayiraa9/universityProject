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

