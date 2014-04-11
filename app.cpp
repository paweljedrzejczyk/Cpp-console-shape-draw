/*
	Program for drawing a shape using specified ASCII character
	Author: Pawel Jedrzejczyk <pawel.jedrzejczyk@pjwstk.edu.pl>
	Date: 19.01.2014
*/

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

/*
	Main class for the Application
*/
class Application {
private:
	int
		windowWidth = 0, // console window width
		windowHeight = 0, // console window height
		shapeSize = 0, // current shape size
		currentX = 0, // where to start the drawing process - x axis
		currentY = 0, // minimal shape size to be able to draw it
		pressedKey;

	const int
		minimalShapeSize = 5; // minimal shape size to be able to draw it

	char
		drawingChar; // user specified drawing char

	/*
		list of keyboard buttons and corresponding codes
	*/
	enum {
		ESC			= 27,
		ARROW_UP	= 256 + 72,
		ARROW_DOWN	= 256 + 80,
		ARROW_LEFT	= 256 + 75,
		ARROW_RIGHT = 256 + 77,
		KEY_PLUS	= 43,
		KEY_MINUS	= 45
	}; 

	/*
		ignore everything until new line character is present 
	*/
	void _flushCin() {
		while (cin.get() != '\n');
	}

	/*
		function for moving cursor around the window
	*/
	void _gotoXY(int x, int y){
		COORD c;
		c.X = x;
		c.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}

	/*
		function for getting the x coordinate of the cursor
	*/
	int _whereX() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		return csbi.dwCursorPosition.X;
	}

	/*
		function for getting the y coordinate of the cursor
	*/
	int _whereY() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		return csbi.dwCursorPosition.Y;
	}

	/*
		setting the current console window size at the startup
	*/
	void getCurrentWindowSize() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		windowWidth = info.srWindow.Right;
		windowHeight = info.srWindow.Bottom;
	}

	/*
		displaying nice headline at the top of the screen
	*/
	void _displayHeadline() {
		system("cls"); // cleaning the console window
		_gotoXY(0, 0);
		cout << endl << endl << endl;
		cout << "\tProgram for drawing a shape using specified ASCII character" << endl;
		cout << "\tAuthor: Pawel Jedrzejczyk <pawel.jedrzejczyk@pjwstk.edu.pl>" << endl;
		cout << "\tDate: 19.01.2014" << endl << endl << endl << endl;
	}

	/*
		getting the particular ascii character from the user
	*/
	void getPreferredAsciiChar() {
		_displayHeadline();
		cout << "\tPlease specify ASCII character for drawing: ";
		drawingChar = _getche();
	}

	/*
		getting the preferred shape size from the user
	*/
	void getStartupShapeDimensions() {
		_displayHeadline();
		for (;;) { // loop until correct size is provided
			cout << "\tEnter shape size: ";
			cin >> shapeSize;
			if (!cin.good() || shapeSize < minimalShapeSize) {
				_displayHeadline();
				cout << "\tEntered value must be within range " << minimalShapeSize << "-" << windowHeight + 1 << endl << endl;
				cin.clear();
				_flushCin();
			} else {
				_flushCin();
				break;
			}
		}
	}

	/*
		drawing the shape

		x     x
		xx   xx
		x x x x
		x  x  x
		x     x
		x     x

	*/
	void _drawShape(int *startX, int *startY) {
		_gotoXY(*startX, *startY+shapeSize);
		for (int i = 0; i <= shapeSize - 2; i++) {
			cout << drawingChar;
			_gotoXY(_whereX() - 1, _whereY() - 1);
		}
		for (int x = 1; x <= (shapeSize - 3) / 2 + 2; x++) {
			cout << drawingChar;
			_gotoXY(_whereX(), _whereY() + 1);
		}
		_gotoXY(_whereX(), _whereY() - 2);
		cout << drawingChar;
		for (int x = 1; x <= (shapeSize - 3) / 2 - 1; x++) {
			_gotoXY(_whereX(), _whereY() - 1);
			cout << drawingChar;
		}
		_gotoXY(_whereX(), _whereY() - 1);
		for (int i = 0; i <= shapeSize - 1; i++) {
			cout << drawingChar;
			_gotoXY(_whereX() - 1, _whereY() + 1);
		}
		_gotoXY(0, 0);
	}


	/*
		function for checking if the shape will still fit in the window
	*/
	bool _fitsInWindow(int newX, int newY, int newSize) {
		if (newX < 0 || newY < -1 ||
			newX + newSize > windowWidth ||
			newY + newSize > windowHeight ||
			newSize < minimalShapeSize) return false;
		return true;
		
	}

	/*
		function for moving the shape
	*/
	void _updatePosition(int newX, int newY) {
		if (_fitsInWindow(newX, newY, shapeSize)) {
			currentX = newX;
			currentY = newY;
		}
	}

	/*
		function for resizing the shape
		if new size is allowed also update the position of the shape
	*/
	void _resizeShape(int newSize) {
		int newY = currentY + (shapeSize - newSize);
		if (_fitsInWindow(currentX, newY, newSize)) {
			shapeSize = newSize;
			currentY = newY;
		}
	}

	/*
		main loop of the application
	*/
	void drawObject() {

		/*
			placing the cursor at the very bottom of the window
			and moving it up so the shape will fit
		*/
		currentX = 0;
		currentY = windowHeight - shapeSize;

		do {
			system("cls");
			_drawShape(&currentX, &currentY); // calling the function for drawing and passing the current X and Y 
			
			/*
				getting the pressed key from the user
				if special key (eg. up arrow) was pressed the run the function again to get the proper code
				see http://support.microsoft.com/kb/57888
			*/
			pressedKey = _getch(); 
			if (pressedKey == 0 || pressedKey == 0xE0) {
				pressedKey = 256 + _getch();
			}

			switch (pressedKey) {
			case ARROW_RIGHT:
				_updatePosition(currentX + 1, currentY);
			break;
			case ARROW_LEFT:
				_updatePosition(currentX - 1, currentY);
			break;
			case ARROW_UP:
				_updatePosition(currentX, currentY - 1);
			break;
			case ARROW_DOWN:
				_updatePosition(currentX, currentY + 1);
			break;
			case KEY_PLUS:
				_resizeShape(shapeSize + 1);
			break;
			case KEY_MINUS:
				_resizeShape(shapeSize - 1);
			break;
			}
		} while (pressedKey != ESC); // exit the loop if ESC key is pressed
		
	}

public:
	void init() {
		getCurrentWindowSize();
		getPreferredAsciiChar();
		getStartupShapeDimensions();
		drawObject();
	}
};

int main() {
	Application app;
	app.init(); // initializing our Application class 

	return 0;
}