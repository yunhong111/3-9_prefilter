#include "trie.h"
using namespace std;

/** find child whose content is c
*/
/*Node* Node::findChild(char c)
{
    for ( int i = 0; i < mChildren.size(); i++ )
    {
        Node* tmp = mChildren.at(i);
        if ( tmp->content() == c )
        {
            return tmp;
        }
    }

    return NULL;
}*/

Node* Trie::findChildTrie(char c, Node* pnode)
{
    if(pnode == NULL)
        return NULL;
    if(pnode->left!= NULL && pnode->left->content()==c)
    {
        return pnode->left;
    }
    else if(pnode->right!= NULL && pnode->right->content()==c)
    {
        return pnode->right;
    }
    else
    {
        findChildTrie(c,pnode->left);
        findChildTrie(c,pnode->right);
    }

    return NULL;
}

/** constructor
*/
Trie::Trie()
{
    root = new Node();
    root8 = NULL;
    root->left = NULL;
    root->right = NULL;
}
/** deconstructor
*/
Trie::~Trie()
{
    //root = NULL;
    //root8 = NULL;
    // Free memory
}

/** add item
*/
void Trie::addWord(string s,double weight, keyType keytype,int prefixlength, int action)
{
    Node* current = root;
    Node* tmp;

    if ( s.length() == 0 )
    {
        current->setWordMarker(); // an empty word
        return;
    }

    for ( int i = 0; i < prefixlength; i++ )
    {
        //Node* child = current->findChild(s[i]);
        Node* child = findChildTrie(s[i],current);
        if ( child != NULL )
        {
            current = child;
        }
        else
        {

            tmp = new Node();
            tmp->setContent(s[i]);
            tmp->prefixlength = i+1;
            tmp->left = NULL;
            tmp->right = NULL;
            tmp->keytype = keytype;
            tmp->action = action;
            tmp->node_num = 0;
            //current->appendChild(tmp);
            //tmp->parent = current;
            if(i == 0)
            {
                if (s[i] == '0' && root->left == NULL)
                {
                    root->left = tmp;
                    //root->right = NULL;
                    current = root->left;
                    cout<<"root left"<<endl;
                }
                else if(s[i] == '1' && root->right == NULL)
                {
                    //root->left = NULL;
                    root->right = tmp;
                    current = root->right;
                    cout<<"root right"<<endl;
                }
            }
            else
            {
                if (s[i] == '0')
                {
                    current->left = tmp;
                }

                else
                {
                    current->right = tmp;
                }

                current = tmp;

            }
            if(root8 == NULL)
            {
                if(i == 7)
                {
                    root8 = current;
                }
            }
            //delete tmp;

        } //else
        if ( i == prefixlength - 1 )
        {
            if (keytype == iskey)
				current->setWordMarker();
            //current->weight = weight;
            current->prefixlength = prefixlength;
            
            //current->leaf_num = 1<<(32 - prefixlength);
            current->leaf_num = 1;

            if (keytype == iskey)
            {
                //current->key_weight = weight;
                //current->key_num = 1<<(32 - prefixlength);
                current->key_num = 1;
            }


        }

    }//for ( int i = 0; i < s.length(); i++ )
}

/** add item
*/
void Trie::addWordCount(string s,int prefixlength, keyType keytype, size_t pktNum)
{
    Node* current = root;
    Node* tmp;

    if ( s.length() == 0 )
    {
        current->setWordMarker(); // an empty word
        return;
    }

    for ( int i = 0; i < prefixlength; i++ )
    {
        //Node* child = current->findChild(s[i]);
        Node* child = findChildTrie(s[i],current);
        if ( child != NULL )
        {
            current = child;

        }
        else
        {

            tmp = new Node();
            tmp->setContent(s[i]);
            tmp->keytype = iskey;
            tmp->prefixlength = i+1;
            tmp->left = NULL;
            tmp->right = NULL;

           // current->appendChild(tmp);
            if(i == 0)
            {
                if (s[i] == '0' && root->left == NULL)
                {
                    root->left = tmp;
                    //root->right = NULL;
                    current = root->left;
                }
                else if ( root->right == NULL)
                {
                    //root->left = NULL;
                    root->right = tmp;
                    current = root->right;

                }
            }
            else
            {
                if (s[i] == '0')
                {
                    current->left = tmp;
                }

                else
                {
                    current->right = tmp;
                }

                current = tmp;

            }
            if(root8 == NULL)
            {
                if(i == 7)
                {
                    root8 = current;
                }
            }
            //delete tmp;

        } //else

        if ( i == prefixlength - 1 )
        {

            if(current->wordMarker())
            {
                current->leaf_num += pktNum;
            }
            else
            {
                current->leaf_num = pktNum;
            }
            if(i!= 31/*current->left != NULL || current->right != NULL*/)
            {
                current->leaf_num = compPktNum(current);
                if (current->leaf_num == 0)
                    current->leaf_num = pktNum;
            }
            current->keytype = keytype;
            current->prefixlength = prefixlength;
            //current->action = action;
            current->setWordMarker();
        }


    }//for ( int i = 0; i < s.length(); i++ )
    //delete(current);
    //delete(tmp);

}

