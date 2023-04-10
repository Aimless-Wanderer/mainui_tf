/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


#include "Framework.h"
#include "Action.h"
#include "Bitmap.h"
#include "PicButton.h"
#include "YesNoMessageBox.h"
#include "keydefs.h"
#include "MenuStrings.h"
#include "PlayerIntroduceDialog.h"

#define ART_MINIMIZE_N	"gfx/shell/vkBtn_128"
#define ART_MINIMIZE_F	"gfx/shell/vkBtn_128"
#define ART_MINIMIZE_D	"gfx/shell/vkBtn_128"
#define ART_CLOSEBTN_N	"gfx/shell/diBtn_128"
#define ART_CLOSEBTN_F	"gfx/shell/diBtn_128"
#define ART_CLOSEBTN_D	"gfx/shell/diBtn_128"

class CMenuMain: public CMenuFramework
{
public:
	CMenuMain() : CMenuFramework( "CMenuMain" ) { }

	bool KeyDown( int key ) override;

private:
	void _Init() override;
	void _VidInit( ) override;

	void VidInit(bool connected);

	void QuitDialog( void *pExtra = NULL );
	void DisconnectCb();
	void DisconnectDialogCb();

	CMenuPicButton	console;

	CMenuPicButton	resumeGame;
	CMenuPicButton	disconnect;
	CMenuPicButton	createGame;
	CMenuPicButton	configuration;
	CMenuPicButton	multiPlayer;
	CMenuPicButton	previews;
	CMenuPicButton	quit;

	// buttons on top right. Maybe should be drawn if fullscreen == 1?
	CMenuBitmap	minimizeBtn;
	CMenuBitmap	quitButton;

	// quit dialog
	CMenuYesNoMessageBox dialog;
};

void CMenuMain::QuitDialog(void *pExtra)
{
	if( CL_IsActive() && EngFuncs::GetCvarFloat( "host_serverstate" ) && EngFuncs::GetCvarFloat( "maxplayers" ) == 1.0f )
		dialog.SetMessage( L( "StringsList_235" ) );
	else
		dialog.SetMessage( L( "GameUI_QuitConfirmationText" ) );

	dialog.onPositive.SetCommand( FALSE, "quit\n" );
	dialog.Show();
}

void CMenuMain::DisconnectCb()
{
	EngFuncs::ClientCmd( FALSE, "disconnect\n" );
	VidInit( false );
}

void CMenuMain::DisconnectDialogCb()
{
	dialog.onPositive = VoidCb( &CMenuMain::DisconnectCb );
	dialog.SetMessage( L( "Really disconnect?" ) );
	dialog.Show();
}

/*
=================
CMenuMain::Key
=================
*/
bool CMenuMain::KeyDown( int key )
{
	if( UI::Key::IsEscape( key ) )
	{
		if ( CL_IsActive( ))
		{
			if( !dialog.IsVisible() )
				UI_CloseMenu();
		}
		else
		{
			QuitDialog( );
		}
		return true;
	}
	return CMenuFramework::KeyDown( key );
}

