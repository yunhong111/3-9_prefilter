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
typedef vector<string> strings;
typedef vector<float> floats;
typedef vector<size_t> size_ts;

const float load_factor = 0.9;
const int per_key_prefilter = 40;

const int actionSize = 4;

// packet generation rate: pps
const float rateParameter0 = 297298.0;

// time
double runTime = 0.0;

// ---------------------------------------------------
/*define mask*/
vector<size_t> mask;

double nextTime(double rateParameter)
{
    double nTime = -log(1.0f - (double) rand() / double(RAND_MAX + 1.0)) / rateParameter;

    while(nTime == INFINITY)
    {
        double nTime = -log(1.0f - (double) rand() / (RAND_MAX + 1.0)) / rateParameter;
        cout<<"* nextTime: INFINITY!"<<endl;
        exit(0);
    }
    return nTime;
}

void def_mask()
{
    size_t maskIP;
    mask.clear();
    cout<<"* Compute prefixes main ... ..."<<endl<<endl;;
    for(int i = 8; i <= 32; i++)
    {
        maskIP = (size_t(pow(2,i))-1)<<(32-i);
        mask.push_back(maskIP);
    }
}

void to_file(char* file_name, vector<string> &keys, vector<int> &keyaction)
{
	ofstream outfile_key(file_name);
	//ofstream outfile_otherkey("../test/other_keys.csv");

	//vector<string>::iterator it;
	for(int i = 0; i < keys.size(); i++)
	{
		outfile_key<<keys[i]<<" "<<keyaction[i]<<endl;
	}

	outfile_key.close();
}

void prefixNum(vector<int> &keyprefix, vector<int> &UniquePrefix)
{
    vector<int > keyprefixSort = keyprefix;
    sort(keyprefixSort.begin(),keyprefixSort.end());
    vector<int>::iterator it;
    it = unique (keyprefixSort.begin(), keyprefixSort.end());
    keyprefixSort.resize( distance(keyprefixSort.begin(),it) );
    UniquePrefix.clear();
    UniquePrefix = keyprefixSort;

	cout<<"*****************************************************"<<endl;
    for(int i = 0; i < UniquePrefix.size(); i++)
        cout<<"prefix: "<<UniquePrefix[i]<<"    ";
    cout<<"*****************************************************"<<endl;
}

// assign random actions to a key
int assign_rand_action(string& key, ints uni_actions)
{
	uint32_t key_int = parseIPV4string(key.c_str());
	
	int action_size = uni_actions.size();

	//cout<<"* Assign actions ... ..."<<endl<<endl;
    vector<string> actionBound;
    actionBound.push_back("64.0.0.0");// = {"25.0.0.0","32.0.0.0"};
    actionBound.push_back("128.0.0.0");
    actionBound.push_back("192.0.0.0");// = {"25.0.0.0","32.0.0.0"};
    actionBound.push_back("255.255.255.255");

    int action_tmp = 0;

	for (int ai = 0; ai < actionSize; ai ++)
	{
		if(key_int<parseIPV4string(actionBound[ai].c_str()))
		{
			action_tmp = uni_actions[ai];
			if(rand()%100<30)
			{
				action_tmp = uni_actions[rand()%action_size];
			}

			break;
		}
	}
    
	return action_tmp;
}


// add keys to trie
void add_keys(char* infile_name, Trie* trie, ints uni_actions, ints& key_prefixes, strings& keys, ints& key_actions)
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
	int key_action;
	
	size_t key_num = 0;

	//ofstream outfile("../test/key_actions_0_3");
	
	while(infile >> key_prefix >> key_ipv4>>key_action)
	{
		int key_action = assign_rand_action(key_ipv4, uni_actions);
		
		// to binary string
		string key_binary = parseIPV42bin(key_ipv4.c_str());
		trie->addWord(key_binary,0, iskey, key_prefix, key_action);
		//outfile<<key_prefix<<" "<<key_ipv4<<" "<<key_action<<endl;

		key_prefixes.push_back(key_prefix);
		keys.push_back(key_ipv4);
		key_actions.push_back(key_action);
	}
	//outfile.close();
}

float get_total_memory()
{
	// KB
	float storage = 360;
	
	// convert to bits
	storage *= 8.0*1024;

	cout<<"total_memory: "<<storage<<endl;
	return storage;
}

