/*
 * systray.cpp
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

#include "systray.h"

#include <kaboutapplication.h>
#include <kiconloader.h>
#include <kkeydialog.h>
#include <qclipboard.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qtooltip.h>
#include <qtimer.h>
#include <klocale.h>
#include <kprocess.h>



SysTray::SysTray ( QWidget* parent, const char* name )
		: KSystemTray ( parent, name )
{
	QToolTip::add ( this,i18n("seminarix"));

	menu = contextMenu();
	menu->insertItem ( SmallIcon ( "sidux_book" ), "sidux Handbuch", this, SLOT ( siduxManual() ) );
	menu->insertItem ( SmallIcon ( "help" ), "Seminarix Handbch", this, SLOT ( seminarixManual() ) );
	menu->insertItem ( SmallIcon ( "kcmsystem" ), "Programmstarter", this, SLOT ( appStarter() ) );
	menu->insertSeparator();
	menu->insertItem ( SmallIcon ( "info" ), i18n("&About sidux-seminarix"), this, SLOT ( showAbout() ) );

	setPixmap ( loadIcon ( "seminarix" ) );

	dialog = new edu( );
	dialog->show();
}


void SysTray::showAbout()
{
	KAboutApplication* about = new KAboutApplication ( this );
	about->show();
}

void SysTray::appStarter()
{
   if(dialog->isVisible() )
		dialog->hide();
	else
		dialog->show();
}

void SysTray::siduxManual()
{
	KProcess *proc = new KProcess;
	*proc << "x-www-browser" << "/usr/share/sidux-manual/index.html";
	proc->start();
}

void SysTray::seminarixManual()
{
	KProcess *proc = new KProcess;
	*proc << "x-www-browser" << "/usr/share/seminarix-handbook/de/index.htmll";
	proc->start();
}

void SysTray::mousePressEvent(QMouseEvent* e)
{
	KSystemTray::mousePressEvent(e);
	if(e->button() == LeftButton)
		appStarter();
}
