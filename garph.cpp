#include "graph.hpp"

#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

void graph::add_vertex(const std::string& v)
{
    assert(!has_vertex(v));
    unsigned i = m_names.size();
    assert(i == m_indices.size());
    m_names.push_back(v);
    m_indices[v] = i;
    m_adjacent_vertices.push_back(std::vector<unsigned>());
}

unsigned graph::get_index(const std::string& v) const
{
    name_to_index::const_iterator i = m_indices.find(v);
    assert(i != m_indices.end());
    return i->second;
}

void graph::add_edge(const std::string& v1, const std::string& v2, double c, bool r)
{
    if (!has_vertex(v1)) {
        add_vertex(v1);
    }
    if (!has_vertex(v2)) {
        add_vertex(v2);
    }
    unsigned i1 = get_index(v1);
    unsigned i2 = get_index(v2);
    if (i2 < i1) {
        std::swap(i1, i2);
    }
    m_adjacent_vertices[i1].push_back(i2);
    m_adjacent_vertices[i2].push_back(i1);
    m_edges[std::make_pair(i1, i2)] = std::make_pair(c, r);
}

double graph::reliable_cost(unsigned v1, unsigned v2) const
{
    if (v1 > v2) {
        std::swap(v1, v2);
    }
    std::map<std::pair<unsigned, unsigned>, std::pair<double, bool> >::const_iterator x = 
        m_edges.find(std::make_pair(v1, v2));
    assert(x != m_edges.end());
    return x->second.first;
}

double graph::unreliable_cost(unsigned v1, unsigned v2) const
{
    if (v1 > v2) {
        std::swap(v1, v2);
    }
    std::map<std::pair<unsigned, unsigned>, std::pair<double, bool> >::const_iterator x = 
        m_edges.find(std::make_pair(v1, v2));
    assert(x != m_edges.end());
    return x->second.second ? x->second.first : x->second.first + 0.5;
}

double graph::cost(const std::vector<unsigned>& path) const
{
    double c = 0;
    for (unsigned i = 1; i < path.size(); ++i) {
        c += reliable_cost(path[i - 1], path[i]);
    }
    return c;
}

void graph::bfs(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const
{
    unsigned i1 = get_index(v1);
    unsigned i2 = get_index(v2);
    std::queue<std::pair<unsigned, std::vector<unsigned> > > q;
    q.push(std::make_pair(i1, std::vector<unsigned>(1, i1)));
    std::vector<unsigned> found_path;
    while (!q.empty()) {
        i1 = q.front().first;
        std::vector<unsigned> path = q.front().second;
        fl << m_names[i1] << "\n";
        q.pop();
        if(i2 == i1) {
            if (found_path.empty() || cost(found_path) > cost(path)) {
                found_path = path;
            }
            continue;
        }
        std::vector<unsigned>::const_iterator i;
        for (i = m_adjacent_vertices[i1].begin(); 
             i != m_adjacent_vertices[i1].end();
             ++i) {
            if(std::find(path.begin(), path.end(), *i) == path.end()) {
                std::vector<unsigned> np(path);
                np.push_back(*i);
                q.push(std::make_pair(*i, np));
            }
        }
    }
    for (unsigned j = 0; j < found_path.size(); ++j) {
        fp << m_names[found_path[j]] << "\n";
    }
}

void graph::dfs(unsigned i1, unsigned i2, std::ofstream& fl, const std::vector<unsigned>& path, std::vector<unsigned>& found_path) const
{
    fl << m_names[i1] << "\n";
    if (i1 == i2) {
        if (found_path.empty() || cost(found_path) > cost(path)) {
            found_path = path;
        }
        return;
    }
    std::vector<unsigned>::const_reverse_iterator i;
    for (i = m_adjacent_vertices[i1].rbegin(); i != m_adjacent_vertices[i1].rend(); ++i) {
        if(std::find(path.begin(), path.end(), *i) == path.end()) {
            std::vector<unsigned> np(path);
            np.push_back(*i);
            dfs(*i, i2, fl, np, found_path);
        }
    }
}

void graph::dfs(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const
{
    std::vector<unsigned> path;
    std::vector<unsigned> found_path;
    unsigned i1 = get_index(v1);
    unsigned i2 = get_index(v2);
    path.push_back(i1);
    dfs(i1, i2, fl, path, found_path);
    for (unsigned j = 0; j < found_path.size(); ++j) {
        fp << m_names[found_path[j]] << "\n";
    }
}

namespace {

struct ucs_node
{
    ucs_node(unsigned v, const std::vector<unsigned>& p, double c)
        : m_vertex(v)
        , m_path(p)
        , m_cost(c)
    {
    }

    unsigned m_vertex;
    std::vector<unsigned> m_path;
    double m_cost;
};

struct cost_compare
{
    bool operator()(const ucs_node& r, const ucs_node& l)
    {
        return r.m_cost < l.m_cost;
    }
};

typedef std::multiset<ucs_node, cost_compare> ucs_node_queue;

ucs_node_queue::iterator find_in_queue(ucs_node_queue& q, unsigned v)
{
    ucs_node_queue::iterator i = q.begin();
    while (i != q.end()) {
        if (i->m_vertex == v) {
            break;
        }
        ++i;
    }
    return i;
}

}

void graph::ucsr(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const
{
    ucs(v1, v2, fp, fl, &graph::reliable_cost);
}

void graph::ucsu(const std::string& v1, const std::string& v2, std::ofstream& fp, std::ofstream& fl) const
{
    ucs(v1, v2, fp, fl, &graph::unreliable_cost);
}

void graph::ucs(const std::string& v1, const std::string& v2, 
                std::ofstream& fp, std::ofstream& fl,
                double (graph::* cost_f) (unsigned, unsigned) const) const
{
    unsigned i1 = get_index(v1);
    unsigned i2 = get_index(v2);
    ucs_node_queue q;
    std::set<unsigned> e;
    std::vector<unsigned> t(1, i1);
    q.insert(ucs_node(i1, t, 0));
    while (!q.empty()) {
        ucs_node n = *q.begin();
        fl << m_names[n.m_vertex] << "\n";
        q.erase(q.begin());
        if (n.m_vertex == i2) {
            for (unsigned j = 0; j < n.m_path.size(); ++j) {
                fp << m_names[n.m_path[j]] << "\n";
            }
            return;
        }
        e.insert(n.m_vertex);
        std::vector<unsigned>::const_iterator i;
        for (i = m_adjacent_vertices[n.m_vertex].begin(); i != m_adjacent_vertices[n.m_vertex].end(); ++i) {
            if (e.find(*i) == e.end()) {
                ucs_node_queue::iterator qi = find_in_queue(q, *i);
                if (qi == q.end()) {
                    std::vector<unsigned> np(n.m_path);
                    np.push_back(*i);
                    q.insert(ucs_node(*i, np, n.m_cost + (this->*cost_f)(n.m_vertex, *i)));
                } else {
                    if (n.m_cost + (this->*cost_f)(n.m_vertex, *i) < qi->m_cost) {
                        q.erase(qi);
                        std::vector<unsigned> np(n.m_path);
                        np.push_back(*i);
                        q.insert(ucs_node(*i, np, n.m_cost + (this->*cost_f)(n.m_vertex, *i)));
                    }
                }
            }
        }
    }
}