float get_total_prefilter_memory()
{
	size_t slot_num = 20000;
	float storage = slot_num*per_key_prefilter/load_factor;

	cout<<"prefilter+black memory: "<<storage<<endl;
	return storage;
}

// prefilter is a cuckoo table with size of
// table: key+action+1 bit flag = (32+5) + 2 + 1 = 40
// load factor = 
float get_prefilter_size(size_t other_key_size)
{
	float prefilter_size = other_key_size;

	// get prefilter size(bits) = slots*per_slot_size
	float storage = prefilter_size*per_key_prefilter/load_factor;

	cout<<"get_prefilter_size: "<<storage<<endl;
	return storage;
}

float get_black_memory(size_t other_key_size)
{
	float prefilter_memory = get_prefilter_size(other_key_size);

	float total_pre_memory = get_total_prefilter_memory();

	float black_memory = total_pre_memory - prefilter_memory;

	if(black_memory <= 0)
	{
		cout<<"get_black_memory::error::total prefilter memory is too small!";
		exit(0);
	}

	cout<<"black_memory: "<<black_memory<<endl;
	return black_memory;
}

//
float get_act_filter_size()
{
	float total_memory = get_total_memory();

	float total_pre_memory = get_total_prefilter_memory();

	float act_filter_memory = total_memory - total_pre_memory;

	if(act_filter_memory <= 0)
	{
		cout<<"get_act_filter_size::error::total memory is too small!";
		exit(0);
	}

	cout<<"act_filter_memory: "<<act_filter_memory<<endl;
	return act_filter_memory;
}

// 1 bit flag, 2 bits actions, f bits fingerprint for filters
void init_act_filter(CuckooFilter& cuckooFilter, vector<string> &keys, int& finger)
{
	float storage = get_act_filter_size(); // bits
	
    //-----------------------------------------------------------
    // Parameters for cuckoo filter
    float loadFactor = load_factor;
    int slotNo = 4;
    size_t keySize = keys.size();
    size_t bucketNo = size_t(keySize/(loadFactor*slotNo))+1;
    int fingerprint = storage*loadFactor/(keys.size())-3;
    finger = fingerprint;
    long maxNumKicks = 1500;
    cout<<"* Fingerprint length: "<<fingerprint<<endl;
    //mL0 =vector<vector<string> > (bucketNo, vector<string>(slotNo, " "));

    // --------------------------------------------------------
    //init cuckoo filter
    cuckooFilter.ClearTable();
    cuckooFilter.cuckooFilterInit(bucketNo,fingerprint,slotNo,
                                       maxNumKicks); 
}

void add_act_filter(CuckooFilter& cuckooFilter, vector<string> &keys, vector<int> &keyActions)
{
	int finger;
	init_act_filter(cuckooFilter, keys, finger);
                
    // define variables
    cout<<"* Cuckoo filter adding ..."<<endl;
    bool flagAdd;
    int countFail = 0;
    string str;
    size_t keySize = keys.size();

    for(size_t i = 0; i < keySize; i++)
    {
        str = keys[i];
        //add keys to cuckoo filter
        flagAdd = cuckooFilter.AddKey(str,(keyActions[i]));
        if(flagAdd == 0)
        {
            countFail++;
            cout << "* 664 Cuckoo filter flag_add fail"<<endl;
            cout<<"* Order: "<<i<<"  ";
            break;
        }

    }
    cout<<"* Line 670 Count fail num: "<<countFail<<endl;
}

void init_prefilter1(CuckooTable& cuckooPretable, size_t other_key_size)
{
	float prefilter_memory= get_prefilter_size(other_key_size);

	// prefilter table slot number
	size_t prefilter_size = prefilter_memory*load_factor/per_key_prefilter;
	cout<<"prefilter slots: "<<prefilter_size<<endl;
	
	// init prefilter table
	float a = load_factor;
	int f = 12;
	int bc = 4;
	long MaxNumKicks = 1000;
    long m = prefilter_size/(a*bc)+1;
    
    cuckooPretable.ClearTable();
    cuckooPretable.CuckooTableInit(m,f,bc,
                                        MaxNumKicks);
}

// other_key: ipv4/prefix
void add_prefilter1(CuckooTable& cuckooPretable, strings& other_keys, ints& other_keyactions)
{
	init_prefilter1(cuckooPretable, other_keys.size());

	for(size_t i = 0; i < other_keys.size(); i++)
	{
		bool addFlag = cuckooPretable.AddKeyPrefix(other_keys[i],32, other_keyactions[i]);
		if(!addFlag)
		{
			cout<<"2 add fail..."<<endl;
		}
	}
}

