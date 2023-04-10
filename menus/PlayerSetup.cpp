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
#include "Bitmap.h"
#include "PicButton.h"
#include "CheckBox.h"
#include "Slider.h"
#include "Field.h"
#include "SpinControl.h"
#include "YesNoMessageBox.h"
#include "PlayerModelView.h"
#include "StringArrayModel.h"
#include "StringVectorModel.h"

#define ART_BANNER		"gfx/shell/head_customize"

#define MAX_PLAYERMODELS	100

static struct
{
	const char *name;
	int r, g, b;
} g_LogoColors[] =
{
{ "FullColor",     -1,  -1,  -1  },
{ "#Valve_Orange", 255, 120, 24  }, // L( "#Valve_Orange" )
{ "#Valve_Yellow", 225, 180, 24  }, // L( "#Valve_Yellow" )
{ "#Valve_Blue",   0,   60,  255 }, // L( "#Valve_Blue" )
{ "#Valve_Ltblue", 0,   167, 255 }, // L( "#Valve_Ltblue" )
{ "#Valve_Green",  0,   167, 0   }, // L( "#Valve_Green" )
{ "#Valve_Red",    255, 43,  0   }, // L( "#Valve_Red" )
{ "#Valve_Brown",  123, 73,  0   }, // L( "#Valve_Brown" )
{ "#Valve_Ltgray", 100, 100, 100 }, // L( "#Valve_Ltgray" )
{ "#Valve_Dkgray", 36,  36,  36  }, // L( "#Valve_Dkgray" )
};


static byte g_iCrosshairAvailColors[6][3] =
{
	{ 0,   0,   0   },
	{ 50,  250, 50  },
	{ 250, 50,  50  },
	{ 50,  50,  250 },
	{ 250, 250, 50  },
	{ 50,  250, 250 },
};

static const char *g_szCrosshairAvailSizes[] =
{
	"auto", "small", "medium", "large"
};

static const char *g_szCrosshairAvailColors[] =
{
	"Green", "Red", "Blue", "Yellow", "Ltblue"
};

class CMenuPlayerSetup : public CMenuFramework
{
private:
	void _Init() override;
	void Reload() override;
public:
	CMenuPlayerSetup() : CMenuFramework( "CMenuPlayerSetup" ), msgBox( true ) { }

	void SetConfig();
	void UpdateLogo();
	void ApplyColorToImagePreview();
	void ApplyColorToLogoPreview();
	void WriteNewLogo();
	void SaveAndPopMenu() override;

	class CLogosListModel : public CStringVectorModel
	{
	public:
		void Update() override;

		int GetFullPath( char *buf, size_t size, int pos )
		{
			const char *file, *ext;

			file = Element( pos ).String();
			ext = IsPng( pos ) ? "png" : "bmp";

			return snprintf( buf, size, "logos/%s.%s", file, ext );
		}

		bool IsPng( int pos )
		{
			return m_isPngs[pos];
		}

	private:
		CUtlVector<bool> m_isPngs;
	} logosModel;

	CMenuField	name;

	class CMenuLogoPreview : public CMenuBaseItem
	{
	public:
		virtual void Draw();
		int r, g, b;
		HIMAGE hImage;
	} logoImage;

	class CMenuCrosshairPreview : public CMenuBaseItem
	{
	public:
		virtual void Draw();
		int r, g, b;
		HIMAGE hWhite;
	} crosshairView;

	CMenuSpinControl	logo;
	CMenuSpinControl		logoColor;
	CMenuSpinControl	crosshairSize;
	CMenuSpinControl	crosshairColor;
	CMenuCheckBox	crosshairTranslucent;
	CMenuCheckBox	extendedMenus;

	CMenuYesNoMessageBox msgBox;

	bool hideModels, hideLogos;
};

void CMenuPlayerSetup::CMenuLogoPreview::Draw()
{
	if( !hImage )
	{
		// draw the background
		UI_FillRect( m_scPos, m_scSize, uiPromptBgColor );

		UI_DrawString( font, m_scPos, m_scSize, L( "No logo" ), colorBase, m_scChSize, QM_CENTER, ETF_SHADOW );
	}
	else
	{
		if( r != -1 && g != -1 && b != -1 )
			EngFuncs::PIC_Set( hImage, r, g, b );
		else
			EngFuncs::PIC_Set( hImage, 255, 255, 255 );
		EngFuncs::PIC_DrawTrans( m_scPos, m_scSize );
	}

	// draw the rectangle
	if( eFocusAnimation == QM_HIGHLIGHTIFFOCUS && IsCurrentSelected() )
		UI_DrawRectangle( m_scPos, m_scSize, uiInputTextColor );
	else
		UI_DrawRectangle( m_scPos, m_scSize, uiInputFgColor );

}

