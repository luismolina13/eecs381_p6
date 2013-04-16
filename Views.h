/* *** View class ***
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

Usage: 
1. Call the update_location function with the name and position of each object
to be plotted. If the object is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the 
supplied location. If a single object changes location, its location can be separately
updated with a call to update_location. 
2. Call the update_remove function with the name of any object that should no longer
be plotted. This must be done *after* any call to update_location that 
has the same object name since update_location will add any object name supplied.
3. Call the draw function to print out the map. 
4. As needed, change the origin, scale, or displayed size of the map 
with the appropriate functions. Since the view "remembers" the previously updated
information, the draw function will print out a map showing the previous objects
using the new settings.
*/

/* 
*** This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <map>
#include "Geometry.h"
#include "Utility.h"
#include <memory>
#include <vector>

struct ShipData {
	double fuel;
	double course;
	double speed;
};

class View {
public:


	//virtual ~View() = 0; todo: why won't this compile?

	/*	fat interface functions for derived view classes. If called for a view
		for which they are undefined, nothing hapens */
	virtual void update_location(const std::string& name, Point location) {};

	virtual void update_course(const std::string& name, double course) {};

	virtual void update_speed(const std::string& name, double speed) {};

	virtual void update_fuel(const std::string& name, double fuel) {};

	/*	remove the item 'name' from this view	*/
	virtual void update_remove(const std::string& name) {};

	/*	print the view information	*/
	virtual void draw() {};

	/*	clear the view information	*/
	virtual void clear() {};
};

class Map_view : public View {
public:
	// default constructor sets the default size, scale, and origin, outputs constructor message
	Map_view(); 
	~Map_view();	// outputs destructor message
	
	// Save the supplied name and location for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	virtual void update_location(const std::string& name, Point location) override;
	
	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove(const std::string& name) override;
	
	// prints out the current map
	virtual void draw() override;
	
	// Discard the saved information - drawing will show only a empty pattern
	virtual void clear() override; 
	
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	virtual void set_size(int size_);
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	virtual void set_scale(double scale_);
	
	// any values are legal for the origin
	virtual void set_origin(Point origin_);
	
	// set the parameters to the default values
	virtual void set_defaults();
				
private:
	int size;			// current size of the display
	double scale;		// distance per cell of the display
	Point origin;		// coordinates of the lower-left-hand corner
	std::map<std::string, Point> locations;
				
	// Calculate the cell subscripts corresponding to the location parameter, using the 
	// current size, scale, and origin of the display. 
	// Return true if the location is within the map, false if not
	bool get_subscripts(int &ix, int &iy, Point location); 

};

class Data_view : public View {
public:

	/*	get new data from the model	*/
	//virtual void update_location(const std::string& name, Point location, ShipData sd) override;

	virtual void update_course(const std::string& name, double course) override;
	virtual void update_speed(const std::string& name, double speed) override;
	virtual void update_fuel(const std::string& name, double fuel) override;

	/*	inform view that the ship is gone	*/
	virtual void update_remove(const std::string& name) override;
	
	/*	print the sailing data 	*/
	virtual void draw() override;

	/*	discard all ships from the sailing data	*/
	//virtual void clear() override; 

private:
	std::map<std::string, ShipData> ships;
};

class Bridge_view : public View {
public:
	Bridge_view(const std::string& ownship_name_) : 
				View(), ownship_name(ownship_name_), is_ownship_afloat(true){}

	/*	get new data from the model	*/
	virtual void update_location(const std::string& name, Point location) override;

	virtual void update_course(const std::string& name, double course);

	/*	inform view that the ship is gone (sunk)	*/
	virtual void update_remove(const std::string& name) override;
	
	/*	print the sailing data 	*/
	virtual void draw() override;

	/*	discard all ships from the sailing data	*/
	virtual void clear() override; 

private:
	std::string	ownship_name;
	Point ownship_location;
	double ownship_course;
	bool is_ownship_afloat;
	std::map<std::string, Point> locations;

	void draw_matrix(std::vector<std::vector<std::string>> &matrix);
	int get_coordinates_from_bearing(double bearing);
};

#endif