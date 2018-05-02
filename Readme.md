# Project Title

This is an implementation of Chord by Chuan-Yi Chan (cchan88@ucsc.edu)
and Bo-Syun Liao (boliao@ucsc.edu) for the final project of UCSC 
CMPE 252, Computer Network.


A node variable is declared with it ID as follow:

    Node n0(ID);

The following APIs are implemented as described in Project A:

    1. void Node::join(Node* node):
        
        Current node joins the Chord Network through input argument.
        If current node is the first node to join Chord Network, set input node as NULL.
        The program also prints the finger table of current node.

    2. uint8_t Node::find(uint8_t key):
        
        Current node will find the node responsible for such key.
        Return Value:
            (1)Node ID if the key is found.
            (1)-1 if the key is not found in Chord Network.

    3. void Node::insert(uint8_t key):

        Insert key to corresponding node. If not specified, default key value is set to zero.
        If such key already exists in Chord Network, it will change its value;

    4. void Node::insert(uint8_t key, uint8 value):

        Insert key to corresponding node with value.
        If such key already exists in Chord Network, it will change its value;

    5. void Node::remove(uint8_t key):

        Remove key from corresponding node with value.

There are also several helper functions implemented following the psuedo code in the Chord Paper.

A very simple testing main function is developed and please compile with:

    g++ Node.cpp main.cpp -o main
    
Or simple compile with:
   
    make



===================================================================
