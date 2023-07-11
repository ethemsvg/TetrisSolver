#include "driver1.hpp"
#include "driver2.hpp"

using namespace std;
using namespace tetro;

/*
Notes About Some Concepts used in the code.

	Meaningful Column: On a 4x4 array, the meaningful column is the biggest column value which contain at least one full units.

	Meaningful Row: On a 4x4 array, the meaningful row is the biggest row value which contain at least one full units.

	Start Column: The middle column of the board. Is calculated as (width/2) - 1. Is the same value all through the code.

	Meaningful 2D Vector: Speaking for a 4x4 array containing a tetromino, some rows and columns might be totally empty.
	a Meaningful 2D Vector is actually a 4x4 array containing a tetromino with the empty rows and columns stripped away.

	std::cout << "\x1B[2J\x1B[H": Clears the terminal screen for better display.

*/


/*

Tetromino &piece: The piece which'll be removed from the board.


Removes tetromino from the board. This function is needed for re-drawing the board after the rotation to avoid overlaps.

*/
Tetris& Tetris::operator-=(Tetromino piece){

	int MeaningfulColumn = piece.meaningful_column_func(piece);
	int MeaningfulRow = piece.meaningful_row_func(piece);
	
	int numOfRows = 4 - MeaningfulRow;  // Number of rows of the tetromino piece.
	int numOfColumns = MeaningfulColumn + 1; // Number of columns of the tetromino piece.

	int startColm = (width/2) - 1; 
	int i,j,k,l;

	int** meaningful2D;

	meaningful2D = Meaningful2Dgenerator(piece); // Meaningful 2D array is created.

	// Loops through the first few rows (according to the vertical size of the tetromino),
	// To find any full units, if found, clears them.

	for(i=0;i<(numOfRows);i++){
		for(j=0;j<(numOfColumns);j++){
			if(meaningful2D[i][j] == 1){
				board[i+1][j+startColm] = '.';
			}else if(meaningful2D[i][j] == 0){
				board[i+1][j+startColm] = '.'; 
			}
		}
	}

	return *this;

}
/*

*/
void Tetris::Animate(Tetromino &piece, char rotateDirection, int rotateCount, char moveDirection, int moveCount){

	int i=0;
	int j=0;
	int startColm = (width/2) - 1;

	*this+=piece;
	Draw(width,length);

	cout << "Please enter rotation direction" << endl;
	cin >> rotateDirection;

	cout << "Please enter rotation count" << endl;
	cin >> rotateCount;

	cout << "Please enter move direction" << endl;
	cin >> moveDirection;

	cout << "Please enter move count" << endl;
	cin >> moveCount;

	for(i=0;i<rotateCount;i++){
		*this -= piece;
		piece.rotate_tetromino(rotateDirection);
		*this += piece;
		std::cout << "\x1B[2J\x1B[H";
		Draw(width,length);
		usleep(500000);

	}

	while(startColm + moveCount + piece.meaningful_column_func(piece) >= width-1){
		cout << "Move amount is out of boundaries of the board. Please enter a new value:" << endl;
		cin >> moveCount;
	}

	// The suitable destination coordinate for the tetromino is received.
	// and the board is drawn.
	Coordinate coordinate = TetroPlacer(piece,moveDirection,moveCount);
	

	int destinationRow = coordinate.getRow();
	int destinationColumn= coordinate.getColumn();
	cout << "The decided column IN Animate is:" << coordinate.getColumn() << endl;

	int slideAmount;
	char slideDirection;

	// If the destination column is on the left side of the start column, the slide direction is Left,
	// and the slide amount is the difference between them.

	if( startColm > destinationColumn ){

		slideAmount = startColm - destinationColumn;
		slideDirection = 'L';

		for(i=0;i<slideAmount;i++){
			
			// The tetromino is slided the necessary units in the necessary direction.

			SlideTetrominoLR(piece,slideDirection);
			std::cout << "\x1B[2J\x1B[H";
			Draw(width,length);
			usleep(500000);
		
		}

	// If the destination column is on the right side of the start column, the slide direction is Right,
	// and the slide amount is the difference between them.

	}else if( startColm < destinationColumn ){

		slideAmount = destinationColumn - startColm;
		slideDirection = 'R';

		for(i=0;i<slideAmount;i++){

			// The tetromino is slided the necessary units in the necessary direction.

			SlideTetrominoLR(piece,slideDirection);
			std::cout << "\x1B[2J\x1B[H";
			Draw(width,length);
			usleep(500000);
		}

	}else{

		cout << "Slide amount is 0" << endl;

	}


	// Then the tetromino is lowered as an animation row by row, as many rows as possible which is calculated on the LowerTetromino function.

	LowerTetromino(piece,moveDirection,moveCount);

	isGameOver();

}

