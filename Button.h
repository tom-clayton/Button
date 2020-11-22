
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define DEBOUNCE_TIMEOUT	500
#define DEFAULT_LONG_PRESS_TIME		3000

class Button {
	public:
		Button(uint8_t);
		void poll();
		void setLongPressFunction(void (*lpf)());
		void setShortPressFunction(void (*spf)());
		void setLongPressTime(uint16_t);

	private:
		uint8_t pin_;
		bool prev_state_;
		bool debounce_lock_;
		bool long_press_;
		uint32_t press_timer_;
		uint32_t debounce_timer_;
		void (*long_press_function_)();
		void (*short_press_function_)();
		uint16_t long_press_timeout_;
};

#endif
