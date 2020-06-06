
#include <windows.h>
#include <shellapi.h>
#define _INC_SHELLAPI

#ifndef RC_INVOKED

#include <windowsx.h>
#include <winsock.h>
#include <mmsystem.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>
#include <time.h>
#include <float.h>
#include <io.h>
#include <commdlg.h>
#include <math.h>
#include <wchar.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <commctrl.h>
#include <Richedit.h>

#define WM_COMMAND_ID(x)	(LOWORD(x))
#define WM_COMMAND_NOTIFY	(HIWORD(wParam))

//	Sugar coating of gratuitous incompatibilities between 16 and 32 bit Windows

#define LPWAVEFORMATxx LPWAVEFORMATEX

//	Application include files

#include "md5.h"
#include "ulaw.h"
#include "gsm/inc/gsm.h"
#include "vox.h"

#endif

#include "resource.h"

#ifdef NDEBUG
/*	Generate readable compile-time message if we leave debug output in
	a release build.  Genuinely conditional output should be explicitly
	conditioned on NDEBUG.  */
#ifdef OutputDebugString
#undef OutputDebugString
#endif	  
#define OutputDebugString { extern void OutputDebugString_in_release_build(void); \
							OutputDebugString_in_release_build(1); }
#else 
/*	If we're building for debugging, we'll probably eventually
	use the hex dump, so let's compile it in.  */
#define HEXDUMP
#endif

#define Gag(text) OutputDebugString("Gag me with a " #text "!\r\n");

#define IDM_VOX_FAST                    142
#define IDM_VOX_MEDIUM                  143
#define IDM_VOX_SLOW                    144
#define IDM_VOX_NONE                    145

//  Socket used to exchange sound packets

#define DEFAULT_NETFONE_COMMAND_PORT           2067
extern int NETFONE_COMMAND_PORT;

extern SOCKET sCommand;
extern SOCKET sEnvio;

//	If defined, show rant about serial I/O on comm errors

//  Maximum length (in characters) of a host name

#define MAX_HOST                        256

//  Get/SetWindowLong offsets

#define GWL_CLIENT                      GWL_USERDATA        // Client-side data

//  Type fields for client/server window data structures

#define WINDOW_TYPE_CLIENT              1

//  Various timeouts in seconds

#define FRAME_TIMER_ID                  1
#define RETRASO_TIMER_ID                2

#define TIMEOUT_CONNECTION				30				// Remote connection timeout
#define TIMEOUT_AUDIO_OUTPUT			4				// Release audio output timeout

//  Maximum printf-like output allowed by MsgBox

#define MAX_PRINTF_OUTPUT               1024

//	Multicast definitions missing from WINSOCK.H

#ifndef IN_MULTICAST
#define IN_MULTICAST(i) ((ntohl((long)(i)) & 0xF0000000) == 0xE0000000)
#endif

//  Messages posted by the asynchronous socket APIs

#define WM_SOCKET_SELECT	(WM_USER + 100)		// Data socket notifications
#define WM_SOCKET_ASYNC		(WM_USER + 101)		// Asynchronous request completions
#define WM_SOCKET_LWL		(WM_USER + 102)		// LWL socket notifications
#define WM_SOCKET_CONTROL	(WM_USER + 103)		// Control socket notifications


#define HANDLE_WM_SOCKET_SELECT(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (SOCKET)(wParam), (SOCKERR)WSAGETSELECTERROR(lParam), \
    (SOCKEVENT)WSAGETSELECTEVENT(lParam)), 0L)
#define FORWARD_WM_SOCKET_SELECT(hwnd, socket, serr, sevent, fn) \
    (void)(fn)((hwnd), WM_SOCKET_SELECT, (WPARAM)(socket), \
    (LPARAM)WSAMAKESELECTREPLY(sevent, serr))

#define HANDLE_WM_SOCKET_ASYNC(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (HANDLE)(wParam), (SOCKERR)WSAGETASYNCERROR(lParam), \
    (WORD)WSAGETASYNCBUFLEN(lParam)), 0L)
#define FORWARD_WM_GETXBYX(hwnd, hAsync, serr, cbBuffer, fn) \
    (void)(fn)((hwnd), WM_SOCKET_ASYNC, (WPARAM)(hAsync), \
    (LPARAM)WSAMAKEASYNCREPLY(cbBuffer, serr))

