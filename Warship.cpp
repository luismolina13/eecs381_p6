#include "Warship.h"
#include "Utility.h"
#include <iostream>
using std::cout; using std::endl;

Warship::Warship(const std::string& name_, Point position_, double fuel_capacity_, 
		double maximum_speed_, double fuel_consumption_, int resistance_,
		int firepower_, double maximum_range_) : 
		Ship(name_, position_, fuel_capacity_, maximum_speed_, 
			fuel_consumption_, resistance_), 
		firepower(firepower_), maximum_range(maximum_range_) {
	
	warship_state = NOT_ATTACKING;
	target_ptr = nullptr;
	cout << "Warship " << get_name() << " constructed" << endl;
}

Warship::~Warship() {
	cout << "Warship "  << get_name() << " destructed" << endl;
}

void Warship::update() {
	Ship::update();
	if(is_attacking()) {
		if(!is_afloat() || !(target_ptr->is_afloat())) {
			stop_attack();
		} else {
			cout << get_name() << " is attacking " << endl;
		}
	}
}

void Warship::attack(Ship* target_ptr_) {
	if(!is_afloat()) {
		throw Error("Cannot attack!");
	}

	if(target_ptr_ == this) { 
		throw Error("Warship may not attack itself!");
	}

	target_ptr = target_ptr_;
	warship_state = ATTACKING;
	cout << get_name() << " will attack " << target_ptr->get_name() << endl;
}

void Warship::stop_attack() {
	if(!is_attacking()) {
		throw Error("Was not attacking!");
	}
	warship_state = NOT_ATTACKING;
	target_ptr = nullptr;
	cout << get_name() << " stopping attack" << endl;
}

void Warship::describe() const {
	Ship::describe();
	if(is_attacking()) {
		cout << "Attacking " << target_ptr->get_name() << endl;
	} 
}

bool Warship::is_attacking() const {
	if(warship_state == ATTACKING) {
		return true;
	} else {
		return false;
	}
}

void Warship::fire_at_target() {
	cout << get_name() << " fires" << endl;
	target_ptr->receive_hit(firepower, this);
}

bool Warship::target_in_range() const {
	double distance_to_target = cartesian_distance(get_location(), 
												target_ptr->get_location());
	if(distance_to_target <= maximum_range) {
		return true;
	} else {
		return false;
	}
}

Ship* Warship::get_target() const {
	return target_ptr;
}
