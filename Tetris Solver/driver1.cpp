#include "driver1.hpp"
#include "driver2.hpp"

using namespace std;
using namespace tetro;

int main(){

	Shapes tetromino_type;
	int tetrominos_number;
	int i = 0;
	char tetrotype;
	int width;
	int length;
	int tetroNumber;
	char charType;

	width = 12;
	length = 12;
	tetroNumber = 8;
	int anim=0;

	Tetromino toGo('S');

	cout << "The board object is being initialized." << endl<< endl;
	Tetris playground(width,length);
	cout << "The board is being resized with size of 12 width and 12 length." << endl<< endl;
	playground.resizeBoard(12, 12);
	cout << "The board is being created as a 2D array." << endl<< endl;
	playground.createBoard(12, 12);
	playground.Draw(12,12);
	cout << "TESTING...  Meaningful row function (Should return 2 for S tetromino) :";
	cout << toGo.meaningful_row_func(toGo) << endl;
	cout << "TESTING... Meaningful column function (Should return 2 for S tetromino) :";
	cout << toGo.meaningful_column_func(toGo) << endl;
	cout << "Tetromino being added to the board with += operator." << endl<< endl;
	playground+=toGo;
	playground.Draw(width,length);
	cout << "Tetromino being deleted from the board with -= operator." << endl<< endl;
	playground-=toGo;
	playground.Draw(width,length);
	cout << "Tetromino after being rotated to Right 1 time:(It is added again with the += operator.)" << endl<< endl;
	toGo.rotate_tetromino('R');
	playground+=toGo;
	playground.Draw(width,length);
	cout << "Tetromino being moved to the right 3 units:" << endl<< endl;
	for(i=0;i<3;i++){
		playground.SlideTetrominoLR(toGo,'R');
	}
	playground.Draw(width,length);
	cout << "Tetromino after being lowered to the minimum level possible:" << endl<< endl;
	cout << "Please press enter 1 to see the lowering animation:" << endl<< endl;
	cin >> anim;
	playground.LowerTetromino(toGo,'L',2);
	
	

	return 0;

}