/*

Tetromino piece: The tetromino which'll be fitted.
int shiftRow: A control value for whether the value of the lowest row possible to search for a fit value will be shifted upwards (only if necessary.)

Scans the board from top down in forms of the size of the tetromino, looking for fit positions where the existence of the tetromino would not interfere
with any other full units.

*/

Coordinate Tetris::TetroPlacer(Tetromino piece, char moveDirection, int moveCount){

	Coordinate coordinate;
	
	int i=0;
	int j=0;
	
	int lowestRow = length-1; // Lowest row on the tetris board which can hold tetrominos.
	int startColm = (width/2) - 1; 
	int breakcheck = 0;
	int priorityLowestRow; // The lowest row with any empty spaces to search for fit places on the board.
	int priorityStartingRow; // The starting row of the tetromino when being written on the board (the tetromino is started to being written on the left upper unit.)
	int numOfRows = 4 - piece.meaningful_row_func(piece);
	int numOfColumns = piece.meaningful_column_func(piece) + 1;
	int temp;
	int** meaningful2D;
	meaningful2D = Meaningful2Dgenerator(piece);

	

	// The lowest row possible for fitting is detected.
	
	for(i=length-1;i>1;i--){	
		for(j=1;j<width;j++){
			if(board[i][j] == '.' && board[i-1][j] == '.'){
				priorityLowestRow = i;
				breakcheck = 1;
				break;
			}
		}
		if(breakcheck == 1){
			break;
		}
	}

	

	priorityStartingRow = priorityLowestRow - (numOfRows-1);


	coordinate.setRow(priorityLowestRow);

	if(moveDirection == 'R'){
		cout << "YOU ENTERED RIGHT" << endl;

		temp = startColm + moveCount;
		coordinate.setColumn(temp);
		cout << "COLUMNS IS " << coordinate.getColumn() << endl;

	}else if(moveDirection == 'L'){
		cout << "YOU ENTERED LEFT" << endl;
		temp = startColm - moveCount;
		coordinate.setColumn(temp);
		cout << "COLUMNS IS " << coordinate.getColumn() << endl;
	}

	cout << "The decided column IN Fit is:" << coordinate.getColumn() << endl;

	return coordinate;

}
	
/*

Tetromino piece: The tetromino which'll be added to the board (with any rotation position).

Adds the parameter tetromino to the top middle of the board.

*/

Tetris& Tetris::operator+=(Tetromino piece){

	int MeaningfulColumn = piece.meaningful_column_func(piece);
	int MeaningfulRow = piece.meaningful_row_func(piece);
	
	int numOfRows = 4 - MeaningfulRow; 
	int numOfColumns = MeaningfulColumn + 1;


	int startColm = (width/2) - 1;
	int i,j,k,l;

	int** meaningful2D;

	meaningful2D = Meaningful2Dgenerator(piece);


	for(i=0;i<(numOfRows);i++){
		
		// Overwrites the meaningful 2D array of the Tetromino to the board 
		// on the correct position.

		for(j=0;j<(numOfColumns);j++){
			if(meaningful2D[i][j] == 1){
				board[i+1][j+startColm] = piece.typechar;
			}else if(meaningful2D[i][j] == 0){
				board[i+1][j+startColm] = '.'; 
			}
		}
	}

	return *this;

}

/*

Tetromino piece: The tetromino which'll be moved.

Lowers the recently added tetromino to the minimum row level possible.

*/

