#include "Views.h"
#include "Utility.h"
#include "Sim_object.h"
#include "Model.h"
#include "Ship.h"
#include <iostream>
#include <iomanip>
#include <ios>
#include <cmath>
#include <vector>
#include <utility>
#include <string>
#include <memory>
using std::cout; using std::endl; using std::ios; using std::setw;
using std::pair;
using std::vector;
using std::string;
using std::shared_ptr;

const int axes_interval_c = 3;
const int default_size_c = 25;
const double default_scale_c = 2.0;
const double default_origin_c = -10;

/*---------------------------- MAP VIEW CLASS ----------------------------*/

Map_view::Map_view() : 	size(default_size_c), scale(default_scale_c), 
					origin(Point(default_origin_c, default_origin_c)) {
	//cout << "View constructed" << endl;
}

Map_view::~Map_view() {
	//cout << "View destructed" << endl;
}

void Map_view::update(const std::string& name) {
	/*	If locations contains an item at 'name', the reference to its
		mapped value is updated. Otherwise, an item is inserted and 
		its mapped value is set to its location; */
	shared_ptr<Sim_object> obj = Model::getInstance().get_object_ptr(name);
	locations[name] = obj->get_location();
}

void Map_view::update_remove(const std::string& name) {
	locations.erase(name);
}

void Map_view::draw() {
	// save old settings
	ios::fmtflags old_settings = cout.flags();
	int old_precision = cout.precision();

	//plot items on matrix, store those outside of it in outside_map
	vector<vector<string>> matrix(size, 
									vector<string>(size, ". "));
	vector<string> outside_map;
	for(auto loc_pair: locations) {
		int x, y;
 		if(get_subscripts(x, y, loc_pair.second)) {
 			string& point = matrix[size-1-y][x];
 			if(point == ". ") {
				point = loc_pair.first.substr(0, 2);
			} else {
				point = "* ";
			}
		} else {
			outside_map.push_back(loc_pair.first);
		}	
	}

	//print header and ships outside of map
	cout << "Display size: " <<  size << ", scale: " << scale << 
									", origin: " << origin << endl;
	if(!outside_map.empty()) {
		for(auto it = outside_map.begin(); it != outside_map.end(); it++) {
			cout << *it;
			if(it + 1 != outside_map.end()) {
				cout << ", ";
			}
		}
		cout << " outside the map" << endl;
	}

	// set output stream formatting
	cout << std::fixed;
	cout.precision(0);

	//number of axes labels there will be
	int num_labels = (size - 1)/axes_interval_c;
	// use to determine if the current row is divisible by axes_interval
	int axes_counter = 0;
	int labels_counter_y = num_labels;
	//print the grid
	for(auto vec: matrix) {
		if((size-1-axes_counter) % axes_interval_c == 0) {
			cout.width(4);
			double out = (origin.y + (labels_counter_y-- * (scale * axes_interval_c)));
			cout << out;
			cout << " ";
		} else {
			cout.width(5);
			cout << " ";
		}
		for(auto str: vec) {
			cout << str;
		}

		cout << endl;
		axes_counter++;
	}

	// print bottom axes
	int labels_counter_x = 0;
	for(int j = 0; j < size; j++) {
		if(j % axes_interval_c == 0) {
			cout.width(6);
			cout << (origin.x + (labels_counter_x++ * (scale * axes_interval_c)));
		}
	}
	cout << endl;

	// restore old settings
	cout.flags(old_settings);
	cout.precision(old_precision);

}

void Map_view::clear() {
	locations.clear();
}

void Map_view::set_size(int size_) {
	if(size_ > 30) {
		throw Error("New map size is too big!");
	} else if (size_ <= 6) {
		throw Error("New map size is too small!"); 
	}
	size = size_;
}

void Map_view::set_scale(double scale_) {
	if(scale_ < 0) {
		throw Error("New map scale must be positive!");
	}
	scale = scale_;
}

void Map_view::set_origin(Point origin_) {
	origin = origin_;
}

