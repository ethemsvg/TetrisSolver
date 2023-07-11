#include <iostream>
#include <unistd.h>
#include "driver1.hpp"

#ifndef _DRIVER2_HPP_
#define _DRIVER2_HPP_

using namespace std;

namespace tetro{

class Coordinate{

public:
	void setRow(int r){
		row = r;
	}
	void setColumn(int c){
		column = c;
	}
	int getRow(){
		return row;
	}
	int getColumn(){
		return column;
	}
	
private:
	int row;
	int column;
};



class Tetris{


	public:
		
		Tetris(const int& w, const int& l); // Constructor which initializes the tetris board.
		void setWidth(int w){
			width = w;
		}
		void setLength(int l){
			length = l;
		}
		int** Meaningful2Dgenerator(Tetromino piece); // Creates a Meaningful 2D vector out of 4x4 vectors.
		void Animate(Tetromino &piece, char rotateDirection, int rotateCount, char moveDirection, int moveCount); // Animates the moves.
		Coordinate TetroPlacer(Tetromino piece, char moveDirection, int moveCount); // Decides where to place the tetromino.
		void SlideTetrominoLR(Tetromino piece, char direction); // Moves the tetromino horizontally by one unit to the left or right.
		void LowerTetromino(Tetromino piece, char moveDirection, int moveCount); // Lowers the tetromino step by step to the bottom.
		Tetris& operator-=(Tetromino piece);
		void resizeBoard(const int& width, const int& length); // Resizes the board.
		void createBoard(const int& width, const int& length); // Creates the board.
		void Draw(const int& width, const int& length); // Draws the board.
		Tetris& operator+=(Tetromino piece);
		void isGameOver();
	private:
		char** board;
		int width;
		int length;
		int isBotFull = 0;
		int tetrominoCounter = 0;
};


}

#endif
