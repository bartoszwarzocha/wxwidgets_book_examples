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

#include <wx/dialog.h>
#include <wx/statbox.h>
#include <wx/sstream.h>
#include <wx/wfstream.h>
#include <wx/numdlg.h>

#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/ffile.h>
#include <wx/filename.h>
#include <wx/fs_arc.h>
#include <wx/fswatcher.h>
#include <wx/zipstrm.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>

#include "main.h"

// Making work easier :)
#define wxUnusedEvent wxCommandEvent& WXUNUSED(event)

const wxString appName = wxT("Files and File System 1.0.0");

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
    ID_WXSTANDARDPATHS,
    //
    ID_WXMKDIR,
    ID_WXRMDIR,
    ID_WXGETCWD,
    ID_WXSETWD,
    ID_WXGETWD,
    //
    ID_WXCOPYFILE,
    ID_WXRENAMEFILE,
    ID_WXREMOVEFILE,
    ID_WXCONCATFILES,
    ID_WXFINDFIRSTNEXTFILE,
    ID_WXFILEMODIFICATIONTIME,
    //
    ID_WXGETDISKSPACE,
    //
    ID_WXDIRTESTS,
    ID_WXDIRLISTLOOP,
    ID_WXDIRLISTTRAVERSAL,
    ID_WXDIRLISTGETALL,
    //
    ID_WXFILE,
    ID_WXFFILE,
    //
    ID_WXFILENAME,
    ID_WXFILENAMEANALYSIS,
    ID_WXFILENAMEEDIT,
    ID_WXFILENAMEEDIT2,
    ID_WXFILENAMEEDIT3,
    ID_WXFILENAMEEDIT4,
    ID_WXFILENAMETIME,
    ID_WXFILENAMEPERMISSIONS,
    ID_WXFILENAMESIZE,
    ID_WXFILENAMETEMP,
    ID_WXFILENAMETEST,
    //
    ID_WXARCHIVEFSHANDLER,
    ID_WXARCHIVEFSHANDLER2,
    ID_WXZIPSTREAMCREATE,
    ID_WXZIPSTERAMEXTRACT,
    //
    ID_WXTEXTFILEANALYSIS,
    ID_WXTEXTFILEADDLINE,
    ID_WXTEXTFILEGETLINE,
    ID_WXTEXTFILECREATE,
    //
    ID_WXFSWATCHERCWD,
    ID_WXFSWATCHERADDDIR,
    ID_WXFSWATCHERADDFILE,
    ID_WXFSWATCHERDELDIR,
    ID_WXFSWATCHERDELFILE,
    ID_WXFSWATCHERDELALL,
};

//---------------------------------------------------------------------------
// Event table
//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_CLOSE, MyFrame::OnClose)
    EVT_MENU(ID_ABOUT_PROGRAM, MyFrame::OnAboutProgram)
    EVT_CLOSE(MyFrame::CloseApp)
    //
    EVT_MENU(ID_WXSTANDARDPATHS, MyFrame::On_wxStandardPaths)
    //
    EVT_MENU(ID_WXMKDIR, MyFrame::On_wxMkdir)
    EVT_MENU(ID_WXRMDIR, MyFrame::On_wxRmdir)
    EVT_MENU(ID_WXGETCWD, MyFrame::On_wxGetCwd)
    EVT_MENU(ID_WXSETWD, MyFrame::On_wxSetWorkingDirectory)
    //
    EVT_MENU(ID_WXCOPYFILE, MyFrame::On_wxCopyFile)
    EVT_MENU(ID_WXRENAMEFILE, MyFrame::On_wxRenameFile)
    EVT_MENU(ID_WXREMOVEFILE, MyFrame::On_wxRemoveFile)
    EVT_MENU(ID_WXCONCATFILES, MyFrame::On_wxConcatFiles)
    EVT_MENU(ID_WXFINDFIRSTNEXTFILE, MyFrame::On_wxFindFirstNextFile)
    EVT_MENU(ID_WXFILEMODIFICATIONTIME, MyFrame::On_wxFileModificationTime)
    //
    EVT_MENU(ID_WXGETDISKSPACE, MyFrame::On_wxGetDiskSpace)
    //
    EVT_MENU(ID_WXDIRTESTS, MyFrame::On_wxDirTests)
    EVT_MENU(ID_WXDIRLISTLOOP, MyFrame::On_wxDirListLoop)
    EVT_MENU(ID_WXDIRLISTTRAVERSAL, MyFrame::On_wxDirListTraversal)
    EVT_MENU(ID_WXDIRLISTGETALL, MyFrame::On_wxDirListGetAll)
    //
    EVT_MENU(ID_WXFILE, MyFrame::On_wxFile)
    EVT_MENU(ID_WXFFILE, MyFrame::On_wxFFile)
    //
    EVT_MENU(ID_WXFILENAME, MyFrame::On_wxFileName)
    EVT_MENU(ID_WXFILENAMEANALYSIS, MyFrame::On_wxFileNameAnalysis)
    EVT_MENU(ID_WXFILENAMEEDIT, MyFrame::On_wxFileNameEdit)
    EVT_MENU(ID_WXFILENAMEEDIT2, MyFrame::On_wxFileNameEdit2)
    EVT_MENU(ID_WXFILENAMEEDIT3, MyFrame::On_wxFileNameEdit3)
    EVT_MENU(ID_WXFILENAMEEDIT4, MyFrame::On_wxFileNameEdit4)
    EVT_MENU(ID_WXFILENAMETIME, MyFrame::On_wxFileNameTime)
    EVT_MENU(ID_WXFILENAMEPERMISSIONS, MyFrame::On_wxFileNamePermissions)
    EVT_MENU(ID_WXFILENAMESIZE, MyFrame::On_wxFileNameSize)
    EVT_MENU(ID_WXFILENAMETEMP, MyFrame::On_wxFileNameTemp)
    EVT_MENU(ID_WXFILENAMETEST, MyFrame::On_wxFileNameTest)
    //
    EVT_MENU(ID_WXARCHIVEFSHANDLER, MyFrame::On_wxArchiveFSHandler)
    EVT_MENU(ID_WXARCHIVEFSHANDLER2, MyFrame::On_wxArchiveFSHandler2)
    EVT_MENU(ID_WXZIPSTREAMCREATE, MyFrame::On_wxZipStreamCreate)
    EVT_MENU(ID_WXZIPSTERAMEXTRACT, MyFrame::On_wxZipStreamExtract)
    //
    EVT_MENU(ID_WXTEXTFILEANALYSIS, MyFrame::On_wxTextFileAnalysis)
    EVT_MENU(ID_WXTEXTFILEADDLINE, MyFrame::On_wxTextFileAddLine)
    EVT_MENU(ID_WXTEXTFILEGETLINE, MyFrame::On_wxTextFileGetLine)
    EVT_MENU(ID_WXTEXTFILECREATE, MyFrame::On_wxTextFileCreate)
    //
    EVT_MENU(ID_WXFSWATCHERCWD, MyFrame::On_wxFileSystemWatcherCwd)
    EVT_MENU(ID_WXFSWATCHERADDDIR, MyFrame::On_wxFileSystemWatcherAddDir)
    EVT_MENU(ID_WXFSWATCHERADDFILE, MyFrame::On_wxFileSystemWatcherAddFile)
    EVT_MENU(ID_WXFSWATCHERDELDIR, MyFrame::On_wxFileSystemWatcherDelDir)
    EVT_MENU(ID_WXFSWATCHERDELFILE, MyFrame::On_wxFileSystemWatcherDelFile)
    EVT_MENU(ID_WXFSWATCHERDELALL, MyFrame::On_wxFileSystemWatcherDelAll)
    //
END_EVENT_TABLE()

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    // Initializing the handler for archive support
    //wxFileSystem::AddHandler(new wxArchiveFSHandler);

    MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(700,500), wxDEFAULT_FRAME_STYLE);
    frame->Show(true);
    return true;
}

//---------------------------------------------------------------------------
// NormalizeDialog constructor
//---------------------------------------------------------------------------
NormalizeDialog::NormalizeDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, wxT("Normalization"))
{
    wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(ms);

    path = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

    wxArrayString choices;
    choices.Add(wxT("wxPATH_NATIVE"));
    choices.Add(wxT("wxPATH_UNIX"));
    choices.Add(wxT("wxPATH_WIN"));
    format = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_READONLY);
    format->SetSelection(0);

    wxStaticBox* sb = new wxStaticBox(this, wxID_ANY, wxT(" Flags "));
    wxStaticBoxSizer* sbs = new wxStaticBoxSizer(sb, wxVERTICAL);

    norm_env_vars = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_ENV_VARS"));
    norm_dots = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_DOTS"));
    norm_tilde = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_TILDE (Linux only)"));
    norm_case = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_CASE"));
    norm_absolute = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_ABSOLUTE"));
    norm_long = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_LONG (Windows only)"));
    norm_shortcut = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_SHORTCUT (Windows only)"));
    norm_all = new wxCheckBox(this, wxID_ANY, wxT(" wxPATH_NORM_ALL (all flags except CASE)"));
    norm_all->SetValue(true);

    //
    sbs->Add(norm_env_vars, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_dots, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_tilde, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_case, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_absolute, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_long, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_shortcut, 0, wxEXPAND | wxALL, 4);
    sbs->Add(norm_all, 0, wxEXPAND | wxALL, 4);

    wxSizer* row1 = this->CreateTextSizer(wxT("Path"));
    row1->Add(path, 0, wxEXPAND | wxALL, 4);
    ms->Add(row1, 0, wxEXPAND | wxALL, 4);

    wxSizer* row2 = this->CreateTextSizer(wxT("Format"));
    row2->Add(format, 0, wxEXPAND | wxALL, 4);
    ms->Add(row2, 0, wxEXPAND | wxALL, 4);

    ms->Add(sbs, 1, wxEXPAND | wxALL, 4);

    ms->Add(this->CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 4);

    Fit();
}

