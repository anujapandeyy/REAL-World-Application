#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_CITIES 100
#define MAX_EDGES 10 

typedef struct {
    int city;
    int distance;
} Edge;

typedef struct {
    Edge* edges[MAX_CITIES][MAX_EDGES];  
    int edge_count[MAX_CITIES];          
} Graph;

void initializeGraph(Graph* graph, int num_cities) {
    for (int i = 0; i < num_cities; i++) {
        graph->edge_count[i] = 0;
        for (int j = 0; j < MAX_EDGES; j++) {
            graph->edges[i][j] = NULL;  
        }
    }
}

void addRoad(Graph* graph, int city1, int city2, int distance) {
    if (graph->edge_count[city1] < MAX_EDGES && graph->edge_count[city2] < MAX_EDGES) {
        Edge* edge1 = (Edge*)malloc(sizeof(Edge));
        edge1->city = city2;
        edge1->distance = distance;
        graph->edges[city1][graph->edge_count[city1]++] = edge1;

        Edge* edge2 = (Edge*)malloc(sizeof(Edge));
        edge2->city = city1;
        edge2->distance = distance;
        graph->edges[city2][graph->edge_count[city2]++] = edge2;
    } else {
        printf("Maximum edges reached for city %d or city %d.\n", city1, city2);
    }
}


bool dfs(Graph* graph, int start, int goal, bool visited[]) {
    if (start == goal) return true;

    visited[start] = true;

    for (int i = 0; i < graph->edge_count[start]; i++) {
        int neighbor = graph->edges[start][i]->city;
        if (!visited[neighbor] && dfs(graph, neighbor, goal, visited)) {
            return true;
        }
    }
    return false;
}


int dijkstra(Graph* graph, int start, int goal, int num_cities) {
    int distances[num_cities];
    bool visited[num_cities];

    for (int i = 0; i < num_cities; i++) {
        distances[i] = INT_MAX;
        visited[i] = false;
    }
    distances[start] = 0;

    for (int count = 0; count < num_cities - 1; count++) {
        int min_distance = INT_MAX, min_index;

        for (int v = 0; v < num_cities; v++) {
            if (!visited[v] && distances[v] <= min_distance) {
                min_distance = distances[v];
                min_index = v;
            }
        }

        int u = min_index;
        visited[u] = true;

        for (int i = 0; i < graph->edge_count[u]; i++) {
            int v = graph->edges[u][i]->city;
            int weight = graph->edges[u][i]->distance;
            if (!visited[v] && distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }
    return distances[goal];
}

int main() {
    Graph graph;
    int num_cities = 5;
    initializeGraph(&graph, num_cities);

    
    addRoad(&graph, 0, 1, 5);
    addRoad(&graph, 0, 2, 10);
    addRoad(&graph, 1, 3, 15);
    addRoad(&graph, 2, 3, 20);
    addRoad(&graph, 3, 4, 10);

    int start_city = 0;
    int end_city = 4;

   
    bool visited[MAX_CITIES] = {false};
    if (dfs(&graph, start_city, end_city, visited)) {
        printf("There is a route between city %d and city %d.\n", start_city, end_city);
    } else {
        printf("There is no route between city %d and city %d.\n", start_city, end_city);
    }

    int shortest_distance = dijkstra(&graph, start_city, end_city, num_cities);
    if (shortest_distance != INT_MAX) {
        printf("The shortest distance from city %d to city %d is %d.\n", start_city, end_city, shortest_distance);
    } else {
        printf("There is no path from city %d to city %d.\n", start_city, end_city);
    }

    return 0;
}
