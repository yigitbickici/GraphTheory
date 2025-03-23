import networkx as nx
import matplotlib.pyplot as plt

def read_graph_from_file(filename):
    G = nx.DiGraph()
    
    with open(filename, 'r') as f:
        for line in f:
            source, target, weight = map(int, line.strip().split())
            G.add_edge(source, target, weight=weight)
    
    return G

def visualize_graph(G, path=None):
    pos = nx.spring_layout(G)
    
    # Draw the graph
    plt.figure(figsize=(12, 8))
    
    # Draw edges
    nx.draw_networkx_edges(G, pos, edge_color='gray')
    
    # Draw edge labels
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    
    # Draw nodes
    nx.draw_networkx_nodes(G, pos, node_color='lightblue', node_size=500)
    
    # Draw node labels
    nx.draw_networkx_labels(G, pos)
    
    # Highlight the path if provided
    if path:
        path_edges = list(zip(path[:-1], path[1:]))
        nx.draw_networkx_edges(G, pos, edgelist=path_edges, edge_color='r', width=2)
        nx.draw_networkx_nodes(G, pos, nodelist=path, node_color='lightgreen', node_size=500)
    
    plt.title("Graph Visualization")
    plt.axis('off')
    plt.show()

def main():
    # Read the graph
    G = read_graph_from_file('../graph.txt')
    
    # Visualize without path
    visualize_graph(G)
    
    # Example: Visualize with a path (you can modify this based on the actual path)
    # path = [0, 2, 4, 7, 9]  # Example path
    # visualize_graph(G, path)

if __name__ == "__main__":
    main() 