/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

/* 
*** This skeleton file shows the required public and protected interface for the class, which you may not modify. 
If any protected or private members are shown here, then your class must also have them and use them as intended.
You should delete this comment.
*/

#ifndef CONTROLLER_H
#define CONROLLER_H

#include <string>

// Forward declarations
class Model;
class View;
class Ship;
class Island;
class Point;

class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();

private:
	void doViewDefault(View &main_view);
	void doViewSize(View &main_view);
	void doViewZoom(View &main_view);
	void doViewPan(View &main_view);
	void doViewShow(View &main_view);
	void doModelStatus(View &main_view);
	void doModelGo(View &main_view);
	void doModelCreate(View &main_view);
	void doShipCourse(View &main_view);
	void doShipPosition(View &main_view);
	void doShipDestination(View &main_view);
	void doShipLoadAt(View &main_view);
	void doShipUnloadAt(View &main_view);
	void doShipDockAt(View &main_view);
	void doShipAttack(View &main_view);
	void doShipRefuel(View &main_view);
	void doShipStop(View &main_view);
	void doShipStopAttack(View &main_view);
	double getDouble();
	Point getCoords();
	std::string getName();
	double getSpeed();
	Island* getIsland();
	Ship* getShip();

	Ship* current_ship;		// used to hold the ship pointer for ship commands
};

#endif