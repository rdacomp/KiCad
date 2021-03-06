/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2016 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef __PYTHON_SCRIPTING_H
#define __PYTHON_SCRIPTING_H

// undefs explained here: https://bugzilla.redhat.com/show_bug.cgi?id=427617

#ifdef _POSIX_C_SOURCE
    #undef _POSIX_C_SOURCE
#endif
#ifdef _XOPEN_SOURCE
    #undef _XOPEN_SOURCE
#endif

#include <Python.h>
#ifndef NO_WXPYTHON_EXTENSION_HEADERS
#ifdef KICAD_SCRIPTING_WXPYTHON
    #include <wx/wxPython/wxPython.h>
#endif
#endif

#include <wx/string.h>
#include <wx/arrstr.h>

/* Function pcbnewInitPythonScripting
 * Initializes the Python engine inside pcbnew
 */

bool        pcbnewInitPythonScripting( const char * aUserScriptingPath );
void        pcbnewFinishPythonScripting();


#ifdef KICAD_SCRIPTING_WXPYTHON

void        RedirectStdio();
wxWindow*   CreatePythonShellWindow( wxWindow* parent, const wxString& aFramenameId );

class PyLOCK
{
    wxPyBlock_t b;
public:

    // @todo, find out why these are wxPython specific.  We need the GIL regardless.
    // Should never assume python will only have one thread calling it.
    PyLOCK()    { b = wxPyBeginBlockThreads(); }
    ~PyLOCK()   { wxPyEndBlockThreads( b ); }
};


#else
class PyLOCK
{
    PyGILState_STATE gil_state;
public:
    PyLOCK()      { gil_state = PyGILState_Ensure(); }
    ~PyLOCK()     { PyGILState_Release( gil_state ); }
};

#endif

wxArrayString   PyArrayStringToWx( PyObject* arr );
wxString        PyErrStringWithTraceback();

wxString        PyScriptingPath();
wxString        PyPluginsPath();

#endif    // __PYTHON_SCRIPTING_H
