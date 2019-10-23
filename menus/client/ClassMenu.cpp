#include "BaseMenu.h"
#include "Action.h"
#include "ScrollView.h"
#include "ClientWindow.h"

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

	const char *szCommands[] = { "scout", "sniper", "soldier", "demoman", "medic", "hwguy", "pyro", "spy", "engineer" };
	const char *szLabels[] = { "#Scout", "#Sniper", "#Soldier", "#Demoman", "#Medic", "#HWGuy", "#Pyro", "#Spy", "#Engineer" };

	for ( int i = 0; i < 9; i++ )
	{
		AddButton( ( i + 1 ) + '0', L( szLabels[i] ),
			Point( 0, iYOffset ), ExecAndHide( szCommands[i] ) );
		iYOffset += BTN_HEIGHT + BTN_GAP;
	}

	AddButton( '5', L( "#Random" ),
		Point( 0, iYOffset ), ExecAndHide( szCommands[g_pClient->GetRandomClass() - 1] ));

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