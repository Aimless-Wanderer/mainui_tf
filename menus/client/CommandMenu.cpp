#include "BaseMenu.h"
#include "Action.h"
#include "ScrollView.h"
#include "ClientWindow.h"

const char *szClasses[] = { "SCOUT", "SNIPER", "SOLDIER", "DEMOMAN", "MEDIC", "HWGUY", "PYRO", "SPY", "ENGINEER", "CIVILIAN" };

enum
{
	BTN_CUSTOM,
	BTN_MAP,
	BTN_TEAM,
	BTN_TOGGLE,
	BTN_CLASS
};

static class CClientCommandMenu : public CClientWindow
{
public:
	typedef CClientWindow BaseClass;
	CClientCommandMenu() : BaseClass( "CClientCommandMenu" ) {}

	void _Init();
	void Reload();
	void Draw();

	void VidInit()
	{
		size.w = 1024;
		size.h = 768;
		pos.x = 0;
		pos.y = 0;

		CMenuBaseClientWindow::VidInit();
	}

private:
	void AddCustomButton( char *pName, char *pText );
} uiCommandMenu;

void CClientCommandMenu::AddCustomButton( char *pName, char *pText )
{
	if ( !strcmp( pName, "!CHANGETEAM" ) )
	{
		/*
		// ChangeTeam Submenu
		pButton = new CommandButton(pButtonText, 0, BUTTON_SIZE_Y * 2, CMENU_SIZE_X, BUTTON_SIZE_Y);

		// Create the submenu
		pMenu = CreateSubMenu(pButton, m_pCurrentCommandMenu, iYOffset );
		m_pCommandMenus[m_iNumMenus] = pMenu;
		m_iNumMenus++;

		// ChangeTeam buttons
		for (int i = 0; i < 4; i++)
		{
			char sz[256]; 
			sprintf(sz, "jointeam %d", i+1);
			m_pTeamButtons[i] = new TeamButton(i+1, "teamname", 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
			m_pTeamButtons[i]->addActionSignal(new CMenuHandler_StringCommandWatch( sz ));
			pMenu->AddButton( m_pTeamButtons[i] ); 
		}

		// Auto Assign button
		m_pTeamButtons[4] = new TeamButton(5, gHUD.m_TextMessage.BufferedLocaliseTextString( "#Team_AutoAssign" ), 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		m_pTeamButtons[4]->addActionSignal(new CMenuHandler_StringCommand( "jointeam 5" ));
		pMenu->AddButton( m_pTeamButtons[4] ); 

		// Spectate button
		m_pTeamButtons[5] = new SpectateButton( CHudTextMessage::BufferedLocaliseTextString( "#Menu_Spectate" ), 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y, false);
		m_pTeamButtons[5]->addActionSignal(new CMenuHandler_StringCommand( "spectate" ));
		pMenu->AddButton( m_pTeamButtons[5] ); 
		*/
	}
	else if ( !strcmp( pName, "!CHANGECLASS" ) )
	{
		/*
		// Create the Change class menu
		pButton = new ClassButton(-1, pButtonText, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y, false);

		// ChangeClass Submenu
		pMenu = CreateSubMenu(pButton, m_pCurrentCommandMenu, iYOffset );
		m_pCommandMenus[m_iNumMenus] = pMenu;
		m_iNumMenus++;

		for (int i = PC_SCOUT; i <= PC_RANDOM; i++ )
		{
			char sz[256]; 

			// ChangeClass buttons
			CHudTextMessage::LocaliseTextString( sLocalisedClasses[i], sz, 256 );
			ClassButton *pClassButton = new ClassButton( i, sz, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y, false);

			sprintf(sz, "%s", sTFClassSelection[i]);
			pClassButton->addActionSignal(new CMenuHandler_StringCommandClassSelect(sz));
			pMenu->AddButton( pClassButton );
		}
		*/
	}
	else if ( !strcmp( pName, "!MAPBRIEFING" ) )
	{
		/*
		pButton = new CommandButton(pButtonText, 0, BUTTON_SIZE_Y * m_pCurrentCommandMenu->GetNumButtons(), CMENU_SIZE_X, BUTTON_SIZE_Y);
		pButton->addActionSignal(new CMenuHandler_TextWindow(MENU_MAPBRIEFING));
		// Create an input signal that'll popup the current menu
		pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
		*/
	}
	else if ( !strcmp( pName, "!CLASSDESC" ) )
	{
		/*
		pButton = new ClassButton(0, pButtonText, 0, BUTTON_SIZE_Y * m_pCurrentCommandMenu->GetNumButtons(), CMENU_SIZE_X, BUTTON_SIZE_Y, false);
		pButton->addActionSignal(new CMenuHandler_TextWindow(MENU_CLASSHELP));
		// Create an input signal that'll popup the current menu
		pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
		*/
	}
	else if ( !strcmp( pName, "!SERVERINFO" ) )
	{
		/*
		pButton = new ClassButton(0, pButtonText, 0, BUTTON_SIZE_Y * m_pCurrentCommandMenu->GetNumButtons(), CMENU_SIZE_X, BUTTON_SIZE_Y, false);
		pButton->addActionSignal(new CMenuHandler_TextWindow(MENU_INTRO));
		// Create an input signal that'll popup the current menu
		pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
		*/
	}
	else if ( !strcmp( pName, "!SPY" ) )
	{
		/*
		pButton = new DisguiseButton( 0, pButtonText, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y );
		*/
	}
	else if ( !strcmp( pName, "!FEIGN" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "feign" ) );
	}
	else if ( !strcmp( pName, "!FEIGNSILENT" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "sfeign" ) );
	}
	else if ( !strcmp( pName, "!FEIGNSTOP" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "feign" ) );
	}
	else if ( !strcmp( pName, "!DISGUISEENEMY" ) )
	{
		/*
		// Create the disguise enemy button, which active only if there are 2 teams
		pButton = new DisguiseButton(DISGUISE_TEAM2, pButtonText, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		CreateDisguiseSubmenu( pButton, m_pCurrentCommandMenu, "disguise_enemy", iYOffset);
		*/
	}
	else if ( !strcmp( pName, "!DISGUISEFRIENDLY" ) )
	{
		/*
		// Create the disguise friendly button, which active only if there are 1 or 2 teams
		pButton = new DisguiseButton(DISGUISE_TEAM1 | DISGUISE_TEAM2, pButtonText, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		CreateDisguiseSubmenu( pButton, m_pCurrentCommandMenu, "disguise_friendly", iYOffset );
		*/
	}
	else if ( !strcmp( pName, "!DISGUISE" ) )
	{
		/*
		// Create the Disguise button
		pButton = new DisguiseButton( DISGUISE_TEAM3 | DISGUISE_TEAM4, pButtonText, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		CCommandMenu *pDisguiseMenu = CreateSubMenu( pButton, m_pCurrentCommandMenu, iYOffset );
		m_pCommandMenus[m_iNumMenus] = pDisguiseMenu;
		m_iNumMenus++;

		// Disguise Enemy submenu buttons
		for ( int i = 1; i <= 4; i++ )
		{
			// only show the 4th disguise button if we have 4 teams
			m_pDisguiseButtons[i] = new DisguiseButton( ((i < 4) ? DISGUISE_TEAM3 : 0) | DISGUISE_TEAM4, "Disguise", 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);

			pDisguiseMenu->AddButton( m_pDisguiseButtons[i] );
			m_pDisguiseButtons[i]->setParentMenu( pDisguiseMenu );

			char sz[256]; 
			sprintf( sz, "disguise %d", i );
			CreateDisguiseSubmenu( m_pDisguiseButtons[i], pDisguiseMenu, sz, iYOffset, CMENU_SIZE_X - 1 );
		}
		*/
	}
	// Start setting a Detpack
	else if ( !strcmp( pName, "!DETPACKSTART" ) )
	{
		/*
		// Detpack Submenu
		pButton = new DetpackButton(2, pButtonText, 0, BUTTON_SIZE_Y * 2, CMENU_SIZE_X, BUTTON_SIZE_Y);

		// Create the submenu
		pMenu = CreateSubMenu(pButton, m_pCurrentCommandMenu, iYOffset );
		m_pCommandMenus[m_iNumMenus] = pMenu;
		m_iNumMenus++;

		// Set detpack buttons
		CommandButton *pDetButton;
		pDetButton = new CommandButton(m_sDetpackStrings[0], 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		pDetButton->addActionSignal(new CMenuHandler_StringCommand("detstart 5"));
		pMenu->AddButton( pDetButton );
		pDetButton = new CommandButton(m_sDetpackStrings[1], 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		pDetButton->addActionSignal(new CMenuHandler_StringCommand("detstart 20"));
		pMenu->AddButton( pDetButton );
		pDetButton = new CommandButton(m_sDetpackStrings[2], 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		pDetButton->addActionSignal(new CMenuHandler_StringCommand("detstart 50"));
		pMenu->AddButton( pDetButton );
		*/
	}
	else if ( !strcmp( pName, "!DETPACKSTOP" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "detstop" ) );
	}
	else if ( !strcmp( pName, "!BUILD" ) )
	{
		/*
		// only appears if the player is an engineer, and either they have built something or have enough metal to build
		pButton = new BuildButton( BUILDSTATE_BASE, 0, pButtonText, 0, BUTTON_SIZE_Y * 2, CMENU_SIZE_X, BUTTON_SIZE_Y);
		*/
	}
	else if ( !strcmp( pName, "!BUILDSENTRY" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "build 2" ) );
	}
	else if ( !strcmp( pName, "!BUILDDISPENSER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "build 1" ) );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY180" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "rotatesentry180" ) );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "rotatesentry" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEDISPENSER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "dismantle 1" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLESENTRY" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "dismantle 2" ) );
	}
	else if ( !strcmp( pName, "!DETONATEDISPENSER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "detdispenser" ) );
	}
	else if ( !strcmp( pName, "!DETONATESENTRY" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "detsentry" ) );
	}
	else if ( !strcmp( pName, "!BUILDENTRYTELEPORTER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "build 4" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEENTRYTELEPORTER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "dismantle 4" ) );
	}
	else if ( !strcmp( pName, "!DETONATEENTRYTELEPORTER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "detentryteleporter" ) );
	}
	else if ( !strcmp( pName, "!BUILDEXITTELEPORTER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "build 5" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEEXITTELEPORTER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "dismantle 5" ) );
	}	
	else if ( !strcmp( pName, "!DETONATEEXITTELEPORTER" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "detexitteleporter" ) );
	}
	else if ( !strcmp( pName, "!BUILDSTOP" ) )
	{
		AddButton( 666, StringCopy( pText ),
		Point( 0, 0 ), ExecAndHide( "build" ) );
	}
}

void CClientCommandMenu::_Init()
{
	char sToken[1024];
	char *pFile = (char*)EngFuncs::COM_LoadFile( "commandmenu.txt", NULL );
	pFile = EngFuncs::COM_ParseFile( pFile, sToken );

	while ( strlen( sToken ) )
	{
		while ( sToken[0] != '}' && strlen( sToken ) )
		{
			char sKeyBind[32];
			char sText[32];
			char sCommand[32];
			char sMap[32];
			int iTeam;
			int iClass;
			int iButtonType;
			bool bIsSubMenu = true;

			if ( !strcmp( sToken, "CUSTOM" ) )
			{
				iButtonType = BTN_CUSTOM;
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
			else if ( !strcmp( sToken, "MAP" ) )
			{
				iButtonType = BTN_MAP;
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
				strcpy( sMap, sToken );
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
			else if ( !strncmp( sToken, "TEAM", 4 ) )
			{
				iButtonType = BTN_TEAM;
				iTeam = atoi( sToken + 4 );
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
			else if ( !strcmp( sToken, "TOGGLE" ) )
			{
				iButtonType = BTN_TOGGLE;
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
			else
			{
				for ( int i = 0; i <= 9; i++ )
				{
					if ( !strcmp( sToken, szClasses[i] ) )
					{
						iButtonType = BTN_CLASS;
						iClass = i;
						pFile = EngFuncs::COM_ParseFile( pFile, sToken );
						break;
					}
				}
			}

			strcpy( sKeyBind, sToken );

			pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			strcpy( sText, sToken );

			pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			strcpy( sCommand, sToken );

			if ( iButtonType == BTN_CUSTOM )
			{
				AddCustomButton( sCommand, sText );
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );

				if ( sToken[0] == '{' )
					strcpy( sCommand, sToken );
				else
					bIsSubMenu = false;
			}
			else if ( iButtonType == BTN_MAP )
			{
			}
			else if ( iButtonType == BTN_TEAM )
			{
			}
			else if ( iButtonType == BTN_TOGGLE )
			{
			}
			else
			{
			}

			if ( sCommand[0] == '{' )
			{
			}
			else if ( iButtonType != BTN_CUSTOM )
			{
			}

			Con_Printf( "CmdMenuBtn: %s %s %s\n", sKeyBind, sCommand, sText );

			if ( bIsSubMenu )
			{
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
		}

		pFile = EngFuncs::COM_ParseFile( pFile, sToken );
	}

	Reload();
}

void CClientCommandMenu::Draw()
{
	BaseClass::Draw();
}

void CClientCommandMenu::Reload()
{
	int iCount = m_pButtons.Count();
	int iYOffset = ( 768 - ( iCount * BTN_HEIGHT ) ) / 2;

	FOR_EACH_VEC(m_pButtons, i)
	{
		int x = m_pButtons[i]->pos.x;
		m_pButtons[i]->pos = Point( x, iYOffset );
		iYOffset += BTN_HEIGHT;
	}

	Con_Printf( "BtnCount: %i\n", iCount );

	BaseClass::Reload();
}

void UI_CommandMenu_Show( void )
{
	EngFuncs::KEY_SetDest( KEY_MENU );
	uiCommandMenu.Show();
}

void UI_CommandMenu_Hide( void )
{
	uiCommandMenu.Hide();
}