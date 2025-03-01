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

#include <wx/cmdline.h>

#include "main.h"

/***************************************************************************/
// Parameter defining the style of command line handling by the application
// 0 - Variant using wxCmdLineParser class methods to define application parameters
// 1 - Variant using wxCmdLineEntryDesc array to define application parameters
#define _USE_wxCmdLineEntryDesc_ 1
/***************************************************************************/

#if _USE_wxCmdLineEntryDesc_
static const wxCmdLineEntryDesc commands[] =
{
	{ wxCMD_LINE_SWITCH, "h", "help", "Shows this help", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
	{ wxCMD_LINE_OPTION, "n", "name", "Name to show in the app", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
	{ wxCMD_LINE_OPTION, "s", "sname", "Surname to show in the app", wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY },
	{ wxCMD_LINE_SWITCH, "u", "upper", "Make name and surname upper", wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_PARAM, "", "", "copies", wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_NONE }
};
#endif


//---------------------------------------------------------------------------
// Application implementation
//---------------------------------------------------------------------------
IMPLEMENT_APP(MyApp)

//---------------------------------------------------------------------------
// OnInit()
//---------------------------------------------------------------------------
bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

#if _USE_wxCmdLineEntryDesc_
	//=======================================================================================
	// Variant using wxCmdLineEntryDesc array to define application parameters
	//=======================================================================================

	wxCmdLineParser parser(commands, argc, argv);

#else 
	//=========================================================================================
	// Variant using wxCmdLineParser class methods to define application parameters
	//=========================================================================================

	wxCmdLineParser parser;
	parser.SetCmdLine(argc, argv);

	// or simply: wxCmdLineParser parser(argc, argv);

	parser.AddSwitch(wxT("h"), wxT("help"), wxT("Shows this help"), wxCMD_LINE_VAL_NONE | wxCMD_LINE_OPTION_HELP);
	parser.AddOption(wxT("n"), wxT("name"), wxT("Name to show in the app"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	parser.AddOption(wxT("s"), wxT("sname"), wxT("Surname to show in the app"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_OPTION_MANDATORY);
	parser.AddSwitch(wxT("u"), wxT("upper"), wxT("Make name and surname upper"), wxCMD_LINE_VAL_NONE | wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(wxT("copies"), wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL);
	parser.SetSwitchChars(wxT("-/")); // Setting characters preceding switches and options
	// ... or possibly disabling long option names...
	//parser.DisableLongOptions();

#endif

	//=======================================================================================
	// No parameters, displaying information...
	if (argc == 1)
	{
		wxMessageBox(wxT("Welcome to the application demonstrating command line handling.\n" \
			"For more information, run the program with the parameter -h, /h, --help."), wxT("Information"));
		return false;
	}

	// Parsing the command line (!!!)
	int result = parser.Parse();

	// There are parameters, checking the parsing result (1 - help, -1 - error)
	if (result == -1 || result > 0)
	{
		// Help was selected or an error occurred
		return false;
	}

	//=======================================================================================
	// Everything is OK, reading parameters...
	wxString name = wxEmptyString;
	wxString surname = wxEmptyString;
	wxCmdLineSwitchState upper = wxCMD_SWITCH_OFF;
	wxString str_copies = wxT("1");
	long copies = 1;

	parser.Found(wxT("name"), &name);
	parser.Found(wxT("sname"), &surname);
	upper = parser.FoundSwitch(wxT("upper"));
	parser.Found(wxT("copies"), &copies);

	// Parameters can only be read through string chains
	str_copies = parser.GetParam(0);
	if (!str_copies.IsEmpty())
	{
		str_copies.ToLong(&copies);
		if (copies < 1) copies = 1;
	}

	// Constructing the message using parameters
	if (upper == wxCMD_SWITCH_ON)
	{
		name.MakeUpper();
		surname.MakeUpper();
	}

	wxString msg = wxEmptyString;
	for (int i = 1; i <= copies; i++)
	{
		msg += wxString::Format(wxT("%s %s (copy: %d)\n"), name, surname, i);
	}
	wxMessageBox(msg);

	//=======================================================================================
	// Here we display the main application window, perform additional settings, etc...

	return true;
}