//---------------------------------------------------------------------------
// PermissionDialog constructor
//---------------------------------------------------------------------------
PermissionDialog::PermissionDialog(wxWindow* parent, const wxString& file)
    : wxDialog(parent, wxID_ANY, wxT("File Permissions"))
{
    wxBoxSizer* ms = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(ms);

    wxStaticText* label = new wxStaticText(this, wxID_ANY, wxT("File: ") + file);

    wxFlexGridSizer* gs = new wxFlexGridSizer(4, 4, 10, 10);

    urun = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    uread = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    uwrite = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    grun = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    gread = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    gwrite = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    orun = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    oread = new wxCheckBox(this, wxID_ANY, wxEmptyString);
    owrite = new wxCheckBox(this, wxID_ANY, wxEmptyString);

    //
    gs->Add(new wxStaticText(this, wxID_ANY, wxT("")));
    gs->Add(new wxStaticText(this, wxID_ANY, wxT("Read")));
    gs->Add(new wxStaticText(this, wxID_ANY, wxT("Write")));
    gs->Add(new wxStaticText(this, wxID_ANY, wxT("Execute")));

    gs->Add(new wxStaticText(this, wxID_ANY, wxT("User")));
    gs->Add(uread);
    gs->Add(uwrite);
    gs->Add(urun);

    gs->Add(new wxStaticText(this, wxID_ANY, wxT("Group")));
    gs->Add(gread);
    gs->Add(gwrite);
    gs->Add(grun);

    gs->Add(new wxStaticText(this, wxID_ANY, wxT("Others")));
    gs->Add(oread);
    gs->Add(owrite);
    gs->Add(orun);

    ms->Add(label, 0, wxEXPAND | wxALL, 4);
    ms->Add(gs, 0, wxEXPAND | wxALL, 4);
    ms->Add(this->CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 4);

    Fit();
}

//---------------------------------------------------------------------------
// MyFrame constructor
//---------------------------------------------------------------------------
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
    mb = new wxMenuBar();

    mFile = new wxMenu();
    mHelp = new wxMenu();

    mFnc = new wxMenu();
    mDir = new wxMenu();
    mFil = new wxMenu();
    mFin = new wxMenu();
    mZip = new wxMenu();
    mWat = new wxMenu();
    mTxt = new wxMenu();

    mFile->AppendSeparator();
    mFile->Append(ID_CLOSE, wxT("&Close Program\tCtrl-X"), wxT("Closes the program"));

    mFnc->Append(ID_WXMKDIR, wxT("Function wxMkdir()"), wxT("Create directory"));
    mFnc->Append(ID_WXRMDIR, wxT("Function wxRmdir()"), wxT("Remove directory"));
    mFnc->Append(ID_WXGETCWD, wxT("Function wxGetCwd()"), wxT("Current working directory"));
    mFnc->Append(ID_WXSETWD, wxT("Function wxSetWorkingDirectory()"), wxT("Set working directory"));
    mFnc->AppendSeparator();
    mFnc->Append(ID_WXCOPYFILE, wxT("Function wxCopyFile()"), wxT("Copy file"));
    mFnc->Append(ID_WXRENAMEFILE, wxT("Function wxRenameFile()"), wxT("Rename file"));
    mFnc->Append(ID_WXREMOVEFILE, wxT("Function wxRemoveFile()"), wxT("Remove file"));
    mFnc->Append(ID_WXCONCATFILES, wxT("Function wxConcatFiles()"), wxT("Concatenate files"));
    mFnc->Append(ID_WXFINDFIRSTNEXTFILE, wxT("Functions wxFindFirstFile() and wxFindNextFile()"), wxT("Loop through files"));
    mFnc->Append(ID_WXFILEMODIFICATIONTIME, wxT("Function wxFileModificationTime()"), wxT("File modification time"));
    mFnc->AppendSeparator();
    mFnc->Append(ID_WXGETDISKSPACE, wxT("Function wxGetDiskSpace()"), wxT("Calculate free and used disk space"));

    mDir->Append(ID_WXDIRTESTS, wxT("Location testing"), wxT("Location testing"));
    mDir->Append(ID_WXDIRLISTLOOP, wxT("Listing (loop)"), wxT("Listing (loop)"));
    mDir->Append(ID_WXDIRLISTTRAVERSAL, wxT("Listing (wxTraverser)"), wxT("Listing (wxTraverser)"));
    mDir->Append(ID_WXDIRLISTGETALL, wxT("Listing (GetAllFiles())"), wxT("Listing (GetAllFiles())"));

    mFil->Append(ID_WXFILE, wxT("Class wxFile"), wxT("Class wxFile"));
    mFil->Append(ID_WXFFILE, wxT("Class wxFFile"), wxT("Class wxFFile"));

    mFin->Append(ID_WXFILENAME, wxT("Initializing wxFileName objects"), wxT("Class wxFileName"));
    mFin->Append(ID_WXFILENAMEANALYSIS, wxT("Path analysis"), wxT("Path analysis wxFileName"));
    mFin->Append(ID_WXFILENAMEEDIT, wxT("Edit disk name, path, file name, and extension"), wxT("Edit wxFileName"));
    mFin->Append(ID_WXFILENAMEEDIT2, wxT("Edit directory hierarchy"), wxT("Edit wxFileName"));
    mFin->Append(ID_WXFILENAMEEDIT3, wxT("Relative and absolute paths"), wxT("Edit wxFileName"));
    mFin->Append(ID_WXFILENAMEEDIT4, wxT("Path normalization"), wxT("Edit wxFileName"));
    mFin->Append(ID_WXFILENAMETIME, wxT("File modification time"), wxT("File modification time"));
    mFin->Append(ID_WXFILENAMEPERMISSIONS, wxT("File/directory access permissions"), wxT("Set access permissions"));
    mFin->Append(ID_WXFILENAMESIZE, wxT("File size"), wxT("Read and display file size"));
    mFin->Append(ID_WXFILENAMETEMP, wxT("Temporary files"), wxT("Temporary files"));
    mFin->Append(ID_WXFILENAMETEST, wxT("Testing wxFileName objects and files"), wxT("Testing wxFileName objects and files"));

    mZip->Append(ID_WXARCHIVEFSHANDLER, wxT("wxArchiveFSHandler (listing and reading)"), wxT("Listing and preview using virtual file system"));
    mZip->Append(ID_WXARCHIVEFSHANDLER2, wxT("wxArchiveFSHandler (unpacking archive)"), wxT("Unpacking using virtual file system"));
    mZip->AppendSeparator();
    mZip->Append(ID_WXZIPSTREAMCREATE, wxT("wxArchiveOutputStream (create archive)"), wxT("Create archive from folder"));
    mZip->Append(ID_WXZIPSTERAMEXTRACT, wxT("wxArchiveInputStream (unpack archive)"), wxT("Unpack using input stream"));

    mTxt->Append(ID_WXTEXTFILEANALYSIS, wxT("Analyze file"), wxT("File analysis"));
    mTxt->Append(ID_WXTEXTFILEADDLINE, wxT("Add line to existing file"), wxT("Add line to selected file"));
    mTxt->Append(ID_WXTEXTFILEGETLINE, wxT("Get line from file"), wxT("Get selected line from file"));
    mTxt->AppendSeparator();
    mTxt->Append(ID_WXTEXTFILECREATE, wxT("Create new sample file"), wxT("Creates a new file in the working directory"));

    mWat->Append(ID_WXFSWATCHERCWD, wxT("Monitor working directory"), wxT("Monitor working directory"), wxITEM_CHECK);
    mWat->AppendSeparator();
    mWat->Append(ID_WXFSWATCHERADDDIR, wxT("Add directory to monitor"), wxT("Monitor directory"));
    mWat->Append(ID_WXFSWATCHERADDFILE, wxT("Add file to monitor"), wxT("Monitor file"));
    mWat->AppendSeparator();
    mWat->Append(ID_WXFSWATCHERDELDIR, wxT("Stop monitoring directory"), wxT("Stop monitoring directory"));
    mWat->Append(ID_WXFSWATCHERDELFILE, wxT("Stop monitoring file"), wxT("Stop monitoring file"));
    mWat->Append(ID_WXFSWATCHERDELALL, wxT("Stop all"), wxT("Stop monitoring all items"));

    mWat->Enable(ID_WXFSWATCHERADDFILE, false); // Monitoring individual files is not yet available!
                                                // Remove this line when you compile the program with a newer version of the library.

    mHelp->Append(ID_WXSTANDARDPATHS, wxT("Standard paths"), wxT("Standard paths"));
    mHelp->AppendSeparator();
    mHelp->Append(ID_ABOUT_PROGRAM, wxT("About\tF1"), wxT("About"));

    mb->Append(mFile, wxT("&File"));
    mb->Append(mFnc, wxT("&Functions"));
    mb->Append(mDir, wxT("wx&Dir"));
    mb->Append(mFil, wxT("Fi&les"));
    mb->Append(mFin, wxT("wxFile&Name"));
    mb->Append(mZip, wxT("&Archives"));
    mb->Append(mWat, wxT("&Change Monitoring"));
    mb->Append(mTxt, wxT("wxTe&xtFile"));
    mb->Append(mHelp, wxT("&Help"));

    this->SetMenuBar(mb);

    sb = new wxStatusBar(this);
    this->SetStatusBar(sb);
    SetStatusText(title);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(main_sizer);

    panel = new wxPanel(this);
    wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(panel_sizer);

    main_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);

    wxTextCtrl* log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    panel_sizer->Add(log, 1, wxEXPAND | wxALL, 0);
    wxLog::SetActiveTarget(new wxLogTextCtrl(log));

    wxString msg = wxT("Warning!\nSome functions in the program may permanently damage " \
        "the file system of your operating system, so it is recommended to test them only " \
        "within the working directory of this application.");
    wxLogMessage(msg);
    wxMessageBox(msg);

    CenterOnScreen();

    // File system monitoring
    watcher = new wxFileSystemWatcher();
    watcher->SetOwner(this);
    Connect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(MyFrame::On_wxFileSystemWatcher));
}

//---------------------------------------------------------------------------
// MyFrame destructor
//-------------------------------------------------------------------------
MyFrame::~MyFrame()
{
    wxDELETE(watcher);
}

//---------------------------------------------------------------------------
// Function initiates program closure
//---------------------------------------------------------------------------
void MyFrame::OnClose(wxUnusedEvent)
{
    this->Close(true);
}

//---------------------------------------------------------------------------
// Function displays "About" window
//---------------------------------------------------------------------------
void MyFrame::OnAboutProgram(wxCommandEvent& event)
{
    wxMessageBox(appName+wxT("\nCopyright (c) 2025 by ..."), wxT("About..."), wxOK|wxICON_INFORMATION);

    wxLogMessage(
        wxT("\nSystem paths:\nGetAppDocumentsDir(): " + wxStandardPaths::Get().GetAppDocumentsDir() + "\n")
        + wxT("GetConfigDir(): " + wxStandardPaths::Get().GetConfigDir() + "\n")
        + wxT("GetDataDir(): " + wxStandardPaths::Get().GetDataDir() + "\n")
        + wxT("GetDocumentsDir(): " + wxStandardPaths::Get().GetDocumentsDir() + "\n")
        + wxT("GetExecutablePath(): " + wxStandardPaths::Get().GetExecutablePath() + "\n")
        #if defined(__WXGTK__)
        + wxT("GetInstallPrefix(): " + wxStandardPaths::Get().GetInstallPrefix() + "\n")
        #endif
        + wxT("GetLocalDataDir(): " + wxStandardPaths::Get().GetLocalDataDir() + "\n")
        + wxT("GetPluginsDir(): " + wxStandardPaths::Get().GetPluginsDir() + "\n")
        + wxT("GetResourcesDir(): "+wxStandardPaths::Get().GetResourcesDir() + "\n")
        + wxT("GetTempDir(): "+wxStandardPaths::Get().GetTempDir() + "\n")
        + wxT("GetUserConfigDir(): "+wxStandardPaths::Get().GetUserConfigDir() + "\n")
        + wxT("GetUserDataDir(): "+wxStandardPaths::Get().GetUserDataDir() + "\n")
        + wxT("GetUserLocalDataDir(): "+wxStandardPaths::Get().GetUserLocalDataDir() + "\n")
    );

    event.Skip();
}

