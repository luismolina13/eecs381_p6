#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <map>
#include <utility>
#include <string>
using std::cout; using std::endl; using std::cin;
using std::string;
using std::map;
using std::shared_ptr;

typedef map<string, void(Controller::*)(View&)> fn_map;

Controller::Controller() {
	cout << "Controller constructed" << endl;
}

Controller::~Controller() {
	cout << "Controller destructed" << endl;
}

void Controller::run() {
	fn_map fMap;
	shared_ptr<View> main_view(new View);
	Model::getInstance().attach(main_view);

	fMap.insert(fn_map::value_type("default", &Controller::doViewDefault));
	fMap.insert(fn_map::value_type("size", &Controller::doViewSize));
	fMap.insert(fn_map::value_type("zoom", &Controller::doViewZoom));
	fMap.insert(fn_map::value_type("pan", &Controller::doViewPan));
	fMap.insert(fn_map::value_type("show", &Controller::doViewShow));
	fMap.insert(fn_map::value_type("status", &Controller::doModelStatus));
	fMap.insert(fn_map::value_type("go", &Controller::doModelGo));
	fMap.insert(fn_map::value_type("create", &Controller::doModelCreate));
	fMap.insert(fn_map::value_type("course", &Controller::doShipCourse));
	fMap.insert(fn_map::value_type("position", &Controller::doShipPosition));
	fMap.insert(fn_map::value_type("destination", &Controller::doShipDestination));
	fMap.insert(fn_map::value_type("load_at", &Controller::doShipLoadAt));
	fMap.insert(fn_map::value_type("unload_at", &Controller::doShipUnloadAt));
	fMap.insert(fn_map::value_type("dock_at", &Controller::doShipDockAt));
	fMap.insert(fn_map::value_type("attack", &Controller::doShipAttack));
	fMap.insert(fn_map::value_type("refuel", &Controller::doShipRefuel));
	fMap.insert(fn_map::value_type("stop", &Controller::doShipStop));
	fMap.insert(fn_map::value_type("stop_attack", &Controller::doShipStopAttack));

	while(true) {
		try{
			current_ship = nullptr;
			cout << "\nTime " << Model::getInstance().get_time() << ": Enter command: ";
			string first_word;
			cin >> first_word;

			fn_map::iterator it = fMap.end();
			if(first_word == "quit") {
				break;
			} else if (Model::getInstance().is_ship_present(first_word)) {
				//the first word was a ship name, so use the second word
				current_ship = Model::getInstance().get_ship_ptr(first_word);
				string second_word;
				cin >> second_word;
				it = fMap.find(second_word);
			} else { //the word is a command word, so use the first_word
				it = fMap.find(first_word);
			}

			if(it != fMap.end()) {
				(this->*(it->second))(*main_view);
			} else {
				throw Error("Unrecognized command!");
			}
		} catch (Error& e) {
			cout << e.msg << endl;
			// clear input stream and rest of line
			cin.clear();
			while(cin.get() != '\n');
		}
	}

	Model::getInstance().detach(main_view);
	cout << "Done" << endl;
}

void Controller::doViewDefault(View &main_view) {
	main_view.set_defaults();
}

void Controller::doViewSize(View &main_view) {
	int size;
	cin >> size;
	if(!cin) {
		throw Error("Expected an integer!");
	}
	main_view.set_size(size);
}

void Controller::doViewZoom(View &main_view) {
	double zoom = getDouble();
	main_view.set_scale(zoom);
}

double Controller::getDouble() {
	double input;
	cin >> input;
	if(!cin) {
		throw Error("Expected a double!");
	}
	return input;
}

void Controller::doViewPan(View &main_view) {
	main_view.set_origin(getCoords());
}

Point Controller::getCoords() {
	double x = getDouble();
	double y = getDouble();
	return Point(x, y);
}

void Controller::doViewShow(View &main_view) {
	main_view.draw();
}

void Controller::doModelStatus(View &main_view) {
	Model::getInstance().describe();
}

void Controller::doModelGo(View &main_view) {
	Model::getInstance().update();
}

void Controller::doModelCreate(View &main_view) {
	string name;
	cin >> name;
	if(name.size() < 2) {
		throw Error("Name is too short!");
	} else if (Model::getInstance().is_name_in_use(name)) {
		throw Error("Name is already in use!");
	}

	string ship_type;
	cin >> ship_type;

	//add ship with this name, the read-in type, and read-in coordinates
	Model::getInstance().add_ship(shared_ptr<Ship>(create_ship(name, ship_type, getCoords())));

}

string Controller::getName() {
	string name;
	cin >> name;
	if(name.size() < 2) {
		throw Error("Name is too short!");
	} else if (Model::getInstance().is_name_in_use(name)) {
		throw Error("Name is already in use!");
	}

	return name;
}

void Controller::doShipCourse(View &main_view) {
	double heading = getDouble();

	if(heading < 0 || heading >= 360) {
		throw Error("Invalid heading entered!");
	}

	current_ship->set_course_and_speed(heading, getSpeed());
}

double Controller::getSpeed() {
	double speed = getDouble();
	if (speed < 0) {
		throw Error("Negative speed entered!");
	}
	return speed;
}

void Controller::doShipPosition(View &main_view) {
	Point dest = getCoords();
	double speed = getSpeed();
	current_ship->set_destination_position_and_speed(dest, speed);
}

void Controller::doShipDestination(View &main_view) {
	shared_ptr<Island> destIsland = getIsland();
	double speed = getSpeed();
	//get island pointer and retrieve location, set destination to location
	current_ship->set_destination_position_and_speed(
									destIsland->get_location(), speed);
}

shared_ptr<Island> Controller::getIsland() {
	string islandName;
	cin >> islandName;
	return Model::getInstance().get_island_ptr(islandName);
}

void Controller::doShipLoadAt(View &main_view) {
	current_ship->set_load_destination(getIsland());
}

void Controller::doShipUnloadAt(View &main_view) {
	current_ship->set_unload_destination(getIsland());
}

void Controller::doShipDockAt(View &main_view) {
	current_ship->dock(getIsland());
}

void Controller::doShipAttack(View &main_view) {
	current_ship->attack(getShip());
}

shared_ptr<Ship> Controller::getShip() {
	string shipName;
	cin >> shipName;
	return Model::getInstance().get_ship_ptr(shipName);
}

void Controller::doShipRefuel(View &main_view) {
	current_ship->refuel();
}

void Controller::doShipStop(View &main_view) {
	current_ship->stop();
}

void Controller::doShipStopAttack(View &main_view) {
	current_ship->stop_attack();
}
