#include <stdlib.h>
#include <iostream>
#include "Node.h"
#include <math.h>
using namespace std;

#define BITLENGTH 8


//======================interval helper function========================//
bool inRange(uint8_t begin, uint8_t end, uint8_t id,int flag){
	int MAXid = pow(2, BITLENGTH);
	int MINid = 0;
    if(flag==1){
	    return  (begin < end && begin < id && id <= end)||
			    (begin > end && ((begin < id && id <= MAXid) || (MINid <= id && id <= end))) ||
			    ((begin == end) /*&& (id == begin)*/);
    }
    else if(flag==2){
        return  (begin < end && begin < id && id < end)||
			    (begin > end && ((begin < id && id <= MAXid) || (MINid <= id && id < end))) ||
			    ((begin == end) && (id == begin));	
    }
    else{
    	return  (begin < end && begin <= id && id < end)||
			    (begin > end && ((begin <= id && id <= MAXid) || (MINid <= id && id < end))) ||
			    ((begin == end) && (id == begin));	
    }
}
//======================interval helper function========================//




//====================join operation functions===================//
void FingerTable::prettyPrint(){
	printf(" i | FT[i]\n");
	for(int i=1;i<=BITLENGTH;i++){
        printf(" %d | %d\n",i,fingerTable_[i]->get_id());
	}
}

uint8_t Node::get_id(){
	return id_;
}

Node* Node::get_pred(){
	return node_pred;
}

void Node::set_pred(Node* node){
    node_pred = node;
}


//ask node n to find successor
Node* Node::find_successor(uint8_t id_tofind){
    Node* n = this->find_prede(id_tofind);
    return n->get_finger(1); //find the successor of n.
}

//ask node n to find predeccessor
Node* Node::find_prede(uint8_t id_tofind){
    Node* n = this;
    while(!inRange(n->get_id(),n->get_finger(1)->get_id(), id_tofind,1)){
        n = n -> closest_pred_finger(id_tofind);
    }
    return n;
}

Node* Node::closest_pred_finger(uint8_t id_tofind){
	


	for(int i = BITLENGTH; i>=2; i--){
		Node* temp = this->get_finger(i);
		uint8_t temp_id = temp->get_id();
		if(inRange(id_, id_tofind, temp_id,2)){
			return temp;
		}
	}
	return this->get_succ();
	
}


void Node::update_finger_table(Node* node, int num){
    uint8_t id = node->get_id();
    if(id_ < this->get_finger(num)->get_id()){
	    if(id > id_ && id < this->get_finger(num)->get_id()){
	    	
	    	this->set_finger(num, node);
	    	Node* p = this->get_pred();
	    	p->update_finger_table(node, num);
	    }
	}
	else if(id_ == this->get_finger(num)->get_id()){
        this->set_finger(num, node);
	    Node* p = this->get_pred();
	    p->update_finger_table(node, num);   
	}
	else{
        if(id > id_ || id < this->get_finger(num)->get_id()){
            
            this->set_finger(num, node);
	    	Node* p = this->get_pred();
	    	p->update_finger_table(node, num);
        }
	}
}

void Node::smaller_key(vector<pair<uint8_t, uint8_t> >& keys, uint8_t id){
	for(map<uint8_t, uint8_t>::iterator it = localKeys_.begin();it!=localKeys_.end();){
		uint8_t curr = it->first;
		if(curr<=id){
			keys.push_back(make_pair(curr, it->second));
			localKeys_.erase(it++);
		}
		else{
			++it;
		}
	}

}
//====================join operation functions===================//





