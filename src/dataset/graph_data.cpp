#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <set>

using namespace std;

vector<pair<int, pair<int, int>>> generateRandomGraph(int numVertices, int numEdges) {
    vector<pair<int, pair<int, int>>> edges; 
    set<pair<int, int>> edgeSet; 
    srand(time(0));
    
    for (int i = 0; i < numVertices - 1; i++) {
        int weight = rand() % 10 + 1; 
        edges.push_back({weight, {i, i + 1}});
        edgeSet.insert({i, i + 1});
        
        // Çapraz bağlantılar ekle
        if (i < numVertices - 2) {
            weight = rand() % 10 + 1;
            edges.push_back({weight, {i, i + 2}});
            edgeSet.insert({i, i + 2});
        }
    }
    
    // Ek rastgele kenarlar ekle
    int attempts = 0;
    while (edges.size() < numEdges && attempts < 100) {
        int u = rand() % numVertices;
        int v = rand() % numVertices;
        int weight = rand() % 10 + 1;
        
        // Kendine bağlantı olmasın ve aynı kenar tekrar eklenmesin
        if (u != v && edgeSet.find({u, v}) == edgeSet.end()) {
            edges.push_back({weight, {u, v}});
            edgeSet.insert({u, v});
        }
        attempts++;
    }
    
    int currentSize = edges.size();
    for (int i = 0; i < currentSize && edges.size() < numEdges; i++) {
        auto edge = edges[i];
        int u = edge.second.second;
        int v = edge.second.first;
        
        if (edgeSet.find({u, v}) == edgeSet.end()) {
            int weight = rand() % 10 + 1;
            edges.push_back({weight, {u, v}});
            edgeSet.insert({u, v});
        }
    }
    
    return edges;
}