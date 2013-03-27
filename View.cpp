#include "Utility.h"
#include "View.h"
#include <iostream>
#include <ios>
#include <cmath>
#include <vector>
#include <utility>
#include <string>
using std::cout; using std::endl; using std::ios;
using std::pair;
using std::vector;
using std::string;

View::View() {
	size = default_size_c;
	scale = default_scale_c;
	origin = Point(default_origin_c, default_origin_c);
	cout << "View constructed" << endl;
}

View::~View() {
	cout << "View destructed" << endl;
}

void View::update_location(const std::string& name, Point location) {
	/*	If locations contains an item at 'name', the reference to its
		mapped value is updated. Otherwise, an item is inserted and 
		its mapped value is set to location; */
	locations[name] = location;
}

void View::update_remove(const std::string& name) {
	locations.erase(name);
}

void View::draw() {
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

void View::clear() {
	locations.clear();
}

void View::set_size(int size_) {
	if(size_ > 30) {
		throw Error("New map size is too big!");
	} else if (size_ <= 6) {
		throw Error("New map size is too small!"); 
	}
	size = size_;
}

void View::set_scale(double scale_) {
	if(scale_ < 0) {
		throw Error("New map scale must be positive!");
	}
	scale = scale_;
}

void View::set_origin(Point origin_) {
	origin = origin_;
}

void View::set_defaults() {
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
bool View::get_subscripts(int &ix, int &iy, Point location)
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