ADD_MENU3( menu_playersetup, CMenuPlayerSetup, UI_PlayerSetup_Menu );

void CMenuPlayerSetup::CMenuCrosshairPreview::Draw()
{

	UI_DrawPic(m_scPos, m_scSize, 0x00FFFFFF, "gfx/vgui/crosshair" );


	int l;
	switch( (int)menu_playersetup->crosshairSize.GetCurrentValue() )
	{
	case 1:
		l = 10;
		break;
	case 2:
		l = 20;
		break;
	case 3:
		l = 30;
		break;
	case 0:
		if( ScreenWidth < 640 )
			l = 30;
		else if( ScreenWidth < 1024 )
			l = 20;
		else l = 10;
	}

	l *= ScreenHeight / 768.0f;

	int x = m_scPos.x, // xpos
		y = m_scPos.y, // ypos
		w = m_scSize.w, // width
		h = m_scSize.h, // height
		// delta distance
		d = (m_scSize.w / 2 - l) * 0.5,
		// alpha
		a = 180,
		// red
		r = g_iCrosshairAvailColors[(int)menu_playersetup->crosshairColor.GetCurrentValue()+1][0],
		// green
		g = g_iCrosshairAvailColors[(int)menu_playersetup->crosshairColor.GetCurrentValue()+1][1],
		// blue
		b = g_iCrosshairAvailColors[(int)menu_playersetup->crosshairColor.GetCurrentValue()+1][2];

	if( menu_playersetup->crosshairTranslucent.bChecked )
	{
		// verical
		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + w / 2, y + d,         1, l );

		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + w / 2, y + h / 2 + d, 1, l );

		// horizontal
		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + d,         y + h / 2, l, 1 );

		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + w / 2 + d, y + h / 2, l, 1 );
	}
	else
	{
		// verical
		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + w / 2, y + d,         1, l );

		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + w / 2, y + h / 2 + d, 1, l );

		// horizontal
		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + d,         y + h / 2, l, 1 );

		EngFuncs::PIC_Set(hWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + w / 2 + d, y + h / 2, l, 1 );
	}
}


/*
=================
CMenuPlayerSetup::FindLogos

=================
*/
void CMenuPlayerSetup::CLogosListModel::Update( )
{
	char	**filenames;
	int numFiles, i;

	// Get file list
	filenames = EngFuncs::GetFilesList( "logos/*.*", &numFiles, FALSE );

	if( !filenames || !numFiles )
	{
		m_isPngs.RemoveAll();
		RemoveAll();
		return;
	}

	// build the model list
	for( i = 0; i < numFiles; i++ )
	{
		CUtlString logoFileName = filenames[i];
		char temp[256];
		bool png;

		if(( png = logoFileName.BEndsWithCaseless( ".png" )) ||
			 logoFileName.BEndsWithCaseless( ".bmp" ))
		{
			COM_FileBase( logoFileName.String(), temp );

			if( !stricmp( temp, "remapped" ))
				continue;

			AddToTail( temp );
			m_isPngs.AddToTail( png );
		}
	}
}

/*
=================
UI_PlayerSetup_SetConfig
=================
*/
void CMenuPlayerSetup::SetConfig( void )
{
	name.WriteCvar();
	char curColor[CS_SIZE];
	int i = menu_playersetup->crosshairColor.GetCurrentValue() + 1;
	snprintf( curColor, CS_SIZE, "%i %i %i",
			  g_iCrosshairAvailColors[i][0],
			  g_iCrosshairAvailColors[i][1],
			  g_iCrosshairAvailColors[i][2]);
	EngFuncs::CvarSetString( "cl_crosshair_color", curColor );
	crosshairSize.WriteCvar();
	crosshairTranslucent.WriteCvar();
	extendedMenus.WriteCvar();
	WriteNewLogo();
}

void CMenuPlayerSetup::SaveAndPopMenu()
{
	if( !UI::Names::CheckIsNameValid( name.GetBuffer() ) )
	{
		msgBox.Show();
		return;
	}

	SetConfig();
	CMenuFramework::SaveAndPopMenu();
}

