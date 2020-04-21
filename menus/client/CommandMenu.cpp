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
	CClientCommandMenu( bool subMenu ) { m_bInit = true; }

	void _Init() override;
	void Reload() override;
	
	void VidInit() override
	{
		Reload();
		CMenuBaseClientWindow::VidInit();
	}
	
	void Draw() override
	{
		CMenuBaseClientWindow::Draw();
	}

	void SetParent( CClientCommandMenu* cmdMenu )
	{
		m_pParentCommandMenu = cmdMenu;
	}

	CClientCommandMenu *Parent()
	{
		return m_pParentCommandMenu;
	}

private:
	CCommandButton *CreateCustomButton( char *pName, char *pText, int iKeyBind );
	CClientCommandMenu *m_pCurrentCommandMenu;
	CClientCommandMenu *m_pParentCommandMenu;
} uiCommandMenu;

CCommandButton *CClientCommandMenu::CreateCustomButton( char *pName, char *pText, int iKeyBind )
{
	CCommandButton *pButton = NULL;

	if ( !strcmp( pName, "!CHANGETEAM" ) )
	{
		CClientCommandMenu *cmdMenu = new CClientCommandMenu( true );
		cmdMenu->SetParent( this );
		pButton = new CCommandButton( iKeyBind, pText, cmdMenu );
		int iNumTeams = g_pClient->GetNumberOfTeams();
		char **szTeamNames = g_pClient->GetTeamNames();

		for ( int i = 1; i <= iNumTeams; i++ )
		{
			char *cmd = new char[16];
			sprintf( cmd, "jointeam %i", i );
			CCommandButton *pTeamButton = new CCommandButton( '0' + i, L( szTeamNames[i - 1] ), cmd );
			cmdMenu->AddItem( pTeamButton );
		}

		CCommandButton *pAutoAssignButton = new CCommandButton( '5', L( "#Team_AutoAssign" ), "jointeam 5" );
		cmdMenu->AddItem( pAutoAssignButton );

		CCommandButton *pSpectateButton = new CSpectateButton( '6', L( "#Menu_Spectate" ), "spectate" );
		cmdMenu->AddItem( pSpectateButton );
	}
	else if ( !strcmp( pName, "!CHANGECLASS" ) )
	{
		CClientCommandMenu *cmdMenu = new CClientCommandMenu( true );
		cmdMenu->SetParent( this );
		pButton = new CCommandButton( iKeyBind, pText, cmdMenu );

		for ( int i = PC_SCOUT; i <= PC_ENGINEER; i++ )
		{
			CCommandButton *pClassButton = new CCommandButton( '0' + i, L( szClassLabels[i - 1] ), szClassCommands[i - 1] );
			cmdMenu->AddItem( pClassButton );
		}

		CCommandButton *pRandomButton = new CCommandButton( '0', L( "#Random" ), szClassCommands[g_pClient->GetRandomClass() - 1] );
		cmdMenu->AddItem( pRandomButton );
	}
	else if ( !strcmp( pName, "!MAPBRIEFING" ) )
	{
		//Velaron: TODO
		pButton = new CCommandButton( iKeyBind, pText );
	}
	else if ( !strcmp( pName, "!CLASSDESC" ) )
	{
		//Velaron: TODO
		pButton = new CCommandButton( iKeyBind, pText );
	}
	else if ( !strcmp( pName, "!SERVERINFO" ) )
	{
		//Velaron: TODO
		pButton = new CCommandButton( iKeyBind, pText );
	}
	else if ( !strcmp( pName, "!SPY" ) )
	{
		pButton = new CDisguiseButton( 0, iKeyBind, pText );
	}
	else if ( !strcmp( pName, "!FEIGN" ) )
	{
		pButton = new CFeignButton( false, iKeyBind, pText, "feign" );
	}
	else if ( !strcmp( pName, "!FEIGNSILENT" ) )
	{
		pButton = new CFeignButton( false, iKeyBind, pText, "sfeign" );
	}
	else if ( !strcmp( pName, "!FEIGNSTOP" ) )
	{
		pButton = new CFeignButton( true, iKeyBind, pText, "feign" );
	}
	else if ( !strcmp( pName, "!DISGUISEENEMY" ) )
	{
		CClientCommandMenu *cmdMenu = new CClientCommandMenu( true );
		cmdMenu->SetParent( this );
		pButton = new CDisguiseButton( DISGUISE_TEAM2, iKeyBind, pText, cmdMenu );

		for ( int i = PC_SCOUT; i <= PC_ENGINEER; i++ )
		{
			char *cmd = new char[32];
			sprintf( cmd, "disguise_enemy %i", i );
			CCommandButton *pDisguiseButton = new CCommandButton( i, L( szClassLabels[i - 1] ), cmd );
			cmdMenu->AddItem( pDisguiseButton );
		}
	}
	else if ( !strcmp( pName, "!DISGUISEFRIENDLY" ) )
	{
		CClientCommandMenu *cmdMenu = new CClientCommandMenu( true );
		cmdMenu->SetParent( this );
		pButton = new CDisguiseButton( DISGUISE_TEAM1 | DISGUISE_TEAM2, iKeyBind, pText, cmdMenu );

		for ( int i = PC_SCOUT; i <= PC_ENGINEER; i++ )
		{
			char *cmd = new char[32];
			sprintf( cmd, "disguise_friendly %i", i );
			CCommandButton *pDisguiseButton = new CCommandButton( i, L( szClassLabels[i - 1] ), cmd );
			cmdMenu->AddItem( pDisguiseButton );
		}
	}
	else if ( !strcmp( pName, "!DISGUISE" ) )
	{
		CClientCommandMenu *cmdMenu = new CClientCommandMenu( true );
		cmdMenu->SetParent( this );
		pButton = new CDisguiseButton( DISGUISE_TEAM3 | DISGUISE_TEAM4, iKeyBind, pText, cmdMenu );

		for ( int i = 1; i <= 4; i++ )
		{
			CClientCommandMenu *cmdMenu_ = new CClientCommandMenu( true );
			cmdMenu_->SetParent( cmdMenu );
			CDisguiseButton *pDisguiseButton = new CDisguiseButton( ( ( i < 4 ) ? DISGUISE_TEAM3 : 0) | DISGUISE_TEAM4, iKeyBind, "Disguise", cmdMenu_ );

			for ( int j = PC_SCOUT; j <= PC_ENGINEER; j++ )
			{
				char *cmd = new char[32];
				sprintf( cmd, "disguise %i %i", i, j );
				CCommandButton *pDisguiseButton = new CCommandButton( i, L( szClassLabels[j - 1] ), cmd );
				cmdMenu_->AddItem( pDisguiseButton );
			}

			cmdMenu->AddItem( pDisguiseButton );
		}
	}
	else if ( !strcmp( pName, "!DETPACKSTART" ) )
	{
		CClientCommandMenu *cmdMenu  = new CClientCommandMenu( true );
		cmdMenu->SetParent( this );
		pButton = new CDetpackButton( 2, iKeyBind, pText, cmdMenu );

		CCommandButton *pDetButton;
		pDetButton = new CCommandButton( '1', L( "#DetpackSet_For5Seconds" ), "detstart 5" );
		cmdMenu->AddItem(pDetButton);

		pDetButton = new CCommandButton( '2', L( "#DetpackSet_For20Seconds" ), "detstart 20" );
		cmdMenu->AddItem(pDetButton);

		pDetButton = new CCommandButton( '3', L( "#DetpackSet_For50Seconds" ), "detstart 50" );
		cmdMenu->AddItem(pDetButton);
	}
	else if ( !strcmp( pName, "!DETPACKSTOP" ) )
	{
		pButton = new CDetpackButton( 1, iKeyBind, pText, "detstop" );
	}
	else if ( !strcmp( pName, "!BUILD" ) )
	{
		pButton = new CBuildButton( BS_BASE, 0, iKeyBind, pText );
	}
	else if ( !strcmp( pName, "!BUILDSENTRY" ) )
	{
		pButton = new CBuildButton( BS_CAN_BUILD, BLD_SENTRY, iKeyBind, pText, "build 2" );
	}
	else if ( !strcmp( pName, "!BUILDDISPENSER" ) )
	{
		pButton = new CBuildButton( BS_CAN_BUILD, BLD_DISPENSER, iKeyBind, pText, "build 1" );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY180" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_SENTRY, iKeyBind, pText, "rotatesentry180" );
	}
	else if ( !strcmp( pName, "!ROTATESENTRY" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_SENTRY, iKeyBind, pText, "rotatesentry" );
	}
	else if ( !strcmp( pName, "!DISMANTLEDISPENSER" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_DISPENSER, iKeyBind, pText, "dismantle 1" );
	}
	else if ( !strcmp( pName, "!DISMANTLESENTRY" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_SENTRY, iKeyBind, pText, "dismantle 1" );
	}
	else if ( !strcmp( pName, "!DETONATEDISPENSER" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_DISPENSER, iKeyBind, pText, "detdispenser" );
	}
	else if ( !strcmp( pName, "!DETONATESENTRY" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_SENTRY, iKeyBind, pText, "detsentry" );
	}
	else if ( !strcmp( pName, "!BUILDENTRYTELEPORTER" ) )
	{
		pButton = new CBuildButton( BS_CAN_BUILD, BLD_TELE_ENTRANCE, iKeyBind, pText, "build 4" );
	}
	else if ( !strcmp( pName, "!DISMANTLEENTRYTELEPORTER" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_TELE_ENTRANCE, iKeyBind, pText, "dismantle 4" );
	}
	else if ( !strcmp( pName, "!DETONATEENTRYTELEPORTER" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_TELE_ENTRANCE, iKeyBind, pText, "detentryteleporter" );
	}
	else if ( !strcmp( pName, "!BUILDEXITTELEPORTER" ) )
	{
		pButton = new CBuildButton( BS_CAN_BUILD, BLD_TELE_EXIT, iKeyBind, pText, "build 5" );
	}
	else if ( !strcmp( pName, "!DISMANTLEEXITTELEPORTER" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_TELE_EXIT, iKeyBind, pText, "dismantle 5" );
	}	
	else if ( !strcmp( pName, "!DETONATEEXITTELEPORTER" ) )
	{
		pButton = new CBuildButton( BS_HAS_BUILDING, BLD_TELE_EXIT, iKeyBind, pText, "detexitteleporter" );
	}
	else if ( !strcmp( pName, "!BUILDSTOP" ) )
	{
		pButton = new CBuildButton( BS_IS_BUILDING, 0, iKeyBind, pText, "build" );
	}

	return pButton;
}