void init_black_table(CuckooTable& cuckooBlackKeyTable, size_t other_key_size)
{
	float black_memory = get_black_memory(other_key_size);

	// black table slot number
	size_t black_size = black_memory*load_factor/per_key_prefilter;
	cout<<"black table slots: "<<black_size<<endl;
	
	// init black table
	float a = load_factor;
	int f = 12;
	int bc = 4;
	long MaxNumKicks = 1000;
    long m = black_size/(a*bc)+1;
    
    cuckooBlackKeyTable.ClearTable();
    cuckooBlackKeyTable.CuckooTableInit(m,f,bc,
                                        MaxNumKicks);
}

void clear_black_table()
{

}

void update_black_table()
{

}

void aggr(Trie* trie, ints uni_actions, CuckooTable& cuckooPretable, CuckooTable& cuckooBlackKeyTable, CuckooFilter& cuckooFilter, ints& uni_aggr_prefix)
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
    vector<int> aggr_actions;
    
    int prefixlength;
    bool isPrint = 1; 
    bool isInit;
                   
	trie->find_domi_action(trie->root, uni_actions);
	
	trie->aggregate_output(trie->root,  countkey, countaggregatekey,  countblackkey,\
                    countorikey, keys,keyaction,other_keys, other_keyaction,blackkey,\
                   blackkeyPrefixes, aggregatekey, aggr_actions, isPrint);
             
    cout<<keys.size()<<" "<<other_keys.size()<<" "<<blackkey.size()<<endl;

    // output to file
    char* key_file_name = (char* )("../test/key_out_0_3_noise");
    to_file(key_file_name, keys,keyaction);

    char* otherkey_file_name = (char* )("../test/other_key_out_0_3_noise");
    to_file(otherkey_file_name, other_keys,other_keyaction);

    char* aggrkey_file_name = (char* )("../test/aggr_key_out_0_3_noise");
    to_file(aggrkey_file_name, aggregatekey,aggr_actions);

    // add to prefilter
    add_prefilter1(cuckooPretable, other_keys, other_keyaction);
	
    // add to actual filter
    add_act_filter(cuckooFilter, keys, keyaction);

    // init blacktable
	init_black_table(cuckooBlackKeyTable, other_keys.size());

    // get key prefixes
    // Compute unique prefix length
    vector<int> keyPrefixes;
    for(size_t i = 0; i< keys.size(); i++)
    {
        size_t found = keys[i].find('/');
        string str = keys[i].substr(0,found);
        string prefixStr = keys[i].substr(found+1, keys[i].size()-found);
        keyPrefixes.push_back(str2num(prefixStr));
    }

    // unique prefix number
    prefixNum(keyPrefixes, uni_aggr_prefix);
}

// 1 bit flag, 2 bits actions, f bits fingerprint for filters
int init_ori_filter(CuckooFilter& cuckooFilterInit0, vector<string> &keys,vector<int> &keyPrefixes,
                vector<int> &keyActions, int& finger)
{
	float storage = get_total_memory();
    //-----------------------------------------------------------
    // Parameters for cuckoo filter
    float loadFactor = load_factor;
    int slotNo = 4;
    size_t keySize = keys.size();
    size_t bucketNo = size_t(keySize/(loadFactor*slotNo))+1;
    int fingerprint = storage*loadFactor/(keys.size())-3;
    finger = fingerprint;
    long maxNumKicks = 1500;
    cout<<"* ori fingerprint length: "<<fingerprint<<endl;
    //mL0 =vector<vector<string> > (bucketNo, vector<string>(slotNo, " "));

    // --------------------------------------------------------
    //init cuckoo filter
    cuckooFilterInit0.ClearTable();
    cuckooFilterInit0.cuckooFilterInit(bucketNo,fingerprint,slotNo,
                                       maxNumKicks);

    return 1;

}

