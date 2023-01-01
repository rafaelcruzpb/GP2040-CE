using namespace std;
#include <string>
#include <vector>
#include "OneBitDisplay.h"
#include "gamepad.h"

struct MenuItem {
	string description;
	MenuItem(string d){
		description = d;
	};
};

class DisplayMenu
{
private:
	OBDISP *obd;
  bool inGamepadAction = false;
	int8_t selected = 0;
	vector<MenuItem> menu;
  Gamepad* gamepad;
  void processGamepad();
public:
	DisplayMenu(OBDISP *obd);
	void processMenu();
};