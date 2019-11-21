/*
dll_int.cpp - dll entry point
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
*/

#include "../cl_dll/IGameMenuExports.h"
#include "../cl_dll/tfc/tf_defs.h"
#include "extdll_menu.h"
#include "BaseMenu.h"
#include "Utils.h"

ui_enginefuncs_t EngFuncs::engfuncs;
ui_extendedfuncs_t EngFuncs::textfuncs;
ui_globalvars_t	*gpGlobals;
IGameClientExports *g_pClient;
CMenu gMenu;
bool g_bIsForkedEngine;

static UI_FUNCTIONS gFunctionTable = 
{
	UI_VidInit,
	UI_Init,
	UI_Shutdown,
	UI_UpdateMenu,
	UI_KeyEvent,
	UI_MouseMove,
	UI_SetActiveMenu,
	UI_AddServerToList,
	UI_GetCursorPos,
	UI_SetCursorPos,
	UI_ShowCursor,
	UI_CharEvent,
	UI_MouseInRect,
	UI_IsVisible,
	UI_CreditsActive,
	UI_FinalCredits
};

//=======================================================================
//			GetApi
//=======================================================================
extern "C" EXPORT int GetMenuAPI(UI_FUNCTIONS *pFunctionTable, ui_enginefuncs_t* pEngfuncsFromEngine, ui_globalvars_t *pGlobals)
{
	if( !pFunctionTable || !pEngfuncsFromEngine )
	{
		return FALSE;
	}

	// copy HUD_FUNCTIONS table to engine, copy engfuncs table from engine
	memcpy( pFunctionTable, &gFunctionTable, sizeof( UI_FUNCTIONS ));
	memcpy( &EngFuncs::engfuncs, pEngfuncsFromEngine, sizeof( ui_enginefuncs_t ));
	memset( &EngFuncs::textfuncs, 0, sizeof( ui_extendedfuncs_t ));

	gpGlobals = pGlobals;

	// can be hijacked, but please, don't do it
	const char *version = EngFuncs::GetCvarString( "host_ver" );

	g_bIsForkedEngine = version && version[0];

	return TRUE;
}

static UI_EXTENDED_FUNCTIONS gExtendedTable =
{
	AddTouchButtonToList,
	UI_MenuResetPing_f,
	UI_ConnectionWarning_f,
	UI_UpdateDialog,
	UI_ShowMessageBox,
	UI_ConnectionProgress_Disconnect,
	UI_ConnectionProgress_Download,
	UI_ConnectionProgress_DownloadEnd,
	UI_ConnectionProgress_Precache,
	UI_ConnectionProgress_Connect,
	UI_ConnectionProgress_ChangeLevel,
	UI_ConnectionProgress_ParseServerInfo
};

extern "C" EXPORT int GetExtAPI( int version, UI_EXTENDED_FUNCTIONS *pFunctionTable, ui_extendedfuncs_t *pEngfuncsFromEngine )
{
	if( !pFunctionTable || !pEngfuncsFromEngine )
	{
		return FALSE;
	}

	if( version != MENU_EXTENDED_API_VERSION )
	{
		Con_Printf( "Error: failed to initialize extended menu API. Expected by DLL: %d. Got from engine: %d\n", MENU_EXTENDED_API_VERSION, version );
		return FALSE;
	}

	memcpy( &EngFuncs::textfuncs, pEngfuncsFromEngine, sizeof( ui_extendedfuncs_t ) );
	memcpy( pFunctionTable, &gExtendedTable, sizeof( UI_EXTENDED_FUNCTIONS ));

	return TRUE;
}

void UI_TeamMenu_Show( void );
void UI_ClassMenu_Show( void );
void UI_CommandMenu_Show( void );
void UI_CommandMenu_Hide( void );

static class CGameMenuExports : public IGameMenuExports
{
public:
	bool Initialize( CreateInterfaceFn factory ) override
	{
		g_pClient = (IGameClientExports*)factory( GAMECLIENTEXPORTS_INTERFACE_VERSION, NULL );

		return g_pClient ? true : false;
	}

	const char *L( const char *szStr ) override
	{
		return ::L( szStr );
	}

	bool IsActive( void ) override
	{
		return uiStatic.client.IsActive() && !uiStatic.menu.IsActive();
	}

	bool IsMainMenuActive( void) override
	{
		return uiStatic.menu.IsActive();
	}

	void Key( int key, int down ) override
	{
		down ? uiStatic.client.KeyDownEvent( key ) : uiStatic.client.KeyUpEvent( key );
	}

	void MouseMove( int x, int y ) override
	{
		uiStatic.cursorX = x;
		uiStatic.cursorY = y;
		uiStatic.client.MouseEvent( x, y );
	}

	HFont BuildFont( CFontBuilder &builder ) override
	{
		return builder.Create();
	}

	void GetCharABCWide( HFont font, int ch, int &a, int &b, int &c ) override
	{
		g_FontMgr->GetCharABCWide( font, ch, a, b, c );
	}

	int GetFontTall( HFont font ) override
	{
		return g_FontMgr->GetFontTall( font );
	}

	int GetCharacterWidth( HFont font, int ch, int charH ) override
	{
		return g_FontMgr->GetCharacterWidthScaled( font, ch, charH );
	}

	void GetTextSize( HFont font, const char *text, int *wide, int *height = 0, int size = -1 ) override
	{
		g_FontMgr->GetTextSize( font, text, wide, height, size );
	}

	int	 GetTextHeight( HFont font, const char *text, int size = -1 ) override
	{
		return g_FontMgr->GetTextHeight( font, text, size );
	}

	int DrawCharacter( HFont font, int ch, int x, int y, int charH, const unsigned int color, bool forceAdditive = false ) override
	{
		return g_FontMgr->DrawCharacter( font, ch, Point( x, y ), charH, color, forceAdditive );
	}

	void ShowVGUIMenu( int menuType ) override
	{
		if ( IsActive() ) return;
		switch( menuType )
		{
			case MENU_TEAM: UI_TeamMenu_Show(); break;
			case MENU_CLASS: UI_ClassMenu_Show(); break;
		}
	}

	void OpenCommandMenu( void ) override
	{
		UI_CommandMenu_Show();
	}

	void CloseCommandMenu( void ) override
	{
		if ( !IsActive() ) return;
		UI_CommandMenu_Hide();
	}
} s_Menu;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGameMenuExports, IGameMenuExports, GAMEMENUEXPORTS_INTERFACE_VERSION, s_Menu );