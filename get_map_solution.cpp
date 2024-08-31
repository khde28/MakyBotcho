#include <iostream>
#include <vector>
#include <unordered_map>

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
unordered_map<int, vector<Edge>> graph;
unordered_map<int, int> nodeWeights;
unordered_map<int, Nodito> noditos;

void addNodito(int x, int y, vector<vector<int>>& matrix) {
    int nodito_name = x * matrix[0].size() + y;
    nodeWeights[nodito_name] = 0; 

    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        if ((newX >= 0) && (newX < int(matrix.size())) && (newY >= 0) && (newY < int(matrix[0].size())) &&
            (matrix[newX][newY] == 0 || matrix[newX][newY] == -1)) {
            int adjNodeName = newX * matrix[0].size() + newY;
            graph[nodito_name].push_back({adjNodeName, 1});
            noditos[nodito_name] = {nodito_name,x,y};
            
        }
    }   
}

int main() {
    vector<vector<int>> matrix = {
        {0, 1, 2, 3, 4, 5, 6, 20},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {7, 6, 5, 4, 3, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 4, 5, 6, 7},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {7, 6, 5, 4, 3, 2, 1, 0},
        {-1, 0, 0, 0, 0, 0, 0, 0}
    };


    for (int i = 0; i < int(matrix.size()); ++i) {
        for (int j = 0; j < int(matrix[0].size()); ++j) {
            if (matrix[i][j] == 0 || matrix[i][j] == -1) {
                addNodito(i, j, matrix);
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

    

    return 0;
}
