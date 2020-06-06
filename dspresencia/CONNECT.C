/*

	Window procedure for connection MDI window
	
*/

#include "netfone.h"

#define ASYNC_OUTPUT


                            
static struct {
	char header[4];
	unsigned short len, ilen;
	soundbuf sbm;
} mb = {{1, 2, 3, 4}};
#define sb	mb.sbm

soundbuf ebuf;				  		  // Utility sound buffer
static LONG pktlen;					  // Packet length to transmit
static int squelched = FALSE;		  // Squelched by VOX ?
static int sqpacket = FALSE;		  // Is this packet squelched
static char ourSendingHost[16];		  // Host name for sound buffer sendinghost
static int iseq = 0;				  // Image transmit sequence number
static int dseq = 0;				  // Data transmit sequence number

extern long outputPending;
char blankit[] = "                                                              "
				 "                                                              "
				 "                                                              ";

/*  GSMCOMP  --  Compress the contents of a sound buffer using GSM.  */

static void gsmcomp(soundbuf *asb)
{
    gsm_signal src[160];
    gsm_frame dst;
    int i, j, l = 0;
    char *dp = (asb->buffer.buffer_val) + sizeof(short);
    long ldata = (asb->compression&0xFFF); 
    
    for (i = 0; i < ldata; i += 160) {
        for (j = 0; j < 160; j++) 
		{
            if ((i + j) < (asb->compression&0xFFF)) 
			{
                src[j] = audio_u2s(asb->buffer.buffer_val[i + j]);
            } else {
                src[j] = 0;
            }
        }
        gsm_encode(gsmh, src, dst);
        _fmemcpy(dp, dst, sizeof dst);
        dp += sizeof dst;
        l += sizeof dst;
    }

    /* Hide original uncompressed buffer length in first 2 bytes of buffer. */
    
    *((short *) asb->buffer.buffer_val) = (short) ldata;
    revshort((short *) asb->buffer.buffer_val);
	
    asb->compression = (l + sizeof(short)) | (asb->compression&0xFFFFF000);
}


static void gsm723comp(soundbuf *asb)
{
    char *dp = (asb->buffer.buffer_val) + sizeof(short);
    long ldata = (asb->compression&0xFFF);
	int l = 0,i;
    static unsigned char src[BUFL];   
	

	for (i = 0;i < ldata;i++)
	{
        src[i] = audio_u2c(dp[i]);
	}

    //l = RGL_Compress (-1, (uint8 *) src, ldata,(uint8 *)dp, 2001);

    /* Hide original uncompressed buffer length in first 2 bytes of buffer. */
    
    *((short *) asb->buffer.buffer_val) = (short) ldata;
    revshort((short *) asb->buffer.buffer_val);	
    asb->compression = (l + sizeof(short)) | (asb->compression&0xFFFFF000);
}



/*	LPC10STUFF	--	Stuff last 16 bytes of LPC10 packet in sendinghost. */

static int lpc10stuff(struct soundbuf FAR *asb, int pktlen,int *xseq)
{
	if ((asb->compression & fCompLPC10)) 
	{
		if (robust > 1) 
		{
			asb->compression |= fCompRobust;
		}
	}
	/* SIEMPRE!!! */
	if ((asb->compression & fCompLPC10) || (asb->compression & fCompGSM610) || (asb->compression & fCompGSM723)) 
	{
		asb->compression &= 0xFFFFFFF;
		*xseq = ((*xseq) + 1) & 0xFF;
		asb->compression |= ((long) (*xseq)) << 24;		
	}
	else if ((asb->compression & fImage)) 
	{
		asb->compression &= 0xFFFFFFF;
		asb->compression |= ((long) iseq) << 24;
		iseq = (iseq + 1) & 0xFF;
	}
	else
	{
		asb->compression &= 0xFFFFFFF;
		asb->compression |= ((long) dseq) << 24;
		dseq = (dseq + 1) & 0xFF;
	}

	return pktlen;
}

/*	LPC10UNSTUFF  --  Reverse the effect of lpc10stuff. */

static void lpc10unstuff(struct soundbuf FAR *asb)
{
	if ((asb->compression & fCompLPC10))
	{		
		if (asb->compression & fCompRobust) 
		{
			asb->compression &= ~fCompRobust;
			/*asb->buffer.buffer_len &= 0xFFFFFFL;*/
		}
	}
}


