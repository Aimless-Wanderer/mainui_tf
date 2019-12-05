#include "Action.h"

#define BS_HAS_BUILDING ( 1 << 0 )
#define BS_IS_BUILDING ( 1 << 1 )
#define BS_BASE ( 1 << 2 )
#define BS_CAN_BUILD ( 1 << 3 )

#define BS2_IS_BUILDING ( 1 << 0 )
#define BS2_HAS_DISPENSER ( 1 << 1 )
#define BS2_HAS_SENTRY ( 1 << 2 )
#define BS2_HAS_TELE_ENTRANCE ( 1 << 3 )
#define BS2_HAS_TELE_EXIT ( 1 << 4 )
#define BS2_CAN_BUILD_DISPENSER ( 1 << 5 )
#define BS2_CAN_BUILD_SENTRY ( 1 << 6 )
#define BS2_CAN_BUILD_TELE_ENTRANCE ( 1 << 7 )
#define BS2_CAN_BUILD_TELE_EXIT ( 1 << 8 )

#define DISGUISE_TEAM1 ( 1<<0 )
#define DISGUISE_TEAM2 ( 1<<1 )
#define DISGUISE_TEAM3 ( 1<<2 )
#define DISGUISE_TEAM4 ( 1<<3 )

enum
{
	BLD_DISPENSER = 0,
	BLD_SENTRY= 1,
	BLD_TELE_ENTRANCE = 2,
	BLD_TELE_EXIT = 3
};

class CCommandButton : public CMenuAction
{
public:
	CCommandButton( int key, const char *name, CClientWindow *cmdMenu ) : CCommandButton( key, name )
	{
		SetCommandMenuCallback( cmdMenu );
	}

	CCommandButton( int key, const char *name, const char* cmd ) : CCommandButton( key, name )
	{
		onPressed = CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
		{
			pSelf->Parent()->Hide();
			EngFuncs::ClientCmd( FALSE, (const char*)pExtra );
		}, (void *)cmd );
	}

	CCommandButton( int key, const char *name )
	{
		m_iKeyBind = key;

		if( !onPressed )
		{
			onPressed = CEventCallback( []( CMenuBaseItem *pSelf, void* pExtra )
			{
				pSelf->Parent()->Hide();
			});
		}
		
		pos = Point( 0, 0 );
		SetBackground( PackRGBA( 0, 0, 0, 0 ), PackRGBA( 156, 77, 20, 128 ) );

		if ( *name == '&' )
			name++;

		m_szLabel = new char[64];
		sprintf( m_szLabel, "%c %s", key, name );	
		szName = m_szLabel;
		SetCharSize( QM_DEFAULTFONT );
		eTextAlignment = QM_LEFT | QM_CENTER;

		m_bLimitBySize = true;
		size = Size( BTN_WIDTH, BTN_HEIGHT );

		bDrawStroke = true;
		iStrokeWidth = 1;
		colorStroke = PackRGBA( 156, 77, 20, 200 );
	}

	void SetCommandMenuCallback( CClientWindow* cmdMenu )
	{
		onPressed = CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
		{
			CClientWindow *cmdMenu = ( (CClientWindow *)pExtra );
			cmdMenu->pos.x = pSelf->Parent()->size.w + pSelf->Parent()->pos.x;
			cmdMenu->pos.y = pSelf->Parent()->pos.y + pSelf->pos.y;
			cmdMenu->Show();
		}, (void *)cmdMenu );
	}

	int m_iKeyBind;
	char *m_szLabel;
};

class CSpectateButton : public CCommandButton
{
public:
	CSpectateButton( int key, const char *name, const char *cmd ) : CCommandButton( key, name, cmd ) {}

	bool IsVisible() const override
	{
		if ( !g_pClient->GetAllowSpectators() )
			return false;

		return CCommandButton::IsVisible();
	}
};

class CDisguiseButton : public CCommandButton
{
public:
	CDisguiseButton( int validbits, int key, const char *name ) : CCommandButton( key, name )
	{
		m_iValidTeamsBits = validbits;
	}

	CDisguiseButton( int validbits, int key, const char *name, CClientWindow *cmdMenu ) : CCommandButton( key, name, cmdMenu )
	{
		m_iValidTeamsBits = validbits;
	}

	int m_iValidTeamsBits;

	bool IsVisible() const override
	{
		if ( g_pClient->GetPlayerClass() != PC_SPY )
			return false;
		
		if ( !m_iValidTeamsBits )
			return CCommandButton::IsVisible();
		
		int iMaxTeams = 1 << ( g_pClient->GetNumberOfTeams() - 1 );
		if ( m_iValidTeamsBits & iMaxTeams )
			return CCommandButton::IsVisible();

		return false;
	}
};

class CFeignButton : public CCommandButton
{
public:
	CFeignButton( int state, int key, const char *name, const char *cmd ) : CCommandButton( key, name, cmd )
	{
		m_iFeignState = state;
	}

	int	m_iFeignState;

