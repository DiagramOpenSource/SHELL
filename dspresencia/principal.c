

#include "netfone.h"


#ifndef IPTOS_THROUGHPUT
#define IPTOS_THROUGHPUT 0x08
#endif
#ifndef IPTOS_LOWDELAY
#define IPTOS_LOWDELAY 0x10
#endif


char szVoxHost[51] = "";

int NETFONE_COMMAND_PORT = DEFAULT_NETFONE_COMMAND_PORT;

int bTieneAudio = FALSE;

HMIXER    hmx = NULL;
volumen    VolSpeaker;
volumen    VolWaveOut;
volumen    VolWaveIn;
volumen    VolMicrophone;
volumen    *Volumes[5] = {NULL,NULL,NULL,NULL,NULL};
HWND hwndVolumen = NULL;
/*volumen    *Volumes[5] = {&VolSpeaker,&VolWaveOut,&VolWaveIn,&VolMicrophone,NULL};*/

HWAVEOUT hWaveOut = NULL;				// Wave output handle
HWAVEIN  hWaveIn = NULL;                 // Wave input handle

static int listeners = 0;							// Current wantsInput windows
static int outputTimeout = 0;			// Output release timeout

int UsarElCtrl = 0;
int NoEnviarAudio = 0;

int nvoces_activas = 0;
long voces_activas[MAXIPS];
int netMaxSamples = 0;						// Maximum samples network can send

int isWaveSound = FALSE;				// Input from wave file ?
int outputActive = FALSE;				// Is wave output open ?
int inputActive = FALSE;				// Is wave input open ?
int inputPaused = FALSE;				// Is input paused for output ?
int openConnections = 0;				// Number of open connections

int audioChannels = 1;			// Audio input channels

int envia_debug_info = 1;

static long ultimaipactiva = 0;

/*
int samplesPerSecond = 11025;		// Sound sampling rate
int bytesPerSecond = 22050;		// Sample bytes per second
int sampleAlignment = 2;			// Sample frame size
int bitsPerSample = 16;			// Bits per sample
*/
int samplesPerSecond = 8000;		// Sound sampling rate
int bytesPerSecond = 16000;		// Sample bytes per second
int sampleAlignment = 2;			// Sample frame size
int bitsPerSample = 16;			// Bits per sample

int outputInShutdown = FALSE;			// Close output when last buffer returned

int spurt = FALSE;						// Start of talk spurt flag	

int halfDuplex = FALSE;					// Audio hardware is half-duplex
int halfDuplexTransition = FALSE;		// Transitioning from output to input ?
static int audioIs8Bit = FALSE;			// Nonzero if audio is 8 bit only

int alwaysBindSocket = TRUE;			// Bind output socket (WINSOCK bug work-around)

int useSendNotSendto = FALSE;			// Use send() to socket, not sendto()
int waNetNoConnect = TRUE;				// Don't connect(), use sendto()
int waNetUseSend = FALSE;				// Use send(), not sendto() always
int waNetMultiTTLisChar = FALSE;		// Argument to IP_MULTICAST_TTL setsockopt is char
int waNetNoOutOverflow = FALSE;			// Disable output overflow detection and recovery
int waNetSynchronousGetHostname = 2;	// Use gethostbyaddr(), not WASAsync variant
int waNetSynchronousGetHostnameAction = TRUE; // Action set by above variable


static int currentInputLength;			// Current expected input buffer length
int currentInputSamples;				// Samples desired in current input buffers

gsm gsmh = NULL;						// GSM handle

unsigned long ssrc;			  			// RTP synchronisation source identifier
unsigned long timestamp; 	  			// RTP packet timestamp
unsigned short seq;			  			// RTP packet sequence number


#define nWaveHeaders	4
SOCKET sCommand = INVALID_SOCKET;	// Command socket
SOCKET sEnvio = INVALID_SOCKET;	// 
//static SOCKET lwlsock = INVALID_SOCKET;	// Look Who's Listening periodic update socket
struct auxSocket FAR *asList = NULL;	// Auxiliary socket list
static LPWAVEHDR inWaveHeader[nWaveHeaders];// Pointers to wave input buffers
static int waveHeadersAllocated = 0;	// Number of allocated wave headers
static int createSuccessful = FALSE;	// Set once we've loaded .INI parameters
static int inputTerm = FALSE;			// Input terminating
/*static soundbuf receivedSoundBuffer;	// Sound buffer from network*/

static UINT waAudioInDevice = (UINT) WAVE_MAPPER;	// Default audio input device
static UINT waAudioOutDevice = (UINT) WAVE_MAPPER;	// Default audio output device

static UINT jitterBuf = 50;//1000;			// Milliseconds to anti-jitter output
BOOL jitterPause = FALSE;				// Queueing packets for anti-jitter ?
static UINT jitterTimer = 0;			// Jitter timer
#define JITTER_TIMER_ID	6				// Timer ID for anti-jitter

long packetsReceived = 0,				// Network packet traffic counters
	 packetsSent = 0,
	 inputPacketsLost = 0,				// Input packets lost due to half-duplex
	 outputPacketsLost = 0,				// Output packets lost due to net traffic
	 messageChecks = 0;					// Anti-lockup calls on DefaultMessageLoop
	 
int messageQueueSize = 600;				// Inbound message queue size

int waNetNoMsgLoopIns = TRUE;			// Disable message loop insurance

//	Network properties

int aboutUDPmax = 0;					// Maximum UDP packet size

long outputPending = 0;                 // Output buffers in queue

int voxmode = IDM_VOX_SLOW;				// VOX mode
int breakinput = FALSE;					// break input audio stream for output
int gsmcompress = TRUE;					// GSM compression mode
int gsm723 = FALSE;	  						    
int lpc10compress = FALSE;				// LPC-10 compression mode
int robust = 1;							// Robust mode for LPC-10
int protocolXmit = PROTOCOL_SPEAKFREE;	// Default protocol to send (!protocolAuto or new connection)
int protocolSent = PROTOCOL_SPEAKFREE;	// Protocol we're currently sending 
int protocolAuto = TRUE;				// Send same protocol as received from connection

int waProtNoHeartbeat = TRUE;			// No heartbeat in Speak Freely protocol

INT	tmAveCharWidth;						// TEXTMETRIC.tmAveCharWidth
INT	tmHeight;							// TEXTMETRIC.tmHeight

u_long Lazarus = 0;						// Lazarus host ID
int LazarusLong = 0;					// If nonzero, anti-reincarnation timeout

#define DESIRED_WINSOCK_VERSION 0x0101  // we'd like winsock ver 1.1...
#define MINIMUM_WINSOCK_VERSION 0x0001  // ...but we'll take ver 1.0

//  REVSHORT  --  Reverse bytes in a short

void revshort(short *s)
{
	short s1 = *s;
	LPSTR ip = (LPSTR) &s1, op = (LPSTR) s;
	
	op[0] = ip[1];
	op[1] = ip[0];
}

//  REVLONG  --  Reverse bytes in a long

void revlong(long *l)
{
	long l1 = *l;
	LPSTR ip = (LPSTR) &l1, op = (LPSTR) l;
	
	op[0] = ip[3];
	op[1] = ip[2];
	op[2] = ip[1];
	op[3] = ip[0]; 
}

void sessionKeyGenerate(LPSTR key, BOOL binary)
{
	int j, k;
	char s[256];
    struct MD5Context md5c;
    char md5key[16], md5key1[16];
    POINT p;
	MEMORYSTATUS ms;
	
	/*	The following gets all kind of information likely
		to vary from moment to moment and uses it as the initial
		seed for the random number generator.  If any of these
		causes porting problems in the future, just delete them.  */
	
    wsprintf(s, "%lu" , GetTickCount());
    wsprintf(s + strlen(s), "%lu", time(NULL));
    gethostname(s + strlen(s), 256);
    wsprintf(s + strlen(s), "%u", GetActiveWindow());
    wsprintf(s + strlen(s), "%lu", GetFreeSpace(0));
	ms.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&ms);
	wsprintf(s + strlen(s), "%lu", ms.dwMemoryLoad); 
	wsprintf(s + strlen(s), "%lu", ms.dwAvailPhys); 
	wsprintf(s + strlen(s), "%lu", ms.dwAvailPageFile); 
    GetCursorPos(&p);
    wsprintf(s + strlen(s), "%u%u", p.x, p.y); 
    MD5Init(&md5c);
    MD5Update(&md5c, s, strlen(s));
    MD5Final(md5key, &md5c);
    wsprintf(s + strlen(s), "%lu", (time(NULL) + 65121) ^ 0x375F);
    MD5Init(&md5c);
    MD5Update(&md5c, s, strlen(s));
    MD5Final(md5key1, &md5c);
    for (j = k = 0; j < 16; j++) {
		unsigned char rb = (unsigned char) (md5key[j] ^ md5key1[j]);               
        if (binary) {
        	key[j] = (char) rb;
        } else {
#define Rad16(x) ((x) + 'A')
	        key[k++] = Rad16((rb >> 4) & 0xF);
	        key[k++] = Rad16(rb & 0xF);
	        if (j & 1) {
	            key[k++] = '-';
	        }
        }
    }
    if (!binary) {
    	key[--k] = 0;
    }
}

SOCKERR ResetSocket(SOCKET sock)
{
    LINGER linger;

    if (sock == INVALID_SOCKET || sock == sCommand || sock == sEnvio) 
	{
        return 0;
    }

    /*  Enable linger with a timeout of zero.  This will
		force the hard close when we call closesocket().

 		We ignore the error return from setsockopt.  If it
		fails, we'll just try to close the socket anyway. */

    linger.l_onoff  = TRUE;
    linger.l_linger = 0;

    setsockopt(sock, SOL_SOCKET, SO_LINGER, (CHAR *) &linger, sizeof(linger));

    return closesocket(sock);
}

//	CREATESOCKET  --  Create a new socket
static void AjustesSocket(SOCKET sock)
{
	int i;

	i = -1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const char *)&i,sizeof(i));

	i = 1;
	setsockopt(sock,SOL_SOCKET,SO_OOBINLINE,(const char *)&i,sizeof(i));

	i = IPTOS_LOWDELAY;
	setsockopt(sock,IPPROTO_IP,IP_TOS,(const char *)&i,sizeof(i));

	i = -1;
	setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(const char *)&i,sizeof(i));	
}


