
#include "netfone.h"

#define TRACE nula
#define	BAD_DWORD	(DWORD)-1


extern int UsarElCtrl;
extern int NoEnviarAudio;

static int nula()
{
	return 0;
}

#ifdef BASURA
static int BuscaLinea(DWORD dwSelectControlID,DWORD dwMultipleItems,DWORD dwComponentType,volumen *pvolumen,int oChannels)
{
		MIXERCONTROLDETAILS mxcd;
	    MIXERCONTROLDETAILS_LISTTEXT *pmxcdSelectText;
	    MIXERCONTROL Control;
	    MIXERLINECONTROLS LineControls;
		int Index = -1;
		HRESULT mmResult;

		pmxcdSelectText = (MIXERCONTROLDETAILS_LISTTEXT*)malloc(sizeof(MIXERCONTROLDETAILS_LISTTEXT)*dwMultipleItems);

		mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID = dwSelectControlID;
		mxcd.cChannels = 1;
		mxcd.cMultipleItems = dwMultipleItems;
		mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
		mxcd.paDetails = pmxcdSelectText;
		if (mixerGetControlDetails((HMIXEROBJ)hmx,
									 &mxcd,
									 MIXER_OBJECTF_HMIXER |
									 MIXER_GETCONTROLDETAILSF_LISTTEXT)
			== MMSYSERR_NOERROR)
		{
			DWORD dwi;
			MIXERLINE mxl;
			// determine which controls the Microphone source line
			for (dwi = 0; dwi < dwMultipleItems; dwi++)
			{				
				mxl.cbStruct = sizeof(MIXERLINE);
				mxl.dwLineID = pmxcdSelectText[dwi].dwParam1;
				if (mixerGetLineInfo((HMIXEROBJ)hmx,
									   &mxl,
									   MIXER_OBJECTF_HMIXER |
									   MIXER_GETLINEINFOF_LINEID)
					== MMSYSERR_NOERROR &&					
					mxl.dwComponentType == dwComponentType)
				{
					// found, dwi is the index.
					if (pvolumen)
					{
						memset( &Control, 0, sizeof(MIXERCONTROL) );
						Control.cbStruct = sizeof(MIXERCONTROL);					
						memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
						LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
						LineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
						LineControls.dwLineID = mxl.dwLineID;		
						LineControls.cControls = 1;
						LineControls.cbmxctrl = sizeof(MIXERCONTROL);
						LineControls.pamxctrl = &Control;
						mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
						if ( mmResult == MMSYSERR_NOERROR )
						{
							pvolumen->dwChannelCount = oChannels;		
							pvolumen->dwLineID = LineControls.dwLineID;
							pvolumen->dwControlID = Control.dwControlID;
							pvolumen->dwEnabled = TRUE;;
							pvolumen->dwMinimalVolume = Control.Bounds.dwMinimum;
							pvolumen->dwMaximalVolume = Control.Bounds.dwMaximum;
							pvolumen->dwVolumeStep = Control.Metrics.cSteps;							
							pvolumen->dwMute = 0;
							pvolumen->dwControlMute = BAD_DWORD;
							pvolumen->dwActualVolume = 0;
							GetCurrentVolumen(pvolumen);
							SetCurrentVolumen( pvolumen, pvolumen->dwActualVolume, (DWORD)FALSE );
						}
					}
					Index = (int)dwi;
					//m_strMicName = pmxcdSelectText[dwi].szName;
					break;
				}
			}

			if (dwi >= dwMultipleItems)
			{
				// could not find it using line IDs, some mixer drivers have
				// different meaning for MIXERCONTROLDETAILS_LISTTEXT.dwParam1.
				// let's try comparing the item names.
				for (dwi = 0; dwi < dwMultipleItems; dwi++)
				{
					if (lstrcmp(pmxcdSelectText[dwi].szName,
								  "Microphone") == 0)
					{
						// found, dwi is the index.
						if (pvolumen)
						{
							memset( &Control, 0, sizeof(MIXERCONTROL) );
							Control.cbStruct = sizeof(MIXERCONTROL);					
							memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
							LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
							LineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
							LineControls.dwLineID = pmxcdSelectText[dwi].dwParam1;
							LineControls.cControls = 1;
							LineControls.cbmxctrl = sizeof(MIXERCONTROL);
							LineControls.pamxctrl = &Control;
							mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
							if ( mmResult == MMSYSERR_NOERROR )
							{
								pvolumen->dwChannelCount = oChannels;		
								pvolumen->dwLineID = LineControls.dwLineID;
								pvolumen->dwControlID = Control.dwControlID;
								pvolumen->dwEnabled = TRUE;
								pvolumen->dwMinimalVolume = Control.Bounds.dwMinimum;
								pvolumen->dwMaximalVolume = Control.Bounds.dwMaximum;
								pvolumen->dwVolumeStep = Control.Metrics.cSteps;							
							    pvolumen->dwMute = 0;
							    pvolumen->dwControlMute = BAD_DWORD;
							    pvolumen->dwActualVolume = 0;
							    GetCurrentVolumen(pvolumen);
							    SetCurrentVolumen( pvolumen, pvolumen->dwActualVolume, (DWORD)FALSE );
							}
						}
						Index = (int)dwi;
						//m_strMicName = pmxcdSelectText[dwi].szName;
						break;
					}
				}
			}
		}
		free((void *)pmxcdSelectText);

		return Index;
}
#endif