//  Determine number of elements in an array

#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

//  Make life with RC a little easier

#ifdef RC_INVOKED
#define ID(x)           x
#else
#define ID(x)           MAKEINTRESOURCE(x)
#endif

///	Resource IDs

#define IDI_BASE                        1000
#define IDM_BASE                        2000
#define IDD_BASE                        3000
#define IDA_BASE                        4000

//  Icon resource IDs

#define IDI_FRAME                       ID(1000)        // IDI_BASE + 0
#define IDI_CLIENT                      ID(1001)        // IDI_BASE + 1
#define IDI_SERVER                      ID(1002)        // IDI_BASE + 2

//  Menu resource IDs

#define IDM_FRAME_BASE                  2000            // IDM_BASE + 0
#define IDM_FRAME                       ID(IDM_FRAME_BASE)
#define IDM_CONNECTION_BASE             (IDM_FRAME_BASE + 0)
#define IDM_CONNECTION_EXIT             (IDM_CONNECTION_BASE + 3)

#define IDM_WINDOW_BASE                 (IDM_FRAME_BASE + 100)
#define IDM_WINDOW_CASCADE              (IDM_WINDOW_BASE + 0)
#define IDM_WINDOW_TILE_VERTICALLY      (IDM_WINDOW_BASE + 1)
#define IDM_WINDOW_TILE_HORIZONTALLY    (IDM_WINDOW_BASE + 2)
#define IDM_WINDOW_ARRANGE_ICONS        (IDM_WINDOW_BASE + 3)
#define IDM_WINDOW_FIRST_CHILD          (IDM_WINDOW_BASE + 4)

#define IDM_HELP_BASE                   (IDM_FRAME_BASE + 200)
#define IDM_HELP_ABOUT                  (IDM_HELP_BASE + 0)

#define IDM_CUSTOM						9000			// Custom (remembered file) menu items

//  Dialog resource IDs

#define IDD_ABOUT                       ID(3000)        // IDD_BASE + 0

#define IDD_NEW                         ID(3100)        // IDD_BASE + 100
#define IDD_NEW_HOST                    3101
#define IDD_NEW_HOST_LABEL              3102

//  Accelerator table resource IDs

#define IDA_FRAME	ID(4000)			// IDA_BASE + 0

#define BUFL	3600					// Sound buffer maximum length (actually less)

#define LONG	long
#define ULONG	unsigned long

#define SPEAK_FREE_FRAME_CLASS "SpeakFreeFrameClass"

struct soundbuf 
{
	ULONG compression;	
	struct 
	{
		/*LONG buffer_len;*/
		char buffer_val[BUFL];
	} buffer;
};
typedef struct soundbuf soundbuf;

// Packet mode flags

#define fMessage    0x1000		      // Message
#define fRing       0x2000		      // Ring
#define fEnd	    0x4000		      // Close connection
#define fCompGSM610 0x8000		      // GSM6.10 compression
#define fCompGSM723	0x40000	   	      // GSM7.11 compression
#define fCompLPC10	0x10000		 	  // LPC-10 compressed
#define fCompRobust 0x20000			  // Robust duplicate packet mode
#define fListening  0x80000           // I am listening
#define fImage      0x100000          // Image packet
#define fAck        0x200000          // Ack packet
#define fInfo       0x400000          // Info packet
#define fPing       0x800000          // Ping packet
#define fProtocol   0x8000000         // Own protocol flag

// Protocol types

#define PROTOCOL_SPEAKFREE  0		// Speak Freely protocol
#define PROTOCOL_UNKNOWN    4		// No evidence as to protocol yet


//  Socket-specific types

typedef INT	SOCKERR;        			// A socket error code
typedef WORD PORT;           			// A socket port number
typedef WORD SOCKEVENT;					// An asynchronous socket event


#define MAXIPS 24
//  Potential states for connection windows

typedef enum _CLIENT_STATE
{
    Embryonic,                          // Newly creates connection
    Idle,                               // Idle
    SendingLiveAudio,                   // Sending live audio
    Transferring,                       // Sending audio file
    PlayingReceivedAudio                // Playing audio received from remote host

} CLIENT_STATE;

