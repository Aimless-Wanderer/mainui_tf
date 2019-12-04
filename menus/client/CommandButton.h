#include "Action.h"

class CCommandButton : public CMenuAction
{
public:
	CCommandButton( const char *name, CEventCallback callback )
	{
		pos = Point( 0, 0 );
		onPressed = callback;
		SetBackground( PackRGBA( 0, 0, 0, 0 ), PackRGBA( 156, 77, 20, 128 ) );

		if( *name == '&' )
			name++;
			
		szName = StringCopy( name );
		SetCharSize( QM_DEFAULTFONT );
		eTextAlignment = QM_LEFT | QM_CENTER;

		m_bLimitBySize = true;
		size = Size( BTN_WIDTH, BTN_HEIGHT );

		bDrawStroke = true;
		iStrokeWidth = 1;
		colorStroke = PackRGBA( 156, 77, 20, 200 );
	}
};

class CSpectateButton : public CCommandButton
{
public:
	CSpectateButton( const char *name, CEventCallback callback ) : CCommandButton( name, callback ) {}

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
	CDisguiseButton( const char *name, CEventCallback callback ) : CCommandButton( name, callback ) {}

	bool IsVisible() const override
	{
		if ( g_pClient->GetPlayerClass() != PC_SPY )
			return false;

		return CCommandButton::IsVisible();
	}
};

class CFeignButton : public CCommandButton
{
public:
	CFeignButton( int state, const char *name, CEventCallback callback ) : CCommandButton( name, callback )
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