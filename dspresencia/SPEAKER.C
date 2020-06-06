/*

	Play sound buffer
	
*/

#include "netfone.h"

/*  GSMDECOMP  --  Uncompress the contents of a sound buffer using GSM.  */

static void gsmdecomp(LPCLIENT_DATA pClientData, soundbuf *sb)
{
    gsm_signal dst[160];
    int i, j, l = 0;
    char *dpx = ((char *) sb->buffer.buffer_val) + sizeof(short);
    static char dcb[BUFL];
    short declen = ntohs(*((short *) sb->buffer.buffer_val));
    
    if (declen <= 0 || declen > 1600) 
	{
        declen = 1600;
    }
    for (i = 0; i < (int) ((sb->compression&0xfff) - sizeof(short));
                i += sizeof(gsm_frame))
	{
		// Decode audio using client's gsm handle.
        gsm_decode(pClientData->gsmh, (gsm_byte *) dpx, dst);
        dpx += sizeof(gsm_frame);
        
		for (j = 0; j < 160; j++)
		{
            dcb[l++] = audio_s2u(dst[j]);
        }
    }
    memcpy(sb->buffer.buffer_val, dcb, declen);
	sb->compression = (declen) | (sb->compression&0xfffff000);
    /*sb->buffer.buffer_len = declen;*/
}


static void gsm723decomp(LPCLIENT_DATA pClientData, soundbuf *sb)
{    
    int i, l = 0;
    char *dpx = ((char *) sb->buffer.buffer_val) + sizeof(short);
    static char dst[BUFL];
    short declen = ntohs(*((short *) sb->buffer.buffer_val));    

    //l = RGL_Decompress (-1, (uint8 *)dpx,declen,(uint8 *)dst, 2001);

	for (i = 0;i < l;i++)
	{
        sb->buffer.buffer_val[i] = audio_c2u(dst[i]);
	}

	if (l != declen)
	{
		declen = l;
	}

    memcpy(sb->buffer.buffer_val, dst, l);
	sb->compression = (l) | (sb->compression&0xfffff000);
    /*sb->buffer.buffer_len = declen;*/
}


/*	LPC10DECOMP  --  Uncompress the contents of a sound buffer using LPC10.  */

static void lpc10decomp(struct soundbuf *sb, LPCLIENT_DATA c)
{
	int j;
	char *dpx = ((char *) sb->buffer.buffer_val);
	char dcb[BUFL];

	j = lpc10decode(dpx, dcb, (sb->compression&0xfff));
	memcpy(sb->buffer.buffer_val, dcb, j);
	sb->compression = (j) | (sb->compression&0xfffff000);
}

/*  PLAYSOUND  --  Play a sound buffer, decrypting and decompressing
				   as required.  */

#define MAXIMOBUF 1024
typedef struct
{
	LPWAVEHDR wh;
	int len;
	unsigned long ips_playing[64];
	int nips_playing;
	int enviado;
} Mezcla;
static Mezcla mezcla[MAXIMOBUF];
static int nmezcla = 0;

void limpia_ultimo_buffer(LPWAVEHDR quita)
{
	int i;
	if (outputPending < 3)
	{
		LPWAVEHDR wh;		
		int primera;		
		for (primera = 0;primera < nmezcla;primera++)
		{
			if (!mezcla[primera].enviado)
			{
				break;
			}
		}
		while (nmezcla > primera && outputPending < 5)
		{
			wh = mezcla[primera].wh;			
			if (wh && wh->lpData)
			{				
				wh->dwFlags = 0;
				mezcla[primera].enviado = 1;
				waveOutPrepareHeader(hWaveOut, wh, sizeof(WAVEHDR));		
				if (waveOutWrite(hWaveOut, wh, sizeof(WAVEHDR)) == 0)
				{
					outputPending++;					
					primera++;
				}
				else
				{
					mezcla[primera].enviado = 0;
					break;
				}
			}
			else
			{
				if (!mezcla[primera].enviado && mezcla[primera].wh)
				{
					GlobalFreePtr(mezcla[primera].wh);
				}
				nmezcla--;
				for (i = primera;i < nmezcla;i++)
				{
					memcpy(&(mezcla[i]),&(mezcla[i+1]),sizeof(Mezcla));
				}		
				mezcla[nmezcla].wh = NULL;
				mezcla[nmezcla].nips_playing = 0;
				mezcla[nmezcla].len = 0;
				mezcla[nmezcla].enviado = 0;
			}
		}
	}
	if (!quita)
		return;
	for (i = 0;i < nmezcla;i++)
	{
		if (mezcla[i].wh == quita)
		{
			break;
		}
	}
	if (i >= nmezcla)
		return;
	if (i)
	{
		i = i;// debug!!!
	}
	nmezcla--;
	for (;i < nmezcla;i++)
	{
		memcpy(&(mezcla[i]),&(mezcla[i+1]),sizeof(Mezcla));
	}		
	mezcla[nmezcla].wh = NULL;
	mezcla[nmezcla].nips_playing = 0;
	mezcla[nmezcla].len = 0;
	mezcla[nmezcla].enviado = 0;
}