void Trie::addWordCountNum(string s,int prefixlength, keyType keytype, int action, size_t keyNum)
{
    Node* current = root;
    Node* tmp;

    if ( s.length() == 0 )
    {
        current->setWordMarker(); // an empty word
        return;
    }

    for ( int i = 0; i < prefixlength; i++ )
    {
        //Node* child = current->findChild(s[i]);
        Node* child = findChildTrie(s[i],current);
        if ( child != NULL )
        {
            current = child;

        }
        else
        {

            tmp = new Node();
            tmp->setContent(s[i]);
            tmp->keytype = iskey;
            tmp->prefixlength = i+1;
            tmp->left = NULL;
            tmp->right = NULL;

            //current->appendChild(tmp);
            if(i == 0)
            {
                if (s[i] == '0' && root->left == NULL)
                {
                    root->left = tmp;
                    //root->right = NULL;
                    current = root->left;
                }
                else if ( root->right == NULL)
                {
                    //root->left = NULL;
                    root->right = tmp;
                    current = root->right;

                }
            }
            else
            {
                if (s[i] == '0')
                {
                    current->left = tmp;
                }

                else
                {
                    current->right = tmp;
                }

                current = tmp;

            }
            if(root8 == NULL)
            {
                if(i == 7)
                {
                    root8 = current;
                }
            }
            //delete tmp;

        } //else

        if ( i == prefixlength - 1 )
        {

            if(current->wordMarker())
            {
                current->leaf_num += keyNum;
            }
            else
            {
                current->leaf_num = keyNum;
            }
            if(i!= 31/*current->left != NULL || current->right != NULL*/)
            {
                current->leaf_num = compPktNum(current);
                if (current->leaf_num == 0)
                    current->leaf_num = keyNum;
            }
            current->keytype = keytype;
            current->prefixlength = prefixlength;
            current->action = action;
            current->setWordMarker();
        }


    }//for ( int i = 0; i < s.length(); i++ )
    //delete(current);
    //delete(tmp);
}

size_t Trie::compPktNum(Node *pnode)
{
    int num;
    if( pnode==NULL)
    {
        return 0;
    }

    if(pnode->wordMarker())  /*pnode->wordMarker()*/
    {
        num = pnode->leaf_num;
    }
    else
    {
        num = compPktNum(pnode->left) + compPktNum(pnode->right);
    }

    pnode->leaf_num = num;
    return num;
}

bool Trie::getLeaf(Node *pnode,vector<char> word,vector<string> &flow, vector<size_t> &flow_cnt, vector<int>& flowActions)
{


    if(pnode == NULL)
    {
        return false;
    }


    if(pnode->wordMarker() && pnode != root)
    {
        //if(pnode->keytype == iskey)
        {

            word.push_back(pnode->content());

            vector<char> word_reverse;
            word_reverse.clear();

            for(int i= word.size(); i <32; i++ )
            {
                word.push_back('0');
            }
            for(int i =0; i <word.size(); i++)
            {
                word_reverse.push_back(word[word.size()-i-1]);

            }
            flow.push_back((parsebin2IPV4(&word_reverse[0])));
            flow_cnt.push_back(pnode->leaf_num);
            flowActions.push_back(pnode->action);
        }
    }
    else
    {
        if(pnode != root)
        {
            word.push_back(pnode->content());

        }

        getLeaf(pnode->left,word,flow,flow_cnt, flowActions);
        getLeaf(pnode->right,word,flow,flow_cnt, flowActions);
    }
    return true;
}

