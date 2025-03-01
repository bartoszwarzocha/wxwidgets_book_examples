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

WX_DEFINE_ARRAY_PTR(wxThread*, wxThreadPool);

//---------------------------------------------------------------------------
// Class MyApp
//---------------------------------------------------------------------------
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

    wxThreadPool thread_pool;
    wxCriticalSection cs;
};

DECLARE_APP(MyApp);

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

    void OnConnect(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);
    void OnMkDir(wxCommandEvent& event);
    void OnRmDir(wxCommandEvent& event);
    void OnRmFile(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnDownload(wxCommandEvent& event);
    void OnUpload(wxCommandEvent& event);

    void OnListActivated(wxListEvent& event);

private:
    void SetFTPStatusInfo()
    {
        SetStatusText(wxT("FTP: ") + ftp.GetLastResult(), 1);
    }

    void RefreshList()
    {
        list->DeleteAllItems();

        wxArrayString files;
        ftp.GetFilesList(files);

        list->InsertItem(0, wxT(".."));

        int index = 1;
        int size = 0;
        for (wxString s : files)
        {
            list->InsertItem(index, s);

            size = ftp.GetFileSize(s);
            if (size > 0)
            {
                list->SetItem(index, 1, wxString::Format(wxT("%d"), size));
                list->SetItemFont(index, list->GetItemFont(index).MakeBold());
            }
            else
            {
                list->SetItem(index, 1, wxT("<DIR>"));
                list->SetItemFont(index, list->GetItemFont(index).MakeItalic());
            }
            index++;
        }

        wxLogMessage(wxT("Read items: %d"), files.size());
    }

    wxTextCtrl* address;
    wxTextCtrl* port;
    wxTextCtrl* user;
    wxTextCtrl* passwd;
    wxTextCtrl* log;

    wxButton* conn_btn;
    wxButton* dwn_btn;
    wxButton* up_btn;

    wxListView* list;

    //
    wxFTP ftp;

    DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------------------------
// Class DownloadThread
//---------------------------------------------------------------------------
class DownloadThread : public wxThread
{
public:
    DownloadThread(wxFTP* f, const wxString& s, const wxString& d) : wxThread(wxTHREAD_JOINABLE), ftp(f), src(s), dest(d) {}

    virtual wxThread::ExitCode Entry()
    {
        wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());
        wxLogMessage(wxT("Downloading '%s' started... (Thread ID: %lld)"), src, this->GetId());

        // Determine size based on file size...
        //long long size = ftp->GetFileSize(src);
        //char* buffer = new char[size];

        int read = 0;
        char buffer[1024];
        memset(buffer, 0x00, sizeof(buffer));

        wxInputStream* is = ftp->GetInputStream(src);
        wxFileOutputStream* os = new wxFileOutputStream(dest + wxT("/") + wxFileNameFromPath(src));

        if (is)
        {
            do
            {
                is->Read(buffer, 1024);
                os->Write(buffer, is->LastRead());
                read = is->LastRead();
            } while (read > 0);
        }
        else
        {
            wxLogError(wxT("Cannot download file!"));
        }

        //wxDELETEA(buffer); // When dynamic buffer size
        wxDELETE(is);
        wxDELETE(os);

        wxLogMessage(wxT("Downloading '%s' finished..."), src);

        wxGetApp().thread_pool.Remove(this);

        return (wxThread::ExitCode)0;
    }

private:
    wxFTP* ftp;
    wxString src;
    wxString dest;
};

//---------------------------------------------------------------------------
// Class UploadThread
//---------------------------------------------------------------------------
class UploadThread : public wxThread
{
public:
    UploadThread(wxFTP* f, const wxString& s) : wxThread(wxTHREAD_JOINABLE), ftp(f), src(s) {}

    virtual wxThread::ExitCode Entry()
    {
        wxLog::SetThreadActiveTarget(wxLog::GetActiveTarget());
        wxLogMessage(wxT("Uploading '%s' started... (Thread ID: %lld)"), src, this->GetId());

        int read = 0;
        char buffer[1024];
        memset(buffer, 0x00, sizeof(buffer));

        wxOutputStream* os = ftp->GetOutputStream(ftp->Pwd() + wxT("/") + wxFileNameFromPath(src));
        wxFileInputStream* is = new wxFileInputStream(src);

        if (os)
        {
            do
            {
                is->Read(buffer, 1024);
                os->Write(buffer, is->LastRead());
                read = is->LastRead();
            } while (read > 0);
        }
        else
        {
            wxLogError(wxT("Cannot upload file!"));
        }

        wxDELETE(os);
        wxDELETE(is);

        wxLogMessage(wxT("Uploading '%s' finished..."), src);

        return (wxThread::ExitCode)0;
    }

private:
    wxFTP* ftp;
    wxString src;
};

#endif