void Node::join(Node* node){
    if(node){
        //init_finger();
        Node* successor = node->find_successor(id_+1);  //need to implement find_successor
        fingerTable_.set(1, successor);
        node_pred = successor->get_pred();
        node_succ = successor;
        successor->set_pred(this);
        node_pred->set_finger(1, this);
        
        for(int i=1;i<=7;i++){
            
        	uint8_t finger_entry = (id_+uint8_t(pow(2,i)));
        	if(inRange(id_, fingerTable_.get(i)->get_id(), finger_entry,3)){
        		fingerTable_.set(i+1, fingerTable_.get(i));
        	
        	}
        	else{
        		Node* next_node = node->find_successor(finger_entry%int(pow(2,8)));
                fingerTable_.set(i+1,next_node);
        	}

        }
        
        //update_other();
        for(int i =1;i<=BITLENGTH;i++){
            Node* p = this->find_prede(id_ - pow(2, i-1));
            p->update_finger_table(this, i);
        }
      
    }
    else{
    	for(int i=1;i<=8;i++){
            fingerTable_.set(i, this);
    	}
        node_succ = this;
    	node_pred = this;
    }
    
    vector<pair<uint8_t,uint8_t> > smaller_keys;
    node_succ->smaller_key(smaller_keys, id_); // find keys that should store in newly joined node.
    

    printf("-------------------------\n");
    printf("Node %d joins.\n", this->get_id());
    printf("Finger Table of Node %d is as follow:\n",this->get_id());
    fingerTable_.prettyPrint();
    for(int i=0;i<smaller_keys.size();i++){
    	pair<uint8_t, uint8_t> curr_pair = smaller_keys[i];
    	localKeys_[curr_pair.first]=curr_pair.second;
    	printf("Key %d is moved from successor Node %d to Node %d\n",curr_pair.first, node_succ->get_id(), this->get_id());
    }
    printf("--------end join---------\n");
}







//===============Key Handle Functions================//
bool Node::find_key(uint8_t id_tofind){
	if(localKeys_.find(id_tofind)!=localKeys_.end())
		return true;
	return false;
}

void Node::set_key(uint8_t id_toset, uint8_t value){
	if(localKeys_.find(id_toset)!=localKeys_.end()){
		printf("Key %d has already been in Node %d, value change to %d\n",id_toset,this->get_id(),value);
	}
	localKeys_[id_toset] = value;
}

void Node::remove_key(uint8_t id_toRemove){
	if(localKeys_.find(id_toRemove)!=localKeys_.end()){
		localKeys_.erase(id_toRemove);
	}
	else{
		printf("Key %d does not exist in Chord", id_toRemove);
	}
}
//=============Key Handle Function Ends=================//



uint8_t Node::find(uint8_t key){
    if(inRange(node_pred->get_id(), id_, key,1)){
    	//I am responsible for this key
    	if(localKeys_.find(key)!=localKeys_.end()){
    		return id_;
    	}
    	else{
    		printf("Key %d does not exist in Chord",key);
    		return -1;
    	}
    }
    else{
    	
    	Node* key_succ = this->find_successor(key);
        if(key_succ->find_key(key)){
        	return key_succ->get_id();
        }
        else {
            printf("Key %d does not exist in Chord",key);
            return -1;
        }
    }
}

//function overload, key value is defaultly set to 0 if not specified.
void Node::insert(uint8_t key){
    if(inRange(node_pred->get_id(), id_, key,1)){
    	//I am responsible for this key

    	if(localKeys_.find(key)!=localKeys_.end()){
		    printf("Key %d has already been in Node %d, value change to %d\n",key,this->get_id(),0);
	    }
    	localKeys_[key] = 0;
    }
    else{

    	Node* key_succ = this->find_successor(key);
        key_succ->set_key(key, 0);
        printf("Key %d has successfully insert into Node %d with value %d\n",key,key_succ->get_id(),0);
    }


}

void Node::insert(uint8_t key, uint8_t value){

    if(inRange(node_pred->get_id(), id_, key,1)){
    	//I am responsible for this key
    	
    	if(localKeys_.find(key)!=localKeys_.end()){
		    printf("Key %d has already been in Node %d, value change to %d\n",key,this->get_id(),value);
	    }
    	localKeys_[key] = value;
    }
    else{
    	Node* key_succ = this->find_successor(key);
        key_succ->set_key(key, value);
        printf("Key %d has successfully insert into Node %d with value %d\n",key,key_succ->get_id(),value);
    }


}


void Node::remove(uint8_t key){
    if(inRange(node_pred->get_id(), id_, key,1)){
    	//I am responsible for this key
    	if(localKeys_.find(key)!=localKeys_.end()){
		    localKeys_.erase(key);
	    }
	    else{
		    printf("Key %d does not exist in Chord", key);
	    }
    }
    else{
    	Node* key_succ = this->find_successor(key);
        key_succ->remove_key(key);
        printf("Key %d has successfully remove from Node %d\n",key,key_succ->get_id());
    }    
}