void add_ori_filter(CuckooFilter& cuckooFilterInit0, ints& key_prefixes,strings& keys,ints& key_actions)
{
	//float storage = 360;
	int finger = 12;
	init_ori_filter(cuckooFilterInit0, keys,key_prefixes,key_actions, finger);
	
	// define variables
    cout<<"* Cuckoo filter adding ..."<<endl;
    bool flagAdd;
    int countFail = 0;
    string str;
    size_t keySize = keys.size();

    for(size_t i = 0; i < keySize; i++)
    {
        str = keys[i] + '/'+num2str(key_prefixes[i]);
        //add keys to cuckoo filter
        flagAdd = cuckooFilterInit0.AddKey(str,(key_actions[i]));
        if(flagAdd == 0)
        {
            countFail++;
            cout << "* 429 Cuckoo filter flag_add fail"<<endl;
            cout<<"* Order: "<<i<<"  ";
            break;
        }

    }
    //cout<<"* Line 435 Count fail num: "<<countFail<<endl;
}

void init_table(CuckooTable& cuckooTableKey, size_t key_size)
{
	cout<<"* Init cuckoo table ... ..."<<endl<<endl;
    float a;
    int f,bc;
    a = load_factor;
    bc = 4;
    long m = long(key_size/(a*bc))+1;
    f = 12;
    long MaxNumKicks = 1000;
    cuckooTableKey.ClearTable();
    cuckooTableKey.CuckooTableInit(m,f,bc,MaxNumKicks);
}
void add_ori_table(CuckooTable& cuckooTableKey, ints& key_prefixes,strings& keys,ints& key_actions)
{
	init_table(cuckooTableKey, keys.size());
	
	cout<<"* Add key to cuckoo table ... ..."<<endl;
    bool isAddTable;
    for (int i = 0; i < keys.size(); i++)
    {
        isAddTable = cuckooTableKey.AddKeyPrefix(keys[i],key_prefixes[i],key_actions[i]);

        if(isAddTable == 0)
        {
            cout<<"* Flag_add fail"<<endl;
            cout<<"* Order: "<<i<<"  ";
        }

    }
}
bool readFile0(ifstream& infile, vector<string> &flow, vector<size_t> &flow_cnt, size_t readNum, bool& isEndFlag)
{
    Trie *bTrie = new Trie();
    uint32_t flowInt;
    size_t flowNo;
    size_t in_num = 0;

    double timeInv = 0.0f;
    double nTime = 0.0f;

    while((infile >> flowInt >>flowNo) && timeInv < 1.0f/*&& (in_num < readNum)*/)
    {

        bTrie->addWordCount(DecToBin(flowInt),32, isnonkey, flowNo);
        in_num ++;

        // generate next time
        nTime = nextTime(rateParameter0);
        timeInv += nTime;

        if(nTime< 0)
        {
            cout<<"* readFile0: timeInv < 0. wrong!"<<endl;
            exit(0);
        }
        //cout<<"* timeInv: "<<endl;
        //if(in_num%1000000 == 0)
            //cout<<"loading file ...."<<in_num<<"  ";
    }

    // run time
    runTime += timeInv;
    if(runTime >= INFINITY)
    {
        cout<<"* readFile0:runTime: "<<runTime<<" timeInv: "<<timeInv<<" nTime: "<<nTime<<endl;
        exit(0);
    }
    //cout<<"* timeInv: "<<endl;

    vector<char> word;
    vector<int> flowActions;
    bTrie->getLeaf(bTrie->root,word,flow,flow_cnt,flowActions);


    if(timeInv < 1.0f/*in_num < readNum*/)
    {
        isEndFlag = 1;
        cout<<"* readFile0:timeInv: "<<timeInv<<endl;
    }

    //cout<<"* Flow size: "<<flow.size()<<endl;
	bTrie->deleteChild(bTrie->root);
    delete bTrie;
    return true;
}

void lookup_table(CuckooTable& cuckooTableKey, uint32_t ip, size_t flowNoInt, ints& uniquePrefix, bool& flag_lookkey)
{
	//if (flag_lookblack == 0) // not a blackkey
	{
		// look up key
		for(int mi = uniquePrefix.size()-1; mi >=0; mi--)
		{
			uint32_t subIP = ip & mask[uniquePrefix[mi]-8];
			string flowstr = parsedec2IPV4(subIP);
			int prefix = uniquePrefix[mi];

			int iflowaction;
			flag_lookkey = cuckooTableKey.LookUpKeyActionCount(flowstr,
															   prefix,iflowaction,flowNoInt);
			if (flag_lookkey)
			{
				//keySumLocal += (flowNoInt);
				for(int ai = 0; ai <actionSize; ai++)
				{
					if(iflowaction == ai)
					{
						//#pragma omp atomic
						//keySumsLocal[ai] += (flowNoInt);
					}

				}
				break;
			}
		}
	}
}