void Map_view::set_defaults() {
	size = default_size_c;
	scale = default_scale_c;
	origin = Point(default_origin_c, default_origin_c);
}


/* *** Use this function to calculate the subscripts for the cell. */
/* *** This code assumes the specified private member variables. */
// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Map_view::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will produce integer smaller than even for negative values, 
	// so - 0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

/*---------------------------- DATA VIEW CLASS ----------------------------*/

void Data_view::update(const std::string& name) {
	try{
		ShipData sd = Model::getInstance().get_data_from_ship(name);
		ships[name] = sd;
	} catch(Error& e) {
		//update by island, no action needed for data view
	}
}

void Data_view::update_remove(const std::string& name) {
	ships.erase(name);
}

void Data_view::draw() {
	double not_moving_speed = 0;
	cout << "----- Sailing Data -----" << endl;
	cout << setw(10) << "Ship" << setw(10) << "Fuel" << setw(10) << "Course" 
		<< setw(10) << "Speed" << endl;
	for(auto cur: ships) {
		cout << setw(10) << cur.first << setw(10) << cur.second.fuel << setw(10) <<  
			cur.second.course << setw(10) << cur.second.speed << endl;
		//if(Model::getInstance().get_ship_ptr(cur.first)->is_moving()) {
		//	cout << setw(10) << cur.second.speed << endl;
		//} else {
		//	cout << setw(10) << not_moving_speed << endl;
		//}
	}
}

void Data_view::clear()  {
	ships.clear();
}

/*--------------------------- BRIDGE VIEW CLASS ---------------------------*/

const int bridge_height = 3;
const int bridge_width = 19;

void Bridge_view::update(const std::string& name) {
	shared_ptr<Sim_object> obj = Model::getInstance().get_object_ptr(name);
	locations[name] = obj->get_location();
}

void Bridge_view::update_remove(const std::string& name) {
	locations.erase(name);
}

void Bridge_view::draw() {
	if(!(ownship->is_afloat())) { //draw water view
		cout << "Bridge view from " << ownship->get_name() << " sunk at " 
										<< ownship->get_location() << endl;
		vector<vector<string>> matrix(bridge_height, vector<string>(bridge_width, "w-"));
		draw_matrix(matrix);
		return;
	}
	cout << "Bridge view from " << ownship->get_name() << " position " << 
		ownship->get_location() << " heading " << ownship->get_ship_course() << endl;
	vector<vector<string>> matrix(bridge_height, vector<string>(bridge_width, ". "));
	
	for(auto cur: locations) { 
		//for each location in the list
		Compass_position cp(ownship->get_location(), cur.second);
		if(cp.range < 20 && cp.range > .005) { //if it is in sight
			int coords = get_coordinates_from_bearing(cp.bearing - ownship->get_ship_course());
			//if the returned bearing is within our forward field of vision
			if(coords >= 0 && coords <= 180) {
				string& point = matrix[2][coords/10];
				//print the appropriate information on the matrix
				if(point == ". ") {
					point = cur.first.substr(0, 2);
				} else {
					point = "**";
				}
			}
		}
	}

	draw_matrix(matrix);
}

void Bridge_view::draw_matrix(vector<vector<string>> &matrix) {
	for(int i = 0; i < bridge_height; i++) {
		cout << setw(7);
		for(int j = 0; j < bridge_width; j++) {
			cout << matrix[i][j];
		}
		cout << endl;
	}

	cout << setw(6) << "-90" << setw(6) << "-60" << setw(6) << "-30" << setw(6)
		<< "0" << setw(6) << "30" << setw(6) << "60" << setw(6) << "90" << endl;
}

int Bridge_view::get_coordinates_from_bearing(double bearing) {
	if(bearing < -180) {
		bearing += 360;
	} else if (bearing > 180) {
		bearing -= 360;
	}

	//add 90 to make values from -90 to positive 90 fall within the range 0-18 inclusive
	return (bearing + 90);

}

void Bridge_view::clear() {

}
