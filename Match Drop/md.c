#include "mydefs.h"

bool file2str(const char* fname, char* str);
state* str2state(const char* str);
int solve(state* s, bool verbose);
void test(void);
void getRowAndCol(const char* str, state*s, int len);
board* createNewBoard(state* s, board* parentBoard, int colIndex);
bool match(state*s, board* board);
state *expandBoard(state* s);
void printSolution(state*s);
void printSingleBoard(state *s, int boardIndex);
bool checkLayerPlus(int index, int cols, int layer);
void freeAllBoard(state*s);
int findFirstHyphenIndex(const char* str);
int findLastIndex(int index, char* str);
int getFirstHyphenIndex(char* str);
int getSecondHyphenIndex(char* str, int len ,int firstHyphenIndex);
bool checkStrFormat(char*str);

bool file2str(const char* fname, char* str)
{
   if(fname == NULL || str == NULL){
      return false;
   }
   
   FILE*fp = fopen(fname, "r");
   if(fp == NULL){
      fprintf(stderr, "cannot read the file %s \n", fname);
      return false;
   }

   // create space to receive the every rows of character
   int charRange = MAXSTR;
   char* tempStr = (char*)calloc(charRange, sizeof(char));

   if (tempStr == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      fclose(fp);
      free(tempStr);
      return NULL;
   }

   // count of every character
   int index = 0; 
   while(fgets(tempStr, charRange, fp) != NULL){
      int len = (int)strlen(tempStr);
      for(int i = 0; i < len;i++){
         if(tempStr[i] == '\n'){
            str[index] = '-';
         }else{
            str[index] = tempStr[i];
         }
         index++;
      }
   }

   // find the last index
   int lastIndex = findLastIndex(index, str);
   str[lastIndex+1] = '\0';
   fclose(fp);
   free(tempStr);
   
   // check string format is right 
   if(!checkStrFormat(str)){
      return false;
   }
   
   return true;
}

state* str2state(const char* str)
{
   if(str == NULL){
      return NULL;
   }

   // create state    
   state* s = (state*)calloc(1, sizeof(state));
   s->boardArr[0] = (board*)calloc(1, sizeof(board));
   
   // get column and row
   int len = (int)strlen(str);
   getRowAndCol(str, s, len);

   int firstHyphenIndex = findFirstHyphenIndex(str);
   // A-(start from this hyphen)
   int index = firstHyphenIndex + 1; 
   // create board 
   for(int row = 0; row < s->rows; row++){
      for(int col = 0; col < s->cols; col++){
         //printf("index = %i \n", index);
         if(str[index] == '-'){
            index++;
         }
         s->boardArr[0]->arr[row][col] = str[index];
         index++;
      }
   }

   s->boardArr[0]->hawk =  str[0];
   s->boardArr[0]->parent = NULL;
   s->boardArr[0]->isCreateNextBoard = false;
   s->boardArr[0]->index = 0;
   s->boardArr[0]->nextPeriodStartIndex = (s->boardArr[0]->index) * (s->cols) +1;
   s->boardArr[0]->parentIndex = -1;
   s->numberOfBoard = 1;
   s->numberOfLayer = 0;
   s->findSolution = false;
   s->solutionIndex = 0;
   return s;
}

int solve(state* s, bool verbose)
{
   int result = 0;
   s = expandBoard(s);
   result = s->moves;
   if(verbose){
      printSolution(s);
   }
   // free all board
   freeAllBoard(s);
   return result;
}

