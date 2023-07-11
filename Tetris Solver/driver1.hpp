#include <iostream>
#include <unistd.h>

#ifndef _DRIVER1_HPP_
#define _DRIVER1_HPP_

using namespace std;

namespace tetro{

enum class Shapes{O,I,T,J,L,S,Z};

class Tetromino
{

public:
	Tetromino();
	Tetromino(char inpchar); // Constructor (This constructor uses strongly-typed enums inside of it to determine the type of the Tetromino)
	friend class Tetris;

	Shapes tetromino_type;
	void rotate_tetromino(const char& direction); // Function which rotates the tetrominos 90 degrees to the left or right.

	int meaningful_column_func(Tetromino tetromino); // Detects meaningufl column.
	int meaningful_row_func(Tetromino tetromino); // Detects meaningful row.
	
	void fixFirst(Tetromino& veryFirstEntry); // Function that fixes the position of the first tetromino entered in a way that there is no blank units between the tetromino and de left border of the grid.
	void tetromino_array_filler(Shapes type); // The function which fills the 4x4 empty matrixes as the shape of the Tetromino.
	void tetromino_matrix_sizer();
	

private:
	char typechar;
	int** tetromino_matrix = new int*[4];
	

	//vector<vector<int> > tetromino_vector;	

};

}

#endif
