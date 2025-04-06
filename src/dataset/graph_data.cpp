#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>

using namespace std;

vector<pair<int, pair<int, int>>> generateRandomGraph(int numVertices, int numEdges) {
    vector<pair<int, pair<int, int>>> edges;
    random_device rd;
    mt19937 gen(rd());
    
    // Minimum spanning tree benzeri yapı oluştur (bağlantılı graf için)
    int initialEdges = numVertices - 1;
    for (int i = 0; i < initialEdges && edges.size() < numEdges; i++) {
        int weight = 1 + (gen() % 20);
        edges.push_back({weight, {i, i + 1}});
    }
    
    // Son düğümü ilk düğüme bağla (döngü oluştur)
    if (edges.size() < numEdges) {
        int weight = 1 + (gen() % 20);
        edges.push_back({weight, {numVertices - 1, 0}});
    }
    
    // Kalan kenarları rastgele ekle
    while (edges.size() < numEdges) {
        int i = gen() % numVertices;
        int j = gen() % numVertices;
        
        if (i != j) {
            // Aynı kenarın tekrar eklenmesini önle
            bool exists = false;
            for (const auto& edge : edges) {
                if ((edge.second.first == i && edge.second.second == j) ||
                    (edge.second.first == j && edge.second.second == i)) {
                    exists = true;
                    break;
                }
            }
            
            if (!exists) {
                int weight = 1 + (gen() % 20); // 1-20 arası ağırlık
                edges.push_back({weight, {i, j}});
            }
        }
    }
    
    return edges;
}