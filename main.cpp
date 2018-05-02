#include <stdlib.h>
#include <iostream>
#include "Node.h"
#include <math.h>
using namespace std;


int main(){
    Node n0(5);
    Node n1(200);
    n0.join(NULL);
    n0.insert(30,100);
    n1.join(&n0);
    Node n2(100);
    n2.join(&n1);
    n0.insert(5,200);
    n0.remove(30);
    Node n3(2);
    n3.join(&n0);
    Node n4(150);
    n4.join(&n2);
    n3.insert(130,50);
}