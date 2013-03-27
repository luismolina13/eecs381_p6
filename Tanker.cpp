#include "Tanker.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>
using std::cout; using std::endl;


Tanker::Tanker(const std::string& name_, Point position_) : 
				Ship(name_, position_, TANKER_FUEL_CAPACITY, TANKER_MAX_SPEED,
					TANKER_FUEL_CONSUMPTION, TANKER_RESISTANCE), 
				cargo(INITIAL_CARGO), cargo_capacity(CARGO_CAPACITY), 
				tanker_state(NO_CARGO_DESTINATIONS), load_destination(nullptr), 
				unload_destination(nullptr){

	cout << "Tanker " <<  get_name() << " constructed" << endl;
}

Tanker::~Tanker() {
	cout << "Tanker " << get_name() << " destructed" << endl;
}

void Tanker::set_destination_position_and_speed(Point destination, double speed) {
	checkStateNoDests();
	Ship::set_destination_position_and_speed(destination, speed);
}

void Tanker::set_course_and_speed(double course, double speed) {
	checkStateNoDests();
	Ship::set_course_and_speed(course, speed);
}

void Tanker::set_load_destination(Island* load_dest_in) {
	checkStateNoDests();
	load_destination = load_dest_in;
	if(load_destination == unload_destination) {
		throw Error("Load and unload cargo destinations are the same!");
	}

	cout << get_name() << " will load at " << load_dest_in->get_name() << endl;
	if(load_destination != nullptr && unload_destination != nullptr) {
		set_load_unload_destination_state_changes();
	}
}

void Tanker::set_unload_destination(Island* unload_dest_in) {
	checkStateNoDests();
	unload_destination = unload_dest_in;
	if(load_destination == unload_destination) {
		throw Error("Load and unload cargo destinations are the same!");
	}

	cout << get_name() << " will unload at " << unload_dest_in->get_name() << endl;
	if(load_destination != nullptr && unload_destination != nullptr) {
		set_load_unload_destination_state_changes();
	}
}

void Tanker::set_load_unload_destination_state_changes() {
	if(is_docked()) {
		if(get_docked_Island() == load_destination) {
			tanker_state = LOADING;
			return;
		} else if(get_docked_Island() == unload_destination) {
			tanker_state = UNLOADING;
			return;
		}
	} 
	if (!is_moving()) {
		if(cargo == 0 && can_dock(load_destination)) {
			dock(load_destination);
			tanker_state = LOADING;
			return;
		} else if(cargo > 0 && can_dock(unload_destination)) {
			dock(unload_destination);
			tanker_state = UNLOADING;
			return;
		}
	} 
	if (cargo == 0) {
		Ship::set_destination_position_and_speed(load_destination->get_location(), get_maximum_speed());
		tanker_state = MOVING_TO_LOADING;
		return;
	} 
	if (cargo > 0) {
		Ship::set_destination_position_and_speed(unload_destination->get_location(), get_maximum_speed());
		tanker_state = MOVING_TO_UNLOADING;
		return;
	}
}

void Tanker::stop() {
	Ship::stop();
	set_no_cargo_destinations();
}

void Tanker::set_no_cargo_destinations() {
	load_destination = nullptr;
	unload_destination = nullptr;
	tanker_state = NO_CARGO_DESTINATIONS;
	cout << get_name() << " now has no cargo destinations" << endl;
}

void Tanker::update() {
	Ship::update();
	if(!can_move()) {
		if(tanker_state == NO_CARGO_DESTINATIONS) {
			return;
		}
		set_no_cargo_destinations();
	} else if(tanker_state == NO_CARGO_DESTINATIONS) {
		//do nothing
	} else if (tanker_state == MOVING_TO_LOADING) {
		if(!is_moving() && can_dock(load_destination)) {
			dock(load_destination);
			tanker_state = LOADING;
		}
	} else if (tanker_state == MOVING_TO_UNLOADING) {
		if(!is_moving() && can_dock(unload_destination)) {
			dock(unload_destination);
			tanker_state = UNLOADING;
		}
	} else if (tanker_state == LOADING) {
		Ship::refuel();
		double roomInCargo = cargo_capacity - cargo;
		if(roomInCargo < .005) {
			cargo = cargo_capacity;
			Ship::set_destination_position_and_speed(unload_destination->get_location(), get_maximum_speed());
			tanker_state = MOVING_TO_UNLOADING;
		} else {
			double reload_amount = get_docked_Island()->provide_fuel(roomInCargo);
			cargo += reload_amount;
			cout << get_name() << " now has " << cargo << " of cargo" << endl;
		}
	} else if (tanker_state == UNLOADING) {
		if(cargo == 0.0) {
			Ship::set_destination_position_and_speed(load_destination->get_location(), get_maximum_speed());
			tanker_state = MOVING_TO_LOADING;
		} else {
			get_docked_Island()->accept_fuel(cargo);
			cargo = 0.0;
		}
	}

}

void Tanker::describe() const {
	cout << "\nTanker ";
	Ship::describe();

	cout << "Cargo: " <<  cargo << " tons";

	switch(tanker_state) {
	case NO_CARGO_DESTINATIONS:
		cout << ", no cargo destinations" << endl;
		break;
	case LOADING:
		cout << ", loading" << endl;
		break;
	case UNLOADING:
		cout << ", unloading" << endl;
		break;
	case MOVING_TO_LOADING:
		cout << ", moving to loading destination" << endl;
		break;
	case MOVING_TO_UNLOADING:
		cout << ", moving to unloading destination" << endl;
		break;
	}
}

void Tanker::checkStateNoDests() {
	if(tanker_state != NO_CARGO_DESTINATIONS) {
		throw Error("Tanker has cargo destinations!");
	}
}