static int enregistro = 0;
int RecogeVolumenes(volumen *avolumen[],int nvols)
{
HKEY hKey;
LONG retv;
DWORD Disposition;
DWORD Type;
DWORD Size;
DWORD vol,mute;
char data[512];
int i;
volumen *pvolumen;

enregistro++;
if (						   /*  */
  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\Voz",0,
                 "Voz",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,				 
				 &hKey,&Disposition)) == ERROR_SUCCESS
	)
  {  
	  
	  
	  for (i = 0;i < nvols;i++)
	  {
		  pvolumen = avolumen[i];
		  if (!pvolumen)
			  continue;
		  vol = pvolumen->dwActualVolume;
		  mute = pvolumen->dwMute;

		  Size = sizeof(pvolumen->dwActualVolume);
		  Type = REG_DWORD;
		  sprintf(data,"Level_%lX",pvolumen->dwControlID);
		  retv = RegQueryValueEx(  hKey,data,NULL,&Type,&(pvolumen->dwActualVolume),&Size);  
		  Size = sizeof(pvolumen->dwMute);
		  Type = REG_DWORD;
		  sprintf(data,"Mute_%lX",pvolumen->dwControlID);
		  retv = RegQueryValueEx(  hKey,data,NULL,&Type,&(pvolumen->dwMute),&Size);  	  
		  if (vol != pvolumen->dwActualVolume || mute != pvolumen->dwMute)
		  {			  
			  SetCurrentVolumen(pvolumen,pvolumen->dwActualVolume,pvolumen->dwMute);			  
		  }
	  }


      Size = sizeof(Disposition);
      Type = REG_DWORD;	  
	  retv = RegQueryValueEx(  hKey,"CTRL",NULL,&Type,&(Disposition),&Size);  	  
	  UsarElCtrl = (int)Disposition;

      Size = sizeof(Disposition);
      Type = REG_DWORD;	  
	  retv = RegQueryValueEx(  hKey,"NULO",NULL,&Type,&(Disposition),&Size);  	  
	  NoEnviarAudio = (int)Disposition;

	  RegCloseKey(hKey);
  }
enregistro--;
return 0;
}


int GrabaVolumenes(volumen *avolumen[],int nvols)
{
HKEY hKey;
LONG retv;
DWORD Disposition;
char data[512];
volumen *pvolumen;
int i;


enregistro++;
	if (						   /*  */
	  (retv = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\Diagram Software S.L.\\Voz",0,
					 "Voz",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,				 
					 &hKey,&Disposition)) == ERROR_SUCCESS
		)
	{
		  for (i = 0;i < nvols;i++)
		  {
			  pvolumen = avolumen[i];
			  if (!pvolumen)
				  continue;
  			  sprintf(data,"Level_%lX",pvolumen->dwControlID);
			  retv = RegSetValueEx(hKey,data,0,REG_DWORD,&(pvolumen->dwActualVolume),sizeof(DWORD));
			  sprintf(data,"Mute_%lX",pvolumen->dwControlID);
			  retv = RegSetValueEx(hKey,data,0,REG_DWORD,&(pvolumen->dwMute),sizeof(DWORD));	  	  

		  }

		  Disposition = (DWORD)UsarElCtrl;
		  retv = RegSetValueEx(hKey,"CTRL",0,REG_DWORD,&(Disposition),sizeof(DWORD));	  
		  Disposition = (DWORD)NoEnviarAudio;
		  retv = RegSetValueEx(hKey,"NULO",0,REG_DWORD,&(Disposition),sizeof(DWORD));	  

  		  RegCloseKey(hKey);
	}
 