/** print node
*/
bool Trie::printNodes(Node *pnode,vector<char> word,vector<string> &key,
                     vector<int> &keyaction,vector<string> &blackkey,
                     vector<int> &blackkey_prefix, vector<string> &aggregatekey, vector<int>& aggr_actions, vector<string> &other_keys,
                     vector<int> &other_keyactions)
{

    if(pnode == NULL)
        return false;

    if(pnode->keytype==isaggregatekey && pnode != root && pnode->prefixlength>=8)
    {

        vector<char> word_reverse;
        vector<char> word_whole;

        word_whole = word;
        word_whole.push_back(pnode->content());
        for(int i= word_whole.size(); i <32; i++ )
        {
            word_whole.push_back('0');
        }

        for(int i =0; i <word_whole.size(); i++)
        {
            word_reverse.push_back(word_whole[word_whole.size()-i-1]);

        }
        key.push_back((parsebin2IPV4(&word_reverse[0])+"/"+num2str(pnode->prefixlength)));
        keyaction.push_back(pnode->domi_action);
        aggregatekey.push_back((parsebin2IPV4(&word_reverse[0])+"/"
                                +num2str(pnode->prefixlength)));
        aggr_actions.push_back(pnode->domi_action);

        if(word.size() == 0)
        cout<<"agg word is null!"<<endl;
        if(pnode->prefixlength == 2)
            cout<<"* Line 560 Aggr: "<<(parsebin2IPV4(&word_reverse[0])+
                                        "/"+num2str(pnode->prefixlength))<<endl;

        // --------------------------
        // print blackkeys and other keys
        // printBlackKey(pnode, word, blackkey, blackkey_prefix);

        //cout<<"key:"<<(parsebin2IPV4(&word_reverse[0])+"/"+num2str(pnode->prefixlength))<<" "<<pnode->action<<endl;
        //cout<<"key:"<<endl<<((&word_reverse[0]))<<endl;
        //cout<<"other keys start."<<endl;
        
        printOtherKey(pnode, word, other_keys, other_keyactions);

        //cout<<"other keys end."<<endl;
    }
    else if((pnode->wordMarker()) & pnode != root)
    {

        word.push_back(pnode->content());
        if(pnode->keytype == iskey | pnode->keytype == isblackkey)
        {

            vector<char> word_reverse;

            for(int i= word.size(); i <32; i++ )
            {
                word.push_back('0');
            }
            for(int i =0; i <word.size(); i++)
            {
                word_reverse.push_back(word[word.size()-i-1]);

            }

            if(pnode->keytype == iskey)
            {
                key.push_back((parsebin2IPV4(&word_reverse[0])+"/"+num2str(pnode->prefixlength)));
                keyaction.push_back((pnode->action));

                if(word.size() == 0)
                    cout<<"leaf word is null!"<<endl;

                if(pnode->prefixlength == 2)
            cout<<"* Line 592 Aggr: "<<(parsebin2IPV4(&word_reverse[0])+
                                        "/"+num2str(pnode->prefixlength))<<endl;

            }
            
            /*else if(pnode->keytype == iskey && pnode->action != maction)
            {
				
			}
            else if(pnode->keytype == isblackkey)
            {
                g_vcountblackkey[maction]++;
                blackkey.push_back((parsebin2IPV4(&word_reverse[0])));
                blackkey_prefix.push_back(pnode->prefixlength);


            }*/

        }

    }

    else
    {
        if(pnode != root)
            word.push_back(pnode->content());
        printNodes(pnode->left,word,key,keyaction,blackkey,blackkey_prefix,aggregatekey, aggr_actions, other_keys, other_keyactions);
        printNodes(pnode->right,word,key,keyaction,blackkey,blackkey_prefix,aggregatekey, aggr_actions, other_keys, other_keyactions);
    }

    return true;

}

