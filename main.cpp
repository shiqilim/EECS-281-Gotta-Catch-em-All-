//Project Identifier : 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <iomanip>
#include <getopt.h>
#include "mst.h"
#include "fasttsp.h"
#include "opttsp.h"

using namespace std;

void printHelp(char* argv[]) {
	cout << "Usage: " << argv[0] << endl;
	cout << "\t[--help] | [-h]\n";
	cout << "\t[--mode MST|FASTTSP|OPTTSP] | [-m MST|FASTTSP|OPTTSP]\n";
}

int getMode(int argc, char* argv[]) {
	opterr = false; // Let us handle all error output for command line options
	int choice;
	int option_index = 0;
	int output = -1; //-1 indicates nothing
	string mode;
	option long_options[] = {
		{"mode",required_argument,nullptr,'m'},
		{"help",no_argument,nullptr,'h'},
		{ nullptr, 0, nullptr, '\0' }
	};

	while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
		switch (choice) {
		case 'm':
			mode = optarg;
			if (mode != "MST" && mode != "FASTTSP" && mode != "OPTTSP") {
				cerr << "Invalid mode" << endl;
				exit(1);
			}
			if (mode.at(0) == 'M') {
				output = 0;
			}
			else if (mode.at(0) == 'F') {
				output = 1;
			}
			else {
				output = 2;
			}
			break;
		case 'h':
			printHelp(argv);
			exit(0);
			break;
		default:
			cerr << "Invalid command line option" << endl;
			exit(1);
		} // switch
	} // while
	return output;
} // getMode()

int main(int argc, char* argv[]) {
	cout << setprecision(2); // Always show 2 decimal places
	cout << fixed; // Disable scientific notation for large numbers

	int mode = getMode(argc, argv);

	if (mode == 0) {
		MST mst;
		mst.start();
	}
	else if (mode == 1) {
		FASTTSP fasttsp;
		fasttsp.start();
	}
	else {
		OPTTSP opttsp;
		opttsp.start();
	}
}