/*	LPC10COMP  --  Compress the contents of a sound buffer using LPC-10.  */

static void lpc10comp(soundbuf *asb)
{
	char *dp = (char *) asb->buffer.buffer_val;
	unsigned char *src = ((unsigned char *) asb->buffer.buffer_val);

	asb->compression |= fCompLPC10;
	asb->compression = lpc10encode(src, dp,(asb->compression&0xFFF)) | (asb->compression&0xFFFFF000);
}



//	WRITEOUTPUT  --  Transmit output buffer to destination

static int writeOutput(LPCLIENT_DATA d, LPSTR buf, int buflen,long ip)
{
	SOCKADDR_IN name;
	int i;
	/*static char relleno[1500] = "";*/

	/*
	if (d->outputSocketBusy) 
	{
		return buflen;					
	} else 
	*/
	{
		int stat = 0, wasSentTo = FALSE;
		
		
		memset(&name,0,sizeof(name));
		name.sin_family = AF_INET;
		name.sin_port = htons(d->port); /* NETFONE_COMMAND_PORT */
		for (i = 0;i < d->naceptadas;i++)
		{
			if (d->ipaceptada[i] == ip)
			{
				name.sin_port = htons(d->portdestino[i]);
				break;
			}
		}
		name.sin_addr.S_un.S_addr = (unsigned long)ip;

		if (((!useSendNotSendto || waNetNoConnect) &&
									(!waNetUseSend))) {
			{
#ifdef GRABARAFICHERO
				static mifile = -1;
				if (mifile == -1)
				{
					mifile = open(ajusta_a_home_dir("\\data.vox"),O_RDWR|O_BINARY|O_CREAT,S_IREAD|S_IWRITE);
				}
#endif
#ifdef MEDIRRATE
				static long ticks = 0;
				static long total_enviado = 0L;
				static long total_envios = 0L;
				if (!total_enviado)
				{
					ticks = GetTickCount();
				}
				total_enviado += buflen;
				total_envios++;
#endif
				stat = sendto(sEnvio, buf, buflen, 0,
						(LPSOCKADDR) &(name), sizeof name);
#ifdef GRABARAFICHERO
				write(mifile,buf, buflen);
#endif
#ifdef MEDIRRATE
				if (ticks && (GetTickCount()-ticks) >= 10000)
				{
					total_enviado += (total_envios*28);
					total_enviado = 0;
					total_envios = 0;
				}
#endif


				/*
				stat = sendto(d->sReply, relleno, 1400, 0,
						(LPSOCKADDR) &(d->name), sizeof d->name);				
				*/

			}
			if (stat < 0) 
			{
				if (!waNetNoConnect) 
				{
					//useSendNotSendto = TRUE;
					/*
					if (hDlgPropeller != NULL) 
					{
						SetDlgItemText(hDlgPropeller, IDC_PH_SENDTO, rstring(IDS_T_SEND));
					}
					*/
				}
				/*
				if (hDebugEdit)
				{
					SetWindowText(hDebugEdit,"OPPS!!");
				}
				*/
			} else 
			{
				wasSentTo = TRUE;
			}
		}
		/*	Careful!  Don't "optimise" this to "else if"; we have to be
			able to switch-hit when the first sendto() fails above. */
		/*
		if (!wasSentTo) 
		{
			stat = send(d->sReply, buf, buflen, 0);
		}
		*/
		if (stat == buflen) 
		{
		//	propeller(IDC_PH_PACKETS_SENT, ++packetsSent);
		} else {
			//propeller(IDC_PH_OUTPUT_LOST, ++outputPacketsLost);

			if (!waNetNoOutOverflow) 
			{
				int eerror;

				eerror = WSAGetLastError();

				/* Great leap of faith.  Treat a "WSAEWOULDBLOCK" error as
				   a truncated buffer and do nothing other than mark the
				   socket busy. */				

				if (stat == SOCKET_ERROR &&
					eerror == WSAEWOULDBLOCK) {
					stat = 0;
					d->outputSocketBusy = TRUE;
				}
			} 
		}
		return stat;
	}
}

//	SOCKETERRORBOX  --  Show message box for socket error

static void socketerrorbox(HWND hwnd, LPCLIENT_DATA d)
{
	/*
	if (d->modemConnection) {
	    MessageBox(hwnd, rstring(IDS_T_MODEM_WRITE_ERR), NULL,
	    	MB_ICONEXCLAMATION | MB_OK);
	} else {
	    MessageBox(hwnd, SockerrToString(WSAGetLastError()), rstring(IDS_T_SOCKET_WRITE_ERR),
	    	MB_ICONEXCLAMATION | MB_OK);
    }
	*/
} 