void Tetris::LowerTetromino(Tetromino piece, char moveDirection, int moveCount){ 

	int MeaningfulColumn = piece.meaningful_column_func(piece);
	int MeaningfulRow = piece.meaningful_row_func(piece);
	int shiftRow = 0;
	int numOfRows = 4 - MeaningfulRow; 
	int numOfColumns = MeaningfulColumn + 1;

	cout << "Num of columns is: " << numOfColumns << endl;
	cout << "Num of rows is : " << numOfRows << endl;

	int i=0;
	int j=0;

	// The coordinate values of the fit place is received.

	Coordinate coordinate = TetroPlacer(piece,moveDirection,moveCount);	
	

	// The bottom limit of the lowering process is received from the Fit function.

	int botLimitRow = coordinate.getRow(); 
	int PieceStartColm=0;
	int k=0;
	int breakcheck=0;

	int** meaningful2D = Meaningful2Dgenerator(piece);

	// The starting point of the Tetromino's current location is detected.
	// There is a special case for a certain rotation position of the J tetromino.

	for(i=0;i<length;i++){
	
		for(j=0;j<width;j++){
			if(board[1][j] == piece.typechar || board[2][j] == piece.typechar ){
				PieceStartColm = j;
				if( (piece.typechar == 'J' ) && meaningful2D[0][0] != 1){
					PieceStartColm--;
				}
				breakcheck = 1;
				break;
			}
	
		}
		if(breakcheck == 1){
			break;
		}
	}

	breakcheck = 0;

	// The tetromino is shifted downwards starting from the lowest row to one row below, so there is no overlapping.

		for(i=1;i<(botLimitRow-numOfRows+1);i++){ // 
			if(breakcheck == 1){
				break;
			}
			for(j=PieceStartColm;j<numOfColumns+PieceStartColm;j++){
				if(board[i+numOfRows][j] != '.' ){
						breakcheck = 1;
						break;
				}
				for(k=numOfRows;k>0;k--){
					if(breakcheck == 1){
						break;
					}
					if( (board[i+k-1][j] == piece.typechar) && breakcheck != 1){

						board[i+k][j] = board[i+k-1][j]; 
						board[i+k-1][j] = '.';
					}
				}
					board[i][j] = '.'; 
			}
			std::cout << "\x1B[2J\x1B[H";
			Draw(width,length);
			usleep(50000);
			if(breakcheck == 1){break;}

		}


}

/*

Tetromino piece: The piece which'll be slided.

char direction: The direction which the tetromino will be slided. (Left or Right).

The tetromino is slided to left or right by one units. 

*/

void Tetris::SlideTetrominoLR(Tetromino piece, char direction){

	int MeaningfulColumn = piece.meaningful_column_func(piece);
	int MeaningfulRow = piece.meaningful_row_func(piece);
	
	int numOfRows = 4 - MeaningfulRow; 
	int numOfColumns = MeaningfulColumn + 1;

	int startColm;
	int i=0;
	int j=0;
	int breakcheck = 0;

	int** meaningful2D = Meaningful2Dgenerator(piece);

	// The starting point of the Tetromino's current location is detected.
	// There is a special case for a certain rotation position of the J tetromino.

	for(i=0;i<width;i++){
		if(board[1][i] == piece.typechar || board[2][i] == piece.typechar){
			startColm = i;
			if((piece.typechar == 'J') && meaningful2D[0][0] != 1){
				startColm--;
			}

			break;
		}

	}


	// The tetromino is shifted to the right column by column. The shifting starts with the rightmost column of the tetromino.

	if(direction == 'R'){

			for(i=1;i<numOfRows+1;i++){
					
					//if(breakcheck == 1){
						//break;
					//}
					for(j=0;j<numOfColumns+1;j++){

						//if(board[i][(startColm + numOfColumns) - j] != '.'){
							//breakcheck = 1;
							//break;
						//}
		
						if(j == numOfColumns){
							board[i][(startColm + numOfColumns) - j] = '.';
						}
						board[i][(startColm + numOfColumns) - j] = board[i][(startColm + numOfColumns) - j -1];
					}
		
			}
		
	}

	// The tetromino is shifted to the left column by column. The shifting starts with the leftmost column of the tetromino.

	if(direction == 'L'){

		for(i=1;i<numOfRows+1;i++){

			for(j=0;j<numOfColumns+1;j++){
										

				board[i][ (startColm - 1)  + j ] = board[i][ startColm +j  ];
			}
		}
	}

}

