#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include "TCanvas.h"
#include "TGraph.h"
#include "TArrow.h"
#include "TLatex.h"

using namespace std;

const int INF = numeric_limits<int>::max();

// Estructura para representar una arista
struct Edge {
    int to;
    int weight;
};

// Estructura para representar un nodo en la cola de prioridad
struct Node {
    int vertex;
    int distance;
    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

// Función para el algoritmo de Dijkstra
vector<int> dijkstra(int source, const vector<vector<Edge>>& graph, vector<int>& dist, vector<int>& prev) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({source, 0});
    dist[source] = 0;

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();

        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({v, dist[v]});
            }
        }
    }

    return dist;
}

// Función para reconstruir el camino mínimo desde el nodo fuente al nodo destino
vector<int> reconstruct_path(int source, int target, const vector<int>& prev) {
    vector<int> path;
    for (int at = target; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    if (path[0] == source) {
        return path;
    }
    return {};
}

// Función para dibujar el grafo completo
void drawGraph(const vector<vector<Edge>>& graph, const vector<pair<double, double>>& positions) {
    TCanvas *canvas = new TCanvas("canvas1", "Grafo Completo", 800, 600);
    TGraph *graphPoints = new TGraph();

    // Dibujar nodos y etiquetas de los nodos
    for (size_t i = 0; i < positions.size(); ++i) {
        graphPoints->SetPoint(i, positions[i].first, positions[i].second);
        TLatex *label = new TLatex(positions[i].first, positions[i].second, Form("%d", (int)i));
        label->SetTextSize(0.03);
        label->SetTextAlign(22); // Centrar el texto
        label->Draw();
    }

    graphPoints->SetMarkerStyle(20);
    graphPoints->SetMarkerSize(1);
    graphPoints->Draw("AP");

    // Dibujar aristas dirigidas y mostrar distancias
    for (size_t u = 0; u < graph.size(); ++u) {
        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            TArrow *arrow = new TArrow(positions[u].first, positions[u].second, positions[v].first, positions[v].second, 0.02, "|>");
            arrow->SetLineColor(kBlack);
            arrow->Draw();

            // Calcular la posición del texto
            double midX = (positions[u].first + positions[v].first) / 2;
            double midY = (positions[u].second + positions[v].second) / 2;
            TLatex *weightLabel = new TLatex(midX, midY, Form("%d", edge.weight));
            weightLabel->SetTextSize(0.03);
            weightLabel->Draw();
        }
    }

    canvas->Update();
}

// Función para dibujar el grafo y la ruta solución
void drawSolutionGraph(const vector<vector<Edge>>& graph, const vector<pair<double, double>>& positions, const vector<int>& path, const vector<int>& dist, int target) {
    TCanvas *canvas = new TCanvas("canvas2", "Ruta Optima algoritmo de Dijkstra", 800, 600);
    TGraph *graphPoints = new TGraph();

    // Dibujar nodos y etiquetas de los nodos
    for (size_t i = 0; i < positions.size(); ++i) {
        graphPoints->SetPoint(i, positions[i].first, positions[i].second);
        TLatex *label = new TLatex(positions[i].first, positions[i].second, Form("%d", (int)i));
        label->SetTextSize(0.03);
        label->SetTextAlign(22); // Centrar el texto
        label->Draw();
    }

    graphPoints->SetMarkerStyle(20);
    graphPoints->SetMarkerSize(1);
    graphPoints->Draw("AP");

    // Dibujar aristas dirigidas y mostrar distancias
    for (size_t u = 0; u < graph.size(); ++u) {
        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            TArrow *arrow = new TArrow(positions[u].first, positions[u].second, positions[v].first, positions[v].second, 0.02, "|>");
            arrow->SetLineColor(kBlack);
            arrow->Draw();

            // Calcular la posición del texto
            double midX = (positions[u].first + positions[v].first) / 2;
            double midY = (positions[u].second + positions[v].second) / 2;
            TLatex *weightLabel = new TLatex(midX, midY, Form("%d", edge.weight));
            weightLabel->SetTextSize(0.03);
            weightLabel->Draw();
        }
    }

    // Dibujar ruta óptima
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        TArrow *arrow = new TArrow(positions[u].first, positions[u].second, positions[v].first, positions[v].second, 0.02, "|>");
        arrow->SetLineColor(kRed);
        arrow->SetLineWidth(2);
        arrow->Draw();

        // Calcular la posición del texto para la ruta óptima
        double midX = (positions[u].first + positions[v].first) / 2;
        double midY = (positions[u].second + positions[v].second) / 2;
        TLatex *weightLabel = new TLatex(midX, midY, Form("%d", graph[u][0].weight));
        weightLabel->SetTextSize(0.03);
        weightLabel->SetTextColor(kRed);
        weightLabel->Draw();
    }

    // Mostrar la distancia total en el plot
    TLatex *distanceLabel = new TLatex(0.1, 0.95, Form("Distancia total: %d", dist[target]));
    distanceLabel->SetTextSize(0.04);
    distanceLabel->SetTextColor(kBlue);
    distanceLabel->Draw();

    canvas->Update();
}

int main() {
    // Grafo de ejemplo
    vector<vector<Edge>> graph = {
        {{1, 4}, {2, 1}},
        {{2, 2}, {3, 5}},
        {{1, 2}, {3, 1}},
        {}
    };

    // Posiciones de los nodos para la visualización
    vector<pair<double, double>> positions = {
        {0.1, 0.9},
        {0.5, 0.9},
        {0.3, 0.5},
        {0.7, 0.1}
    };

    int n = graph.size();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);

    // Nodo fuente y destino
    int source = 0;
    int target = 3;

    // Ejecutar Dijkstra
    dijkstra(source, graph, dist, prev);

    // Reconstruir el camino
    vector<int> path = reconstruct_path(source, target, prev);

    // Imprimir la distancia mínima y el camino
    cout << "Distancia mínima desde el nodo " << source << " al nodo " << target << ": " << dist[target] << endl;
    cout << "Camino: ";
    for (int node : path) {
        cout << node << " ";
    }
    cout << endl;

    // Dibujar el grafo completo
    drawGraph(graph, positions);

    // Dibujar el grafo con la ruta solución
    drawSolutionGraph(graph, positions, path, dist, target);

    return 0;
}