bool Trie::printNode(Node *pnode,vector<char> word,vector<string> &key,
                     vector<int> &keyaction,vector<string> &blackkey,
                     vector<int> &blackkey_prefix, vector<string> &aggregatekey)
{

    if(pnode == NULL)
        return false;

    if(pnode->keytype==isaggregatekey && pnode != root && pnode->prefixlength>=8)
    {

        vector<char> word_reverse;
        vector<char> word_whole;

        word_whole = word;
        word_whole.push_back(pnode->content());
        for(int i= word_whole.size(); i <32; i++ )
        {
            word_whole.push_back('0');
        }

        for(int i =0; i <word_whole.size(); i++)
        {
            word_reverse.push_back(word_whole[word_whole.size()-i-1]);

        }
        key.push_back((parsebin2IPV4(&word_reverse[0])+"/"+num2str(pnode->prefixlength)));
        keyaction.push_back((pnode->action));
        aggregatekey.push_back((parsebin2IPV4(&word_reverse[0])+"/"
                                +num2str(pnode->prefixlength)));

        if(word.size() == 0)
        cout<<"agg word is null!"<<endl;
        if(pnode->prefixlength == 2)
            cout<<"* Line 560 Aggr: "<<(parsebin2IPV4(&word_reverse[0])+
                                        "/"+num2str(pnode->prefixlength))<<endl;

        // --------------------------
        // print blackkeys and other keys
        printBlackKey(pnode, word, blackkey, blackkey_prefix);
        //printOtherKey(pnode, word, other_keys, other_keyactions);
    }
    else if((pnode->wordMarker()) & pnode != root)
    {

        word.push_back(pnode->content());
        if(pnode->keytype == iskey | pnode->keytype == isblackkey)
        {

            vector<char> word_reverse;

            for(int i= word.size(); i <32; i++ )
            {
                word.push_back('0');
            }
            for(int i =0; i <word.size(); i++)
            {
                word_reverse.push_back(word[word.size()-i-1]);

            }

            if(pnode->keytype == iskey && pnode->action == maction)
            {
                key.push_back((parsebin2IPV4(&word_reverse[0])+"/"+num2str(pnode->prefixlength)));
                keyaction.push_back((maction));

                if(word.size() == 0)
                    cout<<"leaf word is null!"<<endl;

                if(pnode->prefixlength == 2)
            cout<<"* Line 592 Aggr: "<<(parsebin2IPV4(&word_reverse[0])+
                                        "/"+num2str(pnode->prefixlength))<<endl;



            }     
            else if(pnode->keytype == iskey && pnode->action != maction)
            {
				
			}
            else if(pnode->keytype == isblackkey)
            {
                g_vcountblackkey[maction]++;
                blackkey.push_back((parsebin2IPV4(&word_reverse[0])));
                blackkey_prefix.push_back(pnode->prefixlength);


            }

        }

    }

    else
    {
        if(pnode != root)
            word.push_back(pnode->content());
        printNode(pnode->left,word,key,keyaction,blackkey,blackkey_prefix,aggregatekey);
        printNode(pnode->right,word,key,keyaction,blackkey,blackkey_prefix,aggregatekey);
    }

    return true;

}
bool Trie::nodeCount(Node *pnode,size_t &countkey,size_t &countaggregatekey,
                     size_t &countblackkey,size_t &countorikey)
{

    if(pnode == NULL)
        return false;

    if(pnode->keytype == isblackkey)
    {
        countblackkey++;
        cout<<"black key!"<<endl;
    }

    if(pnode->keytype==isaggregatekey)
    {
        countkey++;
        //g_vcountkey[maction]++;
        countaggregatekey++;

        // find blackkey
        findBlackKey(pnode, countblackkey);
        //cout<<"isaggregatekey. "<<" ";
    }
    else if((pnode->wordMarker()) && pnode != root)
    {
		//cout<<pnode->prefixlength<<" ";

        if(pnode->keytype == iskey | pnode->keytype == isblackkey)
        {

            countkey++;
            //g_vcountkey[maction]++;

            if(pnode->keytype == iskey)
            {
                countorikey++;
            }
            else if(pnode->keytype == isblackkey)
            {
                countblackkey++;
                //g_vcountblackkey[maction]++;
            }
            
        }
    }
    else
    {
        nodeCount(pnode->left,countkey,countaggregatekey,countblackkey,countorikey);
        nodeCount(pnode->right,countkey,countaggregatekey,countblackkey,countorikey);
    }

    return true;

}

void Trie::printOtherKey(Node *pnode,vector<char> word,vector<string> &other_keys,
                     vector<int> &other_keyactions)
{
    if( pnode==NULL)
    {
        return;
    }
    
    if((pnode->keytype == iskey || pnode->keytype == invalidkey) && pnode->wordMarker() && pnode->action != pnode->domi_action)
    {
        vector<char> word_reverse;
        vector<char> word_whole;

        word_whole = word;
        word_whole.push_back(pnode->content());
        for(int i= word_whole.size(); i <32; i++ )
        {
            word_whole.push_back('0');
        }

        for(int i =0; i <word_whole.size(); i++)
        {
            word_reverse.push_back(word_whole[word_whole.size()-i-1]);

        }

        other_keys.push_back((parsebin2IPV4(&word_reverse[0])+"/"+num2str(pnode->prefixlength)));//+"/"+num2str(pnode->domi_action)));
        other_keyactions.push_back((pnode->action));

        if(word.size() == 0)
        cout<<"agg word is null!"<<endl;


    }

    else
    {
        if(pnode != root)
        word.push_back(pnode->content());
        printOtherKey(pnode->left,  word,other_keys, other_keyactions);
        printOtherKey(pnode->right, word,other_keys, other_keyactions);
    }
}
void Trie::findBlackKey(Node *pnode, size_t &countblackkey)
{
    if( pnode==NULL)
    {
        return;
    }

    if(pnode->keytype == isblackkey)
    {
        countblackkey ++;
        //cout<<"black: "<<pnode->prefixlength<<" "<<pnode->parent->prefixlength<<endl;
        //cout<<"set black keys ."<<endl;
            //return;
    }

    else
    {
        findBlackKey(pnode->left, countblackkey);
        findBlackKey(pnode->right, countblackkey);
    }
}

void Trie::printBlackKey(Node *pnode,vector<char> word,vector<string> &blackkey,
                     vector<int> &blackkey_prefix)
{
    if( pnode==NULL)
    {
        return;
    }

    if(pnode->keytype == isblackkey)
    {
        vector<char> word_reverse;
        vector<char> word_whole;

        word_whole = word;
        word_whole.push_back(pnode->content());
        for(int i= word_whole.size(); i <32; i++ )
        {
            word_whole.push_back('0');
        }

        for(int i =0; i <word_whole.size(); i++)
        {
            word_reverse.push_back(word_whole[word_whole.size()-i-1]);

        }
        blackkey.push_back((parsebin2IPV4(&word_reverse[0])));
        blackkey_prefix.push_back(pnode->prefixlength);

        if(word.size() == 0)
        cout<<"agg word is null!"<<endl;

    }

    else
    {
        if(pnode != root)
        word.push_back(pnode->content());
        printBlackKey(pnode->left,  word,blackkey, blackkey_prefix);
        printBlackKey(pnode->right, word,blackkey, blackkey_prefix);
    }
}

