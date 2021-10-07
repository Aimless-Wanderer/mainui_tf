/*
Copyright (C) 2021 Velaron.
UpdateAvailableDialog.cpp -- notify players of a new update

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

#ifdef _WIN32
#include <Windows.h>
#endif

#include "YesNoMessageBox.h"

#define ACTIONS_URL "https://github.com/Velaron/tf15-client/actions"

class CMenuUpdateAvailableDialog : public CMenuYesNoMessageBox
{
public:
	CMenuUpdateAvailableDialog() :
	    CMenuYesNoMessageBox( false )
	{
	}

	void OpenUpdatePage( void );
	void _Init( void ) override;
};

void CMenuUpdateAvailableDialog::OpenUpdatePage( void )
{
#if defined( _WIN32 )
	ShellExecute( NULL, "open", ACTIONS_URL, NULL, NULL, SW_SHOWNORMAL );
#elif defined( __linux__ )
	system( "x-www-browser " ACTIONS_URL );
#endif

	EngFuncs::ClientCmd( FALSE, "quit\n" );
}

void CMenuUpdateAvailableDialog::_Init( void )
{
	onPositive = VoidCb( &CMenuUpdateAvailableDialog::OpenUpdatePage );
	onNegative.SetCommand( FALSE, "quit\n" );

	SetMessage( L( "Update available.\nGo to the downloads page?" ) );
	bAutoHide = false;

	CMenuYesNoMessageBox::_Init();
}

ADD_MENU( menu_updateavailabledialog, CMenuUpdateAvailableDialog, UI_UpdateAvailableDialog_Show );