/*

Tetromino piece: The tetromino who's 4x4 array will be turned into another 2D array with no competely empty rows or columns.

This function creates 2x2,2x3,3x2,4x1,1x4 etc. arrays from 4x4 arrays by stripping away the completely empty rows and columns.

*/

int** Tetris::Meaningful2Dgenerator(Tetromino piece){

	int i=0;
	int j=0;

	// The 2D array is created with no size.


	// The upmost meaningful row and rightmost meaningful column is detected.

	int MeaningfulRow = piece.meaningful_row_func(piece);
	int MeaningfulColumn = piece.meaningful_column_func(piece);

	int numOfRows = 4 - MeaningfulRow; 
	int numOfColumns = MeaningfulColumn + 1;

	// The empty array is resized in accordance to the number of rows and columns of the tetromino.

	// Resize of the rows.

	int **meaningful2D = new int*[numOfRows];

	// Resize of the columns.

	for(i=0;i<numOfRows;i++){
		meaningful2D[i] = new int[numOfColumns];
	}

	// Fills the 2D Meaningful array from the 4x4 default array.
	for(i=piece.meaningful_row_func(piece);i<4;i++){ 

		for(j=0;j<=piece.meaningful_column_func(piece);j++){ 

			meaningful2D[i-piece.meaningful_row_func(piece)][j] = piece.tetromino_matrix[i][j];
		}

	}


	// The final Meaningful 2D array of the tetromino is returned.

	return meaningful2D;
}

/*

const int& width: The width of the board which is taken from the user as an input.
const int& length: The length of the board which is taken from the user as an input.

This function basically resizes the 2D board array in accordance to the width and length values.

*/
void Tetris::resizeBoard(const int& width, const int& length){

	int i=0;

	board = new char*[length];

	for(i=0;i<length;i++){
		board[i] = new char[width];
	}

}

/*

const int& w: The width of the board. 
const int& l: The length of the board.

The default constructor of a Tetris object.

Sets the width and length values taken from the user into the object,
and then resizes the board in accordance to that values.

*/

Tetris::Tetris(const int& w, const int& l){

	setWidth(w);
	setLength(l);
	resizeBoard(width,length);
	cout <<"Board has succesfully initalized." << endl;

}

/*

const int& width: The width of the board. 
const int& length: The length of the board.

Displays the board on the terminal screen by basically looping through every unit on the board array.

*/
void Tetris::Draw(const int& width, const int& length){

	int i=0;
	int j=0;

	for(i=0;i<length;i++){

		for(j=0;j<width;j++){

			cout << board[i][j];
		}

		cout << endl;

	}

	return;

}

/*

const int& width: The width of the board. 
const int& length: The length of the board.

Creates an empty board with borders and empty units.

*/

void Tetris::createBoard(const int& width, const int& length){

	int i=0;
	int j=0;


	for(i=0;i<length;i++){ 

		for(j=0;j<width;j++){

			if(i==0 || i==length-1){

				board[i][j] = '#';

			}else if(j==0 || j==width-1){

				board[i][j] = '#';

			}else{

				board[i][j] = '.';
			}

		}

	} 

	return;

}

/*

Tetromino tetromino: The related tetromino.

Detects the meaningful column (which is a concept explained at the top of the file as a block comment.)

*/

int Tetromino::meaningful_column_func(Tetromino tetromino){

	int meaningful_column = 3;


	if(tetromino.tetromino_matrix[0][3] == 0 && tetromino.tetromino_matrix[1][3] == 0 && tetromino.tetromino_matrix[2][3] == 0 && tetromino.tetromino_matrix[3][3] == 0  ){

		meaningful_column = 2;

	}

	if(tetromino.tetromino_matrix[0][2] == 0 && tetromino.tetromino_matrix[1][2] == 0 && tetromino.tetromino_matrix[2][2] == 0 && tetromino.tetromino_matrix[3][2] == 0  ){

		meaningful_column = 1;

	}



	return meaningful_column;

}

