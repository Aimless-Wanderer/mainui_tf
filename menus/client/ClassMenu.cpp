#include "BaseMenu.h"
#include "Action.h"
#include "ScrollView.h"
#include "ClientWindow.h"

const char *szClassCommands[] = { "scout", "sniper", "soldier", "demoman", "medic", "hwguy", "pyro", "spy", "engineer" };
const char *szClassLabels[] = { "#Scout", "#Sniper", "#Soldier", "#Demoman", "#Medic", "#HWGuy", "#Pyro", "#Spy", "#Engineer" };
const char *szClassNames[] = { "SCOUT", "SNIPER", "SOLDIER", "DEMOMAN", "MEDIC", "HWGUY", "PYRO", "SPY", "ENGINEER", "CIVILIAN" };

static class CClientClassMenu : public CClientWindow
{
public:
	typedef CClientWindow BaseClass;
	CClientClassMenu() : BaseClass( "CClientClassMenu" ) {}

	void _Init();
	void Draw();
} uiClassMenu;

void CClientClassMenu::_Init()
{
	int iYOffset = 80;

	for ( int i = PC_SCOUT; i <= PC_ENGINEER; i++ )
	{
		CClientMenuButton *pClassButton = new CClientMenuButton( '0' + i, L( szClassLabels[i - 1] ), Point( 0, iYOffset ), szClassCommands[i - 1] );
		AddItem( pClassButton );
		iYOffset += BTN_HEIGHT + BTN_GAP;
	}

	CClientMenuButton *pRandomButton = new CClientMenuButton( '0', L( "#Random" ), Point( 0, iYOffset ), szClassCommands[g_pClient->GetRandomClass()] );
	AddItem( pRandomButton );

	szTitle = L( "#Title_SelectYourClass" );
}

void CClientClassMenu::Draw()
{
	BaseClass::Draw();
}

void UI_ClassMenu_Show( void )
{
	EngFuncs::KEY_SetDest( KEY_MENU );
	uiClassMenu.Show();
}