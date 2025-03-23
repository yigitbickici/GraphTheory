#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>

using namespace std;

struct Path {
    string path;
    unsigned int cost;
    unsigned int hCost;
};

struct GraphNode {
    string name;
    list<pair<unsigned int, GraphNode*>> links;
};

struct TreeNode {
    GraphNode* node;
    Path path;
};

struct CompareNode_Astar {
    bool operator()(const TreeNode* n1, const TreeNode* n2) const {
        return (n1->path.cost + n1->path.hCost) > (n2->path.cost + n2->path.hCost);
    }
};

Path solveAstar(GraphNode* graph, string goal, unordered_map<string, unsigned int>& heuristic) {
    cout << "A* Search" << endl;

    Path ans;
    ans.path = "NO SOLUTION";
    ans.cost = 0;

    unsigned int expandedNodes = 0;
    unordered_set<string> visited;

    if (graph != nullptr) {
        TreeNode* root = new TreeNode;
        root->node = graph;
        root->path.path = root->node->name;
        root->path.cost = 0;
        root->path.hCost = heuristic.at(root->node->name);

        priority_queue<TreeNode*, vector<TreeNode*>, CompareNode_Astar> fringe;
        fringe.push(root);

        while (!fringe.empty()) {
            TreeNode* stateToExpand = fringe.top();
            fringe.pop();

            if (visited.count(stateToExpand->node->name) == 0) {
                visited.insert(stateToExpand->node->name);
                expandedNodes++;

                if (stateToExpand->node->name == goal) {
                    ans = stateToExpand->path;
                    delete stateToExpand;
                    while (!fringe.empty()) {
                        TreeNode* toDelete = fringe.top();
                        fringe.pop();
                        delete toDelete;
                    }
                    break;
                }

                for (auto it = stateToExpand->node->links.begin(); it != stateToExpand->node->links.end(); ++it) {
                    TreeNode* state = new TreeNode;
                    state->node = it->second;
                    state->path.path = stateToExpand->path.path + ", " + state->node->name;
                    state->path.cost = stateToExpand->path.cost + it->first;
                    state->path.hCost = heuristic.at(state->node->name);
                    fringe.push(state);
                }
            }

            delete stateToExpand;
        }
    }
    cout << "Node expanded: " << expandedNodes << endl;
    return ans;
} 