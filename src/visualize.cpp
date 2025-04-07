#include <fstream>
#include <string>
#include <vector>

using namespace std;

void generateDotFile(const vector<pair<int, pair<int, int>>>& edges, const string& path, const string& filename) {
    ofstream file(filename);
    
    // Grafiği başlat ve görünüm ayarlarını yap
    file << "digraph G {\n";
    file << "    rankdir=RL;\n";
    file << "    size=\"30,20\";\n";        // Büyük boyut
    file << "    ratio=fill;\n";            // Belirtilen boyuta tam dolduracak şekilde ölçekle
    file << "    node [shape=circle, fixedsize=true, width=1, fontsize=14];\n";  // Daha büyük node yazıları
    file << "    edge [fontsize=30, penwidth=2.0];\n";  // Kenar yazılarını daha büyük yap
    
    // Tüm kenarları koyu gri yap ve kalınlaştır
    for (const auto& edge : edges) {
        file << "    " << edge.second.first << " -> " << edge.second.second 
             << " [label=\"" << edge.first << "\", color=\"#404040\", penwidth=2.0, fontsize=20];\n";
    }
    
    // Path'teki nodeları işaretle
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
        
        // Başlangıç nodunu kırmızı, hedef nodunu yeşil yap
        if (!path_nodes.empty()) {
            file << "    " << path_nodes.front() << " [style=filled, fillcolor=red, fontcolor=white];\n";
            file << "    " << path_nodes.back() << " [style=filled, fillcolor=green, fontcolor=white];\n";
            
            // Path üzerindeki ara nodeları açık mavi yap
            for (size_t i = 1; i < path_nodes.size() - 1; ++i) {
                file << "    " << path_nodes[i] << " [style=filled, fillcolor=lightblue];\n";
            }
        }
        
        // Path'teki kenarları mavi yap ve kalınlaştır
        for (size_t i = 0; i < path_nodes.size() - 1; ++i) {
            file << "    " << path_nodes[i] << " -> " << path_nodes[i + 1] 
                 << " [color=blue, penwidth=4.0, fontsize=20];\n";  // Path kenarlarının yazı boyutunu da artır
        }
    }
    
    file << "}\n";
    file.close();
} 