	bool IsVisible() const override
	{
		if ( g_pClient->GetPlayerClass() != PC_SPY )
			return false;
		
		if ( m_iFeignState != g_pClient->GetIsFeigning() )
			return false;

		return CCommandButton::IsVisible();
	}
};

class CMapButton : public CCommandButton
{
public:
	CMapButton( const char *mapname, int key, const char *name, const char *cmd ) : CCommandButton( key, name, cmd )
	{
		sprintf( m_szMapName, "maps/%s.bsp", mapname );
	}

	char m_szMapName[64];

	bool IsVisible() const override
	{
		const char *szMap = g_pClient->GetLevelName();

		if ( !szMap )
			return false;

		if ( strcmp( m_szMapName, szMap ) )
			return false;

		return CCommandButton::IsVisible();
	}
};

class CTeamButton : public CCommandButton
{
public:
	CTeamButton( int team, int key, const char *name, const char *cmd ) : CCommandButton( key, name, cmd )
	{
		m_iTeam = team;
	}

	int m_iTeam;

	bool IsVisible() const override
	{
		if ( g_pClient->GetTeamNumber() != m_iTeam )
			return false;
		
		return CCommandButton::IsVisible();
	}
};

class CBuildButton : public CCommandButton
{
public:
	CBuildButton( int state, int data, int key, const char *name ) : CCommandButton( key, name )
	{
		m_iBuildState = state;
		m_iBuildData = data;
	}

	CBuildButton( int state, int data, int key, const char *name, const char *cmd ) : CCommandButton( key, name, cmd )
	{
		m_iBuildState = state;
		m_iBuildData = data;
	}

	int	m_iBuildState;
	int m_iBuildData;

	bool IsVisible() const override
	{
		if ( g_pClient->GetPlayerClass() != PC_ENGINEER )
			return false;
		
		if ( m_iBuildState & BS_IS_BUILDING )
		{
			if ( !( g_pClient->GetBuildState() & BS2_IS_BUILDING ) )
				return false;
		}
		else
		{
			if ( g_pClient->GetBuildState() & BS2_IS_BUILDING )
				return false;
		}

		if ( m_iBuildState & BS_BASE )
		{
			if ( g_pClient->GetBuildState() & ( BS2_HAS_SENTRY | BS2_HAS_DISPENSER | BS2_CAN_BUILD_SENTRY | BS2_CAN_BUILD_DISPENSER | BS2_HAS_TELE_ENTRANCE | BS2_HAS_TELE_EXIT | BS2_CAN_BUILD_TELE_ENTRANCE | BS2_CAN_BUILD_TELE_EXIT ) )
				return CCommandButton::IsVisible();

			return false;
		}

		if ( m_iBuildState & BS_HAS_BUILDING )
		{
			if ( m_iBuildData == BLD_DISPENSER && !( g_pClient->GetBuildState() & BS2_HAS_DISPENSER ) )
				return false;

			if ( m_iBuildData == BLD_SENTRY && !( g_pClient->GetBuildState() & BS2_HAS_SENTRY ) )
				return false;

			if ( m_iBuildData == BLD_TELE_ENTRANCE && !( g_pClient->GetBuildState() & BS2_HAS_TELE_ENTRANCE ) )
				return false;

			if ( m_iBuildData == BLD_TELE_EXIT && !( g_pClient->GetBuildState() & BS2_HAS_TELE_EXIT ) )
				return false;
		}

		if ( m_iBuildState & BS_CAN_BUILD )
		{
			if ( m_iBuildData == BLD_DISPENSER && ( g_pClient->GetBuildState() & BS2_CAN_BUILD_DISPENSER ) )
				return CCommandButton::IsVisible();

			if ( m_iBuildData == BLD_SENTRY && ( g_pClient->GetBuildState() & BS2_CAN_BUILD_SENTRY ) )
				return CCommandButton::IsVisible();

			if ( m_iBuildData == BLD_TELE_ENTRANCE && ( g_pClient->GetBuildState() & BS2_CAN_BUILD_TELE_ENTRANCE ) )
				return CCommandButton::IsVisible();

			if ( m_iBuildData == BLD_TELE_EXIT && ( g_pClient->GetBuildState() & BS2_CAN_BUILD_TELE_EXIT ) )
				return CCommandButton::IsVisible();

			return true;
		}

		return CCommandButton::IsVisible();
	}
};

class CDetpackButton : public CCommandButton
{
public:
	CDetpackButton( int state, int key, const char *name, const char *cmd ) : CCommandButton( key, name, cmd )
	{
		m_iDetpackState = state;
	}

	CDetpackButton( int state, int key, const char *name, CClientWindow *cmdMenu ) : CCommandButton( key, name, cmdMenu )
	{
		m_iDetpackState = state;
	}

	int	m_iDetpackState;

	bool IsVisible() const override
	{
		if ( g_pClient->GetPlayerClass() != PC_DEMOMAN )
			return false;

		if ( m_iDetpackState != g_pClient->GetIsSettingDetpack() )
			return false;

		return CCommandButton::IsVisible();
	}
};