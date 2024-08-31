#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include "get_graph_al.h"
using namespace std;

vector<int> dijkstra(int startNode, int rows, int cols, vector<int>& previousNode) {
    vector<int> distances(rows * cols, numeric_limits<int>::max());
    previousNode.resize(rows * cols, -1);
    distances[startNode] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, startNode});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDist > distances[currentNode]) continue;

        for (const Edge& edge : graph[currentNode]) {
            int nextNode = edge.to;
            int newDist = currentDist + edge.weight;

            if (newDist < distances[nextNode]) {
                distances[nextNode] = newDist;
                previousNode[nextNode] = currentNode;
                pq.push({newDist, nextNode});
            }
        }
    }

    return distances;
}


vector<int> reconstructPath(int startNode, int targetNode, const vector<int>& previousNode) {
    vector<int> path;
    for (int at = targetNode; at != -1; at = previousNode[at]) {
        path.push_back(at);
        if (at == startNode) break;
    }
    reverse(path.begin(), path.end());
    return path;
}

inline vector<int> optPath() {
    generando();

    int startNode = 0; 
    int targetNode = 7 * 8 + 0; 

    vector<int> previousNode;

    vector<int> distances = dijkstra(startNode, 8, 8, previousNode);

    int optDist =  distances[targetNode];

    vector<int> path = reconstructPath(startNode, targetNode, previousNode);
    for (int node : path) {
        cout << "(" << node / 8 << ", " << node % 8 << ") ";
    }
    
    cout <<"estodo"<< endl;
    return path;
}
