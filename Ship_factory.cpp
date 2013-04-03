#include "Ship_factory.h"
#include "Tanker.h"
#include "Cruiser.h"
#include "Cruise_ship.h"
#include "Utility.h"
#include <memory>
using std::shared_ptr; using std::make_shared;

#include <iostream>

shared_ptr<Ship> create_ship(const std::string& name, const std::string& type, 
											Point initial_position) {

	shared_ptr<Ship> return_ptr;
	if(type == "Tanker") {
		return_ptr = make_shared<Tanker>(name, initial_position); 
	} else if (type == "Cruiser") {
		return_ptr = make_shared<Cruiser>(name, initial_position); 
	} else if (type == "Cruise_ship") {
		return_ptr = make_shared<Cruise_ship>(name, initial_position); 
	} else {
		throw Error("Trying to create ship of unknown type!");
	}
	return return_ptr;
}