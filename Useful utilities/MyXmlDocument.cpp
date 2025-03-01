#include "MyXmlDocument.h"
#include <wx/log.h>

wxXmlNode* MyXmlDocument::AddRoot(const wxString &name)
{
    wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, name);
    this->SetRoot(root);
    return root;
}

void MyXmlDocument::AddTopChild(wxXmlNode* child)
{
    this->GetRoot()->AddChild(child);
}

wxXmlNode* MyXmlDocument::SimpleNode(const wxString &name)
{
    return new wxXmlNode(wxXML_ELEMENT_NODE, name);
}

wxXmlNode* MyXmlDocument::TextValueNode(const wxString &name, const wxString &value)
{
    wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, name);
    node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, value));
    return node;
}

wxXmlNode* MyXmlDocument::AttrTextValueNode(const wxString &name, const wxString &value, int attrs, ...)
{
    int i;
    wxString node_name = wxEmptyString;
    attrs *= 2;
    char* tmp = NULL;

    wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, name);

    va_list args;
    va_start(args, attrs);
    for (i = 0; i < attrs; i++)
    {
        tmp = (char*)("");
        tmp = va_arg(args, char*);

        if (i == 0 || i % 2 == 0)
        {
            node_name = wxString(tmp);
        }
        else
        {
            node->AddAttribute(node_name, wxString(tmp));
        }
    }
    va_end(args);

    if (!value.IsEmpty()) // Dodanie wartoœci tekstowej, jeœli ró¿na od wxEmptyString
    {
        node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, value));
    }

    tmp = (char*)("");

    return node;
}

void MyXmlDocument::AddVersion(const wxString &release, const wxString &major, const wxString &minor, const wxString &build)
{
    this->GetRoot()->AddChild(AttrTextValueNode("version", wxEmptyString, 4, "release", release.ToStdString().c_str(), "major", major.ToStdString().c_str(), "minor", minor.ToStdString().c_str(), "build", build.ToStdString().c_str()));
}

void MyXmlDocument::AddSettings(const wxString& set_name, const wxString& value)
{
    this->GetRoot()->AddChild(AttrTextValueNode("settings", value, 1, "name", set_name.ToStdString().c_str()));
}

bool MyXmlDocument::LoadAndCheck(const wxString &fileName, const wxString &rootName, bool show_error_msg)
{
    if (!this->Load(fileName))
    {
        if (show_error_msg)
        {
            wxMessageBox(wxT("Odczyt pliku się nie powiódł!"), wxT("Błąd!"), wxOK | wxICON_ERROR);
        }
        return false;
    }

    if (!this->GetRoot()->GetName().IsSameAs(rootName))
    {
        if (show_error_msg)
        {
            wxMessageBox(wxT("Niewłaściwy plik!"), wxT("Błąd!"), wxOK | wxICON_ERROR);
        }
        return false;
    }

    return true;
}

wxXmlNode* MyXmlDocument::GetFirst()
{
    return this->GetRoot()->GetChildren();
}

bool MyXmlDocument::SaveXML(const wxString &fileName)
{
    return this->Save(fileName, 4);
}