/*  SENDPKT  --  Send a message to the destination.  */

static int sendpkt(HWND hwnd, LPCLIENT_DATA d, struct soundbuf *asb)
{
    LONG lts = (asb->compression&0xFFF);
    LONG pkl = pktlen;
    LPCLIENT_DATA pClientData;    
    struct soundbuf *osb = asb;

	pClientData = CLIENTPTR(hwnd);	

	if (pClientData != d)
	{
		d = pClientData;
	}
	if (!d)
		return -1;
     
    {
    	int stat, nr;
		
		for (nr = 0; nr < ((lpc10compress && (robust > 1)) ? robust : 1); nr++) 
		{
			int i;
			for (i = 0;i < d->naceptadas && i < MAXIPS;i++)
			{
				if (d->activa[i])
				{
					pkl = lpc10stuff(osb, pkl,&(d->sseq[i]));
	                revlong(&osb->compression);
					stat = writeOutput(d, (LPSTR) osb, (int) pkl,d->ipaceptada[i]);
					revlong(&osb->compression);
					lpc10unstuff(osb);
				}
				/*
				if (stat < 0) 
				{
					break;
				}
				*/
			}
		}
        if (stat < 0 && WSAGetLastError() != WSAEINPROGRESS) 
		{
            d->state = d->wantsInput ? SendingLiveAudio : Idle;
            /*d->wantsInput = FALSE;*/
            socketerrorbox(hwnd, d);
            return FALSE;
        }
    }
    return TRUE;
}


/*  SHIPSOUNDBUFFER  --  Output sound buffer to connection.  */

void shipSoundBuffer(HWND hwnd, LPCLIENT_DATA pClientData)
{
	if (sqpacket != squelched) 
	{	
		squelched = sqpacket;
	}

	if (squelched)
	{
		return;
	}
    
    if (protocolSent == PROTOCOL_SPEAKFREE) 
	{		
	    sb.compression |= fProtocol;
	    sb.compression |= gsmcompress ? fCompGSM610 : 0;	    
		sb.compression |= gsm723 ? fCompGSM723 : 0;	    
	    sb.compression |= lpc10compress ? fCompLPC10 : 0;
	}
    if (sendpkt(hwnd, pClientData, &sb))
	{
	   //PonTexto("E ",0);	   
	}
}

void SendDebug(HWND hwnd, LPCLIENT_DATA pClientData,LPCSTR texto)
{
	if (envia_debug_info)
	{
		unsigned long ip = 0x0; /* estaba mi ip, mejor configurar un servidor para ello si es necesario este tipo de debug */
		SendUdpControl(hwnd,pClientData,fMessage,(void *)texto,strlen(texto)+1,ip);
	}
}

void SendMensaje(HWND hwnd, LPCLIENT_DATA pClientData,LPCSTR texto,long ip)
{
	SendUdpControl(hwnd,pClientData,fMessage,(void *)texto,strlen(texto)+1,ip);
}


void SendUdpControl(HWND hwnd, LPCLIENT_DATA pClientData,ULONG flag,void *buffer,int len,unsigned long ip)
{
	soundbuf sc;
	int pkl;

	sc.compression = fProtocol | (len&0xFFF);
	sc.compression |= flag;
	/*sc.buffer.buffer_len = len;*/
	memcpy(sc.buffer.buffer_val,buffer,len);
    pkl = len + (sizeof(struct soundbuf) - BUFL);
    revlong(&sc.compression);
    /*revlong(&sc.buffer.buffer_len);*/
	writeOutput(pClientData, (LPSTR) &sc, (int) pkl,ip);
}

/*  CREATESOUNDBUFFER  --  Create a standard format sound buffer
						   with selected compression modes from a
						   set of raw samples received from the audio
						   input port.  Special gimmick: align==0 means
						   the data are already in place in mu-law
						   encoding (rate==8000 only).  This allows fast
						   processing of mu-law encoded sound files.  */
						
