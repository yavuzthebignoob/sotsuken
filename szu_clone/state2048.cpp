#include <iostream>
#include <vector>
#include <utility>
using namespace std;

class State2048 {
public:
	static const int SIZE = 4;
	static const RectSize BOARD_SIZE = new RectSize(SIZE);
	static const int NUM_INITIAL_LOCATION = 2;
	static double RANDOM_FOUR_PROB = 0.1;

	static int REWARDS[] = { 0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

private:
	const int board[][];

public:
	static int getNumValues();
	int getPowerGrid();
	double getFeatures();
	vector<pair <double, State2048> > getPossibleNextStates();

private:
	int getValue(int flatlocation);
	void setValue(int flatlocation, int value);

private:
	State2048() {
		board = new int[SIZE][SIZE];
	}

public:	
	State2048(State2048 state) {
		board = new int[SIZE][SIZE];
		for (int row = 0; row < SIZE; row++) {
			board[row] = state.board[row].clone;
		}
	}
	State2048(double[] features) {
		board = new int[SIZE][SIZE];
		int index = 0;
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				board[row][col] = (int)features[index++];
			}
		}
	}
	

};

static int State2048::getNumValues() {
	return REWARDS.length;
}

// pending inmplementing 'getPowerGrid'
int State2048::getPowerGrid() {
	int grid = int[SIZE][SIZE];
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			grid[row][col] = REWARDS[board[row][col]];
		}
	}
	return grid;
}

double State2048::getFeatures() {
	int index = 0;
	double[SIZE*SIZE] features;
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			features[index++] = board[row][col];
		}
	}
	return features;
}

int State2048::getValue(int flatlocation) {
	return board[flatlocation / SIZE][flatlocation % SIZE];
}

void State2048::setValue(int flatlocation, int value) {
	board[flatlocation / SIZE][flatlocation % SIZE] = value;
}

vector<pair <double, State2048> > getPossibleNextStates() {
	vector<int> emptyLocations;
	for (int location = 0; location < SIZE * SIZE; location++) {
		if (getValue(location) == 0) {
			emptyLocations.push_back(location);
		}
	}

	double prob2 = (1.0 - RANDOM_FOUR_PROB) / emptyLocations.size();
	double prob4 = RANDOM_FOUR_PROB / emptyLocations.size();

	vector<pair <double, State2048> > states;
	for (int loc = 0; loc < emptyLocations.size(); loc++) {
		State2048 state2();
		state2.setValue(loc, 1);
		pair<double, State2048> stateBuf = make_pair(prob2, state2);
		states.push_back(stateBuf);

		State2048 state4 = new State2048(getFeatures());
		state4.setValue(location, 2);
		states.add(new Pair<>(prob4, state4));
	}

	return states;
}