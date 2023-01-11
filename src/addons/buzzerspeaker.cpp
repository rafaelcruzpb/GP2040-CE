#include "hardware/pwm.h"
#include "songs.h"
#include "storagemanager.h"
#include "math.h"
#include "usb_driver.h"

bool BuzzerSpeakerAddon::available() {
	BoardOptions boardOptions = Storage::getInstance().getBoardOptions();
	return boardOptions.buzzerEnabled && 
		boardOptions.buzzerPin != (uint8_t)-1;
}

void BuzzerSpeakerAddon::setup() {
	BoardOptions boardOptions = Storage::getInstance().getBoardOptions();

	gpio_set_function(boardOptions.buzzerPin, GPIO_FUNC_PWM);
	buzzerPinSlice = pwm_gpio_to_slice_num (boardOptions.buzzerPin); 
	buzzerPinChannel = pwm_gpio_to_channel (boardOptions.buzzerPin);
	buzzerVolume = boardOptions.buzzerVolume;
	introPlayed = false;
}

void BuzzerSpeakerAddon::process() {
	if (!introPlayed) playIntro();

	processBuzzer();
}

void BuzzerSpeakerAddon::playIntro() {
	if (getMillis() < 1000) return;

	BoardOptions boardOptions = Storage::getInstance().getBoardOptions();

	bool isConfigMode = Storage::getInstance().GetConfigMode();
	
	if (!get_usb_mounted() || isConfigMode) {
		play("CONFIG_MODE_SONG");
	} else if (boardOptions.buzzerIntroSong == 0) {
		play(boardOptions.buzzerCustomIntroSongToneDuration, boardOptions.buzzerCustomIntroSong);
	} else {
		play(boardOptions.buzzerIntroSong-1);
	}
	introPlayed = true;
}

void BuzzerSpeakerAddon::processBuzzer() {

	if (currentSong == NULL) {
		return;
	}

	uint32_t currentTimeSong = getMillis() - startedSongMils;
	uint32_t totalTimeSong = currentSong->song.size() * currentSong->toneDuration;
	uint16_t currentTonePosition = floor((currentTimeSong * currentSong->song.size()) / totalTimeSong);
	uint16_t currentTone = currentSong->song[currentTonePosition];

	if (currentTonePosition >= currentSong->song.size()) {
		stop();
		return;
	}

	if (currentTone == PAUSE) {
		pwm_set_enabled (buzzerPinSlice, false);
		return;
	}

	pwmSetFreqDuty(buzzerPinSlice, buzzerPinChannel, currentTone, 0.03 * ((float) buzzerVolume));
	pwm_set_enabled (buzzerPinSlice, true);
}

void BuzzerSpeakerAddon::play(const Song *song) {
	startedSongMils = getMillis();
	currentSong = (Song *) song;
}

void BuzzerSpeakerAddon::play(uint8_t song) {
	if (song > -1 && songs.size() > song) {
		play(&songs[song]);
	}
}

void BuzzerSpeakerAddon::play(string song) {
	for (int s = 0; s < songs.size()-1; s++) {
		if (songs[s].name == song) {
			play(&songs[s]);
		}
	};
}

void BuzzerSpeakerAddon::play(uint8_t toneDuration, char songNotes[512]) {
	currentSong = new Song("", toneDuration, explode(songNotes, ',')); 
	startedSongMils = getMillis();
}

void BuzzerSpeakerAddon::stop() {
	pwm_set_enabled (buzzerPinSlice, false);
	currentSong = NULL;
}

vector<int> BuzzerSpeakerAddon::explode(const string& str, const char& ch)  {
	string next;
    vector<int> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(stoi(next));
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(stoi(next));
    return result;
}

uint32_t BuzzerSpeakerAddon::pwmSetFreqDuty(uint slice, uint channel, uint32_t frequency, float duty) {
	uint32_t clock = 125000000;
	uint32_t divider16 = clock / frequency / 4096 + 
							(clock % (frequency * 4096) != 0);
	if (divider16 / 16 == 0)
	divider16 = 16;
	uint32_t wrap = clock * 16 / divider16 / frequency - 1;
	pwm_set_clkdiv_int_frac(slice, divider16/16,
										divider16 & 0xF);
	pwm_set_wrap(slice, wrap);
	pwm_set_chan_level(slice, channel, wrap * duty / 100);
	return wrap;
}

