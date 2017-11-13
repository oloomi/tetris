#include "tetris.h"

const unsigned char 
		red  [3] = {255,  0,  0}, 
		green[3] = {  0,255,  0},
		nili [3] = {128,128,255},
		white[3] = {255,255,255},
		black[3] = {  0,  0,  0};

#define MAXX 450
#define MAXY 600
#define MAX_SCORE_DIGITS 4
#define FONT_SIZE 13
#define DELAY_TIME 300
#define LEVEL1 50
#define LEVEL2 100
#define LEVEL3 150
#define LEVEL4 200


/**
 * This function is not defined in ANSI-C and hence re-implemented here
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

/************ Functions declarations **************/

void loadHighScores( int highScores[10])
{
	int num = 0;
	char strHighScore[ MAX_SCORE_DIGITS + 1];

	FILE *inFile;
	inFile = fopen("highScore.txt", "r");
	
	if( inFile == NULL)
	{
		inFile = fopen("highScore.txt", "w");
		for( num = 0; num < 10; num++)
			fprintf( inFile, "%d ", 0);
	}
	else
	{
		while( fscanf( inFile, "%s", strHighScore) != -1)
		{
			highScores[ num] = atoi( strHighScore);
			num++;
		}
	}
	fclose( inFile);
}

void showHighScores( CImg<unsigned char>* image, CImgDisplay *disp, int highScores[10])
{
	
	image->draw_text( "  Tetris!  ",100, 20, red ,black, 5*FONT_SIZE, 1);
	image->draw_text( "     High Scores     ",90, 100, black ,white, 2*FONT_SIZE + 5, 1);
	image->display( *disp);

	int num = 0;
	char strHighScore[MAX_SCORE_DIGITS + 1];

	for( num = 1; num <= 10; num++)
	{
		itoa( num , strHighScore, 10 );
		image->draw_text( strHighScore, 100, 150 + num*30, white, black, FONT_SIZE + 5);

		itoa( highScores[ num - 1], strHighScore, 10 );
		image->draw_text( strHighScore, 150, 150 + num*30, white, black, FONT_SIZE + 5);
		image->display( *disp);
		
	}
	#ifdef _WIN32
	Sleep(2000);
	#else
	usleep(2000 * 1000);
	#endif
}

void showGameBoard( CImg<unsigned char>* image, CImgDisplay *disp, int highScores[10])
{
	image->draw_rectangle( MAXX, MAXY, 0, 0, black);
	image->draw_rectangle( MAXX - 150 + 5, MAXY, MAXX - 150 + 1 ,0, green);

	image->draw_text( "Score",375 - 20, 200, white, black, FONT_SIZE);
	image->draw_text( "Level",375 - 20, 270, white, black, FONT_SIZE);
	image->draw_text( "High Score",375 - 20, 340, white, black, FONT_SIZE);
	
	char strHighScore[ MAX_SCORE_DIGITS + 1];
	itoa( highScores[0], strHighScore, 10);
	image->draw_text( strHighScore,375 - 10, 370, white, black, FONT_SIZE);
	image->display( *disp);
}

void gameOver( CImg<unsigned char>* image, CImgDisplay *disp, int highScores[10], int *score, int map[20][10])
{
	//	Checking Game Over
		int GOFlag = 0;
		int cc, num, n;

		for( cc = 0; cc < 10; cc++)
			if( map[0][cc] == 1)
			{
				GOFlag = 1;
				break;
			}
		
		if( GOFlag == 1)
		{
			image->draw_text(" Game Over!", 10, 250, red, black, 40,1);
			image->display( *disp);

			FILE * inFile;
			inFile = fopen("highScore.txt", "w");
			
			if ( *(score) > highScores[9])
			{
				
				for( num=0 ; num < 10; num++)
				{
					if( *(score) > highScores[num])
					{
						break;
					}
				}
				
				for( n = 9; n > num; n--)
				{
					highScores[n] = highScores[n - 1];
				}
				
				highScores[n] = *score;
			}

			for( num=0; num < 10; num++)
				fprintf( inFile, "%d ", highScores[num]);

			fclose( inFile);
			#ifdef _WIN32
			Sleep(3000);
			#else
			usleep(3000 * 1000);
			#endif
			exit(0);
		}
}

void random( int *randNum, int *nextRandNum)
{
	*randNum = *nextRandNum;
	srand(time(NULL));
	*nextRandNum  = rand();	
	*nextRandNum = (*nextRandNum) % 3;
//	*nextRandNum = (*nextRandNum) % 2;
}

