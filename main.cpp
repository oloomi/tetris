/*------------------------------------------------------------------------------
  This program is an implementation of the famous Tetris game.
  The keys can be used are: ARROWRIGHT, ARROWLEFT, ARROWDOWN for moving shapes, 
  SPACE for rotation and P for pausing the game.
  
  Developed By: Seyed Mohammad Hossein Oloomi	Feb. 2007
  -----------------------------------------------------------------------------*/

#include "tetris.h"

int main()
{
	int map[20][10] = {0};
	int highScores[10] = {0};
	int score = 0, level = 1, delayTime = DELAY_TIME, firstFlag = 0;
	int randNum, nextRandNum;

	// Opens a MAXX * MAXY size window
	CImg<unsigned char> image(MAXX,MAXY,1,3, 0);
	CImgDisplay disp(image, " .: Tetris game :.");
	
	// Reads high scores from file
	loadHighScores( highScores);

	// Shows high scores
	showHighScores( &image, &disp, highScores);

	// Draws game board
	showGameBoard( &image, &disp, highScores);
	
	while(1)
	{
		
		// Game Over!
		gameOver( &image, &disp, highScores, &score, map);	
		
		// Makes random codes for shapes
		random( &randNum, &nextRandNum);
	
		// Omitting filled rows & Shifting remained rows down
		omitRow( &image, &disp, map, &score);
		
		// Changing Level
		changeLevel( &score, &level, &delayTime);

		// Showing Score
		showScore( &image, &disp, &score);

		// Showing level
		showLevel( &image, &disp, &level);

		// Showing next shape
		showNextShape( &image, &disp, &nextRandNum, &firstFlag);
		
		if( randNum == 0)
		{
			// Showing, moving and rotating the shape: SQUARE
			squareShape( &image, &disp, map, &delayTime);
		}

		else if( randNum == 1)	
		{
			// Showing, moving and rotating the shape: RECTANGLE
			rectangleShape( &image, &disp, map, &delayTime);		
		}
		 
		else if( randNum == 2) 
		{
			// Showing and moving the shape: Z 
			zShape( &image, &disp, map, &delayTime);
		} 

	}

	return 0;
}
