/* AbiWord
 * Copyright (C) 2000 AbiSource, Inc.
 * Copyright (C) 2004 Hubert Figuiere
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#ifndef XAP_UNIXDIALOG_LISTDOCUMENTS_H
#define XAP_UNIXDIALOG_LISTDOCUMENTS_H

#include "xap_Dlg_ListDocuments.h"

class XAP_Frame;

/*****************************************************************/

class XAP_UnixDialog_ListDocuments: public XAP_Dialog_ListDocuments
{
public:
	XAP_UnixDialog_ListDocuments(XAP_DialogFactory * pDlgFactory, XAP_Dialog_Id id);
	virtual ~XAP_UnixDialog_ListDocuments(void);

	virtual void			runModal(XAP_Frame * pFrame);

	static XAP_Dialog *		static_constructor(XAP_DialogFactory *, XAP_Dialog_Id id);

protected:
private:
	void event_View(void);
	void event_Cancel(void);
	GtkWidget * _constructWindow(void);
	void _populateWindowData(void);
	static void s_list_dblclicked(GtkTreeView *treeview,
				  GtkTreePath *arg1,
				  GtkTreeViewColumn *arg2,
				  XAP_UnixDialog_ListDocuments * me);

	GtkWidget *m_listWindows;
	GtkWidget *m_windowMain;
};

#endif /* XAP_UNIXDIALOG_LISTDOCUMENTS_H */
