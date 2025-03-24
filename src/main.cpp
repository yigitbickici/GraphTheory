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

int main() {
    const string filename = "graph.txt";
    const string path_file = "path.txt";
    const string dot_file = "graph.dot";
    bool generateRandom = true;  // Set this to false to only read from file
    
    int numVertices = 20;
    int numEdges = 35;     // Daha fazla kenar ekleyelim
    vector<pair<int, pair<int, int>>> edges;

    if (generateRandom) {
        // Generate random graph
        edges = generateRandomGraph(numVertices, numEdges);
        writeGraphToFile(edges, filename);
    }

    // Read graph from file
    edges = readGraphFromFile(filename);
    numVertices = 0;  // Calculate actual number of vertices from edges
    for (const auto& edge : edges) {
        numVertices = max(numVertices, max(edge.second.first, edge.second.second) + 1);
    }

    // Graph nodes
    vector<GraphNode> nodes(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        nodes[i].name = to_string(i);
    }

    // Add edges to nodes
    for (const auto& edge : edges) {
        nodes[edge.second.first].links.push_back({edge.first, &nodes[edge.second.second]});
    }

    cout << "Graph Structure:" << endl;
    for (const auto& edge : edges) {
        cout << edge.second.first << " -(" << edge.first << ")-> " << edge.second.second << endl;
    }

    // Calculate heuristic values for each node
    unordered_map<string, unsigned int> heuristic;
    int target = numVertices - 1;
    for (int i = 0; i < numVertices; ++i) {
        heuristic[to_string(i)] = calculateHeuristic(nodes, i, target);
    }

    cout << "\nHeuristic values for each node:" << endl;
    for (int i = 0; i < numVertices; ++i) {
        cout << "h(" << i << ") = " << heuristic[to_string(i)] << endl;
    }

    const int NUM_RUNS = 1;
    long long total_duration_astar = 0;
    long long total_duration_ucs = 0;

    for (int run = 0; run < NUM_RUNS; run++) {
        cout << "\nRun " << run + 1 << ":" << endl;
        
        auto start = chrono::high_resolution_clock::now();
        Path result_astar = solveAstar(&nodes[0], to_string(numVertices - 1), heuristic);
        auto end = chrono::high_resolution_clock::now();
        auto duration_astar = chrono::duration_cast<chrono::microseconds>(end - start);
        total_duration_astar += duration_astar.count();

        start = chrono::high_resolution_clock::now();
        Path result_ucs = solveUCS(&nodes[0], to_string(numVertices - 1));
        end = chrono::high_resolution_clock::now();
        auto duration_ucs = chrono::duration_cast<chrono::microseconds>(end - start);
        total_duration_ucs += duration_ucs.count();

        cout << "A* Runtime: " << duration_astar.count() << " microseconds" << endl;
        cout << "Dijkstra Runtime: " << duration_ucs.count() << " microseconds" << endl;

        if (run == 0) {
            cout << "\nShortest Path (A*): " << result_astar.path << endl;
            cout << "Total Cost (A*): " << result_astar.cost << endl;
            cout << "\nShortest Path (Dijkstra): " << result_ucs.path << endl;
            cout << "Total Cost (Dijkstra): " << result_ucs.cost << endl;
        }

        
        if (run == 0) {
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

    cout << "\nAverage Runtime over " << NUM_RUNS << " runs:" << endl;
    cout << "A* Average: " << total_duration_astar / NUM_RUNS << " microseconds" << endl;
    cout << "UCS Average: " << total_duration_ucs / NUM_RUNS << " microseconds" << endl;

    return 0;
} 