SOCKERR CreateSocket(SOCKET *psock, INT type, ULONG address, PORT port)
{
    SOCKET  sNew;
    SOCKERR serr;


    //  Create the socket

    sNew = socket(PF_INET, type, 0);
    serr = (sNew == INVALID_SOCKET) ? WSAGetLastError(): 0;

	if (serr == 0)
	{
		AjustesSocket(sNew);
	}
    if ((serr == 0)
			&& (alwaysBindSocket || (port != 0))
        ) {
        SOCKADDR_IN sockAddr;

        //  Bind an address to the socket

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_addr.s_addr = address;
        sockAddr.sin_port = port;

		/*
		{
			char tmp[512];
			sprintf(tmp,"%lX %d",address,port);
			MessageBox(NULL,tmp ,	NULL, MB_OK | MB_ICONEXCLAMATION);
		}
		*/

        if (bind(sNew, (SOCKADDR *) &sockAddr, sizeof(sockAddr)) != 0) 
		{
            serr = WSAGetLastError();
			{
				char tmp[512];
				sprintf(tmp,"%d",serr);
        	    MessageBox(NULL,tmp ,	NULL, MB_OK | MB_ICONEXCLAMATION);
			}
        }
    }

    if (serr != 0) 
	{
        ResetSocket(sNew);
        sNew = INVALID_SOCKET;
    }
	else
	{
		/*
		i = IPTOS_LOWDELAY;
		setsockopt(sNew,IPPROTO_IP,IP_TOS,(const char *)&i,sizeof(i));
		*/
	}


    *psock = sNew;
    return serr;
}


int inputSampleCount(void)
{
	int l = 400;

	switch (protocolSent) 
	{
		case PROTOCOL_SPEAKFREE:	
			l = lpc10compress ? ( 1800 ) :					 
						((gsmcompress) ?
							((gsm723) ? /*1600*/800 : /*1600*/400) : ((512 - (sizeof(soundbuf) - BUFL)) ));
			break;			
	}
	return l;
}						  													 

static int inputBufferLength(void)
{
	return (bitsPerSample / 8) * inputSampleCount();
}						  													 

int startWaveInput(HWND hwnd)
{
    //	Attempt to initialise the audio input port    

	if (NoEnviarAudio)
		return FALSE;

	if (inputTerm) 
	{
		MSG msg;
		int i;
		for (i = 0;i < 5 && inputActive;i++)
		{
			while(PeekMessage(&msg,hwndMDIFrame,MM_WIM_DATA,MM_WIM_DATA,PM_REMOVE))
			{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
			}
			if (inputActive && i < 5)
				Sleep(100);
		}
		if (inputActive)
		{
			inputActive = FALSE;
		}
		inputTerm = FALSE;		
	}

    if (!inputActive) 
	{
    	int i;
	    PCMWAVEFORMAT wfi;    
    	MMRESULT woo;
    	
    	wfi.wf.wFormatTag = WAVE_FORMAT_PCM;
    	wfi.wf.nChannels = audioChannels;
    	
    	while (TRUE) 
		{
	    	wfi.wf.nSamplesPerSec = samplesPerSecond;
	    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
	    	wfi.wf.nBlockAlign = sampleAlignment;
	    	wfi.wBitsPerSample = bitsPerSample;
	 		woo = waveInOpen(&hWaveIn, waAudioInDevice, (LPWAVEFORMATxx) &wfi,
	 				0L, 0L, WAVE_FORMAT_QUERY);
	 		if (bitsPerSample > 8 && woo == WAVERR_BADFORMAT) 
			{
	 			audioIs8Bit = TRUE;
	 		} 
	 				
	 		/* If our preferred mode (16 bit) isn't supported, try falling
	 		   back to bottom-feeder 8 bit per sample mode. */			
	
	 		if ((woo == WAVERR_BADFORMAT) && (bitsPerSample > 8)) 
			{
	 			bitsPerSample /= 2;
	 			sampleAlignment /= 2;
	 			bytesPerSecond /= 2;	
		    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
		    	wfi.wf.nBlockAlign = sampleAlignment;
		    	wfi.wBitsPerSample = bitsPerSample;
		 		woo = waveInOpen(&hWaveIn, waAudioInDevice, (LPWAVEFORMATxx) &wfi,
		 				0L, 0L, WAVE_FORMAT_QUERY);
	 		}
	 		
	 		/* If we've failed to initialise in either 16 or 8 bit mode
	 		   at 8000 samples per second, it's possible the sound card
	 		   doesn't support any sampling mode below the Windows standard
	 		   of 11025 samples per second.  Have another go-round and see
	 		   if 11025 works. */
	 		
	 		if (woo == WAVERR_BADFORMAT && samplesPerSecond == 8000) 
			{
	 			samplesPerSecond = 11025;
	 			bitsPerSample = 16;
	 			sampleAlignment = bitsPerSample / 8;
	 			bytesPerSecond = samplesPerSecond * sampleAlignment;  
	 		} else 
			{
	 			break;
	 		}
	 	} 
		if (woo != 0) 
		{
			/*
        	MessageBox(hwnd, rstring(IDS_T_WAVE_RECORD_FORMAT_ERR),
		   		NULL, MB_OK | MB_ICONEXCLAMATION);
			*/
			return FALSE;
    	}
    	for (i = 0; i < 2; i++) 
		{
		    if ((woo = waveInOpen(&hWaveIn, waAudioInDevice,
				  (LPWAVEFORMATxx) &wfi, (DWORD) (UINT) hwndMDIFrame, 0L, (DWORD) CALLBACK_WINDOW)) != 0) {
		    	char et[MAXERRORLENGTH];
			    
	    	
		    	/* The next line looks wrong, doesn't it?  But I've seen drivers
		    	   for half-duplex sound boards that return NOTSUPPORTED instead
		    	   of ALLOCATED when you try to open input and output at the
		    	   same time. */
		    
			    if (i == 0 && outputActive && (woo == MMSYSERR_ALLOCATED || 
			    		woo == MMSYSERR_NOTSUPPORTED)) {
			    
			    	/* Okay, the preponderance of evidence points at our
			    	   machine being burdened with a half-duplex audio
			    	   device--one where the fact that we're playing audio
			    	   prevents from from simultaneously receiving it.
			    	   This is bad.  Let's proceed as we'd have done on
			    	   the 80 metre band in 1935--mute the receiver and
			    	   blast on our carrier regardless. */
			    	
			    	V waveOutReset(hWaveOut);
			    	halfDuplex = TRUE;			// Indicate wave output swiped
					if (outputPending) 
					{
						halfDuplexTransition = TRUE;
	    				//propUpdateAudio();
						return TRUE;
					} else {
						V waveOutClose(hWaveOut);
						outputActive = FALSE;
	    				//propUpdateAudio();
					}
			    	continue;   
			    }	
		    	waveInGetErrorText(woo, et, sizeof et);
				/*
	        	MessageBox(hwnd, et,
			   		rstring(IDS_T_ERR_OPEN_WAVE_INPUT), MB_OK | MB_ICONEXCLAMATION);
				*/
				return FALSE;
		    }
		    break;
	    }
	    
	    /*	Now allocate and prepare the sound input buffers.  Don't
	    	you just love the code vomit Windows' inability to free
	    	resources when a program terminates creates?  */
	    
	    currentInputLength = inputBufferLength();
	    currentInputSamples = inputSampleCount();
	    for (i = 0; i < nWaveHeaders; i++) 
		{
	    	inWaveHeader[i] = NULL;
	    }
	    for (i = 0; i < nWaveHeaders; i++) 
		{
			inWaveHeader[i] = (LPWAVEHDR) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,
												sizeof(WAVEHDR));
			if (inWaveHeader[i] == NULL) 
			{
				int j;
				
				for (j = i - 1; j >= 0; j++) 
				{
					waveInUnprepareHeader(hWaveIn, inWaveHeader[j], sizeof(WAVEHDR));
					GlobalFreePtr(inWaveHeader[j]->lpData);
					GlobalFreePtr(inWaveHeader[j]);
					inWaveHeader[j] = NULL;
				}
				/*
	        	MessageBox(hwnd, rstring(IDS_T_INPUT_HEADER_ERR),
			   		NULL, MB_OK | MB_ICONEXCLAMATION);
				*/
			   	waveInClose(hWaveIn);
				return FALSE;
			}
			
			/*	Since the user is allowed to change the sample size
				and compression modes on the fly, but the audio input
				buffers are used for the entire run of the program, we
				allocate them for the worst-case scenario: both LPC and
				2X compression enabled with 16 bit samples.  We've already
				established whether the hardware can run at 8000 samples
				per second, so only allocate the larger buffer needed for
				11025 sample per second hardware if it's actually required. */
				
			inWaveHeader[i]->lpData = (LPSTR) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,
				(DWORD) ((samplesPerSecond == 8000) ? 7200 : 10000));
			if (inWaveHeader[i]->lpData == NULL) 
			{
				int j;
				
				GlobalFreePtr(inWaveHeader[i]);
				inWaveHeader[i] = 0;
				for (j = i - 1; j >= 0; j++) 
				{
					waveInUnprepareHeader(hWaveIn, inWaveHeader[j], sizeof(WAVEHDR));
					GlobalFreePtr(inWaveHeader[j]->lpData);
					GlobalFreePtr(inWaveHeader[j]);
					inWaveHeader[j] = NULL;
				}

				/*
	        	MessageBox(hwnd, rstring(IDS_T_INPUT_BUFFER_ERR),
			   		NULL, MB_OK | MB_ICONEXCLAMATION);
				*/
			   	waveInClose(hWaveIn);
				return FALSE;
			}
			
			inWaveHeader[i]->dwBufferLength = currentInputLength;
			inWaveHeader[i]->dwFlags = 0;
			waveInPrepareHeader(hWaveIn, inWaveHeader[i], sizeof(WAVEHDR));
	    }
	    waveHeadersAllocated = nWaveHeaders; 
	    
		for (i = 0; i < nWaveHeaders; i++) 
		{
			waveInAddBuffer(hWaveIn, inWaveHeader[i], sizeof(WAVEHDR)); 
		}		
	    waveInStart(hWaveIn);
		inputTerm = FALSE;
	    inputActive = TRUE;
	    //aboutInSamples = samplesPerSecond;
	    //aboutInBits = bitsPerSample;
	    //propUpdateAudio();
    }
    return TRUE;
}
                                      
