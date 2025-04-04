#include <iostream>
#include <queue>
#include <unordered_set>
#include <string>
#include <list>

using namespace std;

struct CompareNode {
    bool operator()(const TreeNode* n1, const TreeNode* n2) const {
        return n1->path.cost > n2->path.cost;
    }
};

Path solveUCS(GraphNode* graph, string goal, int& expandedNodes) {
    

    Path ans;
    ans.path = "NO SOLUTION";
    ans.cost = 0;

    expandedNodes = 0;  // Initialize counter
    unordered_set<string> visited;

    if (graph != nullptr) {
        TreeNode* root = new TreeNode;
        root->node = graph;
        root->path.path = root->node->name;
        root->path.cost = 0;

        priority_queue<TreeNode*, vector<TreeNode*>, CompareNode> fringe;
        fringe.push(root);

        while (!fringe.empty()) {
            TreeNode* stateToExpand = fringe.top();
            fringe.pop();

            if (visited.count(stateToExpand->node->name) == 0) {
                visited.insert(stateToExpand->node->name);
                expandedNodes++;  // Increment counter

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
                    fringe.push(state);
                }
            }

            delete stateToExpand;
        }
    }
    cout << "Node expanded: " << expandedNodes << endl;
    return ans;
} 