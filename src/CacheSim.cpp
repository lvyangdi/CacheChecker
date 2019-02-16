#include "CacheSim.h"

CacheSim::CacheSim(int nodeNum , string type, bool showtrace, bool showset):
	nodeNum_(nodeNum), type_(type), totalActions_(0),
	showTrace_(showtrace), dumpHashSet_(showset)
{
	// Initialize state as all Is
	initialState_.assign(nodeNum, 'I');
	// Add initialState to visited nodes
	visited.insert(initialState_);
	// Make current inital state
	states_ = initialState_;
}

/**
 * From current state (states_), find next state
 */
void CacheSim::nextAction(int nAction)
{
	string next = action(type_, states_, nAction);
	if (!next.compare("Invalid")) {
		return;
	}
	visited.insert(next);
	
	// Create test suit
	string nedge = states_ + "->" + next + to_string(nAction);
	if (!edges.count(nedge)){
		edges.insert(nedge);
		if (dumpHashSet_)
			cout << nedge << endl;
	}

	if(showTrace_)	cout << nedge << endl;

	states_ = next;
	totalActions_ ++;

	// Dump every 1000 actions
	if(totalActions_ % 1000 == 0)
	{
		dumpInfo += to_string(totalActions_) + " " + to_string(visited.size()) +" " + to_string(edges.size()) + "\n";
	}

}

/**
 * summary all the info
 */
void CacheSim::summary()
{
	cout<<"Total states: " << visited.size() << endl;
	cout<<"Total edges: " << edges.size() << endl;
	cout<<"Total Actions: " << totalActions_ << endl;
	dumpToFile("ex_data.dat",dumpInfo);
}

/**
 * dump edge to screen
 */
void CacheSim::dump(unordered_set<string> edge)
{
	for (auto e : edge)
		cout << e << endl;
}

/**
 * dump data to file
 */
void CacheSim::dumpToFile(string file, string data) {
	ofstream myfile(file.c_str());
	if (!myfile.is_open())
		cout << "Fail to open file " << file << endl;
	else {
		myfile << data;
		myfile.close();
	}
}

void CacheSim::BFS() {
	vector<string> buffer;
	buffer.push_back(initialState_);
	vector<int> trace;
	trace.push_back(0);
	visited.insert(initialState_);
	states_ = initialState_;

	int head = 0;
	string last;
	while (head < buffer.size()) {
		string c = buffer[head];
		for (int i = 0; i < nodeNum_ * 4; i++) {
			if (i % 4 == 3) {
				continue;
			}
			string next = action(type_, c , i);
			if (!next.compare("Invalid")) {
				continue;
			}
			if (!visited.count(next) && next.compare(c)) {
				buffer.push_back(next);
				visited.insert(next);
				trace.push_back(trace[head] + 1);
			}
			//Create test suit
			string nedge = c + "->" + next + to_string(i);
			if (!edges.count(nedge)){
				edges.insert(nedge);
			}
			//Adding another two operation to global invalid
			totalActions_ += trace[head] + 3;
			if(totalActions_ % 6 == 0)
			{
				dumpInfo += to_string(totalActions_) + " " + to_string(visited.size()) +" " + to_string(edges.size()) + "\n";
			}
		}
		head++;
	}
	summary();
}

void CacheSim::mcjammer(int length_)
{
	srand(time(NULL));

	for(int i = 0 ; i < length_ ; i++)
	{
		int node = (rand() % nodeNum_) * 4;
		int a = rand() % 100;
		if(a < 45)
			a=0;
		else if( a < 56)
			a = 1;
		else
			a = 2;
		nextAction(a + node);
	}
	summary();
}

void CacheSim::random(int length_)
{
	srand(time(NULL));

	for(int i = 0 ; i < length_ ; i++)
	{
		int node = (rand() % nodeNum_) * 4;
		int a = rand() % 100;
		if (a < 34)
			a = 0;
		else if (a < 67)
			a = 1;
		else
			a = 2;
		nextAction(a + node);
	}
	summary();
}
