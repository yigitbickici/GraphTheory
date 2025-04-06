#include "algorithm/astar.cpp"
#include "algorithm/ucs.cpp"
#include "dataset/graph_data.cpp"
#include "visualize.cpp"
#include <fstream>
#include <chrono>
#include <queue>
#include <limits>

void writeGraphToFile(const vector<pair<int, pair<int, int>>>& edges, const string& filename) {
    ofstream file(filename);
    for (const auto& edge : edges) {
        file << edge.second.first << " " << edge.second.second << " " << edge.first << endl;
    }
    file.close();
}

vector<pair<int, pair<int, int>>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    vector<pair<int, pair<int, int>>> edges;
    int u, v, w;
    while (file >> u >> v >> w) {
        edges.push_back({w, {u, v}});
    }
    file.close();
    return edges;
}

// Calculate minimum weighted distance to target using Dijkstra
unsigned int calculateHeuristic(const vector<GraphNode>& nodes, int start, int target) {
    vector<unsigned int> dist(nodes.size(), numeric_limits<unsigned int>::max());
    vector<bool> visited(nodes.size(), false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == target) {
            return dist[u];
        }

        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& link : nodes[u].links) {
            int v = stoi(link.second->name);
            unsigned int weight = link.first;

            if (!visited[v] && dist[u] != numeric_limits<unsigned int>::max() && 
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    // If no path exists, return a value that won't overestimate
    return 0;
}

void writePath(const string& path, const string& filename) {
    ofstream file(filename);
    string number;
    vector<int> path_nodes;
    
    // Parse path string and extract node numbers
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
    
    // Write to file
    for (int node : path_nodes) {
        file << node << endl;
    }
    file.close();
}

// Graphviz PNG oluşturma fonksiyonu
void generatePNG(const string& dot_file, const string& algorithm_name) {
    string timestamp = to_string(chrono::system_clock::now().time_since_epoch().count());
    string output_file = algorithm_name + "_path_" + timestamp;
    string command = "dot -Tpng " + dot_file + " -o " + output_file + ".png";
    system(command.c_str());
    cout << algorithm_name << " için görselleştirme oluşturuldu: " << output_file << ".png" << endl;
}

void writeResultToCSV(const string& filename, int graphId, int vertices, int edges, int expandedNodes_astar, int expandedNodes_ucs) {
    // Check if file exists
    bool fileExists = false;
    ifstream checkFile(filename);
    if (checkFile.good()) {
        fileExists = true;
    }
    checkFile.close();

    // Open file in append mode
    ofstream file;
    if (!fileExists) {
        file.open(filename);
        if (!file.is_open()) {
            cout << "Error: Could not create file " << filename << endl;
            return;
        }
        file << "graph_id,vertices,edges,expanded_nodes_astar,expanded_nodes_ucs\n";
    } else {
        file.open(filename, ios::app);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return;
        }
    }

    // Write data
    file << graphId << "," 
         << vertices << "," 
         << edges << "," 
         << expandedNodes_astar << "," 
         << expandedNodes_ucs << "\n";
    
    file.close();
}

int main() {
    const string filename = "graph.txt";
    const string path_file = "path.txt";
    const string dot_file = "graph.dot";
    const string results_file = "algorithm_comparison.csv";  // Directly in current directory
    bool generateRandom = true;
    
    int globalGraphId = 1;

    vector<pair<int, int>> graphSizes = {
        {5, 8},     // Level 1: 5 vertices, 8 edges
        {10, 16},   // Level 2
        {15, 25},   // Level 3
        {20, 35},   // Level 4
        {25, 45},   // Level 5
        {30, 55},   // Level 6
        {35, 65}    // Level 7
    };

    // For each level of graph size
    for (const auto& size : graphSizes) {
        int numVertices = size.first;
        int numEdges = size.second;
        
        cout << "\n=== Testing Level with " << numVertices << " vertices and " 
             << numEdges << " edges ===" << endl;

        // Test 10 different graphs for each level
        for(int localGraphId = 1; localGraphId <= 10; localGraphId++, globalGraphId++) {
            cout << "\n--- Graph " << globalGraphId << " (Level Graph " << localGraphId << ") ---" << endl;
            
            vector<pair<int, pair<int, int>>> edges;
            
            if (generateRandom) {
                edges = generateRandomGraph(numVertices, numEdges);
                writeGraphToFile(edges, filename);
            }

            edges = readGraphFromFile(filename);

            vector<GraphNode> nodes(numVertices);
            for (int i = 0; i < numVertices; ++i) {
                nodes[i].name = to_string(i);
            }

            for (const auto& edge : edges) {
                nodes[edge.second.first].links.push_back({edge.first, &nodes[edge.second.second]});
            }

            cout << "\nGraph Structure:" << endl;
            for (const auto& edge : edges) {
                cout << edge.second.first << " -(" << edge.first << ")-> " << edge.second.second << endl;
            }

            unordered_map<string, unsigned int> heuristic;
            int target = numVertices - 1;
            for (int i = 0; i < numVertices; ++i) {
                heuristic[to_string(i)] = calculateHeuristic(nodes, i, target);
            }

            cout << "\nHeuristic values for each node:" << endl;
            for (int i = 0; i < numVertices; ++i) {
                cout << "h(" << i << ") = " << heuristic[to_string(i)] << endl;
            }

            // Variables to store expanded nodes
            int expandedNodes_astar = 0;
            int expandedNodes_ucs = 0;

            // Run A*
            auto start = chrono::high_resolution_clock::now();
            Path result_astar = solveAstar(&nodes[0], to_string(numVertices - 1), heuristic, expandedNodes_astar);
            auto end = chrono::high_resolution_clock::now();
            auto duration_astar = chrono::duration_cast<chrono::microseconds>(end - start);

            // Run UCS
            start = chrono::high_resolution_clock::now();
            Path result_ucs = solveUCS(&nodes[0], to_string(numVertices - 1), expandedNodes_ucs);
            end = chrono::high_resolution_clock::now();
            auto duration_ucs = chrono::duration_cast<chrono::microseconds>(end - start);

            // Save results with level information
            writeResultToCSV(results_file, globalGraphId, numVertices, numEdges, expandedNodes_astar, expandedNodes_ucs);

            cout << "A* Expanded Nodes: " << expandedNodes_astar << endl;
            cout << "UCS Expanded Nodes: " << expandedNodes_ucs << endl;
            cout << "A* Runtime: " << duration_astar.count() << " microseconds" << endl;
            cout << "UCS Runtime: " << duration_ucs.count() << " microseconds" << endl;

            // Only generate visualizations for first graph of each level
            if (localGraphId == 1) {
                if (result_astar.path != "NO SOLUTION") {
                    writePath(result_astar.path, "astar_" + path_file);
                    generateDotFile(edges, result_astar.path, "astar_" + dot_file);
                    generatePNG("astar_" + dot_file, "AStar");
                }
                if (result_ucs.path != "NO SOLUTION") {
                    writePath(result_ucs.path, "ucs_" + path_file);
                    generateDotFile(edges, result_ucs.path, "ucs_" + dot_file);
                    generatePNG("ucs_" + dot_file, "UCS");
                }
            }
        }
    }

    return 0;
} 