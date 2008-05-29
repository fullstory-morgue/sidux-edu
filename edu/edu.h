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



#include <kde_terminal_interface.h>
#include <kparts/part.h>
#include <kiconloader.h>

#include <../libappinstaller/functions.h>
#include <edudialog.h>



class edu : public EduDialog
{
	Q_OBJECT

	public:
		edu(QWidget *parent = 0L, const char *name = 0L, const QStringList &foo = QStringList());
		void getCategories();
		void importApps();

		QString appdir;
		QString iconpath;
		QString changedPackages;
		QString getDescription(QString);

		Functions *myFunctions;
		KIconLoader *loader;


	public slots:
		virtual void showCategoryApps();
		virtual void showApp();
		virtual void execApp();
		virtual void changed();
		virtual void showHomepage();
		virtual void copyExample();
		virtual void disableButtons();
		virtual void about();
		virtual void aboutKDE();
		virtual void siduxManual();
		virtual void seminarixManual();
		virtual void homepage();
		virtual void searchApp();
		virtual void applyChanges1();
		virtual void applyChanges2();
		virtual void cancel();
		virtual void back();
		virtual void openUrl(const QString&);


};

#endif
