/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

//---------------------------------------------------------------------------
// Class MyApp
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

//---------------------------------------------------------------------------
// Class MyFrame
//---------------------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style);
	~MyFrame();

	void OnAboutProgram(wxCommandEvent& event);
	void OnClose(wxCommandEvent& event);
	void CloseApp(wxCloseEvent& event);

	void On_wxStandardPaths(wxCommandEvent& event);

	void On_wxMkdir(wxCommandEvent& event);
	void On_wxRmdir(wxCommandEvent& event);
	void On_wxGetCwd(wxCommandEvent& event);
	void On_wxSetWorkingDirectory(wxCommandEvent& event);
	//
	void On_wxCopyFile(wxCommandEvent& event);
	void On_wxRenameFile(wxCommandEvent& event);
	void On_wxRemoveFile(wxCommandEvent& event);
	void On_wxConcatFiles(wxCommandEvent& event);
	void On_wxFindFirstNextFile(wxCommandEvent& event);
	void On_wxFileModificationTime(wxCommandEvent& event);
	//
	void On_wxGetDiskSpace(wxCommandEvent& event);
	//
	void On_wxDirTests(wxCommandEvent& event);
	void On_wxDirListLoop(wxCommandEvent& event);
	void On_wxDirListTraversal(wxCommandEvent& event);
	void On_wxDirListGetAll(wxCommandEvent& event);
	//
	void On_wxFile(wxCommandEvent& event);
	void On_wxFFile(wxCommandEvent& event);
	//
	void On_wxFileName(wxCommandEvent& event);
	void On_wxFileNameAnalysis(wxCommandEvent& event);
	void On_wxFileNameEdit(wxCommandEvent& event);
	void On_wxFileNameEdit2(wxCommandEvent& event);
	void On_wxFileNameEdit3(wxCommandEvent& event);
	void On_wxFileNameEdit4(wxCommandEvent& event);
	void On_wxFileNameTime(wxCommandEvent& event);
	void On_wxFileNamePermissions(wxCommandEvent& event);
	void On_wxFileNameSize(wxCommandEvent& event);
	void On_wxFileNameTemp(wxCommandEvent& event);
	void On_wxFileNameTest(wxCommandEvent& event);

	void On_wxArchiveFSHandler(wxCommandEvent& event);
	void On_wxArchiveFSHandler2(wxCommandEvent& event);
	void On_wxZipStreamCreate(wxCommandEvent& event);
	void On_wxZipStreamExtract(wxCommandEvent& event);

	void On_wxTextFileAnalysis(wxCommandEvent& event);
	void On_wxTextFileAddLine(wxCommandEvent& event);
	void On_wxTextFileGetLine(wxCommandEvent& event);
	void On_wxTextFileCreate(wxCommandEvent& event);

	void On_wxFileSystemWatcherCwd(wxCommandEvent& event);
	void On_wxFileSystemWatcherAddDir(wxCommandEvent& event);
	void On_wxFileSystemWatcherAddFile(wxCommandEvent& event);
	void On_wxFileSystemWatcherDelDir(wxCommandEvent& event);
	void On_wxFileSystemWatcherDelFile(wxCommandEvent& event);
	void On_wxFileSystemWatcherDelAll(wxCommandEvent& event);

	void On_wxFileSystemWatcher(wxFileSystemWatcherEvent& event);

private:
	wxStatusBar* sb;
	wxMenuBar* mb;
	wxMenu* mFile;
	wxMenu* mHelp;

	wxMenu* mFnc;
	wxMenu* mDir;
	wxMenu* mFil;
	wxMenu* mFin;
	wxMenu* mZip;
	wxMenu* mWat;
	wxMenu* mTxt;

	wxPanel* panel;

	wxFileSystemWatcher* watcher;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Class MyTraverser
//---------------------------------------------------------------------------
class MyTraverser : public wxDirTraverser
{
public:
	MyTraverser(wxArrayString* results, wxArrayString* errors = NULL) : res(results), err(errors) {}

	virtual wxDirTraverseResult OnDir(const wxString& dirname)
	{
		res->Add(dirname);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		res->Add(filename);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname)
	{
		if (err) { err->Add(openerrorname); }
		return wxDIR_IGNORE;
	}

private:
	wxArrayString* res;
	wxArrayString* err;
};

//---------------------------------------------------------------------------
// Class MyZipTraverser
//---------------------------------------------------------------------------
class MyZipTraverser : public wxDirTraverser
{
public:
	MyZipTraverser(wxArrayString* files, wxArrayString* dirs) : fil(files), dir(dirs) {}

	virtual wxDirTraverseResult OnDir(const wxString& dirname)
	{
		dir->Add(dirname);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		fil->Add(filename);
		return wxDIR_CONTINUE;
	}

private:
	wxArrayString* fil;
	wxArrayString* dir;
};

//---------------------------------------------------------------------------
// Class NormalizeDialog
//---------------------------------------------------------------------------
class NormalizeDialog : public wxDialog
{
public:
	NormalizeDialog(wxWindow* parent);
	virtual ~NormalizeDialog() {}

	wxTextCtrl* path;
	wxComboBox* format;
	wxCheckBox* norm_env_vars;
	wxCheckBox* norm_dots;
	wxCheckBox* norm_tilde;
	wxCheckBox* norm_case;
	wxCheckBox* norm_absolute;
	wxCheckBox* norm_long;
	wxCheckBox* norm_shortcut;
	wxCheckBox* norm_all;
};

//---------------------------------------------------------------------------
// Class PermissionDialog
//---------------------------------------------------------------------------
class PermissionDialog : public wxDialog
{
public:
	PermissionDialog(wxWindow* parent, const wxString& file);
	virtual ~PermissionDialog() {}

	wxCheckBox* urun;
	wxCheckBox* uread;
	wxCheckBox* uwrite;
	wxCheckBox* grun;
	wxCheckBox* gread;
	wxCheckBox* gwrite;
	wxCheckBox* orun;
	wxCheckBox* oread;
	wxCheckBox* owrite;
};


#endif