void test(void)
{
   char str[MAXSTR];
   state* s;

   // check string format
   assert(checkStrFormat("AAA-ABC-ABC")==false);
   assert(checkStrFormat("A-AB-ABC-ABC")==false);
   assert(checkStrFormat("A-ABC-ABC-ABC-CBA")==true);

   // file2str
   assert(file2str("2moves.brd", str));
   assert(strcmp("A-ABC-ABC-ABC-CBA", str)==0);
   
   // str2state & solve
   strcpy(str, "A-AB-BA");
   s = str2state(str);
   assert(s);
   assert(solve(s, false)==2);
   free(s);

   // findFirstHyphenIndex
   strcpy(str, "A-AB-BA");
   assert(findFirstHyphenIndex(str)==1);

   //match
   strcpy(str, "A-ABC-ABC-ABC-ABC");
   s = str2state(str);
   assert(s);
   assert((match(s, s->boardArr[0])) == true);
   freeAllBoard(s);
   free(s);
}

/* Many of other functions, as required */

bool checkStrFormat(char*str){
   if(str == NULL){
      return false;
   }
   int len = (int)strlen(str);
   int strCount = 0;
   int firstHyphenIndex = getFirstHyphenIndex(str);
   int secondHyphenIndex = getSecondHyphenIndex(str, len, firstHyphenIndex);
   int rows = 0; 
   int cols = 1; 
   
   
   // check hawk 
   if(firstHyphenIndex != 1){
      return false;
   }
   //  check board format
   if(secondHyphenIndex != firstHyphenIndex){
      cols = secondHyphenIndex - firstHyphenIndex -1;
   }

   for(int i = 0; i < len; i++){
      if(str[i] == '-'){
         rows++;
      }
      if(isalpha(str[i])){
         strCount++;
      }
   }

   strCount--;
   int count = rows*cols;
   
   if(strCount == count){
      return true;
   }else{
      return false;
   }
}

int getFirstHyphenIndex(char* str){
   int firstHyphen = -1;
   int len = (int)strlen(str);
   for(int i = 0; i < len && (firstHyphen == -1); i++){
      if(str[i] == '-'){
         firstHyphen = i;
      }
   }
   return firstHyphen;
}

int getSecondHyphenIndex(char* str, int len ,int firstHyphenIndex){
   int secondHyphenIndex = 1;
   for(int i = firstHyphenIndex; i < len && secondHyphenIndex == 1; i++){
      if(str[i] == '-'){
         secondHyphenIndex = i;
      }
   }
   return secondHyphenIndex;
}


int findLastIndex(int index, char* str){
   // find the last index
   int lastIndex = -1; 
   for(int i = index-1; i > 0 && lastIndex < 0; i--){
      if(isalpha(str[i])){
         lastIndex = i;
      }
   }
   return lastIndex;
}

/* find the first hyphen */
int findFirstHyphenIndex(const char* str){
   int firstHyphen = -1;
   int len = (int)strlen(str);
   for(int i = 0; i < len && (firstHyphen == -1); i++){
      if(str[i] == '-'){
         firstHyphen = i;
      }
   }
   return firstHyphen;
}

/* get the board's row and column */
void getRowAndCol(const char* str, state*s, int len){
   int firstHyphenIndex = 1;
   int secondHyphenIndex = 1;
   // counts of hyphen represent counts of rows
   int rows = 0; 
   int cols = 1; 

   for(int i = firstHyphenIndex; i < len && secondHyphenIndex == 1; i++){
      if(str[i] == '-'){
         secondHyphenIndex = i;
      }
   }

   if(secondHyphenIndex != firstHyphenIndex){
      cols = secondHyphenIndex - firstHyphenIndex -1;
   }

   for(int i = 0; i < len; i++){
      if(str[i] == '-'){
         rows++;
      }
   }

   s->cols = cols;
   s->rows = rows;
}

