#include "cli/ConsoleMessage.h"

#include "base/Graph.h"

using namespace erconv;

int main() {
    HelloConsoleMessage();

    DirectedGraph g;
    g.AddVertex(1);
    g.AddVertex(2);
    g.AddVertex(3);
    g.AddVertex(4);
    g.AddVertex(5);
    g.AddVertex(6);
    g.AddVertex(7);
    g.AddVertex(8);
    g.AddVertex(9);
    g.AddVertex(10);

    g.AddEdge(3, 5, 4);
    g.AddEdge(3, 10, 5);
    g.AddEdge(10, 10, 6);
    g.AddEdge(10, 10, 7);
    g.AddEdge(10, 8, 8);
    g.AddEdge(8, 7, 9);
    g.AddEdge(8, 7, 10);
    g.AddEdge(8, 7, 11);
    g.AddEdge(8, 9, 12);
    g.AddEdge(5, 9, 13);
    g.AddEdge(5, 6, 14);
    g.AddEdge(6, 7, 15);

    g.AddEdge(1, 2, 0);
    g.AddEdge(1, 4, 1);
    g.AddEdge(2, 3, 2);
    g.AddEdge(2, 4, 3);

    g.AddVertex(174);

    g.AddVertex(15);
    g.AddVertex(16);
    g.AddEdge(15, 16, 32);

    auto a = g.GetVerticesInTopologicalOrder();
    for (auto i : a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    g.__protoprint();
    g.RemoveVertex(10);
    g.__protoprint();
    a = g.GetVerticesInTopologicalOrder();
    for (auto i : a) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
