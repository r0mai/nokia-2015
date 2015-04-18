
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

struct vertex_property {};

struct edge_property {
    unsigned speed = 100;
    bool canHaveOptic = false;
};

struct vertex_property_tag {
    typedef boost::vertex_property_tag kind;
};

struct edge_property_tag {
    typedef boost::edge_property_tag kind;
};

typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    boost::property<vertex_property_tag, vertex_property>,
    boost::property<edge_property_tag, edge_property>> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

auto& getEdgeProperty(Graph& g, edge_descriptor e) {
    return boost::get(edge_property_tag(), g, e);
}

void addEdgeBetween(Graph& g, unsigned x, unsigned y) {
    auto e1 = boost::add_edge(x-1, y-1, g).first;
    auto e2 = boost::add_edge(y-1, x-1, g).first;
    if (x == 20 || y == 20) {
        // Coming from ISP
        getEdgeProperty(g, e1).speed = 10000;
        getEdgeProperty(g, e2).speed = 10000;
    }
    auto isOpticDevice = [](unsigned n) {
        return n == 11 || n == 12 || n == 16 || n == 17 || n == 18 || n == 19;
    };
    if (isOpticDevice(x) && isOpticDevice(y)) {
        getEdgeProperty(g, e1).canHaveOptic = true;
        getEdgeProperty(g, e2).canHaveOptic = true;
    }
}

inline
Graph createGraph() {
    Graph g;
    for (unsigned i = 0; i < 20; ++i) {
        boost::add_vertex(g);
    }

    addEdgeBetween(g, 1, 5);
    addEdgeBetween(g, 1, 16);
    addEdgeBetween(g, 2, 3);
    addEdgeBetween(g, 2, 16);
    addEdgeBetween(g, 3, 16);
    addEdgeBetween(g, 3, 17);
    addEdgeBetween(g, 4, 17);
    addEdgeBetween(g, 5, 10);
    addEdgeBetween(g, 5, 17);
    addEdgeBetween(g, 6, 7);
    addEdgeBetween(g, 6, 18);
    addEdgeBetween(g, 7, 18);
    addEdgeBetween(g, 7, 19);
    addEdgeBetween(g, 8, 18);
    addEdgeBetween(g, 9, 18);
    addEdgeBetween(g, 9, 19);
    addEdgeBetween(g, 10, 19);
    addEdgeBetween(g, 11, 16);
    addEdgeBetween(g, 11, 18);
    addEdgeBetween(g, 11, 20);
    addEdgeBetween(g, 12, 16);
    addEdgeBetween(g, 12, 18);
    addEdgeBetween(g, 12, 20);
    addEdgeBetween(g, 13, 14);
    addEdgeBetween(g, 13, 15);
    addEdgeBetween(g, 13, 16);
    addEdgeBetween(g, 14, 15);
    addEdgeBetween(g, 14, 16);
    addEdgeBetween(g, 15, 18);
    addEdgeBetween(g, 16, 17);
    addEdgeBetween(g, 16, 18);
    addEdgeBetween(g, 17, 19);
    addEdgeBetween(g, 18, 19);

    return g;
}

