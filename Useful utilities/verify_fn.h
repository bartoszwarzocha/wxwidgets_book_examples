/////////////////////////////////////////////////////////////////////////////
// Name:        verify_fn.h
// Purpose:     My wxWidgets app
// Author:      Imie Nazwisko <imie.nazwisko@email.org>
// Created:     2018-01-01
// Copyright:   (c) 2018 by Imie Nazwisko
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _VERIFY_FN_H_
#define _VERIFY_FN_H_

//---------------------------------------------------------------------------
// Funcja sprawdza poprawnoœæ nuemru PESEL
// Zwracane wartoœci: 1 - PESEL poprawny (mê¿czyzna), 2 - PESEL poprawny
// (kobieta), 0 - PESEL niepoprawny
//---------------------------------------------------------------------------
int wxVerifyPESEL(const wxString& PESEL)
{
	const int len = 11;

	if (PESEL.IsEmpty() || PESEL.IsNull()) return 0;

	// Sprawdzenie d³ugoœci PESEL - 11 cyfr
	if (PESEL.Len() != len) return 0;

	wxVector<long> num;

	long tmp;
	for (int i = 0; i < len; i++)
	{
		if (static_cast<wxString>(PESEL.GetChar(i)).IsNumber())
		{
			(static_cast<wxString>(PESEL.operator[](i))).ToLong(&tmp);
			num.push_back(tmp);
		}
		else return 0;
	}

	// Sprawdzanie poprawnoœci PESEL
	int peselsum = (num[0] * 1)
		+ (num[1] * 3)
		+ (num[2] * 7)
		+ (num[3] * 9)
		+ (num[4] * 1)
		+ (num[5] * 3)
		+ (num[6] * 7)
		+ (num[7] * 9)
		+ (num[8] * 1)
		+ (num[9] * 3);

	int modulo = peselsum % 10;
	if (modulo == 10) modulo = 0;

	int ctrl = 10 - modulo;

	// PESEL POPRAWNY - mê¿czyzna
	if ((num[10] == ctrl) && ((num[9] % 2) != 0))
	{
		return 1;
	}
	// PESEL POPRAWNY - kobieta
	else if ((num[10] == ctrl) && ((num[9] % 2) == 0))
	{
		return 2;
	}
	else return 0; // PESEL B£ÊDNY
}


#endif