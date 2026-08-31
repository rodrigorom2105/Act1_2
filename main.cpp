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
using namespace std;
using ll = long long;


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

  vector<vector<int>> res(M, vector<int>(N, 0));

  cout << "\nOutput : " << endl;

  if (backtrack(0, 0, laberinto, res))
  {
    for (int i = 0; i < M; i++)
    {
      for (int j = 0; j < N; j++)
      {
        cout << res[i][j] << " ";
      }
      cout << "\n";
    }
    cout << endl;
  }
  else
  {
    cout << "No backtrack solution";
  }


}