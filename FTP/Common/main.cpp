/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/listctrl.h>
#include <wx/protocol/ftp.h>
#include <wx/thread.h>
#include <wx/wfstream.h>

#include "main.h"

const wxString appName = wxT("FTP Handling Test 1.0.0");

//---------------------------------------------------------------------------
// Application implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// Identifiers
//---------------------------------------------------------------------------
enum
{
    ID_CLOSE = wxID_HIGHEST + 1,
    ID_ABOUT_PROGRAM,
    //
    ID_CONNECT,
    ID_DISCONNECT,
    ID_MKDIR,
    ID_RMDIR,
    ID_RMFILE,
    ID_REFRESH,
    ID_DOWNLOAD,
    ID_UPLOAD,
    ID_LIST
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_CLOSE, MyFrame::OnClose)
    EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
    EVT_CLOSE(MyFrame::CloseApp)
    //
    EVT_BUTTON(ID_CONNECT, MyFrame::OnConnect)
    EVT_BUTTON(ID_DISCONNECT, MyFrame::OnDisconnect)
    EVT_MENU(ID_MKDIR, MyFrame::OnMkDir)
    EVT_MENU(ID_RMDIR, MyFrame::OnRmDir)
    EVT_MENU(ID_RMFILE, MyFrame::OnRmFile)
    EVT_MENU(ID_REFRESH, MyFrame::OnRefresh)
    EVT_BUTTON(ID_DOWNLOAD, MyFrame::OnDownload)
    EVT_BUTTON(ID_UPLOAD, MyFrame::OnUpload)
    //
    EVT_LIST_ITEM_ACTIVATED(ID_LIST, MyFrame::OnListActivated)
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600,400), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// OnExit()
//---------------------------------------------------------------------------
int MyApp::OnExit()
{
    for (wxThread* th : wxGetApp().thread_pool)
    {
        th->Wait();
    }

    thread_pool.Clear();
    return wxApp::OnExit();
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
    wxMenuBar* mb = new wxMenuBar();

    wxMenu* mFile = new wxMenu();
    wxMenu* mFTP = new wxMenu();
    wxMenu* mHelp = new wxMenu();

    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close program\tCtrl-X"), wxT("Closes the program"));

    mFTP->Append(ID_MKDIR, wxT("Create directory"), wxT("Creates a new directory on FTP"));
    mFTP->Append(ID_RMDIR, wxT("Delete directory"), wxT("Deletes a directory from FTP"));
    mFTP->Append(ID_RMFILE, wxT("Delete file"), wxT("Deletes a file on FTP"));
    mFTP->AppendSeparator();
    mFTP->Append(ID_REFRESH, wxT("Refresh\tF5"));

    mHelp->Append(ID_ABOUT_PROGRAM, wxT("&About\tF1"), wxT("About the program"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mFTP, wxT("&FTP"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    wxStatusBar* sb = new wxStatusBar(this);
    sb->SetFieldsCount(2);
    this->SetStatusBar(sb);
    SetStatusText(title, 0);

    wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(ms);

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* ps = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(ps);

    address = new wxTextCtrl(panel, wxID_ANY, wxT("127.0.0.1"));
    port = new wxTextCtrl(panel, wxID_ANY, wxT("21"));
    user = new wxTextCtrl(panel, wxID_ANY, wxT("test"));
    passwd = new wxTextCtrl(panel, wxID_ANY, wxT("test_user@2025"), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    log->SetMinSize(wxSize(-1, 100));

    wxLog::SetActiveTarget(new  wxLogTextCtrl(log));

    list = new wxListView(panel, ID_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    list->SetMinSize(wxSize(-1, 250));
    list->AppendColumn(wxT("Directory/file"), wxLIST_FORMAT_LEFT, 450);
    list->AppendColumn(wxT("Size"));

    conn_btn = new wxButton(panel, ID_CONNECT, wxT("Connect"));
    dwn_btn = new wxButton(panel, ID_DOWNLOAD, wxT("Download"));
    up_btn = new wxButton(panel, ID_UPLOAD, wxT("Upload"));

    wxStaticText* address_label = new wxStaticText(panel, wxID_ANY, wxT("Address:"));
    wxStaticText* port_label = new wxStaticText(panel, wxID_ANY, wxT("Port:"));
    wxStaticText* user_label = new wxStaticText(panel, wxID_ANY, wxT("User:"));
    wxStaticText* passwd_label = new wxStaticText(panel, wxID_ANY, wxT("Password:"));
    
    //
    wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
    row->Add(address_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
    row->Add(address, 0, wxALL, 4);
    row->Add(port_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
    row->Add(port, 0, wxALL, 4);
    row->Add(user_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
    row->Add(user, 0, wxALL, 4);
    row->Add(passwd_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
    row->Add(passwd, 0, wxALL, 4);

    wxBoxSizer* row2 = new wxBoxSizer(wxHORIZONTAL);
    row2->Add(conn_btn, 1, wxEXPAND | wxALL, 4);
    row2->Add(dwn_btn, 0, wxALL, 4);
    row2->Add(up_btn, 0, wxALL, 4);

    ps->Add(row, 0, wxEXPAND | wxALL, 0);
    ps->Add(row2, 0, wxEXPAND | wxALL, 0);
    ps->Add(list, 1, wxEXPAND | wxALL, 0);
    ps->Add(log, 0, wxEXPAND | wxTOP, 4);

    ms->Add(panel, 1, wxEXPAND | wxALL, 0);

    Fit();
    CenterOnScreen();
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
    
}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About the program..." window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(appName+wxT("\nCopyright (c) 2018 by ..."), wxT("About the program..."), wxOK|wxICON_INFORMATION);
}

//---------------------------------------------------------------------------
// Function displays a question about closing the program
//---------------------------------------------------------------------------
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
    int action = wxMessageBox(wxT("Do you want to close the program?"), wxT("Question"), wxOK|wxCANCEL|wxICON_QUESTION);

    if(action == wxOK)
    {
        this->Destroy();
    }
    else
    {
        return;
    }
}

//---------------------------------------------------------------------------
// Function performs FTP connection
//---------------------------------------------------------------------------
void MyFrame::OnConnect(wxCommandEvent& WXUNUSED(event))
{
    if (address->GetValue().IsEmpty() ||
        port->GetValue().IsEmpty() ||
        user->GetValue().IsEmpty() ||
        passwd->GetValue().IsEmpty())
    {
        wxMessageBox(wxT("Fill in the data!"), wxT("Attention!"));
        return;
    }

    ftp.SetUser(user->GetValue());
    ftp.SetPassword(passwd->GetValue());

    long port_tmp = 21;
    port->GetValue().ToLong(&port_tmp);
    if (ftp.Connect(address->GetValue(), (unsigned short)port_tmp))
    {
        wxLogMessage(wxT("Connected to %s:%d"), address->GetValue(), port_tmp);
        conn_btn->SetLabel(wxT("Disconnect"));
        conn_btn->SetId(ID_DISCONNECT);

        wxLogMessage(wxT("Remote folder: ") + ftp.Pwd());

        RefreshList();
    }
    else
    {
        wxLogWarning(wxT("Connection not established!"));
    }

    SetFTPStatusInfo();
}
//---------------------------------------------------------------------------
// Function closes the FTP connection
//---------------------------------------------------------------------------
void MyFrame::OnDisconnect(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.Close())
    {
        wxLogMessage(wxT("Successfully disconnected"));
        conn_btn->SetLabel(wxT("Connect"));
        conn_btn->SetId(ID_CONNECT);
    }
    else
    {
        wxLogWarning(wxT("There were problems closing the connection!"));
    }

    SetFTPStatusInfo();
}

//---------------------------------------------------------------------------
// Function creates an FTP directory
//---------------------------------------------------------------------------
void MyFrame::OnMkDir(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.IsDisconnected())
    {
        wxLogWarning("Operation not possible. FTP client not connected!");
        return;
    }

    wxString dir = wxGetTextFromUser(wxT("Enter the name of the new directory"), wxT("New FTP directory"));
    if (dir.IsEmpty()) return;

    if (ftp.MkDir(dir))
    {
        wxLogMessage(wxT("Successfully created directory: %s"), dir);
        RefreshList();
    }
    else
    {
        wxLogWarning(wxT("Creating directory '%s' failed!"), dir);
    }

    SetFTPStatusInfo();
}

//---------------------------------------------------------------------------
// Function deletes an FTP directory
//---------------------------------------------------------------------------
void MyFrame::OnRmDir(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.IsDisconnected())
    {
        wxLogWarning("Operation not possible. FTP client not connected!");
        return;
    }

    int index = list->GetFirstSelected();
    if (index < 1) return;

    if (wxMessageBox(wxT("Do you really want to delete the directory?"), wxT("Question"), wxYES_NO | wxICON_QUESTION) == wxYES)
    {
        wxString dir = list->GetItemText(index);
        if (ftp.RmDir(dir))
        {
            wxLogMessage(wxT("Successfully deleted directory: %s"), dir);
            RefreshList();
        }
        else
        {
            wxLogWarning(wxT("Deleting directory '%s' failed!"), dir);
        }
    }

    SetFTPStatusInfo();
}

//---------------------------------------------------------------------------
// Function deletes a file in the remote directory
//---------------------------------------------------------------------------
void MyFrame::OnRmFile(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.IsDisconnected())
    {
        wxLogWarning("Operation not possible. FTP client not connected!");
        return;
    }

    int index = list->GetFirstSelected();
    if (index < 1) return;

    if (wxMessageBox(wxT("Do you really want to delete the file?"), wxT("Question"), wxYES_NO | wxICON_QUESTION) == wxYES)
    {
        wxString file = list->GetItemText(index);
        if (ftp.RmFile(file))
        {
            wxLogMessage(wxT("Successfully deleted file: %s"), file);
            RefreshList();
        }
        else
        {
            wxLogWarning(wxT("Deleting file '%s' failed!"), file);
        }
    }

    SetFTPStatusInfo();
}

//---------------------------------------------------------------------------
// Function initiates file list refresh
//---------------------------------------------------------------------------
void MyFrame::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.IsDisconnected())
    {
        wxLogWarning("Operation not possible. FTP client not connected!");
        return;
    }

    RefreshList();
}

//---------------------------------------------------------------------------
// Function initiates the file download thread
//---------------------------------------------------------------------------
void MyFrame::OnDownload(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.IsDisconnected())
    {
        wxLogWarning("Operation not possible. FTP client not connected!");
        return;
    }

    int index = list->GetFirstSelected();
    if (index < 1) return;

    wxString dest = wxDirSelector(wxT("Where to save the downloaded data?"), wxEmptyString, wxFD_SAVE);
    if (dest.IsEmpty())  return;

    wxCriticalSectionLocker locker(wxGetApp().cs);

    wxString path = ftp.Pwd() + wxT("/") + list->GetItemText(index);
    DownloadThread* th = new DownloadThread(&ftp, path, dest);
    if (th->Create() == wxTHREAD_NO_ERROR)
    {
        wxGetApp().thread_pool.Add(th);
        th->Run();
    }
    else
    {
        wxLogWarning(wxT("Failed to create new download thread!"));
    }
}