//---------------------------------------------------------------------------
// Function displays a prompt to close the program
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
// 
//---------------------------------------------------------------------------

void MyFrame::On_wxStandardPaths(wxUnusedEvent)
{
    wxLogMessage(wxT("Standard paths:"));

    // Folder documents used by the application
    wxString p1 = wxStandardPaths::Get().GetAppDocumentsDir();

    // Folder containing system configuration files
    // or application configuration files
    wxString p2 = wxStandardPaths::Get().GetConfigDir();

    // Folder of global application files, independent of the user
    wxString p3 = wxStandardPaths::Get().GetDataDir();

    // Documents/My Documents folder
    wxString p4 = wxStandardPaths::Get().GetDocumentsDir();

    // Full path to the application's executable file
    wxString p5 = wxStandardPaths::Get().GetExecutablePath();

    // Folder of application files related to a specific device
    wxString p6 = wxStandardPaths::Get().GetLocalDataDir();

    // Folder of plugins/dynamically loaded libraries
    wxString p7 = wxStandardPaths::Get().GetPluginsDir();

    // Application resources folder
    wxString p8 = wxStandardPaths::Get().GetResourcesDir();

    // System temporary files folder
    wxString p9 = wxStandardPaths::Get().GetTempDir();

    // User settings folder
    wxString p10 = wxStandardPaths::Get().GetUserConfigDir();

    // User application data folder
    wxString p11 = wxStandardPaths::Get().GetUserDataDir();

    // User-dependent application data folder
    wxString p12 = wxStandardPaths::Get().GetUserLocalDataDir();


    // User documents folder (Documents/My Documents)
    wxString u1 = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Documents);

    // User desktop folder
    wxString u2 = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Desktop);

    // User downloads folder
    wxString u3 = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Downloads);

    // User music folder
    wxString u4 = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Music);

    // User pictures folder
    wxString u5 = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Pictures);

    // User videos folder
    wxString u6 = wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Videos);


    // No application identification
    wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_None);

    // Application identification by its name
    wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_AppName);

    // Application identification by the name of the application provider
    wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_VendorName);

    // Application identification by its name and the name of its provider
    wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_AppName | wxStandardPaths::AppInfo_VendorName);

#if defined(__WXMSW__)
    // Ignoring selected application folders
    //wxStandardPaths::Get().IgnoreAppSubDir(wxT("bin*vc*debug*"));

    // Ignoring application folders related to its build
    //wxStandardPaths::Get().IgnoreAppBuildSubDirs();
#endif

#if defined(__WXGTK__)
    // Program installation path (usually /home/user)
    wxString prefix = wxStandardPaths::Get().GetInstallPrefix();

    // Setting a custom installation path
    //wxStandardPaths::Get().SetInstallPrefix(wxT("xxx"));
#endif

    // LOG
    wxLogMessage(wxT("GetAppDocumentsDir(): ") + p1);
    wxLogMessage(wxT("GetConfigDir(): ") + p2);
    wxLogMessage(wxT("GetDataDir(): ") + p3);
    wxLogMessage(wxT("GetDocumentsDir(): ") + p4);
    wxLogMessage(wxT("GetExecutablePath(): ") + p5);
    wxLogMessage(wxT("GetLocalDataDir(): ") + p6);
    wxLogMessage(wxT("GetPluginsDir(): ") + p7);
    wxLogMessage(wxT("GetResourcesDir(): ") + p8);
    wxLogMessage(wxT("GetTempDir(): ") + p9);
    wxLogMessage(wxT("GetUserConfigDir(): ") + p10);
    wxLogMessage(wxT("GetUserDataDir(): ") + p11);
    wxLogMessage(wxT("GetUserLocalDataDir(): ") + p12);
    wxLogMessage(wxT("GetUserDir(wxStandardPaths::Dir_Documents): ") + u1);
    wxLogMessage(wxT("GetUserDir(wxStandardPaths::Dir_Desktop): ") + u2);
    wxLogMessage(wxT("GetUserDir(wxStandardPaths::Dir_Downloads): ") + u3);
    wxLogMessage(wxT("GetUserDir(wxStandardPaths::Dir_Music): ") + u4);
    wxLogMessage(wxT("GetUserDir(wxStandardPaths::Dir_Pictures): ") + u5);
    wxLogMessage(wxT("GetUserDir(wxStandardPaths::Dir_Videos): ") + u6);

#if defined(__WXGTK__)
    wxLogMessage(wxT("GetInstallPrefix()") + prefix);
#endif
}

void MyFrame::On_wxMkdir(wxUnusedEvent)
{
    wxString dir = wxGetTextFromUser(wxT("Enter the name of the new directory"), wxT("New folder"));
    if (!dir.empty())
    {
        dir.Trim().Trim(false); // Formatting the name

        if (wxDirExists(dir))
        {
            wxMessageBox(wxT("Folder already exists."), wxT("Warning!"), wxOK | wxICON_EXCLAMATION);
        }
        else
        {
            if (wxMkdir(dir))
            {
                wxLogMessage(wxString::Format(wxT("Successfully created directory %s"), dir));
            }
            else
            {
                wxLogWarning(wxString::Format(wxT("Creating directory %s failed!"), dir));
            }
        }
    }
}

void MyFrame::On_wxRmdir(wxUnusedEvent)
{
    wxString dir = wxDirSelector(wxT("Select directory to delete"), wxEmptyString, wxDD_DIR_MUST_EXIST);
    if (!dir.empty())
    {
        if (wxRmdir(dir))
        {
            wxLogMessage(wxString::Format(wxT("Successfully deleted directory %s"), dir));
        }
        else
        {
            wxLogWarning(wxString::Format(wxT("Failed to delete directory %s"), dir));
        }
    }
}

void MyFrame::On_wxGetCwd(wxUnusedEvent)
{
    wxString cwd = wxGetCwd();
    wxLogMessage(cwd);
    wxMessageBox(cwd);
}

void MyFrame::On_wxSetWorkingDirectory(wxUnusedEvent)
{
    if (wxSetWorkingDirectory(wxGetCwd() + wxT("\\gfx")))
    {
        wxLogMessage(wxT("New working directory: ") + wxGetCwd());
        if (wxSetWorkingDirectory(wxGetCwd() + wxT("\\..")))
        {
            wxLogMessage(wxT("Restored working directory: ") + wxGetCwd());
        }
    }
}