/*  Connection window data.  A pointer to this structure is
	kept at offset GWL_CLIENT in each connection window. */

typedef struct _CLIENT_DATA
{
    DWORD dwType;                		// Type of window (WINDOW_TYPE_CLIENT)
    CLIENT_STATE state;                 // Current state.
    SOCKET sReply;                		// Socket waiting for reply from srv    
    int timeout;               			// Timeout counter
    SOCKADDR_IN inetSock;            	// Client's socket address
    unsigned short port;				// Destination data port number    
    CHAR szHost[MAX_HOST];    			// Target server's host name    
    DWORD cbSent;                		// Count of bytes sent so far.
    DWORD cbReceived;            		// Count of bytes received so far    
    HANDLE getNameTask;					// Get full site name task handle
    BYTE hostBuffer[MAXGETHOSTSTRUCT];	// Host name reply buffer
    int outputSocketBusy;				// Output socket is busy with a sendto()
	int wantsInput;						// Is wave input wanted ?
    struct sockaddr_in name;			// Target system address
    int squelch;		
	int naceptadas;
    unsigned long ipaceptada[MAXIPS];   // Accepted ips ... conversation
	unsigned short portdestino[MAXIPS];
	unsigned int  activa[MAXIPS];        // 	
    gsm gsmh;							// GSM handle for incoming audio    
	int rseq[MAXIPS];							// Receiving Sequence number
	int sseq[MAXIPS];							// Sendind Sequence number
    short protocol;               		// Transmission protocol        					    
    DWORD dwStartTime;                  // Time (ticks) the call was started.
} CLIENT_DATA, *LPCLIENT_DATA;

#define CLIENTPTR(w)            ((LPCLIENT_DATA)GetWindowLong((w), GWL_CLIENT))
#define IS_CLIENT_WINDOW(w)     (CLIENTPTR(w)->dwType == WINDOW_TYPE_CLIENT)

typedef enum {
	AE_PCMU, AE_PCMA, AE_G721, AE_IDVI, AE_G723, AE_GSM,
	AE_1016, AE_LPC, 
	AE_L8, AE_L16, AE_L24, AE_L32,
	AE_G728, AE_TRUE,
	AE_MAX
} audio_encoding_t;

/* Common audio description for files and workstations */

typedef struct {
	audio_encoding_t encoding;  /* type of encoding (differs) */
	unsigned sample_rate;       /* sample frames per second */
	unsigned channels;          /* number of interleaved channels */
} audio_descr_t;

//  Socket related data

extern SOCKET sCommand;							// Command socket
extern struct auxSocket *asList;				// List of auxiliary sockets

//  Various handles

extern HINSTANCE hInst;							// The current instance handle
extern HWND hwndMDIFrame;						// MDI frame  window handle
extern HWND hwndMDIClient;						// MDI client window handle
extern HWND hDlgOperacion;
extern HWND hTexto;
extern HWND hVolume;
extern HWND hDebugEdit;


extern char szVoxHost[51];
//  Window class names

extern LPSTR pszFrameClass;						// MDI frame  window class
extern LPSTR pszMDIClientClass;					// MDI client window class
extern LPSTR pszClientClass;					// MDI child window class

//  Miscellaneous data
extern int envia_debug_info;

extern LPSTR pszAppName;						// Application name
extern INT tmAveCharWidth;						// TEXTMETRIC.tmAveCharWidth
extern INT tmHeight;							// TEXTMETRIC.tmHeight
extern LPSTR commandLine;						// Command line from invocation
extern int holped;								// Help was invoked somewhere
extern UINT fileOpenHelpButton;					// File open help button message value
extern char *fileHelpKey;						// Help key for file open/save in progress
extern BOOL jitterPause;						// Queueing packets for anti-jitter ?


typedef struct 
{ 
   DWORD dwLineID;
   DWORD dwControlID;   
   DWORD dwEnabled;
   DWORD dwChannelCount;
   DWORD dwMinimalVolume;
   DWORD dwMaximalVolume;
   DWORD dwVolumeStep;
   DWORD dwActualVolume;
   DWORD dwControlMute;      
   DWORD dwMute;
   DWORD MutedwChannelCount;
   DWORD MutecMultipleItems;
   DWORD MutedwSource;
} volumen;
extern HMIXER   hmx;
extern HWAVEOUT hWaveOut;						// Wave output handle
extern HWAVEIN  hWaveIn;                  		// Wave input handle

