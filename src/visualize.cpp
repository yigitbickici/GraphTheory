#include <fstream>
#include <string>
#include <vector>

using namespace std;

void generateDotFile(const vector<pair<int, pair<int, int>>>& edges, 
                    const string& path,
                    const string& outputFile) {
    ofstream file(outputFile);
    
    // DOT dosyası başlangıcı
    file << "digraph G {\n";
    file << "    node [style=filled];\n";
    
    // Tüm kenarları ekle
    for (const auto& edge : edges) {
        int source = edge.second.first;
        int target = edge.second.second;
        int weight = edge.first;
        
        file << "    " << source << " -> " << target;
        file << " [label=\"" << weight << "\"];\n";
    }
    
    // Yolu işaretle (eğer varsa)
    if (!path.empty()) {
        file << "    // Highlight path nodes\n";
        string number;
        vector<int> pathNodes;
        
        // Parse path string
        for (char c : path) {
            if (isdigit(c)) {
                number += c;
            } else if (!number.empty()) {
                pathNodes.push_back(stoi(number));
                number.clear();
            }
        }
        if (!number.empty()) {
            pathNodes.push_back(stoi(number));
        }
        
        // Yoldaki düğümleri yeşil yap
        for (int node : pathNodes) {
            file << "    " << node << " [fillcolor=lightgreen];\n";
        }
        
        // Yoldaki kenarları kırmızı yap
        for (size_t i = 0; i < pathNodes.size() - 1; i++) {
            file << "    " << pathNodes[i] << " -> " << pathNodes[i+1];
            file << " [color=red, penwidth=2.0];\n";
        }
    }
    
    file << "}\n";
    file.close();
} 