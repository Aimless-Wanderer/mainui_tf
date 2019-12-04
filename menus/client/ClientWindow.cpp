#include "BaseMenu.h"
#include "ClientWindow.h"

CClientWindow::~CClientWindow()
{
	FOR_EACH_VEC( m_pItems, i )
	{
		delete m_pItems[i];
	}
}

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

CEventCallback CClientWindow::ExecAndHide( const char *szCmd )
{
	return CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
	{
		pSelf->Parent()->Hide();
		EngFuncs::ClientCmd( FALSE, (const char*)pExtra );
	}, (void *)szCmd );
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
	
	UI_DrawString( font, m_scPos, Size( m_scSize.w, UI_BIG_CHAR_HEIGHT ),
		szTitle, PackRGB( 255, 174, 0 ), UI_BIG_CHAR_HEIGHT, QM_LEFT, ETF_NOSIZELIMIT );

	BaseClass::Draw();
}

bool CClientWindow::KeyDown( int key )
{
	return BaseClass::KeyDown( key );
}

bool CClientWindow::KeyUp( int key )
{
	return false;
}

CMenuAction *CClientWindow::AddButton( const char *name, Point pos, CEventCallback callback )
{
	CMenuAction *act = new CMenuAction();

	act->pos = pos;
	act->onPressed = callback;
	act->SetBackground( PackRGBA( 0, 0, 0, 0 ), PackRGBA( 156, 77, 20, 128 ) );

	if( *name == '&' ) // fast hack
		name++;
		
	act->szName = name;
	act->SetCharSize( QM_DEFAULTFONT );
	act->eTextAlignment = QM_LEFT | QM_CENTER;

	act->m_bLimitBySize = true;
	act->size = Size( BTN_WIDTH, BTN_HEIGHT );

	act->bDrawStroke = true;
	act->iStrokeWidth = 1;
	act->colorStroke = PackRGBA( 156, 77, 20, 200 );

	AddItem( act );

	return act;
}
