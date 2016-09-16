
#include "Init.h"

#include <tuiframework/client/client.h>
#include <tuitypes/common/CommonTypeReg.h>

#include <iostream>
#include <stdlib.h>
#include <cstdio>

using namespace tuiframework;
using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "Usage tuidtrack2client <sender port> <receiver port> <ipaddress:port>" << endl;
		return 1;
	}
	
	Init init;
	initTypeRegistration(getEventFactory());
	CommonTypeReg::registerTypes(&getEventFactory(), &getEventChannelFactory());
	
	getchar();
	
	connectWithTUIServer(atoi(argv[1]), atoi(argv[2]), argv[3], &init);
	return 0;
}
