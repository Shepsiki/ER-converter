#pragma once

#include <vector>
#include <map>
#include <set>

#include "base/Errors.h"

namespace erconv {

template <class V = size_t, class E = size_t>
class DirectedGraph {
public:
    using Edge = std::pair<V, V>;
    using EdgeDirection = std::pair<V, E>;

    DirectedGraph() = default;

    DirectedGraph& operator=(const DirectedGraph& gr) {
        this->edges = gr.GetEdges();
        this->vertices = gr.GetVertices();
        this->edgeList = gr.GetEdgeList();
        this->edgeListReversed = gr.GetEdgeListReversed();

        return *this;
    }

    bool AddVertex(const V& vertex) {
        if (vertices.count(vertex) != 0) {
            return false;
        }
        vertices.insert(vertex);
        edgeList[vertex];
        edgeListReversed[vertex];
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

    bool HasEdge(const E& edge) const {
        if (edges.count(edge) == 0) {
            return false;
        }
        return true;
    }

    bool HasVertex(const V& vertex) const {
        if (vertices.count(vertex) == 0) {
            return false;
        }
        return true;
    }

    const std::set<V>& GetVertices() const {
        return vertices;
    }

    const std::map<E, Edge>& GetEdges() const {
        return edges;
    }

    const std::map<V, std::set<EdgeDirection>>& GetEdgeList() const {
        return edgeList;
    }

    const std::map<V, std::set<EdgeDirection>>& GetEdgeListReversed() const {
        return edgeListReversed;
    }

    Edge GetEdge(const E& edge) const {
        return edges[edge];
    }

    std::vector<E> GetEdgesFromCurrent(const V& current) const {
        existenceCheckV(current);
        std::vector<V> result;
        for (const auto& p : edgeList.at(current)) {
            result.push_back(p.second);
        }
        return result;
    }

    std::vector<E> GetEdgesToCurrent(const V& current) const {
        existenceCheckV(current);
        std::vector<V> result;
        for (const auto& p : edgeListReversed.at(current)) {
            result.push_back(p.second);
        }
        return result;
    }

    std::set<V> GetVerticesReachableFromCurrent(const V& current) const {
        existenceCheckV(current);
        std::set<V> result;
        for (const auto& p : edgeList.at(current)) {
            result.insert(p.first);
        }
        return result;
    } 

    std::set<V> GetVerticesCurrentReachableFrom(const V& current) const {
        existenceCheckV(current);
        std::set<V> result;
        for (const auto& p : edgeListReversed.at(current)) {
            result.insert(p.first);
        }
        return result;
    }

    // !!! Loop is not a cycle !!!
    bool HasCycle() const {
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

    std::vector<V> GetVerticesInTopologicalOrder() const {
        if (HasCycle()) {
            throw TError("Graph has cycles!");
        }
        std::vector<V> sources;
        for (const auto& v : vertices) {
            if (edgeListReversed.at(v).size() == 0) {
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

    std::set<V> vertices;
    std::map<E, Edge> edges;
    std::map<V, std::set<EdgeDirection>> edgeList; 
    std::map<V, std::set<EdgeDirection>> edgeListReversed;

    bool internalRecursiveDFSForCycleDetection(
        std::map<V, int>& indicator,
        const V& current
    ) const {
        indicator[current] = 1;
        for (const auto& dir : edgeList.at(current)) {
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
    ) const {
        indicator[current] = 1;
        for (const auto& dir : edgeList.at(current)) {
            V to = dir.first;
            if (indicator[to] == 0) {
                internalRecursiveDFSForTopologicalSort(indicator, result, to);
            }
        }
        result.push_back(current);
        return false;
    }

    void existenceCheckV(const V& v) const {
        if (vertices.count(v) == 0) {
            throw TError("Graph: Vertex with id " + std::to_string(v) + " doesn't exist in graph.");
        }
    }

    void existanceCheckE(const E& e) const {
        if (edges.count(e) == 0) {
            throw TError("Graph: Edge with id " + std::to_string(e) + " doesn't exist in graph.");
        }
    }
};

}