/*  TERMINATEWAVEINPUT  --  Shut down wave input and release all
							resources associated with it.  */
							
void terminateWaveInput(void)
{
	if (inputActive && !inputTerm) 
	{
		int i;
		
		inputTerm = TRUE;
		waveInReset(hWaveIn);
		for (i = 0; i < nWaveHeaders; i++) 
		{
			inWaveHeader[i] = NULL;
		}
	}
}

int EsAceptada(LPCLIENT_DATA pClientData,long ip,int seq)
{
	int i;
	if (!pClientData)
		return FALSE;


	if (pClientData->naceptadas > MAXIPS || pClientData->naceptadas < 0)
	{
		return FALSE;
	}

	for (i = 0;i < pClientData->naceptadas;i++)
	{
		if (pClientData->ipaceptada[i] == ip)
		{
			if (pClientData->rseq[i] == -1)
			{
				pClientData->rseq[i] = seq;
			}
			else
			{
				if (pClientData->rseq[i] == seq)
					return FALSE;
				pClientData->rseq[i] = seq;
			}
			return TRUE;
		}
	}
	return FALSE;
}

void TerminaConexion(HWND hWnd,BOOL avisar,long ip)
{
	LPCLIENT_DATA pClientData = CLIENTPTR(hWnd);	
	int i,j;	
	
	GrabaVolumenes(Volumes,4);

	if (!pClientData)
	{
		nvoces_activas = 0;
		return;
	}
	
	if (avisar)
	{		
		if (ip)
		{
			SendUdpControl(hWnd,pClientData,fEnd,"",1,ip);
		}
		else
		{
			for (i = 0;i < pClientData->naceptadas;i++)
			{
				SendUdpControl(hWnd,pClientData,fEnd,"",1,pClientData->ipaceptada[i]);
			}
		}
		//Sleep(100);
	}

	/*PostMessage(hDlgOperacion,WM_COMMAND,ID_Terminar,(LPARAM)ip);*/

	if (pClientData->naceptadas > MAXIPS || pClientData->naceptadas < 0)
	{
		return;
	}
	
	if (ip)
	{
		for (i = 0;i < pClientData->naceptadas;i++)
		{
			if (pClientData->ipaceptada[i] == ip)
			{
				pClientData->naceptadas--;
				for (j = i;j < pClientData->naceptadas;j++)
				{
					pClientData->ipaceptada[j] = pClientData->ipaceptada[j+1];
					pClientData->portdestino[j] = pClientData->portdestino[j+1];
					pClientData->rseq[j] = pClientData->rseq[j+1];
					pClientData->sseq[j] = pClientData->sseq[j+1];
					pClientData->activa[j] = pClientData->activa[j+1];
				}
				break;
			}
		}
	}
	else
	{
		pClientData->naceptadas = 0;
	}

	nvoces_activas = pClientData->naceptadas;
	{
		int j;
		for (j = 0;j < nvoces_activas;j++)
		{
			voces_activas[j] = CodigoPorIp(pClientData->ipaceptada[j]);
		}
	}

	if (pClientData->naceptadas)
	{
		return;
	}

	terminateWaveInput();
	listeners = 0;
    gsm_destroy(pClientData->gsmh);
	/*
	if (pClientData->sReply != INVALID_SOCKET) 
	{
		ResetSocket(pClientData->sReply);
		pClientData->sReply = INVALID_SOCKET;
	}
	*/	
	if (pClientData->getNameTask != NULL) 
	{
	    WSACancelAsyncRequest(pClientData->getNameTask);
	    pClientData->getNameTask = NULL;	
	}	

    GlobalFreePtr(pClientData);
    SetWindowLong(hWnd, GWL_CLIENT, 0L);
	openConnections--;
}

static int isHalfDuplex(HWND hwnd)
{
    PCMWAVEFORMAT wfi;
	MMRESULT woo;
	int hdx = FALSE;

	bTieneAudio = TRUE;
    	
	wfi.wf.wFormatTag = WAVE_FORMAT_PCM;
	wfi.wf.nChannels = audioChannels;
	
	//	Input initialisation
    	
	while (TRUE) 
	{
    	wfi.wf.nSamplesPerSec = samplesPerSecond;
    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
    	wfi.wf.nBlockAlign = sampleAlignment;
    	wfi.wBitsPerSample = bitsPerSample;
 		woo = waveInOpen(&hWaveIn, waAudioInDevice, (LPWAVEFORMATxx) &wfi,
 				0L, 0L, WAVE_FORMAT_QUERY);
 		if (bitsPerSample > 8 && woo == WAVERR_BADFORMAT) {
 			audioIs8Bit = TRUE;
 		} 
	 				
 		/* If our preferred mode (16 bit) isn't supported, try falling
 		   back to bottom-feeder 8 bit per sample mode. */			
	
 		if ((woo == WAVERR_BADFORMAT) && (bitsPerSample > 8)) 
		{
 			bitsPerSample /= 2;
 			sampleAlignment /= 2;
 			bytesPerSecond /= 2;	
	    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
	    	wfi.wf.nBlockAlign = sampleAlignment;
	    	wfi.wBitsPerSample = bitsPerSample;
	 		woo = waveInOpen(&hWaveIn, waAudioInDevice, (LPWAVEFORMATxx) &wfi,
	 				0L, 0L, WAVE_FORMAT_QUERY);
 		}	 		
		break;
 	} 
	if (woo != 0) 
	{
		bTieneAudio = FALSE;
		return -1;
	}
    if ((woo = waveInOpen(&hWaveIn, waAudioInDevice,
		  (LPWAVEFORMATxx) &wfi, (DWORD) (UINT) hwnd, 0L, (DWORD) CALLBACK_WINDOW)) != 0) 
	{
    	char et[MAXERRORLENGTH];
			    
    	waveInGetErrorText(woo, et, sizeof et);
		bTieneAudio = FALSE;
		return -1;
    }  

	{
		HRESULT e;
	    e = mixerOpen(&hmx,(UINT)hWaveIn,(DWORD)hwnd,(DWORD)hInst,CALLBACK_WINDOW|MIXER_OBJECTF_HWAVEIN);
		if (e != MMSYSERR_NOERROR)
		{
			bTieneAudio = FALSE;
			hdx = -1;
			goto FatalAudioExit;
		}
	}
   
    //	Output initialisation
	while (TRUE) 
	{
 		woo = waveOutOpen(&hWaveOut, waAudioOutDevice, (LPWAVEFORMATxx) &wfi,
 								0L, 0L, WAVE_FORMAT_QUERY);
 		if (bitsPerSample > 8 && woo == WAVERR_BADFORMAT) 
		{
 			audioIs8Bit = TRUE;
 		} 	 				

 		if ((woo == WAVERR_BADFORMAT) && bitsPerSample > 8)
		{
 			bitsPerSample /= 2;
 			sampleAlignment /= 2;
 			bytesPerSecond /= 2;	
	    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
	    	wfi.wf.nBlockAlign = sampleAlignment;
	    	wfi.wBitsPerSample = bitsPerSample;
	 		woo = waveOutOpen(&hWaveOut, waAudioOutDevice, (LPWAVEFORMATxx) &wfi,
	 				0L, 0L, WAVE_FORMAT_QUERY);
 		}
		break; 		
 	} 
 		
	if (woo != 0)
	{				
    	char et[MAXERRORLENGTH];
	    	
    	waveOutGetErrorText(woo, et, sizeof et);
    	hdx = -1;
		bTieneAudio = FALSE;
		goto FatalAudioExit;
	}

    if ((woo = waveOutOpen(&hWaveOut, waAudioOutDevice,
		  (LPWAVEFORMATxx) &wfi, (DWORD) (UINT) hwnd, 0, (DWORD) CALLBACK_WINDOW)) != 0) {
    	char et[MAXERRORLENGTH];
	    	
    	/* The next line looks wrong, doesn't it?  But I've seen drivers
    	   for half-duplex sound boards that return NOTSUPPORTED instead
    	   of ALLOCATED when you try to open input and output at the
    	   same time. */
		    
	    if ((woo == MMSYSERR_ALLOCATED || woo == MMSYSERR_NOTSUPPORTED)) 
		{
			hdx = halfDuplex = TRUE;
			waveInClose(hWaveIn);
			if (waveOutOpen(&hWaveOut, waAudioOutDevice,
		  			(LPWAVEFORMATxx) &wfi, (DWORD) (UINT) hwnd, 0,
		  			(DWORD) CALLBACK_WINDOW) == 0) 
			{
			    waveOutClose(hWaveOut);
			    goto HdxAudioExit;						  		
		    }
			bTieneAudio = FALSE;
	    } else 
		{	
	    	waveOutGetErrorText(woo, et, sizeof et);
			/*
	        MessageBox(hwnd, et, rstring(IDS_T_ERR_OPEN_WAVE_OUTPUT),
				MB_OK | MB_ICONEXCLAMATION);
			*/
			hdx = -1;
			bTieneAudio = FALSE;
	    }
	} else 
	{
	    //aboutOutBits = bitsPerSample;
	    //aboutOutSamples = samplesPerSecond;
		waveOutClose(hWaveOut);
	}
	
FatalAudioExit:	
	{
    	waveInClose(hWaveIn);
    }
HdxAudioExit:
    return hdx;
}					  

/*	WAVEOUTSHUTDOWN  --  Shutdown wave audio output, if open.  */
						 
