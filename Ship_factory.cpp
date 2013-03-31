#include "Ship_factory.h"
#include "Tanker.h"
#include "Cruiser.h"
#include "Utility.h"
#include <memory>
using std::shared_ptr; using std::make_shared;

shared_ptr<Ship> create_ship(const std::string& name, const std::string& type, 
											Point initial_position) {

	shared_ptr<Ship> return_ptr;
	if(type == "Tanker") {
		return_ptr = make_shared<Tanker>(name, initial_position); //todo shared
	} else if (type == "Cruiser") {
		return_ptr = make_shared<Cruiser>(name, initial_position); //todo shared
	} else {
		throw Error("Trying to create ship of unknown type!");
	}
	return return_ptr;
}