#pragma once

#define BTN_HEIGHT 48
#define BTN_WIDTH 256
#define BTN_GAP 12

enum
{
	BTN_CUSTOM,
	BTN_MAP,
	BTN_TEAM,
	BTN_TOGGLE,
	BTN_CLASS
};

extern const char *szClassCommands[];
extern const char *szClassLabels[];
extern const char *szClassNames[];