static void waveOutShutdown(void)
{
	if (outputActive) 
	{
		V waveOutReset(hWaveOut);
		if (outputPending) 
		{
			outputInShutdown = TRUE;
	    	//propUpdateAudio();
		} else 
		{
			V waveOutClose(hWaveOut);
			outputActive = FALSE;
	    	//propUpdateAudio();
		}
	}
}

/*  NEWCONNECTION  --  Initiate a connection to a given named
					   host.  If there's already a connection
					   to this host active, activate its window.  */
							
VOID newConnection(HWND hwnd, LPSTR knownHost,BOOL MultiConferencia,BOOL Aceptada,BOOL incluir)
{
    CHAR szHostName[MAX_HOST];
    SOCKADDR_IN sockHost;
#define addrHost sockHost.sin_addr
    LPCLIENT_DATA pClientData = NULL;
    HWND hwndClient;
    unsigned short port;    
	

	{
	    	char *cp;
	    	long iport;
		    char cb[MAX_HOST];
			
			strcpy(cb, knownHost);
		    if ((cp = strchr(cb, '/')) != NULL ||
		    	(cp = strchr(cb, ':')) != NULL) 
			{		    	
		    	iport = atol(cp + 1);
		    	if (iport <= 0) 
				{
					/*
			        MsgBox(hwnd, MB_ICONSTOP | MB_OK, Format(72), *cp); 
					*/
			        return;
		    	}
		    	port = (unsigned short) iport;
		    	*cp = 0;
		    } else 
			{
		    	port = NETFONE_COMMAND_PORT;
		    }
			wsprintf(szHostName, "(%s)", (LPSTR) cb);
			addrHost.s_addr = inet_addr(cb);
	}
    
    /*	See if there's already a client window communicating with
    	this host.  Use equality of IP number and port as the criterion to
    	prevent spoofing due to aliases. */
    
    sockHost.sin_port = htons(port);	
    hwndClient = NULL;


	pClientData = CLIENTPTR(hwnd);		


	if (!pClientData)
	{
	    //  Allocate a new connection structure
	
	    pClientData = (LPCLIENT_DATA) GlobalAllocPtr(GPTR, sizeof(CLIENT_DATA));
	
	    if (pClientData != NULL) 
		{		
		    //  Initialize the connection descriptor
		    
		    memset(pClientData, 0, sizeof(CLIENT_DATA));
		    pClientData->dwType = WINDOW_TYPE_CLIENT;
		    pClientData->wantsInput = FALSE;
		    pClientData->state = Embryonic;
		    pClientData->sReply = INVALID_SOCKET;
		    pClientData->timeout = -1;		// Connection with local origin is immortal		    
		    pClientData->inetSock.sin_addr.s_addr = addrHost.s_addr;
			pClientData->protocol = PROTOCOL_UNKNOWN;			
			pClientData->port = port;			
			pClientData->naceptadas = 0;
			nvoces_activas = 0;
		    strcpy(pClientData->szHost, szHostName);
		} else 
		{
	        return;
	    }
	    hwndClient = createNewConnection(pClientData);
	
	    if (hwndClient == NULL) 
		{
	        GlobalFreePtr(pClientData);
	        return;
		}
	}	    
	else
	{
		pClientData->inetSock.sin_addr.s_addr = addrHost.s_addr;
		pClientData->port = port;
	}

	/*volumen    *Volumes[5] = {&VolSpeaker,&VolWaveOut,&VolWaveIn,&VolMicrophone,NULL};*/
	if (!Volumes[0])
	{
		if (InitVolume(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,&VolSpeaker,TRUE,TRUE,0))
		{
			Volumes[0] = &VolSpeaker;
		}
	}
	if (!Volumes[1])
	{
		if (InitVolume(MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT,&VolWaveOut,TRUE,TRUE,0))
		{
			Volumes[1] = &VolWaveOut;
		}
	}
	if (!Volumes[2])
	{
		if (InitVolume(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,&VolWaveIn,TRUE,TRUE,0))
		{
			Volumes[2] = &VolWaveIn;
		}
	}
	if (!Volumes[3])
	{
		if (InitVolume(MIXERLINE_COMPONENTTYPE_DST_WAVEIN,&VolMicrophone,TRUE,TRUE,MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE))
		{			
			Volumes[3] = &VolMicrophone;
		}
	}
	RecogeVolumenes(Volumes,4);
	if (hwndVolumen)
	{
		SendMessage(hwndVolumen,WM_COMMAND,ID_Volumen,hwndVolumen);
	}

	if (incluir)
	{
		inputPaused = FALSE;
		if (!inputActive)
			spurt = TRUE;
		if (!UsarElCtrl && !startWaveInput(hwndMDIClient)) 
		{
			//	Couldn't turn on wave audio input
			SendMessage(hDlgOperacion,WM_COMMAND,MAKEWPARAM(ID_Mensaje,0),(LPARAM)pClientData->inetSock.sin_addr.s_addr);
			return;
		}
		pClientData->wantsInput = TRUE;		
		pClientData->timeout = -1;	// Send audio immortalises connection
		pClientData->state = SendingLiveAudio;	        		
	}

	if (pClientData->naceptadas > MAXIPS || pClientData->naceptadas < 0)
	{
		return;
	}
	if (incluir)
	{
		int i;
		for (i = 0;i < pClientData->naceptadas;i++)
		{
			if (pClientData->ipaceptada[i] == pClientData->inetSock.sin_addr.s_addr)
			{
				pClientData->portdestino[i] = pClientData->port;
				break;
			}
		}		
		if (i == pClientData->naceptadas)
		{
			if (i >= MAXIPS)
			{
				return;
			}
			else
			{
				pClientData->ipaceptada[i] = pClientData->inetSock.sin_addr.s_addr;
				pClientData->portdestino[i] = pClientData->port;
				pClientData->activa[i] = 0;
				pClientData->rseq[i] = -1;
				pClientData->sseq[i] = -1;
				pClientData->naceptadas++;
			}
		}
	    if (MultiConferencia)
		{
		    pClientData->activa[i] = 0x2;
		}
		else
		{
			int j;
			for (j = 0;j < pClientData->naceptadas;j++)
			{
				pClientData->activa[j] &= ~0x1;
			}
			pClientData->activa[i] |= 0x1;
		}
	}
	nvoces_activas = pClientData->naceptadas;
	{
		int j;
		for (j = 0;j < nvoces_activas;j++)
		{
			voces_activas[j] = CodigoPorIp(pClientData->ipaceptada[j]);
		}
	}

	if (incluir)
	{
		if (Aceptada)
		{
			SendUdpControl(hwnd,pClientData,fListening,"",1,addrHost.s_addr);
		}
		else
		{
			SendUdpControl(hwnd,pClientData,fRing,"",1,addrHost.s_addr);
		}	
	}


#undef addrHost	
}


//	onCreate  --  Initialise frame window when newly created
static BOOL onCreate(HWND hwnd, CREATESTRUCT FAR *pCreateStruct)
{
    /*CLIENTCREATESTRUCT ccs;*/
    WSADATA wsadata;
    SOCKERR serr;
    HDC hdc;
    TEXTMETRIC tm;
	HFONT ofont;    
    char md5key[16];
    
    serr = WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata);

    if (serr != 0) 
	{
		/*
        MsgBox(NULL, MB_ICONSTOP | MB_OK, Format(12), serr, SockerrToString(serr));
		*/
        return -1;
    }

    if (wsadata.wVersion < MINIMUM_WINSOCK_VERSION) {
		/*
        MsgBox(NULL, MB_ICONSTOP | MB_OK, Format(13), LOBYTE(wsadata.wVersion),
                HIBYTE(wsadata.wVersion), LOBYTE(MINIMUM_WINSOCK_VERSION),
                HIBYTE(MINIMUM_WINSOCK_VERSION));
		*/
        return -1;
    }
    aboutUDPmax = wsadata.iMaxUdpDg;
    
    /*	If the network can't transmit sound packets as large as we'd like
    	to send, restrict the size to what it can accommodate.  */
    	
    if (aboutUDPmax > 0) 
	{
    	if ((netMaxSamples + (sizeof(soundbuf) - BUFL) + 40) > (UINT) aboutUDPmax) {
    		netMaxSamples = aboutUDPmax - ((sizeof(soundbuf) - BUFL) + 40); 
    	}
    }    

    //  Create the command and control sockets


	
	/*
    serr = CreateSocket(&sEnvio, SOCK_DGRAM, 0, 0);
	if (serr != 0) 
	{
		return -1;
	}
	*/

    gsmh = gsm_create();

	lpc10init();


	sessionKeyGenerate(md5key, TRUE);
	memcpy((char *) &ssrc, md5key, sizeof ssrc);
	memcpy((char *) &timestamp, md5key + sizeof ssrc,
		  sizeof timestamp);
	memcpy((char *) &seq, md5key + sizeof ssrc + sizeof timestamp,
		  sizeof seq);
	/*
	memcpy((char *) &rtpdesrand, md5key + sizeof ssrc + sizeof timestamp +
		  sizeof seq, sizeof rtpdesrand);
	*/

#undef BoolProfile

    //	Check out the audio hardware
    
    halfDuplex = isHalfDuplex(hwnd);


    if (SetTimer(hwnd, FRAME_TIMER_ID, 10, NULL) == 0) 
	{
		/*
        MsgBox(NULL, MB_ICONSTOP | MB_OK, Format(15));
		*/
        return -1;
    }
	    	
	//	Calculate packet size based on selected modes
	
    currentInputLength = inputBufferLength();
    currentInputSamples = inputSampleCount();	

    //  Get textmetric data

    hdc = GetDC(hwnd);
	ofont = SelectObject(hdc, GetStockObject(ANSI_VAR_FONT));
    GetTextMetrics(hdc, &tm);
	SelectObject(hdc, ofont);
    ReleaseDC(hwnd, hdc);

    tmAveCharWidth = (INT) tm.tmAveCharWidth;
    tmHeight = (INT) tm.tmHeight;
    
    DragAcceptFiles(hwnd, TRUE);
    
    //	Finally, open the input sockets for business.

    serr = CreateSocket(&sCommand, SOCK_DGRAM, htonl(INADDR_ANY),(unsigned short)htons(NETFONE_COMMAND_PORT));
	if (serr != 0) 
	{
		/*
        MsgBox(NULL, MB_ICONSTOP | MB_OK, Format(14),
                serr, SockerrToString(serr));
		*/
        return -1;
	}
	sEnvio = sCommand;

    if (WSAAsyncSelect(sCommand, hwnd, WM_SOCKET_SELECT, FD_READ|FD_WRITE) != 0) {
        serr = WSAGetLastError();
    }
    if (serr != 0) 
	{
		/*
        MsgBox(NULL, MB_ICONSTOP | MB_OK, Format(14),
                serr, SockerrToString(serr));
		*/
    }	

	createSuccessful = TRUE;		
	
    return TRUE;
}

