#include "BaseMenu.h"

static class CClientTeamMenu : public CClientWindow
{
public:
	typedef CClientWindow BaseClass;
	CClientJoinGame() : BaseClass( "CClientJoinGame" ) {}

	void _Init();
	void Reload();
	void Draw();

	bool hasSpectator;
	bool hasVIP;
	bool hasCancel;
	CMenuAction *spectate;
	CMenuAction *vipbutton;
	CMenuAction *confirm;
	CMenuAction *cancel;
	CMenuAction text;
	CMenuScrollView scroll;

	CEventCallback MakeCb( const char *cmd )
	{
		return CEventCallback( MenuCb( &CClientJoinGame::cb ), (void*)cmd );
	}

private:
	const char *command;
	char textbuffer[1024];

	void ConfirmSelection()
	{
		EngFuncs::ClientCmd( FALSE, command );
		Hide();
	}

	void cb( void *pExtra )
	{
		command = (const char*)pExtra;
		confirm->Show();
	}

} uiTeamMenu;

void UI_TeamMenu_Show( void )
{
	EngFuncs::KEY_SetDest( KEY_MENU );
	uiTeamMenu.Show();
}