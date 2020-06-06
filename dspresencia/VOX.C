/*

			Voice Activated Transmission

*/

#include "netfone.h"

int noise_threshold = 420;

/*  LOAD_VOX_PARAMS  --  Restore VOX parameters from .ini file.  */

int load_vox_params(void)
{
	/*
	char *pfn = rstring(IDS_PF_PROFILE_FILE);
		
	voxmode = GetPrivateProfileInt(rstring(IDS_PF_VOX), rstring(IDS_PF_VOX_SPEED), voxmode, pfn);
	breakinput = GetPrivateProfileInt(rstring(IDS_PF_VOX), rstring(IDS_PF_VOX_BREAK), FALSE, pfn);
	noise_threshold = GetPrivateProfileInt(rstring(IDS_PF_VOX), rstring(IDS_PF_VOX_NOISE_THRESH), 330, pfn);
	*/
	return TRUE;
}

/*  SAVE_VOX_PARAMS  --  Save VOX parameters in .ini file.  */

int save_vox_params(void)
{
	/*
	char *pfn = rstring(IDS_PF_PROFILE_FILE);
	char buf[16];
	
	_ltoa(breakinput, buf, 10);
	WritePrivateProfileString(rstring(IDS_PF_VOX), rstring(IDS_PF_VOX_BREAK), buf, pfn);
	_ltoa(voxmode, buf, 10);
	WritePrivateProfileString(rstring(IDS_PF_VOX), rstring(IDS_PF_VOX_SPEED), buf, pfn);
	_ltoa(noise_threshold, buf, 10);
	WritePrivateProfileString(rstring(IDS_PF_VOX), rstring(IDS_PF_VOX_NOISE_THRESH), buf, pfn);
	*/
	return TRUE;
}
