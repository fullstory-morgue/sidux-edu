/*
 * kappstarter.h
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

#ifndef KAPPINSTALLER_H_
#define KAPPINSTALLER_H_


#include <kiconloader.h>
#include <ksystemtray.h>

#include "process.h"
#include "kappinstallerdialog.h"
#include "functions.h"


class kappinstaller : public KappinstallerDialog
{
	Q_OBJECT

	public:
		kappinstaller(QWidget *parent = 0L, const char *name = 0L, const QStringList &foo = QStringList());
		void importApps();
		void importApps2();
		QString appdir;
		QString datadir;
		QString iconpath;
		QString changedPackages;
		QStringList categoriesID;
		QStringList subCategoriesID;
		QString getDescription(QString);
		QString getHomepage(QString);
		QStringList getDescriptionHomepage(QString);
		QStringList readFile(QString);
		QPixmap getIcon(QString);

		KIconLoader *loader;
		Functions *myFunctions;

	private:
		KSystemTray * systray;

	public slots:
		virtual void showApp();
		virtual void changed();
		virtual void showCategoryApps();
		virtual void execApp();
		virtual void applyChanges1();
		virtual void applyChanges2();
		virtual void showHomepage();
		virtual void cancel();
		virtual void back();
		virtual void disableButtons();
		virtual void about();
		virtual void aboutKDE();
		virtual void minimize();
		virtual void searchApp();
		virtual void openUrl(const QString&);

	private:
		Process* shell;

};


#endif
