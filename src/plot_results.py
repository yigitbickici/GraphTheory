import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Create results directory if it doesn't exist
os.makedirs('src/results', exist_ok=True)

try:
    # Read the CSV file from the correct location
    df = pd.read_csv('src/algorithm_comparison.csv')

    # Process each vertex size separately
    for vertices in sorted(df['vertices'].unique()):
        level_data = df[df['vertices'] == vertices]
        edges = level_data['edges'].iloc[0]  # Get edge count for this level
        
        plt.figure(figsize=(15, 10))
        
        # Plot 1: Comparison of expanded nodes for each graph
        plt.subplot(2, 2, 1)
        x = range(1, len(level_data) + 1)
        plt.plot(x, level_data['expanded_nodes_astar'], marker='o', label='A*')
        plt.plot(x, level_data['expanded_nodes_ucs'], marker='o', label='UCS')
        plt.xlabel('Graph Number')
        plt.ylabel('Number of Expanded Nodes')
        plt.title(f'Expanded Nodes Comparison\n(V={vertices}, E={edges})')
        plt.legend()
        plt.grid(True)

        # Plot 2: Box plot of expanded nodes
        plt.subplot(2, 2, 2)
        data_to_plot = pd.melt(level_data[['expanded_nodes_astar', 'expanded_nodes_ucs']], 
                            var_name='Algorithm', value_name='Expanded Nodes')
        sns.boxplot(data=data_to_plot, x='Algorithm', y='Expanded Nodes')
        plt.title(f'Distribution of Expanded Nodes\n(V={vertices}, E={edges})')

        # Plot 3: Improvement (UCS - A*)
        plt.subplot(2, 2, 3)
        improvement = level_data['expanded_nodes_ucs'] - level_data['expanded_nodes_astar']
        plt.bar(x, improvement)
        plt.xlabel('Graph Number')
        plt.ylabel('UCS - A* (Expanded Nodes)')
        plt.title(f'Node Expansion Improvement\n(V={vertices}, E={edges})')
        plt.grid(True)

        # Plot 4: Improvement percentage
        plt.subplot(2, 2, 4)
        improvement_percentage = (level_data['expanded_nodes_ucs'] - level_data['expanded_nodes_astar']) / level_data['expanded_nodes_ucs'] * 100
        plt.bar(x, improvement_percentage)
        plt.xlabel('Graph Number')
        plt.ylabel('Improvement Percentage')
        plt.title(f'Improvement Percentage\n(V={vertices}, E={edges})')
        plt.grid(True)

        # Adjust layout and save
        plt.tight_layout()
        plt.savefig(f'src/results/comparison_V{vertices}_E{edges}.png')
        plt.close()

        # Print statistics for this level
        print(f"\nStatistics for Graph Size: {vertices} vertices, {edges} edges")
        print("A* Statistics:")
        print(level_data['expanded_nodes_astar'].describe())
        print("\nUCS Statistics:")
        print(level_data['expanded_nodes_ucs'].describe())
        print(f"\nAverage improvement: {improvement.mean():.2f} nodes")
        print(f"Average improvement percentage: {improvement_percentage.mean():.2f}%")
        print("-" * 50)

    # Create a summary plot for all levels
    plt.figure(figsize=(12, 6))
    summary_data = df.groupby('vertices').agg({
        'expanded_nodes_astar': ['mean', 'std'],
        'expanded_nodes_ucs': ['mean', 'std']
    }).reset_index()

    plt.errorbar(summary_data['vertices'], 
                summary_data['expanded_nodes_astar']['mean'], 
                yerr=summary_data['expanded_nodes_astar']['std'], 
                label='A*', marker='o')
    plt.errorbar(summary_data['vertices'], 
                summary_data['expanded_nodes_ucs']['mean'], 
                yerr=summary_data['expanded_nodes_ucs']['std'], 
                label='UCS', marker='o')
    plt.xlabel('Number of Vertices')
    plt.ylabel('Average Expanded Nodes')
    plt.title('Algorithm Performance Across Graph Sizes')
    plt.legend()
    plt.grid(True)
    plt.savefig('src/results/overall_comparison.png')
    plt.close()

    print("\nPlots have been saved in the src/results directory!")

except Exception as e:
    print(f"An error occurred: {str(e)}")
    print("Please make sure the CSV file exists and contains the correct data.")