static int mirasimezclar(unsigned long iprecibida,LPWAVEHDR *wh,int **plen)
{
	int i;
	int mezclar = 0;
	int j;
	int indice = 0;
	int siesta = 0;


	if (!nmezcla)
	{
		mezcla[0].nips_playing = 1;
		mezcla[0].len = 0;
		mezcla[0].ips_playing[0] = iprecibida;
		mezcla[0].wh = NULL;
		mezcla[0].enviado = 0;
		nmezcla++;
	}
	else 
	{
		for (j = 0;j < nmezcla;j++)
		{		
			for (i = 0;i < mezcla[j].nips_playing;i++)
			{
				if (iprecibida == mezcla[j].ips_playing[i])
				{
					if (mezcla[j].enviado)
						siesta = 1;
					break;
				}
			}
			if (i >= mezcla[j].nips_playing)
			{
				if (mezcla[j].enviado)
				{
					/*
					if (siesta)
						return 2;
					*/
					continue;
				}
				if (mezcla[j].wh)
					mezclar = 1;
				mezcla[j].ips_playing[mezcla[j].nips_playing] = iprecibida;
				mezcla[j].nips_playing++;
				indice = j;
				break;
			}
		}
		if (mezclar == 1 && mezcla[j].wh)
		{
			*wh = mezcla[j].wh;
			*plen = &(mezcla[j].len);
			return mezclar;
		}
		if (j >= nmezcla)
		{
			if (nmezcla >= MAXIMOBUF)
			{
				*wh = mezcla[nmezcla-1].wh;
				*plen = NULL;
				return 1;
			}
			indice = nmezcla;
			mezcla[nmezcla].nips_playing = 1;
			mezcla[nmezcla].ips_playing[0] = iprecibida;
			mezcla[nmezcla].wh = NULL;
			mezcla[nmezcla].enviado = 0;
			nmezcla++;
		}
	}	

	if (!nmezcla || !mezcla[indice].wh)
	{
		(*wh) = (LPWAVEHDR) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,
				sizeof(WAVEHDR) + sizeof(CHAR *));
		if ((*wh) == NULL) 
		{		
			return 0;
		}
		memset(*wh,0,sizeof(WAVEHDR) + sizeof(CHAR *));
		*plen = NULL;
	}

	if (nmezcla)
	{
		if (!mezcla[indice].wh)
		{
			mezcla[indice].wh = *wh;
		}
		*plen = &(mezcla[indice].len);
	}
	return mezclar;
}


int asigna_lpData(LPWAVEHDR wh,int len,int *plen)
{
	LPSTR old;
	if (!wh->lpData)
	{			   
		wh->lpData = (LPSTR) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,(DWORD)len); 
		if (wh->lpData == NULL) 
		{
			GlobalFreePtr(wh);
			return 0;
		}
		memset(wh->lpData,0,len);
		if (plen)
			*plen = len;
	}
	else
	{
		if (plen)
		{
			if (*plen < len)
			{
				old = wh->lpData;
				wh->lpData = (LPSTR) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,(DWORD)len); 
				if (wh->lpData == NULL) 
				{
					GlobalFreePtr(wh);
					return 0;
				}
				memset(wh->lpData,0,len);				
				memcpy(wh->lpData,old,*plen);
				*plen = len;				
				GlobalFreePtr(old);
			}
		}
	}
	return len;
}