/** search a prefix
*/
bool Trie::searchPrefix(string s)
{
    Node* current = root;

    while ( current != NULL )
    {
        for ( int i = 0; i < s.length(); i++ )
        {
            //Node* tmp = current->findChild(s[i]);
            Node* tmp = findChildTrie(s[i],current);
            if ( tmp == NULL )
                return false;
            current = tmp;
        }

        return true;
    }

    return false;
}

/** search word
*/
bool Trie::searchWord(string s)
{
    Node* current = root;

    while ( current != NULL )
    {
        for ( int i = 0; i < s.length(); i++ )
        {
            //Node* tmp = current->findChild(s[i]);
            Node* tmp = findChildTrie(s[i],current);
            if ( tmp == NULL )
                return false;
            current = tmp;
        }

        if ( current->wordMarker() )
        {
            return true;
        }

        else
            return false;
    }
    //delete(current);
    return false;
}

/** recover aggregate key
*/
bool Trie::searchAggrPrefix(string s, int length, size_t& aggrCount)
{
    Node* current = root;

    while ( current != NULL )
    {
        for ( int i = 0; i < length; i++ )
        {
            //Node* tmp = current->findChild(s[i]);
            Node* tmp = findChildTrie(s[i],current);
            if ( tmp == NULL )
                return false;
            current = tmp;
            if (current->keytype != isaggregatekey)
            current->keytype = cannotaggr;
        }
        if(current->keytype == isaggregatekey)
        {
            current->keytype = cannotaggr;
            recoverTrie(current, aggrCount);
            aggrCount --;

        }
        return true;

    }

    return false;
}

/** recover aggregate key
*/
bool Trie::searchAggrPrefixQuery(string s, int length, size_t& aggrCount)
{
    Node* current = root;

    while ( current != NULL )
    {
        for ( int i = 0; i < length; i++ )
        {
            //Node* tmp = current->findChild(s[i]);
            Node* tmp = findChildTrie(s[i],current);
            if ( tmp == NULL )
                return false;
            current = tmp;
            //if (current->keytype != isaggregatekey)
            //current->keytype = cannotaggr;
        }
        if(current->keytype == isaggregatekey)
        {
            //current->keytype = cannotaggr;
            queryAggrTrie(current, aggrCount);
            aggrCount --;

        }
        return true;

    }

    return false;
}


/**
compute leaf number for a prefix if key_type == isnonkey
compute key number for a prefix if key_type == iskey
*/
int Trie::computeKeyNum(Node *pnode, keyType key_type)
{
    int num;
    if( pnode==NULL)
    {
        return 0;
    }

    if(pnode->wordMarker())  /*pnode->wordMarker()*/
    {
        num = pnode->key_num;
        if (key_type == iskey && pnode->action == maction)
        {
            num = pnode->key_num;
            pnode->domi_action = maction;
        }
        else
        {
			num = 0;
			pnode->domi_action = maction;
		}
    }

    else
    {
        num = computeKeyNum(pnode->left,key_type) + computeKeyNum(pnode->right,key_type);
        pnode->domi_action = maction;
    }

    if (key_type == iskey)
    {
        pnode->key_num = num;
    }
    else if(key_type == isnonkey)
    {
        pnode->leaf_num = num;
    }


    return num;
}

/** whether a node is a leaf
*/
bool Trie::isLeaf(Node *pnode)
{
    if(pnode == NULL)
        return false;
    if(pnode->left == NULL & pnode->right == NULL)
    {
        return true;
    }
    else return false;
}
/** aggregate keys to prefix
0. leaf_num > 1
1. key_weight/weight>threshold
2. key_num/leaf_num > threshold
3. bigkeynum < threshold
*/

