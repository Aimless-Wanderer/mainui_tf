#include "BaseClientWindow.h"

class CCommandMenu : public CMenuBaseClientWindow
{
public:
	typedef CMenuBaseClientWindow BaseClass;
	CCommandMenu( const char *name = "CCommandMenu" ) : BaseClass( name )
	{
	}
};