#ifndef CRUISE_SHIP_H
#define CRUISE_SHIP_H

#include "Ship.h"
#include <set>
#include <memory>

enum CruiseState_e {NO_CRUISE_DESTINATIONS, CRUISE_DOCKED, REFUEL, 
												SIGHTSEE, CRUISING};

class Island; //todo: necessary?

class Cruise_ship : public Ship {

public:
	Cruise_ship(const std::string& name_, Point position_);

	~Cruise_ship();

	void update() override;

	void describe() const override;

	//override ship functions to cancel cruise before calling
	void set_destination_position_and_speed(Point destination_position, double speed) override;
	void set_course_and_speed(double course, double speed) override;
	void stop() override;
	void dock(std::shared_ptr<Island> island_ptr) override;
	void refuel() override;

private:
	std::set<std::shared_ptr<Island>> islandsTraveled;
	std::shared_ptr<Island> cruise_start;
	std::shared_ptr<Island> next_destination;
	CruiseState_e cruise_state;
	double cruise_speed;
	bool cruise_over;

	void cancelCruise();
	void set_no_cruise_destinations();
	bool is_cruise_over();

};

#endif