void lookup_black(CuckooTable& cuckooBlackKeyTable, uint32_t ip, size_t flowNoInt, bool& flag_lookblack)
{
	int iflowaction;
	int mi = 24;
	//for(int mi = 0; mi <= 32-8; mi++)
	{
		uint32_t subIP = ip & mask[mi]; // mask
		string flowstr = parsedec2IPV4(ip);//s.str();
		int prefix = mi+8;

		flag_lookblack = cuckooBlackKeyTable.LookUpKeyAction(flowstr,prefix,iflowaction);
		if (flag_lookblack)
		{
			//countBlack += flowNoInt;
			//break;
		}
	}
}

void lookup_prefilter(CuckooTable& cuckooPretable, uint32_t ip, size_t flowNoInt, ints& uniquePrefix, bool& flag_prelook_key)
{
	//if (flag_lookblack == 0) // not a blackkey
	{
		// look up key
		for(int mi = uniquePrefix.size()-1; mi >=0; mi--)
		{
			uint32_t subIP = ip & mask[uniquePrefix[mi]-8];
			string flowstr = parsedec2IPV4(subIP);
			int prefix = uniquePrefix[mi];

			int iflowaction;
			flag_prelook_key = cuckooPretable.LookUpKeyActionCount(flowstr,
															   prefix,iflowaction,flowNoInt);
			if (flag_prelook_key)
			{
				//keySumLocal += (flowNoInt);
				for(int ai = 0; ai <actionSize; ai++)
				{
					if(iflowaction == ai)
					{
						//keySumsLocal[ai] += (flowNoInt);
					}
				}
				break;

			}
		}
	}
}

void lookup_ori_filter(CuckooFilter& cuckooFilterInit0, uint32_t ip, size_t flowNoInt, ints&uniquePrefix)
{
	// lookup key from cuckoo filterInit0
	vector<int> iactions;
	for(int mi = 0; mi < uniquePrefix.size(); mi++)
	{
		uint32_t subIP = ip & mask[uniquePrefix[mi]-8];
		string flowstr = parsedec2IPV4(subIP)+"/"+num2str(uniquePrefix[mi]);
		iactions.clear();
		int flag_look0 =cuckooFilterInit0.LookUpKeyActions(flowstr,iactions);
		if (flag_look0)
		{

			//countNum0Local += flag_look0;

			//countIP0Local += flag_look0*(flowNoInt);

		}

	}
}

void lookup_act_filter(CuckooFilter& cuckooFilter, Trie* trie, uint32_t ip, size_t floaNoInt, bool& flag_lookkey, ints& uniqueAggPrefix)
{
	// lookup key from cuckoo filter
	vector<int> iactions;

	for(int mi = 0; mi < uniqueAggPrefix.size(); mi++)
	{
		bool overbigFlag = 0;
		uint32_t subIP = ip & mask[uniqueAggPrefix[mi]-8];
		string flowstr = parsedec2IPV4(subIP)+"/"+num2str(uniqueAggPrefix[mi]);
		iactions.clear();
		
		int flag_look = cuckooFilter.LookUpKeyActions(flowstr,iactions);

		if (flag_look != 0)
		{
			//countNumLocal += flag_look;
			//countIPLocal += flag_look*(flowNoInt);

			// each action lookups
			for(int ai = 0; ai <actionSize; ai++)
			{
				for(int li = 0; li < iactions.size(); li++)
				{
					if(iactions[li] == ai)
					{
						//countIPsLocal[ai] += (flowNoInt);
					}

				}
			}

			// overselection count
			if(!flag_lookkey && !overbigFlag)
			{
				overbigFlag = 1;
				//#pragma omp critical
				{
					//trie->addWordCountNum(DecToBin(flowInt),32, isnonkey, iactions[0], countIPLocal);
				}

			}
		}
	}
}