void createSoundBuffer(LPSTR buffer, WORD buflen, DWORD channels,
					   DWORD rate, DWORD bytesec, WORD align)
{
	int knownFormat = FALSE;


#ifdef TICKTRACE
	sb.tick = GetTickCount();
#endif

	//	If the spectrum display is open, let it see the raw samples
	//spectrumUpdate(buffer, buflen, channels, rate, bytesec, align, FALSE);

	sb.compression = fProtocol; 
    sb.compression |= gsmcompress ? fCompGSM610 : 0;	    
	sb.compression |= gsm723 ? fCompGSM723 : 0;	    
	sb.compression |= lpc10compress ? fCompLPC10 : 0;


	if (rate == 8000) 
	{
		if (align == 2) {
			LONG i;
			int j;
			
			for (i = j = 0; i < (LONG) buflen / align; i++) 
			{
				sb.buffer.buffer_val[j++] = audio_s2u((((WORD FAR *) buffer)[i]));
			} 
		} else if (align == 1) {	// align == 1
			LONG i;
			int j;
			
			for (i = j = 0; i < (LONG) buflen; i++) {
				sb.buffer.buffer_val[j++] = audio_c2u((((BYTE FAR *) buffer)[i]));
			} 
		} else  {	//	(align == 0) already stored in mu-law encoding
			align = 1;
		}
		sb.compression = (buflen / align) | (sb.compression&0xfffff000);
		knownFormat = TRUE;
	} else if (rate == 11025 && align == 2) 
	{
		LONG i;
		int j, k;
		
		for (i = j = k = 0; i < (LONG) (buflen / align); i++) 
		{
			if ((k & 3) != 2  && ((i % 580) != 579)) 
			{
				sb.buffer.buffer_val[j++] = audio_s2u((((WORD FAR *) buffer)[i]));
			}
			k = (k + 1) % 11;
		} 
		sb.compression = (j) | (sb.compression&0xfffff000);		
		knownFormat = TRUE;
	} else if (rate == 11025 && align == 1) 
	{
		LONG i;
		int j, k;
		
		for (i = j = k = 0; i < (LONG) (buflen / align); i++) {
			if ((k & 3) != 2  && ((i % 580) != 579)) {
				sb.buffer.buffer_val[j++] = audio_c2u((((BYTE FAR *) buffer)[i]));
			}
			k = (k + 1) % 11;
		} 
		sb.compression = (j) | (sb.compression&0xfffff000);		
		knownFormat = TRUE;
	}
	
	sqpacket = FALSE;
	if (knownFormat) 
	{
		if (voxmode != IDM_VOX_NONE) 
		{
			LONG i;
			long alevel = 0;
			static long voxSampleCountdown;
			int j, thresh;
			
			thresh = (int) exp(log(32767.0) * ((1000 - noise_threshold) / 1000.0)); 
			for (i = 0, j = 0; i < (sb.compression&0xfff); i++, j++) {
				int samp = audio_u2s(sb.buffer.buffer_val[j]);
				
				if (samp < 0) {
					samp = -samp;
				}
				alevel += samp;
			}
			alevel /= (sb.compression&0xfff);
//{char s[132]; sprintf(s, "Nt = %d, Thresh = %d, Max = %d, Alevel = %ld, VU = %.2g\r\n",
//noise_threshold, thresh, maxsamp, alevel,
//log((double) alevel) / log(32767.0)); OutputDebugString(s);}			
			//voxMonitorUpdate(alevel, 0);
			if (alevel < thresh) {
				if (voxSampleCountdown <= 0) {
					sqpacket = TRUE;
					sb.compression &= 0xfffff000;
					/*sb.buffer.buffer_len = 0;*/
					return;
				}
				voxSampleCountdown -= (sb.compression&0xfff);
			} else {
				switch (voxmode) {
					case IDM_VOX_FAST:
						voxSampleCountdown = 4000;
						break;
					case IDM_VOX_MEDIUM:
						voxSampleCountdown = 8000;
						break;
					case IDM_VOX_SLOW:
						voxSampleCountdown = 12000;
						break;
				}
			}			 	        
		}
    
	    if (gsmcompress && gsmh) 
		{
	        gsmcomp(&sb);
	    }

	    if (gsm723) 
		{
	        gsm723comp(&sb);
	    }

		if (lpc10compress) 
		{
			lpc10comp(&sb);
		}
        
        pktlen = (sb.compression&0xfff) + (sizeof(struct soundbuf) - BUFL);
        
	
		spurt = FALSE;					  /* Not the start of a talk spurt */
	}
}											

/*  CREATENEWCONNECTION  --  Create a new connection MDI window.  */

