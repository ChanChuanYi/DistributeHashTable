#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <map>
#include <set>
#include <vector>

#define BITLENGTH 8
using namespace std;
//forward declaration
class Node;

//The following code is just for reference. You can define your own finger table class.
//Since the index uniquely determines the interval, only the successor needs to be maintained.  
class FingerTable{
public:
	/**
	 * @param nodeId: the id of node hosting the finger table.
	 */
	FingerTable(){
		nodeId_=0;
		fingerTable_.resize(BITLENGTH+1);
	}	

	void set(size_t index, Node* successor){
		fingerTable_[index] = successor;
	}
	Node* get(size_t index) {
		return fingerTable_[index];
	}
	void set_id(uint8_t nodeId){
        nodeId_=nodeId;
        fingerTable_.resize(BITLENGTH+1);
	}
	// TODO: complete print function
	void prettyPrint();
private:
	uint8_t nodeId_;
	std::vector<Node*> fingerTable_;
};


class Node {
public:
	Node(uint8_t id){
		id_=id;
        fingerTable_.set_id(id);
        Node* node_pred = NULL;
	}
	//TODO: implement node join function
	/**
	 * @param node: the first node to contact with to initialize join process. If this is the first node to join the Chord network, the parameter is NULL.
	 */
    
    uint8_t get_id();    
    void set_pred(Node* node);
	Node* get_pred();
    
    void set_finger(size_t index, Node* node){
    	fingerTable_.set(index, node);
    }
    Node* get_finger(uint8_t num){
    	return fingerTable_.get(num);
    }
    Node* get_succ(){
    	return fingerTable_.get(1);
    }
     Node* find_successor(uint8_t id_tofind);

    Node* find_prede(uint8_t id_tofind);

    Node* closest_pred_finger(uint8_t id_tofind);

    void update_finger_table(Node* node, int num);
	
	void smaller_key(vector<pair<uint8_t, uint8_t> >& keys, uint8_t id);

	void join(Node* node);
	//TODO: implement DHT lookup
    bool find_key(uint8_t id_tofind);
    
    void set_key(uint8_t id_toset, uint8_t value);

    void remove_key(uint8_t id_toRemove);

	uint8_t find(uint8_t key);
	//TODO: implement DHT key insertion
    void insert(uint8_t key);

	void insert(uint8_t key, uint8_t value);
	//TODO: implement DHT key deletion
	void remove(uint8_t key);
   
   
private:
	uint8_t id_;
	Node* node_pred;
    Node* node_succ;
	FingerTable fingerTable_;
	std::map<uint8_t, uint8_t> localKeys_;
    

};

#endif