#include "Cruise_ship.h"
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
	cout << "Cruise ship " << get_name() << " constructed" << endl;
}

Cruise_ship::~Cruise_ship() {
	cout << "Cruise ship " << get_name() << " destructed" << endl;
}

void Cruise_ship::update() {
	Ship::update();
	
}

void Cruise_ship::describe() {
	cout << "\nCruise_ship ";
	Ship::describe();
}