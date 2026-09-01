#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Node {
	int i;
	int j;
	int steps;
	int bound;
	bool operator<(const Node &other) const {
		return bound > other.bound;
	}
};

// Returns distance between square (M-1, N-1) and square (i, j)
int getDistance(int i, int j, int rows, int cols) {
	return (rows - 1 - i) + (cols - 1 - j);
}

// Builds the solution matrix for B&B using the father matrix that stores the previous square for square (i,j)
void reconstructPath(int rows, int cols, vector<vector<pair<int, int>>> &father, vector<vector<int>> &solution) {
	int i = rows - 1;
	int j = cols - 1;
	while (i != -1) {
		solution[i][j] = 1;
		auto previous = father[i][j];
		i = previous.first;
		j = previous.second;
	}
}

void printMatrix(vector<vector<int>> &m) {
	int M = m.size();
	int N = m[0].size();
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (j)
				cout << " ";
			cout << m[i][j];
		}
		cout << "\n";
	}
	cout << "\n";
}

bool branchAndBound(vector<vector<int>> &maze, vector<vector<int>> &solution) {
	int rows = maze.size();
	int cols = maze[0].size();

	if (maze[0][0] == 0 || maze[rows - 1][cols - 1] == 0)
		return false;

	// Matrix that stores the lowest amount of steps to reach square (i,j)
	vector<vector<int>> minSteps(rows, vector<int>(cols, INT_MAX));

	// Matrix that stores the previous square to reach square (i,j) in the shortest path found
	vector<vector<pair<int, int>>> previousSquare(rows, vector<pair<int, int>>(cols, {-1, -1}));

	// Min heap of Nodes, ordered by bound, to retrieve the most promising path to explore in O(1)
	priority_queue<Node> alive;

	minSteps[0][0] = 0;
	alive.push({0, 0, 0, getDistance(0, 0, rows, cols)});

	// Lowest bound found
	int best = INT_MAX;

	int di[4] = {1, 0, -1, 0};
	int dj[4] = {0, 1, 0, -1};

	// While there are paths to explore
	while (!alive.empty()) {
		Node current = alive.top();
		alive.pop();

		// If the best possible path is worse than the best solution found, there are no more Nodes to review. Prune and construct path
		if (current.bound >= best)
			break;

		// If the number of steps it took to reach the Node is greater that the minimum steps needed to reach that Node, prune that branch
		if (current.steps > minSteps[current.i][current.j])
			continue;

		// If the Node is the end point, store the bound as the best solution found and prune the branch
		if (current.i == rows - 1 && current.j == cols - 1) {
			best = current.steps;
			continue;
		}

		// Branch to all four directions
		for (int k = 0; k < 4; k++) {
			int nextI = current.i + di[k];
			int nextJ = current.j + dj[k];

			// If the branch leaves the boundaries of the maze, discard it
			if (nextI < 0 || nextI >= rows || nextJ < 0 || nextJ >= cols)
				continue;

			// If the square is a wall, discard it
			if (maze[nextI][nextJ] == 0)
				continue;

			int nextSteps = current.steps + 1;
			int nextBound = nextSteps + getDistance(nextI, nextJ, rows, cols);

			// Only check in that direction if the branch is more promising (lower bound and lower steps)
			if (nextBound < best && nextSteps < minSteps[nextI][nextJ]) {
				minSteps[nextI][nextJ] = nextSteps;
				previousSquare[nextI][nextJ] = {current.i, current.j};
				alive.push({nextI, nextJ, nextSteps, nextBound});
			}
		}
	}

	// There was no recorded best solution
	if (best == INT_MAX)
		return false;

	reconstructPath(rows, cols, previousSquare, solution);
	return true;
}

bool backtrack(int i, int j, vector<vector<int>> &maze, vector<vector<int>> &solution) {
	int rows = maze.size();
	int cols = maze[0].size();

	// If square (i,j) is out of the maze boundaries, no possible path. Backtrack to previous point
	if (i < 0 || i >= rows || j < 0 || j >= cols)
		return false;

	// If square (i,j) is a wall, no possible path. Backtrack to previous point
	if (maze[i][j] == 0)
		return false;

	// If square (i,j) was already visited, no possible path. Backtrack to previous point
	if (solution[i][j] == 1)
		return false;

	// Mark the path taken
	solution[i][j] = 1;

	// If you are at the end square, solution found
	if (i == rows - 1 && j == cols - 1)
		return true;

	// If any of the directions finds a solution, return true
	if (backtrack(i + 1, j, maze, solution))
		return true;
	if (backtrack(i, j + 1, maze, solution))
		return true;
	if (backtrack(i - 1, j, maze, solution))
		return true;
	if (backtrack(i, j - 1, maze, solution))
		return true;

	// If no direction found a solution, unmark the square and backtrack
	solution[i][j] = 0;
	return false;
}

int main() {

	ios::sync_with_stdio(false);

	// Test 1

	cout << "Test 1: Base case" << endl;
	
	int M = 4;
	int N = 4;

	vector<vector<int>> maze = {
	    {1, 0, 0, 0},
	    {1, 1, 0, 1},
	    {0, 1, 0, 0},
	    {1, 1, 1, 1}};

	printMatrix(maze);
	
	vector<vector<int>> solutionBacktracking(M, vector<int>(N, 0));

	if (backtrack(0, 0, maze, solutionBacktracking)) {
		cout << "Backtrack:" << endl;
		printMatrix(solutionBacktracking);
	} else {
		cout << "No backtrack solution" << endl;
	}

	vector<vector<int>> solutionBranchAndBound(M, vector<int>(N, 0));

	if (branchAndBound(maze, solutionBranchAndBound)) {
		cout << "Branch & Bound" << endl;
		printMatrix(solutionBranchAndBound);
	} else {
		cout << "No B&B solution";
	}

	// Test 2
	/*
	1 1 1 1 1
	1 0 0 0 1
	1 0 1 1 1
	1 0 1 0 1
	1 1 1 0 1
	*/

	return 0;
}