void MyFrame::On_wxCopyFile(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (file.empty()) return;

    wxString dest = wxFileSelector(wxT("Destination file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_SAVE | wxFD_FILE_MUST_EXIST);
    if (dest.empty()) return;

    if (wxCopyFile(file, dest, false))
    {
        wxLogMessage(wxString::Format(wxT("File: %s was copied to: %s"), file, dest));
    }
    else
    {
        wxLogWarning(wxString::Format(wxT("Failed to copy file %s"), file));
    }
}

void MyFrame::On_wxRenameFile(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    wxString file_tmp = wxEmptyString;
    if (!file.empty())
    {
        file_tmp = file + wxT(".tmp");
        wxRenameFile(file, file_tmp, true);
    }
}

void MyFrame::On_wxRemoveFile(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        if (wxRemoveFile(file))
        {
            wxLogMessage(wxString::Format(wxT("File %s was deleted"), file));
        }
        else
        {
            wxLogWarning(wxString::Format(wxT("Failed to delete file %s"), file));
        }
    }
}

void MyFrame::On_wxConcatFiles(wxUnusedEvent)
{
    wxString file1 = wxFileSelector(wxT("Select file 1"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (file1.empty()) return;

    wxString file2 = wxFileSelector(wxT("Select file 2"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_SAVE | wxFD_FILE_MUST_EXIST);
    if (file2.empty()) return;

    wxString dest = wxT("Concatenated_") + wxDateTime::Now().FormatISOCombined('_');

    if (wxConcatFiles(file1, file2, dest))
    {
        wxLogMessage(wxString::Format(wxT("File %s was created"), dest));
    }
    else
    {
        wxLogMessage(wxT("File concatenation failed"));
    }
}

void MyFrame::On_wxFindFirstNextFile(wxUnusedEvent)
{
    wxArrayString choices;
    choices.Add(wxT("*.txt"));
    choices.Add(wxT("*.png"));
    choices.Add(wxT("*.bmp"));
    choices.Add(wxT("*.avi"));
    choices.Add(wxT("*.*"));

    wxString spec = wxEmptyString;
    wxString dir = wxDirSelector(wxT("Select directory to scan files"));
    if (!dir.empty())
    {
        wxString filter = wxGetSingleChoice(wxT("Select file filter"), wxT("Filter"), choices, 4);
        if (!filter.empty())
        {
            wxLogMessage(wxString::Format(wxT("SCANNING DIRECTORY: %s (filter: %s)"), dir, filter));

            // Find the first file
            wxString file = wxFindFirstFile(dir + wxT("\\") + filter);
            while (!file.empty())
            {
                wxLogMessage(wxFileNameFromPath(file)); // Log the file name
                file = wxFindNextFile(); // Find the next file
            }

            wxLogMessage(wxT(">>> End of file search >>>"));
        }
    }
}

void MyFrame::On_wxFileModificationTime(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        wxDateTime dt(wxFileModificationTime(file));
        wxString dt_str = dt.FormatISOCombined(' ');
        wxString msg = wxString::Format(wxT("The last modification date and time of the file %s is: %s"), file, dt_str);

        wxLogMessage(msg);
        wxMessageBox(msg);
    }
}

void MyFrame::On_wxGetDiskSpace(wxUnusedEvent)
{
    wxDiskspaceSize_t total = 0;
    wxDiskspaceSize_t free = 0;
    wxDiskspaceSize_t used = 0;

    wxString dir = wxDirSelector(wxT("Select disk"));
    if (!dir.empty())
    {
        if (wxGetDiskSpace(dir, &total, &free))
        {
            used = total - free;
            wxString msg = wxString::Format(wxT("Disk with location: %s\nTotal size: %lld bytes\nFree: %lld bytes\nUsed: %lld bytes"), dir, total, free, used);
            wxLogMessage(msg);
        }
    }
}

void MyFrame::On_wxDirTests(wxUnusedEvent)
{
    wxString dir = wxDirSelector(wxT("Select location"));
    if (!dir.empty())
    {
        wxDir test;
        if (test.Open(dir))
        {
            wxLogMessage(wxT("Testing location: ") + dir);
            wxLogMessage(wxString::Format(wxT("Contains subdirectories: %s"), (test.HasSubDirs() ? wxT("YES") : wxT("NO"))));
            wxLogMessage(wxString::Format(wxT("Contains files: %s"), (test.HasFiles() ? wxT("YES") : wxT("NO"))));
            wxLogMessage(wxString::Format(wxT("Contains TXT files: %s"), (test.HasFiles(wxT("*.txt")) ? wxT("YES") : wxT("NO"))));
            wxLogMessage(wxString::Format(wxT("Contains JPG files: %s"), (test.HasFiles(wxT("*.jpg")) ? wxT("YES") : wxT("NO"))));
            wxLogMessage(wxString::Format(wxT("Contains PNG files: %s"), (test.HasFiles(wxT("*.png")) ? wxT("YES") : wxT("NO"))));

            wxULongLong size = wxDir::GetTotalSize(dir, NULL);
            wxLogMessage(wxString::Format(wxT("Size: %lld B"), size.GetValue()));

            test.Close();
        }
    }
}

void MyFrame::On_wxDirListLoop(wxUnusedEvent)
{
    wxArrayString choices;
    choices.Add(wxT("*.txt"));
    choices.Add(wxT("*.png"));
    choices.Add(wxT("*.bmp"));
    choices.Add(wxT("*.avi"));
    choices.Add(wxT("*.*"));

    wxString dir = wxDirSelector(wxT("Select location"));
    if (!dir.empty())
    {
        wxString filter = wxGetSingleChoice(wxT("Select file filter"), wxT("Filter"), choices, 4);
        if (!filter.empty())
        {
            wxLogMessage(wxT("Listing (loop):"));

            wxDir source(dir);
            if (source.IsOpened())
            {
                wxString file;
                bool cont = source.GetFirst(&file, filter, wxDIR_DEFAULT | wxDIR_NO_FOLLOW);
                while (cont)
                {
                    wxLogMessage(wxFileNameFromPath(file));
                    // Do something with the file...
                    cont = source.GetNext(&file);
                }

                source.Close();
            }

            wxLogMessage(wxT(">>> End of listing >>>"));
        }
    }
}

void MyFrame::On_wxDirListTraversal(wxUnusedEvent)
{
    wxString dir = wxDirSelector(wxT("Select location"));
    if (!dir.empty())
    {
        wxArrayString results;
        wxArrayString errors;
        MyTraverser traverser(&results, &errors);

        wxDir source(dir);
        if (source.IsOpened())
        {
            source.Traverse(traverser, wxT("*.*"), wxDIR_DEFAULT | wxDIR_NO_FOLLOW);
            source.Close();
        }

        wxLogMessage(wxT("Listing (wxTraverser):"));
        for (wxString s : results) wxLogMessage(s);
        wxLogMessage(wxT("Encountered errors:"));
        for (wxString s : errors) wxLogWarning(s);
        wxLogMessage(wxT(">>> End of listing >>>"));
    }
}

void MyFrame::On_wxDirListGetAll(wxUnusedEvent)
{
    wxString dir = wxDirSelector(wxT("Select location"));
    if (!dir.empty())
    {
        wxArrayString results;
        wxLogMessage(wxT("Listing (GetAllFiles()):"));

        wxDir::GetAllFiles(dir, &results, wxT("*.*"), wxDIR_DEFAULT | wxDIR_NO_FOLLOW);

        for (wxString s : results) wxLogMessage(s);
        wxLogMessage(wxT(">>> End of listing >>>"));
    }
}

void MyFrame::On_wxFile(wxUnusedEvent)
{
    wxMessageBox(wxT("This test will create a file named " \
        "people.dat in the application folder.\nAnalyze its content and log output to " \
        "understand the operation of the wxFile class."));

    typedef struct Person
    {
        char name[50];
        int age;
        char phone[24];
    } PERSON;

    PERSON p1 = { "Jan Kowalski", 25, "123456789" };
    PERSON p2 = { "Janina Nowakówna", 36, "123123123" };
    PERSON p3 = { "Witold Koniecpolski", 31, "999888777" };

    // CREATING FILE AND WRITING DATA

    wxString file_name = wxT("people.dat");
    wxFile file(file_name, wxFile::write); // Creating file and opening it for writing
    if (file.IsOpened()) // Check if the file was opened correctly
    {
        // Writing data structures of a specific size
        file.Write(&p1, sizeof(PERSON));
        file.Write(&p2, sizeof(PERSON));
        file.Write(&p3, sizeof(PERSON));

        // Writing text from wxString variable
        file.Write(wxT("Everything that irritates us about others can lead us to an understanding of ourselves. (Carl Jung)"), wxConvAuto(wxFONTENCODING_ISO8859_2));

        // Checking the amount of data in the file
        wxFileOffset len = file.Length(); // Actually a long long type
        wxLogMessage(wxString::Format(wxT("Bytes written: %lld"), len));

        // Closing the file
        file.Close();
    }
    else
    {
        wxLogMessage(wxT("Failed to create file ") + file_name);
        return;
    }

    // READING SELECTED DATA

    wxFile file2;
    if (file2.Open(file_name, wxFile::read, wxS_DEFAULT))
    {
        // Reading data of the FIRST person
        PERSON p4;
        file2.Read(&p4, sizeof(PERSON));

        wxLogMessage(wxString::Format(wxT("Person 1: %s, age %d"), p4.name, p4.age));

        // Checking the read position
        wxFileOffset offset = file2.Tell();

        wxLogMessage(wxString::Format(wxT("Read position: %lld"), offset));

        // Reading data of the THIRD person
        offset += sizeof(PERSON); // Shifting the read position
        file2.Seek(offset, wxFromStart); // or file2.Seek(sizeof(PERSON), wxFromCurrent);

        wxLogMessage(wxString::Format(wxT("Read position: %lld"), offset));

        PERSON p5;
        file2.Read(&p5, sizeof(PERSON));

        wxLogMessage(wxString::Format(wxT("Person 3: %s, age %d"), p5.name, p5.age));

        // Checking the read position
        offset = file2.Tell(); // Actually a long long type

        wxLogMessage(wxString::Format(wxT("Read position: %lld"), offset));

        // Reading to the end of the file
        char buffer[1024];
        memset(buffer, 0x00, sizeof(buffer));

        while (!file2.Eof())
        {
            file2.Read(&buffer, 128);
        }

        wxLogMessage(wxString::Format(wxT("Remaining data to the end of the file: %s"), buffer));
    }
    else
    {
        wxLogMessage(wxT("Failed to read file ") + file_name);
    }

    // READING THE ENTIRE FILE
    wxFile file3(file_name, wxFile::read_write);
    if (file3.IsOpened())
    {
        wxString buffer;
        file3.ReadAll(&buffer, wxConvAuto(wxFONTENCODING_ISO8859_2));

        wxLogMessage(wxString::Format(wxT("All file data: %s\nData length: %d"), buffer, buffer.Len()));
    }
    else
    {
        wxLogMessage(wxT("Failed to read file ") + file_name);
    }
}

void MyFrame::On_wxFFile(wxUnusedEvent)
{
    wxMessageBox(wxT("This test will create a file named " \
        "books.dat in the application folder.\nAnalyze its content and log output to " \
        "understand the operation of the wxFFile class."));

    typedef struct Book
    {
        char title[70];
        char author[50];
        char printer[30];
        int year;
        int pages;
    } BOOK;

    BOOK b1 = { "In the Land of Thunderstorms", "Jan Bomba", "OKO Publishing", 2017, 322 };
    BOOK b2 = { "Janko the Apprentice", "Henryk Sienkiewski", "Bright Thunder Publishing", 2004, 595 };
    BOOK b3 = { "Selected Poems", "Jacek Placek", "Pixel Publishing", 1975, 36 };
    BOOK b4 = { "Holidays with a Flute", "Paulina Jarzyna", "IKS Publishing", 1998, 438 };

    // CREATING FILE AND WRITING DATA

    wxString file_name = wxT("books.dat");
    wxFFile file(file_name, "w"); // Creating file and opening it for writing
    if (file.IsOpened()) // Check if the file was opened correctly
    {
        // Writing data structures of a specific size
        file.Write(&b1, sizeof(BOOK));
        file.Write(&b2, sizeof(BOOK));
        file.Write(&b3, sizeof(BOOK));
        file.Write(&b4, sizeof(BOOK));

        // Writing text from wxString variable
        file.Write(wxT("When writing, one should not exhaust the subject so that " \
            "nothing is left for the reader. The point is not for people to read, but to think. (Montesquieu)"),
            wxConvAuto(wxFONTENCODING_ISO8859_2));

        // Checking the amount of data in the file
        wxFileOffset len = file.Length(); // Actually a long long type
        wxLogMessage(wxString::Format(wxT("Bytes written: %lld"), len));

        // Closing the file
        file.Close();
    }
    else
    {
        wxLogMessage(wxT("Failed to create file ") + file_name);
        return;
    }

    // READING SELECTED DATA

    wxFFile file2;
    if (file2.Open(file_name, "r"))
    {
        // Reading data of the FIRST book
        BOOK b5;
        file2.Read(&b5, sizeof(BOOK));

        wxLogMessage(wxString::Format(wxT("Book 1: %s, %s"), b5.title, b5.author));

        // Checking the read position
        wxFileOffset offset = file2.Tell();

        wxLogMessage(wxString::Format(wxT("Read position: %lld"), offset));

        // Reading data of the FOURTH book
        file2.Seek(sizeof(BOOK) * 2, wxFromCurrent); // Shifting the pointer

        wxLogMessage(wxString::Format(wxT("Read position: %lld"), offset));

        BOOK b6;
        file2.Read(&b6, sizeof(BOOK));

        wxLogMessage(wxString::Format(wxT("Book 4: %s, %s"), b6.title, b6.author));

        // Checking the read position
        offset = file2.Tell(); // Actually a long long type

        wxLogMessage(wxString::Format(wxT("Read position: %lld"), offset));

        // Reading to the end of the file
        char buffer[1024];
        memset(buffer, 0x00, sizeof(buffer));

        while (!file2.Eof())
        {
            file2.Read(&buffer, sizeof(buffer));
        }

        wxLogMessage(wxString::Format(wxT("Remaining data to the end of the file: %s"), buffer));
    }
    else
    {
        wxLogMessage(wxT("Failed to read file ") + file_name);
    }

    // READING THE ENTIRE FILE
    wxFFile file3(file_name, "r");
    if (file3.IsOpened())
    {
        wxString buffer;
        file3.ReadAll(&buffer, wxConvAuto(wxFONTENCODING_ISO8859_2));

        wxLogMessage(wxString::Format(wxT("All file data: %s\nData length: %d"), buffer, buffer.Len()));
    }
    else
    {
        wxLogMessage(wxT("Failed to read file ") + file_name);
    }
}


void MyFrame::On_wxFileName(wxUnusedEvent)
{
    //wxString file = wxFileSelector(wxT("Select file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);

    wxFileName fn1(wxT("C:\\Windows\\System32\\netapi32.dll"), wxPATH_NATIVE);
    wxFileName fn2(wxT("C:\\Windows\\System32"), wxT("netapi32.dll"));
    wxFileName fn3(wxT("C:/Windows/System32"), wxT("netapi32"), wxT("dll"));
    wxFileName fn4(wxT("C"), wxT("\\Windows\\System32"), wxT("netapi32"), wxT("dll"));

    wxLogMessage(wxT("(Constructor 1) fn1: ") + fn1.GetFullPath(wxPATH_NATIVE));
    wxLogMessage(wxT("(Constructor 2) fn2: ") + fn2.GetFullPath());
    wxLogMessage(wxT("(Constructor 3) fn3: ") + fn3.GetFullPath());
    wxLogMessage(wxT("(Constructor 4) fn4: ") + fn4.GetFullPath());

    wxFileName fn5(wxT("/home/wx-maniak/Desktop/todo.odt"), wxPATH_UNIX);
    fn1.Assign(fn5); // Like a copy constructor
    fn2.Assign(wxT("/home/wx-maniak/Desktop/todo.odt"), wxPATH_UNIX);
    fn3.Assign(wxT("/home/wx-maniak/Desktop"), wxT("todo.odt"), wxPATH_UNIX);
    fn4.Assign(wxT("/home/wx-maniak/Desktop"), wxT("todo"), wxT("odt"), wxPATH_UNIX);
    fn5.Assign(wxT("home"), wxT("/wx-maniak/Desktop"), wxT("todo"), wxT("odt"), wxPATH_UNIX);

    wxLogMessage(wxT("(Assign 1) fn1: ") + fn1.GetFullPath(wxPATH_UNIX));
    wxLogMessage(wxT("(Assign 2) fn2: ") + fn2.GetFullPath(wxPATH_UNIX));
    wxLogMessage(wxT("(Assign 3) fn3: ") + fn3.GetFullPath(wxPATH_UNIX));
    wxLogMessage(wxT("(Assign 4) fn4: ") + fn4.GetFullPath(wxPATH_UNIX));
    wxLogMessage(wxT("(Assign 5) fn5: ") + fn5.GetFullPath(wxPATH_UNIX));

    wxFileName fn6;
    fn6.AssignCwd(); // Sets the current working directory

    wxFileName fn7(wxT("D:\\Libs\\wxWidgets-3.1.0\\unins000.exe"));
    fn7.AssignCwd(); // Sets the current working directory

    wxFileName fn8(wxT("file.txt"));
    fn8.AssignDir(wxT("/home/user/"), wxPATH_UNIX); // Sets the specified path

    wxFileName fn9;
    fn9.AssignHomeDir(); // Sets the user's home directory

    wxLogMessage(wxT("(AssignCwd 1) fn6: ") + fn6.GetFullPath(wxPATH_WIN));
    wxLogMessage(wxT("(AssignCwd 2) fn7: ") + fn7.GetFullPath(wxPATH_WIN));
    wxLogMessage(wxT("(AssignCwd 3) fn8: ") + fn8.GetFullPath(wxPATH_WIN));
    wxLogMessage(wxT("(AssignCwd 4) fn9: ") + fn9.GetFullPath(wxPATH_WIN));
}

void MyFrame::On_wxFileNameAnalysis(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file..."), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        wxFileName fn1(file);

        wxLogMessage(wxT("Path analysis: ") + file);

        // BASIC PATH ANALYSIS
        if (fn1.HasVolume()) // Does it contain a drive name?
        {
            // Get the drive name
            wxLogMessage(wxT("GetVolume(): ") + fn1.GetVolume());
            // Get the drive separator
            wxLogMessage(wxT("GetVolumeSeparator(): ") + fn1.GetVolumeSeparator());
#if __WXMSW__
            // Get the drive letter
            char disk_letter = (char)(fn1.GetVolume()).at(0);
            wxLogMessage(wxT("GetVolumeString(): ") + fn1.GetVolumeString(disk_letter, wxPATH_GET_SEPARATOR));
#endif
        }

        if (fn1.HasName()) // Does it contain a file name?
        {
            // Get the file name
            wxLogMessage(wxT("GetName(): ") + fn1.GetName());
        }

        if (fn1.HasExt()) // Does it contain an extension?
        {
            // Get the file name with extension
            wxLogMessage(wxT("GetFullName(): ") + fn1.GetFullName());
            // Get the extension
            wxLogMessage(wxT("GetExt(): ") + fn1.GetExt());
            // Strip the extension from the file name (virtual method)
            wxLogMessage(wxT("wxFileName::StripExtension(): ") + wxFileName::StripExtension(fn1.GetFullName()));
        }

        // DIRECTORY ANALYSIS
        size_t dir_count = fn1.GetDirCount(); // Number of directories in the path
        wxLogMessage(wxString::Format(wxT("GetDirCount(): %d"), dir_count));

        // Get the directories in the path
        wxArrayString dirs = fn1.GetDirs();
        wxString dirs_str(wxT("GetDirs(): Directories: "));
        for (wxString s : dirs) dirs_str += (s + wxT(" | "));
        wxLogMessage(dirs_str);

        // GETTING THE PATH
        wxString path = wxEmptyString;
        path = fn1.GetPath(true, wxPATH_NATIVE); // Get the path with separator
        wxLogMessage(wxT("GetPath() (variant 1, with separator): ") + path);
        path = fn1.GetPath(false, wxPATH_NATIVE); // Get the path without separator
        wxLogMessage(wxT("GetPath() (variant 1, without separator): ") + path);
        path = fn1.GetPath(wxPATH_GET_SEPARATOR, wxPATH_NATIVE); // Get the path with separator
        wxLogMessage(wxT("GetPath() (variant 2, with separator): ") + path);
        path = fn1.GetPath(wxPATH_NO_SEPARATOR, wxPATH_NATIVE); // Get the path without separator
        wxLogMessage(wxT("GetPath() (variant 2, without separator): ") + path);
        path = fn1.GetPath(wxPATH_GET_VOLUME, wxPATH_NATIVE); // Get the path with drive name
        wxLogMessage(wxT("GetPath() (variant 2, with drive name): ") + path);
        path = fn1.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR, wxPATH_NATIVE); // Get the path with drive name and separator
        wxLogMessage(wxT("GetPath() (variant 2, with drive name and separator): ") + path);
        path = fn1.GetPathWithSep(wxPATH_NATIVE); // Get the path with separator
        wxLogMessage(wxT("GetPathWithSep(): ") + path);
        path = fn1.GetFullPath(wxPATH_NATIVE); // Get the full path
        wxLogMessage(wxT("GetFullPath(): ") + path);
        path = fn1.GetShortPath(); // Get the short DOS-style path
        wxLogMessage(wxT("GetShortPath(): ") + path);

        // PATH SEPARATOR ANALYSIS
        wxUniChar path_sep = fn1.GetPathSeparator(wxPATH_NATIVE); // Path separator
        wxLogMessage(wxT("GetPathSeparator(): ") + wxString(path_sep));

        // Get the system path separators
        wxString separators = wxFileName::GetPathSeparators(wxPATH_NATIVE);
        wxLogMessage(wxT("wxFileName::GetPathSeparators(): ") + separators);

        wxString terminators = wxFileName::GetPathTerminators(wxPATH_NATIVE);
        wxLogMessage(wxT("wxFileName::GetPathTerminators(): ") + terminators);

        // EXTRACTING PATH COMPONENTS
        // Drive name + path
        wxString volume1, path1;
        wxFileName::SplitVolume(fn1.GetFullPath(), &volume1, &path1, wxPATH_NATIVE);
        wxLogMessage(wxString::Format(wxT("wxFileName::SplitVolume(): v=%s p=%s"), volume1, path1));

        // Drive name + path + name + extension + is extension?
        wxString volume2, path2, name2, ext2;
        bool is_ext2;
        wxFileName::SplitPath(fn1.GetFullPath(), &volume2, &path2, &name2, &ext2, &is_ext2, wxPATH_NATIVE);
        wxLogMessage(wxString::Format(wxT("wxFileName::SplitPath() (variant 1): v=%s p=%s n=%s e=%s is_ext=%d"), volume2, path2, name2, ext2, (int)is_ext2));

        // Drive name + path + name + extension
        wxString volume3, path3, name3, ext3;
        wxFileName::SplitPath(fn1.GetFullPath(), &volume3, &path3, &name3, &ext3, wxPATH_NATIVE);
        wxLogMessage(wxString::Format(wxT("wxFileName::SplitPath() (variant 2): v=%s p=%s n=%s e=%s"), volume3, path3, name3, ext3));

        // Path with drive name + name + extension
        wxString path4, name4, ext4;
        wxFileName::SplitPath(fn1.GetFullPath(), &path4, &name4, &ext4, wxPATH_NATIVE);
        wxLogMessage(wxString::Format(wxT("wxFileName::SplitPath() (variant 3): p=%s n=%s e=%s"), path4, name4, ext4));
    }
}

#define FILE_COMPONENT_PATH       wxT("Path")
#define FILE_COMPONENT_VOLUME     wxT("Drive name")
#define FILE_COMPONENT_NAME       wxT("File name only")
#define FILE_COMPONENT_NAMEEXT    wxT("File name and extension")
#define FILE_COMPONENT_EXT        wxT("Extension only")
#define FILE_COMPONENT_EXT_DELETE wxT("Remove extension")
void MyFrame::On_wxFileNameEdit(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file..."), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        wxLogMessage(wxT("Selected path: ") + file);

        wxFileName fn1(file);
        wxArrayString arr;
        arr.Add(FILE_COMPONENT_PATH);
        if (fn1.HasVolume()) arr.Add(FILE_COMPONENT_VOLUME);
        if (fn1.HasName()) arr.Add(FILE_COMPONENT_NAME);
        if (fn1.HasExt())
        {
            arr.Add(FILE_COMPONENT_EXT);
            arr.Add(wxT("Remove extension"));
        }

        wxString choice = wxGetSingleChoice(wxT("Which component do you want to change?"), wxT("Choice..."), arr, 0);
        if (!choice.empty())
        {
            wxString new_str = wxEmptyString;

            if (!choice.IsSameAs(FILE_COMPONENT_EXT_DELETE))
            {
                new_str = wxGetTextFromUser(wxT("Enter new value..."));
            }

            if (!new_str.empty())
            {
                if (choice.IsSameAs(FILE_COMPONENT_PATH))
                {
                    fn1.SetPath(new_str, wxPATH_NATIVE); // Change path
                }
                else if (choice.IsSameAs(FILE_COMPONENT_VOLUME))
                {
                    fn1.SetVolume(new_str); // Change drive name/letter
                }
                else if (choice.IsSameAs(FILE_COMPONENT_NAME))
                {
                    fn1.SetName(new_str); // Change file name
                }
                else if (choice.IsSameAs(FILE_COMPONENT_NAMEEXT))
                {
                    fn1.SetFullName(new_str); // Change file name and extension
                }
                else if (choice.IsSameAs(FILE_COMPONENT_EXT))
                {
                    fn1.SetExt(new_str); // Change file extension
                }
                else if (choice.IsSameAs(FILE_COMPONENT_EXT_DELETE))
                {
                    fn1.SetEmptyExt(); // Remove extension (set to empty)
                }
            }

            wxLogMessage(wxT("Path after changes: ") + fn1.GetFullPath());
            fn1.Clear();
            wxLogMessage(wxT("Restored original path: ") + fn1.GetFullPath());
        }
    }
}