/* create single board */
board* createNewBoard(state* s, board* parentBoard, int colIndex){
   board* newBoard = (board*)calloc(1, sizeof(board));

   // copy parent
   for(int row = 0; row < s->rows; row++){
      for(int col = 0; col < s->cols; col++){
         newBoard->arr[row][col] =  parentBoard->arr[row][col];
      }
   }
   newBoard->parent = parentBoard;
   char tempChar;

   for(int row = s->rows -1; row >= 0; row--){
      if(row == s->rows -1){
         tempChar = newBoard->arr[row][colIndex];
      }

      if(row > 0){
         newBoard->arr[row][colIndex] = newBoard->arr[row-1][colIndex];
      }

      if(row == 0){
         newBoard->arr[row][colIndex] = parentBoard->hawk;
         newBoard->hawk = tempChar;
      }
   }
   newBoard->index = s->numberOfBoard;
   newBoard->layer = s->numberOfLayer;
   newBoard->parentIndex = parentBoard->index;
   newBoard->isCreateNextBoard = false;
   newBoard->nextPeriodStartIndex = newBoard->index * s->cols + 1;

   return newBoard;
}

/* match all columns */
bool match(state*s, board* board){
   int rows = s->rows;
   int cols = s->cols;
   
   for(int col = 0; col < cols ; col++){
      char firstChar = board->arr[0][col];
      for(int row = 0; row < rows; row++){
         if(board->arr[row][col] != firstChar){
            return false;
         }
      }
   }
   return true;
}

/* expand the board array until find the solution */
state *expandBoard(state* s){
   // check if board's row is only 1;
   if(s->rows == 1){
      s->moves = 0;
      return s;
   }

   // first layer
   s->numberOfLayer++;
   int parentIndex = 0;
   while(s->numberOfLayer < MAXLAYER && !s->findSolution && s->numberOfBoard < MAXBRDS){
      for(int col = 0; col < s->cols && !s->findSolution; col++){
         int boardIndex = s->numberOfBoard;
         s->boardArr[boardIndex]= createNewBoard(s, s->boardArr[parentIndex], col);
         s->numberOfBoard++;
         bool result = match(s,s->boardArr[boardIndex]);
         // find the solution
         if(result){ 
            s->findSolution = true;
            s->solutionIndex = s->numberOfBoard - 1;
         }
      }

      // if current board is the last one of current layer, layer++
      if(checkLayerPlus(s->numberOfBoard-1, s->cols, s->numberOfLayer)){
         s->numberOfLayer++;
      }
      s->boardArr[parentIndex]->isCreateNextBoard = true;
      parentIndex++;
   }
    
   if(s->findSolution){
      int endIndex = s->numberOfBoard - 1;
      s->moves = s->boardArr[endIndex]->layer;
   }else{
      s->moves = -1;
   }
   
   return s;
}

/* if current board index is the last one of current layer, layer++ */
bool checkLayerPlus(int index, int cols, int layer){
   int count = 0;
   if(layer == 0 && index == 0){
      return true;
   }

   int num = 1;
   for(int i = 0; i < layer; i++){
      num = num * cols;
      count = count + num;
   }

   if(index == count){
      return true;
   }
   return false;
}

void printSolution(state*s){
   // find every parentIndex until 0;
   int solutionIndex = s->solutionIndex;
   int moveIndexArray[MAXLAYER];
   int count = 0;
   board * currentBoard = s->boardArr[solutionIndex];

   while(solutionIndex > 0){
      moveIndexArray[count] = solutionIndex;
      count++;
      currentBoard = currentBoard->parent;
      solutionIndex = currentBoard->index;
   }

   //if need to print the first board
   moveIndexArray[count] = 0;
   count++;

   for(int i = count-1; i >=0; i--){
      printSingleBoard(s,moveIndexArray[i]);
   }
}

/* free all board */
void freeAllBoard(state*s){
   for(int i = 0; i < s->numberOfBoard; i++){
      free(s->boardArr[i]);
   }
}

/* print all board */
void printSingleBoard(state *s, int boardIndex){
   //printf("%c \n",s->boardArr[boardIndex]->hawk);
   for(int row = 0; row < s->rows; row++){
      for(int col = 0; col < s->cols; col++){
         printf("%c", s->boardArr[boardIndex]->arr[row][col]);
      }
      printf("\n");
   }
   printf("\n");
}
