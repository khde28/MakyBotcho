#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "mapas2.h"

using namespace std;
//directions
/*  */

// Edge and node
struct Edge {
    int to;
    int weight;
};
struct Nodito{
    int name;
    int cx;
    int cy;
};

//adjacency list
inline unordered_map<int, vector<Edge>> graph;
inline unordered_map<int, int> nodeWeights;
inline unordered_map<int, Nodito> noditos;

inline void addNodito(int x, int y, int matrix[8][8]) {
    int nodito_name = x * 8 + y;
    nodeWeights[nodito_name] = 0; 

    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        if ((newX >= 0) && (newX < 8) && (newY >= 0) && (newY < 8) &&
            (matrix[newX][newY] == 0 || matrix[newX][newY] == -1)) {
            int adjNodeName = newX * 8 + newY;
            graph[nodito_name].push_back({adjNodeName, 1});
            noditos[nodito_name] = {nodito_name,x,y};
            
        }
    }   
}
inline void generando(){
    mapLoaded::loadFile("mc.txt");
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (mapLoaded::mapa2[0][i][j] == 0 || mapLoaded::mapa2[0][i][j] == -1) {
                    addNodito(i, j, mapLoaded::mapa2[0]);
                }
            }
        }
    /* cout << "Grafo:" << endl;
    for (auto& node : graph) {
        cout << "Nodo " << noditos[node.first].cx<<" , "<<noditos[node.first].cy << " tiene conexiones: ";
        for (auto& edge : node.second) {
            cout << "(" << noditos[edge.to].cx<< " , " << noditos[edge.to].cy << ", peso: " << edge.weight << ") ";
        }
        cout << endl;
    } */
}

    

 