void asigna_grabados(LPWAVEHDR wh,LPSTR sbuf)
{
	DWORD len = (((LPSTR) sbuf) - wh->lpData);
	if (wh->dwBufferLength < len)
	{
		wh->dwBufferLength = wh->dwBytesRecorded = len;
	}		
}


short mezcla_sonido(short a ,short b)
{
	short r;
	int test;

	test = (int)a + (int)b;
	if (test > 32767)
	{
		r = 32767;
	}
	else if (test < -32768)
	{
		r = -32768;
	}
	else r = a + b;

	return r;
}

				   
void playSound(HWND hWnd, LPCLIENT_DATA pClientData, soundbuf *d,
			   int bitsPerSample, int samplesPerSecond,unsigned long iprecibida)
{
	LPWAVEHDR wh = NULL;
	short *sbuf;
	unsigned char *ulp;
	/*WORD stat;*/
	int i, len;
    char *val, **specsamp;
	int mezclar = 0;    
	int *plen = NULL;

    /*	If the message queue is close to exhaustion, ditch the output
    	buffer to avoid a hangup due to queue overflow.  */
    	
    if (outputPending >= ((3 * messageQueueSize) / 4)) 
	{
		//propeller(IDC_PH_INPUT_LOST, ++inputPacketsLost);
		SendDebug(hwndMDIFrame,pClientData," outputPending ");
    	return;
    }	

	/*
	if (outputPending < 5 && primeraip && primeraip != iprecibida)
		return;	
	*/

	mezclar = mirasimezclar(iprecibida,&wh,&plen);

	if (wh == NULL) 
	{
		SendDebug(hwndMDIFrame,pClientData," wh ");
		return ;
	}


	/* esto no se usa pero lo guardamos para ampliaciones */
	specsamp = (char **) (((char *) wh) + sizeof(WAVEHDR));
	*specsamp = NULL;
	
	/* Perform requested decryption and decompression of the
	   received data. */
	   
    len = (int) (d->compression&0xfff);
    val = d->buffer.buffer_val;

    /* If message is compressed, decompress appropriately. */

    if (d->compression & fCompGSM610) 
	{
        gsmdecomp(pClientData, d);
        len = (int) (d->compression&0xfff);
    }    

    if (d->compression & fCompGSM723) 
	{
        gsm723decomp(pClientData, d);
        len = (int) (d->compression&0xfff);
    }    

	if (d->compression & fCompLPC10) 
	{
		lpc10decomp(d, pClientData);
		len = (int) (d->compression&0xfff);
	}    
			
    if (samplesPerSecond == 11025) 
	{
	    if (bitsPerSample == 16) 
		{
			
			/* Convert the resulting u-law samples in the sound buffer
			   to 16 bit signed linear format. */
			if (!asigna_lpData(wh,(((BUFL * ((DWORD) sizeof(short)) * 12) / 8)),plen))
				return;
			  
			sbuf = (short *) wh->lpData;
			ulp = (unsigned char *) val;
			
			for (i = 0; i < len; i++) 
			{
				int j = i & 7;
				if (mezclar)
				{					
					*sbuf = mezcla_sonido(audio_u2s(*ulp),*sbuf);
					sbuf++;					
				}
				else
				{
					*sbuf++ = audio_u2s(*ulp);
				}
				//	This should be written out for better optimisation
				if (j > 0 && !(j & 1)) 
				{
					if (mezclar)
					{
						*sbuf++ += audio_u2s(*ulp);
					}
					else
					{
						*sbuf++ = audio_u2s(*ulp);
					}
				} else if (j % 320 == 319) 
				{
					if (mezclar)
					{
						*sbuf++ += audio_u2s(*ulp);
					}
					else
					{
						*sbuf++ = audio_u2s(*ulp);
					}
				}
				ulp++; 
			}
			asigna_grabados(wh,(LPSTR)sbuf);
		} else if (bitsPerSample == 8) 
		{
			BYTE *bbuf;
			
			/* Convert the resulting u-law samples in the sound buffer
			   to 8 bit PCM format. */
			if (!asigna_lpData(wh,(((BUFL * ((DWORD) sizeof(short)) * 12) / 16)),plen))
				return;

			bbuf = (BYTE *) wh->lpData;
			ulp = (unsigned char *) val;
	
			for (i = 0; i < len; i++) {
				int j = i & 7;
				*bbuf++ = audio_u2c(*ulp);
				//	This should be written out for better optimisation
				if (j > 0 && !(j & 1)) 
				{
					if (mezclar)
					{
						*bbuf++ += audio_u2c(*ulp);
					}
					else
					{
						*bbuf++ = audio_u2c(*ulp);
					}
				} else if (j % 320 == 319) 
				{
					if (mezclar)
					{
						*bbuf++ += audio_u2c(*ulp);
					}
					else
					{
						*bbuf++ = audio_u2c(*ulp);
					}
				}
				ulp++; 
			}
			asigna_grabados(wh,(LPSTR)bbuf);
		}
	} else 
	{	// samplesPerSecond == 8000
	    if (bitsPerSample == 16) 
		{	   			
			/* Convert the resulting u-law samples in the sound buffer
			   to 16 bit signed linear format. */
			if (!asigna_lpData(wh,len * sizeof(short),plen))
				return;
			sbuf = (short *) wh->lpData;
			ulp = (unsigned char *) val;
			
			for (i = 0; i < len; i++) 
			{
				if (mezclar)
				{
					*sbuf = mezcla_sonido(audio_u2s(*ulp++),*sbuf);
					sbuf++;					
				}
				else
				{
					*sbuf++ = audio_u2s(*ulp++);
				}
			}
			asigna_grabados(wh,(LPSTR)sbuf);
		} else if (bitsPerSample == 8) 
		{
			BYTE  *bbuf;
			
			/* Convert the resulting u-law samples in the sound buffer
			   to 8 bit PCM format. */
			if (!asigna_lpData(wh,len * sizeof(BYTE),plen))
				return;
			 
			bbuf = (BYTE  *) wh->lpData;
			ulp = (unsigned char *) val;
	
			for (i = 0; i < len; i++) 
			{
				if (mezclar)
				{
					*bbuf++ += audio_u2c(*ulp++);
				}
				else
				{
					*bbuf++ = audio_u2c(*ulp++);
				}
			}
			asigna_grabados(wh,(LPSTR)bbuf);
		}
	}

	if (outputPending < 2)
	{
		limpia_ultimo_buffer(NULL);		
	}
	return;

#ifdef YANOSIRVE
	if (mezclar == 1 || nmezcla)
	{
		return;
	}
	
	/* No deberia pasar por aqui ya */

	wh->dwFlags = 0;
	waveOutPrepareHeader(hWaveOut, wh, sizeof(WAVEHDR));

	/* If output is paused due to anti-jitter, but we've used up half
	   our message queue with held packets, start output anyway.  A
	   little jitter is better than packets discarded due to the
	   message queue limitation. */

	if (jitterPause && (outputPending >= (messageQueueSize / 2)))
	{
		jitterPause = FALSE;
		waveOutRestart(hWaveOut);
//OutputDebugString("Restart for queue length\r\n");		
	}		
	
	stat = waveOutWrite(hWaveOut, wh, sizeof(WAVEHDR));		

	if (stat == 0) 
	{		
		SendDebug(hwndMDIFrame,pClientData," Ok1 ");
		outputPending++;
		
		/*
		if (hDlgPropeller != NULL) {
			char s[80];
					
			if (outputPending == 0) {
				wsprintf(s, Format(6));
			} else {
				wsprintf(s, Format(7), outputPending);
			}
			SetDlgItemText(hDlgPropeller, IDC_PH_AUDIO_OUT_QUEUE, s);
		}
		*/
	} else 
	{	
		SendDebug(hwndMDIFrame,pClientData," stat ");
		/*
		char et[MAXERRORLENGTH];
	    	
		waveOutGetErrorText(stat, et, sizeof et);
		*/
		waveOutUnprepareHeader(hWaveOut, wh, sizeof(WAVEHDR));
		GlobalFreePtr(wh);
		/*
		MsgBox(hWnd, MB_OK | MB_ICONEXCLAMATION, Format(45), et);
		*/
		return;
	}
#endif
}