void MyFrame::On_wxFileNameEdit2(wxUnusedEvent) // EDITING DIRECTORY HIERARCHY
{
    wxString file = wxFileSelector(wxT("Select file..."));
    if (!file.empty())
    {
        wxLogMessage(wxT("Initial path: ") + file);

        wxFileName fn1(file);
        fn1.PrependDir(wxT("Start")); // Add directory at the beginning
        wxLogMessage(wxT("Added directory at the beginning: ") + fn1.GetFullPath());
        fn1.AppendDir(wxT("End")); // Add directory at the end
        wxLogMessage(wxT("Added directory at the end: ") + fn1.GetFullPath());

        int count = fn1.GetDirCount();
        if (count > 1)
        {
            fn1.InsertDir(count - 1, wxT("ABC")); // Add directory at the second last position
            wxLogMessage(wxT("Added directory at the second last position: ") + fn1.GetFullPath());

            if (count >= 3)
            {
                fn1.InsertDir(2, wxT("Position_3")); // Add directory at the third position
                wxLogMessage(wxT("Added directory at the third position: ") + fn1.GetFullPath());
            }
        }

        // Removing directories
        fn1.RemoveDir(1); // Remove directory at position 2
        wxLogMessage(wxT("Removed directory at position 2: ") + fn1.GetFullPath());
        fn1.RemoveLastDir(); // Remove the last directory
        wxLogMessage(wxT("Removed the last directory: ") + fn1.GetFullPath());
    }
}