void omitRow( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *score)
{
	int omited = 0, row, i, rc, cc, full = 0;

	for( row = 0; row <= 19; row++)
	{
		full = 0;
		for( i=0; i<10; i++)
			if( map[row][i] == 1)
			{
				full++;
			}
	
		if( full == 10)  // if one row is completed
		{
			omited++;
			for( rc=row; rc >0; rc--)
				for( cc=0; cc<10; cc++){
					map[rc][cc] = map[rc-1][cc]; // copies all matrix down
				}
		
			image->draw_rectangle( MAXX - 150, MAXY, 0, 0, black);
			image->display( *disp);
			
			for( rc=0; rc<= 19; rc++)	// draws all matrix
				for( cc=0; cc<10; cc++)
					if( map[rc][cc] == 1)
					{
						image->draw_rectangle( (cc + 1)*30, (rc + 1)*30, cc * 30, rc * 30, nili);
						image->display( *disp);
					}
		}
	}
	*score += ( omited * ( omited  * 5));
}

void changeLevel( int *score, int *level, int *delayTime)
{
	int level1Flag = 0, level2Flag = 0, level3Flag = 0, level4Flag = 0;

	if( *score >= LEVEL1 && *score <= LEVEL2 && level2Flag == 0)
	{
		*level = 2;
		*delayTime = DELAY_TIME / 2;
		level2Flag = 1;
	}
	else if( *score >= LEVEL2 && *score <= LEVEL3 && level3Flag == 0)
	{
		*level = 3;
		*delayTime = DELAY_TIME / 3;
		level3Flag = 1;
	}
	else if( *score >= LEVEL3 && *score <= LEVEL4 && level4Flag == 0)
	{
		*level = 4;
		*delayTime = DELAY_TIME / 6;
		level4Flag = 1;
	}
}

void showNextShape( CImg<unsigned char>* image, CImgDisplay *disp, int *nextRandNum, int *flag)
{
	if( *flag == 1)
	{
		image->draw_rectangle(375 + 60, 120 + 20 , 375 - 60, 10, black);
		image->display( *disp);
			
		switch( *nextRandNum)
		{
		
			case 0:
				image->draw_rectangle( 375 + 30 , 20 + 60, 375 - 30, 20, nili);
				image->display( *disp);
				break;
		
			case 1:
				image->draw_rectangle( 375 , 120 + 20, 375 - 30, 20, nili);
				image->display( *disp);
				break;
			
			case 2:
				image->draw_rectangle( 375 + 30, 30 + 20, 375 - 30, 20, nili);
				image->draw_rectangle( 375 + 60, 60 + 20, 375, 30 + 20, nili);
				image->display( *disp);
				break;
		}
	}

	*flag = 1;
}

void squareShape( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *delayTime)
{
	int angle = 0, preAngle = 0;

	int x = ( MAXX - 150)/ 2, preX, y = 0, preY;
	
	int r = 0, preR, c = 4, preC;

	int down = 2, j, pause = 0;
	
	image->draw_rectangle( x + 30 , 60, x - 30, y, nili);
	image->display( *disp);
	map[r][c] = 1;
	map[r][c + 1] = 1;
	map[r + 1][c] = 1;
	map[r + 1][c + 1] = 1;

	while(y <= 510)
	{		
		down = 2;

		preX = x;
		preY = y;

		preR = r + down - 2;
		preC = c;

		for(j=0; j<10; j++)
		{
					
			switch (disp->key)
			{
				
				case cimg::keyARROWLEFT:  
					if(pause)
						 break;
					if( ( x - 30) >= 30 && map[r][c-1] == 0 && map[r+1][c-1] == 0){
						x-=30;
						c--;
					}
					disp->key=0; 
					break;
					
				case cimg::keyARROWRIGHT: 
					if(pause)
						 break;
					if( ( x + 30) <= 270 && map[r][c+2] == 0 && map[r+1][c+2] == 0){
						x+=30;  
						c++;
					}
					disp->key=0;
					break;
					
				case cimg::keyARROWDOWN:  
					if(pause)
						 break;
					if( ( y + 30) <= 510  && map[r+2][c] == 0 && map[r+2][c+1] == 0){
						y+=30;
						r++;
						down = 2;
					}
					disp->key=0; 
					break;
						
				case cimg::keyP:
					if(pause)
						pause = 0;
					else
						pause = 1;
					disp->key=0; 
					break;
			} // switch
				
			cimg::wait ( (*delayTime)/10);
		}// for	
			
		if( map[r + down][c] == 0 && map[r + down][c+1] == 0){
			image->draw_rectangle( preX + 30 , preY + 60, preX - 30, preY , black);
			image->display( *disp);
			map[preR][preC] = 0;
			map[preR][preC + 1] = 0;
			map[preR + 1][preC] = 0;
			map[preR + 1][preC + 1] = 0;
					
			if(!pause){
				y+=30;
				r++;
			}
			image->draw_rectangle( x + 30 , y + 60, x - 30, y , nili);
			image->display( *disp);
			map[r + down - 2][c] = 1;
			map[r + down - 2][c + 1] = 1;
			map[r + down - 1][c] = 1;
			map[r + down - 1][c + 1] = 1;
		}
		else
			break;
	} // while
}

