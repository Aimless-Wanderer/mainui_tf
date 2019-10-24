#include "BaseMenu.h"
#include "Action.h"
#include "ScrollView.h"
#include "ClientWindow.h"

static class CClientCommandMenu : public CClientWindow
{
public:
	typedef CClientWindow BaseClass;
	CClientCommandMenu() : BaseClass( "CClientCommandMenu" ) {}
	void Init( const char *szName );

	void _Init();
	void VidInit();
	void Reload();
	void Draw();

	~CClientCommandMenu()
	{
		FOR_EACH_VEC( m_pSubMenus, i )
		{
			delete m_pSubMenus[i];
		}
	}

private:
	void AddCustomButton( char *pName, char *pText, int iKeyBind );

	CEventCallback ShowSubmenu( const char *szName );
	void SetSubmenuPosition( CMenuBaseItem *pParent );
	CUtlVector<CClientCommandMenu *> m_pSubMenus;
	bool m_bIsSubmenu;
	char *m_szSubmenuName;
} uiCommandMenu;

void CClientCommandMenu::VidInit()
{
	CMenuBaseClientWindow::VidInit();
}

CEventCallback CClientCommandMenu::ShowSubmenu( const char *szName )
{
	CClientCommandMenu *cmdMenu = NULL;

	FOR_EACH_VEC( m_pSubMenus, i )
	{
		if ( !strcmp( m_pSubMenus[i]->m_szSubmenuName, szName ) )
			cmdMenu = m_pSubMenus[i];
	}

	if ( !cmdMenu )
	{
		cmdMenu = new CClientCommandMenu();
		cmdMenu->Init( szName );
		m_pSubMenus.AddToTail( cmdMenu );
	}

	return CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
	{
		CClientCommandMenu *cmdMenu = ( (CClientCommandMenu *)pExtra );
		cmdMenu->SetSubmenuPosition( pSelf );
		cmdMenu->Show();
	}, (void *)cmdMenu );
}

void CClientCommandMenu::SetSubmenuPosition( CMenuBaseItem *pParent )
{
	pos.x = pParent->Parent()->size.w;
	pos.y = pParent->pos.y;
	while ( pos.y + BTN_HEIGHT > ScreenHeight )
		pos.y -= BTN_HEIGHT;
}

