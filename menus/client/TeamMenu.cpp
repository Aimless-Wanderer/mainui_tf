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
	int iYOffset = 80;
	int iNumTeams = g_pClient->GetNumberOfTeams();
	char **szTeamNames = g_pClient->GetTeamNames();
	const char *szJoinCommands[] = { "jointeam 1", "jointeam 2", "jointeam 3", "jointeam 4" };

	for ( int i = 0; i < iNumTeams; i++ )
	{
		AddButton( ( i + 1 ) + '0', L( szTeamNames[i] ),
			Point( 40, iYOffset ), MakeCb( szJoinCommands[i] ) );
		iYOffset += 32;
	}

	AddButton( '5', L( "#Team_AutoAssign" ),
		Point( 40, iYOffset ), MakeCb( "jointeam 5" ));

	AddButton( '6', L( "#Menu_Spectate" ),
		Point( 40, iYOffset + 32 ), MakeCb( "jointeam 6" ));

	szName = L( "#Title_SelectYourTeam" );
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