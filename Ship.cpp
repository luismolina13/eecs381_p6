#include "Ship.h"
#include "Island.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>
#include <string>
using std::cout; using std::endl;
using std::string;
using std::shared_ptr;

Ship::Ship(const string& name_, Point position_, double fuel_capacity_, 
		double maximum_speed_, double fuel_consumption_, int resistance_) :
		Sim_object(name_), fuel_consumption(fuel_consumption_), 
		fuel_capacity(fuel_capacity_), maximum_speed(maximum_speed_), 
		resistance(resistance_) {

	trackBase.set_speed(0.);
	fuel = fuel_capacity;
	docked_island = nullptr;
	ship_state = STOPPED;
	cout << "Ship " << get_name() << " constructed" << endl;
}

/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract, 
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/
Ship::~Ship()
{
	cout << "Ship "  << get_name() << " destructed" << endl;
}
	
bool Ship::can_move() const {
	if(	ship_state == DEAD_IN_THE_WATER || !is_afloat()) {
		return false;
	} else {
		return true;
	}
}

bool Ship::is_moving() const {
	if(ship_state == MOVING_TO_POSITION || ship_state == MOVING_ON_COURSE) {
		return true;
	} else {
		return false;
	}
}

bool Ship::is_docked() const {
	if(ship_state == DOCKED) {
		return true;
	} else {
		return false;
	}
}

bool Ship::is_afloat() const {
	if(ship_state == SUNK) {
		return false;
	} else {
		return true;
	}
}

bool Ship::can_dock(shared_ptr<Island> island_ptr) const {
	if(ship_state == STOPPED && 
		cartesian_distance(get_location(), island_ptr->get_location()) <= .1) {
		return true;
	} else {
		return false;
	}
}

void Ship::update() {
	if(is_afloat()) {
		if(resistance < 0) {
			ship_state = SUNK;
			cout << get_name() << " sunk" << endl;
			Model::getInstance().notify_gone(get_name());
			return;
		}
	} 

	if(is_afloat()) { 
		if(ship_state == MOVING_ON_COURSE || ship_state == MOVING_TO_POSITION) {
			calculate_movement();
			cout << get_name() << " now at " << get_location() << endl;
			Model::getInstance().notify_location(get_name(), get_location());
		} else if (ship_state == STOPPED) {
			cout << get_name() << " stopped at " << get_location() << endl;
		} else if (ship_state == DOCKED) {
			cout << get_name() << " docked at " << docked_island->get_name() << endl;
		} else if (ship_state == DEAD_IN_THE_WATER) {
			cout << get_name() << " dead in the water at " << get_location() << endl;
		}
	}
}

void Ship::describe() const {
	cout << get_name() << " at " << get_location();

	if(ship_state == SUNK ) {
		cout << " sunk" << endl;
		return;
	}

	cout << ", fuel: " << fuel << " tons" << ", resistance: " 
		<< resistance << endl;

	if(ship_state == DEAD_IN_THE_WATER ) {
		cout << "Dead in the water" << endl;
	} else if(ship_state == DOCKED ) {
		cout << "Docked at " << docked_island->get_name() << endl;
	} else if(ship_state == STOPPED ) {
		cout << "Stopped" << endl;
	} else if(ship_state == MOVING_TO_POSITION ) {
		cout << "Moving to " << destination << " on " << trackBase.get_course_speed() << endl;
	} else if(ship_state == MOVING_ON_COURSE ) {
		cout << "Moving on " << trackBase.get_course_speed() << endl;
	}
}

void Ship::broadcast_current_state() {
	Model::getInstance().notify_location(get_name(), get_location());
}

void Ship::set_destination_position_and_speed(Point destination_position, double speed) {
	setDestAndCourseChecks(speed);

	Compass_vector cv(get_location(), destination_position);
	trackBase.set_course_speed(Course_speed(cv.direction, speed));

	destination = destination_position;
	cout << get_name() << " will sail on " << trackBase.get_course_speed() 
		<< " to " << destination << endl;
	ship_state = MOVING_TO_POSITION;
}

