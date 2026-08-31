/*
Using the "Backtracking" and "Branch and Bound" programming techniques write a program that solves a maze in C++. You can work in couples.

The program receives from the standard input two integers M and N, followed by M lines of N boolean values (0|1) separated by a space, representing the maze.  A 1 represents a square in which it is possible to move, a 0 is a square through which it is NOT possible to pass.

The origin or start square is always the square (0,0) and the exit or goal is always the square (M-1, N-1).

The output of the program is a matrix of Boolean values (0|1) representing the path out of the maze. You must first display the solution using the backtracking technique, and then using the branch and bound technique.

Your program should be called main.cpp and should compile using the g++ command in a Linux environment.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>
using namespace std;
using ll = long long;

struct Node
{
  int i, j, g, bound;
  bool operator<(const Node &otro) const { return bound > otro.bound; }
};

int h(int i, int j, int M, int N)
{
  return (M - 1 - i) + (N - 1 - j);
}

void imprimir(vector<vector<int>> &m)
{
  int M = m.size(), N = m[0].size();
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if (j)
        cout << " ";
      cout << m[i][j];
    }
    cout << "\n";
  }
}

bool branchAndBound(vector<vector<int>> &laberinto, vector<vector<int>> &res)
{
  int M = laberinto.size();
  int N = laberinto[0].size();

  if (laberinto[0][0] == 0 || laberinto[M - 1][N - 1] == 0)
    return false;

  // g[i][j] = least number of steps known to get to (i,j)
  vector<vector<int>> g(M, vector<int>(N, INT_MAX));

  // father[i][j] = which square was passed before that square
  vector<vector<pair<int, int>>> father(M, vector<pair<int, int>>(N, {-1, -1}));

  priority_queue<Node> alive;
  g[0][0] = 0;
  alive.push({0, 0, 0, h(0, 0, M, N)});

  // Best solution so far
  int best = INT_MAX;

  // 4 directions
  int di[4] = {1, 0, -1, 0};
  int dj[4] = {0, 1, 0, -1};

  while (!alive.empty())
  {
    Node n = alive.top();
    alive.pop();

    // Prune if expected solution for the branch is worse or the same that the best recorded
    if (n.bound >= best)
      continue;

    // Prune if required more steps than the minimum recorded for that square
    if (n.g > g[n.i][n.j])
      continue;

    // Record best solution and prune the branch
    if (n.i == M - 1 && n.j == N - 1)
    {
      best = n.g;
      continue;
    }

    // Branching
    for (int k = 0; k < 4; k++)
    {
      int ni = n.i + di[k];
      int nj = n.j + dj[k];

      if (ni < 0 || ni >= M || nj < 0 || nj >= N)
        continue;
      if (laberinto[ni][nj] == 0)
        continue;

      int ng = n.g + 1;
      int nbound = ng + h(ni, nj, M, N);

      if (nbound < best && ng < g[ni][nj])
      {
        g[ni][nj] = ng;
        father[ni][nj] = {n.i, n.j};
        alive.push({ni, nj, ng, nbound});
      }
    }
  }

  if (best == INT_MAX)
    return false;

  int i = M - 1, j = N - 1;
  while (i != -1)
  {
    res[i][j] = 1;
    auto p = father[i][j];
    i = p.first;
    j = p.second;
  }
  return true;
}

bool backtrack(int i, int j, vector<vector<int>> &laberinto, vector<vector<int>> &res)
{
  int M = laberinto.size();
  int N = laberinto[0].size();
  if ((i < 0) || (i >= M) || (j < 0) || (j >= N))
    return false;

  if (laberinto[i][j] == 0)
    return false;

  if (res[i][j] == 1)
    return false;

  res[i][j] = 1;
  if ((i == M - 1) && (j == N - 1))
    return true;

  if (backtrack(i + 1, j, laberinto, res))
    return true;
  if (backtrack(i, j + 1, laberinto, res))
    return true;
  if (backtrack(i - 1, j, laberinto, res))
    return true;
  if (backtrack(i, j - 1, laberinto, res))
    return true;

  res[i][j] = 0;
  return false;
}

int main()
{

  ios::sync_with_stdio(false);

  cin.tie(nullptr);

  int M, N;
  cout << "Input: " << endl;
  cin >> M >> N;

  vector<vector<int>> laberinto(M, vector<int>(N, 0));

  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cin >> laberinto[i][j];
    }
  }

  if (laberinto[0][0] == 0 || laberinto[M - 1][N - 1] == 0)
  {
    cout << "Invalid input";
    return 0;
  }

  cout << "\nOutput : " << endl;

  vector<vector<int>> solutionBacktracking(M, vector<int>(N, 0));

  if (backtrack(0, 0, laberinto, solutionBacktracking))
  {
    imprimir(solutionBacktracking);
    cout << endl;
  }
  else
  {
    cout << "No backtrack solution" << endl;
  }

  vector<vector<int>> solutionBranchAndBound(M, vector<int>(N, 0));

  if (branchAndBound(laberinto, solutionBranchAndBound))
  {
    imprimir(solutionBranchAndBound);
  }
  else
  {
    cout << "No branch and bound solution";
  }

  return 0;
}