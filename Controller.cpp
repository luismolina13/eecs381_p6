#include "Controller.h"
#include "Model.h"
#include "Views.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <map>
#include <utility>
#include <string>
#include <memory>
using std::cout; using std::endl; using std::cin;
using std::string;
using std::map;
using std::shared_ptr; using std::make_shared;

typedef map<string, shared_ptr<Bridge_view>> bridge_view_map_t;

struct Views_container {
	shared_ptr<Map_view> map_view;
	shared_ptr<Data_view> data_view;
	bridge_view_map_t bridge_views;
};

typedef map<string, void(Controller::*)(Views_container&)> fn_map;

Controller::Controller() {
	//cout << "Controller constructed" << endl;
}

Controller::~Controller() {
	//cout << "Controller destructed" << endl;
}

void Controller::run() {
	fn_map fMap;
	Views_container views;
	views.map_view = nullptr;
	views.data_view = nullptr;

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
	fMap.insert(fn_map::value_type("open_map_view", &Controller::doOpenMap));
	fMap.insert(fn_map::value_type("close_map_view", &Controller::doCloseMap));
	fMap.insert(fn_map::value_type("open_sailing_view", &Controller::doOpenSailing));
	fMap.insert(fn_map::value_type("close_sailing_view", &Controller::doCloseSailing));
	fMap.insert(fn_map::value_type("open_bridge_view", &Controller::doOpenBridge));
	fMap.insert(fn_map::value_type("close_bridge_view", &Controller::doCloseBridge));

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
				(this->*(it->second))(views);
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

	cout << "Done" << endl;
}

void Controller::doViewDefault(Views_container &views) {
	views.map_view->set_defaults();
}

void Controller::doViewSize(Views_container &views) {
	int size;
	cin >> size;
	if(!cin) {
		throw Error("Expected an integer!");
	}
	views.map_view->set_size(size);
}

void Controller::doViewZoom(Views_container &views) {
	double zoom = getDouble();
	views.map_view->set_scale(zoom);
}

double Controller::getDouble() {
	double input;
	cin >> input;
	if(!cin) {
		throw Error("Expected a double!");
	}
	return input;
}

void Controller::doViewPan(Views_container &views) {
	views.map_view->set_origin(getCoords());
}

Point Controller::getCoords() {
	double x = getDouble();
	double y = getDouble();
	return Point(x, y);
}

void Controller::doViewShow(Views_container &views) {
	if(views.map_view) {
		views.map_view->draw();
	}
	if(views.data_view) {
		views.data_view->draw();
	}
	for(auto cur: views.bridge_views) {
		cur.second->draw();
	}
}

void Controller::doModelStatus(Views_container &views) {
	Model::getInstance().describe();
}

void Controller::doModelGo(Views_container &views) {
	Model::getInstance().update();
}

void Controller::doModelCreate(Views_container &views) {
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

void Controller::doShipCourse(Views_container &views) {
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

void Controller::doShipPosition(Views_container &views) {
	Point dest = getCoords();
	double speed = getSpeed();
	current_ship->set_destination_position_and_speed(dest, speed);
}

void Controller::doShipDestination(Views_container &views) {
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

void Controller::doShipLoadAt(Views_container &views) {
	current_ship->set_load_destination(getIsland());
}

void Controller::doShipUnloadAt(Views_container &views) {
	current_ship->set_unload_destination(getIsland());
}

void Controller::doShipDockAt(Views_container &views) {
	current_ship->dock(getIsland());
}

void Controller::doShipAttack(Views_container &views) {
	current_ship->attack(getShip());
}

shared_ptr<Ship> Controller::getShip() {
	string shipName;
	cin >> shipName;
	return Model::getInstance().get_ship_ptr(shipName);
}

void Controller::doShipRefuel(Views_container &views) {
	current_ship->refuel();
}

void Controller::doShipStop(Views_container &views) {
	current_ship->stop();
}

void Controller::doShipStopAttack(Views_container &views) {
	current_ship->stop_attack();
}

void Controller::doOpenMap(Views_container &views) {
	if(views.map_view != nullptr) {
		throw Error("Map view is already open!");
	}
	views.map_view = make_shared<Map_view>();
	Model::getInstance().attach(views.map_view);
}

void Controller::doCloseMap(Views_container& views) {
	if(views.map_view == nullptr) {
		throw Error("Map view is not open!");
	}
	Model::getInstance().detach(views.map_view);
	views.map_view = nullptr;

}

void Controller::doOpenSailing(Views_container &views) {
	if(views.data_view != nullptr) {
		throw Error("Sailing data view is already open!");
	}
	views.data_view = make_shared<Data_view>();
	Model::getInstance().attach(views.data_view);
}

void Controller::doCloseSailing(Views_container& views) {
	if(views.data_view == nullptr) {
		throw Error("Sailing data view is not open!");
	}
	Model::getInstance().detach(views.data_view);
	views.data_view = nullptr;
}

void Controller::doOpenBridge(Views_container &views) {
	shared_ptr<Ship> ship = getShip();
	string name = ship->get_name();
	if(views.bridge_views.find(name) != views.bridge_views.end()) {
		throw Error("Bridge view is already open for that ship!");
	}
	shared_ptr<Bridge_view> new_view = make_shared<Bridge_view>(ship);
	views.bridge_views.insert(bridge_view_map_t::value_type(name, new_view));
	Model::getInstance().attach(new_view);
}

void Controller::doCloseBridge(Views_container& views) {
	string name;
	cin >> name;
	bridge_view_map_t::iterator it = views.bridge_views.find(name);
	if(it == views.bridge_views.end()) {
		throw Error("Bridge view for that ship is not open!");
	}
	Model::getInstance().detach(it->second);
	views.bridge_views.erase(name);
}
