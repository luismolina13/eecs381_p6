#include "Model.h"
#include "Sim_object.h"
#include "Island.h"
#include "Ship.h"
#include "View.h"
#include "Utility.h"
#include <iostream>
#include <utility>
#include <algorithm>
using std::cout; using std::endl;
using std::map;
using std::string;
using std::pair;
using std::set;
using std::shared_ptr;

/* create some islands and ships using the following code in the Model constructor.
Do not change the execution order of these code fragments. You should delete this comment. */
Model::Model() {
	time = 0;

	// Construct initial objects
	shared_ptr<Island> i1(new Island("Exxon", Point(10, 10), 1000, 200));
	shared_ptr<Island> i2(new Island("Shell", Point(0, 30), 1000, 200));
	shared_ptr<Island> i3(new Island("Bermuda", Point(20, 20)));
	shared_ptr<Ship> s1 = create_ship("Ajax", "Cruiser", Point (15, 15));
	shared_ptr<Ship> s2 = create_ship("Xerxes", "Cruiser", Point (25, 25));
	shared_ptr<Ship> s3 = create_ship("Valdez", "Tanker", Point (30, 30));

	// Fill containerds with initial objects
	islands.insert(pair<string, shared_ptr<Island>>(i1->get_name(), i1));
	islands.insert(pair<string, shared_ptr<Island>>(i2->get_name(), i2));
	islands.insert(pair<string, shared_ptr<Island>>(i3->get_name(), i3));

	ships.insert(pair<string, shared_ptr<Ship>>(s1->get_name(), s1));
	ships.insert(pair<string, shared_ptr<Ship>>(s2->get_name(), s2));
	ships.insert(pair<string, shared_ptr<Ship>>(s3->get_name(), s3));

	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(i1->get_name(), i1));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(i2->get_name(), i2));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(i3->get_name(), i3));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(s1->get_name(), s1));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(s2->get_name(), s2));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(s3->get_name(), s3));
	
	cout << "Model constructed" << endl;
}

Model::~Model() {
	for(auto cur: sim_objects) {
		//delete cur.second;
	}
	cout << "Model destructed" << endl;
}

bool Model::is_name_in_use(const std::string& name) const {
	bool nameInUse = false;
	for(auto cur: sim_objects) {
		if(cur.first == name || firstTwoLettersMatch(cur.first, name)) {
			nameInUse = true;
		}
	}
	return nameInUse;
}

bool firstTwoLettersMatch(string s1, string s2) {
	if(s1.size() > 2 && s2.size() > 2) {
		if(s1[0] == s2[0] && s1[1] == s2[1]) {
			return true;
		}
	}
	return false;
}

bool Model::is_island_present(const std::string& name) const {
	return (islands.find(name) != islands.end());
}

void Model::add_island(shared_ptr<Island> new_island) {
	islands.insert(pair<string, shared_ptr<Island>>(new_island->get_name(), new_island));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(new_island->get_name(), new_island));
	new_island->broadcast_current_state();
}

shared_ptr<Island> Model::get_island_ptr(const std::string& name) const {
	map<string, shared_ptr<Island>>::const_iterator it = islands.find(name);
	if(it == islands.end()) {
		throw Error("Island not found!");
	}

	return (*it).second;
}

bool Model::is_ship_present(const std::string& name) const {
	return (ships.find(name) != ships.end());
}

void Model::add_ship(shared_ptr<Ship> new_ship) {
	ships.insert(pair<string, shared_ptr<Ship>>(new_ship->get_name(), new_ship));
	sim_objects.insert(pair<string, shared_ptr<Sim_object>>(new_ship->get_name(), new_ship));
	new_ship->broadcast_current_state();
}

shared_ptr<Ship> Model::get_ship_ptr(const std::string& name) const {
	map<string, shared_ptr<Ship>>::const_iterator it = ships.find(name);
	if(it == ships.end()) {
		throw Error("Ship not found!");
	}

	return (*it).second;
}

void Model::describe() const {
	for(auto cur: sim_objects) {
		cur.second->describe();
	}
}

void Model::update() {
	time++;

	//update
	for(auto cur: sim_objects) {
		cur.second->update();
	}

	// collect and remove "on the bottom"
	set<shared_ptr<Ship>> toRemove;
	for(auto cur: ships) {
		if(!(cur.second->is_afloat())) {
			toRemove.insert(cur.second);
		}
	}

	for(auto cur: toRemove) {
		ships.erase(cur->get_name());
		sim_objects.erase(cur->get_name());
		//delete cur;
	}
}

void Model::attach(shared_ptr<View> new_view) {
	cout << "functional if 1: " << new_view->check_exists() << endl;
	views.insert(new_view);
	cout << "functional if 1 (after): " << new_view->check_exists() << endl;
	for(auto cur: sim_objects) {
		cur.second->broadcast_current_state();
	}
}

void Model::detach(shared_ptr<View> remove_view) {
	views.erase(remove_view);
}

void Model::notify_location(const std::string& name, Point location) {
	for(auto cur: views) {
		cur->update_location(name, location);		
	}
}

void Model::notify_gone(const std::string& name) {
	for(auto cur: views) {
		cur->update_remove(name);			
	}
}
