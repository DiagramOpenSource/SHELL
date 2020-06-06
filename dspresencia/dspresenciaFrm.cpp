// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h".
#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif
#include "wx/wxprec.h"
#include "dspresenciaApp.h"
#include "dspresenciaFrm.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "dspresencia.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(dspresenciaFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  dspresenciaFrame::OnQuit)
    EVT_MENU(Minimal_About, dspresenciaFrame::OnAbout)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
dspresenciaFrame::dspresenciaFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    // set the frame icon
    SetIcon(wxICON(dspresencia));

    // create a menu bar
    wxMenu *menuFile = new wxMenu("", wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About...\tCtrl-A", "Show about dialog");

    menuFile->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Listo");
}


// event handlers

void dspresenciaFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void dspresenciaFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("About dspresencia...\n"));
    wxMessageBox(msg, "About dspresencia", wxOK | wxICON_INFORMATION, this);
}