enregistro--;

return 0;
}


int InitVolume(DWORD dwComponentType,volumen *pvolumen,int _Enabled,int NotMuted,DWORD subcomponentes)
{
	int bVolume = TRUE;
	int enabled = TRUE;
	HRESULT mmResult;
	MIXERLINE MixerLine;
	MIXERLINE MixerLine2;
	MIXERCONTROL Control;
	MIXERLINECONTROLS LineControls;


		memset( &MixerLine, 0, sizeof(MIXERLINE) );
		MixerLine.cbStruct = sizeof(MIXERLINE);
		MixerLine.dwComponentType = dwComponentType;
		mmResult = mixerGetLineInfo( (HMIXEROBJ)hmx, &MixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE );
		if ( mmResult != MMSYSERR_NOERROR )
		{
			bVolume = FALSE;
			return bVolume;
		}

		if (subcomponentes)
		{
			int i;			

			for (i = 0; i < MixerLine.cConnections;i++)
			{
				memset( &MixerLine2, 0, sizeof(MIXERLINE) );
				MixerLine2.cbStruct = sizeof(MIXERLINE);
				MixerLine2.dwDestination = MixerLine.dwDestination;
				MixerLine2.dwSource = i;
				mmResult = mixerGetLineInfo( ( HMIXEROBJ )hmx, &MixerLine2, MIXER_GETLINEINFOF_SOURCE );
				if ( mmResult != MMSYSERR_NOERROR )
				{
					bVolume = FALSE;
					return bVolume;
				}
				if (MixerLine2.dwComponentType == subcomponentes)
				{
					MIXERLINE MixerLine3;
					MixerLine3 = MixerLine;
					MixerLine = MixerLine2;
					MixerLine2 = MixerLine3;
					break;
				}
			}
		}

		memset( &Control, 0, sizeof(MIXERCONTROL) );
		Control.cbStruct = sizeof(MIXERCONTROL);					
		memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
		LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
		LineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		LineControls.dwLineID = MixerLine.dwLineID;		
		LineControls.cControls = 1;
		LineControls.cbmxctrl = sizeof(MIXERCONTROL);
		LineControls.pamxctrl = &Control;
		mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
		if ( mmResult == MMSYSERR_NOERROR )
		{
			if ( !(Control.fdwControl & MIXERCONTROL_CONTROLF_DISABLED) )
			{
				enabled = TRUE;
				/*
				m_bAvailable = true;
				TRACE(".MasterOutputVolume: \"%s\" Volume control for the Speakers Source Line adopted.\n", Control.szShortName );
				*/
			} else 
			{
				enabled = FALSE;
				/*
				TRACE(".MasterOutputVolume: WARNING: The Volume Control is disabled.\n" );
				*/
			}
		} else 
		{
			if (_Enabled)
			{
				EnableLine(MixerLine.dwLineID,TRUE,NotMuted);
			}
			bVolume = FALSE;
			return bVolume;
		}	

	pvolumen->dwControlMute = BAD_DWORD;

	if (bVolume)
	{
		if (_Enabled)
		{
			EnableLine(LineControls.dwLineID,TRUE,NotMuted);
		}		
		pvolumen->dwChannelCount = MixerLine.cChannels;		
		pvolumen->dwLineID = LineControls.dwLineID;
		pvolumen->dwControlID = Control.dwControlID;
		pvolumen->dwEnabled = (DWORD)enabled;
		pvolumen->dwMinimalVolume = Control.Bounds.dwMinimum;
		pvolumen->dwMaximalVolume = Control.Bounds.dwMaximum;
		pvolumen->dwVolumeStep = Control.Metrics.cSteps;
		
		memset( &Control, 0, sizeof(MIXERCONTROL) );
		Control.cbStruct = sizeof(MIXERCONTROL);					
		memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
		LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
		LineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
		LineControls.dwLineID = MixerLine.dwLineID;		
		LineControls.cControls = 1;
		LineControls.cbmxctrl = sizeof(MIXERCONTROL);
		LineControls.pamxctrl = &Control;
		mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );
		pvolumen->dwMute = 0;
		if ( mmResult == MMSYSERR_NOERROR )
		{
			pvolumen->dwControlMute = Control.dwControlID;			
			pvolumen->MutedwChannelCount = 1/*MixerLine.cChannels*/;
			pvolumen->MutecMultipleItems = 0;
			pvolumen->MutedwSource = 0;
		}
		else
		{
			if (!subcomponentes)
			{ 
				MixerLine2 = MixerLine;
			}
			memset( &Control, 0, sizeof(MIXERCONTROL) );
			Control.cbStruct = sizeof(MIXERCONTROL);					
			memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
			LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
			LineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUX; //SINGLESELECT
			LineControls.dwLineID = MixerLine2.dwLineID;		
			LineControls.cControls = 1;
			LineControls.cbmxctrl = sizeof(MIXERCONTROL);
			LineControls.pamxctrl = &Control;				

			mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE );				
			if ( mmResult == MMSYSERR_NOERROR && Control.cMultipleItems > 0)
			{
				pvolumen->dwControlMute = Control.dwControlID;
				pvolumen->MutecMultipleItems = Control.cMultipleItems;				
				pvolumen->MutedwChannelCount = 1/*MixerLine2.cChannels*/;
				pvolumen->MutedwSource = MixerLine.dwSource;
			}
			else
			{
				pvolumen->dwControlMute = BAD_DWORD;
			}
		}
		pvolumen->dwActualVolume = 0;
		GetCurrentVolumen(pvolumen);				
	}
	

	if (pvolumen->dwControlMute == BAD_DWORD && dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE)
	{	
		/*
		if (SelectLine(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,NULL))
		{
			pvolumen->dwControlMute = (DWORD)-2;
		}
		*/
	}

	return bVolume;
}


