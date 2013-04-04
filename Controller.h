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
#include <memory>

// Forward declarations
class Model;
class View;
class Ship;
class Island;
class Point;
struct Views_container;

class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();

private:
	void doViewDefault(Views_container &views);
	void doViewSize(Views_container &views);
	void doViewZoom(Views_container &views);
	void doViewPan(Views_container &views);
	void doViewShow(Views_container &views);
	void doModelStatus(Views_container &views);
	void doModelGo(Views_container &views);
	void doModelCreate(Views_container &views);
	void doShipCourse(Views_container &views);
	void doShipPosition(Views_container &views);
	void doShipDestination(Views_container &views);
	void doShipLoadAt(Views_container &views);
	void doShipUnloadAt(Views_container &views);
	void doShipDockAt(Views_container &views);
	void doShipAttack(Views_container &views);
	void doShipRefuel(Views_container &views);
	void doShipStop(Views_container &views);
	void doShipStopAttack(Views_container &views);
	void doOpenMap(Views_container &views);
	void doCloseMap(Views_container& views);
	void doOpenSailing(Views_container &views);
	void doCloseSailing(Views_container& views);
	void doOpenBridge(Views_container &views);
	void doCloseBridge(Views_container& views);	

	double getDouble();
	Point getCoords();
	std::string getName();
	double getSpeed();
	std::shared_ptr<Island> getIsland();
	std::shared_ptr<Ship> getShip();
	void removeFromViewsList(std::shared_ptr<View>, Views_container& views);

	std::shared_ptr<Ship> current_ship;		// used to hold the ship pointer for ship commands
};

#endif