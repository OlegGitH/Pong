struct Button_State {
	bool is_down;
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_ESC,
	BUTTON_TAB,
	BUTTON_F1,

	BUTTON_COUNT, // sHOULD BE THE LAST ITEM
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};