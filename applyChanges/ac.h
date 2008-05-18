/*
 * ac.h
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

#ifndef AC_H_
#define AC_H_


#include <kde_terminal_interface.h>
#include <kparts/part.h>

#include <acdialog.h>


class ac : public AcDialog
{
	Q_OBJECT

	public:
		ac(const QString &changes = QString(), QWidget *parent = 0L, const char *name = 0L, const QStringList &foo = QStringList());
		QString appdir;
		QString changes2;

		int j;
		int step;
		bool firstdownload;


		// console
		ExtTerminalInterface *terminal()
		{
			return static_cast<ExtTerminalInterface*>(konsole->qt_cast( "ExtTerminalInterface" ) );
		}
		virtual bool eventFilter( QObject *o, QEvent *e );


	public slots:
		virtual void details();
		virtual void shellExited(const QString&);

	protected:
		void loadKonsole();
		KParts::Part *konsole;

};


#endif
