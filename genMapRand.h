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
using namespace std;
typedef long long ll;

int randFun(int beg, int end);

vector<pair<int,int>> genPathRand(pair<int,int> startPoint,  int size);

vector<pair<int,int>> ordenarVPar(vector<pair<int,int>> v);

void printVectorPair(vector<pair<int,int>> v, int size);

void printMatriz(vector<vector<int>> mk);

void randomMapGen(int size, int finishPointFila, int startPointFila);

int main();