void Trie::arregatePrefix(Node *pnode,double sBIGKEYTHLD, bool isInit)
{
    if( pnode==NULL)
    {
        return;
    }
    if(pnode->wordMarker())
    {
        return;
    }
    if(pnode->node_num > 1 && pnode->keytype != isaggregatekey && pnode->keytype != cannotaggr && pnode->prefixlength >= 8) // if key_num > 1
    {

        if(((float(pnode->key_num)/float(pow(2.0f,32-pnode->prefixlength))) >= (32-pnode->prefixlength)/2.0*sBIGKEYTHLD) && isInit)
        {
            //setPrefixAggregate(pnode);
            //cout<<pnode->node_num
            //cout<<pnode->key_num<<" ";
            setBlackKey(pnode);
            setBigKey(pnode);
            setPrefixAggregate(pnode);
            //if(pnode->prefixlength == 17)
            //cout<<"Line 910, prefix error!"<<endl;
            vector<char> word;
            vector<string> other_keys;
            vector<int> other_keyactions;
            printOtherKey(pnode, word, other_keys, other_keyactions);

        }

        /*else if(pnode->key_weight/pnode->weight >= sBIGKEYTHLD && isInit == 0)
        {
            setPrefixAggregate(pnode);
            setBigKey(pnode);
            setPrefixAggregate(pnode);
            //cout<<"weight    "<<(pnode->key_weight/pnode->weight)<<endl;
        }*/
        else if((float(pnode->key_num)/float(pnode->leaf_num)) >= KEYTHLD&& isInit == 0)
        {
            //cout<<(float(pnode->key_num)/float(pnode->leaf_num))<<endl;
            //if(pnode->bignk <= pnode->key_num/2)
            {
                setBigKey(pnode);
                setPrefixAggregate(pnode);
                //cout<<"num"<<endl;

            }

        }
        else
        {


            arregatePrefix(pnode->left,sBIGKEYTHLD,isInit);
            arregatePrefix(pnode->right,sBIGKEYTHLD,isInit);

        }

    }
    else
    {

        {
            arregatePrefix(pnode->left,sBIGKEYTHLD,isInit);
            arregatePrefix(pnode->right,sBIGKEYTHLD,isInit);
        }
    }
}

/** aggregate keys to a small range of prefix
0. leaf_num > 1
1. key_weight/weight>threshold
2. key_num/leaf_num > threshold
3. bigkeynum < threshold
*/
void Trie::arregatePrefix8(Node *pnode,double sBIGKEYTHLD, int& aggrprefixlength, bool isInit)
{
    if( pnode==NULL)
    {
        return;
    }
    if(pnode->wordMarker())
    {
        return;
    }


    if(((pnode->prefixlength==aggrprefixlength)||(pnode->prefixlength==21)|| (pnode->prefixlength==22)|| (pnode->prefixlength==23)|
    (pnode->prefixlength==24)||(pnode->prefixlength==28)||(pnode->prefixlength==29)|| (pnode->prefixlength==30)|| (pnode->prefixlength==31))
    && pnode->node_num > 1 && pnode->keytype != isaggregatekey && pnode->keytype != cannotaggr)
    {
		
        if(((float(pnode->key_num)/float(pow(2.0f,32-pnode->prefixlength))) >= (32-pnode->prefixlength)/2.0*sBIGKEYTHLD) && isInit)
        {
            //setPrefixAggregate(pnode);
            //cout<<pnode->node_num
            //cout<<pnode->key_num<<" ";
            setBlackKey(pnode);
            setBigKey(pnode);
            setPrefixAggregate(pnode);
            //if(pnode->prefixlength == 17)
            //cout<<"Line 910, prefix error!"<<endl;
            vector<char> word;
            vector<string> other_keys;
            vector<int> other_keyactions;
            printOtherKey(pnode, word, other_keys, other_keyactions);

        }

        /*else if(pnode->key_weight/pnode->weight >= sBIGKEYTHLD && isInit == 0)
        {
            setPrefixAggregate(pnode);
            setBigKey(pnode);
            setPrefixAggregate(pnode);
            //cout<<"weight    "<<(pnode->key_weight/pnode->weight)<<endl;
        }*/
        else if((float(pnode->key_num)/float(pnode->leaf_num)) >= KEYTHLD&& isInit == 0)
        {
            //cout<<(float(pnode->key_num)/float(pnode->leaf_num))<<endl;
            //if(pnode->bignk <= pnode->key_num/2)
            {
                setBigKey(pnode);
                setPrefixAggregate(pnode);
                //cout<<"num"<<endl;

            }

        }
        else
        {


            arregatePrefix8(pnode->left,sBIGKEYTHLD,aggrprefixlength,isInit);
            arregatePrefix8(pnode->right,sBIGKEYTHLD,aggrprefixlength,isInit);

        }

    }
    else
    {

        {
            arregatePrefix8(pnode->left,sBIGKEYTHLD,aggrprefixlength,isInit);
            arregatePrefix8(pnode->right,sBIGKEYTHLD,aggrprefixlength,isInit);
        }
    }

}


