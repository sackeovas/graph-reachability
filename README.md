# Graph Reachability

This project implements a simple program to count how many vertices in a directed graph are reachable from a given initial vertex.

The vertices are represented by a `struct vertex` with an ID and a list of edges. The main function `count_reachable` uses a basic hash table to keep track of visited vertices and a queue to perform a breadth-first search (BFS).

All code, including simple test cases, is contained in one file: reach.c. When you compile and run this file, the tests will run automatically and print a confirmation message if everything works correctly.

## How to compile and run

You need a C compiler, e.g. GCC. From the terminal, in the project folder:

gcc reach.c -o reach.exe
.\reach.exe

This will compile reach.c and run the included test cases. If everything works, you should see:
"All tests passed!"

## Files:
  reach.c – the complete implementation with BFS and hash table, including test cases.
  README.md – this file.
