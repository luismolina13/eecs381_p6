/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Islands and Ships there are, but it does not
know about any of their derived classes, nor which Ships are of what kind of Ship. 
It has facilities for looking up objects by name, and removing Ships.  When
created, it creates an initial group of Islands and Ships using the Ship_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.
*/

/* 
This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If no protected members are shown, there must be none in your version. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

#ifndef MODEL_H
#define MODEL_H

#include "Ship_factory.h"
#include "Utility.h"
#include <string>
#include <map>
#include <set>
#include <memory>

// Declare the global model pointer
class Model;

// Forward declarations
class Sim_object;
class Island;
class Ship;
class View;

class Model {
public:	
	//create the model if it doesn't already exist
	static Model& getInstance() {
		static Model model;
		return model;
	}
	// return the current time
	int get_time() {return time;}

	// is name already in use for either ship or island?
    // either the identical name, or identical in first two characters counts as in-use
	bool is_name_in_use(const std::string& name) const;

	// is there such an island?
	bool is_island_present(const std::string& name) const;
	// add a new island to the lists
	void add_island(std::shared_ptr<Island>);
	// will throw Error("Island not found!") if no island of that name
	std::shared_ptr<Island> get_island_ptr(const std::string& name) const;

	// is there such an ship?
	bool is_ship_present(const std::string& name) const;
	// add a new ship to the list, and update the view
	void add_ship(std::shared_ptr<Ship>);
	// will throw Error("Ship not found!") if no ship of that name
	std::shared_ptr<Ship> get_ship_ptr(const std::string& name) const;
	/*	find and return a sim_object pointer by name, will throw 
		Error("Sim_object not found!") if not object of that name,
		but this should only be called from functions within the program
		so no user input is being processed */
	std::shared_ptr<Sim_object> get_object_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves
	void describe() const;
	// increment the time, and tell all objects to update themselves
	// then update the view.
	void update();	
	
    /* Note: In Project 4 there is only one View. There will be multiple View objects
    later. So implement the View services so that multiple Views are possible by
    using a container of View pointers.  You should delete this comment. */
    
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::shared_ptr<View>);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View>);
	
    // notify the views about an object's location
	void notify_location(const std::string& name);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);

	ShipData get_data_from_ship(const std::string& name);

	/* services for island information */
	// given a position, return a pointer to the island at that point
	std::shared_ptr<Island> getIslandFromPosition(Point location);

	/*	given a set of visited islands, find the one closest to the current
		location that is not in the set */
	std::shared_ptr<Island> getNearestIsland(Point location, 
				std::set<std::shared_ptr<Island>> islandsTraveled);

	
private:
	int time;										// the simulated time
	std::map<std::string, std::shared_ptr<Sim_object>> sim_objects;	// shared_ptr<Sim_object> container
	std::map<std::string, std::shared_ptr<Island>> islands;			// shared_ptr<Island> container
	std::map<std::string, std::shared_ptr<Ship>> ships;			// shared_ptr<Ship> container
	std::set<std::shared_ptr<View>> views;							// shared_ptr<View> container

	// create the initial objects, output constructor message
	// private for singleton pattern
	Model();
	// destroy all objects, output destructor message
	~Model();

	// disallow copy/move construction or assignment
	Model(const Model&) = delete; 				// disallow copy construction
	Model(Model&&) = delete; 					// disallow move construction
	Model& operator=(const Model&) = delete; 	// disallow copy assignment
	Model& operator=(Model&&) = delete; 		// disallow move assignment
};

bool firstTwoLettersMatch(std::string s1, std::string s2);

#endif