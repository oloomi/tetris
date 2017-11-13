#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "CImg.h"
using namespace cimg_library;

#define MAXX 450
#define MAXY 600
#define MAX_SCORE_DIGITS 4
#define FONT_SIZE 13
#define DELAY_TIME 300
#define LEVEL1 50
#define LEVEL2 100
#define LEVEL3 150
#define LEVEL4 200

void loadHighScores( int highScores[10]);
void showHighScores( CImg<unsigned char>* image, CImgDisplay *disp, int highScores[10]);
void showGameBoard( CImg<unsigned char>* image, CImgDisplay *disp, int highScores[10]);
void gameOver( CImg<unsigned char>* image, CImgDisplay *disp, int highScores[10], int *score, int map[20][10]);
void random( int *randNum, int *nextRandNum);
void omitRow( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *score);
void changeLevel( int *score, int *level, int *delayTime);
void showScore( CImg<unsigned char>* image, CImgDisplay *disp, int *score);
void showLevel( CImg<unsigned char>* image, CImgDisplay *disp, int *level);
void showNextShape( CImg<unsigned char>* image, CImgDisplay *disp, int* nextRandNum, int *flag);
void squareShape( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *delayTime);
void rectangleShape( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *delayTime);
void zShape( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *delayTime);
