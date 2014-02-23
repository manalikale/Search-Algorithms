#include "graph.hpp"
#include "reader.hpp"
#include "utility.hpp"

#include <iostream>
#include <map>
#include <string>

class bad_args
{
public:
    bad_args(const std::string& w)
        : m_error(w)
    {
    }

    const std::string& get_error() const
    {
        return m_error;
    }

private:
    std::string m_error;
};

enum task {
    bfs = 1,
    dfs = 2,
    ucsr = 3,
    ucsu = 4
};

void check_argument(const std::map<std::string, std::string>& args, const std::string& arg)
{
    if (args.find(arg) == args.end()) {
        throw bad_args(arg + " argument is not specified");
    }
}

void parse_arguments(char** argv, task& t, std::string& v1, std::string& v2, std::string& in, 
        std::string& outp, std::string& outl)
{
    std::map<std::string, std::string> args;
    for (unsigned i = 0; i < 6; ++i) {
        args[argv[2 * i + 1]] = argv[2 * i + 2];
    }
    check_argument(args, "-t");
    check_argument(args, "-s");
    check_argument(args, "-g");
    check_argument(args, "-i");
    check_argument(args, "-op");
    check_argument(args, "-ol");
    unsigned tv = to_numeric<unsigned>(args["-t"]);
    if (tv > 4 || tv == 0) {
        throw bad_args("Task ID is invalid");
    }
    t = static_cast<task>(tv);
    v1 = args["-s"];
    v2 = args["-g"];
    in = args["-i"];
    outp = args["-op"];
    outl = args["-ol"];
}

int main(int argc, char** argv)
{
    if (argc != 13) {
        std::cout << "Arguments count is invalid" << std::endl;
        return 1;
    }
    task t;
    std::string v1;
    std::string v2;
    std::string in;
    std::string outp;
    std::string outl;
    try {
        parse_arguments(argv, t, v1, v2, in, outp, outl);
    } catch (const bad_args& e) {
        std::cout << e.get_error() << std::endl;
        return 1;
    }
    graph* g = reader::read_graph(in);
    if (!g->has_vertex(v1)) {
        std::cout << "The vertex " << v1 << " does not exist in the graph." << std::endl;
        return 1;
    }
    if (!g->has_vertex(v2)) {
        std::cout << "The vertex " << v2 << " does not exist in the graph." << std::endl;
        return 1;
    }
    std::ofstream fp(outp.c_str());
    std::ofstream fl(outl.c_str());
    switch(t) {
        case bfs :
            g->bfs(v1, v2, fp, fl);
            break;
        case dfs :
            g->dfs(v1, v2, fp, fl);
            break;
        case ucsr :
            g->ucsr(v1, v2, fp, fl);
            break;
        case ucsu :
            g->ucsu(v1, v2, fp, fl);
            break;
    }
    fp.close();
    fl.close();
    return 0;
}