HWND createNewConnection(LPCLIENT_DATA pClientData)
{    
    HWND hwnd;
    SOCKERR serr = 0;
    int vover;

#ifdef TRACE_FACE
	OutputDebugString("createNewConnection()\r\n");
#endif
	
	vover = (GetSystemMetrics(SM_CYFRAME) * 2) + GetSystemMetrics(SM_CYCAPTION) - 1;                    

    hwnd = hwndMDIClient;

    if (hwnd == NULL) 
	{
        return NULL;
    }
    
    pClientData->state = Idle;
    SetWindowLong(hwnd, GWL_CLIENT, (LONG) pClientData);    
	{
		/*
	    serr = CreateSocket(&(pClientData->sReply), SOCK_DGRAM,
	                         htonl(INADDR_ANY), 0);
		*/
		serr = 0;
		pClientData->sReply = sCommand;
	
	    if (serr == 0) 
		{
			/*
#ifdef ASYNC_OUTPUT	    
		    if (WSAAsyncSelect(pClientData->sReply, hwnd, WM_SOCKET_SELECT, FD_WRITE) != 0) 
			{
		        serr = WSAGetLastError();
		    }
#endif
			*/
			if (serr == 0) 
			{
		        pClientData->name.sin_family = PF_INET;
		        pClientData->name.sin_addr = pClientData->inetSock.sin_addr;
		        pClientData->name.sin_port = htons(pClientData->port);		        
		        if (!waNetNoConnect) 
				{
			        if (connect(pClientData->sReply, (LPSOCKADDR) &(pClientData->name),
			        	 sizeof(pClientData->name)) != 0) {
			            serr = WSAGetLastError();
			        }
		        }
		    }
   	    }
		
	    if (serr != 0) 
		{
			/*
	       MsgBox(hwnd, MB_ICONSTOP | MB_OK, Format(23),
	                (LPSTR) pClientData->szHost, serr, SockerrToString(serr));
			*/
	        return NULL;
	    }	    
    }

	if (!pClientData->gsmh)
	{
		pClientData->gsmh = gsm_create();
	}
	
    
    /* Save the sending host for Speak Freely protocol packets. */
    
    {
    	char gh[MAX_HOST];
    	
    	gethostname(gh, sizeof gh);
    	if (strlen(gh) > ((sizeof ourSendingHost) - 1)) 
		{
    		gh[(sizeof ourSendingHost) - 1] = 0;
    	}
    	strcpy(ourSendingHost, gh);
    	/*strcpy(sb.sendinghost, ourSendingHost);*/
    }

    if (pClientData->szHost[0] == '(') 
	{
    
    	/* I'm sure by now you're shocked and stunned to discover that
    	   some Winsock implementations, Sun PC-NFS 5.1, to name one,
    	   don't correctly implement the WSAAsyncGetHostByAddr function.
    	   Oh, you can make the call, and you even get back a valid host
    	   name.  But doing so plants a time bomb which will kill you
    	   (at least under the debug kernel) much, much later when you call
    	   WSACleanup() right before exiting the program.  At that time,
    	   depending on where the random pointer inside their so-called
    	   WSHELPER points, you get either two invalid global pointer
    	   errors or a fatal error due to an object usage count underflow in the
    	   (bogus) global block.  If the waNetSynchronousGetHostnameAction is
    	   set, we eschew the asynchronous request and make the user
    	   wait for a blocking gethostbyaddr() which has the merit, at
    	   least, of not blowing us away at program termination time. */  
    
		if (!waNetSynchronousGetHostnameAction) 
		{
		    	   
	    	/* This is a temporary connection initiated from the remote site.
	    	   Schedule a lookup to obtain the full domain name of the host,
	    	   not just the hostname included in the sound packet. */
	    	   
		    /*
		    pClientData->getNameTask = WSAAsyncGetHostByAddr(hwnd, WM_SOCKET_ASYNC,
		                                    (CHAR FAR *) &pClientData->inetSock.sin_addr,
		                                    sizeof(pClientData->inetSock.sin_addr),
		                                    PF_INET, pClientData->hostBuffer,
		                                    sizeof(pClientData->hostBuffer));
			if (pClientData->getNameTask == NULL) 
			{
				int serr = WSAGetLastError();
			}
		     */
		} else{
			LPHOSTENT h = gethostbyaddr((CHAR FAR *) &pClientData->inetSock.sin_addr,
		                                 sizeof(pClientData->inetSock.sin_addr),
		                                 PF_INET);
					
			if (h == NULL) {
#ifdef SHOW_GET_HOST_NAME_ERROR			
	            int serr = WSAGetLastError();
								            
				MsgBox(hwnd, MB_ICONSTOP | MB_OK, Format(27),
				        pClientData->szHost, serr,
				        SockerrToString(serr));
#endif				        
			} else {
				_fstrcpy(pClientData->szHost, h->h_name);

				/*
			    SetWindowText(hwnd, pClientData->localLoopback ?
			    	rstring(IDS_T_LOOPBACK) : pClientData->szHost);
	    		changeAudioState(hwnd, pClientData);
				*/
			}
			pClientData->getNameTask = NULL;			                             
		}	
	}

    DragAcceptFiles(hwnd, TRUE);
    ShowWindow(hwnd, SW_SHOW);
    openConnections++;
	//propUpdateAudio();

  // Save start time for calculating call duration later.
    pClientData->dwStartTime = GetTickCount();
    return hwnd;
}


