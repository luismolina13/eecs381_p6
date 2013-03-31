#include "Cruiser.h"
#include <iostream>
using std::cout; using std::endl;
using std::shared_ptr;

const double cruiser_fuel_capacity_c = 1000;
const double cruiser_max_speed_c = 20;
const double cruiser_fuel_consumption_c = 10;
const int cruiser_resistance_c = 6;
const int firepower_c = 3;
const double attack_range_c = 15;

Cruiser::Cruiser(const std::string& name_, Point position_) : 
				Warship(name_, position_, cruiser_fuel_capacity_c, 
					cruiser_max_speed_c, cruiser_fuel_consumption_c, 
					cruiser_resistance_c, firepower_c, attack_range_c) {
	cout << "Cruiser " << get_name() << " constructed" << endl;
}

Cruiser::~Cruiser() {
	cout << "Cruiser " << get_name() << " destructed" << endl;
}

void Cruiser::update() {
	Warship::update();
	if(is_attacking()) {
		if(target_in_range()) {
			Warship::fire_at_target();
		} else {
			cout << get_name() << " target is out of range" << endl;
			Warship::stop_attack();
		}
	}
}

void Cruiser::describe() const {
	cout << "\nCruiser ";
	Warship::describe();
}

void Cruiser::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr) {
	Ship::receive_hit(hit_force, attacker_ptr);
	if(!is_attacking()) {
		Warship::attack(attacker_ptr);
	}
}
