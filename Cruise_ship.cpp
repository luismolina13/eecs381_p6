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
	set_no_cruise_destinations();							
	//cout << "Cruise ship " << get_name() << " constructed" << endl;
}

Cruise_ship::~Cruise_ship() {
	//cout << "Cruise ship " << get_name() << " destructed" << endl;
}

void Cruise_ship::update() {
	Ship::update();
	if(cruise_state == NO_CRUISE_DESTINATIONS) {
		return;
	} 

	if(cruise_state == CRUISE_DOCKED) {
		Ship::refuel();
		cruise_state = REFUEL;
		return;
	} else if(cruise_state == REFUEL) {
		cruise_state = SIGHTSEE;
		return;
	} else if(cruise_state == SIGHTSEE) {
		// get the island closest to this that has not yet been visited
		try {
			next_destination = Model::getInstance().getNearestIsland(get_location(), islandsTraveled);
			Ship::set_destination_position_and_speed(next_destination->get_location(), cruise_speed);
		} catch (Error& e) {
			/* 	this condition occurs when all islands have been visited. In
				this case, the next destination should be set to the starting 
				position and the cruise is considered over once docked*/
			if(e.msg = "set_done error") {
				cruise_over = true;
				next_destination = cruise_start;
				Ship::set_destination_position_and_speed(next_destination->get_location(), cruise_speed);
			} else {
				throw e;
			}
		}
		cout << get_name() << " will visit " << next_destination->get_name() << endl;
		cruise_state = CRUISING;
		return;
	} else if(cruise_state == CRUISING) {
		if(!is_moving() && can_dock(next_destination)) {
			Ship::dock(next_destination);
			cruise_state = CRUISE_DOCKED;
			islandsTraveled.insert(next_destination);
		}
		//if we have reached the end
		if(is_cruise_over()) {
			cout << get_name() << " cruise is over at " << 
				cruise_start->get_name() << endl;
			set_no_cruise_destinations();
		}
	}
}

bool Cruise_ship::is_cruise_over() {
	return (get_location() == cruise_start->get_location() && is_docked() && cruise_over);
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
	bool cancelMsg = false; 
	if(cruise_state != NO_CRUISE_DESTINATIONS) {
		cancelCruise();
		cancelMsg = true;
	}

	try {
		cruise_start = Model::getInstance().getIslandFromPosition(destination_position);
	} catch (Error& e) {
		/*	no island at the given destination, simply move to it as a ship 
			and cancel cruise if necessary */
		set_no_cruise_destinations();
		Ship::set_destination_position_and_speed(destination_position, speed);
		return;
	}
	try {
		cruise_speed = speed;
		cruise_state = CRUISING;
		next_destination = cruise_start;
		Ship::set_destination_position_and_speed(destination_position, speed);
		cout << get_name() << " will visit " << next_destination->get_name() << endl;
		cout << get_name() << " cruise will start and end at " << 
						cruise_start->get_name() << endl;	
	} catch (Error& e) {
		if(cancelMsg) {
			set_no_cruise_destinations();
		} else {
			cancelCruise();
		}
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
	set_no_cruise_destinations();
}

void Cruise_ship::set_no_cruise_destinations() {
	islandsTraveled.clear();
	cruise_start = nullptr;
	next_destination = nullptr;
	cruise_state = NO_CRUISE_DESTINATIONS;
	cruise_speed = 0;
	cruise_over = false;
}