void EnableLine(DWORD dwLineID , int bEnable, int NotMuted)
{
	int bAnyEnabled = FALSE;
	MMRESULT mmResult;
	MIXERLINE lineDestination;
	MIXERLINECONTROLS LineControls;
	MIXERCONTROL* aControls;
	int nControlCount;
	int nChannelCount;
	int i;

	if ( !hmx )
		return;
	
	memset( &lineDestination, 0, sizeof(MIXERLINE) );
	lineDestination.cbStruct = sizeof(MIXERLINE);
	lineDestination.dwLineID = dwLineID;
	mmResult = mixerGetLineInfo( (HMIXEROBJ)hmx, &lineDestination, MIXER_GETLINEINFOF_LINEID );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the Speakers Destination Line while enabling. mmResult=%d\n", mmResult );
		} else {
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the Speakers Destination Line while disabling. mmResult=%d\n", mmResult );
		}
		return;
	}
	// Getting all line's controls
	nControlCount = lineDestination.cControls;
    nChannelCount = lineDestination.cChannels;
	
	memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
	aControls = (MIXERCONTROL*)malloc( nControlCount * sizeof(MIXERCONTROL) );
	if ( !aControls )
	{
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
		} else {
			//TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
		}
		return;
	}
	memset( &aControls[0], 0, sizeof(nControlCount * sizeof(MIXERCONTROL)) );
	for ( i = 0; i < nControlCount; i++ )
	{
		aControls[i].cbStruct = sizeof(MIXERCONTROL);
	}
	LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
	LineControls.dwLineID = lineDestination.dwLineID;
	LineControls.cControls = nControlCount;
	LineControls.cbmxctrl = sizeof(MIXERCONTROL);
	LineControls.pamxctrl = &aControls[0];
	mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ALL );
	if ( mmResult == MMSYSERR_NOERROR )
	{
		for ( i = 0; i < nControlCount; i++ )
		{
			LONG lValue;
			int bReadyToSet = FALSE;

			switch (aControls[i].dwControlType)
			{			
			case MIXERCONTROL_CONTROLTYPE_MUTE:
				lValue = (BOOL)!NotMuted;

				bReadyToSet = TRUE;
				break;			
			case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MUX:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MIXER:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_ONOFF:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			}

			if ( bReadyToSet )
			{
				if (aControls[i].cMultipleItems > 1 && (aControls[i].dwControlType & MIXERCONTROL_CONTROLTYPE_SINGLESELECT))
				{
					bReadyToSet = FALSE;
				}
			}
			if ( bReadyToSet )
			{
				int nItem;
				int nChannel;
				MIXERCONTROLDETAILS_BOOLEAN* aDetails = NULL;
				int nMultipleItems = aControls[i].cMultipleItems;
				int nChannels = nChannelCount;
				// MIXERCONTROLDETAILS
				MIXERCONTROLDETAILS ControlDetails;

				memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
				ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
				ControlDetails.dwControlID = aControls[i].dwControlID;
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_UNIFORM )
				{
					nChannels = 1;
				}
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
				{
					nMultipleItems = aControls[i].cMultipleItems;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nMultipleItems*nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						if ( bEnable )
						{
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
						} else {
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
						}
						continue;
					}
					for ( nItem = 0; nItem < nMultipleItems; nItem++ )
					{						
						/*
						if ( ( aControls[i].dwControlType & MIXERCONTROL_CONTROLTYPE_SINGLESELECT )
						  && ( nItem > 0 ) )
						{
							lValue = (LONG)!((BOOL)lValue);
						}
						*/
						for ( nChannel = 0; nChannel < nChannels; nChannel++ )
						{
							aDetails[nItem+nChannel].fValue = lValue;
						}
					}
				} else 
				{
					int nChannel;

					nMultipleItems = 0;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						if ( bEnable )
						{
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
						} else {
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
						}
						continue;
					}
					for ( nChannel = 0; nChannel < nChannels; nChannel++ )
					{
						aDetails[nChannel].fValue = (LONG)lValue;
					}
				}
				ControlDetails.cChannels = nChannels;
				ControlDetails.cMultipleItems = nMultipleItems;
				ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
				ControlDetails.paDetails = &aDetails[0];
				mmResult = mixerSetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, 0L );
				if ( mmResult == MMSYSERR_NOERROR )
				{
					if ( bEnable )
					{
						//TRACE(".MasterOutputVolume: Enabling Line: Speakers Line control \"%s\"(0x%X) has been set to %d.\n", aControls[i].szShortName, aControls[i].dwControlType, lValue );
					} else {
						//TRACE(".MasterOutputVolume: Disabling Line: Speakers Line control \"%s\"(0x%X) has been set to %d.\n", aControls[i].szShortName, aControls[i].dwControlType, lValue );
					}
					bAnyEnabled = TRUE;
				}
				free( aDetails );
			}
		}
	} else {
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the line controls while enabling. mmResult=%d\n", mmResult );
		} else {
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the line controls while disabling. mmResult=%d\n", mmResult );
		}
	}
	free( aControls );
	if ( !bAnyEnabled )
	{
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: WARNING: No controls were found for enabling the line.\n" );
		} else {
			//TRACE(".MasterOutputVolume: WARNING: No controls were found for disabling the line.\n" );
		}
	}
}

