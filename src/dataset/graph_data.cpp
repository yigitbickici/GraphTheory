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
    
    // Her node için en az bir bağlantı garantile
    for (int i = 0; i < numVertices; i++) {
        int j = (i + 1) % numVertices;
        int weight = 1 + (gen() % 20);
        edges.push_back({weight, {i, j}});
    }
    
    // Kalan kenarları rastgele ekle
    while (edges.size() < numEdges) {
        int i = gen() % numVertices;
        int j = gen() % numVertices;
        
        if (i != j) {
            // Aynı kenarın tekrar eklenmesini önle
            bool exists = false;
            for (const auto& edge : edges) {
                if (edge.second.first == i && edge.second.second == j) {
                    exists = true;
                    break;
                }
            }
            
            if (!exists) {
                int weight = 1 + (gen() % 20); // 1-20 arası ağırlık
                edges.push_back({weight, {i, j}});
                
                // Bazen çift yönlü kenar ekle (farklı ağırlıklarla)
                if (gen() % 2 == 0) {
                    weight = 1 + (gen() % 20); // Farklı bir ağırlık
                    edges.push_back({weight, {j, i}});
                }
            }
        }
    }
    
    // Uzun mesafeli kısayollar ekle
    int extraEdges = numVertices / 2;
    for (int i = 0; i < extraEdges; i++) {
        int start = gen() % numVertices;
        int end = (start + 3 + (gen() % (numVertices-3))) % numVertices;
        int weight = 1 + (gen() % 10); // Düşük ağırlıklı uzun bağlantılar
        edges.push_back({weight, {start, end}});
    }
    
    return edges;
}