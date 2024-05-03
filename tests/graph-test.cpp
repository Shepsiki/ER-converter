#include <gtest/gtest.h>

#include "base/Graph.h"
#include "base/IDManager.h"

using namespace erconv;

TEST(GraphTest, BasicTest) {
    DirectedGraph g;
    IDManager man;

    for (size_t i = 1; i <= 8; i++) {
        g.AddVertex(i);
    }

    g.AddEdge(1, 2, man.NewID());
    g.AddEdge(1, 3, man.NewID());
    g.AddEdge(1, 4, man.NewID());
    g.AddEdge(4, 5, man.NewID());
    g.AddEdge(3, 5, man.NewID());
    g.AddEdge(3, 6, man.NewID());
    g.AddEdge(5, 7, man.NewID());
    g.AddEdge(7, 8, man.NewID());

    // Cycle checking:
    ASSERT_FALSE(g.HasCycle());

    g.AddEdge(8, 5, man.NewID());

    ASSERT_TRUE(g.HasCycle());

    g.RemoveVertex(8);

    ASSERT_FALSE(g.HasCycle());

    // Topological order:
    std::vector<size_t> vs = g.GetVerticesInTopologicalOrder();
    std::set<size_t> was;
    for (const auto& v : vs) {
        std::set<size_t> reachable = g.GetVerticesReachableFromCurrent(v);
        for (const auto& i : reachable) {
            ASSERT_TRUE(was.count(i) != 0);
        }
        was.insert(v);
    }

}
