# Compact-Binary-Trees
Generate a compact (shortest possible) binary tree from a Boolean function

To compile, enter on command line: g++ main.cpp -o main

To run, enter on command line: ./main

Functionality of the project is exactly same as Binary Trees project, with added compacting capability. 

Compacting method: where the leaves of two nodes are the same, they are combined into one so as to reduce the number of total nodes. 

For example, for original tree X1 & X2: 

                                                  X1 
                                             X2         X2 
                                           0    0     0    1 
                                          
The compact tree would look like:

                                                  X1
                                              0         X2 
                                                      0    1
