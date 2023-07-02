#include "pch.h"
#include "virtual_keys.h"
#include "WODMenuID.h"


std::vector<ACCEL> _accelerators;

//#define FVIRTKEY  TRUE          /* Assumed to be == TRUE */
//#define FNOINVERT 0x02
//#define FSHIFT    0x04
//#define FCONTROL  0x08
//#define FALT      0x10

void WODApplication::buildAccelerator()
{

	_accelerators.push_back({FVIRTKEY, VK_I, IDM_BKMK_ADD});
	_accelerators.push_back({FVIRTKEY, VK_O, IDM_BKMK_ADD});
	_accelerators.push_back({FVIRTKEY, VK_P, IDM_BKMK_ADD});

	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_W, IDM_SHUTDOWN});
	_accelerators.push_back({FVIRTKEY|FALT, VK_F4, IDM_SHUTDOWN});
	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_MBUTTON, IDM_FULLSCREEN});
	_accelerators.push_back({FVIRTKEY, VK_RETURN, IDM_MAXMISE});
	//_accelerators.push_back({FVIRTKEY, VK_RETURN, IDM_FULLSCREEN});
	_accelerators.push_back({FVIRTKEY, VK_SPACE, IDM_PAUSE});

	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_DELETE, IDM_DELETE});
	_accelerators.push_back({FVIRTKEY|FCONTROL|FSHIFT, VK_DELETE, IDM_DELETE_FOREVER});

	_accelerators.push_back({FVIRTKEY, VK_PRIOR, IDM_PLAY_PRV});
	_accelerators.push_back({FVIRTKEY, VK_NEXT, IDM_PLAY_NXT});

	// bookmark
	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_PRIOR, IDM_BKMK_PRV});
	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_NEXT, IDM_BKMK_NXT});
	_accelerators.push_back({FVIRTKEY|FSHIFT|FCONTROL, VK_LEFT, IDM_BKMK_PRV});
	_accelerators.push_back({FVIRTKEY|FSHIFT|FCONTROL, VK_RIGHT, IDM_BKMK_NXT});
	// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	_accelerators.push_back({FVIRTKEY, VK_OEM_4, IDM_BKMK_PRV});
	_accelerators.push_back({FVIRTKEY, VK_OEM_6, IDM_BKMK_NXT});

	_accelerators.push_back({FVIRTKEY, VK_OEM_1, IDM_BKMK_DEL});
	_accelerators.push_back({FVIRTKEY, VK_OEM_7, IDM_BKMK_DEL});

	_accelerators.push_back({FVIRTKEY, VK_H, IDM_BKMK_RETURN});
	_accelerators.push_back({FVIRTKEY|FALT, VK_P, IDM_BKMK_DEL});
	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_P, IDM_BKMK_DEL});

	// seek
	_accelerators.push_back({FVIRTKEY, VK_RIGHT, IDM_SEEK_FORE});
	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_RIGHT, IDM_SEEK_FORE_FAST});
	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_RIGHT, IDM_SEEK_FORE_FASTER});
	_accelerators.push_back({FVIRTKEY, VK_LEFT, IDM_SEEK_BACK});
	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_LEFT, IDM_SEEK_BACK_FAST});
	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_LEFT, IDM_SEEK_BACK_FASTER});


	// speed
	_accelerators.push_back({FVIRTKEY, VK_C, IDM_SPEED_UP});
	_accelerators.push_back({FVIRTKEY, VK_X, IDM_SPEED_DOWN});
	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_C, IDM_SPEED_UP_FAST});
	_accelerators.push_back({FVIRTKEY|FCONTROL, VK_X, IDM_SPEED_DOWN_FAST});
	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_C, IDM_SPEED_UP_FASTER});
	_accelerators.push_back({FVIRTKEY|FSHIFT, VK_X, IDM_SPEED_DOWN_FASTER});
	_accelerators.push_back({FVIRTKEY, VK_Z, IDM_SPEED_RESET});

	// volume
	_accelerators.push_back({FVIRTKEY, VK_UP, IDM_VOLUME_UP});
	_accelerators.push_back({FVIRTKEY, VK_DOWN, IDM_VOLUME_DOWN});

	_hAccTable = ::CreateAcceleratorTable(_accelerators.data(), _accelerators.size());

}