#ifdef BASURA
int SelectLine(DWORD dwComponentType, DWORD dwSelComponent,volumen *pvolumen)
{
	int bAnyEnabled = FALSE;
	MMRESULT mmResult;
	MIXERLINE lineDestination;
	MIXERLINECONTROLS LineControls;
	MIXERCONTROL* aControls;
	int nControlCount;
	int nChannelCount;
	int i;	
	DWORD bEnable = TRUE;

	if ( !hmx )
		return;

	if (pvolumen)
	{
		pvolumen->dwMaximalVolume = 0;
		pvolumen->dwMinimalVolume = 0;
	}
	
	memset( &lineDestination, 0, sizeof(MIXERLINE) );
	lineDestination.cbStruct = sizeof(MIXERLINE);
	lineDestination.dwComponentType = dwComponentType;
	mmResult = mixerGetLineInfo( (HMIXEROBJ)hmx, &lineDestination, MIXER_GETLINEINFOF_COMPONENTTYPE );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the Speakers Destination Line while enabling. mmResult=%d\n", mmResult );
		} else {
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the Speakers Destination Line while disabling. mmResult=%d\n", mmResult );
		}
		return;
	}
	// Getting all line's controls
	nControlCount = lineDestination.cControls;
    nChannelCount = lineDestination.cChannels;
	
	memset( &LineControls, 0, sizeof(MIXERLINECONTROLS) );
	aControls = (MIXERCONTROL*)malloc( nControlCount * sizeof(MIXERCONTROL) );
	if ( !aControls )
	{
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
		} else {
			//TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
		}
		return;
	}
	memset( &aControls[0], 0, sizeof(nControlCount * sizeof(MIXERCONTROL)) );
	for ( i = 0; i < nControlCount; i++ )
	{
		aControls[i].cbStruct = sizeof(MIXERCONTROL);
	}
	LineControls.cbStruct = sizeof(MIXERLINECONTROLS);
	LineControls.dwLineID = lineDestination.dwLineID;
	LineControls.cControls = nControlCount;
	LineControls.cbmxctrl = sizeof(MIXERCONTROL);
	LineControls.pamxctrl = &aControls[0];
	mmResult = mixerGetLineControls( (HMIXEROBJ)hmx, &LineControls, MIXER_GETLINECONTROLSF_ALL );
	if ( mmResult == MMSYSERR_NOERROR )
	{
		for ( i = 0; i < nControlCount; i++ )
		{
			LONG lValue;
			int bReadyToSet = FALSE;

			switch (aControls[i].dwControlType)
			{			
			case MIXERCONTROL_CONTROLTYPE_MUX:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_MIXER:
				lValue = (BOOL)bEnable;
				bReadyToSet = TRUE;
				break;
			case MIXERCONTROL_CONTROLTYPE_VOLUME:
				break;
			}
			if ( bReadyToSet )
			{
				int nItem;
				int nChannel;
				int cualItem = -1;
				MIXERCONTROLDETAILS_BOOLEAN* aDetails = NULL;
				
				int nMultipleItems = aControls[i].cMultipleItems;
				int nChannels = nChannelCount;
				// MIXERCONTROLDETAILS
				MIXERCONTROLDETAILS ControlDetails;				

				memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
				ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
				ControlDetails.dwControlID = aControls[i].dwControlID;
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_UNIFORM )
				{
					nChannels = 1;
				}
				if ( aControls[i].fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
				{
					nMultipleItems = aControls[i].cMultipleItems;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nMultipleItems*nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));

                    if ( nMultipleItems > 1 && (aControls[i].dwControlType & MIXERCONTROL_CONTROLTYPE_SINGLESELECT))
					{
						cualItem = BuscaLinea(ControlDetails.dwControlID,nMultipleItems,dwSelComponent,pvolumen,nChannelCount);
						lValue = !lValue;
					}
					else
						cualItem = -1;
					if ( !aDetails )
					{
						if ( bEnable )
						{
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
						} else {
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
						}
						continue;
					}
					for ( nItem = 0; nItem < nMultipleItems; nItem++ )
					{
						for ( nChannel = 0; nChannel < nChannels; nChannel++ )
						{
							if (nItem == cualItem)
							{
								aDetails[nItem+nChannel].fValue = !lValue;
							}
							else
							{
								aDetails[nItem+nChannel].fValue = lValue;
							}
						}
					}
				} else 
				{
					int nChannel;

					nMultipleItems = 0;
					aDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(nChannels*sizeof(MIXERCONTROLDETAILS_BOOLEAN));
					if ( !aDetails )
					{
						if ( bEnable )
						{
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while enabling the line.\n" );
						} else {
							//TRACE(".MasterOutputVolume: FAILURE: Out of memory while disabling the line.\n" );
						}
						continue;
					}
					for ( nChannel = 0; nChannel < nChannels; nChannel++ )
					{
						aDetails[nChannel].fValue = (LONG)lValue;
					}
				}
				ControlDetails.cChannels = nChannels;
				ControlDetails.cMultipleItems = nMultipleItems;
				ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
				ControlDetails.paDetails = &aDetails[0];
				mmResult = mixerSetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, 0L );
				if ( mmResult == MMSYSERR_NOERROR )
				{
					if ( bEnable )
					{
						//TRACE(".MasterOutputVolume: Enabling Line: Speakers Line control \"%s\"(0x%X) has been set to %d.\n", aControls[i].szShortName, aControls[i].dwControlType, lValue );
					} else {
						//TRACE(".MasterOutputVolume: Disabling Line: Speakers Line control \"%s\"(0x%X) has been set to %d.\n", aControls[i].szShortName, aControls[i].dwControlType, lValue );
					}
					bAnyEnabled = TRUE;
				}
				free( aDetails );
			}
		}
	} else {
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the line controls while enabling. mmResult=%d\n", mmResult );
		} else {
			//TRACE(".MasterOutputVolume: FAILURE: Could not get the line controls while disabling. mmResult=%d\n", mmResult );
		}
	}
	free( aControls );
	if ( !bAnyEnabled )
	{
		if ( bEnable )
		{
			//TRACE(".MasterOutputVolume: WARNING: No controls were found for enabling the line.\n" );
		} else {
			//TRACE(".MasterOutputVolume: WARNING: No controls were found for disabling the line.\n" );
		}
	}
	if (pvolumen && pvolumen->dwMaximalVolume != pvolumen->dwMinimalVolume)
	{
		
	}

	return bAnyEnabled;
}
#endif


