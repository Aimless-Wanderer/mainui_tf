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
	void Draw();
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
			Point( 0, iYOffset ), ExecAndHide( szJoinCommands[i] ) );
		iYOffset += BTN_HEIGHT + BTN_GAP;
	}

	AddButton( '5', L( "#Team_AutoAssign" ),
		Point( 0, iYOffset ), ExecAndHide( "jointeam 5" ));

	AddButton( '6', L( "#Menu_Spectate" ),
		Point( 0, iYOffset + BTN_HEIGHT + BTN_GAP ), ExecAndHide( "jointeam 6" ));

	szTitle = L( "#Title_SelectYourTeam" );
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