int procesar_timer(HWND hwnd, LPCLIENT_DATA pClientData,WPARAM wParam, LPARAM lParam) 
{
	{
   		DWORD startTicks = GetTickCount();
        DWORD dwElapsedTime = GetTickCount();

			    if (pClientData == NULL) 
				{					
					return 0;
			    }

				if (wParam == FRAME_TIMER_ID && pClientData->outputSocketBusy) 
				{
					fd_set fds;
					struct timeval t;
					
					t.tv_sec = t.tv_usec = 0;
					FD_ZERO(&fds);
					FD_SET(pClientData->sReply, &fds);
					if (select(1, NULL, &fds, NULL, &t) > 0) 
					{
						pClientData->outputSocketBusy = FALSE;
					}
				}

				if (wParam == RETRASO_TIMER_ID)
				{
					//comprueba_nmezclas();
					return 0;
				}

				
				/*
				if (pClientData && wParam == FRAME_TIMER_ID)
				{
					int maximo = 3;
					int i = 0;
				    long cuanto;				    
					do
					{
						VOID socketInput(HWND hwnd, SOCKET sock, SOCKERR serr, SOCKEVENT sevent,LPCLIENT_DATA pClientData);

						cuanto = 0;
						ioctlsocket(sCommand,FIONREAD,&cuanto);
						if (cuanto < (sizeof(LONG)))
							break;
						socketInput(hwnd,sCommand,0,FD_READ,pClientData);
						pClientData = CLIENTPTR(hwnd);	
						i++;
					} while(pClientData && i < maximo);
				}
				*/

				if (!pClientData)
					return 0;
				
				

				/* If there are no buffers pending, advance the timeout
				   counter.  When it reaches TIMEOUT_CONNECTION, close the
				   connection. */
			
			    if (wParam == FRAME_TIMER_ID  &&
			    	pClientData->timeout >= 0 && outputPending == 0) 
				{
			        if (pClientData->timeout == 5) {
						if (!IsIconic(hwnd)) {
							pClientData->cbReceived = 0;
							pClientData->state = Idle;
						}
			        }
			    }				

				   
			   
			     
        dwElapsedTime -= pClientData->dwStartTime;
        dwElapsedTime /= 1000;

		/*
        if((dwElapsedTime % 6) == 0)  // Only paint every 6 seconds for timer.
        {
			      InvalidateRect(hwnd, NULL, TRUE);
			      UpdateWindow(hwnd);
        }
		*/
	}
    return 0;
}


typedef struct
{
  ULONG  compression;	
  LONG   tick;
  WPARAM wparam;
  LPARAM lparam;  
} infopars;

void EnviaInfo(WPARAM wparam,LPARAM lparam)
{
	infopars buf;
	SOCKADDR_IN dest;

	dest.sin_family = AF_INET;
	dest.sin_port   = htons(NETFONE_COMMAND_PORT);
	dest.sin_addr.S_un.S_addr = 0x0;

	buf.compression = fProtocol | fInfo | fMessage;
	buf.compression = htonl(buf.compression);

	buf.tick        = GetTickCount();
	buf.wparam      = wparam ^ 0x1f1f;
	buf.lparam      = lparam ^ 0x1f1f;
    sendto(sEnvio,(const char *)&buf, sizeof(buf), 0,
						(LPSOCKADDR) &(dest), sizeof dest);
}