void check_member(ifstream& infile, Trie* trie, CuckooFilter& cuckooFilterInit0, CuckooTable& cuckooBlackKeyTable,
CuckooTable& cuckooTableKey, CuckooTable& cuckooPretable, CuckooFilter& cuckooFilter,
ints& uni_ori_prefix, ints& uni_aggr_prefix, bool& isEndFlag)
{
	// read file
    strings flows;
    size_ts flowNos;
    size_t readNum = 10;
    readFile0(infile, flows, flowNos, readNum, isEndFlag);
    size_t flowNum = flows.size();
	cout<<"* flow No: "<<flowNum<<endl;

	for(size_t ei = 0; ei < flowNum; ei++)
	{
        size_t flowNoInt = flowNos[ei];
		uint32_t ip = parseIPV4string(flows[ei].c_str()); //convert IP to int
		
		// lookup in ori filter
		lookup_ori_filter(cuckooFilterInit0, ip, flowNoInt, uni_ori_prefix);
		
		// check blacklist
		bool flag_lookblack = 0;
		lookup_black(cuckooBlackKeyTable, ip, flowNoInt,flag_lookblack);

		if(!flag_lookblack)
		{
			// check table
			bool flag_lookkey = 0;
			lookup_table(cuckooTableKey, ip, flowNoInt, uni_ori_prefix, flag_lookkey);

			// check prefilter
			bool flag_prefilter;
			lookup_prefilter(cuckooPretable, ip, flowNoInt, uni_aggr_prefix, flag_prefilter);

			if(!flag_prefilter)
			{
				// check actual filter
				lookup_act_filter(cuckooFilter, trie,  ip, flowNoInt, flag_lookkey, uni_aggr_prefix);
			}
		}
	}
}

void simul_trace(char** argv, CuckooFilter& cuckooFilterInit0, CuckooTable& cuckooBlackKeyTable,
CuckooTable& cuckooTableKey, CuckooTable& cuckooPretable, CuckooFilter& cuckooFilter,
ints& uni_ori_prefix, ints& uni_aggr_prefix)
{
	// ---------------------------
    // Define a trie
    Trie *trie;            // define tree
    trie = new Trie();            // define tree
    
	// File name for caida trace
    const char * fp[] = {"mapped-caida-1","mapped-caida-6","mapped-caida-11",
                         "mapped-caida-16","mapped-caida-21", "mapped-caida-26",
                         "mapped-caida-31", "mapped-caida-36","mapped-caida-41",
                         "mapped-caida-46","mapped-caida-51","mapped-caida-56"
                        };
                      
                        
    // load packets from file
    for (int fi = 0; fi < 12; fi++)
    {
        // --------------------------
        // Open trace file
        string pathname = fp[fi];
        std::string inFileName = argv[2];
        inFileName += pathname;

        ifstream infile;
        infile.open(inFileName.c_str());
        cout<<inFileName.c_str()<<endl;
        cout<<inFileName<<endl;
        if(!infile)
            std::cout << "* TestIP File Error " << std::endl;

        bool isEndFlag = 0;
        
        while(!isEndFlag )
        {
			check_member(infile, trie, cuckooFilterInit0, cuckooBlackKeyTable,
			cuckooTableKey, cuckooPretable, cuckooFilter,
			uni_ori_prefix, uni_aggr_prefix, isEndFlag);
		}
		infile.close();

	}
   
}

void feedback_recv()
{
	
}

int main(int argc, char** argv)
{
	// initialize rand 
	srand(time(NULL));

	// define mask
	def_mask();

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
	strings keys;
	ints key_prefixes;
	ints key_actions;
	
	add_keys(infile_name, trie, uni_actions, key_prefixes, keys, key_actions);
	
	// aggregate the keys and output the aggregated keys and add keys to filter
	ints uni_aggr_prefix;
	CuckooTable cuckooPretable;
	CuckooFilter cuckooFilter;
	CuckooTable cuckooBlackKeyTable;
	aggr(trie, uni_actions, cuckooPretable, cuckooBlackKeyTable, cuckooFilter, uni_aggr_prefix);
	
	// add original keys to table
	CuckooTable cuckooTableKey;
	add_ori_table(cuckooTableKey, key_prefixes, keys, key_actions);

	ints uni_ori_prefix;
	prefixNum(key_prefixes, uni_ori_prefix);

	// add original keys to filter

	CuckooFilter cuckooFilterInit0;
	add_ori_filter(cuckooFilterInit0, key_prefixes, keys, key_actions);

	exit(0);
	// -------------------------------------------------------------
	// check membership
	simul_trace(argv, cuckooFilterInit0, cuckooBlackKeyTable, cuckooTableKey,
	cuckooPretable, cuckooFilter,uni_ori_prefix, uni_aggr_prefix);

	// delete the trie
	delete trie;
}