void CMenuPlayerSetup::UpdateLogo()
{
	char filename[1024];
	int pos = logo.GetCurrentValue();

	if( pos < 0 )
		return;

	logosModel.GetFullPath( filename, sizeof( filename ), pos );
	logoImage.hImage = EngFuncs::PIC_Load( filename, 0 );
	if( logosModel.IsPng( pos ))
	{
		logoImage.r = logoImage.g = logoImage.b = -1;
		logoColor.SetGrayed( true );
	}
	else
	{
		CBMP *bmpFile = CBMP::LoadFile( filename );
		if( bmpFile->GetBitmapHdr()->bitsPerPixel == 8 )
		{
			ApplyColorToLogoPreview();
			logoColor.SetGrayed( false );
		}
		else
		{
			logoImage.r = logoImage.g = logoImage.b = -1;
			logoColor.SetGrayed( true );
		}
		delete bmpFile;
	}

	EngFuncs::CvarSetString( "cl_logofile", logo.GetCurrentString() );
	logoColor.WriteCvar();
}

void CMenuPlayerSetup::ApplyColorToLogoPreview()
{
	const char *logoColorStr = logoColor.GetCurrentString();

	for( size_t i = 0; i < V_ARRAYSIZE( g_LogoColors ) && logoColorStr; i++ )
	{
		if( !stricmp( logoColorStr, L( g_LogoColors[i].name )))
		{
			logoImage.r = g_LogoColors[i].r;
			logoImage.g = g_LogoColors[i].g;
			logoImage.b = g_LogoColors[i].b;
			return;
		}
	}

	logoColor.SetCurrentValue( L( g_LogoColors[0].name ) );
	logoImage.r = g_LogoColors[0].r;
	logoImage.g = g_LogoColors[0].g;
	logoImage.b = g_LogoColors[0].b;
}

void CMenuPlayerSetup::WriteNewLogo( void )
{
	char filename[1024];
	int pos = logo.GetCurrentValue();

	if( pos < 0 || hideLogos )
		return;

	EngFuncs::DeleteFile( "logos/remapped.png" );
	EngFuncs::DeleteFile( "logos/remapped.bmp" );

	logosModel.GetFullPath( filename, sizeof( filename ), pos );

	// TODO: check file size and throw a messagebox if it's too big?
	if( logosModel.IsPng( pos ))
	{
		int len;
		void *afile = EngFuncs::COM_LoadFile( filename, &len );

		// just copy file, nothing special
		EngFuncs::COM_SaveFile( "logos/remapped.png", afile, len );

		EngFuncs::COM_FreeFile( afile );

		EngFuncs::CvarSetString( "cl_logoext", "png" );
	}
	else
	{
		CBMP *bmpFile = CBMP::LoadFile( filename );

		// not valid logo BMP file
		if( !bmpFile )
			return;

		// remap logo if needed
		if( logoImage.r != -1 && logoImage.g != -1 && logoImage.b != -1 )
			bmpFile->RemapLogo( logoImage.r, logoImage.g, logoImage.b );

		EngFuncs::COM_SaveFile( "logos/remapped.bmp", bmpFile->GetBitmap(), bmpFile->GetBitmapHdr()->fileSize );
		EngFuncs::CvarSetString( "cl_logoext", "bmp" );

		delete bmpFile;
	}

	logo.WriteCvar();
	logoColor.WriteCvar();
}

