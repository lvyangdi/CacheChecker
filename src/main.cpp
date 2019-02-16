#include "CacheTester.h"
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[]) {
	bool dump = false;
	if (argc < 3){
		cout << "Usage: ./CacheChecker <PROTOCOL> <CORENUM> [dump] {default}/\"BFS\"/\"Random\"/\"MCjammer\"\n";
		return 0;
	}
	int nodeNum = atoi(argv[2]);
	if ((argc == 4) && (!strcmp(argv[3], "dump")))
		dump = true;
	CacheTester tester(nodeNum, argv[1], dump, true);
	if (!strcmp(argv[argc - 1], "BFS"))
		tester.BFS();
	else if (!strcmp(argv[argc - 1], "Random"))
		tester.random(20000000);
	else if (!strcmp(argv[argc - 1], "MCjammer"))
		tester.mcjammer(20000000);
	else
		tester.test();
	return 0;
}