/*

Tetromino tetromino: The related tetromino.

Detects the meaningful row (which is a concept explained at the top of the file as a block comment.)

*/

int Tetromino::meaningful_row_func(Tetromino tetromino){

	int meaningful_row = 0;

	if(tetromino.tetromino_matrix[0][0] == 0 && tetromino.tetromino_matrix[0][1] == 0 && tetromino.tetromino_matrix[0][2] == 0 && tetromino.tetromino_matrix[0][3] == 0  ){

		meaningful_row = 1;

	}

	if(tetromino.tetromino_matrix[1][0] == 0 && tetromino.tetromino_matrix[1][1] == 0 && tetromino.tetromino_matrix[1][2] == 0 && tetromino.tetromino_matrix[1][3] == 0  ){

		meaningful_row = 2;

	}


	
	return meaningful_row;
	

}

/*

const char& RorL: The direction of the rotation (Right or Left).

This function simply Transposes the 2D array and then mirrors it to the y axis for left rotation .
												and mirrors it to the x axis for right rotation	.	

Also it moves the 2D array to the most left and bottom side of the matrix.

If there is a empty row on bottom of the Tetromino, (or two, or three rows), the rows are shifted to the bottom of the matrix;
Then if there are empty columns on the left of the Tetromino (or two, or three columns), the columns are shifted to the left side of the matrix.

*/

