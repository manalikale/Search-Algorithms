#pragma once

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

/**
 * @class graph.
 * @brief Represents undirected graph, with valued edges.
 */
class graph
{
public:
    void add_edge(const std::string& v1, const std::string& v2, double c, bool r);

    bool has_vertex(const std::string& v) const
    {
        return m_indices.find(v) != m_indices.end();
    }

    /// @brief Breadth First Search algorithm on graph.
    void bfs(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const;

    /// @brief Depth First Search algorithm on graph.
    void dfs(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const;

    /// @brief Uniform Cost Search algorithm on graph with reliable edges.
    void ucsr(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const;

    /// @brief Uniform Cost Search algorithm on graph with unreliable edges.
    void ucsu(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const;

    graph()
    {
    }

    ~graph();

private:
    void add_vertex(const std::string& v);
    unsigned get_index(const std::string& v) const;
    double cost(const std::vector<unsigned>& p) const;
    double reliable_cost(unsigned i1, unsigned i2) const;
    double unreliable_cost(unsigned i1, unsigned i2) const;
    void dfs(unsigned i1, unsigned i2, std::ofstream& fl, const std::vector<unsigned>& p, std::vector<unsigned>& fp) const;
    void ucs(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl, double (graph::* cost_f) (unsigned, unsigned) const) const;

private:
    graph(const graph&);
    graph& operator=(const graph&);

private:
    typedef std::map<std::string, unsigned> name_to_index;
    typedef std::vector<std::string> index_to_name;
    name_to_index m_indices;
    index_to_name m_names;

    std::vector<std::vector<unsigned> > m_adjacent_vertices;
    std::map<std::pair<unsigned, unsigned>, std::pair<double, bool> > m_edges;
};
