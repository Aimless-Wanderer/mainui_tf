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
	
	UI_DrawString( font, m_scPos + Size( 40, 32 ), Size( m_scSize.w, 24 ),
		szTitle, PackRGB( 255, 174, 0 ), m_scChSize, QM_LEFT, ETF_NOSIZELIMIT );

	BaseClass::Draw();
}

bool CClientWindow::KeyDown( int key )
{
	if( key >= '0' && key <= '9' )
	{
		if( keys[key-'0'] )
		{
			(keys[key-'0'])( buttons[0] );
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
	act->SetCharSize( QM_SMALLFONT );
	act->size = Size( 124, 24 );
	act->bDrawStroke = true;
	act->m_bLimitBySize = true;
	act->colorStroke = PackRGBA( 156, 77, 20, 200 );
	//act->colorFocus = PackRGBA( 156, 77, 20, 128 );
	act->iStrokeWidth = 1;

	if( key >= '0' && key <= '9' )
		keys[key - '0'] = callback;

	buttons[m_iNumBtns] = act;
	m_iNumBtns++;

	AddItem( act );

	return act;
}