DWORD GetCurrentVolumen(volumen *pvolumen)
{
	DWORD dw,dw2;
	MMRESULT mmResult;
	MIXERCONTROLDETAILS_UNSIGNED* aDetails;
	MIXERCONTROLDETAILS_BOOLEAN* abDetails;
	MIXERCONTROLDETAILS ControlDetails;	

	if ( !hmx )
		return BAD_DWORD;
	aDetails = (MIXERCONTROLDETAILS_UNSIGNED*)malloc(pvolumen->dwChannelCount*sizeof(MIXERCONTROLDETAILS_UNSIGNED));

	if ( !aDetails )
		return BAD_DWORD;
	
	memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
	ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	ControlDetails.dwControlID = pvolumen->dwControlID;
	ControlDetails.cChannels = pvolumen->dwChannelCount;
	ControlDetails.cMultipleItems = 0;
	ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	ControlDetails.paDetails = &aDetails[0];
	mmResult = mixerGetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, MIXER_GETCONTROLDETAILSF_VALUE );
	dw = aDetails[0].dwValue;
	free( aDetails );

	if ( mmResult != MMSYSERR_NOERROR )
	{
		//TRACE(".MasterOutputVolume: FAILURE: Could not get volume. mmResult=%d\n", mmResult );		
		return BAD_DWORD;
	}
	pvolumen->dwActualVolume = dw;

	if (pvolumen->dwControlMute != BAD_DWORD)
	{
		int nitems = pvolumen->MutecMultipleItems;
		if (nitems < 1)
			nitems = 1;

		abDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(pvolumen->MutedwChannelCount*sizeof(MIXERCONTROLDETAILS_BOOLEAN)*nitems);

		memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
		ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		ControlDetails.dwControlID = pvolumen->dwControlMute;
		ControlDetails.cChannels = pvolumen->MutedwChannelCount;
		ControlDetails.cMultipleItems = pvolumen->MutecMultipleItems;
		ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		ControlDetails.paDetails = &abDetails[0];
		mmResult = mixerGetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, MIXER_GETCONTROLDETAILSF_VALUE  );
		dw2 = (DWORD)abDetails[pvolumen->MutedwSource].fValue;
		if (nitems > 1)
			dw2 = !dw2;
		free( abDetails );
		if ( mmResult != MMSYSERR_NOERROR )
		{			
			//TRACE(".MasterOutputVolume: FAILURE: Could not get volume. mmResult=%d\n", mmResult );
			return BAD_DWORD;
		}
		pvolumen->dwMute = dw2;
	}		

	return dw;
}
//////////////
void SetCurrentVolumen( volumen *pvolumen, DWORD dwValue ,DWORD dwMute )
{
	MMRESULT mmResult;
	MIXERCONTROLDETAILS_UNSIGNED* aDetails;
	MIXERCONTROLDETAILS_BOOLEAN* abDetails;
	MIXERCONTROLDETAILS ControlDetails;
	DWORD i;
	if ( !hmx  || (dwValue<pvolumen->dwMinimalVolume) || (dwValue>pvolumen->dwMaximalVolume) )
		return;
	aDetails = (MIXERCONTROLDETAILS_UNSIGNED*)malloc(pvolumen->dwChannelCount*sizeof(MIXERCONTROLDETAILS_UNSIGNED));
	if ( !aDetails )
		return;
	
	memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
	ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	ControlDetails.dwControlID = pvolumen->dwControlID;
	ControlDetails.cChannels = pvolumen->dwChannelCount;
	ControlDetails.cMultipleItems = 0;
	ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	ControlDetails.paDetails = &aDetails[0];
	mmResult = mixerGetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, MIXER_GETCONTROLDETAILSF_VALUE );	
	for ( i = 0; i < pvolumen->dwChannelCount; i++ )
	{
		aDetails[i].dwValue = dwValue;
	}
	mmResult = mixerSetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, MIXER_SETCONTROLDETAILSF_VALUE );	
	free( aDetails );
	if ( mmResult != MMSYSERR_NOERROR )
	{
		//TRACE(".MasterOutputVolume: FAILURE: Could not set volume(%d) mmResult=%d\n", dwValue, mmResult );
	}
	else
	{
		// pvolumen->dwActualVolume = dwValue;
	}

	/*EnableLine(pvolumen->dwLineID , TRUE, !dwMute);*/	
	
	if (pvolumen->dwControlMute != BAD_DWORD)
	{
		int nitems = pvolumen->MutecMultipleItems;
		if (nitems < 1)
			nitems = 1;
		abDetails = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(pvolumen->MutedwChannelCount*sizeof(MIXERCONTROLDETAILS_BOOLEAN)*nitems);

		memset( &ControlDetails, 0, sizeof(MIXERCONTROLDETAILS) );
		ControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		ControlDetails.dwControlID = pvolumen->dwControlMute;
		ControlDetails.cChannels = pvolumen->MutedwChannelCount;
		ControlDetails.cMultipleItems = pvolumen->MutecMultipleItems;
		ControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		ControlDetails.paDetails = &abDetails[0];
		mmResult = mixerGetControlDetails( (HMIXEROBJ)hmx, &ControlDetails, MIXER_GETCONTROLDETAILSF_VALUE  );
		if ( mmResult != MMSYSERR_NOERROR )
		{
			switch(mmResult)
			{
			case MIXERR_INVALCONTROL:
				i = 0;
				break;
			case MMSYSERR_BADDEVICEID:
				i = 1;
				break;
			case MMSYSERR_INVALFLAG:
				i = 2;
				break;
			case MMSYSERR_INVALHANDLE:
				i = 3;
				break;
			case MMSYSERR_INVALPARAM:
				i = 4;
				break;
			case MMSYSERR_NODRIVER:
				i = 5;
				break;
			default:
				i = -1;
			}
		}	
		if (nitems > 1)
			dwMute = !dwMute;
		
		for ( i = 0; i < /*pvolumen->MutedwChannelCount**/nitems; i++ )
		{
			if (i != pvolumen->MutedwSource)
				abDetails[i].fValue = (LONG)!dwMute;
			else
				abDetails[i].fValue = (LONG)dwMute;
		}	
		mmResult = mixerSetControlDetails( (HMIXEROBJ)hmx, &ControlDetails,MIXER_SETCONTROLDETAILSF_VALUE);
		free( abDetails );
		if ( mmResult != MMSYSERR_NOERROR )
		{
			i = i;
		}
	}	
	else
	{		
		if (pvolumen->dwControlMute == (DWORD)-2)
		{
		    //SelectLine(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE,NULL);
		}		
	}

	GetCurrentVolumen(pvolumen);	
}
