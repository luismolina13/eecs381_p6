#include "Island.h"
#include "Model.h"
using std::cout; using std::endl;

Island::Island (const std::string& name_, Point position_, double fuel_,
				double production_rate_) : Sim_object(name_), 
				position(position_), fuel(fuel_), 
				production_rate(production_rate_) {
	//cout << "Island " << get_name() << " constructed" << endl;
}

Island::~Island() {
	//cout << "Island " << get_name() << " destructed" << endl;
}

double Island::provide_fuel(double request) {
	double amount = 0;
	if(request < fuel) {
		amount = request;
		fuel -= request;
	} else {
		amount = fuel;
		fuel = 0;
	}
	cout << "Island " << get_name() << " supplied " << amount << " tons of fuel" << endl;
	return amount;
}

void Island::accept_fuel(double amount) {
	fuel += amount;
	cout << "Island " << get_name() << " now has " << fuel << " tons" << endl;
}

void Island::update() {
	double amount = 0;
	if(production_rate >0) {
		amount = production_rate * 1.0;
		fuel += amount;
	}
	if(production_rate > 0) {
		cout << "Island " << get_name() << " now has " << fuel << " tons" << endl;
	}
}

void Island::describe() const {
	cout << "\nIsland " << get_name() << " at position " << position << endl;
	cout << "Fuel available: " << fuel << " tons" << endl;
}

void Island::broadcast_current_state() {
	Model::getInstance().notify_location(get_name(), get_location());
}
