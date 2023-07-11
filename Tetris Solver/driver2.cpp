#include "driver1.hpp"
#include "driver2.hpp"

using namespace std;
using namespace tetro;

int main(){

	int i = 0;
	int width;
	int length;
	int tetroNumber;  // Number of tetrominos which'll be entered.
	char moveDirection;
	int moveCount;
	char rotateDirection;
	int rotateCount;
	char inpchar;

	cout << "Please enter the width of the board: ";
	cin >> width;


	// Checks for bad inputs, asks again if any.

	while( cin.fail() ){
		cin.clear();
		cin.ignore();
		cout << "Please enter a valid input." << endl;
		cin >> width;
	}

	cout << "Please enter the length of the board: ";
	cin >> length;

	while( cin.fail() ){
		cin.clear();
		cin.ignore();
		cout << "Please enter a valid input." << endl;
		cin >> length;
	}

	Tetris playground(width,length);
	playground.createBoard(width,length);
	
	playground.Draw(width,length);
	cout << "Please enter the number of tetrominos" << endl;
	cin >> tetroNumber;

	
	while( cin.fail() ){
		cin.clear();
		cin.ignore();
		cout << "Please enter a valid input." << endl;
		cin >> tetroNumber;
	}

	// Runs the program for the requested number of tetrominos.

	for(i=0;i<tetroNumber;i++){
		cout << "Enter tetromino type (O,I,T,J,L,S,Z) or R for Random Tetromino, or Enter q to terminate program." << endl;
		cin >> inpchar;
		// If the user enters R, the tetromino is chosen randomly.
		if(inpchar == 'R'){
			srand(time(0));
			int randInt = rand();
			if( randInt %7 == 0){
				inpchar = 'O';
			}else if(randInt %7 == 1){
				inpchar = 'I';
			}else if(randInt %7 == 2){
				inpchar = 'T';
			}else if(randInt %7 == 3){
				inpchar = 'J';
			}else if(randInt %7 == 4){
				inpchar = 'L';
			}else if(randInt %7 == 5){
				inpchar = 'S';
			}else if(randInt %7 == 6){
				inpchar = 'Z';
			}else{
				cout << "PLEASE REACH THE DEVELOPER, OR SHOT HIM IN THE HEAD (TWICE) IF YOU WANT TO BE COOPERATIVE." << endl;
			}
		}
		std::cout << "\x1B[2J\x1B[H"; 

		cin. clear();
		Tetromino toGo(inpchar);
		if(i==0){
			toGo.fixFirst(toGo);
		}

		playground.Animate(toGo,rotateDirection,rotateCount,moveDirection,moveCount);
	}
	
	return 0;

}