/*
=================
UI_PlayerSetup_Init
=================
*/
void CMenuPlayerSetup::_Init( void )
{
	int addFlags = 0;

	hideModels = hideLogos = false;

	// disable playermodel preview for HLRally to prevent crash
	if( !stricmp( gMenu.m_gameinfo.gamefolder, "hlrally" ))
		hideModels = true;

	if( gMenu.m_gameinfo.flags & GFL_NOMODELS )
		addFlags |= QMF_INACTIVE;

	banner.SetPicture(ART_BANNER);

	name.szStatusText = L( "Enter your multiplayer display name" );
	name.iMaxLength = 32;
	name.LinkCvar( "name" );
	name.SetRect( 320, 260, 256, 36 );

	crosshairView.SetRect( 320, 340, 96, 96 );
	crosshairView.SetNameAndStatus( "Crosshair preview", "Choose dynamic crosshair" );
	crosshairView.hWhite = EngFuncs::PIC_Load("*white");

	static CStringArrayModel modelSizes( g_szCrosshairAvailSizes, V_ARRAYSIZE( g_szCrosshairAvailSizes ));
	crosshairSize.SetRect( 480, 345, 256, 26 );
	crosshairSize.SetNameAndStatus( "Crosshair size", "Set crosshair size" );
	crosshairSize.Setup(&modelSizes);
	crosshairSize.LinkCvar("cl_crosshair_size", CMenuEditable::CVAR_STRING);

	static CStringArrayModel modelColors( g_szCrosshairAvailColors, V_ARRAYSIZE( g_szCrosshairAvailColors ));
	crosshairColor.SetRect( 480, 415, 256, 26 );
	crosshairColor.SetNameAndStatus( "Crosshair color", "Set crosshair color" );
	crosshairColor.Setup(&modelColors);

	crosshairTranslucent.SetCoord( 320, 450 );
	crosshairTranslucent.SetNameAndStatus( "Translucent crosshair", "Set additive render crosshair" );
	crosshairTranslucent.LinkCvar( "cl_crosshair_translucent" );

	extendedMenus.SetCoord( 320, 500 );
	extendedMenus.SetNameAndStatus( "Extended touch menu", "Force touch menus for radio" );
	extendedMenus.LinkCvar( "_extended_menus" );

	msgBox.SetMessage( L( "Please, choose another player name" ) );
	msgBox.Link( this );

	AddItem( background );
	AddItem( banner );

	AddButton( L( "Done" ), L( "Go back to the Multiplayer Menu" ), PC_DONE, VoidCb( &CMenuPlayerSetup::SaveAndPopMenu ) );
	CMenuPicButton *gameOpt = AddButton( L( "Game options" ), L( "Configure handness, fov and other advanced options" ), PC_GAME_OPTIONS );
	SET_EVENT_MULTI( gameOpt->onReleased,
	{
		((CMenuPlayerSetup*)pSelf->Parent())->SetConfig();
		UI_AdvUserOptions_Menu();
	});

	AddButton( L( "Adv. Options" ), "", PC_ADV_OPT, UI_GameOptions_Menu );
	gameOpt->SetGrayed( !UI_AdvUserOptions_IsAvailable() );


	if( !hideLogos )
	{
		logosModel.Update();
		if( !logosModel.GetRows() )
		{
			// don't add to framework
			hideLogos = true;
		}
		else
		{
			static const char *itemlist[V_ARRAYSIZE( g_LogoColors )];
			static CStringArrayModel colors( itemlist, V_ARRAYSIZE( g_LogoColors ) );
			for( size_t i = 0; i < V_ARRAYSIZE( g_LogoColors ); i++ )
				itemlist[i] = L( g_LogoColors[i].name );

			logoImage.SetRect( 72, 230 + m_iBtnsNum * 50 + 10, 200, 200 );

			logo.Setup( &logosModel );
			logo.LinkCvar( "cl_logofile", CMenuEditable::CVAR_STRING );
			logo.onChanged = VoidCb( &CMenuPlayerSetup::UpdateLogo );
			logo.SetRect( 72, logoImage.pos.y + logoImage.size.h + UI_OUTLINE_WIDTH, 200, 32 );

			logoColor.Setup( &colors );
			logoColor.LinkCvar( "cl_logocolor", CMenuEditable::CVAR_STRING );
			logoColor.onChanged = VoidCb( &CMenuPlayerSetup::ApplyColorToLogoPreview );;
			logoColor.SetRect( 72, logo.pos.y + logo.size.h + UI_OUTLINE_WIDTH, 200, 32 );
		}
	}

	AddItem( name );
	AddItem( crosshairSize );
	AddItem( crosshairColor );
	AddItem( crosshairTranslucent );
	AddItem( extendedMenus );
	AddItem( crosshairView );


	if( !hideLogos )
	{
		UpdateLogo();
		AddItem( logo );
		AddItem( logoColor );
		AddItem( logoImage );
	}
}

void CMenuPlayerSetup::Reload()
{
	if( !hideLogos ) UpdateLogo();
}

/*
=================
UI_PlayerSetup_Menu
=================
*/
void UI_PlayerSetup_Menu( void )
{
	if ( gMenu.m_gameinfo.gamemode == GAME_SINGLEPLAYER_ONLY )
		return;

	menu_playersetup->Show();

	char curColor[CS_SIZE];
	int rgb[3];
	strncpy( curColor, EngFuncs::GetCvarString("cl_crosshair_color"), CS_SIZE);
	sscanf( curColor, "%d %d %d", rgb, rgb + 1, rgb + 2 );

	// check for custom colors
	int i;
	for( i = 1; i < 6; i++)
	{
		if( rgb[0] == g_iCrosshairAvailColors[i][0] &&
			rgb[1] == g_iCrosshairAvailColors[i][1] &&
			rgb[2] == g_iCrosshairAvailColors[i][2] )
		{
			break;
		}
	}

	if( i == 6 )
	{
		g_iCrosshairAvailColors[0][0] = rgb[0];
		g_iCrosshairAvailColors[0][1] = rgb[1];
		g_iCrosshairAvailColors[0][2] = rgb[2];
		menu_playersetup->crosshairColor.SetCurrentValue(curColor);
	}
	else
		menu_playersetup->crosshairColor.SetCurrentValue(i-1);
}