void MyFrame::On_wxFileNameEdit3(wxUnusedEvent) // RELATIVE AND ABSOLUTE PATHS
{
    wxString file1 = wxFileSelector(wxT("Select file 1"));
    wxString file2 = wxFileSelector(wxT("Select file 2"));

    if (!file1.IsEmpty() && !file2.IsEmpty())
    {
        wxFileName fn1(file1);
        wxFileName fn2(file2);

        wxLogMessage(wxT("Path/file 1: ") + file1);
        wxLogMessage(wxT("Path/file 2: ") + file2);

        if (fn1.IsAbsolute())
        {
            fn1.MakeRelativeTo();
            wxLogMessage(wxT("Path 1 relative to working directory:") + fn1.GetFullPath());
            fn1.MakeRelativeTo(file2, wxPATH_NATIVE);
            wxLogMessage(wxT("Path 1 relative to path 2:") + fn1.GetFullPath());
        }

        if (fn2.IsAbsolute())
        {
            fn2.MakeRelativeTo();
            wxLogMessage(wxT("Path 2 relative to working directory:") + fn2.GetFullPath());
            fn2.MakeRelativeTo(file1);
            wxLogMessage(wxT("Path 2 relative to path 1:") + fn2.GetFullPath());
        }

        if (fn1.IsRelative())
        {
            fn1.MakeAbsolute();
        }

        if (fn2.IsRelative())
        {
            fn2.MakeAbsolute(wxGetCwd(), wxPATH_NATIVE);
        }

        wxLogMessage(wxT("Path 1 absolute for cwd=NULL: ") + fn1.GetFullPath());
        wxLogMessage(wxT("Path 2 absolute for current working directory: ") + fn2.GetFullPath());
    }
}

void MyFrame::On_wxFileNameEdit4(wxUnusedEvent) // PATH NORMALIZATION
{
    NormalizeDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxLogMessage("Normalizing path: " + dlg.path->GetValue());

        int format;
        switch (dlg.format->GetSelection())
        {
            case 1: format = wxPATH_UNIX; break;
            case 2: format = wxPATH_WIN; break;
            default: format = wxPATH_NATIVE; break;
        }

        int flags = 0;
        if (dlg.norm_case->IsChecked())
        {
            flags |= wxPATH_NORM_CASE;
        }

        if (dlg.norm_all->IsChecked()) // If ALL, no need to check others
        {
            flags |= wxPATH_NORM_ALL;
        }
        else
        {
            if (dlg.norm_env_vars->IsChecked())
            {
                flags |= wxPATH_NORM_ENV_VARS;
            }

            if (dlg.norm_dots->IsChecked())
            {
                flags |= wxPATH_NORM_DOTS;
            }

            if (dlg.norm_tilde->IsChecked())
            {
                flags |= wxPATH_NORM_TILDE;
            }

            if (dlg.norm_absolute->IsChecked())
            {
                flags |= wxPATH_NORM_ABSOLUTE;
            }

            if (dlg.norm_long->IsChecked())
            {
                flags |= wxPATH_NORM_LONG;
            }

            if (dlg.norm_shortcut->IsChecked())
            {
                flags |= wxPATH_NORM_SHORTCUT;
            }
        }

        //
        wxFileName fn1(dlg.path->GetValue());

        if (fn1.Normalize(flags, wxGetCwd(), (wxPathFormat)format))
        {
            wxLogMessage(wxT("Normalization successful: ") + fn1.GetFullPath());
        }
        else
        {
            wxLogWarning(wxT("Path normalization failed!"));
        }
    }
}


#define FILE_TIME_OPTION1 wxT("Set file times to current values")
#define FILE_TIME_OPTION2 wxT("Set file times to January 1st of this year, 12:00")
void MyFrame::On_wxFileNameTime(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file..."), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        wxLogMessage(wxT("Selected file: ") + file);

        wxFileName fn1(file);

        wxDateTime access;
        wxDateTime creation;
        wxDateTime modification;

        if (fn1.GetTimes(&access, &modification, &creation))
        {
            wxLogMessage(wxT("Created: ") + creation.FormatISOCombined(' '));
            wxLogMessage(wxT("Last modification: ") + modification.FormatISOCombined(' '));
            wxLogMessage(wxT("Last access: ") + modification.FormatISOCombined(' '));

            wxLogMessage(wxT("GetModificationTime() method: ") + fn1.GetModificationTime().FormatISOCombined(' '));

            wxArrayString choices;
            choices.Add(FILE_TIME_OPTION1);
            choices.Add(FILE_TIME_OPTION2);
            wxString choice = wxGetSingleChoice(wxT("What do you want to do?"), wxT("Question"), choices, 0);

            if (!choice.IsEmpty())
            {
                if (choice == FILE_TIME_OPTION1)
                {
                    fn1.Touch(); // Set times to current values
                }
                else
                {
                    // January 1st of the current year
                    wxDateTime fj(1, wxDateTime::Jan, wxDateTime::Now().GetYear(), 12, 0, 0);
                    fn1.SetTimes(&fj, &fj, &fj);
                }

                // Print new file dates
                if (fn1.GetTimes(&access, &modification, &creation))
                {
                    wxLogMessage(wxT("New file times for ") + file);
                    wxLogMessage(wxT("Created: ") + creation.FormatISOCombined(' '));
                    wxLogMessage(wxT("Last modification: ") + modification.FormatISOCombined(' '));
                    wxLogMessage(wxT("Last access: ") + modification.FormatISOCombined(' '));
                }
            }
        }
        else
        {
            wxLogMessage(wxT("Failed to get file times for ") + file);
        }

    }
}

void MyFrame::On_wxFileNamePermissions(wxUnusedEvent)
{
    // For clarity, permission checking is limited to files only
    wxString file = wxFileSelector(wxT("Select file..."), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        wxLogMessage(wxT("Selected file: ") + file);

        wxFileName fn1(file);
        PermissionDialog dlg(this, file);

        // Example of checking permissions (in the context of user permissions)
        if (fn1.IsFileExecutable()) { dlg.urun->SetValue(true); }
        if (fn1.IsFileReadable()) { dlg.uread->SetValue(true); }
        if (fn1.IsFileWritable()) { dlg.uwrite->SetValue(true); }

        // SETTING PERMISSIONS
        if (dlg.ShowModal() == wxID_OK)
        {
            int flags = 0x00;

            if (dlg.uread->IsChecked()) { flags |= wxPOSIX_USER_READ; }
            if (dlg.uwrite->IsChecked()) { flags |= wxPOSIX_USER_WRITE; }
            if (dlg.urun->IsChecked()) { flags |= wxPOSIX_USER_EXECUTE; }
            if (dlg.gread->IsChecked()) { flags |= wxPOSIX_GROUP_READ; }
            if (dlg.gwrite->IsChecked()) { flags |= wxPOSIX_GROUP_WRITE; }
            if (dlg.grun->IsChecked()) { flags |= wxPOSIX_GROUP_EXECUTE; }
            if (dlg.oread->IsChecked()) { flags |= wxPOSIX_OTHERS_READ; }
            if (dlg.owrite->IsChecked()) { flags |= wxPOSIX_OTHERS_WRITE; }
            if (dlg.orun->IsChecked()) { flags |= wxPOSIX_OTHERS_EXECUTE; }

            if (fn1.SetPermissions(flags))
            {
                wxLogMessage(wxT("Successfully applied new permissions to file ") + file);
            }
            else
            {
                wxLogWarning(wxT("Failed to set new permissions for file ") + file);
            }
        }
    }
}

