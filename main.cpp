#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Node {
	int i, j, g, bound;
	bool operator<(const Node &otro) const {
		return bound > otro.bound;
	}
};

int getDistance(int i, int j, int M, int N) {
	return (M - 1 - i) + (N - 1 - j);
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
}

bool branchAndBound(vector<vector<int>> &maze, vector<vector<int>> &solution) {
	int M = maze.size();
	int N = maze[0].size();

	if (maze[0][0] == 0 || maze[M - 1][N - 1] == 0)
		return false;

	vector<vector<int>> g(M, vector<int>(N, INT_MAX));
	vector<vector<pair<int, int>>> father(M, vector<pair<int, int>>(N, {-1, -1}));

	priority_queue<Node> alive;
	g[0][0] = 0;
	alive.push({0, 0, 0, getDistance(0, 0, M, N)});

	int best = INT_MAX;

	int di[4] = {1, 0, -1, 0};
	int dj[4] = {0, 1, 0, -1};

	while (!alive.empty()) {
		Node n = alive.top();
		alive.pop();

		if (n.bound >= best)
			continue;

		if (n.g > g[n.i][n.j])
			continue;

		if (n.i == M - 1 && n.j == N - 1) {
			best = n.g;
			continue;
		}

		for (int k = 0; k < 4; k++) {
			int ni = n.i + di[k];
			int nj = n.j + dj[k];

			if (ni < 0 || ni >= M || nj < 0 || nj >= N)
				continue;
			if (maze[ni][nj] == 0)
				continue;

			int ng = n.g + 1;
			int nbound = ng + getDistance(ni, nj, M, N);

			if (nbound < best && ng < g[ni][nj]) {
				g[ni][nj] = ng;
				father[ni][nj] = {n.i, n.j};
				alive.push({ni, nj, ng, nbound});
			}
		}
	}

	if (best == INT_MAX)
		return false;

	int i = M - 1;
	int j = N - 1;
	while (i != -1) {
		solution[i][j] = 1;
		auto p = father[i][j];
		i = p.first;
		j = p.second;
	}
	return true;
}

bool backtrack(int i, int j, vector<vector<int>> &maze, vector<vector<int>> &solution) {
	int rows = maze.size();
	int cols = maze[0].size();
	if (i < 0 || i >= rows || j < 0 || j >= cols)
		return false;

	if (maze[i][j] == 0)
		return false;

	if (solution[i][j] == 1)
		return false;

	solution[i][j] = 1;
	if ((i == rows - 1) && (j == cols - 1))
		return true;

	if (backtrack(i + 1, j, maze, solution))
		return true;
	if (backtrack(i, j + 1, maze, solution))
		return true;
	if (backtrack(i - 1, j, maze, solution))
		return true;
	if (backtrack(i, j - 1, maze, solution))
		return true;

	solution[i][j] = 0;
	return false;
}

int main() {

	ios::sync_with_stdio(false);

	cin.tie(nullptr);

	int M;
	int N;
	cout << "Input: " << endl;
	cin >> M >> N;

	vector<vector<int>> maze(M, vector<int>(N, 0));

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			cin >> maze[i][j];
		}
	}

	if (maze[0][0] == 0 || maze[M - 1][N - 1] == 0) {
		cout << "Invalid input";
		return 0;
	}

	cout << "\nOutput : " << endl;

	vector<vector<int>> solutionBacktracking(M, vector<int>(N, 0));

	if (backtrack(0, 0, maze, solutionBacktracking)) {
		printMatrix(solutionBacktracking);
		cout << endl;
	} else {
		cout << "No backtrack solution" << endl;
	}

	vector<vector<int>> solutionBranchAndBound(M, vector<int>(N, 0));

	if (branchAndBound(maze, solutionBranchAndBound)) {
		printMatrix(solutionBranchAndBound);
	} else {
		cout << "No branch and bound solution";
	}

	return 0;
}