extern soundbuf ebuf;							// Utility sound buffer

extern int audioChannels, samplesPerSecond, bytesPerSecond, sampleAlignment, bitsPerSample;

//	Compression modes

extern int voxmode;								// VOX mode
extern int breakinput;							// break input audio stream for output
extern int gsmcompress;							// GSM compression mode
extern int gsm723;	  						    // GSM 711 compression mode
extern int lpc10compress;						// LPC-10 compression mode
extern int robust;								// Robust mode for LPC-10
extern int protocolXmit;						// Default protocol to send (!protocolAuto or new connection)
extern int protocolSent;						// Protocol we're currently sending 
extern int protocolAuto;						// Send same protocol as received from connection
extern gsm gsmh;								// GSM compression handle
extern int currentInputSamples;					// Samples desired in current input buffers

//	Transmission modes

extern int listeners;							// Current wantsInput windows


//	Audio settings for About dialogue

extern int isWaveSound;							// Input from wave file ?
extern int inputActive, outputActive;			// Current activity flags
extern int inputPaused;							// Is input paused for output ?
extern int halfDuplex;                          // Is hardware half-duplex
extern int aboutInSamples;						// Input samples per second
extern int aboutInBits;							// Input bits per sample
extern int aboutOutSamples;						// Output samples per second
extern int aboutOutBits;						// Output bits per sample 

//	Propeller head information

extern long packetsReceived, packetsSent;		// Network packet traffic counters
extern long inputPacketsLost, outputPacketsLost;// Packets lost counters
extern int openConnections;						// Number of open connections
extern int halfDuplexTransition;				// Transitioning from output to input ?
extern int outputInShutdown;					// Close output when last buffer returned
extern long outputPending;						// Output buffers in queue
extern int messageQueueSize;					// Message queue size
extern long messageChecks;						// Anti-lockup calls on DefaultMessageLoop

//	Network configuration for About dialogue

extern int aboutUDPmax;                         // Longest UDP packet network handles
extern int netMaxSamples;						// Maximum samples, possibly net constrained

//	Windows and driver bug work-around options

extern int alwaysBindSocket;					// Bind output socket to avoid
												// bug in some WINSOCK implementations
extern int useSendNotSendto;					/* Use send() instead of sendto()
												   if buggy (Microsoft) WINSOCK
												   refuses to accept sendto() on
												   a connect()-ed socket. */
extern int waNetNoConnect;						// Don't connect(), use sendto()
extern int waNetUseSend;						// Use send(), not sendto() always
extern int waNetMultiTTLisChar;					// Argument to IP_MULTICAST_TTL setsockopt is char
extern int waNetNoOutOverflow;					// Disable output overflow detection and recovery
extern int waNetNoMsgLoopIns;					// Disable message loop insurance
extern int waNetSynchronousGetHostname;			// Use gethostbyaddr(), not WASAsync variant
extern int waNetSynchronousGetHostnameAction;	// Action depending on above and Winsock type

extern int waAudioHalf;							// Assume audio half-duplex; don't test
extern int waAudio11025;						// Assume audio 11025 samples/sec												

//	Anti-remote reincarnation protection

extern u_long Lazarus;							// Lazarus host ID
extern int LazarusLong;							// If nonzero, anti-reincarnation timeout
#define LazarusLength	15						// Lazarus timeout in seconds

extern char blankit[];                          // Long string of blanks

#define V	(void)

#ifdef RANT_ON_ERROR
#define errorRant(hwnd)	{if (modemShowRant) { modemRant(hwnd); } }
#else
#define errorRant(hwnd)
#endif


//	Function prototypes

extern void revlong(long *l);
extern void revshort(short *s);                                                        

extern HWND createNewConnection(LPCLIENT_DATA pClientData);

extern int spurt;								// Start of talk spurt flag	