// Controlador de mensajes del cuadro Acerca de.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//	OBTAINOUTPUT  --  Try to obtain control of wave audio output

int obtainOutput(HWND hwnd)
{
	    
    //	Grab the audio device if we don't already have it
	
	outputTimeout = 0;					// Reset release output timeout    
    if (!outputActive) 
	{
	    PCMWAVEFORMAT wfi;    
    	MMRESULT woo;
    	
		/* If we already know the port is half duplex, just ditch the
		   packet right away rather than walking into the door again. */
	    	
		if (halfDuplex && inputActive && !breakinput) 
		{
			//propeller(IDC_PH_INPUT_LOST, ++inputPacketsLost);
			return FALSE;
		} else {
			if (halfDuplex && inputActive && breakinput) 
			{
				terminateWaveInput();
				/* terminateWaveInput() does not guarantee input is
				   actually closed when it returns.  It has to wait for
				   all kinds of debris to rattle through the message loop
				   before it's actually done.  So, if that's the case, deep six
				   this packet. */
				inputPaused = TRUE;
				if (inputActive) 
				{
					//propeller(IDC_PH_INPUT_LOST, ++inputPacketsLost);
					return FALSE;
				}
			}
    	
	    	wfi.wf.wFormatTag = WAVE_FORMAT_PCM;
	    	wfi.wf.nChannels = audioChannels;
	    	wfi.wf.nSamplesPerSec = samplesPerSecond;
	    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
	    	wfi.wf.nBlockAlign = sampleAlignment;
	    	wfi.wBitsPerSample = bitsPerSample;
	    	while (TRUE) {
		 		woo = waveOutOpen(&hWaveOut, waAudioOutDevice, (LPWAVEFORMATxx) &wfi,
		 								0L, 0L, WAVE_FORMAT_QUERY);
		 		if (bitsPerSample > 8 && woo == WAVERR_BADFORMAT) {
		 			audioIs8Bit = TRUE;
		 		} 
		 				
		 		/* If our preferred mode (16 bit, 11025 samples/second) isn't
		 		   supported, try falling back to bottom-feeder 8 bit per sample
		 		   mode. */			
		
		 		if ((woo == WAVERR_BADFORMAT) && bitsPerSample > 8) {
		 			bitsPerSample /= 2;
		 			sampleAlignment /= 2;
		 			bytesPerSecond /= 2;	
			    	wfi.wf.nAvgBytesPerSec = bytesPerSecond;
			    	wfi.wf.nBlockAlign = sampleAlignment;
			    	wfi.wBitsPerSample = bitsPerSample;
			 		woo = waveOutOpen(&hWaveOut, waAudioOutDevice, (LPWAVEFORMATxx) &wfi,
			 				0L, 0L, WAVE_FORMAT_QUERY);
		 		}
		 		
		 		/* If we've failed to initialise in either 16 or 8 bit mode
		 		   at 8000 samples per second, it's possible the sound card
		 		   doesn't support any sampling mode below the Windows standard
		 		   of 11025 samples per second.  Have another go-round and see
		 		   if 11025 works. */
		 		
		 		if (woo == WAVERR_BADFORMAT && samplesPerSecond == 8000) 
				{
		 			samplesPerSecond = 11025;
		 			bitsPerSample = 16;
		 			sampleAlignment = bitsPerSample / 8;
		 			bytesPerSecond = samplesPerSecond * sampleAlignment;  
		 		} else {
		 			break;
		 		}
		 	} 
	 		
	 		if (woo != 0) {				
		    	char et[MAXERRORLENGTH];
		    	
		    	waveOutGetErrorText(woo, et, sizeof et);
				/*
	        	MessageBox(hwnd, et, rstring(IDS_T_WAVE_PLAY_FORMAT_ERR),
			   		MB_OK | MB_ICONEXCLAMATION);
				*/
				return FALSE;
	    	}
		    if ((woo = waveOutOpen(&hWaveOut, waAudioOutDevice,
				  (LPWAVEFORMATxx) &wfi, (DWORD) (UINT) hwnd, 0, (DWORD) CALLBACK_WINDOW)) != 0) {
		    	char et[MAXERRORLENGTH];
		    	
		    	/* The next line looks wrong, doesn't it?  But I've seen drivers
		    	   for half-duplex sound boards that return NOTSUPPORTED instead
		    	   of ALLOCATED when you try to open input and output at the
		    	   same time. */
			    
			    if ((woo == MMSYSERR_ALLOCATED || woo == MMSYSERR_NOTSUPPORTED)
			    	&& inputActive) {
			    	/* Yuck.  The fact that we're receiving audio input
			     	   appears to have rendered the audio output port
			     	   busy.  Silently sacrifice the packet on the altar
			     	   of this most un-creative sound bobbler. */ 
					//propeller(IDC_PH_INPUT_LOST, ++inputPacketsLost);
					return FALSE;
			    }	
		    	waveOutGetErrorText(woo, et, sizeof et);
				/*
		        MessageBox(hwnd, et, rstring(IDS_T_ERR_OPEN_WAVE_OUTPUT),
					MB_OK | MB_ICONEXCLAMATION);
				*/
				return FALSE;
		    }
		    outputActive = TRUE;
		    //aboutOutSamples = samplesPerSecond;
		    //aboutOutBits = bitsPerSample;
	    	//propUpdateAudio();
		}
    }
    return TRUE;
}

//	createClient  --  Allocate and initialise client data for new connection

static LPCLIENT_DATA createClient(SOCKADDR_IN addrClient, soundbuf *d)
{
	LPCLIENT_DATA pClientData = (LPCLIENT_DATA) GlobalAllocPtr(GPTR, sizeof(CLIENT_DATA));
	if (pClientData != NULL) 
	{
	    memset(pClientData, 0, sizeof(CLIENT_DATA));
	    pClientData->dwType = WINDOW_TYPE_CLIENT;
	    pClientData->wantsInput = FALSE;
	    pClientData->state = Embryonic;
	    pClientData->sReply = INVALID_SOCKET;
		pClientData->timeout = 0;		
		pClientData->naceptadas = 0;
		pClientData->inetSock = addrClient;		
		pClientData->protocol = PROTOCOL_UNKNOWN;		
		pClientData->port = ntohs(addrClient.sin_port);		
		nvoces_activas = 0;
	    {
	    	/* Set host only on socket connections.  Leave dial string
	    	   blank for remote-initiated modem connections. */
	    	pClientData->szHost[0] = '(';
			/*
	    	if (d != NULL) 
			{
				strcpy(pClientData->szHost + 1, d->sendinghost);
				strcat(pClientData->szHost, " ");
			}
			*/
			strcat(pClientData->szHost, inet_ntoa(pClientData->inetSock.sin_addr));
			strcat(pClientData->szHost, ")"); 
		}
	}
	return pClientData;
}


