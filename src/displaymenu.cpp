#include "displaymenu.h"
#include "storagemanager.h"

DisplayMenu::DisplayMenu(OBDISP *obd) {
	this->obd = obd;
    this->gamepad = Storage::getInstance().GetProcessedGamepad();
    this->menu = {
        {
            "SETTINGS",  
        },
        {
            "PIN MAPPING",  
        },
        {
            "LED",  
        },
        {
            "DISPLAY",  
        },
        {
            "BUZZER",  
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
    
    string indexString = to_string(selected+1)+"/"+to_string(this->menu.size());
    obdWriteString(obd, 0, 0, 0, (char*) getStringCentered("MENU").c_str(), FONT_6x8, 0, 0);
    obdWriteString(obd, 0, 0, 7, (char*) getStringCentered(indexString + " " + m.description).c_str(), FONT_6x8, 0, 0);
}

string DisplayMenu::getStringCentered(string str) {
    uint8_t filler = (DISPLAY_MENU_TEXT_LIMIT / 2) - (str.length() / 2);
    str.insert(0, filler, ' ');
    str.append(filler, ' ');
    return str;
}