#pragma once

#include <vector>
#include <map>
#include <set>

#include "base/Errors.h"

namespace erconv {

template <class V = size_t, class E = size_t>
class DirectedGraph {
public:
    DirectedGraph() = default;

    void __protoprint() {
        std::cout << "V:\n";
        for (const auto i : vertices) {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        std::cout << "E:\n";
        for (const auto i : edges) {
            std::cout << i.first << " ";
        }
        std::cout << std::endl;

        std::cout << "EL:\n";
        for (const auto i : edgeList) {
            std::cout << i.first <<  ": ";
            for (const auto p : i.second) {
                std::cout << "(" << p.first << " " << p.second << ") ";
            }
        }
        std::cout << std::endl;

        std::cout << "ELR:\n";
        for (const auto i : edgeListReversed) {
            std::cout << i.first <<  ": ";
            for (const auto p : i.second) {
                std::cout << "(" << p.first << " " << p.second << ") ";
            }
        }
        std::cout << std::endl;
    }

    bool AddVertex(const V& vertex) {
        if (vertices.count(vertex) != 0) {
            return false;
        }
        vertices.insert(vertex);
        return true;
    }

    bool AddEdge(const V& from, const V& to, const E& edge) {
        if (vertices.count(from) == 0 || vertices.count(to) == 0) {
            return false;
        }
        if (edges.count(edge) != 0) {
            return false;
        }
        edges[edge] = Edge(from, to);
        edgeList[from].insert(EdgeDirection(to, edge));
        edgeListReversed[to].insert(EdgeDirection(from, edge));
        return true;
    }

    void RemoveEdge(const E& edge) {
        if (edges.count(edge) != 0) {
            Edge cur = edges[edge];
            edgeList[cur.first].erase(EdgeDirection(cur.second, edge));
            edgeListReversed[cur.second].erase(EdgeDirection(cur.first, edge));
            edges.erase(edge);
        }
    }

    void RemoveVertex(const V& vertex) {
        if (vertices.count(vertex) != 0) {
            std::vector<E> edgesToDelete;
            for (const EdgeDirection& dir : edgeList[vertex]) {
                edgesToDelete.push_back(dir.second);
            }
            for (const EdgeDirection& dir : edgeListReversed[vertex]) {
                edgesToDelete.push_back(dir.second);
            }
            for (const auto& e : edgesToDelete) {
                RemoveEdge(e);
            }
            vertices.erase(vertex);
        }
    }

    // !!! Loop is not a cycle !!!
    bool HasCycle() {
        std::map<V, int> indicator;
        for (const auto& v : vertices) {
            indicator[v] = 0;
        }
        for (const auto& v : vertices) {
            if (indicator[v] == 0) {
                if (internalRecursiveDFSForCycleDetection(indicator, v)) {
                    return true;
                }
            } 
        }
        return false;
    }

    std::vector<V> GetVerticesInTopologicalOrder() {
        if (HasCycle()) {
            throw TError("Graph has cycles!");
        }
        std::vector<V> sources;
        for (const auto& v : vertices) {
            if (edgeListReversed[v].size() == 0) {
                sources.push_back(v);
            }
        }
        std::vector<V> result;
        std::map<V, int> indicator;
        for (const auto& src : sources) {
            internalRecursiveDFSForTopologicalSort(indicator, result, src);
        }
        return result;
    }

private:
    using EdgeDirection = std::pair<V, E>;
    using Edge = std::pair<V, V>;

    std::set<V> vertices;
    std::map<E, Edge> edges;
    std::map<V, std::set<EdgeDirection>> edgeList; 
    std::map<V, std::set<EdgeDirection>> edgeListReversed;

    bool internalRecursiveDFSForCycleDetection(
        std::map<V, int>& indicator,
        const V& current
    ) {
        indicator[current] = 1;
        for (const auto& dir : edgeList[current]) {
            V to = dir.first;
            if (to == current) {
                continue;
            }
            if (indicator[to] == 0) {
                if (internalRecursiveDFSForCycleDetection(indicator, to)) {
                    return true;
                }
            }
            if (indicator[to] == 1) {
                return true;
            }
            if (indicator[to] == 2) {
                continue;
            }
        }
        indicator[current] = 2;
        return false;
    }

    bool internalRecursiveDFSForTopologicalSort(
        std::map<V, int>& indicator,
        std::vector<V>& result,
        const V& current
    ) {
        indicator[current] = 1;
        for (const auto& dir : edgeList[current]) {
            V to = dir.first;
            if (indicator[to] == 0) {
                internalRecursiveDFSForTopologicalSort(indicator, result, to);
            }
        }
        result.push_back(current);
        return false;
    }
};

}