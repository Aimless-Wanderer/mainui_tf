#include "BaseMenu.h"
#include "Action.h"
#include "ScrollView.h"
#include "ClientWindow.h"
#include "CommandButton.h"

static class CClientCommandMenu : public CClientWindow
{
public:
	typedef CClientWindow BaseClass;
	CClientCommandMenu() : BaseClass( "CClientCommandMenu" ) {}

	void Init( const char *szName )
	{
		m_bInit = true;
		m_bIsSubmenu = true;
		m_szSubmenuName = StringCopy( szName );
	}

	void _Init();
	void Reload();
	
	void VidInit()
	{
		Reload();
		CMenuBaseClientWindow::VidInit();
	}
	
	void Draw()
	{
		CMenuBaseClientWindow::Draw();
	}

	~CClientCommandMenu()
	{
		FOR_EACH_VEC( m_pSubMenus, i )
		{
			delete m_pSubMenus[i];
		}
	}

private:
	CCommandButton *CreateCustomButton( char *pName, char *pText, int iKeyBind );

	CEventCallback ShowSubmenu( CClientCommandMenu *cmdMenu );
	CUtlVector<CClientCommandMenu *> m_pSubMenus;
	CClientCommandMenu *CreateSubmenu( const char *szName );

	bool m_bIsSubmenu;
	char *m_szSubmenuName;
	CClientCommandMenu *m_pCurrentCommandMenu;
} uiCommandMenu;

CEventCallback CClientCommandMenu::ShowSubmenu( CClientCommandMenu *cmdMenu )
{
	return CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
	{
		CClientCommandMenu *cmdMenu = ( (CClientCommandMenu *)pExtra );
		cmdMenu->pos.x = pSelf->Parent()->size.w;
		cmdMenu->pos.y = pSelf->Parent()->pos.y + pSelf->pos.y;
		cmdMenu->Show();
	}, (void *)cmdMenu );
}

CClientCommandMenu *CClientCommandMenu::CreateSubmenu( const char *szName )
{
	FOR_EACH_VEC( m_pSubMenus, i )
	{
		if ( !strcmp( m_pSubMenus[i]->m_szSubmenuName, szName ) )
			return m_pSubMenus[i];
	}

	CClientCommandMenu * cmdMenu = new CClientCommandMenu();
	cmdMenu->Init( szName );
	m_pSubMenus.AddToTail( cmdMenu );

	return cmdMenu;
}