extern BOOL voxMonitorDialog(HWND hwndParent);
extern BOOL IsVoxMonitorMessage(MSG *pmsg);
extern void voxMonitorUpdate(long nAudio, long nVox);
extern BOOL IsVoxMonitorOn();


//  Initialisation functions

extern BOOL InitApplication(HINSTANCE hInstance);
extern BOOL InitInstance(HINSTANCE hInstance, LPSTR pszCmdLine, INT nCmdShow);

// Default Message Loop

extern void DefaultMessageLoop();


//	LPC10 compression functions

extern void lpc10init(void);
extern int lpc10encode(unsigned char *in, unsigned char *out, int inlen);
extern int lpc10decode(unsigned char *in, unsigned char *out, int inlen);

//  Utility functions

extern int procesar_timer(HWND hWnd, LPCLIENT_DATA pClientData,WPARAM wParam, LPARAM lParam);
extern char *rstring(int resid);
extern char *rfilter(int resid);
extern INT MsgBox(HWND hwndParent, UINT fuType, LPSTR pszFormat, ...);
extern VOID WinPrintf(HDC hdc, INT row, INT col, LPSTR pszFormat, ...);
extern LPSTR SockerrToString(SOCKERR serr);
extern SOCKERR ResetSocket(SOCKET sock);
extern SOCKERR CreateSocket(SOCKET *psock, INT type, ULONG address, PORT port);
extern void playSound(HWND hWnd, LPCLIENT_DATA pClientData, soundbuf *d,
					  int bitsPerSample, int samplesPerSecond,unsigned long iprecibida);
extern void decodeSound(LPCLIENT_DATA pClientData, soundbuf *d,
			      int bitsPerSample, int samplesPerSecond, LPSTR* lpPCMData, DWORD* pdwPCMLength);
extern VOID newConnection(HWND hwnd, LPSTR knownHost,BOOL Muticonferencia,BOOL Aceptada,BOOL incluir);
extern void adpcmdecomp( struct soundbuf *sb);					  
extern int startWaveInput(HWND hwnd);
extern int inputSampleCount(void);
extern void terminateWaveInput(void);
extern void compress2X(soundbuf *asb);
extern void createSoundBuffer(LPSTR buffer, WORD buflen, DWORD channels,
							  DWORD rate, DWORD bytesec, WORD align);
extern void shipSoundBuffer(HWND hwnd, LPCLIENT_DATA pClientData);							  
extern VOID startSoundFile(HWND hwnd, LPSTR pszFile);
extern LPBYTE GIFtoBMP(LPBYTE gif, int imageNumber);
extern void sessionKeyGenerate(LPSTR key, BOOL binary);
extern int makeInternalEncryptionKeys(HWND hwnd, LPCLIENT_DATA d);
extern void multicastJoin(HWND hwnd, int join);
extern void propeller(int control, DWORD value);
extern int obtainOutput(HWND hwnd);
extern unsigned char alaw2ulaw(unsigned char aval);
#ifdef HEXDUMP
extern void xd(void *buf, int bufl, int dochar);
#endif


// Definitions for different builds of the program (e.g. domestic/exportable versions).

void PonTexto(LPCSTR texto,UINT id);
void SendUdpControl(HWND hwnd, LPCLIENT_DATA pClientData,ULONG flag,void *buffer,int len,unsigned long ip);
void SendDebug(HWND hwnd, LPCLIENT_DATA pClientData,LPCSTR texto);
void SendMensaje(HWND hwnd, LPCLIENT_DATA pClientData,LPCSTR texto,long ip);
BOOL InsertaBmpEnTexto(UINT id);

int InitVolume(DWORD dwComponentType,volumen *pvolumen,int _Enabled,int NotMuted,DWORD subcomponentes);
void EnableLine(DWORD dwLineID , int bEnable, int NotMuted);
DWORD GetCurrentVolumen(volumen *pvolumen);
void SetCurrentVolumen( volumen *pvolumen, DWORD dwValue, DWORD dwMute );
int SelectLine(DWORD dwComponentType, DWORD dwSelComponent,volumen *pvolumen);
void limpia_ultimo_buffer(LPWAVEHDR quitar);
void comprueba_nmezclas();
void desactiva_retraso();
void RecibeInfo(void *info,unsigned long ip);