/** aggregate keys to its prefix,
find keys covered by aggregated key, and set them to invalid key,
find big nonkey, and set them to black keys
*/
void Trie::setBigKey(Node *pnode)
{
    if( pnode==NULL)
    {
        return;
    }
    if(pnode->wordMarker())
    {
        /*if(pnode->keytype == isnonkey)
        {
            if(pnode->weight > BIGNONKEYTHLD)
            {
                pnode->keytype = isblackkey;
                //g_blackkey_weight_sum += pnode->weight;
                //g_vblackkey_weight_sum[maction] += pnode->weight;
            }
            else
            {
                nonkey_weight_sum += pnode->weight;
                //g_vnonkey_weight_sum[maction] += pnode->weight;
            }

        }
        else*/ if(pnode->keytype == iskey)
        {
            pnode->keytype = invalidkey;
        }


    }

    else
    {
        setBigKey(pnode->left);
        setBigKey(pnode->right);
    }
}


/** aggregate keys to its prefix,
find keys covered by aggregated key, and set them to invalid key,
find big nonkey, and set them to black keys
*/
void Trie::setBlackKey(Node *pnode)
{
    if( pnode==NULL)
    {
        return;
    }

    if(pnode->keytype == isblackkey)
    {
        return;
    }

    if(pnode->keytype == isnonkey)
    {
        pnode->keytype = isblackkey;
        //pnode->setWordMarker();

        return;
    }

    else
    {
        setBlackKey(pnode->left);
        setBlackKey(pnode->right);
    }
}

/**
set aggregated keys
*/
bool Trie::setPrefixAggregate(Node *pprefix)
{
    pprefix->keytype = isaggregatekey;

    //cout<<"aggr: "<<pprefix->prefixlength<<endl;

    if(pprefix->prefixlength == 2)
        cout<<"************** Line 1007 set Aggregate: "<<pprefix->leaf_num<<endl;
    return true;
}


/** set the aggregated keys covered by another aggregated key, and set them to invalid keys
*/
void Trie::setAgtInvalid(Node *pnode)
{
    if( pnode==NULL)
    {
        return;
    }

    if (pnode->wordMarker())
    {
        return;
    }
    //if (pnode->keytype == isaggregatekey)


    else
    {
        setAgtInvalid(pnode->left);
        setAgtInvalid(pnode->right);

    }
    {
        pnode->keytype = invalid;

    }

}
/** set the aggregated keys covered by another aggregated key, and set them to invalid keys
*/
bool Trie::setAgtInvalidTrie(Node *pnode)
{
    if( pnode==NULL)
    {
        return false;
    }
    if (pnode->keytype == isaggregatekey)
    {
        if( pnode==NULL)
        {
            return false;
        }
        else if (pnode->wordMarker())
        {
            return false;
        }

        else
        {
            setAgtInvalid(pnode->left);
            setAgtInvalid(pnode->right);

        }

    }
    else
    {

        if( pnode==NULL)
        {
            return false;
        }
        else if (pnode->wordMarker())
        {
            return false;
        }

        else
        {
            setAgtInvalidTrie(pnode->left);
            setAgtInvalidTrie(pnode->right);

        }
    }
    return true;
}

bool Trie::recoverTrie(Node *pnode, size_t& aggrCount)
{
    if( pnode==NULL)
    {
        return false;
    }
    if(pnode->keytype == isaggregatekey)
    {
        pnode->keytype = iskey;

    }
    if(pnode->wordMarker())
    {
        if(pnode->keytype == isblackkey)
        {
            pnode->keytype = isnonkey;

        }
        else if(pnode->keytype == invalidkey)
        {
            pnode->keytype = iskey;
            aggrCount++;
        }


    }
    else
    {
        recoverTrie(pnode->left, aggrCount);
        recoverTrie(pnode->right, aggrCount);
    }
    return true;
}

bool Trie::queryAggrTrie(Node *pnode, size_t& aggrCount)
{
    if( pnode==NULL)
    {
        return false;
    }
    if(pnode->keytype == isaggregatekey)
    {
        //pnode->keytype = iskey;

    }
    if(pnode->wordMarker())
    {
        if(pnode->keytype == isblackkey)
        {
            //pnode->keytype = isnonkey;

        }
        else if(pnode->keytype == invalidkey)
        {
            //pnode->keytype = iskey;
            aggrCount++;
        }


    }
    else
    {
        queryAggrTrie(pnode->left, aggrCount);
        queryAggrTrie(pnode->right, aggrCount);
    }
    return true;
}

void Trie::deleteChild(Node *pnode)
{
    if(pnode == NULL)
        return;

    deleteChild(pnode->left);
    deleteChild(pnode->right);
    delete pnode;
}

void Trie::computeNodeNum(Node *pnode)
{
    if(pnode == NULL)
        return;

    if(isLeaf(pnode))
    {
        if(pnode->keytype == iskey )
            pnode->node_num = 1;
        else
            pnode->node_num = 0;
    }
    else if(pnode->left != NULL  && (pnode->left->keytype == iskey) && pnode->right == NULL)
    {
        pnode->node_num = 1;
    }
    else if(pnode->right != NULL  && pnode->right->keytype == iskey && pnode->left == NULL)
    {
        pnode->node_num = 1;
    }
    else if(pnode->right != NULL  && pnode->right->keytype == iskey && pnode->left != NULL && pnode->left->keytype == iskey)
    {
        pnode->node_num = 2;
    }

    computeNodeNum(pnode->left);
    computeNodeNum(pnode->right);
}

