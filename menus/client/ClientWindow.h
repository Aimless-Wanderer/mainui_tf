#pragma once

#include "BaseClientWindow.h"
#include "Action.h"
#include "EventSystem.h"

class CClientWindow : public CMenuBaseClientWindow
{
public:
	typedef CMenuBaseClientWindow BaseClass;
	CClientWindow( const char *name = "CClientWindow" ) : BaseClass( name )
	{
	}
	
	~CClientWindow()
	{
		FOR_EACH_VEC( m_pButtons, i )
		{
			delete m_pButtons[i];
		}
	}

	void Show() override
	{
		if( !m_pStack->IsActive() )
		{
			EngFuncs::KEY_SetDest( KEY_MENU );
			EngFuncs::ClientCmd( TRUE, "touch_setclientonly 1");
		}
		
		BaseClass::Show();
	}

	void Hide() override
	{		
		BaseClass::Hide();
		m_pStack->Clean();
		
		if( !m_pStack->IsActive() )
		{
			EngFuncs::KEY_SetDest( KEY_GAME );
			EngFuncs::ClientCmd( FALSE, "touch_setclientonly 0");
		}
	}

	CEventCallback ExecAndHide( const char *szCmd )
	{
		return CEventCallback( []( CMenuBaseItem *pSelf, void *pExtra )
		{
			pSelf->Parent()->Hide();
			EngFuncs::ClientCmd( FALSE, (const char*)pExtra );
		}, (void *)szCmd );
	}

	CMenuAction *AddButton( int key, const char *name, Point pos, CEventCallback callback );

	bool KeyDown( int key );
	void VidInit() override;
	void Draw() override;
	CEventCallback keys[10];

protected:
	CUtlVector<CMenuAction *> m_pButtons;
	const char *szTitle;
};