void CMenuMain::_Init( void )
{
	// console
	console.SetNameAndStatus( L( "GameUI_Console" ), L( "Show console" ) );
	console.iFlags |= QMF_NOTIFY;
	console.SetPicture( PC_CONSOLE );
	SET_EVENT_MULTI( console.onReleased,
	{
		UI_SetActiveMenu( FALSE );
		EngFuncs::KEY_SetDest( KEY_CONSOLE );
	});

	createGame.SetNameAndStatus( L( "GameUI_GameMenu_CreateServer" ), L( "StringsList_189" ) );
	createGame.SetPicture( PC_CREATE_GAME );
	createGame.iFlags |= QMF_NOTIFY;
	createGame.onReleased = UI_CreateGame_Menu;

	resumeGame.SetNameAndStatus( L( "GameUI_GameMenu_ResumeGame" ), L( "StringsList_188" ) );
	resumeGame.SetPicture( PC_RESUME_GAME );
	resumeGame.iFlags |= QMF_NOTIFY;
	resumeGame.onReleased = UI_CloseMenu;

	disconnect.SetNameAndStatus( L( "GameUI_GameMenu_Disconnect" ), L( "Disconnect from server" ) );
	disconnect.SetPicture( PC_DISCONNECT );
	disconnect.iFlags |= QMF_NOTIFY;
	disconnect.onReleased = VoidCb( &CMenuMain::DisconnectDialogCb );

	multiPlayer.SetNameAndStatus( L( "GameUI_Multiplayer" ), L( "StringsList_198" ) );
	multiPlayer.SetPicture( PC_MULTIPLAYER );
	multiPlayer.iFlags |= QMF_NOTIFY;
	multiPlayer.onReleased = UI_MultiPlayer_Menu;

	configuration.SetNameAndStatus( L( "GameUI_Options" ), L( "StringsList_193" ) );
	configuration.SetPicture( PC_CONFIG );
	configuration.iFlags |= QMF_NOTIFY;
	configuration.onReleased = UI_Options_Menu;

	previews.SetNameAndStatus( L( "Credits" ), L( "StringsList_194" ) );
	previews.SetPicture( PC_VIEW_README );
	previews.iFlags |= QMF_NOTIFY;
	previews.onReleased = UI_FinalCredits;

	quit.SetNameAndStatus( L( "GameUI_GameMenu_Quit" ), L( "StringsList_196" ) );
	quit.SetPicture( PC_QUIT );
	quit.iFlags |= QMF_NOTIFY;
	quit.onReleased = MenuCb( &CMenuMain::QuitDialog );

	dialog.Link( this );

	quitButton.SetPicture( ART_CLOSEBTN_N, ART_CLOSEBTN_F, ART_CLOSEBTN_D );
	quitButton.SetRenderMode( QM_DRAWTRANS, QM_DRAWTRANS, QM_DRAWTRANS );
	quitButton.iFlags = QMF_MOUSEONLY;
	quitButton.eFocusAnimation = QM_HIGHLIGHTIFFOCUS;
	SET_EVENT( quitButton.onReleased, EngFuncs::ShellExecute( "http://discord.me/fwgs", NULL, false ));

	minimizeBtn.SetPicture( ART_MINIMIZE_N, ART_MINIMIZE_F, ART_MINIMIZE_D );
	minimizeBtn.SetRenderMode( QM_DRAWTRANS, QM_DRAWTRANS, QM_DRAWTRANS );
	minimizeBtn.iFlags = QMF_MOUSEONLY;
	minimizeBtn.eFocusAnimation = QM_HIGHLIGHTIFFOCUS;
	SET_EVENT( minimizeBtn.onReleased, EngFuncs::ShellExecute( "http://vk.com/cs_xash3d", NULL, false ));

	AddItem( background );

	if ( gpGlobals->developer )
		AddItem( console );

	AddItem( disconnect );
	AddItem( resumeGame );
	AddItem( createGame );
	AddItem( multiPlayer );
	AddItem( configuration );
	AddItem( previews );
	AddItem( quit );
	AddItem( minimizeBtn );
	AddItem( quitButton );
}

/*
=================
UI_Main_Init
=================
*/
void CMenuMain::VidInit( bool connected )
{
	if( connected )
	{
		console.SetCoord( 72, 280 );

		resumeGame.Show();
		disconnect.Show();
		resumeGame.SetCoord( 72, 330 );
		disconnect.SetCoord( 72, 380 );
	}
	else
	{
		resumeGame.Hide();
		disconnect.Hide();
		console.SetCoord( 72, 380 );
	}

	createGame.SetCoord( 72, 430 );
	multiPlayer.SetCoord( 72, 480 );
	configuration.SetCoord( 72, 530 );
	previews.SetCoord( 72, 580 );
	quit.SetCoord( 72, 630 );

	minimizeBtn.SetRect( uiStatic.width - 144 - 13, 13, 64, 64 );
	quitButton.SetRect( uiStatic.width - 72, 13, 64, 64 );
}

void CMenuMain::_VidInit()
{
	VidInit( CL_IsActive() );
}

ADD_MENU( menu_main, CMenuMain, UI_Main_Menu );
