Search-Algorithms
=================

Implemented a path finding algorithm for the Internet. Specifically, written a  program that will apply Breadth-first search, Depth-first search and Uniform-cost search to find a path between two specific nodes.

Running The Program:
The input to the program needs to be in the form of a valid input.txt file and needs to be stored in the same directory as the rest of the code files and the makefile. The program then saves the 2 output files(output_path and output_log) in the same directory as the input file.

Program Structure:
There are in all 3 header files: reader.hpp, graph.hpp and utility.hpp and their respective code files: reader.cpp, graph.cpp and main.cpp

reader.hpp: The read_graph() is included in the reader namespace

reader.cpp: The read_graph() is used to read the input graph to the algorithms from the input.txt file and the parse_line() is used to parse the input

graph.hpp:Contains all the declarations of graph class public and private methods

graph:cpp:Contains all the graph methods and implementation of the 4 algorithms: DFS, BFS, Uniform Cost Search with reliable and unreliable edges

utility.hpp:Is a template for checking the task number and ensuring that its between 1-4

main.cpp: Implements methods to check the number of arguments, parsing them,prints error messages for bad arguments and invokes the bfs, dfs and ucs(reliable) and usc(unreliable) methods of the graph class

Analysis of the differences between optimal paths found by UCS with reliable edges(Task 3) and UCS with unreliable edges(Task 4):

The difference between the path found by UCS with reliable edges(Task 3)and UCS with unreliable edges(Task 4) is that UCS with reliable edges algorithm ignores reliable flag and computes the cost of path by sum of costs of edges in path. UCS with unreliable edges computes the cost of path as sum of edge costs plus 1/2 for each unreliable edge.
1/2 is the expected value of Irwin-Hall distribution for 1 uniform distributed random variable(between 0 and 1).
So the cost of path in UCS with unreliable edges equals to cost of path in UCS with reliable edges plus m/2, where m is the count of unreliable edges in path.
This means that the minimal path found by UCS with reliable edges cannot be the same as the  minimal path found by UCS with unreliable edges. 
For example, if it contains many unreliable edges, its cost increases more than another path, which contains small count of unreliable edges.
Let's see this for two path examples:

  2.8  1   1.6  0   1.94 0   2.02 0
A--------S--------R--------P--------B   cost t3: 8.36   cost t4: 9.86


  2.8  1   1.98 1   4.22 1
A--------S--------F--------B            cost t3: 9      cost t4: 9


As you can see, the cost of first path for UCS with reliable edges is less than the cost of second. But first path contains 3 unreliable edges, while in second path all edges are reliable. Therefore the cost of first path increased by (1.5=3/2) and is greater than second path's cost. So UCS with reliable edges will return first path as a result, UCS with unreliable edges -second path.

Execution of the program:

For code compilation:
Run the makefile by first running "make clean" and then followed by the command "make makefile" and then followed by the command "make"

For running the code:
Execute the command in the following format as given in the specification:-
search -t <task> -s <start_node> -g <goal_node> -i <input_file> -op <output_path> -ol <output_log>