// compute key numbers per action
void Trie::compute_action_kn(Node* pnode, vector<size_t> &key_num_vec, vector<int>& actions)
{
	if(pnode == NULL) return;

	for (int ai = 0; ai < key_num_vec.size(); ai++)
	{
		if(pnode->action == actions[ai])
		{
			key_num_vec[ai] += 1;
		}
	}
	
	compute_action_kn(pnode->left, key_num_vec, actions);
	compute_action_kn(pnode->right, key_num_vec, actions);
}
void set_other_keys()
{

}
// find dominant action
void Trie::find_domi_action(Node* pnode, vector<int>& actions)
{
	if(pnode == NULL) return;
	
	vector<size_t> key_num_vec;
	key_num_vec.assign(actions.size(), 0);
	compute_action_kn(pnode, key_num_vec, actions);

	
	// if dominant action found, aggregate the node
	if(isDominate(key_num_vec) && pnode->prefixlength >= 8)
	{
		/*vector<size_t>::iterator it;
		for(it = key_num_vec.begin(); it!= key_num_vec.end(); it++)
		{
			cout<<*it<<" ";
		}
		cout<<endl;*/
		// set domi_action
		
		//arregatePrefix8(pnode,BIGKEYTHLD, 1);	
		//cout<<"computeKeyNum."<<endl;
		computeKeyNum(pnode,iskey);

		//cout<<pnode->key_num<<endl;
		//cout<<"computeNodeNum."<<endl;
		computeNodeNum(pnode);
	
		//cout<<"arregatePrefix8."<<endl;
		float weight_threshold = 0.00001;
		switch(maction)
		{
		case 0:
			{
				//weight_threshold = 0.0000173; // domi_threshold = 0.5
				//weight_threshold = 0.0000174; // domi_threshold = 1, no noise
				weight_threshold = 0.000015;
				//weight_threshold = 0.00001;
				break;
			}
		case 1:
			{
				//weight_threshold = 0.0000178;
				//weight_threshold = 0.0000175;
				weight_threshold = 0.000015;
				//weight_threshold = 0.00001;
				break;
			}
		case 2:
			{
				//weight_threshold = 0.0000173;
				//weight_threshold = 0.0000174;
				weight_threshold = 0.000015;
				//weight_threshold = 0.00001;
				break;
			}
		case 3:
			{
				//weight_threshold = 0.0000172;
				//weight_threshold = 0.0000174;
				weight_threshold = 0.000015;
				//weight_threshold = 0.000095;
				break;
			}
		}
		weight_threshold = 0.00001;
		int prefixlength = 1755;
		bool isInit = 1;
		
		arregatePrefix(pnode, weight_threshold,isInit);
	}
	else
	{
		find_domi_action(pnode->left, actions);
		find_domi_action(pnode->right, actions);
	}
}

const double domi_threshold = 0.5;
bool Trie::isDominate(vector<size_t> &key_num_vec)
{
	int domi_index = distance(key_num_vec.begin(), max_element(key_num_vec.begin(), key_num_vec.end()));
	size_t max_sum = key_num_vec[domi_index];
	
	// sum of the  total number of nodes of all actions
	size_t sum_num = accumulate(key_num_vec.begin(), key_num_vec.end(),0);

	
	// decide dominant action
	if(max_sum >= domi_threshold*sum_num && sum_num > 1)
	{
		//cout<<max_sum<<" "<<sum_num<<" ";
		// set dominant actions
		dominant_action = domi_index;
		maction = domi_index;
		//cout<<"* dominant index: "<<domi_index<<" domi action: "<<dominant_action<<endl;
		return true;	
	}
	
	return false;
}

void Trie::aggregate_output(Node* pnode, size_t &countkey,size_t &countaggregatekey, size_t &countblackkey,
                   size_t &countorikey, vector<string> &keys,
                   vector<int> &keyaction,vector<string> &other_keys,
                   vector<int> &other_keyaction,vector<string> &blackkey,
                   vector<int> &blackkeyPrefixes, vector<string> &aggregatekey, vector<int>& aggr_actions, bool isPrint)
{
	
	cout<<"nodeCount....."<<endl;
    nodeCount(root,countkey,countaggregatekey,countblackkey,countorikey);

	cout<<"printNodes."<<endl;
	
    // print key and blackkey to file
    vector<char> word;
    if(isPrint)
         printNodes(root,word,keys,keyaction,blackkey,blackkeyPrefixes,aggregatekey,aggr_actions, other_keys, other_keyaction);

}



