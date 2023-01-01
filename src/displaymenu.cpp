#include "displaymenu.h"
#include "storagemanager.h"

DisplayMenu::DisplayMenu(OBDISP *obd) {
	this->obd = obd;
    this->gamepad = Storage::getInstance().GetProcessedGamepad();
    this->menu = {
        {
            "Display",  
        },
        {
            "Buttons",  
        },
        {
            "About",  
        },
    };
}

void DisplayMenu::processGamepad() {

    if(!this->inGamepadAction && this->gamepad->pressedLeft()) {
        this->inGamepadAction = true;
        this->selected--;
        if (this->selected < 0) this->selected = this->menu.size()-1;
    } else if (!this->inGamepadAction && this->gamepad->pressedRight()) {
        this->inGamepadAction = true;
        this->selected++;
        if (this->selected > this->menu.size()-1) this->selected = 0;
    }

    if (!this->gamepad->state.dpad) this->inGamepadAction = false;
}

void DisplayMenu::processMenu() {
    this->processGamepad();

    MenuItem m = this->menu[selected];
    obdWriteString(obd, 0, 0, 5, (char*)m.description.c_str(), FONT_6x8, 0, 0);
}