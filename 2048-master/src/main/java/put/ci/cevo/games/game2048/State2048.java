package put.ci.cevo.games.game2048;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.math3.random.RandomDataGenerator;
import put.ci.cevo.games.board.RectSize;
import put.ci.cevo.rl.environment.State;
import put.ci.cevo.util.Pair;
import put.ci.cevo.util.RandomUtils;

public class State2048 implements State {

	public static final int SIZE = 4;
	public static final RectSize BOARD_SIZE = new RectSize(SIZE);
	public static final int NUM_INITIAL_LOCATIONS = 2;
	public static final double RANDOM_FOUR_PROB = 0.1;

	public static int REWARDS[] = { 0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

	private final int board[][];

	public static int getNumValues() {
		return REWARDS.length;
	}

	private State2048() {
		board = new int[SIZE][SIZE];
	}

	public State2048(State2048 state) {
		board = new int[SIZE][SIZE];
		for (int row = 0; row < SIZE; row++) {
			board[row] = state.board[row].clone();
		}
	}

	public State2048(double[] features) {
		board = new int[SIZE][SIZE];
		int index = 0;
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				board[row][col] = (int) features[index++];
			}
		}
	}

	public int[][] getPowerGrid() {
		int[][] grid = new int[SIZE][SIZE];
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				grid[row][col] = REWARDS[board[row][col]];
			}
		}
		return grid;
	}

	@Override
	public double[] getFeatures() {
		int index = 0;
		double[] features = new double[SIZE * SIZE];
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				features[index++] = board[row][col];
			}
		}
		return features;
	}

	private int getValue(int flatLocation) {
		return board[flatLocation / SIZE][flatLocation % SIZE];
	}

	private void setValue(int flatLocation, int value) {
		board[flatLocation / SIZE][flatLocation % SIZE] = value;
	}

	public List<Pair<Double, State2048>> getPossibleNextStates() {
		List<Integer> emptyLocations = new ArrayList<>();
		for (int location = 0; location < SIZE * SIZE; location++) {
			if (getValue(location) == 0) {
				emptyLocations.add(location);
			}
		}

		double prob2 = (1.0 - RANDOM_FOUR_PROB) / emptyLocations.size();
		double prob4 = RANDOM_FOUR_PROB / emptyLocations.size();

		List<Pair<Double, State2048>> states = new ArrayList<>();
		for (int location : emptyLocations) {

			State2048 state2 = new State2048(getFeatures());
			state2.setValue(location, 1);
			states.add(new Pair<>(prob2, state2));

			State2048 state4 = new State2048(getFeatures());
			state4.setValue(location, 2);
			states.add(new Pair<>(prob4, state4));
		}

		return states;
	}

	public void addRandomTile(RandomDataGenerator random) {
		List<Integer> emptyLocations = new ArrayList<>();
		for (int location = 0; location < SIZE * SIZE; location++) {
			if (getValue(location) == 0) {
				emptyLocations.add(location);
			}
		}

		Integer randomEmptyLocation = RandomUtils.pickRandom(emptyLocations, random);
		boolean isFour = (random.nextUniform(0, 1, true) < RANDOM_FOUR_PROB);
		if (isFour) {
			setValue(randomEmptyLocation, 2);
		} else {
			setValue(randomEmptyLocation, 1);
		}
	}

	public int moveUp() {
		int reward = 0;

		for (int col = 0; col < SIZE; col++) {
			int firstFreeRow = 0;
			boolean alreadyAggregated = false;
			for (int row = 0; row < SIZE; row++) {
				if (board[row][col] == 0) {
					continue;
				}
				if (firstFreeRow > 0 && !alreadyAggregated && board[firstFreeRow - 1][col] == board[row][col]) {
					board[firstFreeRow - 1][col]++;
					board[row][col] = 0;
					reward += REWARDS[board[firstFreeRow - 1][col]];
					alreadyAggregated = true;
				} else {
					int temp = board[row][col];
					board[row][col] = 0;
					board[firstFreeRow++][col] = temp;
					alreadyAggregated = false;
				}
			}
		}

		return reward;
	}

	public void rotateBoard() {
		for (int i = 0; i < SIZE / 2; i++) {
			for (int j = i; j < SIZE - i - 1; j++) {
				int tmp = board[i][j];
				board[i][j] = board[j][SIZE - i - 1];
				board[j][SIZE - i - 1] = board[SIZE - i - 1][SIZE - j - 1];
				board[SIZE - i - 1][SIZE - j - 1] = board[SIZE - j - 1][i];
				board[SIZE - j - 1][i] = tmp;
			}
		}
	}

	public int makeMove(Action2048 action) {
		// TODO: Performance of this method could be significantly improved
		int reward = 0;

		if (action == Action2048.UP) {
			reward = moveUp();
		}

		if (action == Action2048.DOWN) {
			rotateBoard();
			rotateBoard();
			reward = moveUp();
			rotateBoard();
			rotateBoard();
		}

		if (action == Action2048.RIGHT) {
			rotateBoard();
			reward = moveUp();
			rotateBoard();
			rotateBoard();
			rotateBoard();
		}

		if (action == Action2048.LEFT) {
			rotateBoard();
			rotateBoard();
			rotateBoard();
			reward = moveUp();
			rotateBoard();
		}

		return reward;
	}

	public ArrayList<Action2048> getPossibleMoves() {
		// Set<Action2048> moves = new HashSet<>();
		boolean[] set = new boolean[4];
		ArrayList<Action2048> moves = new ArrayList<>(4);

		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				if (board[row][col] > 0) {
					continue;
				}

				if (!set[Action2048.RIGHT.id()]) {
					for (int col2 = 0; col2 < col; col2++) {
						if (board[row][col2] > 0) {
							set[Action2048.RIGHT.id()] = true;
							moves.add(Action2048.RIGHT);
							break;
						}
					}
				}

				if (!set[Action2048.LEFT.id()]) {
					for (int col2 = col + 1; col2 < SIZE; col2++) {
						if (board[row][col2] > 0) {
							set[Action2048.LEFT.id()] = true;
							moves.add(Action2048.LEFT);
							break;
						}
					}
				}

				if (!set[Action2048.DOWN.id()]) {
					for (int row2 = 0; row2 < row; row2++) {
						if (board[row2][col] > 0) {
							set[Action2048.DOWN.id()] = true;
							moves.add(Action2048.DOWN);
							break;
						}
					}
				}

				if (!set[Action2048.UP.id()]) {
					for (int row2 = row + 1; row2 < SIZE; row2++) {
						if (board[row2][col] > 0) {
							set[Action2048.UP.id()] = true;
							moves.add(Action2048.UP);
							break;
						}
					}
				}

				if (moves.size() == 4) {
					return moves;
				}
			}
		}

		if (!set[Action2048.RIGHT.id()] || !set[Action2048.LEFT.id()]) {
			for (int row = 0; row < SIZE; row++) {
				for (int col = 0; col < SIZE - 1; col++) {
					if (board[row][col] > 0 && board[row][col] == board[row][col + 1]) {
						set[Action2048.LEFT.id()] = true;
						set[Action2048.RIGHT.id()] = true;
						moves.add(Action2048.LEFT);
						moves.add(Action2048.RIGHT);
					}
				}
			}
		}

		if (!set[Action2048.DOWN.id()] || !set[Action2048.UP.id()]) {
			for (int col = 0; col < SIZE; col++) {
				for (int row = 0; row < SIZE - 1; row++) {
					if (board[row][col] > 0 && board[row][col] == board[row + 1][col]) {
						set[Action2048.UP.id()] = true;
						set[Action2048.DOWN.id()] = true;
						moves.add(Action2048.UP);
						moves.add(Action2048.DOWN);
					}
				}
			}
		}

		return moves;
	}

	private boolean hasEqualNeighbour(final int row, final int column) {
		if ((row > 0 && board[row - 1][column] == board[row][column])
			|| (column < SIZE - 1 && board[row][column + 1] == board[row][column])
			|| (row < SIZE - 1 && board[row + 1][column] == board[row][column])
			|| (column > 0 && board[row][column - 1] == board[row][column])) {
			return true;
		} else {
			return false;
		}
	}

	public boolean isTerminal() {
		for (int row = 0; row < SIZE; row++) {
			for (int column = 0; column < SIZE; column++) {
				if (board[row][column] == 0) {
					return false;
				}
			}
		}
		for (int row = 0; row < SIZE; row++) {
			for (int column = 0; column < SIZE; column++) {
				if (hasEqualNeighbour(row, column)) {
					return false;
				}
			}
		}
		return true;
	}

	public int getMaxTile() {
		int maxTile = 0;
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				maxTile = Math.max(maxTile, board[row][col]);
			}
		}

		return REWARDS[maxTile];
	}

	public void printHumanReadable() {
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				System.out.printf("%5d", REWARDS[board[row][col]]);
			}
			System.out.println();
		}
		System.out.println();
	}

	public static State2048 getInitialState(int numLocations, RandomDataGenerator random) {
		State2048 state = new State2048();

		for (int i = 0; i < numLocations; i++) {
			state.addRandomTile(random);
		}

		return state;
	}

	public static State2048 getInitialState(RandomDataGenerator random) {
		return getInitialState(NUM_INITIAL_LOCATIONS, random);
	}
}
