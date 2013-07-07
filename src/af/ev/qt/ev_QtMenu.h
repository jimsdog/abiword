/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */
/* AbiWord
 * Copyright (C) 2004-2006 Tomas Frydrych <dr.tomas@yahoo.co.uk>
 * Copyright (C) 2009 Hubert Figuiere
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

#ifndef EV_QTMENU_H
#define EV_QTMENU_H

#include "ut_types.h"
#include "ut_vector.h"
#include "xap_Types.h"
#include "ev_Menu.h"

#include <QMenu>
#include <QWidget>
#include <QVector>

class AV_View;
class XAP_QtApp;
class XAP_QtFrameImpl;
class XAP_Frame;

/*****************************************************************/

class EV_QtMenu : public EV_Menu
{
public:
	EV_QtMenu(XAP_QtApp * pQtApp,
		    XAP_Frame * pFrame,
		    const char * szMenuLayoutName,
		    const char * szMenuLabelSetName);
	virtual ~EV_QtMenu();

	bool				synthesizeMenu(QMenu * menuRoot, bool isPopup);
	bool				menuEvent(XAP_Menu_Id id);
	virtual bool		refreshMenu(AV_View * pView) = 0;

 	XAP_Frame * 	getFrame();

protected:
	bool				_refreshMenu(AV_View * pView, QMenu * menuRoot);
	bool				_isItemPresent(XAP_Menu_Id id) const;
	virtual bool		_doAddMenuItem(UT_uint32 layout_pos);

protected: // FIXME! These variables should be private.
	XAP_QtApp *		m_pQtApp;
	XAP_Frame *  	m_pFrame;

	// actual QT menu widgets
	QVector<QWidget*> m_vecMenuWidgets;
private:
        QMenu * s_createNormalMenuEntry(const XAP_Menu_Id id,
											bool isCheckable,
											bool isRadio,
											bool isPopup,
											const char *szLabelName,
											const char *szMnemonicName);
	std::vector<void *>           m_vecCallbacks;
};

#endif /* EV_QTMENU_H */
