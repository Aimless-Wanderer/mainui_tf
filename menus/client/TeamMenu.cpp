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

	void Update(char teamNames[4][32], int numTeams )
	{
		m_iNumTeams = numTeams;

		for ( int i = 0; i < numTeams; i++ )
			strcpy( m_sTeamNames[i], teamNames[i] );
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
	
	int m_iNumTeams;
	char m_sTeamNames[4][32];

} uiTeamMenu;

void CClientTeamMenu::_Init()
{
	int iYOffset = 80;

	for ( int i = 0; i < m_iNumTeams; i++ )
	{
		char cmd[32];
		sprintf( cmd, "jointeam %i", i + 1 );
		AddButton( ( i + 1 ) + '0', L( m_sTeamNames[i] ),
			Point( 40, iYOffset ), MakeCb( cmd ) );
		iYOffset += 32;
	}

	AddButton( '5', L( "Team_AutoAssign" ),
		Point( 40, iYOffset ), MakeCb( "jointeam 5" ));

	AddButton( '6', L( "Menu_Spectate" ),
		Point( 40, iYOffset + 32 ), MakeCb( "jointeam 6" ));

	szName = L( "Title_SelectYourTeam" );
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

void UI_TeamMenu_Update( char teamNames[4][32], int numTeams )
{
	uiTeamMenu.Update( teamNames, numTeams );
}