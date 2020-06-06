// Define a new frame type: this is going to be our main frame
class dspresenciaFrame : public wxFrame
{
public:
    // ctor(s)
    dspresenciaFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};
