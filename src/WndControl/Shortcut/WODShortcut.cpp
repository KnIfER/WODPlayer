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

	_accelerators.push_back({FVIRTKEY, VK_P, IDM_BKMK_ADD});


	_hAccTable = ::CreateAcceleratorTable(_accelerators.data(), _accelerators.size());

}