void Tetromino::rotate_tetromino(const char& RorL){ 

	int hold;
	int i=0;
	int j=0;

	
	int **temp = new int*[3];
	for(i=0;i<3;i++){
		temp[i] = new int[3];
	}

	int **rotateNinety = new int*[3];
	for(i=0;i<3;i++){
		rotateNinety[i] = new int[3];
	}

	

	if(1==2){

		for(i=0;i<3;i++){

			for(j=0;j<3;j++){

				temp[i][j] = tetromino_matrix[j][i];

			}
		}

		if( RorL == 'R'){

			for(i=0;i<3;i++){
				// MIRROR IMAGE
				rotateNinety[i][0] = temp[i][2];
				rotateNinety[i][1] = temp[i][1];
				rotateNinety[i][2] = temp[i][0];
			}

		}
		else if(RorL == 'L'){

			for(i=0;i<3;i++){

				rotateNinety[0][i] = temp[2][i];
				rotateNinety[1][i] = temp[1][i];
				rotateNinety[2][i] = temp[0][i];
			}
		}

		if(rotateNinety[0][0] == 0 && rotateNinety[1][0] == 0 && rotateNinety[2][0] == 0){  // slides the tetromino to the left side of the matrix
																							// if there is an empty column.	
			rotateNinety[0][0] = rotateNinety[0][1];
			rotateNinety[1][0] = rotateNinety[1][1];
			rotateNinety[2][0] = rotateNinety[2][1];

			rotateNinety[0][1] = rotateNinety[0][2];
			rotateNinety[1][1] = rotateNinety[1][2];
			rotateNinety[2][1] = rotateNinety[2][2];

			rotateNinety[0][2] = 0;
			rotateNinety[1][2] = 0;
			rotateNinety[2][2] = 0;

		}

		if(rotateNinety[2][0] == 0 && rotateNinety[2][1] == 0 && rotateNinety[2][2] == 0){  // slides the tetromino to the bottom of the matrix
																							// if there is an empty row.
			rotateNinety[2][0] = rotateNinety[1][0];
			rotateNinety[2][1] = rotateNinety[1][1];
			rotateNinety[2][2] = rotateNinety[1][2];

			rotateNinety[1][0] = rotateNinety[0][0];
			rotateNinety[1][1] = rotateNinety[0][1];
			rotateNinety[1][2] = rotateNinety[0][2];

			rotateNinety[0][0] = 0;
			rotateNinety[0][1] = 0;
			rotateNinety[0][2] = 0;

		}
		
		tetromino_matrix = rotateNinety;

	}
	else if( tetromino_type == Shapes::S || tetromino_type == Shapes::Z || tetromino_type == Shapes::L || tetromino_type == Shapes::J || tetromino_type == Shapes::T || tetromino_type == Shapes::I ){ ///////////////////////////////////////////////////////////////////////////////////////////////////////
		//resize the matrix

		int **temp = new int*[4];
		for(i=0;i<4;i++){
			temp[i] = new int[4];
		}

		int **rotateNinety = new int*[4];
		for(i=0;i<4;i++){
			rotateNinety[i] = new int[4];
		}		


		for(i=0;i<4;i++){

			for(j=0;j<4;j++){

				temp[i][j] = tetromino_matrix[j][i];

			}
		}

		if(RorL == 'R'){

			for(i=0;i<4;i++){

				rotateNinety[i][0] = temp[i][3];
				rotateNinety[i][1] = temp[i][2];
				rotateNinety[i][2] = temp[i][1];
				rotateNinety[i][3] = temp[i][0];
			}
		}
		else if(RorL == 'L'){

			for(i=0;i<4;i++){

				rotateNinety[3][i] = temp[0][i];  
				rotateNinety[2][i] = temp[1][i];
				rotateNinety[1][i] = temp[2][i];
				rotateNinety[0][i] = temp[3][i];
			}

		}
			// SLIDE TETROMINOS TO THE LEFT AS MUCH AS POSSIBLE
		while((rotateNinety[0][0] == 0 && rotateNinety[1][0] == 0 && rotateNinety[2][0] == 0 && rotateNinety[3][0] == 0)){

		if(  (rotateNinety[0][1] == 0 && rotateNinety[1][1] == 0 && rotateNinety[2][1] == 0 && rotateNinety[3][1] == 0) && 
			 (rotateNinety[0][0] == 0 && rotateNinety[1][0] == 0 && rotateNinety[2][0] == 0 && rotateNinety[3][0] == 0)  ){  // slides the tetromino to the left side of the matrix
																																// if there is an empty column.	
			rotateNinety[0][0] = rotateNinety[0][2];
			rotateNinety[1][0] = rotateNinety[1][2];
			rotateNinety[2][0] = rotateNinety[2][2];
			rotateNinety[3][0] = rotateNinety[3][2];

			rotateNinety[0][1] = rotateNinety[0][3];
			rotateNinety[1][1] = rotateNinety[1][3];
			rotateNinety[2][1] = rotateNinety[2][3];
			rotateNinety[3][1] = rotateNinety[3][3];

			rotateNinety[0][2] = 0;
			rotateNinety[1][2] = 0;
			rotateNinety[2][2] = 0;
			rotateNinety[3][2] = 0;

			rotateNinety[0][3] = 0;
			rotateNinety[1][3] = 0;
			rotateNinety[2][3] = 0;
			rotateNinety[3][3] = 0;

		}
		else if(rotateNinety[0][0] == 0 && rotateNinety[1][0] == 0 && rotateNinety[2][0] == 0 && rotateNinety[3][0] == 0){

			rotateNinety[0][0] = rotateNinety[0][1];
			rotateNinety[1][0] = rotateNinety[1][1];
			rotateNinety[2][0] = rotateNinety[2][1];
			rotateNinety[3][0] = rotateNinety[3][1];

			rotateNinety[0][1] = rotateNinety[0][2];
			rotateNinety[1][1] = rotateNinety[1][2];
			rotateNinety[2][1] = rotateNinety[2][2];
			rotateNinety[3][1] = rotateNinety[3][2];

			rotateNinety[0][2] = rotateNinety[0][3];
			rotateNinety[1][2] = rotateNinety[1][3];
			rotateNinety[2][2] = rotateNinety[2][3];
			rotateNinety[3][2] = rotateNinety[3][3];

			rotateNinety[0][3] = 0; 
			rotateNinety[1][3] = 0;
			rotateNinety[2][3] = 0;
			rotateNinety[3][3] = 0;

		}

	}

		// SLIDE TETROMINOS TO THE BOTTOM AS MUCH AS POSSIBLE

	while((rotateNinety[3][0] == 0 && rotateNinety[3][1] == 0 && rotateNinety[3][2] == 0 && rotateNinety[3][3] == 0)){

		if(  (rotateNinety[2][0] == 0 && rotateNinety[2][1] == 0 && rotateNinety[2][2] == 0 && rotateNinety[2][3] == 0)  &&
			 (rotateNinety[3][0] == 0 && rotateNinety[3][1] == 0 && rotateNinety[3][2] == 0 && rotateNinety[3][3] == 0)	  ){  

			rotateNinety[3][0] = rotateNinety[1][0];
			rotateNinety[3][1] = rotateNinety[1][1];
			rotateNinety[3][2] = rotateNinety[1][2];
			rotateNinety[3][3] = rotateNinety[1][3];

			rotateNinety[2][0] = rotateNinety[0][0];
			rotateNinety[2][1] = rotateNinety[0][1];
			rotateNinety[2][2] = rotateNinety[0][2];
			rotateNinety[2][3] = rotateNinety[0][3];

			rotateNinety[1][0] = 0;
			rotateNinety[1][1] = 0;
			rotateNinety[1][2] = 0;
			rotateNinety[1][3] = 0;

			rotateNinety[0][0] = 0;
			rotateNinety[0][1] = 0;
			rotateNinety[0][2] = 0;
			rotateNinety[0][3] = 0;

		}
		else if(  (rotateNinety[3][0] == 0 && rotateNinety[3][1] == 0 && rotateNinety[3][2] == 0 && rotateNinety[3][3] == 0)  ){

			rotateNinety[3][0] = rotateNinety[2][0];
			rotateNinety[3][1] = rotateNinety[2][1];
			rotateNinety[3][2] = rotateNinety[2][2];
			rotateNinety[3][3] = rotateNinety[2][3];

			rotateNinety[2][0] = rotateNinety[1][0];
			rotateNinety[2][1] = rotateNinety[1][1];
			rotateNinety[2][2] = rotateNinety[1][2];
			rotateNinety[2][3] = rotateNinety[1][3];

			rotateNinety[1][0] = rotateNinety[0][0];
			rotateNinety[1][1] = rotateNinety[0][1];
			rotateNinety[1][2] = rotateNinety[0][2];
			rotateNinety[1][3] = rotateNinety[0][3];

			rotateNinety[0][0] = 0; 
			rotateNinety[0][1] = 0;
			rotateNinety[0][2] = 0;
			rotateNinety[0][3] = 0;

		}

	}

		tetromino_matrix = rotateNinety;

	}

	
}


