#include "Warship.h"
#include "Utility.h"
#include <iostream>
using std::cout; using std::endl;
using std::shared_ptr; using std::weak_ptr;

Warship::Warship(const std::string& name_, Point position_, double fuel_capacity_, 
		double maximum_speed_, double fuel_consumption_, int resistance_,
		int firepower_, double maximum_range_) : 
		Ship(name_, position_, fuel_capacity_, maximum_speed_, 
			fuel_consumption_, resistance_), 
		firepower(firepower_), maximum_range(maximum_range_) {
	
	warship_state = NOT_ATTACKING;
	//cout << "Warship " << get_name() << " constructed" << endl;
	//target_ptr is automatically set to nothing/nullptr
}

Warship::~Warship() {
	//cout << "Warship "  << get_name() << " destructed" << endl;
}

void Warship::update() {
	Ship::update();
	if(is_attacking()) {
		shared_ptr<Ship> target_shared = target_ptr.lock(); //get shared from weak
		if(!is_afloat() || !target_shared || !(target_shared->is_afloat())) {
			stop_attack();
			target_ptr.reset();
		} else {
			cout << get_name() << " is attacking " << endl;
		}
	}
}

void Warship::attack(shared_ptr<Ship> target_ptr_) {
	if(!is_afloat()) {
		throw Error("Cannot attack!");
	}

	if(target_ptr_ == shared_from_this()) { //gets shared ptr to "this" object
		throw Error("Warship may not attack itself!");
	}

	target_ptr = target_ptr_;
	warship_state = ATTACKING;
	cout << get_name() << " will attack " << target_ptr_->get_name() << endl;
}

void Warship::stop_attack() {
	if(!is_attacking()) {
		throw Error("Was not attacking!");
	}
	warship_state = NOT_ATTACKING;
	target_ptr.reset();
	cout << get_name() << " stopping attack" << endl;
}

void Warship::describe() const {
	Ship::describe();
	if(is_attacking()) {
		shared_ptr<Ship> target_shared = target_ptr.lock(); //get shared from weak
		
		if(!target_shared || !(target_shared->is_afloat())) {
			cout << "Attacking absent ship" << endl;
			return;
		}
		cout << "Attacking " << target_shared->get_name() << endl;
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
	if(target_ptr.expired()) {
		cout << "Attacking absent ship" << endl;
		return;
	}
	shared_ptr<Ship> target_shared = target_ptr.lock(); //get shared from weak

	target_shared->receive_hit(firepower, shared_from_this());
}

bool Warship::target_in_range() const {
	if(target_ptr.expired()) {
		return false;
	}
	shared_ptr<Ship> target_shared = target_ptr.lock(); //get shared from weak

	double distance_to_target = cartesian_distance(get_location(), 
												target_shared->get_location());
	if(distance_to_target <= maximum_range) {
		return true;
	} else {
		return false;
	}
}

shared_ptr<Ship> Warship::get_target() const {
	if(target_ptr.expired()) {
		return shared_ptr<Ship>(nullptr);
	}
	
	shared_ptr<Ship> target_shared = target_ptr.lock(); //get shared from weak
	return target_shared;
}