VOID socketInput(HWND hwnd, SOCKET sock, SOCKERR serr, SOCKEVENT sevent,LPCLIENT_DATA pClientData)
{
    int rll,i;
    SOCKADDR_IN addrClient;
    INT cbAddrClient;    
    HWND hwndClient = hwnd;
	soundbuf receivedSoundBuffer;
    soundbuf *d = &receivedSoundBuffer;
    int loopedBack = sock == INVALID_SOCKET; 
    static int errorBoxUp = FALSE;
    static int RecCount = 0;
    // Prevent audio deadlock
    //int hdxPacketLost = (RecCount++ > 0) && (!loopedBack);
    int wasrtp = FALSE, ispurt = !outputActive, packetSeq;
	unsigned long iprecibida = 0;
	unsigned long codigorecibido = 0;
	unsigned short portrecibido = 0;
    
    
    //--RecCount;
    
	cbAddrClient = sizeof(addrClient);
	{
		if ((sevent != FD_READ) || (serr != 0) || errorBoxUp) 
		{
		    //  Unknown command or unknown event or socket error.  Ignore it.
        	if (pClientData != NULL) 
			{
        		pClientData->outputSocketBusy = FALSE;
        	}
		    return;
		}
	
		//  We got a packet.  Let's see what it tells us to do.
	
		rll = recvfrom(sock, (CHAR FAR *) d, sizeof(soundbuf), 0, 
		    				(SOCKADDR FAR *) &addrClient,
							&cbAddrClient);
	}	
	if (rll == SOCKET_ERROR) 
	{
//OutputDebugString("Data socket recvfrom error.\r\n");
		SendDebug(hwndClient,pClientData," SOCKET_ERROR ");
		return;	
	}
	
	//propeller(IDC_PH_PACKETS_RECEIVED, ++packetsReceived);
	

//OutputDebugString("Data input\r\n");
//xd(d, rll, TRUE);
	
    iprecibida = (unsigned long)addrClient.sin_addr.s_addr;
	codigorecibido = CodigoPorIp(iprecibida);
    portrecibido = ntohs(addrClient.sin_port);
    
    /* If the user closed a connection to this window within
    	LazarusLength seconds ago, don't allow a packet from the
    	other end to re-open it. */
    	
    if ((Lazarus == addrClient.sin_addr.s_addr) &&
    	(LazarusLong > 0)) 
	{
		SendDebug(hwndClient,pClientData," Lazarus ");
    	return;
    }

	if (!pClientData)
	{
		if ((pClientData = createClient(addrClient, d)) == NULL) 
		{			
			return;
		}
	}
	
	pClientData->port = portrecibido;
	for (i = 0;i < pClientData->naceptadas;i++)
	{
		if (pClientData->ipaceptada[i] == iprecibida)
		{
			pClientData->portdestino[i] = portrecibido;
			break;
		}
	}

	
	outputTimeout = 0;					// Reset release output timeout    
    if (!obtainOutput(hwnd)) 
	{
    	/*hdxPacketLost = TRUE;*/
    	/* If we're broadcasting, we ignore the failure to obtain
    	   output due to half duplex hardware.  This allows remote
    	   users to blip us to subscribe to the broadcast. */
    	/*if (!broadcasting) */
		/*
		{
			SendDebug(hwndClient,pClientData," !obtainOutput ");
    		goto FatalAudioExit;
    	}
		*/
    }
	    
    if (pClientData == NULL) 
	{
        goto FatalExit;
    }
    
    if (pClientData->state == Embryonic) 
	{   
   	   
    	//  Create a new connection child window
		hwndClient = createNewConnection(pClientData);
	}

    if (hwndClient != NULL) 
	{
		pClientData->cbReceived += rll;
		if (pClientData->timeout > 0) 
		{
			pClientData->timeout = 0;
		}		
		
		if (!wasrtp) 
		{
			LONG xbl;

			/* Convert relevant fields from network to host
			   byte order, if necessary. */

			d->compression = ntohl(d->compression);

			if (!(d->compression & fProtocol))
			{
				return;
			}

			if ((d->compression & (fProtocol | fPing | fAck)) == (fProtocol | fPing | fAck))
			{
				unsigned long *p;
				p = (unsigned long *)d->buffer.buffer_val;
				if (p[0] == 0x12123310)
				{
					p[2] = GetTickCount() - p[2];
				}
				SendMessage(hDlgOperacion,WM_COMMAND,MAKEWPARAM(ID_Echo,(d->compression & 0xFFF)),(LPARAM)d->buffer.buffer_val);
				return;
			}

			if ((d->compression & (fProtocol | fPing)) == (fProtocol | fPing))
			{
				/* peticion de echo */				
				if ((d->compression & 0xFFF) >= (sizeof(long)*35))
				{
					unsigned long *p;					
					int i;
					p = (unsigned long *)d->buffer.buffer_val;
					if (p[0] == 0x12123310)
					{
						if (!outputActive)
							p[3] = 1;
						else
							p[3] = 0;
						if (!inputActive)
						{
							p[3] |= 0x2;
						}
						p[4] = nvoces_activas;
						for (i = 0;i < nvoces_activas && i < 30;i++)
						{
							p[5+i] = voces_activas[i];
						}
					}					
					else if (p[0] == 0x12123311)
					{
						unsigned long *p;					
						int i;
						p = (unsigned long *)d->buffer.buffer_val;
						p[0] = 0x12123312;
						/*p[1] id */
						/*p[2] ticks en origen */
						/*p[3] flags */
						p[4] = 3;
						p[5] = iprecibida;
						p[6] = portrecibido;						
						p[7] = GetTickCount();
						/*addrClient.sin_port = htons(NETFONE_COMMAND_PORT);*/						
						SendUdpControl(hwnd,pClientData,fPing,d->buffer.buffer_val,(d->compression & 0xFFF),iprecibida);
						return;
					}
					else if (p[0] == 0x12123312)
					{
						unsigned long *p;					
						int i;
						p = (unsigned long *)d->buffer.buffer_val;
						p[0] = 0x12123313;
						/*p[1] id */
						/*p[2] ticks aqui */
						p[2] = GetTickCount() - p[2];
						/*p[3] flags */
						p[4] = 5;
						/*
						p[5] = iprecibida;						
						p[6] = portrecibido;						
						*/						
						p[8] = iprecibida;						
						p[9] = portrecibido;
						SendUdpControl(hwnd,pClientData,fPing,d->buffer.buffer_val,(d->compression & 0xFFF),iprecibida);
						return;
					}
					else if (p[0] == 0x12123313)
					{
						p[7] = GetTickCount() - p[7];
						if (p[5] != iprecibida || p[6] != portrecibido)
						{
							p[4] = 7;
							p[10] = p[5];
							p[11] = p[6];
							p[5] = iprecibida;
							p[6] = portrecibido;						
						}						
						{
							struct in_addr in;
							in.S_un.S_addr = iprecibida;
							sprintf(szVoxHost,"%s:%d",inet_ntoa(in),portrecibido);
							newConnection(hwndMDIClient,szVoxHost,0,0,TRUE);
						}						
						SendMessage(hDlgOperacion,WM_COMMAND,MAKEWPARAM(ID_Echo,(d->compression & 0xFFF)),(LPARAM)d->buffer.buffer_val);
						return;
					}
				}
				SendUdpControl(hwnd,pClientData,fPing | fAck,d->buffer.buffer_val,(d->compression & 0xFFF),iprecibida);
				return;
			}

			if ((d->compression & (fProtocol | fInfo | fListening)) == (fProtocol | fInfo | fListening))
			{				
				/*envia_debug_info = 1;*/
				return;
			}

			if ((d->compression & (fProtocol | fInfo | fRing)) == (fProtocol | fInfo | fRing))
			{				
				/*envia_debug_info = 0;*/
				return;
			}

			if ((d->compression & (fProtocol | fInfo | fMessage)) == (fProtocol | fInfo | fMessage))
			{   // Debug
				
				SendMessage(hDlgOperacion,WM_COMMAND,MAKEWPARAM(ID_Mensaje,(d->compression & 0xFFF)),(LPARAM)0);
				RecibeInfo((void *)d,codigorecibido);
				return;
			}

			packetSeq = (d->compression >> 24) & 0xFF;

			if ((d->compression & fEnd))
			{
				/*
				if (EsAceptada(pClientData,CodigoPorIp(addrClient.sin_addr.s_addr)))
				{
					PostMessage(hwndClient,WM_COMMAND,ID_Terminar,(LPARAM)addrClient.sin_addr.s_addr);
				}
				*/
				return;
			}
			if ((d->compression & fRing))
			{
				/*
				if (!EsAceptada(pClientData,CodigoPorIp(addrClient.sin_addr.s_addr)))
				{
					PostMessage(hDlgOperacion,WM_COMMAND,ID_Ring,(LPARAM)addrClient.sin_addr.s_addr);
				}
				*/
				return;
			}
			if ((d->compression & fListening))
			{
				/*
				if (EsAceptada(pClientData,CodigoPorIp(addrClient.sin_addr.s_addr)))
				{
					PostMessage(hDlgOperacion,WM_COMMAND,ID_Listening,(LPARAM)addrClient.sin_addr.s_addr);
				}
				*/
				return;
			}
			if ((d->compression & fMessage))
			{
				//SendMessage(hDlgOperacion,WM_COMMAND,MAKEWPARAM(ID_Mensaje,(d->compression & 0xFFF)),(LPARAM)d->buffer.buffer_val);
				return;
			}

			if (!(d->compression & (fCompGSM610 | fCompGSM723 | fCompLPC10)))
			{
				/* ninguna voz conocida */
				return;
			}

			if (!outputActive)
				return;

			xbl = (d->compression & 0xFFF) + (sizeof(struct soundbuf) - BUFL);

			if (xbl != rll) 
			{
				pClientData->protocol = PROTOCOL_UNKNOWN;
				SendDebug(hwndClient,pClientData," PROTOCOL_UNKNOWN ");
				return;
			}
			/* If does appear to be a genuine Speak Freely sound
			   buffer.	On that basis, set the protocol to Speak Freely
               even if the buffer isn't explicitly tagged. */

			if (pClientData->protocol != PROTOCOL_SPEAKFREE) 
			{
				pClientData->protocol = PROTOCOL_SPEAKFREE;				
			}
		}		

		if (!EsAceptada(pClientData,addrClient.sin_addr.s_addr,packetSeq))
		{
			return;
		}

		if (pClientData->inetSock.sin_addr.s_addr != addrClient.sin_addr.s_addr || pClientData->port != ntohs(addrClient.sin_port))
		{
			pClientData->inetSock = addrClient;		
			pClientData->port = ntohs(addrClient.sin_port);				
			{
	    		pClientData->szHost[0] = '(';
				/*
	    		if (d != NULL) {
					strcpy(pClientData->szHost + 1, d->sendinghost);
					strcat(pClientData->szHost, " ");
				}
				*/
				strcat(pClientData->szHost, inet_ntoa(pClientData->inetSock.sin_addr));
				strcat(pClientData->szHost, ")"); 
			}
		}        
	
		//	Okay, now we're ready to play the sound buffer

		//if (!hdxPacketLost) 
		{						
			if (!outputPending /*ispurt*/ && (jitterBuf > 0) && (jitterTimer == 0)) 
			{
				jitterTimer = SetTimer(hwnd, JITTER_TIMER_ID, jitterBuf, NULL);
				if (jitterTimer != 0) 
				{
					waveOutPause(hWaveOut);
					jitterPause = TRUE;
				} 
			}			

			pClientData->state = PlayingReceivedAudio;
			playSound(hwndClient, pClientData, d, bitsPerSample, samplesPerSecond, iprecibida);

#ifdef TICKTRACE
				{
				static LONG tickmax = 0;
				static LONG tickmin = 999999;
				static int ctickmax = 0;
				static int ctickmin = 0;
				static int ctickotro = 0;
				static int recibidos = 0;
				LONG dif;

				if ((d->compression & fProtocol))
				{
				recibidos++;
				dif = GetTickCount() - d->tick;
				if (dif > tickmax)
				{
					tickmax = dif;
					ctickmax = recibidos;
				} else if (dif < tickmin)
				{
					tickmin = dif;
					ctickmin = recibidos;
				}
				else
				{
					ctickotro = recibidos;
				}
				}
				}
#endif

		}
		return;
    }

    //  Unable to create connection window

    if (pClientData != NULL) 
	{
        GlobalFreePtr(pClientData);
    }

FatalExit:
FatalAudioExit:
	errorBoxUp = FALSE;
}