/*

Shapes type: An enum value to determine the type of the Tetromino.

This function basically initalizes the Tetrominos' as a form of a matrix.

*/

void Tetromino::tetromino_array_filler(Shapes type){ 

	int i=0;
	int j=0;

	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			tetromino_matrix[i][j] = 0;
		}
	}

	switch(type){

		case Shapes::O:

		tetromino_matrix[2][0] = 1;
		tetromino_matrix[2][1] = 1;
		tetromino_matrix[3][0] = 1;
		tetromino_matrix[3][1] = 1;

		//tetromino_matrix = { {0,0,0,0} , {0,0,0,0} , {1,1,0,0} , {1,1,0,0} };
		break;

		case Shapes::I:

		tetromino_matrix[3][0] = 1;
		tetromino_matrix[3][1] = 1;
		tetromino_matrix[3][2] = 1;
		tetromino_matrix[3][3] = 1;

		//tetromino_matrix = { {0,0,0,0} , {0,0,0,0} , {0,0,0,0} , {1,1,1,1} };
		break;

		case Shapes::T:

		tetromino_matrix[2][0] = 1;
		tetromino_matrix[2][1] = 1;
		tetromino_matrix[2][2] = 1;
		tetromino_matrix[3][1] = 1;

		//tetromino_matrix = { {0,0,0,0} , {0,0,0,0} , {1,1,1,0} , {0,1,0,0} };
		break;

		case Shapes::J:

		tetromino_matrix[1][1] = 1;
		tetromino_matrix[2][1] = 1;
		tetromino_matrix[3][0] = 1;
		tetromino_matrix[3][1] = 1;

		//tetromino_matrix = { {0,0,0,0} , {0,1,0,0} , {0,1,0,0} , {1,1,0,0} };
		break;

		case Shapes::L:

		tetromino_matrix[1][0] = 1;
		tetromino_matrix[2][0] = 1;
		tetromino_matrix[3][0] = 1;
		tetromino_matrix[3][1] = 1;

		//tetromino_matrix = { {0,0,0,0} , {1,0,0,0} , {1,0,0,0} , {1,1,0,0} };
		break;

		case Shapes::S:

		tetromino_matrix[2][1] = 1;
		tetromino_matrix[2][2] = 1;
		tetromino_matrix[3][0] = 1;
		tetromino_matrix[3][1] = 1;

		//tetromino_matrix = { {0,0,0,0} , {0,0,0,0} , {0,1,1,0} , {1,1,0,0} };
		break;

		case Shapes::Z:

		tetromino_matrix[2][0] = 1;
		tetromino_matrix[2][1] = 1;
		tetromino_matrix[3][1] = 1;
		tetromino_matrix[3][2] = 1;

		//tetromino_matrix = { {0,0,0,0} , {0,0,0,0} , {1,1,0,0} , {0,1,1,0} };
		break;

	}

}

