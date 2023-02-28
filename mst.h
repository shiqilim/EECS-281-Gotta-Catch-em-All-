/// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
#ifndef MST_H
#define MST_H

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
#include <tuple>
#include <utility>
#include <vector>
#include <string>

using namespace std;

struct Pokemon_lct {
	int x;
	int y;
};

struct Prim {
	bool visited;
	double weight;
	int parent;//index
};

class MST{
public:
	int pokemon_num = 0;
	int starting_lct = 0;
	double weight = 0;
	vector<Pokemon_lct> pokemons;
	vector<Prim> prim_table;
	/***********FUNC*********************/

	void read_input() {
		bool coast_exist = false;
		bool land_exist = false;
		bool sea_exist = false;

		cin >> pokemon_num;
		pokemons.reserve(pokemon_num);
		prim_table.reserve(pokemon_num);
		//read all the pokemon points into vector
		for (int i = 0; i < pokemon_num; i++) {
			int x, y;
			cin >> x >> y;
			if (!(x == 0 || y == 0) && !(x < 0 && y < 0)) {
				//not coast and not sea, so it must be land
				land_exist = true;
			}
			else if (x < 0 && y < 0) {
				//sea
				sea_exist = true;
			}
			else if (x == 0 || y == 0) {
				coast_exist = true;
			}
			Pokemon_lct temp_lct = { x,y };
			pokemons.push_back(temp_lct);
			Prim temp_prim = { false,numeric_limits<double>::infinity(),-1 };//-1 indicate no parent
			prim_table.push_back(temp_prim);
		}
		if (land_exist && sea_exist && !coast_exist) {
			cerr << "Cannot construct MST" << endl;
			exit(1);
		}
	}

	void start() {
		read_input();
		int count = 0; // ++ for every true marked

		// set the 0th pokemon as the starting point
		starting_lct = 0;

		// set the starting point distance =0, visited=true
		prim_table[0].weight = 0;

		//repeat until every k is true
		while (count != pokemon_num) {
			double cur_dist = numeric_limits<double>::infinity();
			int cur_v = -1; //-1 to indicate null

			//Loop through false, find the smallest k
			for (int i = 0; i < pokemon_num; i++) {
				if (!prim_table[i].visited) {
					if (prim_table[i].weight < cur_dist) {
						cur_dist = prim_table[i].weight;
						cur_v = i;
					}
				}
			}

			//Mark the smallest k as true
			prim_table[cur_v].visited = true;
			count++;
			weight += sqrt(prim_table[cur_v].weight);

			//Update the false neighbors
			for (int i = 0; i < pokemon_num; i++) {
				if (!prim_table[i].visited) {
					if (distance(pokemons[cur_v], pokemons[i]) < prim_table[i].weight) {
						prim_table[i].parent = cur_v;
						prim_table[i].weight = distance(pokemons[cur_v], pokemons[i]);
					}
				}
			}
		}

		//print output
		cout << weight << endl;
		for (int i = 1; i < pokemon_num; i++) {
			if (i < prim_table[i].parent) {
				cout << i << " " << prim_table[i].parent << endl;
			}
			else {
				cout << prim_table[i].parent << " " << i << endl;
			}
		}

	}

	double distance(Pokemon_lct& first, Pokemon_lct& second) {
		if (first.x > 0 || first.y > 0) {
			//first is land
			if (second.x < 0 && second.y < 0) {
				return numeric_limits<double>::infinity();
			}
			else {
				int x = second.x - first.x;
				int y = second.y - first.y;
				double x_sq = static_cast<double>(x) * static_cast<double>(x);
				double y_sq = static_cast<double>(y) * static_cast<double>(y);
				return x_sq + y_sq;
			}
		}
		else if (first.x < 0 && first.y < 0) {
			//first is sea
			if (second.x <= 0 && second.y <= 0) {
				//second is sea or coastline
				int x = second.x - first.x;
				int y = second.y - first.y;
				double x_sq = static_cast<double>(x) * static_cast<double>(x);
				double y_sq = static_cast<double>(y) * static_cast<double>(y);
				return x_sq + y_sq;
			}
			else {
				return numeric_limits<double>::infinity();
			}
		}
		else if ((first.x == 0 && first.y <= 0) || (first.y == 0 && first.x <= 0)) {
			//first is coastline
			int x = second.x - first.x;
			int y = second.y - first.y;
			double x_sq = static_cast<double>(x) * static_cast<double>(x);
			double y_sq = static_cast<double>(y) * static_cast<double>(y);
			return x_sq + y_sq;
		}
		else {
			return numeric_limits<double>::infinity();
		}
	}

};

#endif // MST_H