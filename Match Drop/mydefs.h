// Change this file in any way you like
// Do not alter driver.c / Makefile / md.h
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

#define BRDSZ 6
#define MAXBRDS 200000
#define MAXLAYER 15
#define MAXSTR (BRDSZ*BRDSZ+BRDSZ+2)

struct board{
   //2D array of tiles
   char arr[BRDSZ][BRDSZ];
   //hawk
   char hawk;
   //parent
   struct board* parent;
   // current layer
   int layer;
   // board's index
   int index;
   // check create board;
   bool isCreateNextBoard;
   // start index of the next board 
   int nextPeriodStartIndex;
   // current board's parent index
   int parentIndex;
};

typedef struct board board;

struct state{
    // all board
    board* boardArr[MAXBRDS];
    // check whether the solution is found
    bool findSolution;
    // count of all layer
    int numberOfLayer; 
    // count of all layer
    int numberOfBoard;
    // board 's rows
    int rows;
    // board 's cols
    int cols;
    // number of moves of solution board
    int moves;
    // solution's index
    int solutionIndex;
};
typedef struct state state;

#include "md.h"
