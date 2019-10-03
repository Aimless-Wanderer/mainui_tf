#include "BaseMenu.h"
#include "Action.h"
#include "ScrollView.h"
#include "ClientWindow.h"

static class CClientTeamMenu : public CClientWindow
{
public:
	typedef CClientWindow BaseClass;
	CClientTeamMenu() : BaseClass( "CClientTeamMenu" ) {}

	void _Init();
	void Reload();
	void Draw();

	CEventCallback MakeCb( const char *cmd )
	{
		return CEventCallback( MenuCb( &CClientTeamMenu::cb ), (void*)cmd );
	}

private:
	const char *command;
	char textbuffer[1024];

	void cb( void *pExtra )
	{
		command = (const char*)pExtra;
		EngFuncs::ClientCmd( FALSE, command );
		Hide();
	}

} uiTeamMenu;

void CClientTeamMenu::_Init()
{
	AddButton( '1', "BLUE",
		Point( 40, 80 ), MakeCb( "jointeam 1" ));
	AddButton( '2', "RED",
		Point( 40, 112 ), MakeCb( "jointeam 2" ));

	AddButton( '5', "AUTO-ASSIGN",
		Point( 40, 144 ), MakeCb( "jointeam 5" ));
	AddButton( '6', "SPECTATE",
		Point( 40, 176 ), MakeCb( "jointeam 6" ));

	szName = "SELECT YOUR TEAM";
}

void CClientTeamMenu::Reload()
{
}

void CClientTeamMenu::Draw()
{
	BaseClass::Draw();
}

void UI_TeamMenu_Show( void )
{
	EngFuncs::KEY_SetDest( KEY_MENU );
	uiTeamMenu.Show();
}