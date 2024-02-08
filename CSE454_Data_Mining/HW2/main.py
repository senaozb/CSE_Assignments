import numpy as np
import matplotlib.pyplot as plt
import random

# Function to compute Euclidean distance between two points
def euclidean_distance(point1, point2):
    return np.sqrt(np.sum((point1 - point2)**2))

# Function to compute Manhattan distance between two points
def manhattan_distance(point1, point2):
    return np.sum(np.abs(point1 - point2))


# Function to find k-nearest neighbors for a given point
def knn(point, data, k, method):
    if method == "euclidean":
        distances = [euclidean_distance(point, other_point) for other_point in data]
    else:
        distances = [manhattan_distance(point, other_point) for other_point in data]
    indices = np.argsort(distances)[1:k+1]  
    return indices


# Function to perform spectral partitioning and return cluster labels
def spectral_partition(graph, num_clusters):
    adjacency_matrix = np.zeros((len(graph), len(graph)))

    for i, neighbors in graph.items():
        for j in neighbors:
            adjacency_matrix[i, j] = 1
            adjacency_matrix[j, i] = 1

    laplacian_matrix = np.diag(np.sum(adjacency_matrix, axis=1)) - adjacency_matrix

    # Calculate the eigenvectors of the Laplacian matrix
    _, eigenvectors = np.linalg.eigh(laplacian_matrix)

    # Select the first k eigenvectors
    selected_eigenvectors = eigenvectors[:, :num_clusters]

    # Apply k-means clustering to the rows of the selected eigenvectors
    labels = k_means_clustering(selected_eigenvectors, num_clusters)

    return labels

# Function for k-means clustering
def k_means_clustering(data, k, max_iterations=100):
    centroids = data[np.random.choice(len(data), k, replace=False)]

    for _ in range(max_iterations):
        distances = np.linalg.norm(data[:, np.newaxis] - centroids, axis=2)
        labels = np.argmin(distances, axis=1)

        new_centroids = np.array([np.mean(data[labels == j], axis=0) for j in range(k)])

        if np.all(centroids == new_centroids):
            break

        centroids = new_centroids

    return labels

# Function for agglomerative hierarchical clustering
def agglomerative_hierarchical_clustering(num_clusters_init, labels, points, num_clusters, method):
    # Initialize each data point as a separate cluster
    clusters = [[] for i in range(num_clusters_init)]
    
    for i, point in enumerate(points):
        clusters[labels[i]].append(i)
    
    while len(clusters) > num_clusters:
        min_distance = float('inf')
        merge_indices = None

        # Find the closest pair of clusters with complete linkage
        for i in range(len(clusters)):
            for j in range(i + 1, len(clusters)):
                if method == "euclidean":
                    distance = max([euclidean_distance(points[node_i], points[node_j]) 
                                    for node_i in clusters[i] for node_j in clusters[j]])
                else:
                    distance = max([manhattan_distance(points[node_i], points[node_j]) 
                                    for node_i in clusters[i] for node_j in clusters[j]])
                if distance < min_distance:
                    min_distance = distance
                    merge_indices = (i, j)

        clusters[merge_indices[0]].extend(clusters[merge_indices[1]])  
        del clusters[merge_indices[1]]

    # Assign cluster labels
    cluster_labels = np.zeros(len(points), dtype=int)
    for i, cluster in enumerate(clusters):
        cluster_labels[cluster] = i

    return cluster_labels


# Read the data 
with open("worms_2d.txt", "r") as f:
    data = f.readlines()
   
# Prepare the data 
data_array = []
for line in data:
    x = [float(item) for item in line.split()]
    data_array.append(x)
    
# Use the first 100 data entries
data_points = np.array(data_array[:1000])


# k for KNN
k = random.randint(5, 10)
method = "euclidean"

# Create a graph
G = {i: set() for i in range(len(data_points))}

# Add edges to the graph based on k-nearest neighbors
for i, point in enumerate(data_points):
    neighbors = knn(point, data_points, k, method)
    G[i].update(neighbors)
    

# Apply spectral partitioning to the graph
num_clusters_spectral = random.randint(5, 10) 
cluster_labels_spectral = spectral_partition(G, num_clusters_spectral)


# Apply agglomerative hierarchical clustering to merge clusters
num_clusters_final = random.randint(1, num_clusters_spectral)
cluster_labels_final = agglomerative_hierarchical_clustering(num_clusters_spectral, cluster_labels_spectral, data_points, num_clusters_final, method)


print(f"k:{k}, spectral cluster number:{num_clusters_spectral}, merging cluster number:{num_clusters_final}")


# Visualize KNN result
for i, neighbors in G.items():
    for j in neighbors:
        plt.plot(
            [data_points[i, 0], data_points[j, 0]],
            [data_points[i, 1], data_points[j, 1]],
            'k-', alpha=0.2
        )

plt.scatter(data_points[:, 0], data_points[:, 1], label='KNN Result', alpha=0.8)

plt.title("KNN Result")
plt.legend()
plt.show()

# Visualize clusters after spectral partitioning
for i, neighbors in G.items():
    for j in neighbors:
        if cluster_labels_spectral[i] == cluster_labels_spectral[j]:
            plt.plot(
                [data_points[i, 0], data_points[j, 0]],
                [data_points[i, 1], data_points[j, 1]],
                'k-', alpha=0.5
            )

for label in np.unique(cluster_labels_spectral):
    indices = np.where(cluster_labels_spectral == label)[0]
    plt.scatter(
        data_points[indices, 0],
        data_points[indices, 1],
        label=f'Spectral Cluster {label}'
    )

plt.legend()
plt.title("Clusters after Spectral Partitioning")
plt.show()

# Visualize clusters after agglomerative hierarchical clustering
for i, neighbors in G.items():
    for j in neighbors:
        if cluster_labels_final[i] == cluster_labels_final[j]:
            plt.plot(
                [data_points[i, 0], data_points[j, 0]],
                [data_points[i, 1], data_points[j, 1]],
                'k-', alpha=0.5
            )

for label in np.unique(cluster_labels_final):
    indices = np.where(cluster_labels_final == label)[0]
    plt.scatter(
        data_points[indices, 0],
        data_points[indices, 1],
        label=f'Final Cluster {label}'
    )

plt.legend()
plt.title("Clusters after Agglomerative Hierarchical Clustering")
plt.show()

