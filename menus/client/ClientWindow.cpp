#include "BaseMenu.h"
#include "ClientWindow.h"

void CClientWindow::Show()
{
	if( !m_pStack->IsActive() )
	{
		EngFuncs::KEY_SetDest( KEY_MENU );
		EngFuncs::ClientCmd( TRUE, "touch_setclientonly 1");
	}
	
	BaseClass::Show();
}

void CClientWindow::Hide()
{		
	BaseClass::Hide();
	m_pStack->Clean();
	
	if( !m_pStack->IsActive() )
	{
		EngFuncs::KEY_SetDest( KEY_GAME );
		EngFuncs::ClientCmd( FALSE, "touch_setclientonly 0");
	}
}

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
	
	UI_DrawString( font, m_scPos, Size( m_scSize.w, UI_MED_CHAR_HEIGHT ),
		szTitle, PackRGB( 255, 174, 0 ), UI_MED_CHAR_HEIGHT, QM_LEFT, ETF_NOSIZELIMIT );

	BaseClass::Draw();
}

bool CClientWindow::KeyDown( int key )
{
	FOR_EACH_VEC ( m_pItems, i )
	{
		CClientMenuButton *pButton = static_cast<CClientMenuButton *>( m_pItems[i] );
		if ( pButton->m_iKeyBind == key )
			pButton->onPressed( pButton );
	}

	return BaseClass::KeyDown( key );
}

bool CClientWindow::KeyUp( int key )
{
	return false;
}
