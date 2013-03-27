/*
A Tanker is a ship with a large corgo capacity for fuel.
It can be told an Island to load fuel at, and an Island to unload at.
Once it is sent to the loading destination, it will start shuttling between 
the loading and unloading destination. At the loading destination, 
it will first refuel then wait until its cargo hold is full, then it will
go to the unloading destination.

Initial values:
fuel capacity and initial amount 100 tons, maximum speed 10., fuel consumption 2.tons/nm, 
resistance 0, cargo capacity 1000 tons, initial cargo is 0 tons.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

#ifndef TANKER_H
#define TANKER_H

#include "Ship.h"

#define TANKER_FUEL_CAPACITY 100.
#define TANKER_MAX_SPEED 10.
#define TANKER_FUEL_CONSUMPTION 2.
#define TANKER_RESISTANCE 0
#define CARGO_CAPACITY 1000.
#define INITIAL_CARGO 0

class Island;

enum TankerState_e { NO_CARGO_DESTINATIONS, UNLOADING, MOVING_TO_LOADING,
					 LOADING, MOVING_TO_UNLOADING };

class Tanker : public Ship {

public:
	// initialize, the output constructor message
	Tanker(const std::string& name_, Point position_);
	// output destructor message
	~Tanker();
	
	// This class overrides these Ship functions so that it can check if this Tanker has assigned cargo destinations.
	// if so, throw an Error("Tanker has cargo destinations!"); otherwise, simply call the Ship functions.
	virtual void set_destination_position_and_speed(Point destination, double speed);
	virtual void set_course_and_speed(double course, double speed);

	// Set the loading and unloading Island destinations
	// if both cargo destination are already set, throw Error("Tanker has cargo destinations!").
	// if they are the same, leave at the set values, and throw Error("Load and unload cargo destinations are the same!")
	// if both destinations are now set, start the cargo cycle
	void set_load_destination(Island*) override;
	void set_unload_destination(Island*) override;
	
	// when told to stop, clear the cargo destinations and stop
	void stop() override;
	
	void update() override;
	void describe() const override;

private: 
	double cargo_capacity;				// fuel cargo capacity
	double cargo;
	TankerState_e tanker_state;			// state of the tanker
	Island* load_destination;			// pointer to load destination island
	Island* unload_destination;			// pointer to unload destination island

	void set_load_unload_destination_state_changes();
	void set_no_cargo_destinations();
	void checkStateNoDests();
};

#endif