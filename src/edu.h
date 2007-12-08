/*
 * edu.h
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef EDU_H_
#define EDU_H_

#include <edudialog.h>


#include <kde_terminal_interface.h>
#include <kparts/part.h>
#include <kiconloader.h>


class edu : public EduDialog
{
	Q_OBJECT

	public:
		edu(QWidget *parent = 0L, const char *name = 0L, const QStringList &foo = QStringList());
		void load();
		void getCategories();
		void getAllApps();

		KIconLoader *loader;

		// console
		ExtTerminalInterface *terminal()
		{
			return static_cast<ExtTerminalInterface*>(konsole->qt_cast( "ExtTerminalInterface" ) );
		}
		virtual bool eventFilter( QObject *o, QEvent *e );


	public slots:
		virtual void getApps();
		virtual void getDescription();
		virtual void execApp();
		virtual void showHomepage();
		virtual void back();
		virtual void enableButtons();
		virtual void disableButtons();
		virtual void about();
		virtual void aboutKDE();
		virtual void siduxManual();
		virtual void seminarixManual();
		virtual void homepage();
		virtual void searchApp();
		virtual void openUrl(const QString&);

	protected:
		void loadKonsole();
		KParts::Part *konsole;

};

#endif
