#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "cuckoo_table.h"



using namespace std;

class Node
{
public:
    Node()
    {
        mContent = ' ';
        mMarker = false;
    }
    ~Node() {}
    char content()
    {
        return mContent;
    }
    void setContent(char c)
    {
        mContent = c;
    }
    bool wordMarker()
    {
        return mMarker;
    }
    void setWordMarker()
    {
        mMarker = true;
    }
    void resetWordMarker()
    {
        mMarker = false;
    }
    //Node* findChild(char c);
    /*void appendChild(Node* child)
    {
        mChildren.push_back(child);

    }
    /*vector<Node*> children()
    {
        return mChildren;
    }*/

public:
    Node* left;
    Node* right;
    //Node* parent;
    //double key_weight;
    //double weight;
    keyType keytype;
    size_t leaf_num;
    int key_num;
    int node_num;
    int bignk;
    int prefixlength;
    int action;
    int domi_action;
    //vector<Node*> mChildren;
private:
    char mContent;
    bool mMarker;

};

class Trie
{
public:
    Trie();
    ~Trie();
    void addWord(string s,double weight, keyType keytype,int prefixlength,int action);

    void addWordCount(string s,int prefixlength, keyType keytype, size_t pktNum);

    void addWordCountNum(string s,int prefixlength, keyType keytype, int action, size_t keyNum);

    bool searchWord(string s);

    bool searchPrefix(string s);

    bool searchAggrPrefix(string s, int length, size_t& aggrCount);

    bool searchAggrPrefixQuery(string s, int length, size_t& aggrCount);

    int computeKeyNum(Node *pnode, keyType key_type);

    bool isLeaf(Node *pnode);

    void arregatePrefix(Node *pnode,double sBIGKEYTHLD, bool isInit);

    void arregatePrefix8(Node *pnode,double sBIGKEYTHLD, int& prefixlength, bool isInit);

    void setBigKey(Node *pnode);

    void setBlackKey(Node *pnode);

    bool setPrefixAggregate(Node *pprefix);

    bool printNodes(Node *pnode,vector<char> word,vector<string> &key,
                     vector<int> &keyaction,vector<string> &blackkey,
                     vector<int> &blackkey_prefix, vector<string> &aggregatekey, vector<int>& aggr_actions,
                     vector<string> &other_keys, vector<int> &other_keyactions);
                     
    bool printNode(Node *pnode,vector<char> word,vector<string> &key,
                     vector<int> &keyaction,vector<string> &blackkey,
                     vector<int> &blackkey_prefix, vector<string> &aggregatekey);                     

    void setAgtInvalid(Node *pnode);

    bool setAgtInvalidTrie(Node *pnode);

    bool nodeCount(Node *pnode,size_t &countkey,size_t &countaggregatekey,
                     size_t &countblackkey,size_t &countorikey);

    bool recoverTrie(Node *pnode, size_t& aggrCount);

    bool queryAggrTrie(Node *pnode, size_t& aggrCount);

    bool getLeaf(Node *pnode,vector<char> word,vector<string> &flow, vector<size_t> &flow_cnt, vector<int>& flowActions);

    size_t compPktNum(Node *pnode);

    void deleteChild(Node *pnode);

    void findBlackKey(Node *pnode, size_t &countblackkey);

    void printBlackKey(Node *pnode,vector<char> word,vector<string> &blackkey,
                     vector<int> &blackkey_prefix);

    void computeNodeNum(Node *pnode);

    Node* findChildTrie(char c, Node* pnode);

    void compute_action_kn(Node* pnode, vector<size_t> &key_num_vec, vector<int>& actions);
    
    void find_domi_action(Node* pnode, vector<int>& actions);

    void aggregate_output(Node* pnode, size_t &countkey,size_t &countaggregatekey, size_t &countblackkey,
                   size_t &countorikey, vector<string> &keys,
                   vector<int> &keyaction,vector<string> &other_keys,
                   vector<int> &other_keyaction,vector<string> &blackkey,
                   vector<int> &blackkeyPrefixes, vector<string> &aggregatekey, vector<int>& aggr_actions, bool isPrint);
    
    bool isDominate(vector<size_t> &key_num_vec);
    void printOtherKey(Node *pnode,vector<char> word,vector<string> &other_keys,
                     vector<int> &other_keyactions);


    Node* root;
    Node* root8;
    int maction;
    int trieSeq;
    int dominant_action;
private:

};
