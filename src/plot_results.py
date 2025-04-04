import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the CSV file
df = pd.read_csv('src/algorithm_comparison.csv')

# Create the plot
plt.figure(figsize=(12, 6))

# Plot expanded nodes comparison
plt.subplot(1, 2, 1)
plt.plot(df['graph_id'], df['expanded_nodes_astar'], marker='o', label='A*')
plt.plot(df['graph_id'], df['expanded_nodes_ucs'], marker='o', label='UCS')
plt.xlabel('Graph ID')
plt.ylabel('Number of Expanded Nodes')
plt.title('Expanded Nodes Comparison')
plt.legend()
plt.grid(True)

# Plot difference in expanded nodes
plt.subplot(1, 2, 2)
df['node_difference'] = df['expanded_nodes_ucs'] - df['expanded_nodes_astar']
sns.barplot(data=df, x='graph_id', y='node_difference')
plt.xlabel('Graph ID')
plt.ylabel('UCS - A* (Expanded Nodes)')
plt.title('Difference in Expanded Nodes (UCS - A*)')
plt.grid(True)

# Adjust layout and save
plt.tight_layout()
plt.savefig('algorithm_comparison_plot.png')
plt.close()

# Print statistics
print("\nStatistical Summary:")
print("\nA* Expanded Nodes:")
print(df['expanded_nodes_astar'].describe())
print("\nUCS Expanded Nodes:")
print(df['expanded_nodes_ucs'].describe())
print("\nAverage improvement (UCS - A*):")
print(df['node_difference'].mean()) 