#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <queue>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <random>       
#include <cstdlib>
#include <ctime>


using namespace std;
typedef long long ll;

int randFun(int beg, int end);

vector<pair<int,int>> genPathRand2(pair<int,int> startPoint,  int size);

vector<pair<int,int>> ordenarVPar(vector<pair<int,int>> v);

void printVectorPair(vector<pair<int,int>> v, int size);

void printMatriz2(vector<vector<int>> mk);

void randomMapGen2(int size, int finishPointFila, int startPointFila);

//---------------------------------------
void printMaze();
void generateMaze();
//--------------------

int main();
