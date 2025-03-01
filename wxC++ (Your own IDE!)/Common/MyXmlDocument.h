/////////////////////////////////////////////////////////////////////////////
// Name:        MyXmlDocument.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-01-01
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MYXMLDOCUMENT_H_
#define _MYXMLDOCUMENT_H_

#include <wx/msgdlg.h>
#include <wx/xml/xml.h>

class MyXmlDocument : public wxXmlDocument
{
public:
    MyXmlDocument() : wxXmlDocument() {}

    wxXmlNode* AddRoot(const wxString &name);
    void AddTopChild(wxXmlNode* child);
    wxXmlNode* SimpleNode(const wxString &name);
    wxXmlNode* TextValueNode(const wxString &name, const wxString &value);
    wxXmlNode* AttrTextValueNode(const wxString &name, const wxString &value, int attrs, ...);
    void AddVersion(const wxString &release, const wxString &major, const wxString &minor, const wxString &build);
    void AddSettings(const wxString& set_name, const wxString& value);
    bool LoadAndCheck(const wxString &fileName, const wxString &rootName, bool show_error_msg = true);
    wxXmlNode* GetFirst();
    bool SaveXML(const wxString &fileName);
};


#endif
