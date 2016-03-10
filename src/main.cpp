# include <iostream>
# include <vector>
# include <sstream>
# include <fstream>
# include <algorithm>
# include <string>
# include <vector>
# include <time.h>

# include "trie.h"


using namespace std;

typedef vector<int> ints;

// assign random actions to a key
int assign_rand_action(ints uni_actions)
{
	int action_size = uni_actions.size();
	return uni_actions[rand()%action_size];
}

// add keys to trie
void add_keys(char* infile_name, Trie* trie, ints uni_actions)
{
	ifstream infile(infile_name);
	if(!infile) 
	{
		cout<<"load key file error!"<<endl;
		exit(0);
	}
	
	// add the keys to trie
	int key_prefix;
	string key_ipv4;
	size_t key_num = 0;
	
	while(infile >> key_prefix >> key_ipv4)
	{
		int key_action = assign_rand_action(uni_actions);
		trie->addWord(key_ipv4,0, iskey, key_prefix, key_action);
	}
}

void aggr(Trie* trie, ints uni_actions)
{
	cout<<"aggregation start."<<endl;
	
	// aggregate the keys and output the aggregated keys
	double weight_threshold = 0.1; 
	size_t countkey = 0;
	size_t countaggregatekey = 0; 
	size_t countblackkey = 0;
    size_t countorikey = 0; 
    vector<string> keys;
    vector<int> keyaction;
    vector<string> other_keys;
    vector<int> other_keyaction;
    vector<string> blackkey;
    vector<int> blackkeyPrefixes; 
    vector<string> aggregatekey;
    
    int prefixlength;
    bool isPrint; 
    bool isInit;
                   
	trie->find_domi_action(trie->root, uni_actions);
    
    // output to file
    
}

int main(int argc, char** argv)
{
	// initialize rand 
	srand(time(NULL));

	// build a trie
	Trie* trie = new Trie();
	
	// load key file
	char* infile_name = argv[1];
	
	// actions to choose
	int action_size = 4;
	
	ints uni_actions;
	for(int i = 0; i < action_size; i++)
		uni_actions.push_back(i);
	
	// add the keys to trie
	add_keys(infile_name, trie, uni_actions);
	
	// aggregate the keys and output the aggregated keys                
	aggr(trie, uni_actions);
	
	// delete the trie
	delete trie;
}
