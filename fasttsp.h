/// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
#ifndef FASTTSP_H
#define FASTTSP_H

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
#include "mst.h"
#include "opttsp.h"

using namespace std;


class FASTTSP {
public:
	int pokemon_num = 0;
	vector<Pokemon_lct> pokemons;
	vector<uint32_t> path;

	void read_input() {
		cin >> pokemon_num;
		pokemons.reserve(pokemon_num);
		//read all the pokemon points into vector
		for (int i = 0; i < pokemon_num; i++) {
			int x, y;
			cin >> x >> y;
			Pokemon_lct temp_lct = { x,y };
			pokemons.push_back(temp_lct);
		}
	}

	double min_distance(int i, int j, int k) {
		int x_ij = pokemons[i].x - pokemons[j].x;
		int y_ij = pokemons[i].y - pokemons[j].y;
		double x_ij_sq = static_cast<double>(x_ij) * static_cast<double>(x_ij);
		double y_ij_sq = static_cast<double>(y_ij) * static_cast<double>(y_ij);
		double dist_ij = sqrt(x_ij_sq + y_ij_sq);

		int x_ik = pokemons[i].x - pokemons[k].x;
		int y_ik = pokemons[i].y - pokemons[k].y;
		double x_ik_sq = static_cast<double>(x_ik) * static_cast<double>(x_ik);
		double y_ik_sq = static_cast<double>(y_ik) * static_cast<double>(y_ik);
		double dist_ik = sqrt(x_ik_sq + y_ik_sq);

		int x_kj = pokemons[k].x - pokemons[j].x;
		int y_kj = pokemons[k].y - pokemons[j].y;
		double x_kj_sq = static_cast<double>(x_kj) * static_cast<double>(x_kj);
		double y_kj_sq = static_cast<double>(y_kj) * static_cast<double>(y_kj);
		double dist_kj = sqrt(x_kj_sq + y_kj_sq);

		return dist_ik + dist_kj - dist_ij;
	}

	double comp_weight(int i, int j) {
		int x = pokemons[i].x - pokemons[j].x;
		int y = pokemons[i].y - pokemons[j].y;
		double x_sq = static_cast<double>(x) * static_cast<double>(x);
		double y_sq = static_cast<double>(y) * static_cast<double>(y);
		return sqrt(x_sq + y_sq);
	}

	void start() {
		read_input();
		
		//initialize partial tour: 0->1->2->0
		path.push_back(0);
		path.push_back(1);
		path.push_back(2);
		path.push_back(0);

		for (int i = 3; i < pokemon_num; i++) {
			if (find(path.begin(), path.end(), i) == path.end()) {
				//set the edge between 1st and 2nd points as min edge first
				double min_dist = min_distance(path[0], path[1], i);
				uint32_t insertion_pt = 1; //index to insert the new point
				for (int j = 1; j < static_cast<int>(path.size()) - 1; j++) {
					double temp = min_distance(path[j], path[j + 1], i);
					if (temp < min_dist) {
						min_dist = temp;
						insertion_pt = j + 1;
					}
				}
				path.insert(path.begin() + insertion_pt, i);
			}
		}

		double weight = 0;
		for (int i = 0; i < static_cast<int>(path.size()) - 1; i++) {
			weight += comp_weight(path[i], path[i + 1]);
		}

		cout << weight << endl;
		for (int i = 0; i < static_cast<int>(path.size()) - 1; i++) {
			cout << path[i] << " ";
		}
		cout << endl;
	}
};

#endif // FASTTSP_H