void MyFrame::On_wxFileNameSize(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file..."), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.empty())
    {
        wxLogMessage(wxT("Selected file: ") + file);

        wxFileName fn1(file);

        wxULongLong size = fn1.GetSize(); // or wxFileName::GetSize(file);
        wxLogMessage(wxT("Size in bytes: ") + size.ToString());
        wxLogMessage(wxT("Human readable (traditional): ") + fn1.GetHumanReadableSize(wxT("Not available")));
        wxLogMessage(wxT("Human readable (IEC): ") + fn1.GetHumanReadableSize(wxT("Not available"), 1, wxSIZE_CONV_IEC));
        wxLogMessage(wxT("Human readable (SI): ") + fn1.GetHumanReadableSize(wxT("Not available"), 1, wxSIZE_CONV_SI));
        wxLogMessage(wxT("Human readable (static, traditional): ") + wxFileName::GetHumanReadableSize(size, wxT("Not available")));
        wxLogMessage(wxT("Human readable (static, IEC): ") + wxFileName::GetHumanReadableSize(size, wxT("Not available"), 1, wxSIZE_CONV_IEC));
        wxLogMessage(wxT("Human readable (static, SI): ") + wxFileName::GetHumanReadableSize(size, wxT("Not available"), 1, wxSIZE_CONV_SI));
    }
}

void MyFrame::On_wxFileNameTemp(wxUnusedEvent)
{
    wxLogMessage(wxT("Creating temporary files"));

    // Creating a temporary file name using a static method
    wxString name0a = wxFileName::CreateTempFileName(wxT("tem"), (wxFile*)NULL);
    wxString name0b = wxFileName::CreateTempFileName(wxT("temp_"), (wxFFile*)NULL);

    wxLogMessage(wxT("Generated name (static method + wxFile): ") + name0a);
    wxLogMessage(wxT("Generated name (static method + wxFFile): ") + name0b);

    // Creating a temporary file using a static method with wxFile
    wxString file1_name = wxT("test_file1.dat");
    wxFile file1(file1_name, wxFile::write);
    wxString name1 = wxFileName::CreateTempFileName(wxT("abc"), &file1);
    if (file1.IsOpened())
    {
        file1.Write(wxT("test file: ") + name1);
        file1.Write(wxT(" ") + wxDateTime::Now().FormatISOCombined());
    }
    file1.Close();

    wxLogMessage(wxT("File 1: ") + file1_name);
    wxLogMessage(wxT("Temporary file 1 (static method + wxFile): ") + name1);

    // Creating a temporary file using a static method with wxFFile
    wxString file2_name = wxT("test_file2.dat");
    wxFFile file2(file2_name, "w");
    wxString name2 = wxFileName::CreateTempFileName(wxT("TMP"), &file2);
    if (file2.IsOpened())
    {
        file2.Write(wxT("test file: ") + name2);
        file2.Write(wxT(" ") + wxDateTime::Now().FormatISOCombined());
    }
    file2.Close();

    wxLogMessage(wxT("File 2: ") + file2_name);
    wxLogMessage(wxT("Temporary file 2 (static method + wxFFile): ") + name2);

    // Creating a wxFileName object and a temporary file name
    wxFileName fn1(wxT("test_file.dat"));
    wxString name = fn1.CreateTempFileName(wxT("Rozdzial15_"));

    wxLogMessage(wxT("File from wxFileName object: ") + fn1.GetFullPath());
    wxLogMessage(wxT("Temporary path generated for file: ") + name);

    // Creating a temporary file on an object with wxFile
    wxString file3_name = wxT("test_file3.dat");
    wxFile file3(file3_name, wxFile::write);
    wxFileName fn3; // Initializing wxFileName object is not required here
    wxString name3 = fn3.CreateTempFileName(wxT("XXX"), &file3);
    if (file3.IsOpened())
    {
        file3.Write(wxT("test file: ") + name3);
        file3.Write(wxT(" ") + wxDateTime::Now().FormatISOCombined());
    }
    file3.Close();

    wxLogMessage(wxT("File 3 (wxFileName): ") + file3_name);
    wxLogMessage(wxT("Temporary file 3 (wxFFile): ") + name3);

    // Creating a temporary file on an object with wxFFile
    wxString file4_name = wxT("test_file4.dat");
    wxFFile file4(file4_name, "w");
    wxFileName fn4(file4_name);
    wxString name4 = fn4.CreateTempFileName(wxT("YYY"), &file4);
    if (file4.IsOpened())
    {
        file4.Write(wxT("test file: ") + name4);
        file4.Write(wxT(" ") + wxDateTime::Now().FormatISOCombined());
    }
    file4.Close();

    wxLogMessage(wxT("File 4 (wxFileName): ") + file4_name);
    wxLogMessage(wxT("Temporary file 4 (wxFFile): ") + name4);

    // Creating a temporary file in the application location
    wxString file5_name = wxT("test_file5.dat");
    wxFFile file5(file5_name, "w");
    wxFileName fn5(file5_name);

    wxString name5 = fn5.CreateTempFileName(wxGetCwd() + "\\", &file5);
    wxLogMessage(wxT("File 5: ") + file5_name);
    wxLogMessage(wxT("Temporary file 5 (prefix: wxGetCwd() + separator): ") + name5);

    name5 = fn5.CreateTempFileName(wxGetCwd(), &file5);
    wxLogMessage(wxT("Temporary file 5 (prefix: wxGetCwd() without separator): ") + name5);

    name5 = fn5.CreateTempFileName(wxEmptyString, &file5);
    wxLogMessage(wxT("Temporary file 5 (prefix: wxEmptyString): ") + name5);
}

void MyFrame::On_wxFileNameTest(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file..."));
    if (!file.empty())
    {
        wxString test_file = wxT("test_file1.dat");

        // Creating objects
        wxFileName fn1(file);
        fn1.MakeAbsolute();
        wxFileName fn2(test_file);
        fn2.MakeAbsolute();

        wxLogMessage(wxT("Selected file: ") + fn1.GetFullPath());
        wxLogMessage(wxT("Test file: ") + fn2.GetFullPath());

        // Comparing objects
        wxLogMessage(wxString::Format(wxT("Comparison fn1 == fn2: %d"), (int)(fn1 == fn2)));
        wxLogMessage(wxString::Format(wxT("Comparison fn1 != fn2: %d"), (int)(fn1 != fn2)));
        wxLogMessage(wxString::Format(wxT("Comparison fn1 == file path 1: %d"), (int)(fn1 == fn1.GetFullPath())));
        wxLogMessage(wxString::Format(wxT("Comparison fn1 != file path 1: %d"), (int)(fn1 != fn1.GetFullPath())));
        wxLogMessage(wxString::Format(wxT("Comparison fn2 == file path 1: %d"), (int)(fn2 == fn1.GetFullPath())));
        wxLogMessage(wxString::Format(wxT("Comparison fn2 != file path 1: %d"), (int)(fn2 != fn1.GetFullPath())));

        // Property tests
        wxLogMessage(wxT("Property tests of the selected file"));

        bool test1 = fn1.IsOk(); // Checking the validity of the object (whether initialized)
        bool test2 = fn1.IsDir(); // Is the path to a folder/location?

        wxLogMessage(wxString::Format(wxT("Testing validity of fn1: %d"), (int)test1));
        wxLogMessage(wxString::Format(wxT("Does fn1 contain a unique Windows path?: %d"), (int)test2));
    }
}

void MyFrame::On_wxFileSystemWatcher(wxFileSystemWatcherEvent& event)
{
    wxLogMessage(wxT("[MONITORING] ") + event.ToString());
}

void MyFrame::On_wxFileSystemWatcherCwd(wxCommandEvent& event)
{
    wxFileName current(wxGetCwd());
    current.DontFollowLink();

    if (event.IsChecked())
    {
        watcher->AddTree(current);
        wxLogMessage(wxT("Monitoring of the working directory STARTED"));
    }
    else
    {
        watcher->RemoveTree(current);
        wxLogMessage(wxT("Monitoring of the working directory STOPPED"));
    }
}

void MyFrame::On_wxFileSystemWatcherAddDir(wxUnusedEvent)
{
    wxString dir = wxDirSelector(wxT("Select folder to monitor"), wxEmptyString, wxDD_DIR_MUST_EXIST);
    if (!dir.IsEmpty())
    {
        if (watcher->AddTree(dir))
        {
            wxLogMessage(wxT("Monitoring of the directory started: ") + dir);
        }
        else
        {
            wxLogWarning(wxT("Failed to add folder to monitoring!"));
        }
    }
}

void MyFrame::On_wxFileSystemWatcherAddFile(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file to monitor"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.IsEmpty())
    {
        if (watcher->Add(file))
        {
            wxLogMessage(wxT("Monitoring of the file started: ") + file);
        }
        else
        {
            wxLogWarning(wxT("Failed to add file to monitoring!"));
        }
    }
}

void MyFrame::On_wxFileSystemWatcherDelDir(wxUnusedEvent)
{
    wxString dir = wxDirSelector(wxT("Select folder to monitor"), wxEmptyString, wxDD_DIR_MUST_EXIST);
    if (!dir.IsEmpty())
    {
        if (watcher->RemoveTree(dir))
        {
            wxLogMessage(wxT("Monitoring of the directory ended: ") + dir);
        }
        else
        {
            wxLogWarning(wxT("Failed to stop monitoring the directory!"));
        }
    }
}

void MyFrame::On_wxFileSystemWatcherDelFile(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file to monitor"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.IsEmpty())
    {
        if (watcher->Remove(file))
        {
            wxLogMessage(wxT("Monitoring of the file ended: ") + file);
        }
        else
        {
            wxLogWarning(wxT("Failed to stop monitoring the file!"));
        }
    }
}

void MyFrame::On_wxFileSystemWatcherDelAll(wxUnusedEvent)
{
    if (watcher->RemoveAll())
    {
        wxLogMessage(wxT("All monitored items have been removed."));
    }
    else
    {
        wxLogWarning(wxT("An error occurred while removing monitored items!"));
    }
}

void MyFrame::On_wxArchiveFSHandler(wxUnusedEvent)
{
    // Creating a file system handler
    wxFileSystem fs;
    wxArchiveFSHandler ah;
    fs.AddHandler(&ah);

    // Listing the contents of the selected archive
    wxString file = wxFileSelector(wxT("Select archive file"), wxEmptyString, wxEmptyString, wxT("zip"), wxT("ZIP Archive|*.zip"), wxFD_FILE_MUST_EXIST);
    if (!file.IsEmpty())
    {
        wxLogMessage(wxString::Format(wxT("Contents of the archive %s:"), file));

        wxString fs_name = ah.FindFirst(file + wxT("#zip:*.*"));
        while (!fs_name.IsEmpty())
        {
            wxLogMessage(fs_name);
            fs_name = ah.FindNext();
        }
    }

    // Displaying the contents of a file
    int res = wxMessageBox(wxT("Do you want to display the contents of the file test/archive_test.zip#zip:file_004.txt?"), wxT("Question"), wxYES_NO | wxICON_QUESTION);
    if (res == wxYES)
    {
        wxFSFile* fsf = fs.OpenFile(wxT("test/archive_test.zip#zip:file_004.txt"));
        if (fsf)
        {
            wxInputStream* istream = fsf->GetStream();
            if (istream)
            {
                wxStringOutputStream ss; // requires wx/sstream.h
                ss.Write(*istream);
                ss.Close();

                wxLogMessage(ss.GetString());
            }
        }
    }

    // Removing the file system handler
    fs.RemoveHandler(&ah);
}

