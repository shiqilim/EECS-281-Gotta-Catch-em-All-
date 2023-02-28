/// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
#ifndef OPTTSP_H
#define OPTTSP_H

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
#include "fasttsp.h"

using namespace std;

class OPTTSP {
public:
	int pokemon_num = 0;
	int perm_len = 0;
	double A1 = 0;
	double A2 = 0;
	double estimate = 0;
	double upper_bound = 0; //Initial cost
	double lower_bound = 0; //MST of unvisited vertices
	double cur_running = 0; //Current cost
	vector<Pokemon_lct> pokemons; //pokemons location
	vector<uint32_t> path; //Initial path from FASTTSP, best so far 
	vector<uint32_t> perm_path;

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

	/*******************FASTTSP**************/

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

	void get_upper_bound() {
		for (int i = 0; i < static_cast<int>(path.size()) - 1; i++) {
			upper_bound += comp_weight(path[i], path[i + 1]);
		}
	}

	void get_initial_path() {
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
	}

	void fast() {
		get_initial_path();
		get_upper_bound();
		path.pop_back();
	}

	/*****************MST*****************/

	double mst(uint32_t permLength) {
		int count = 0;
		double estimate = 0;
		vector<Prim> prim_table;
		prim_table.reserve(path.size() - permLength);

		//push unvisited point to prim_table
		for (int i = permLength; i < static_cast<int>(path.size()); i++) {
			Prim temp = { false,numeric_limits<double>::infinity(),-1 };
			prim_table.push_back(temp);
		}

		// set the starting point distance =0
		prim_table[0].weight = 0;

		while (count != static_cast<int>(prim_table.size())) {
			double cur_dist = numeric_limits<double>::infinity();
			int cur_v = -1; //-1 to indicate null

			//Loop through false, find the smallest k
			for (int i = 0; i < static_cast<int>(prim_table.size()); i++) {
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
			estimate += prim_table[cur_v].weight;

			//Update the false neighbors
			for (int i = 0; i < static_cast<int>(prim_table.size()); i++) {
				if (!prim_table[i].visited) {
					double temp = distance(pokemons[perm_path[cur_v + permLength]], pokemons[perm_path[i + permLength]]);
					if (temp < prim_table[i].weight) {
						prim_table[i].parent = cur_v;
						prim_table[i].weight = temp;
					}
				}
			}
		}

		return estimate;
	}

	double distance(Pokemon_lct& lhs, Pokemon_lct& rhs) {
		int x = lhs.x - rhs.x;
		int y = lhs.y - rhs.y;
		double x_sq = static_cast<double>(x) * static_cast<double>(x);
		double y_sq = static_cast<double>(y) * static_cast<double>(y);
		return sqrt(x_sq + y_sq);
	}

	/***************OPTTSP***************/

	void get_perm_path() {
		/*
		perm_path.reserve(path.size());
		for (int i = 0; i < static_cast<int>(path.size()); i++) {
			perm_path.push_back(i);
		}
		*/
		perm_path = path;
	}

	bool promising(uint32_t permLength) {
		estimate = mst(permLength);
		double temp = round((cur_running + estimate + A1 + A2) * 100) / 100;
		if (temp >= upper_bound) {
			return false;
		}
		else {
			return true;
		}
	}

	void genPerms(uint32_t permLength) {
		if (permLength == path.size()) {
			double temp = cur_running - distance(pokemons[perm_path[permLength - 1]], pokemons[perm_path[permLength - 2]]);
			lower_bound = temp + estimate + A1 + A2;
			if (lower_bound < upper_bound) {
				upper_bound = lower_bound;
				upper_bound = round(upper_bound * 100) / 100;
				path = perm_path;
			}
			return;
		}
		arm_len(permLength);
		if (!promising(permLength)) {
			return;
		}
		for (size_t i = permLength; i < path.size(); ++i) {
			swap(perm_path[permLength], perm_path[i]);
			cur_running += distance(pokemons[perm_path[permLength]], pokemons[perm_path[permLength - 1]]);
			genPerms(permLength + 1);
			cur_running -= distance(pokemons[perm_path[permLength]], pokemons[perm_path[permLength - 1]]);
			swap(perm_path[permLength], perm_path[i]);
		}
	}

	void arm_len(uint32_t permLength) {
		A1 = distance(pokemons[0], pokemons[perm_path[permLength]]);
		A2 = distance(pokemons[perm_path[permLength - 1]], pokemons[perm_path[permLength]]);
		for (int i = permLength + 1; i < static_cast<int>(path.size()); i++) {
			double temp1 = distance(pokemons[0], pokemons[perm_path[i]]);
			double temp2 = distance(pokemons[perm_path[permLength - 1]], pokemons[perm_path[i]]);
			if (temp1 < A1) {
				A1 = temp1;
			}
			if (temp2 < A2) {
				A2 = temp2;
			}
		}
	}
	
	void start() {
		cerr << fixed << showpoint << setprecision(2);
		read_input(); //read input
		fast(); //get best so far path and cost
		get_perm_path();
		genPerms(1);
		cout << upper_bound << endl;
		for (int i = 0; i < static_cast<int>(path.size()); i++) {
			cout << path[i] << " ";
		}
		cout << endl;
	}
};


#endif // OPTTSP_H