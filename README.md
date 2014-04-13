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


Execution of the program:

For code compilation:
Run the makefile by first running "make clean" and then followed by the command "make makefile" and then followed by the command "make"

For running the code:
Execute the command in the following format as given in the specification:-
search -t <task> -s <start_node> -g <goal_node> -i <input_file> -op <output_path> -ol <output_log>