void MyFrame::On_wxArchiveFSHandler2(wxUnusedEvent)
{
    // Creating a file system handler
    wxFileSystem fs;
    wxArchiveFSHandler ah;
    fs.AddHandler(&ah);
    wxFSFile* fsf = NULL;

    // Getting the path to the archive
    wxString source = wxFileSelector(wxT("Select archive file"), wxEmptyString, wxEmptyString, wxT("zip"), wxT("ZIP Archive|*.zip"), wxFD_FILE_MUST_EXIST);
    if (!source.IsEmpty())
    {
        // Getting the destination folder
        wxString dest = wxDirSelector(wxT("Where to save?"), wxEmptyString, wxDD_NEW_DIR_BUTTON);
        if (!dest.IsEmpty())
        {
            wxLogMessage(wxString::Format(wxT("Extracting archive %s to %s"), source, dest));

            // Reading the name of the first file
            wxString fs_name = ah.FindFirst(source + wxT("#zip:*.*"));
            while (!fs_name.IsEmpty())
            {
                fsf = fs.OpenFile(fs_name);

                // Extracting the name (cutting off the left side and protocol)
                wxString tmp = fs_name;
                wxString fn_name = tmp.SubString(tmp.Find(':', true) + 1, tmp.Len());

                // Reading the file into a stream
                wxInputStream* istream = fsf->GetStream();

                // Writing the stream to a file
                wxFileOutputStream fstream(dest + wxT("\\") + fn_name); // Requires wx/wfstream.h
                fstream.Write(*istream);
                fstream.Close();
                wxLogMessage(wxT("Extracted file: ") + fn_name);

                // Reading the name of the next file
                fs_name = ah.FindNext();
            }
        }
    }

    // Removing the file system handler
    fs.RemoveHandler(&ah);
}

void MyFrame::On_wxZipStreamCreate(wxUnusedEvent)
{
    // Getting the path of the directory to be zipped
    wxString dir = wxDirSelector(wxT("Select folder to zip"), wxEmptyString, wxDD_DIR_MUST_EXIST);
    if (dir.IsEmpty()) { return; }

    // Compression level and comment
    int level = (int)wxGetNumberFromUser(wxT("Compression level"), wxEmptyString, wxT("Select..."), 6, 1, 9, this); // Requires wx/numdlg.h
    wxString comment = wxGetTextFromUser(wxT("Archive comment"), wxT("Comment"), wxT("ZIP Archive ") + wxDateTime::Now().FormatISOCombined(' '));

    // Getting the list of files from the selected directory
    wxArrayString files;
    wxArrayString dirs;

    MyZipTraverser traverser(&files, &dirs);

    wxDir source(dir);
    if (source.IsOpened())
    {
        source.Traverse(traverser, wxT("*.*"), wxDIR_DEFAULT | wxDIR_NO_FOLLOW);
        source.Close();
    }

    // Preparing the archive path
    wxString zip_path = dir + wxT(".zip");

    // Streams for writing data to the archive file
    wxFFileOutputStream fos(zip_path);
    wxZipOutputStream zip(fos);

    // Archive settings
    zip.SetLevel(level); // Compression level
    zip.SetComment(comment); // Comment

    // Loop to create archive directories
    for (wxString s : dirs)
    {
        wxFileName folder(s);
        if (folder.IsOk())
        {
            // Preparing the folder path
            wxString short_path = s.SubString(dir.Len(), s.Len());
            wxLogMessage(wxT("Creating folder: ") + short_path);

            // Adding the directory to the archive
            zip.PutNextDirEntry(short_path);
        }
    }

    // Loop to create the archive from subsequent files
    for (wxString s : files)
    {
        wxFileName file(s);
        if (file.IsOk())
        {
            // Preparing the file path
            wxString short_path = s.SubString(dir.Len(), s.Len());
            wxLogMessage(wxT("Zipping file: ") + short_path);

            // Reading the source file
            wxFile src_file(s);
            wxFileInputStream fis(src_file);

            // Writing the file to the archive
            zip.PutNextEntry(short_path, file.GetModificationTime());
            zip.Write(fis);
        }
    }

    // Closing the output stream
    if (zip.Close())
    {
        wxLogMessage(wxString::Format(wxT("The archive %s has been created."), zip_path));
    }
}

void MyFrame::On_wxZipStreamExtract(wxUnusedEvent)
{
    // Selecting the archive file to extract
    wxString source = wxFileSelector(wxT("Select archive file"), wxEmptyString, wxEmptyString, wxT("zip"), wxT("ZIP Archive|*.zip"), wxFD_FILE_MUST_EXIST);
    if (source.IsEmpty()) { return; }

    // Selecting the destination path
    wxString dir = wxDirSelector(wxT("Select destination folder"), wxEmptyString, wxDD_NEW_DIR_BUTTON);
    if (dir.IsEmpty()) { return; }

    // Streams for reading the archive
    wxFFileInputStream fis(source);
    wxZipInputStream zip(fis);

    // Getting the first entry in the archive
    wxZipEntry* tmp = zip.GetNextEntry();
    while (tmp)
    {
        wxLogMessage("Extracting: " + tmp->GetName());

        if (tmp->IsDir()) // If the entry is a directory...
        {
            // ... create the directory in the destination location
            wxMkdir(dir + wxFileName::GetPathSeparator() + tmp->GetName());
        }
        else
        {
            // Writing files to the destination folder
            wxFFileOutputStream fos(dir + wxFileName::GetPathSeparator() + tmp->GetName());
            fos.Write(zip);
            fos.Close();

            zip.CloseEntry();
        }

        // Getting the next entry in the archive
        tmp = zip.GetNextEntry();
    }
}

void MyFrame::On_wxTextFileAnalysis(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file for analysis"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.IsEmpty())
    {
        wxTextFile tfile(file);
        tfile.Open();

        wxLogMessage(wxT("File: ") + tfile.GetName());
        wxLogMessage(wxString::Format(wxT("Lines: %d"), tfile.GetLineCount()));

        wxString ltype = wxEmptyString;
        switch (tfile.GuessType())
        {
        case wxTextFileType_None: ltype = wxT("unknown"); break;
        case wxTextFileType_Unix: ltype = wxT("Unix"); break;
        case wxTextFileType_Dos: ltype = wxT("DOS"); break;
        case wxTextFileType_Mac: ltype = wxT("Mac"); break;
        case wxTextFileType_Os2: ltype = wxT("OS2"); break;
        }
        wxLogMessage(wxT("Line ending type: ") + ltype);

        wxString term(wxTextFile::GetEOL(tfile.GuessType()));
        wxString term_codes;
        for (wxChar c : term) { term_codes += wxString::Format("%d(%#X) ", (int)c, (int)c); }
        wxLogMessage(wxT("EOL characters: ") + term_codes);

        tfile.Close();
    }
}

void MyFrame::On_wxTextFileAddLine(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file to edit"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.IsEmpty())
    {
        wxTextFile tfile(file);
        if (!tfile.Open())
        {
            wxLogWarning(wxT("An error occurred while opening the file ") + file);
            return;
        }

        wxString line = wxGetTextFromUser(wxT("Enter the content of the new line"));
        if (line.IsEmpty()) { return; }

        int res = wxMessageBox(wxT("Add the line at the end of the file?"), wxT("Question"), wxYES_NO | wxICON_QUESTION);
        if (res == wxYES)
        {
            tfile.AddLine(line, tfile.GuessType());
        }
        else
        {
            int nr = wxGetNumberFromUser(wxString::Format(wxT("Before which line to insert the new line?\nRange: 1 - %d"), \
                tfile.GetLineCount()), wxEmptyString, wxT("Line selection"), 1, 1, tfile.GetLineCount(), this);
            if (nr < 0) { return; }

            tfile.InsertLine(line, nr - 1, tfile.GuessType());
        }

#if __WXMSW__
        tfile.Write(wxTextFileType_Dos);
#elif __WXGTK__
        tfile.Write(wxTextFileType_Unix);
#endif

        if (tfile.Close())
        {
            wxLogMessage(wxT("The new line has been inserted into the file ") + file);
        }
    }
}

void MyFrame::On_wxTextFileGetLine(wxUnusedEvent)
{
    wxString file = wxFileSelector(wxT("Select file"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxFD_FILE_MUST_EXIST);
    if (!file.IsEmpty())
    {
        wxTextFile tfile(file);
        if (!tfile.Open())
        {
            wxLogWarning(wxT("An error occurred while opening the file ") + file);
            return;
        }

        int nr = wxGetNumberFromUser(wxString::Format(wxT("Which line to read?\nRange: 1 - %d"),
            tfile.GetLineCount()), wxEmptyString, wxT("Line selection"), 1, 1, tfile.GetLineCount(), this);
        if (nr < 0) { return; }

        wxLogMessage(wxString::Format(wxT("Line %d: %s"), nr, tfile.GetLine(nr - 1)));

        tfile.Close();
    }
}

void MyFrame::On_wxTextFileCreate(wxUnusedEvent)
{
    // Creating sample file lines
    wxString l1 = wxDateTime::Now().FormatISOCombined();
    l1.Replace(":", "-");
    wxString l2 = wxString::Format(wxT("ABCDEFGHIJKLMNOPRSTUWXYZ %f"), wxDateTime::Now().GetMinute() / 33);
    wxString l3 = wxString::Format(wxT("wxWidgets: %s, system: %s"), wxVERSION_STRING, wxGetOsDescription());
    wxString l4 = wxT("Litwo, Ojczyzno moja! ty jesteś jak zdrowie; Ile cię trzeba cenić, ten tylko się dowie, Kto cię stracił.");

    // Creating the file
    wxTextFile tfile(wxT("text_file_") + l1);
    if (!tfile.Create())
    {
        wxLogWarning(wxT("An error occurred while creating the file!"));
        return;
    }

    // Opening the file
    if (!tfile.Open())
    {
        wxLogWarning(wxT("An error occurred while opening the file ") + tfile.GetName());
        return;
    }

    // Adding lines to the file structure
    tfile.AddLine(l1);
    tfile.AddLine(l2);
    tfile.AddLine(l3);
    tfile.AddLine(l4);

    // Saving changes to the file
#if __WXMSW__
    tfile.Write(wxTextFileType_Dos);
#elif __WXGTK__
    tfile.Write(wxTextFileType_Unix);
#endif

    // Closing the file
    if (tfile.Close())
    {
        wxLogMessage(wxT("The file ") + tfile.GetName() + wxT(" has been successfully created."));
    }
}