CCommandButton *CClientCommandMenu::CreateCustomButton( char *pName, char *pText, int iKeyBind )
{
	CCommandButton *pButton = NULL;

	if ( !strcmp( pName, "!CHANGETEAM" ) )
	{
		CClientCommandMenu *cmdMenu = CreateSubmenu( pName );
		pButton = new CCommandButton( pText, ShowSubmenu( cmdMenu ) );
		int iNumTeams = g_pClient->GetNumberOfTeams();
		char **szTeamNames = g_pClient->GetTeamNames();

		for ( int i = 0; i < iNumTeams; i++ )
		{
			char *cmd = new char[16];
			sprintf( cmd, "jointeam %i", i + 1 );
			CCommandButton *pTeamButton = new CCommandButton( L( szTeamNames[i] ), ExecAndHide( cmd ) );
			cmdMenu->AddItem( pTeamButton );
		}

		CCommandButton *pAutoAssignButton = new CCommandButton( L( "#Team_AutoAssign" ), ExecAndHide( "jointeam 5" ) );
		cmdMenu->AddItem( pAutoAssignButton );

		CCommandButton *pSpectateButton = new CSpectateButton( L( "#Menu_Spectate" ), ExecAndHide( "spectate" ) );
		cmdMenu->AddItem( pSpectateButton );
	}
	else if ( !strcmp( pName, "!CHANGECLASS" ) )
	{
		CClientCommandMenu *cmdMenu = CreateSubmenu( pName );
		pButton = new CCommandButton( pText, ShowSubmenu( cmdMenu ) );

		for ( int i = PC_SCOUT; i <= PC_ENGINEER; i++ )
		{
			CCommandButton *pClassButton = new CCommandButton( L( szClassLabels[i - 1] ), ExecAndHide( szClassCommands[i - 1] ) );
			cmdMenu->AddItem( pClassButton );
		}

		CCommandButton *pRandomButton = new CCommandButton( L( "#Random" ), ExecAndHide( szClassCommands[g_pClient->GetRandomClass() - 1] ) );
		cmdMenu->AddItem( pRandomButton );
	}
	else if ( !strcmp( pName, "!MAPBRIEFING" ) )
	{
	}
	else if ( !strcmp( pName, "!CLASSDESC" ) )
	{
	}
	else if ( !strcmp( pName, "!SERVERINFO" ) )
	{
	}
	else if ( !strcmp( pName, "!SPY" ) )
	{
		//CCommandButton *pButton = new CDisguiseButton( pText, NULL );
	}
	else if ( !strcmp( pName, "!FEIGN" ) )
	{
		pButton = new CFeignButton( false, pText, ExecAndHide( "feign" ) );
	}
	else if ( !strcmp( pName, "!FEIGNSILENT" ) )
	{
		pButton = new CFeignButton( false, pText, ExecAndHide( "sfeign" ) );
	}
	else if ( !strcmp( pName, "!FEIGNSTOP" ) )
	{
		pButton = new CFeignButton( true, pText, ExecAndHide( "feign" ) );
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

			pDisguiseMenu->CCommandButton *pButton = new CCommandButton( m_pDisguiseButtons[i] );
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
		pMenu->CCommandButton *pButton = new CCommandButton( pDetButton );
		pDetButton = new CommandButton(m_sDetpackStrings[1], 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		pDetButton->addActionSignal(new CMenuHandler_StringCommand("detstart 20"));
		pMenu->CCommandButton *pButton = new CCommandButton( pDetButton );
		pDetButton = new CommandButton(m_sDetpackStrings[2], 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		pDetButton->addActionSignal(new CMenuHandler_StringCommand("detstart 50"));
		pMenu->CCommandButton *pButton = new CCommandButton( pDetButton );
		*/
	}
	else if ( !strcmp( pName, "!DETPACKSTOP" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "detstop" ) );
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
		pButton = new CCommandButton( pText, ExecAndHide( "build 2" ) );
	}
	else if ( !strcmp( pName, "!BUILDDISPENSER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "build 1" ) );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY180" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "rotatesentry180" ) );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "rotatesentry" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEDISPENSER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "dismantle 1" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLESENTRY" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "dismantle 2" ) );
	}
	else if ( !strcmp( pName, "!DETONATEDISPENSER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "detdispenser" ) );
	}
	else if ( !strcmp( pName, "!DETONATESENTRY" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "detsentry" ) );
	}
	else if ( !strcmp( pName, "!BUILDENTRYTELEPORTER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "build 4" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEENTRYTELEPORTER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "dismantle 4" ) );
	}
	else if ( !strcmp( pName, "!DETONATEENTRYTELEPORTER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "detentryteleporter" ) );
	}
	else if ( !strcmp( pName, "!BUILDEXITTELEPORTER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "build 5" ) );
	}
	else if ( !strcmp( pName, "!DISMANTLEEXITTELEPORTER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "dismantle 5" ) );
	}	
	else if ( !strcmp( pName, "!DETONATEEXITTELEPORTER" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "detexitteleporter" ) );
	}
	else if ( !strcmp( pName, "!BUILDSTOP" ) )
	{
		pButton = new CCommandButton( pText, ExecAndHide( "build" ) );
	}

	return pButton;
}

void CClientCommandMenu::_Init()
{
	m_pCurrentCommandMenu = this;
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
			CCommandButton *pButton = NULL;

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
				for ( int i = PC_SCOUT; i <= PC_ENGINEER; i++ )
				{
					if ( !strcmp( sToken, szClassNames[i - 1] ) )
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

			if ( iButtonType == BTN_CUSTOM )
			{
				pButton = CreateCustomButton( sCommand, sText, iKeyBind );
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );

				if ( sToken[0] == '{' )
					strcpy( sCommand, sToken );
				else
					bIsSubmenu = false;
			}
			else if ( iButtonType == BTN_MAP )
			{
				pButton = new CMapButton( sMap, sText, ExecAndHide( StringCopy( sCommand ) ) );
			}
			else if ( iButtonType == BTN_TEAM )
			{
				pButton = new CTeamButton( iTeam, sText, ExecAndHide( StringCopy( sCommand ) ) );
			}
			else if ( iButtonType == BTN_TOGGLE )
			{
				//Velaron: TODO
				pButton = new CCommandButton( sText, ExecAndHide( StringCopy( sCommand ) ) );
			}
			else
			{
				pButton = new CCommandButton( sText, ExecAndHide( StringCopy( sCommand ) ) );
			}

			if ( pButton )
			{
				m_pCurrentCommandMenu->AddItem( pButton );
			}

			//Velaron: TODO
			if ( sCommand[0] == '{' && pButton )
			{
				CClientCommandMenu *cmdMenu = CreateSubmenu( pButton->szName );
				pButton->onPressed = ShowSubmenu( cmdMenu );
				m_pCurrentCommandMenu = cmdMenu;
			}

			if ( bIsSubmenu )
			{
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
		}

		m_pCurrentCommandMenu = this;
		pFile = EngFuncs::COM_ParseFile( pFile, sToken );
	}
}

void CClientCommandMenu::Reload()
{
	int iCount = 0;

	FOR_EACH_VEC ( m_pItems, i )
	{
		if( m_pItems[i]->IsVisible() )
		{
			m_pItems[i]->pos.y = iCount * BTN_HEIGHT;
			iCount++;
		}
	}

	size.w = BTN_WIDTH;
	size.h = iCount * BTN_HEIGHT;

	if( !m_bIsSubmenu )
	{
		pos.x = 0;
		pos.y = Max( ( 768 - size.h ) / 2, 0 );
	}

	BaseClass::Reload();
}

void UI_CommandMenu_Show( void )
{
	uiCommandMenu.Show();
}

void UI_CommandMenu_Hide( void )
{
	uiCommandMenu.Hide();
}