void Ship::set_course_and_speed(double course, double speed) {
	setDestAndCourseChecks(speed);

	trackBase.set_course_speed(Course_speed(course, speed));

	cout << get_name() << " will sail on " << trackBase.get_course_speed() << endl;
	ship_state = MOVING_ON_COURSE;
}

void Ship::setDestAndCourseChecks(double speed) {
	if(!can_move())
		throw Error("Ship cannot move!");
	else if (speed > maximum_speed)
		throw Error("Ship cannot go that fast!");
}

void Ship::stop() {
	if(!is_afloat() || ship_state == DEAD_IN_THE_WATER) {
		throw Error("Ship cannot move!");
	}
	trackBase.set_speed(0);
	ship_state = STOPPED;
	cout << get_name() << " stopping at " << get_location() << endl;
}

void Ship::dock(shared_ptr<Island> island_ptr) {
	if(!can_dock(island_ptr)) {
		throw Error("Can't dock!");
	}

	trackBase.set_position(island_ptr->get_location());
	docked_island = island_ptr;
	ship_state = DOCKED;
	Model::getInstance().notify_location(get_name(), get_location());
	cout << get_name() << " docked at " << island_ptr->get_name() << endl;
}

void Ship::refuel() {
	if(ship_state != DOCKED) {
		throw Error("Must be docked!");
	}

	double roomInTank = fuel_capacity - fuel;
	if(roomInTank < .005) {
		fuel = fuel_capacity;
	} else {
		double refuel_amount = docked_island->provide_fuel(roomInTank);
		fuel += refuel_amount;
		cout << get_name() << " now has " << fuel << " tons of fuel" << endl;
	}

}

void Ship::set_load_destination(shared_ptr<Island>) {
	throw Error("Cannot load at a destination!");
}

void Ship::set_unload_destination(shared_ptr<Island>) {
	throw Error("Cannot unload at a destination!");
}

void Ship::attack(shared_ptr<Ship> in_target_ptr) {
	throw Error("Cannot attack!");
}

void Ship::stop_attack() {
	throw Error("Cannot attack!");
}

void Ship::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr) {
	resistance -= hit_force;
	cout << get_name() << " hit with " << hit_force << ", resistance now " 
		<< resistance << endl;
}


/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
MOVING_TO_POSITION or MOVING_ON_COURSE.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the Ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or state names if you wish (e.g. movement_state).
*/
void Ship::calculate_movement()
{
	// Compute values for how much we need to move, and how much we can, and how long we can,
	// given the fuel state, then decide what to do.
	double time = 1.0;	// "full step" time
	// get the distance to destination
	double destination_distance = cartesian_distance(get_location(), destination);
	// get full step distance we can move on this time step
	double full_distance = trackBase.get_speed() * time;
	// get fuel required for full step distance
	double full_fuel_required = full_distance * fuel_consumption;	// tons = nm * tons/nm
	// how far and how long can we sail in this time period based on the fuel state?
	double distance_possible, time_possible;
	if(full_fuel_required <= fuel) {
		distance_possible = full_distance;
		time_possible = time;
		}
	else {
		distance_possible = fuel / fuel_consumption;	// nm = tons / tons/nm
		time_possible = (distance_possible / full_distance) * time;
		}
	
	// are we are moving to a destination, and is the destination within the distance possible?
	if(ship_state == MOVING_TO_POSITION && destination_distance <= distance_possible) {
		// yes, make our new position the destination
		trackBase.set_position(destination);
		// we travel the destination distance, using that much fuel
		double fuel_required = destination_distance * fuel_consumption;
		fuel -= fuel_required;
		trackBase.set_speed(0.);
		ship_state = STOPPED;
		}
	else {
		// go as far as we can, stay in the same movement state
		// simply move for the amount of time possible
		trackBase.update_position(time_possible);
		// have we used up our fuel?
		if(full_fuel_required >= fuel) {
			fuel = 0.0;
			ship_state = DEAD_IN_THE_WATER;
			}
		else {
			fuel -= full_fuel_required;
			}
		}
}