void rectangleShape( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *delayTime)
{
	int angle = 0, preAngle = 0;

	int x = ( MAXX - 150)/ 2, preX, y = 0, preY;
	
	int r = 0, preR, c = 4, preC;

	int down = 2, j, pause = 0;
	
	image->draw_rectangle( x , 120, x - 30, y, nili);
	image->display( *disp);
	map[r][c] = 1;
	map[r + 1][c] = 1;
	map[r + 2][c] = 1;
	map[r + 3][c] = 1;

	while( ( (angle == 0) && (y <= 450)) || ( (angle == 1) && (y <= 540)))
	{
				
		preAngle = angle;
			
		preX = x;
		preY = y;

		down = 2;

		preR = r + down - 2;
		preC = c;

		for(j=0; j<10; j++)
		{
		
			switch (disp->key)
			{
									
				case cimg::keyARROWLEFT:
					if(pause)
						 break;
					if( ( angle == 0 && ( x - 30) >= 30  
						&& map[r][c - 1] == 0 && map[r + 1][c - 1] == 0 && map[r + 2][c - 1] == 0 && map[r + 3][c - 1] == 0) || 
						( angle == 1 && (x-30) >= 60 && map[r][c - 2] == 0) )
					{
						
						x-=30;
						c--;
					}
					disp->key=0; 
					break;
					
				case cimg::keyARROWRIGHT: 
					if(pause)
						 break;
					if( ( angle == 0 && ( x + 30) <= 300 
						&& map[r][c + 1] == 0 && map[r + 1][c + 1] == 0 && map[r + 2][c + 1] == 0 && map[r + 3][c + 1] == 0) || 
						( angle == 1 && (x+30) <= 240 && map[r][c + 3] == 0)) 
					{
								
						x+=30;  
						c++;
					}
					disp->key=0; 
					break;
					
				case cimg::keyARROWDOWN:  
					if(pause)
						 break;
					if( ( angle == 0 && (y + 30) <= 450 && map[r + 4][c] == 0) || 
						( angle == 1 && (y + 30) <= 540 && map[r+1][c-1]==0 && map[r+1][c]==0 && map[r+1][c+1]==0 && map[r+1][c+2]==0) )
					{
						y+=30;  
						r++;
					}
					disp->key=0;
					break;
					
				case cimg::keySPACE:
					if(pause)
						 break;
					if( (x >= 60) && (x <= 240) && ( y <= 480)){
						angle += 3;
						angle = angle % 2;
					}
					
					disp->key=0;
					break;
					
				case cimg::keyP:
					if(pause)
						pause = 0;
					else
						pause = 1;
					disp->key=0; 
					break;		
			}
			cimg::wait ( *delayTime/10);
		}
			
		if( ( preAngle == 0 && map[r + 2 + down][c] == 0) || 
			( preAngle == 1 && map[r - 1 + down][c - 1] == 0 &&  map[r - 1 + down][c] == 0 &&
							   map[r - 1 + down][c + 1] == 0 &&  map[r - 1 + down][c + 2] == 0))
		{	
					
			if( preAngle == 0){
				image->draw_rectangle( preX , preY + 120, preX - 30, preY , black);
				image->display( *disp);
				map[preR][preC] = 0;
				map[preR + 1][preC] = 0;
				map[preR + 2][preC] = 0;
				map[preR + 3][preC] = 0;
			}
			else if( preAngle == 1){
				image->draw_rectangle( preX + 60, preY + 30, preX - 60, preY , black);
				image->display( *disp);
				map[preR][preC - 1] = 0;
				map[preR][preC] = 0;
				map[preR][preC + 1] = 0;
				map[preR][preC + 2] = 0;
			}
				
			if(!pause){
				y+=30;
				r++;
			}

			if( angle == 0){
				image->draw_rectangle( x , y + 120, x - 30, y , nili);
				image->display( *disp);
					
				map[r + down - 2][c] = 1;
				map[r + down - 1][c] = 1;
				map[r + down ][c] = 1;
				map[r + down + 1][c] = 1;
			}
			else if( angle == 1){
				image->draw_rectangle( x + 60, y + 30, x - 60, y , nili);
				image->display( *disp);
				
				map[r + down - 2][c - 1] = 1;
				map[r + down - 2][c] = 1;
				map[r + down - 2][c + 1] = 1;
				map[r + down - 2][c + 2] = 1;
			}
		}
		else
			break;
	}
			
}
void zShape( CImg<unsigned char>* image, CImgDisplay *disp, int map[20][10], int *delayTime)
{
	int angle = 0, preAngle = 0;

	int x = ( MAXX - 150)/ 2, preX, y = 0, preY;
	
	int r = 0, preR, c = 4, preC;

	int down = 2, j, pause = 0;

	image->draw_rectangle( x + 30, 30, x - 30, y, nili);
	image->draw_rectangle( x + 60, 60, x, y + 30, nili);
	image->display( *disp);
	map[r][c] = 1;
	map[r][c + 1] = 1;
	map[r + 1][c + 1] = 1;
	map[r + 1][c + 2] = 1;
	
	while( y <= 510)
	{
		
		preX = x;
		preY = y;

		down = 2;
			
		preR = r + down - 2;
		preC = c;

		for(j=0; j<10; j++)
		{
		
			switch (disp->key)
			{
					
				case cimg::keyARROWLEFT: 
					if(pause)
						 break;
					if( ( x - 30) >= 30 && map[r+1][c] == 0 && map[r][c-1] == 0) {
						x-=30;  
						c--;
					}
					disp->key=0; 
					break;
					
				case cimg::keyARROWRIGHT:
					if(pause)
						 break;
					if( ( x + 30) <= 240 && map[r][c+2] == 0 && map[r+1][c+3] == 0) {
						x+=30;  
						c++;
					}
					disp->key=0; 
					break;
					
				case cimg::keyARROWDOWN: 
					if(pause)
						 break;
					if( ( y + 30) <= 510 && map[r+1][c] == 0 && map[r+2][c+1] == 0 && map[r+2][c+2] == 0) {
						y+=30;
						r++;
					}
					disp->key=0; 
					break;
					
				case cimg::keyP:
					if(pause)
						pause = 0;
					else
						pause = 1;
							
					disp->key=0; 
					break;
			}
			cimg::wait ( *delayTime/10);
		}	
		if( map[r + down - 1][preC] == 0 && map[r + down][preC+1] == 0 && map[r + down][preC+2] == 0 ){
				
			image->draw_rectangle( preX + 30, preY + 30, preX - 30, preY, black);
			image->draw_rectangle( preX + 60, preY + 60, preX, preY + 30, black);
			image->display( *disp);
				
			map[preR][preC] = 0;
			map[preR][preC + 1] = 0;
			map[preR + 1][preC + 1] = 0;
			map[preR + 1][preC + 2] = 0;
	
			if(!pause){
				y+=30;
				r++;
			}
			image->draw_rectangle( x + 30, y + 30, x - 30, y, nili);
			image->draw_rectangle( x + 60, y + 60, x, y + 30, nili);
			image->display( *disp);
				
			map[r + down - 2][c] = 1;
			map[r + down - 2][c + 1] = 1;
			map[r + down - 1][c + 1] = 1;
			map[r + down - 1][c + 2] = 1;
		}
		else
			break;
	}
		
}

void showScore( CImg<unsigned char>* image, CImgDisplay *disp, int *score)
{
	char strScore[MAX_SCORE_DIGITS + 1];
	itoa( *score, strScore,10);
	image->draw_text( strScore, 375 - 20, 230, white, black, FONT_SIZE);
	image->display( *disp);
}

void showLevel( CImg<unsigned char>* image, CImgDisplay *disp, int *level)
{
	char strLevel[2];
	
	strLevel[0] = *level + 48;
	strLevel[1] = NULL;

	image->draw_text( strLevel, 375 - 5, 300, white, black, FONT_SIZE);
	image->display( *disp);
}
