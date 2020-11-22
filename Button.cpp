
/*
 *  Button.cpp - De-bounced button library for
 *  Arduino. Distinguishes between short and long
 *  button presses. Short press requires release,
 *  long press requires holding for set time.
 *  Default time is 3 seconds.
 *  2020 Tom Clayton
 */

#include "Button.h"

Button::Button(uint8_t input_pin)
    :pin_(input_pin),
     prev_state_(1),
     debounce_lock_(false),
     long_press_(false),
     short_press_function_(NULL),
     long_press_function_(NULL),
     long_press_timeout_(DEFAULT_LONG_PRESS_TIME)
{
	pinMode(pin_, INPUT_PULLUP);
}

// poll - Check button state and proceed as required.

void Button::poll()
{
	// check de-bounce lock:
	if (!debounce_lock_){

		// poll button state:
		bool current_state = digitalRead(pin_);

        // check for button press:
		if (current_state == 0 && prev_state_ == 1){
            press_timer_ = millis();
            long_press_ = false;
		}

		// check for button released before long press flagged:
		else if (current_state == 1 && prev_state_ == 0 && !long_press_
                                        && short_press_function_ != NULL) {
                short_press_function_();
		}

		// check for button held long enough for long press:
		else if (current_state == 0 && !long_press_ && (uint32_t)(millis() - press_timer_) > long_press_timeout_
                                        && long_press_function_ != NULL) {
                long_press_function_();
                long_press_ = true;
        }

        // set the de-bounce lock if state changes:
        if (current_state != prev_state_){
            debounce_timer_ = millis();
            debounce_lock_ = true;
        }
        // update previous state:
		prev_state_ = current_state;
	}
	// unlock de-bounce if time-out passed:
	else if ((uint32_t)(millis() - debounce_timer_) > DEBOUNCE_TIMEOUT){
		debounce_lock_ = false;
	}
}

void Button::setShortPressFunction(void (*spf)())
{
	short_press_function_ = spf;
}

void Button::setLongPressFunction(void (*lpf)())
{
	long_press_function_ = lpf;
}

void Button::setLongPressTime(uint16_t timeout)
{
    long_press_timeout_ = timeout;
}