void CClientCommandMenu::AddCustomButton( char *pName, char *pText, int iKeyBind )
{
	if ( !strcmp( pName, "!CHANGETEAM" ) )
	{
		AddButton( iKeyBind, StringCopy( pText ),
			Point( 0, 0 ), ShowSubmenu( pName ) );

		CClientCommandMenu *cmdMenu;
		FOR_EACH_VEC( m_pSubMenus, i )
		{
			if ( !strcmp( m_pSubMenus[i]->m_szSubmenuName, pName ) )
				cmdMenu = m_pSubMenus[i];
		}

		int iNumTeams = g_pClient->GetNumberOfTeams();
		char **szTeamNames = g_pClient->GetTeamNames();

		for (int i = 0; i < iNumTeams; i++)
		{
			char cmd[16];
			sprintf( cmd, "jointeam %i", i + 1 );
			cmdMenu->AddButton( ( i + 1 ) + '0', L( szTeamNames[i] ),
				Point( 0, 0 ), ExecAndHide( cmd ) );
		}

		cmdMenu->AddButton( '5', L( "#Team_AutoAssign" ),
			Point( 0, 0 ), ExecAndHide( "jointeam 5" ));
	}
	else if ( !strcmp( pName, "!CHANGECLASS" ) )
	{
		AddButton( iKeyBind, StringCopy( pText ),
			Point( 0, 0 ), ShowSubmenu( pName ) );

		CClientCommandMenu *cmdMenu;
		FOR_EACH_VEC( m_pSubMenus, i )
		{
			if ( !strcmp( m_pSubMenus[i]->m_szSubmenuName, pName ) )
				cmdMenu = m_pSubMenus[i];
		}

		const char *szCommands[] = { "scout", "sniper", "soldier", "demoman", "medic", "hwguy", "pyro", "spy", "engineer" };
		const char *szLabels[] = { "#Scout", "#Sniper", "#Soldier", "#Demoman", "#Medic", "#HWGuy", "#Pyro", "#Spy", "#Engineer" };

		for ( int i = 0; i < 9; i++ )
		{
			cmdMenu->AddButton( ( i + 1 ) + '0', L( szLabels[i] ),
				Point( 0, 0 ), ExecAndHide( szCommands[i] ) );
		}

		cmdMenu->AddButton( '0', L( "#Random" ),
			Point( 0, 0 ), ExecAndHide( szCommands[g_pClient->GetRandomClass() - 1] ) );
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
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "feign" ) );
	}
	else if ( !strcmp( pName, "!FEIGNSILENT" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "sfeign" ) );
	}
	else if ( !strcmp( pName, "!FEIGNSTOP" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "feign" ) );
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
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "detstop" ) );
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
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "build 2" ) );
	}
	else if ( !strcmp( pName, "!BUILDDISPENSER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "build 1" ) );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY180" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "rotatesentry180" ) );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "rotatesentry" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEDISPENSER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "dismantle 1" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLESENTRY" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "dismantle 2" ) );
	}
	else if ( !strcmp( pName, "!DETONATEDISPENSER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "detdispenser" ) );
	}
	else if ( !strcmp( pName, "!DETONATESENTRY" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "detsentry" ) );
	}
	else if ( !strcmp( pName, "!BUILDENTRYTELEPORTER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "build 4" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEENTRYTELEPORTER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "dismantle 4" ) );
	}
	else if ( !strcmp( pName, "!DETONATEENTRYTELEPORTER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "detentryteleporter" ) );
	}
	else if ( !strcmp( pName, "!BUILDEXITTELEPORTER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "build 5" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEEXITTELEPORTER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "dismantle 5" ) );
	}	
	else if ( !strcmp( pName, "!DETONATEEXITTELEPORTER" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "detexitteleporter" ) );
	}
	else if ( !strcmp( pName, "!BUILDSTOP" ) )
	{
		//AddButton( iKeyBind, StringCopy( pText ),
		//Point( 0, 0 ), ExecAndHide( "build" ) );
	}
}

void CClientCommandMenu::Init( const char *szName )
{
	m_bInit = true;
	m_bIsSubmenu = true;
	m_szSubmenuName = StringCopy( szName );
}

void CClientCommandMenu::_Init()
{
	m_bIsSubmenu = false;
	char sToken[1024];
	char *pFile = (char*)EngFuncs::COM_LoadFile( "commandmenu.txt", NULL );
	pFile = EngFuncs::COM_ParseFile( pFile, sToken );

	while ( strlen( sToken ) )
	{
		while ( sToken[0] != '}' && strlen( sToken ) )
		{
			int iKeyBind;
			char sText[32];
			char sCommand[32];
			char sMap[32];
			int iTeam;
			int iClass;
			int iButtonType;
			bool bIsSubmenu = true;

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
					if ( !strcmp( sToken, szClassNames[i] ) )
					{
						iButtonType = BTN_CLASS;
						iClass = i;
						pFile = EngFuncs::COM_ParseFile( pFile, sToken );
						break;
					}
				}
			}

			iKeyBind = atoi( sToken ) + '0';

			pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			strcpy( sText, sToken );

			pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			strcpy( sCommand, sToken );

			Con_Printf( "CmdMenuBtn: %i %s %s\n", iKeyBind, sCommand, sText );

			if ( iButtonType == BTN_CUSTOM )
			{
				AddCustomButton( sCommand, sText , iKeyBind);
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );

				if ( sToken[0] == '{' )
					strcpy( sCommand, sToken );
				else
					bIsSubmenu = false;
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

			if ( bIsSubmenu )
			{
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
		}

		pFile = EngFuncs::COM_ParseFile( pFile, sToken );
	}
}

void CClientCommandMenu::Draw()
{
	CMenuBaseClientWindow::Draw();
}

void CClientCommandMenu::Reload()
{
	int iCount = m_pButtons.Count();

	size.w = BTN_WIDTH;
	size.h = iCount * BTN_HEIGHT;

	if( !m_bIsSubmenu )
	{
		pos.x = 0;
		pos.y = Max( ( 768 - size.h ) / 2, 0 );
	}

	FOR_EACH_VEC ( m_pButtons, i )
	{
		m_pButtons[i]->pos.y = i * BTN_HEIGHT;
	}
	
	BaseClass::Recalculate();
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