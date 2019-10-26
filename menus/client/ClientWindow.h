#pragma once

#include "BaseClientWindow.h"
#include "Action.h"
#include "EventSystem.h"

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

class CClientWindow : public CMenuBaseClientWindow
{
public:
	typedef CMenuBaseClientWindow BaseClass;
	CClientWindow( const char *name = "CClientWindow" ) : BaseClass( name ) {}
	~CClientWindow();

	void Show() override;
	void Hide() override;

	CEventCallback ExecAndHide( const char *szCmd );

	CMenuAction *AddButton( int key, const char *name, Point pos, CEventCallback callback );

	bool KeyDown( int key );
	void VidInit() override;
	void Draw() override;
	CEventCallback keys[10];

protected:
	CUtlVector<CMenuAction *> m_pButtons;
	const char *szTitle;
};
