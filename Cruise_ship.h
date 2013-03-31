#ifndef CRUISE_SHIP_H
#define CRUISE_SHIP_H

#include "Ship.h"

class Cruise_ship : public Ship {

public:
	Cruise_ship(const std::string& name_, Point position_);

	~Cruise_ship();

	void update();

	void describe();

private:

};

#endif