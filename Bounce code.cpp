﻿//BOUNCE- PuperHacker
//29/5/19

#include <iostream>
#include <set>
#include <windows.h>
#include <cassert>
#include <conio.h>
using namespace std;

#define width 100
#define height 20
#define ref_rate 200 //Refresh frame every 200ms

enum dir{UP_LEFT,UP_RIGHT,DOWN_LEFT,DOWN_RIGHT,STABLE}; //Direction of balls

unsigned int PlayerX = 10; //Y coordinate is not needed-player is always at the bottom
unsigned int PlayerSize = 15;
unsigned int Player_move_space = 8; //Player can move -+5 blocks per refresh

class ball {
public:

	inline bool operator<(const ball& b) const {
		if (coordX != b.coordX) return coordX < b.coordX;
		return coordY < b.coordY;
	}

	unsigned int getX() const{ return coordX; }
	unsigned int getY() const{ return coordY; }

	void SetCoords(int x, int y) {
		assert(x >= 0 && x < width && y >= 0 && y < height); //Assert position is in the map
		coordX = x;
		coordY = y;
	}

	dir GetDir()const { return direction; } //Returns direction of ball
	void setDir(dir new_dir) { direction = new_dir; } //Changes direction of ball

private:
	unsigned int coordX = 0, coordY=0;
	dir direction = DOWN_RIGHT; //Ball starts moving towards the lower right angle of the frame
};

void new_ball(set<ball>& vec,int coordx=0, int coordy=0) {
	ball b;
	if (coordx || coordy) b.SetCoords(coordx, coordy); 
	//Set new coordinates only if they have been provided and are not default (0,0)

	vec.insert(b);
}

void draw(set<ball> vec,unsigned int PlayerX,unsigned int PlayerSize) { //Draw the map
	auto it = vec.begin(); //Access first ball in the set
	bool left_ballz = vec.size(); //There are balls to be drawn(if size is not 0)

	for (unsigned int x = 0; x < width; x++) cout << "#";
	cout << "\n";

	for (unsigned int y = 1; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			if (!x || x == width-1) //If the first or last column is being drawn
				cout << "#";
			else if (left_ballz && x == it->getX() && y == it->getY()) { //If the coordinates match the current ball's coordinates
				cout << "O";
				if (++it == vec.end()) left_ballz = false; //Set next ball as available
			}
			else if (x==PlayerX&&y==height-1) {
				for (unsigned int p = 0; p < PlayerSize; p++) cout << "-"; //Draw player
				x += PlayerSize-1; //Move cursor after player
			}
			else cout << " ";
		}
		cout << "\n";
	}
}

void bounce(set<ball> &vec,ball a, bool &defeat){ //Logic function for ball movement
	switch(a.getX()){
	case 1: //Collides with left wall
			switch (a.GetDir()) {
			case DOWN_LEFT:
				a.setDir(DOWN_RIGHT);
				break;
			case UP_LEFT:
				a.setDir(UP_RIGHT);
				break;
			}
			break;
	case width-2: //Collides with right wall
		switch (a.GetDir()) {
		case DOWN_RIGHT:
			a.setDir(DOWN_LEFT);
			break;
		case UP_RIGHT:
			a.setDir(UP_LEFT);
			break;
		}
		break;
	}

	switch (a.getY()) {
		case 1: //Collides with upper wall
			switch (a.GetDir()) {
			case UP_RIGHT:
				a.setDir(DOWN_RIGHT);
				break;
			case UP_LEFT:
				a.setDir(DOWN_LEFT);
				break;
			}
			break;
		case height-2: //Collides with lower wall(player territory)
			if (PlayerX<=a.getX() && (PlayerX + PlayerSize>=a.getX())) //Player deflected ball
				switch (a.GetDir()) {
				case DOWN_RIGHT:
					a.setDir(UP_RIGHT);
					break;
				case DOWN_LEFT:
					a.setDir(UP_LEFT);
					break;
				}
			else defeat = true; //Ball has reached the lowest level and player has not deflected it
			break;
	}

	vec.insert(a); //Insert the ball back into the set after processing
}

void move_balls(set<ball> &vec,bool &defeat) {
	set<ball> sup_vec = vec;
	vec.clear();

	for (auto x : sup_vec) {
		ball a = x;
		switch (a.GetDir()) {
		case UP_LEFT: 
			a.SetCoords(a.getX()-1,a.getY()-1);
			break;
		case UP_RIGHT:
			a.SetCoords(a.getX() + 1, a.getY() - 1);
			break;
		case DOWN_LEFT:
			a.SetCoords(a.getX() - 1, a.getY() + 1);
			break;
		case DOWN_RIGHT:
			a.SetCoords(a.getX() + 1, a.getY() + 1);
			break;
		}
		bounce(vec,a,defeat); //Logic for moving balls
	}
}

void move_player(){
	if (_kbhit()) {
		switch (toupper(getchar())) { //Get single character input
		case 'A': //Move left
			PlayerX -= Player_move_space;
			if (PlayerX <= 0) PlayerX = 1; //Do not allow player to exceed the limits of the map
			break;
		case 'D': //Move right
			PlayerX += Player_move_space;
			if (PlayerX+PlayerSize >= width-1) PlayerX = width-PlayerSize-1;
			break;
		default:
			break;
		}
	}
}

int main(){
	system("color 0A"); //Set console color
	system("title Bounce v0.5 - Puperhacker");
	set<ball> ball_vec;
	new_ball(ball_vec,1,1); //Get 1 ball
	bool defeat = false;

	while (!defeat) {
		move_balls(ball_vec, defeat);
		if(!defeat) draw(ball_vec,PlayerX,PlayerSize);
		Sleep(ref_rate);
		move_player();
		system("cls"); //Erase frame
	}
	cout << "YOU LOST!\n";
	system("PAUSE");
	return 0;
}