//---------------------------------------------------------------------------
// Function initiates the file upload thread
//---------------------------------------------------------------------------
void MyFrame::OnUpload(wxCommandEvent& WXUNUSED(event))
{
    if (ftp.IsDisconnected())
    {
        wxLogWarning("Operation not possible. FTP client not connected!");
        return;
    }

    wxString file = wxFileSelector(wxT("Where to save the downloaded data?"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN);
    if (file.IsEmpty())  return;

    wxCriticalSectionLocker locker(wxGetApp().cs);

    UploadThread* th = new UploadThread(&ftp, file);
    if (th->Create() == wxTHREAD_NO_ERROR)
    {
        wxGetApp().thread_pool.Add(th);
        th->Run();
    }
    else
    {
        wxLogWarning(wxT("Failed to create new upload thread!"));
    }
}

//---------------------------------------------------------------------------
// Function handles navigation between FTP directories
//---------------------------------------------------------------------------
void MyFrame::OnListActivated(wxListEvent& event)
{
    int index = event.GetIndex();
    
    wxString tmp_path = ftp.Pwd() + wxT("/") + list->GetItemText(index);
    if (ftp.ChDir(tmp_path))
    {
        wxLogMessage(wxT("Changed directory to: %s"), tmp_path);

        RefreshList();
    }
}
