#include "Ship_factory.h"
#include "Tanker.h"
#include "Cruiser.h"
#include "Utility.h"

Ship * create_ship(const std::string& name, const std::string& type, 
											Point initial_position) {

	Ship* return_ptr;
	if(type == "Tanker") {
		return_ptr = new Tanker(name, initial_position);
	} else if (type == "Cruiser") {
		return_ptr = new Cruiser(name, initial_position);
	} else {
		throw Error("Trying to create ship of unknown type!");
	}
	return return_ptr;
}