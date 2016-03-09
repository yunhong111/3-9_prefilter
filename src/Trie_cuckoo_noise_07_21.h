/**
trie_cuckoo_noise_07_21.cpp
create by: Yunhong
create time: 07/21/2015
*/

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <sys/time.h>
#include <arpa/inet.h>
#include "aggregation_add_cuckoo.h"
#include "RL.h"
# include <time.h>

using namespace std;

void feedbackBlackkey(vector<string>& overBigKeys);

void feedbackBlackkey1(strings& overBigKeys);

bool readFile0(ifstream& infile, vector<string> &flow, vector<size_t> &flow_cnt, size_t readNum, bool& isEndFlag);

void initRLearn(RLearn* rLearn);

void updateBlacklist(vector<string>& overBigKeys, vector<int>& overActions, RLearn* rLearn, int actionSeq
, vector<string>& blackkeyPres, vector<int>& blackActionPres, ofstream& blackKeyFileOut, size_t& slotNum);

void feedbackBlackkeyRL(vector<string>& overBigKeys, vector<int>& overActions, RLearn* rLearn[], int actionSize,
size_ts& slotNums, size_t line);

void printQList(RLearn* rLearn);

void selectAction(RLearn* rLearn[], int actionSize, size_ts& slotNums);

void findMax(vector<QSum>& qSums, size_ts& slotNums, int actionSize );

void printVec(vector<size_t>& vec);

double nextTime(double rateParameter);


