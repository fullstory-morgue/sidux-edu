/*
 * systray.h
 *
 * Copyright (c) 2007 Fabian Wuertz <xadras@sidux.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <kglobalaccel.h>
#include <khelpmenu.h>
#include <kpopupmenu.h>
#include <ksystemtray.h>
#include <qstring.h>

#include "edu.h"


class SysTray : public KSystemTray
{
	Q_OBJECT

	public:
		SysTray(QWidget* parent = 0, const char* name = 0);
	
	public slots:
		void siduxManual();
		void seminarixManual();
		void showAbout();
		void appStarter();
	
	private:
		KPopupMenu* menu;
		KGlobalAccel* accel;
		KHelpMenu* help;
		QString test;
		edu* dialog;

	protected:
		void mousePressEvent(QMouseEvent*);
};

#endif
