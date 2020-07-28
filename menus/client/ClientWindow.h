#pragma once

#include "BaseClientWindow.h"
#include "Action.h"
#include "EventSystem.h"
#include "ClientMenuButton.h"

#define TF_DEFS_ONLY
#include "tf_defs.h"

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

class CClientWindow : public CMenuBaseClientWindow
{
public:
	typedef CMenuBaseClientWindow BaseClass;
	CClientWindow( const char *name = "CClientWindow" ) : BaseClass( name ) {}

	void Show() override;
	void Hide() override;
	bool KeyDown( int key ) override;
	bool KeyUp( int key ) override;
	void VidInit() override;
	void Draw() override;

protected:
	const char *szTitle;
};
