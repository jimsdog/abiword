/* AbiWord
 * Copyright (C) 1998 AbiSource, Inc.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */

/*****************************************************************
******************************************************************
** IT IS IMPORTANT THAT THIS FILE ALLOW ITSELF TO BE INCLUDED
** MORE THAN ONE TIME.  Each time you add an entry to the top-half
** of this file be sure to add a corresponding entry to the other
** half and be sure to add an entry to each of the other platforms.
******************************************************************
*****************************************************************/

#ifndef AP_UNIXDIALOG_ALL_H

#	define AP_UNIXDIALOG_ALL_H

#	include "xap_UnixDialog_MessageBox.h"
#	include "xap_UnixDialog_FileOpenSaveAs.h"
#	include "xap_UnixDialog_Print.h"
#	include "xap_UnixDialog_FontChooser.h"

	// ... add new dialogs here ...

#else

	DeclareDialog(XAP_DIALOG_ID_MESSAGE_BOX,	AP_UnixDialog_MessageBox)
	DeclareDialog(XAP_DIALOG_ID_FILE_OPEN,		AP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_FILE_SAVEAS,	AP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_PRINT,			AP_UnixDialog_Print)
	DeclareDialog(XAP_DIALOG_ID_PRINTTOFILE,	AP_UnixDialog_FileOpenSaveAs)
	DeclareDialog(XAP_DIALOG_ID_FONT,			AP_UnixDialog_FontChooser)
	
	// ... also add new dialogs here ...

#endif
