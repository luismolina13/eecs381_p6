#include "Cruise_ship.h"
#include "Utility.h"
#include "Model.h"
#include "Island.h"
#include <iostream>
using std::cout; using std::endl;

const double cruise_fuel_capacity_c = 500.;
const double cruise_max_speed_c = 15.;
const double cruise_fuel_consumption_c = 2.;
const int cruise_resistance_c = 0;

Cruise_ship::Cruise_ship(const std::string& name_, Point position_) : 
						Ship(name_, position_, cruise_fuel_capacity_c, 
							cruise_max_speed_c, cruise_fuel_consumption_c, 
							cruise_resistance_c) {
	cruise_state = NO_CRUISE_DESTINATIONS;							
	cout << "Cruise ship " << get_name() << " constructed" << endl;
}

Cruise_ship::~Cruise_ship() {
	cout << "Cruise ship " << get_name() << " destructed" << endl;
}

void Cruise_ship::update() {
	Ship::update();
	if(cruise_state = NO_CRUISE_DESTINATIONS) {
		return;
	} 

	if(cruise_state = CRUISE_DOCKED) {
		Ship::refuel();
		cruise_state = REFUEL;
		return;
	} else if(cruise_state = REFUEL) {
		cruise_state = SIGHTSEE;
		return;
	} else if(cruise_state = SIGHTSEE) {
		// get the island closest to this that has not yet been visited
		next_destination = Model::getInstance().getNearestIsland(get_location(), islandsTraveled);
		cout << get_name() << " will visit " << next_destination << endl;
		cruise_state = CRUISING;
		return;
	} else if(cruise_state = CRUISING) {
		if(!is_moving() && can_dock(next_destination)) {
			Ship::dock(next_destination);
			cruise_state = CRUISE_DOCKED;
			islandsTraveled.insert(next_destination);
		}
		//if we have reached the end
		if(get_location() == cruise_start->get_location() && islandsTraveled.size() > 0) {
			cruise_state = NO_CRUISE_DESTINATIONS;
			cout << get_name() << " cruise is over at " << 
				cruise_start->get_name() << endl;
		}
	}
}

void Cruise_ship::describe() const {
	cout << "\nCruise_ship ";
	Ship::describe();
	if(cruise_state == CRUISE_DOCKED || cruise_state == REFUEL || cruise_state == SIGHTSEE) {
		cout << "Waiting during cruise at " << next_destination->get_name() << endl;
	} else if (cruise_state == CRUISING) {
		cout << "On cruise to " << next_destination->get_name() << endl;
	}
}

void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed) {
	try {
		cruise_start = Model::getInstance().getIslandFromPosition(destination_position);
	} catch (Error& e) {
		/*	no island at the given destination, simply move to it as a ship 
			and cancel cruise if necessary */
		cancelCruise();
		Ship::set_destination_position_and_speed(destination_position, speed);
	}
	try {
		if(cruise_state != NO_CRUISE_DESTINATIONS) {
			cancelCruise();
		}
		next_destination = cruise_start;
		Ship::set_destination_position_and_speed(destination_position, speed);
		cout << get_name() << " cruise will start and end at " << 
						cruise_start->get_name() << endl;	
	} catch (Error& e) {
		cancelCruise();
		throw e;
	}
}

void Cruise_ship::set_course_and_speed(double course, double speed) {
	cancelCruise();
	Ship::set_course_and_speed(course, speed);
}

void Cruise_ship::stop() {
	cancelCruise();
	Ship::stop();
}

void Cruise_ship::dock(std::shared_ptr<Island> island_ptr) {
	cancelCruise();
	Ship::dock(island_ptr);
}

void Cruise_ship::refuel() {
	cancelCruise();
	Ship::refuel();
}

void Cruise_ship::cancelCruise() {
	cout << get_name() << " canceling current cruise" << endl;
	islandsTraveled.clear();
	cruise_state = NO_CRUISE_DESTINATIONS;
}