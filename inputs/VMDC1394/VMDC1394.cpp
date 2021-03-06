#include "VMmoduleBase.h"
#include "DC1394DeviceInput.h"
#include "VideoManInputFormat.h"
#include "VideoInput.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

// DEFINE THE IDENTIFIERS THE MODULE WLL SUPPORT
const char *identifiers[1] = {"DC1394_CAPTURE_DEVICE"};
const int numIdentifiers = 1;

// DEFINE THE CONTROLLERS IDENTIFIERS THE MODULE WILL SUPPORT
const int numControllersIdentifiers = 1;
const char *controllersIdentifiers[1] = {"DC1394_CAPTURE_DEVICE_CONTROLLER"};


#ifdef WIN32
#define VMDC1394_API  __declspec(dllexport)
#elif defined linux
#define VMDC1394_API
#endif

#ifdef linux
/*** MODULE INITIALIZATION AND FINALIZATION  ***/

void __attribute__ ((constructor)) my_init(void);
void __attribute__ ((destructor)) my_fini(void);

static dc1394_t *d;

using namespace VideoMan;
using namespace VideoManPrivate;
using namespace std;

void copyStringToChar( const std::string &src, char **dst )
{
	if ( src.length() > 0 )
	{
		*dst = new char[src.length() + 1];
		strcpy( *dst, src.c_str() );
	}
	else
		*dst = NULL;
}

void my_init()
{
    d = NULL;
	d = dc1394_new ();

	if (d==NULL)
	{
        fprintf(stderr,"DC1394 Library no loaded");
        exit(EXIT_FAILURE);
    }

#ifdef _DEBUG
	fprintf(stderr,"DC1394 Loaded input plugin\n");
#endif
}

void my_fini()
{
	dc1394_free (d);
#ifdef _DEBUG
	fprintf(stderr,"DC1394 Unload input plugin\n");
#endif
}
#endif

VideoInput *initVideoInput( const VMInputIdentification &device, VMInputFormat *format )
{
	if ( strcmp(device.identifier,"DC1394_CAPTURE_DEVICE")==0 )
	{
		DC1394DeviceInput *input = new DC1394DeviceInput(d);
		if ( input->initInput( device, format ) )
			return input;
	}
	return NULL;
}


void getAvailableDevices( VMInputIdentification **deviceList, int &numDevices )
{
	dc1394camera_list_t *list;
	dc1394error_t err;

	err=dc1394_camera_enumerate (d, &list);
	//DC1394_ERR_RTN(err,"Failed to enumerate cameras");

    (*deviceList) = new VMInputIdentification[list->num];
    numDevices = list->num;

	static char identifier_int[] = "DC1394_CAPTURE_DEVICE";

	int i;
	for (i=0;i<list->num;++i)
	{
		(*deviceList)[i].identifier = identifier_int;
		ostringstream ss2;
		ss2 << list->ids[i].guid;					
		copyStringToChar( ss2.str(), &(*deviceList)[i].uniqueName );
	}

	dc1394_camera_free_list (list);
}

void freeAvailableDevices( VMInputIdentification **deviceList, int &numDevices )
{
        delete(*deviceList);
}


const char **getIdentifiers( int &_numIdentifiers )
{
	_numIdentifiers  = numIdentifiers;
	return identifiers;
}

const char **getControllerIdentifiers( int &_numIdentifiers )
{
	_numIdentifiers = numControllersIdentifiers;
	return controllersIdentifiers;
}

char* checkVersion( )
{
	#ifdef _DEBUG
		return "DEBUG" ;
	#endif
	#ifdef NDEBUG
		return "RELEASE";
	#endif
}

/*** METHOD FOR CREATING A CONTROLLER ***/
VideoManInputController *createController( const char *identifier )
{
}

void deleteInput( VideoInput **input )
{
	delete *input;
	*input = NULL;
}

void deleteController( VideoManInputController **controller )
{
	delete *controller;
	*controller = NULL;
}
