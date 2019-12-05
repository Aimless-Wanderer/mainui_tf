#pragma once

#include "Action.h"

#define BTN_HEIGHT 48
#define BTN_WIDTH 256
#define BTN_GAP 12

class CClientMenuButton : public CMenuAction
{
public:
	CClientMenuButton( int key, const char *name, Point pos, const char* cmd ) : CClientMenuButton( key, name , pos )
	{
		onPressed = CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
		{
			pSelf->Parent()->Hide();
			EngFuncs::ClientCmd( FALSE, (const char*)pExtra );
		}, (void *)cmd );
	}

	CClientMenuButton( int key, const char *name, Point btnpos ) : m_iKeyBind( key )
	{
		if( !onPressed )
		{
			onPressed = CEventCallback( []( CMenuBaseItem *pSelf, void* pExtra )
			{
				pSelf->Parent()->Hide();
			});
		}

		pos = btnpos;
		SetBackground( PackRGBA( 0, 0, 0, 0 ), PackRGBA( 156, 77, 20, 128 ) );

		if ( *name == '&' )
			name++;

		m_szLabel = new char[64];
		sprintf( m_szLabel, "%c %s", key, name );	
		szName = m_szLabel;

		SetCharSize( QM_SMALLFONT );
		eTextAlignment = QM_LEFT;

		m_bLimitBySize = true;
		size = Size( BTN_WIDTH, BTN_HEIGHT );

		bDrawStroke = true;
		iStrokeWidth = 1;
		colorStroke = PackRGBA( 156, 77, 20, 200 );
	}

	int m_iKeyBind;
	char *m_szLabel;
};