void CClientCommandMenu::_Init()
{
	m_pParentCommandMenu = NULL;
	m_pCurrentCommandMenu = this;
	char sToken[1024];
	char *pFile = (char*)EngFuncs::COM_LoadFile( "commandmenu.txt", NULL );
	pFile = EngFuncs::COM_ParseFile( pFile, sToken );

	while ( strlen( sToken ) )
	{
		while ( sToken[0] != '}' && strlen( sToken ) )
		{
			int iKeyBind;
			char sText[32] = "";
			char sCommand[32] = "";
			char sMap[64] = "";
			int iTeam = -1;
			int iClass = -1;
			int iButtonType = -1;
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
				pButton = new CMapButton( sMap, iKeyBind, sText, StringCopy( sCommand ) );
			}
			else if ( iButtonType == BTN_TEAM )
			{
				pButton = new CTeamButton( iTeam, iKeyBind, sText, StringCopy( sCommand ) );
			}
			else if ( iButtonType == BTN_TOGGLE )
			{
				//Velaron: TODO
				pButton = new CCommandButton( iKeyBind, sText, StringCopy( sCommand ) );
			}
			else
			{
				pButton = new CCommandButton( iKeyBind, sText, StringCopy( sCommand ) );
			}

			if ( pButton )
			{
				m_pCurrentCommandMenu->AddItem( pButton );
			}

			if ( sCommand[0] == '{' && pButton )
			{
				CClientCommandMenu *cmdMenu = new CClientCommandMenu( true );
				cmdMenu->SetParent( m_pCurrentCommandMenu );
				pButton->SetCommandMenuCallback( cmdMenu ); 
				m_pCurrentCommandMenu = cmdMenu;
			}

			if ( bIsSubmenu )
			{
				pFile = EngFuncs::COM_ParseFile( pFile, sToken );
			}
		}

		m_pCurrentCommandMenu = m_pCurrentCommandMenu->Parent();
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

	if( !m_pParentCommandMenu )
	{
		pos.x = 0;
		pos.y = Max( ( 768 - size.h ) / 2, 0 );
	}
	else
	{
		while ( pos.y + size.h > 768 )
			pos.y -= BTN_HEIGHT;
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