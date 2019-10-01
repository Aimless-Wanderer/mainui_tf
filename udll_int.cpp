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
#include "extdll_menu.h"
#include "BaseMenu.h"
#include "Utils.h"

ui_enginefuncs_t EngFuncs::engfuncs;
#ifndef XASH_DISABLE_FWGS_EXTENSIONS
ui_extendedfuncs_t	EngFuncs::textfuncs;
#endif
ui_globalvars_t	*gpGlobals;
IGameClientExports *g_pClient;
CMenu gMenu;

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
#ifndef XASH_DISABLE_FWGS_EXTENSIONS
	memset( &EngFuncs::textfuncs, 0, sizeof( ui_extendedfuncs_t ));
#endif
	gpGlobals = pGlobals;

	return TRUE;
}

#ifndef XASH_DISABLE_FWGS_EXTENSIONS
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

	bool IsActive() override
	{
		return uiStatic.client.IsActive() && !uiStatic.menu.IsActive();
	}

	bool IsMainMenuActive() override
	{
		return uiStatic.menu.IsActive();
	}

	void Key( int key, int down ) override
	{
		//uiStatic.client.KeyEvent( key, down );
		down ? uiStatic.client.KeyUpEvent( key ) : uiStatic.client.KeyDownEvent ( key );
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
		g_FontMgr.GetCharABCWide( font, ch, a, b, c );
	}

	int  GetFontTall( HFont font ) override
	{
		return g_FontMgr.GetFontTall( font );
	}

	int  GetCharacterWidth(HFont font, int ch, int charH ) override
	{
		return g_FontMgr.GetCharacterWidthScaled( font, ch, charH );
	}

	void GetTextSize( HFont font, const char *text, int *wide, int *height = 0, int size = -1 ) override
	{
		g_FontMgr.GetTextSize( font, text, wide, height, size );
	}

	int	 GetTextHeight( HFont font, const char *text, int size = -1 ) override
	{
		return g_FontMgr.GetTextHeight( font, text, size );
	}

	int  DrawCharacter( HFont font, int ch, int x, int y, int charH, const unsigned int color, bool forceAdditive = false ) override
	{
		return g_FontMgr.DrawCharacter( font, ch, Point( x, y ), charH, color, forceAdditive );
	}

	void SetupScoreboard( int xstart, int xend, int ystart, int yend, unsigned int color, bool drawStroke ) override
	{
		//UI_SetupScoreboard( xstart, xend, ystart, yend, color, drawStroke );
	}

	void DrawScoreboard( void ) override
	{
		//UI_DrawScoreboard();
	}

	void DrawSpectatorMenu( void ) override
	{
	}

	void ShowVGUIMenu( int menuType ) override
	{
		switch( menuType )
		{
			case MENU_TEAM: UI_TeamMenu_Show(); break;
		}
		/*
		switch( menuType )
		{
		case MENU_TEAM: UI_JoinGame_Show( param1, param2 ); break;
		case MENU_CLASS_T: UI_JoinClassT_Show( param1, param2 ); break;
		case MENU_CLASS_CT: UI_JoinClassCT_Show( param1, param2 ); break;
		case MENU_BUY: UI_BuyMenu_Show( param1, param2 ); break;
		case MENU_BUY_PISTOL: UI_BuyMenu_Pistol_Show( param1, param2 ); break;
		case MENU_BUY_SHOTGUN: UI_BuyMenu_Shotgun_Show( param1, param2 ); break;
		case MENU_BUY_RIFLE: UI_BuyMenu_Rifle_Show( param1, param2 ); break;
		case MENU_BUY_SUBMACHINEGUN: UI_BuyMenu_Submachine_Show( param1, param2 ); break;
		case MENU_BUY_MACHINEGUN: UI_BuyMenu_Machinegun_Show( param1, param2 ); break;
		case MENU_BUY_ITEM: UI_BuyMenu_Item_Show( param1, param2 ); break;
		}
		*/
	}
} s_Menu;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CGameMenuExports, IGameMenuExports, GAMEMENUEXPORTS_INTERFACE_VERSION, s_Menu );

#endif // XASH_DISABLE_FWGS_EXTENSIONS