//
//  FUNCIÓN: WndProc(HWND, unsigned, WORD, LONG)
//
//  PROPÓSITO: procesar mensajes de la ventana principal.
//
//  WM_COMMAND	: procesar el menú de aplicación
//  WM_PAINT	: pintar la ventana principal
//  WM_DESTROY	: enviar un mensaje de finalización y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
    LPCLIENT_DATA pClientData;
    pClientData = CLIENTPTR(hWnd);	

	switch (message) 
	{
	HANDLE_MSG(hWnd, WM_CREATE, onCreate);	
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Analizar las selecciones de menú:
		switch (wmId)
		{
		/*
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		*/
		case ID_Activa:
			{
				LRESULT r = 0;
				ultimaipactiva = lParam;
				lParam = IpPorCodigo(lParam);				
				if (pClientData && pClientData->naceptadas)
				{
					int i;

					if (pClientData->naceptadas > MAXIPS || pClientData->naceptadas < 0)
					{
						return 0;
					}

					for (i = 0;i < pClientData->naceptadas;i++)
					{
						if (pClientData->ipaceptada[i] == lParam)
						{
							break;
						}
					}
					if (i < pClientData->naceptadas)
					{
						for (i = 0;i < pClientData->naceptadas;i++)
						{
							pClientData->activa[i] &= ~0x1;
						}					
						for (i = 0;i < pClientData->naceptadas;i++)
						{
							if (pClientData->ipaceptada[i] == lParam)
							{
								pClientData->activa[i] |= 0x1;
								inputPaused = FALSE;
								if (!inputActive)
									spurt = TRUE;
								startWaveInput(hwndMDIClient);
							}
						}					
					}
				}
				return r;
			}
			break;
		case ID_Conexiones:
			{				
				LRESULT r = 0;
				if (pClientData && pClientData->naceptadas)
				{
					r = pClientData->naceptadas;
					if (pClientData->naceptadas > MAXIPS || pClientData->naceptadas < 0)
					{
						return 0;
					}
					nvoces_activas = pClientData->naceptadas;
					{
						int i;
						for (i = 0;i < pClientData->naceptadas;i++)
						{
							voces_activas[i] = CodigoPorIp(pClientData->ipaceptada[i]);
						}
					}
				}
				return r;
			}
			break;
		case ID_Terminar:	
			if (pClientData)
			{				
				TerminaConexion(hWnd,TRUE,IpPorCodigo(lParam));				
			}
			break;
		case ID_Conectar:
			{				
				struct in_addr in;
				long ip = htonl(lParam);
				in.S_un.S_addr = IpPorCodigo(lParam);
				{					
					unsigned short puerto;
					puerto = 5900 + (ip & 0xff);
					sprintf(szVoxHost,"%s:%d",inet_ntoa(in),puerto);
				}								
				newConnection(hWnd,szVoxHost,(wmEvent&0x2),(wmEvent&0x1),TRUE);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: agregar código de dibujo aquí...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
        if (pClientData != NULL) 
		{
			TerminaConexion(hWnd,TRUE,0);
	    }	    
		terminateWaveInput();
	    waveOutShutdown();
		if (hmx)
		{
			mixerClose(hmx);
			hmx = NULL;
		}
		WSACleanup();
		break;
    /*	Input sound buffer received.  Pass it on to the output
        handler of every client window that presently says it
        wants input.  */
    
	case MM_WIM_DATA:
		{
			LPWAVEHDR isb;
			static LPARAM qsb = 0;
			HWND hwndClient = hwndMDIClient;/*GetWindow(hwndMDIClient, GW_CHILD);*/
			static int RecCount = 0;
			int bOverRun;
			
			// Prevent overrun and deadlock			
			

			bOverRun = (RecCount++ > 0);

			if (bOverRun && !inputTerm) 
			{
				//	Attempt to queue buffer for later processing in loop
				if (qsb == 0) 
				{
					qsb = lParam;
					RecCount--;
					break;						 
				}

				/*	No go; there's a buffer already queued.	 Before
					we discard this buffer, it must be placed back into
					the pool for wave input.  */

				isb = (LPWAVEHDR) lParam;
				isb->dwBufferLength = currentInputLength;				    
				waveInAddBuffer(hWaveIn, isb, sizeof(WAVEHDR));
			    //propeller(IDC_PH_OUTPUT_LOST, ++outputPacketsLost);
				RecCount--;
				break;
			}

			--RecCount;

			while (TRUE)
			{
				int firstTime = TRUE;
				
				/*	One little subtlety.  Since we allow the user to
					change compression modes on the fly, the buffer that
					just arrived may have been filled based on out of
					date compression modes.  In particular, if it's longer
					than we currently expect, it could frag the network.
					If this is the case, chop the extra samples off the
					end of the buffer.  This causes momentary loss of sound,
					but that's a lot better than the dreaded "Cannot write to
					socket" network error.  There's no problem processing a
					buffer shorter than expected, except a possible momentary
					click in GSM encoding. */
					
				isb = (LPWAVEHDR) lParam;
				if (isb->dwBytesRecorded > (DWORD) currentInputLength) {
					isb->dwBytesRecorded = currentInputLength;
				} else if (isb->dwBytesRecorded < (DWORD) currentInputLength &&
							isb->dwBytesRecorded > 0) {
					memset(isb->lpData + isb->dwBytesRecorded,
						sampleAlignment == 1 ? 0x80 : 0,
						((int) (currentInputLength - isb->dwBytesRecorded)));
					isb->dwBytesRecorded = currentInputLength;
				} 
						
				if (pClientData && isb->dwBytesRecorded > 0 && !inputPaused) 
				{	
					    hwndClient = hWnd;
					    if (pClientData->wantsInput)
						{
							if (!bOverRun) 
							{	// Overflow
								if (firstTime) 
								{
    								isWaveSound = FALSE;
									createSoundBuffer(isb->lpData,
										(WORD) isb->dwBytesRecorded,
										audioChannels, samplesPerSecond,
										bytesPerSecond, (WORD) sampleAlignment);
									firstTime = FALSE;
								}
								shipSoundBuffer(hwndClient, pClientData);
							}
						}
				}
				if (inputTerm) 
				{
					waveInUnprepareHeader(hWaveIn, isb, sizeof(WAVEHDR));
					if (isb->lpData != NULL) {
						GlobalFreePtr(isb->lpData);
					}
					GlobalFreePtr(isb);
					if (--waveHeadersAllocated == 0) 
					{
						waveInClose(hWaveIn);
						hWaveIn = NULL;
						inputActive = FALSE;
						inputTerm = FALSE;
						//propUpdateAudio();
					}
				} else 
				{
					isb->dwBufferLength = currentInputLength;				    
					waveInAddBuffer(hWaveIn, isb, sizeof(WAVEHDR));
				}
				if (qsb == 0) 
				{
					break;
				}
				lParam = qsb;
				qsb = 0;
			}
		}
		break;    
    //	Output sound buffer complete    
    case MM_WOM_DONE:
		{
			LPWAVEHDR waveHdr = (LPWAVEHDR) lParam;			

			if (pClientData)
			{
				SendDebug(hwndMDIFrame,pClientData," Played ");
			}
			outputPending--;
			/*
			if (hDlgPropeller != NULL) 
			{
				char s[80];
				
				if (outputPending == 0) {
					wsprintf(s, Format(6));
				} else {
					wsprintf(s, Format(7), outputPending);
				}
				SetDlgItemText(hDlgPropeller, IDC_PH_AUDIO_OUT_QUEUE, s);
			}
			*/
			waveOutUnprepareHeader(hWaveOut, waveHdr, sizeof(WAVEHDR));	
			GlobalFreePtr(waveHdr->lpData);

			// antes de borrarlo para evitar coincidencias ...
			limpia_ultimo_buffer(waveHdr);

			GlobalFreePtr(waveHdr);			
			if ((halfDuplexTransition || outputInShutdown) &&
					(outputPending == 0)) {
				waveOutClose(hWaveOut);
				outputActive = FALSE;
				if ((!outputInShutdown && halfDuplexTransition) || inputPaused) 
				{
					V startWaveInput(hWnd);
					inputPaused = FALSE;
				}
				halfDuplexTransition = outputInShutdown = FALSE;
			    //propUpdateAudio();
			}
			
			/*
			{		
				    long cuanto;				    
					int maximo = 6;
					int i = 0;
					do
					{
						cuanto = 0;
						ioctlsocket(sCommand,FIONREAD,&cuanto);
						if (cuanto < (sizeof(LONG)))
							break;
						socketInput(hWnd,sCommand,(SOCKERR)0,
						         FD_READ,pClientData);						
						pClientData = CLIENTPTR(hWnd);	
						i++;
					} while(pClientData && i < maximo);
					
			}
			*/
			
			
		}						 
        break;
        /*HANDLE_MSG(hWnd, WM_SOCKET_SELECT, socketInput);*/
        case WM_SOCKET_SELECT:
			{
			   SOCKEVENT event = WSAGETSELECTEVENT(lParam);
			   if (event == FD_READ && sCommand == (SOCKET)wParam)
			   {		
				    long cuanto;				    
					int maximo = 6;
					int i = 0;
					do
					{
						cuanto = 0;
						ioctlsocket(sCommand/*(SOCKET)wParam*/,FIONREAD,&cuanto);
						if (cuanto < (sizeof(LONG)))
							break;
						socketInput(hWnd,sCommand/*(SOCKET)wParam*/,(SOCKERR)WSAGETSELECTERROR(lParam),
						         event,pClientData);
						pClientData = CLIENTPTR(hWnd);
						i++;
					} while(pClientData && i < maximo);
					return TRUE;
			   }
			}
        	if (pClientData != NULL) 
			{
				pClientData->outputSocketBusy = FALSE;
        	}
			return TRUE;
		case WM_SOCKET_CONTROL:
        	return 0;
        case WM_SOCKET_ASYNC:
        	if (pClientData != NULL) 
			{
       			if (WSAGETASYNCERROR(lParam) == 0) 
				{
					/*
				    LPHOSTENT host;
				
					
				    host = (LPHOSTENT) pClientData->hostBuffer;
				    //SetWindowText(hwnd, pClientData->localLoopback ?
				    //	rstring(IDS_T_LOOPBACK) : host->h_name);
					
					if (host->h_name)
					{
						_fstrcpy(pClientData->szHost, host->h_name);
					}
	        		//changeAudioState(hwnd, pClientData);
					*/
			    }
				pClientData->getNameTask = NULL;
        	}			
			return TRUE;
			break;
        case WM_TIMER:
        	if (wParam == JITTER_TIMER_ID) 
			{
        		KillTimer(hWnd, JITTER_TIMER_ID);
        		jitterTimer = 0;
        		if (jitterPause) 
				{
        			jitterPause = FALSE;
        			if (outputActive) 
					{
        				waveOutRestart(hWaveOut);
        			}
        		}
        		return 0;
        	}
			{
				long cuanto;				    
				if (sCommand != INVALID_SOCKET)
				{
					cuanto = 0;
					ioctlsocket(sCommand/*(SOCKET)wParam*/,FIONREAD,&cuanto);
					if (cuanto > (sizeof(LONG)))
					{
						PostMessage(hWnd,WM_SOCKET_SELECT,(WPARAM)sCommand,(LPARAM)MAKELONG(FD_READ,0));
					}
				}
			}
			//CreateIpForwardEntry
			return procesar_timer(hWnd,pClientData,wParam,lParam);
			break;
		/*case MM_MIXM_LINE_CHANGE:*/
		case MM_MIXM_CONTROL_CHANGE:
			if ( bTieneAudio )
			{
				int i;
				for (i = 0;i < 5;i++)
				{
					if (!Volumes[i])
						continue;
					if (lParam == Volumes[i]->dwControlID || lParam == Volumes[i]->dwControlMute)
					{
						GetCurrentVolumen(Volumes[i]);
						if (hwndVolumen)
						{
							SendMessage(hwndVolumen,WM_COMMAND,ID_Volumen,hwndVolumen);
						}

						/*
						if (Volumes[i] == &VolSpeaker)
						{
							HWND hicono;
							if (lParam == Volumes[i]->dwControlID)
							{
								DWORD vol = VolSpeaker.dwMaximalVolume-(DWORD)SendMessage(hVolume,TBM_GETPOS,0,0);
								if (vol != VolSpeaker.dwActualVolume)
								{
									SendMessage(hVolume,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(VolSpeaker.dwMaximalVolume-VolSpeaker.dwActualVolume));
								}
							}
							else
							{								
								hicono = GetDlgItem(hDlgOperacion,IDC_ICOVOL);
								if (hicono)
								{
									HGDIOBJ ant = (HGDIOBJ)SendMessage(hicono,STM_GETIMAGE ,(WPARAM)IMAGE_BITMAP,0);
									if (ant)
									{
										DeleteObject(ant);
									}
									if (VolSpeaker.dwMute)
									{
										SendMessage(hicono,STM_SETIMAGE ,(WPARAM)IMAGE_BITMAP,(LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MIVOLNO)));
									}
									else
									{
										SendMessage(hicono,STM_SETIMAGE ,(WPARAM)IMAGE_BITMAP,(LPARAM)LoadBitmap(hInst,MAKEINTRESOURCE(IDB_MIVOL)));
									}
									InvalidateRect(hicono,NULL,TRUE);
								}								
							}
						}
						*/
						break;
					}
				}
			}
			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

static void MueveItem(HWND hwnd,UINT id,int dif,BOOL expandir)
{
	HWND hitem;
	RECT rect;
	POINT p1;

	hitem = GetDlgItem(hwnd,id);
	GetWindowRect(hitem,&rect);
	if (expandir)
	{
		p1.x = rect.left;
		p1.y = rect.top;
		rect.right += dif;
	}
	else
	{
		p1.x = rect.left + dif;
		p1.y = rect.top;
	}
	ScreenToClient(hwnd,&p1);
	MoveWindow(hitem,p1.x,p1.y,rect.right-rect.left,rect.bottom-rect.top,FALSE);
}


BOOL PreActivacionesEspeciales(HWND hWnd, UINT message, WPARAM wParam,
                               LPARAM lParam)
{
	int mirateclado = 0;
	if (message == WM_KEYDOWN)
	{	
		mirateclado = 1;
        if ((GetKeyState( VK_LSHIFT ) & 0x8000) && (GetKeyState( VK_RSHIFT ) & 0x8000) && ultimaipactiva)
		{
			LPCLIENT_DATA pClientData = NULL;
			unsigned long idecho = 0x12123310;
			int len = sizeof(long)*35;
			long buffer[35];
			int i;

			pClientData = CLIENTPTR(hwndMDIClient);
			if (!pClientData)
			{
				struct in_addr in;
				long ip = htonl(ultimaipactiva);
				in.S_un.S_addr = IpPorCodigo(ultimaipactiva);				
				{					
					unsigned short puerto;
					puerto = 5900 + (ip & 0xff);
					sprintf(szVoxHost,"%s:%d",inet_ntoa(in),puerto);
				}
				/*strcpy(szVoxHost,inet_ntoa(in));*/
				newConnection(hWnd,szVoxHost,0,0,FALSE);
				pClientData = CLIENTPTR(hwndMDIClient);
			}
			if (pClientData)
			{
				long ip = htonl(ultimaipactiva);
				buffer[0] = (long)idecho;
				buffer[1] = ultimaipactiva;
				buffer[2] = GetTickCount();
				for (i = 3;i < 35;i++)
					buffer[i] = 0;
				{					
					unsigned short puerto;
					puerto = 5900 + (ip & 0xff);
					pClientData->port = puerto;
				}	
				
				/* poner aqui control de debug, acordarse de quitarlo (o el firewall ya te lo recordara!) */

				SendUdpControl(hwndMDIClient,pClientData,fProtocol | fPing,(char *)buffer,len,IpPorCodigo(ultimaipactiva));
			}			


		}
#ifdef AUNNO
		if ((GetKeyState( VK_LSHIFT ) & 0x8000) && (GetKeyState( VK_RSHIFT ) & 0x8000) && (GetKeyState( VK_CONTROL ) & 0x8000))
		{
			if (lpc10compress)
				robust++;
			else
				robust = 1;

			/*
			gsmcompress = FALSE;
			lpc10compress = TRUE;
			*/
		} 
		else if ((GetKeyState( VK_RSHIFT ) & 0x8000) || (GetKeyState( VK_LSHIFT ) & 0x8000) || (GetKeyState( VK_CONTROL ) & 0x8000))
		{
			/*
			gsmcompress = TRUE;
			lpc10compress = FALSE;
			*/
		}
#endif
	}
	else if (message == WM_KEYUP || message == WM_TIMER)
	{		
		mirateclado = 1;
	}

	/*if (mirateclado)*/
	{
		if ((GetKeyState( VK_CONTROL ) & 0x8000))
		{
			if (!inputActive && UsarElCtrl)
			{
				LPCLIENT_DATA pClientData = NULL;
				pClientData = CLIENTPTR(hwndMDIClient);
				if (pClientData && pClientData->naceptadas > 0)
				{
					startWaveInput(hwndMDIClient);
				}
			}
		}
		else
		{
			if (inputActive && UsarElCtrl)
			{
				terminateWaveInput();
			}
		}
	}

	return FALSE;
}

/* Crea la conexion de salida */
void EnviaSenyal(long yo,unsigned long ip,unsigned short port)
{
	LPCLIENT_DATA pClientData = NULL;
	unsigned long idecho = 0x12123311;
	int len = sizeof(long)*35;
	long buffer[35];
	int i;
	
	/*
	pClientData = CLIENTPTR(hwndMDIClient);
	if (!pClientData)
	*/
	{
		struct in_addr in;
		in.S_un.S_addr = ip;
		sprintf(szVoxHost,"%s:%d",inet_ntoa(in),port);
		newConnection(hwndMDIClient,szVoxHost,0,0,TRUE);
		pClientData = CLIENTPTR(hwndMDIClient);
	}
	if (pClientData)
	{
		buffer[0] = (long)idecho;
		buffer[1] = yo;
		buffer[2] = GetTickCount();
		if (!bTieneAudio)
			buffer[3] = 1;
		else
		{
			buffer[3] = 0;

			if (halfDuplex)
			{
				buffer[3] |= 0x4;
			}
			if (!inputActive)
			{
				buffer[3] |= 0x2;
			}
		}

		for (i = 4;i < 35;i++)
			buffer[i] = 0;		
		pClientData->port = port;
		SendUdpControl(hwndMDIClient,pClientData,fProtocol | fPing,(char *)buffer,len,ip);
	}
}

int SetUsarElCtrl(int usar)
{
	int prev = UsarElCtrl;
	UsarElCtrl = usar;
	if (!usar)
	{
		if (!inputActive)
		{
			LPCLIENT_DATA pClientData = NULL;
			pClientData = CLIENTPTR(hwndMDIClient);
			if (pClientData && pClientData->naceptadas > 0)
			{
				startWaveInput(hwndMDIClient);
			}
		}
	}
	else
	{
		if ((GetKeyState( VK_CONTROL ) & 0x8000))
		{
			if (!inputActive)
			{
				LPCLIENT_DATA pClientData = NULL;
				pClientData = CLIENTPTR(hwndMDIClient);
				if (pClientData && pClientData->naceptadas > 0)
				{
					startWaveInput(hwndMDIClient);
				}
			}
		}
		else
		{
			if (inputActive)
			{
				terminateWaveInput();
			}
		}
	}
	return prev;
}

int SetNoEnviarAudio(int noenviar)
{
	int prev = NoEnviarAudio;
    NoEnviarAudio = noenviar;
	if (noenviar)
	{
		if (inputActive)
		{
			terminateWaveInput();
		}
	}
	else
	{
		if (!inputActive && (!UsarElCtrl || (GetKeyState( VK_CONTROL ) & 0x8000)))
		{
			LPCLIENT_DATA pClientData = NULL;
			pClientData = CLIENTPTR(hwndMDIClient);
			if (pClientData && pClientData->naceptadas > 0)
			{
				startWaveInput(hwndMDIClient);
			}
		}
	}
	return prev;
}
