#include <fstream>
#include <string>
#include <vector>

using namespace std;

void generateDotFile(const vector<pair<int, pair<int, int>>>& edges, const string& path, const string& filename) {
    ofstream file(filename);
    
    // Grafiği başlat ve görünüm ayarlarını yap
    file << "digraph G {\n";
    file << "    rankdir=RL;\n";
    file << "    size=\"30,20\";\n";        // Çok daha büyük boyut
    file << "    ratio=fill;\n";            // Belirtilen boyuta tam dolduracak şekilde ölçekle
    file << "    node [shape=circle, fixedsize=true, width=1];\n";  // Sabit boyutlu nodlar
    
    // Tüm kenarları gri yap
    for (const auto& edge : edges) {
        file << "    " << edge.second.first << " -> " << edge.second.second 
             << " [label=\"" << edge.first << "\", color=gray];\n";
    }
    
    // Path'teki kenarları kırmızı yap
    if (path != "NO SOLUTION") {
        string number;
        vector<int> path_nodes;
        
        // Path string'ini parse et
        for (char c : path) {
            if (isdigit(c)) {
                number += c;
            } else if (!number.empty()) {
                path_nodes.push_back(stoi(number));
                number.clear();
            }
        }
        if (!number.empty()) {
            path_nodes.push_back(stoi(number));
        }
        
        // Path'teki nodeları pembe yap
        for (int node : path_nodes) {
            file << "    " << node << " [style=filled, fillcolor=lightpink];\n";
        }
        
        // Path'teki kenarları kırmızı yap
        for (size_t i = 0; i < path_nodes.size() - 1; ++i) {
            file << "    " << path_nodes[i] << " -> " << path_nodes[i + 1] 
                 << " [color=red, penwidth=3.0];\n";  // Kenar kalınlığını artırdık
        }
    }
    
    file << "}\n";
    file.close();
} 