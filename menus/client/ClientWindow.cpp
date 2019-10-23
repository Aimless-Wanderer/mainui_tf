#include "BaseMenu.h"
#include "ClientWindow.h"

void CClientWindow::VidInit()
{
	size.w = 800;
	size.h = 600;
	pos.x = ( ( ScreenWidth - uiStatic.scaleX * 1024 ) / 2 ) / uiStatic.scaleX + 84;
	pos.y = 84;

	BaseClass::VidInit();
}

void CClientWindow::Draw()
{
	UI_FillRect( Point( 0, 0 ), Size( ScreenWidth, ScreenHeight ), PackRGBA( 0, 0, 0 , 128 ) );
	
	UI_DrawString( font, m_scPos, Size( m_scSize.w, UI_BIG_CHAR_HEIGHT ),
		szTitle, PackRGB( 255, 174, 0 ), UI_BIG_CHAR_HEIGHT, QM_LEFT, ETF_NOSIZELIMIT );

	BaseClass::Draw();
}

bool CClientWindow::KeyDown( int key )
{
	if( key >= '0' && key <= '9' )
	{
		if( keys[key-'0'] )
		{
			(keys[key-'0'])( m_pButtons[0] );
			return uiSoundNull;
		}
	}

	return BaseClass::KeyDown( key );
}

CMenuAction *CClientWindow::AddButton( int key, const char *name, Point pos, CEventCallback callback )
{
	CMenuAction *act = new CMenuAction();

	act->pos = pos;
	act->onPressed = callback;
	act->SetBackground( 0U, PackRGBA( 0, 0, 0, 0 ) );

	if( *name == '&' ) // fast hack
		name++;
		
	act->szName = name;
	act->SetCharSize( QM_DEFAULTFONT );
	act->eTextAlignment = QM_LEFT | QM_CENTER;
	act->size = Size( BTN_WIDTH, BTN_HEIGHT );
	act->bDrawStroke = true;
	act->m_bLimitBySize = true;
	act->colorStroke = PackRGBA( 156, 77, 20, 200 );
	act->colorFocus = PackRGBA( 156, 77, 20, 128 );
	act->iStrokeWidth = 1;

	if( key >= '0' && key <= '9' )
		keys[key - '0'] = callback;

	m_pButtons.AddToTail( act );

	AddItem( act );

	return act;
}