/*

Default tetromino constructor with no parameters.

*/
Tetromino::Tetromino(){

	tetromino_matrix_sizer();

}
/*

The Constructor of the Tetromino Class.

Basically assigns a char value to the typechar for further usages in class functions/methods.

*/

Tetromino::Tetromino(char inpchar){
	tetromino_matrix_sizer();
	typechar = inpchar;
	if(typechar == 'O'){
		tetromino_type = Shapes::O;
	}else if(typechar == 'I'){
		tetromino_type = Shapes::I;
	}else if(typechar == 'T'){
		tetromino_type = Shapes::T;
	}else if(typechar == 'J'){
		tetromino_type = Shapes::J;
	}else if(typechar == 'L'){
		tetromino_type = Shapes::L;
	}else if(typechar == 'S'){
		tetromino_type = Shapes::S;
	}else if(typechar == 'Z'){
		tetromino_type = Shapes::Z;
	}else if(typechar == 'q' || typechar == 'Q'){
		cout << "Terminating program.." << endl;
		exit(1);
	}else{
		cout << "Incorrect input. Terminating..." << endl;
		exit(1);
	}
	cout << endl;


	tetromino_array_filler(tetromino_type);

}

/*

Tetromino& veryFirstEntry: The first tetromino type which is entered by the user.

Since the first tetromino does not have a tetromino before it to arrange it's position,
for tetrominos like T and Z there is a blank formed on the bottom left of the plain.

So this function rotates the Tetrominos to the left as there is no blank unit on the bottom left
on the screen.

*/

void Tetromino::fixFirst(Tetromino& veryFirstEntry){

	if( veryFirstEntry.typechar == 'I'){
		veryFirstEntry.rotate_tetromino('L');
	}else if( veryFirstEntry.typechar == 'T'  ){
		veryFirstEntry.rotate_tetromino('L');
		veryFirstEntry.rotate_tetromino('L');
	}else if( veryFirstEntry.typechar == 'Z' ){
		veryFirstEntry.rotate_tetromino('L');
	}


}

/*

Arranges the size of the 2D array where the tetromino is held in.

*/
void Tetromino::tetromino_matrix_sizer(){
	tetromino_matrix[0] = new int[4];
	tetromino_matrix[1] = new int[4];
	tetromino_matrix[2] = new int[4];
	tetromino_matrix[3] = new int[4];
}

/*

Checks whether the game is over or not.

*/
void Tetris::isGameOver(){

	int i=0;

	for(i=1;i<width-1;i++){
		if(board[1][i] != '.'){
			cout << "The Game Is Over. Thanks For Playing..." << endl;
			exit(1);
		}
	}

}
