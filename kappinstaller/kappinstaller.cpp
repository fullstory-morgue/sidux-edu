/*
 * kappinstaller.cpp
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

#include <kgenericfactory.h>



#include <qpushbutton.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
#include <qlistbox.h>
#include <qtextbrowser.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlistview.h>
#include <qheader.h>
#include <qstatusbar.h>
#include <kaboutapplication.h>
#include <qdir.h>
#include <qregexp.h>
#include <qpainter.h>

#include <kmessagebox.h>
#include <kaboutkde.h>
#include <kdesktopfile.h>
#include <kdirlister.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <kprocess.h>
#include <kstandarddirs.h>



#include <klistview.h>
#include <ksystemtray.h>
#include <kappinstaller.h>
#include <../libappinstaller/fancylistviewitem.h>



kappinstaller::kappinstaller(QWidget *parent, const char *name, const QStringList &)
:KappinstallerDialog(parent, name)
{

	if( QFile::exists( "/etc/sidux-version" ) )
		KMessageBox::information( this, i18n("Please use this program just if your system is up-to-date!") );

	this->shell = new Process();
	myFunctions = new Functions();
	loader      =  KGlobal::iconLoader();
	appdir      = "/usr/share/kappinstaller/";


	// load app data
	if( QFile::exists( "/usr/share/doc/app-install-data/copyright" ) )
	{
		datadir = "/usr/share/app-install/";
		importApps2();
	}
	else
	{
		importApps();
	}
		

	statusBar()->hide();
	execPushButton->hide();
	homepagePushButton->hide();
	appsListView->header()->hide();
	categoriesListView->header()->hide();
	categoriesListView->setAlternateBackground( QColor(237, 244, 249) );


	// systray
	systray = new KSystemTray (this);
	systray->setPixmap (KGlobal::iconLoader ()->loadIcon (QString ("kappinstaller"), KIcon::NoGroup, 22));



	//widgetStack->raiseWidget(1);
}



//------------------------------------------------------------------------------
//-- load apps and categories at startup ---------------------------------------
//------------------------------------------------------------------------------

void kappinstaller::importApps()
{
	listView->clear();
	QStringList apps = QDir( appdir+"pmap").entryList( QDir::Files );

	QStringList uniqueCategories;


	for( uint i = 0; i < apps.count(); i++ )
	{

		// create listitem
		QListViewItem * item = new QListViewItem( listView, 0 );

		// var
		QString name ;
		QString id;
		QString package;
		QString cat;
		QString desktoppath;


		//id
		name = QString(apps[i]).replace(".pmap", "");
		item->setText( 1, name);
		id = name.lower();
		item->setText( 0, id);


		// pmap file
		QFile file1( appdir+"pmap/"+apps[i] );
		file1.open( IO_ReadOnly );
		QTextStream stream1( &file1 );
		while ( !stream1.atEnd() )
		{
			QString line = stream1.readLine(); // line of text excluding '\n'
			if ( line.contains("Category=") )
				cat = line.mid(9);
			else if( line.contains("Package=") )
				package = line.mid(8);
			else if ( line.contains("Homepage=") )
				item->setText( 7, line.mid(9) );
			else if ( line.contains("DesktopPath=") )
				desktoppath = line.mid(12);
		}
		file1.close();


		// search for desktop file
		if( desktoppath == "" )
		{
			if( QFile::exists( "/usr/share/applications/"+id+".desktop" ) )
				desktoppath = "/usr/share/applications/"+id+".desktop";
			else if( QFile::exists( "/usr/share/applications/kde/"+id+".desktop" ) )
				desktoppath = "/usr/share/applications/kde/"+id+".desktop";
			else if( QFile::exists( "/usr/share/applications/"+name+".desktop" ) )
				desktoppath = "/usr/share/applications/"+name+".desktop";
			else if( QFile::exists( "/usr/share/applications/kde/"+name+".desktop" ) )
				desktoppath = "/usr/share/applications/kde/"+name+".desktop";
		}


		// read data from desktop file
		if( desktoppath != "" )
		{
			KDesktopFile file( desktoppath );
	
			//exec
			item->setText(5, file.readEntry("Exec").replace("-caption", "").replace("\"%c\"", "").replace("%i", "").replace("%m", "").replace("%U", "").replace("%u", "").replace("%F", "").replace("%f", "") );

	
			// run in terminal
			if( file.readEntry("Terminal") == "yes" )
				item->setText( 8, "T");
			else
				item->setText( 8, "F");
	
			//icon
			item->setText( 9, file.readIcon() );
		}


		// category - desktop envirement
		if( cat.contains("KDE") )
			item->setText( 10, "KDE" );
		else if( cat.contains("GTK") )
			item->setText( 10, "GTK" );
		else
			item->setText( 10, "All" );

		cat = cat.replace( "KDE;", "" ).replace( "GTK;", "" );
		item->setText( 2, cat );
		if( cat != "" and uniqueCategories.grep( QRegExp( "^"+cat+"$") ).count() < 1 )
				uniqueCategories.append(cat);


		// package
		if( package == "" )
			package = id;
		item->setText( 3, package );

		//status
		if( QFile::exists( "/usr/share/doc/"+package+"/copyright" ) )
			item->setText( 4, "installed");
		else	
			item->setText( 4, "notinstalled");

		//description
		item->setText( 6, getDescription(name) );

	}

	// show categories
	QPixmap icon;
	for( QStringList::Iterator it = uniqueCategories.begin(); it != uniqueCategories.end(); ++it )
		if( QFile::exists( "/usr/share/desktop-directories/kde-"+QString(*it).lower()+".directory" ) )
		{
			KDesktopFile file( "/usr/share/desktop-directories/kde-"+QString(*it).lower()+".directory" );
			icon = loader->loadIcon( file.readIcon(), KIcon::Desktop, 32);
	
			KListViewItem * item = new KListViewItem( categoriesListView, 0 );
			item->setText(  0, file.readName() );
			item->setPixmap(0, icon );
			categoriesID.append( file.readName()+"--"+*it );
		}

}


void kappinstaller::importApps2()
{
	listView->clear();
	QStringList apps = QDir( datadir+"desktop").entryList( QDir::Files );
	apps = apps.grep(".desktop");

	QStringList blacklist = readFile( appdir+"lists/blacklist.txt" );

	QStringList exec;
	QStringList uniqueCategories;

	for(uint i = 0; i < apps.count(); i++)
	{
		QString id = QString(apps[i]).replace(".desktop", "");
		if( blacklist.grep( QRegExp("^"+id+"$") ).count() < 1 )
		{
			KDesktopFile file( datadir+"desktop/"+apps[i] );
	
			// create listitem
			QListViewItem * item = new QListViewItem( listView, 0 );
	
			//id
			item->setText( 0, id);
	
			//name
			item->setText( 1, file.readName());
	
			//category
			QString cat = ";"+file.readEntry("Categories");
			if( cat.right(1) != ";")
				cat += ";";

			if( cat.contains(";AudioVideo;") or  cat.contains(";Audio;") or  cat.contains( ";Video;" ) or cat.contains( ";HamRadio;" ))
				cat = "Multimedia";
			else if( cat.contains(";Development;") )
				cat = "Development";
			else if( cat.contains(";Education;") or cat.contains(";Science;") or cat.contains(";MedicalSoftware;") )
				cat = "Edutainment";
			else if( cat.contains(";Game;") or cat.contains(";Amusement;") )
				cat = "Games";
			else if( cat.contains(";Graphics;") )
				cat = "Graphics";
			else if( cat.contains(";Network;") )
				cat = "Internet";
			else if( cat.contains(";Office;") )
				cat = "Office";
			else if( cat.contains(";Settings;")  or cat.contains(";SystemSettings;") )
				cat = "Settingsmenu";
			else if( cat.contains(";System;") or cat.contains("Screensaver;") or cat.contains(";Emulator;") )
				cat = "System";
			else if( cat.contains(";Utility;") or cat.contains(";TextEditor;") )
				cat = "Utility";
			else
				cat = "";
			item->setText(2, cat);

			if( cat != "" and uniqueCategories.grep( QRegExp( "^"+cat+"$") ).count() < 1 )
				uniqueCategories.append(cat);


	
			//package
			QString package = file.readEntry("X-AppInstall-Package");
			if( datadir.contains( "xadras") )
				package = id;
			item->setText( 3, package);
			
	
			//status
			if( QFile::exists( "/usr/share/doc/"+package+"/copyright" ) )
			{
				item->setText( 4, "installed");
			}
			else	
			{
				item->setText( 4, "notinstalled");
			}
	
			//exec
			exec.append( file.readEntry("Exec") );
	
			//description
			item->setText( 6, getDescription(file.readName()) );
	
			//hompage
			//item->setText( 7, readFile( appdir+"homepages/"+package+".txt" )[0] );
	
			// run in terminal
			if( file.readEntry("Terminal") == "yes" )
				item->setText( 8, "T");
			else
				item->setText( 8, "F");
	
			//icon
			if( datadir.contains( "xadras") )
				item->setText( 9, id );
			else
				item->setText( 9, file.readIcon() );

			QString DE;
			// desktop envirement
			if( file.readEntry("Categories").contains("KDE") )
			{
				item->setText( 10, "KDE" );
				DE = "KDE;";
			}
			else if( file.readEntry("Categories").contains("GTK") )
			{
				item->setText( 10, "GTK" );
				DE = "GTK;";
			}
			else
				item->setText( 10, "All" );

			//QString config = 	"Category="+DE+cat;
			//QFile f1( "/home/wuertz/dev/linux.wuertz.org/projekts/kappinstaller/appdata/apps/"+id+".pmap" );
			//f1.remove();
			//f1.open( IO_Raw | IO_ReadWrite | IO_Append );
			//f1.writeBlock( config, qstrlen(config) );
			//f1.close();
		}
	}


	exec.gres("-caption", "").gres("\"%c\"", "").gres("%i", "").gres("%m", "").gres("%U", "").gres("%u", "").gres("%F", "").gres("%f", "");
	QListViewItemIterator it( listView );
	int j = 0;
	while ( it.current() )
	{
		it.current()->setText(5, exec[j]);
		++it;
		j++;
 	}


	// show categories
	QPixmap icon;
	for( QStringList::Iterator it = uniqueCategories.begin(); it != uniqueCategories.end(); ++it )
		if( QFile::exists( "/usr/share/desktop-directories/kde-"+QString(*it).lower()+".directory" ) )
		{
			KDesktopFile file( "/usr/share/desktop-directories/kde-"+QString(*it).lower()+".directory" );
			icon = loader->loadIcon( file.readIcon(), KIcon::Desktop, 32);
	
			KListViewItem * item = new KListViewItem( categoriesListView, 0 );
			item->setText(  0, file.readName() );
			item->setPixmap(0, icon );
			categoriesID.append( file.readName()+"--"+*it );
		}

}


//------------------------------------------------------------------------------
//-- main widget ---------------------------------------------------------------
//------------------------------------------------------------------------------


void kappinstaller::showCategoryApps()
{

	if( categoriesListView->selectedItems().count() < 1)
		return;

	widgetStack->raiseWidget(0);

	appsListView->clear();

	QString category = categoriesID.grep( categoriesListView->selectedItems().first()->text(0)+"--" )[0];
	category = QStringList::split( "--",  category )[1];


	QStringList id;
	QStringList names;
	QStringList icons;
	QStringList status;


	QListViewItemIterator it( listView );
	while ( it.current() )
	{

		// check status
		QString status0 = it.current()->text(4);
		QStringList checkStatus;
		checkStatus.append("T"); // All
		if( status0 == "installed" or  status0 == "remove" )
			checkStatus.append("T");
		else
			checkStatus.append("F");
		if( status0 == "notinstalled" or status0 == "install" )
			checkStatus.append("T");
		else
			checkStatus.append("F");


		// check desktop envirement
		QString de = it.current()->text(10);
		QStringList checkDe;
		if(de == "KDE")
			checkDe.append("T");
		else
			checkDe.append("F");
		if(de == "GTK")
			checkDe.append("T");
		else
			checkDe.append("F");
		checkDe.append("T"); // All
		

		if( it.current()->text(2) == category and checkStatus[comboBox->currentItem()] == "T"  and checkDe[deComboBox->currentItem()]  == "T" )
		{
			id.append(     it.current()->text(0) );
			names.append(  it.current()->text(1) );
			icons.append(  it.current()->text(9) );
			status.append( status0 );
		}
		++it;
	}



	for(uint i = 0; i < names.count(); i++)
	{
		QPixmap ico = myFunctions->getIcon(icons[i]);


		FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBoxController );
		item->setPixmap( 1, ico );
		item->setText(   2, names[i] );
		item->setTristate(FALSE);
		if( status[i] == "installed" )
			item->setOn(TRUE);
		else if( status[i] == "install")
		{
			item->setOn(TRUE);
			for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(green) );
		}
		else if( status[i] == "remove")
			for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(red) );
		appsListView->setColumnAlignment( i, Qt::AlignVCenter );
	}
	appsListView->sort();

	disableButtons();
}


void kappinstaller::searchApp()
{

	appsListView->clear();
	descriptionTextBrowser->clear();

	QStringList names;
	QStringList id;
	QStringList status;
	QStringList icons;

	this->shell->setCommand("apt-cache search "+searchLineEdit->text() );
	this->shell->start(true);
	QStringList tmp = QStringList::split( "\n", this->shell->getBuffer().stripWhiteSpace() );
	QStringList aptSearch;
	for(uint i = 0; i < tmp.count(); i++)
		 aptSearch.append( QStringList::split( " ", tmp[i] )[0] );

	QListViewItemIterator it( listView );
	while ( it.current() ) {
		if ( it.current()->text(1).contains( searchLineEdit->text(), FALSE ) or it.current()->text(6).contains( searchLineEdit->text(), FALSE ) or aptSearch.grep( QRegExp( "^"+it.current()->text(3)+"$") ).count() > 0 )
		{
			id.append(     it.current()->text(0) );
			names.append(  it.current()->text(1) );
			status.append( it.current()->text(4) );
			icons.append(  it.current()->text(9) );
		}
		++it;
	}

	
	appsListView->clear();
	for(uint i = 0; i < names.count(); i++)
	{
		QPixmap ico = myFunctions->getIcon(icons[i]);

		FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBox );
		item->setPixmap( 1, ico );
		item->setText(   2, names[i] );
		item->setTristate(FALSE);

		if( status[i] == "installed" )
			item->setOn(TRUE);
		else if( status[i] == "install")
		{
			item->setOn(TRUE);
			for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(green) );
		}
		else if( status[i] == "remove")
			for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(red) );
		appsListView->setColumnAlignment( i, Qt::AlignVCenter );
	}


	disableButtons();
}


void kappinstaller::showApp()
{

	if (!appsListView->selectedItem()) return;
	QString app = appsListView->selectedItem()->text(2);
	QString name = listView->findItem(app, 1, Qt::ExactMatch )->text(1);
	QString package = listView->findItem(app, 1, Qt::ExactMatch )->text(3);

	// Show description
	QString description = listView->findItem(app, 1, Qt::ExactMatch )->text(6);
	if( description == "" )
	{
		QStringList aptChache = getDescriptionHomepage( name );
		description = aptChache[0];
		listView->findItem(app, 1, Qt::ExactMatch )->setText(6, description);
		listView->findItem(app, 1, Qt::ExactMatch )->setText(7, aptChache[1]);
	}
	descriptionTextBrowser->setText( description );

	// Show homepage buttons
	QString homepage = listView->findItem(app, 1, Qt::ExactMatch )->text(7);
	if( homepage == "" )
	{
		homepage = getHomepage(package);
		listView->findItem(app, 1, Qt::ExactMatch )->setText(7, homepage);
	}
	if( homepage != "")
		homepagePushButton->show();
	else
		homepagePushButton->hide();

	// Show exec button
	QString status = listView->findItem(app, 1, Qt::ExactMatch )->text(4);
	if( status == "installed" or status == "remove" )
		execPushButton->show();
	else
		execPushButton->hide();


}

void kappinstaller::changed()
{
	QListViewItemIterator it( appsListView );
	QString tmp;

	for ( ; it.current(); ++it )
	{

		QString name = it.current()->text(2);
		QString status = listView->findItem(name, 1, Qt::ExactMatch )->text(4);
		QString id = listView->findItem(name, 1, Qt::ExactMatch )->text(0);
		QString icon = listView->findItem(name, 1, Qt::ExactMatch )->text(9);
		QPixmap ico = myFunctions->getIcon(icon);

		if ( ( (QCheckListItem*)it.current() )->isOn() )
		{
			if( status == "notinstalled")
			{
				appsListView->removeItem( it.current() );
				FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBox );
				item->setPixmap( 1, ico );
				item->setText(   2, name );
				item->setTristate(FALSE);
				listView->findItem(id, 0, Qt::ExactMatch )->setText(4, "install");
				item->setOn(TRUE);
				for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(green) );
			}
			else if(  status == "remove" )
			{
				appsListView->removeItem( it.current() );
				FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBox );
				item->setPixmap( 1, ico );
				item->setText(   2, name );
				item->setTristate(FALSE);
				listView->findItem(id, 0, Qt::ExactMatch )->setText(4, "installed");
				item->setOn(TRUE);
				for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(white) );
			}
		}
		else
		{
			if( status == "installed" )
			{
				appsListView->removeItem( it.current() );
				FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBox );
				item->setPixmap( 1, ico );
				item->setText(   2, name );
				item->setTristate(FALSE);
				listView->findItem(id, 0, Qt::ExactMatch )->setText(4, "remove");
				item->setOn(FALSE);
				for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(red) );
			}
			else if(  status == "install" )
			{
				appsListView->removeItem( it.current() );
				FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBox );
				item->setPixmap( 1, ico );
				item->setText(   2, name );
				item->setTristate(FALSE);
				listView->findItem(id, 0, Qt::ExactMatch )->setText(4, "notinstalled");
				item->setOn(FALSE);
				for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(white) );
			}
		}
	}

	if( listView->findItem("install", 4, Qt::ExactMatch ) or listView->findItem("remove", 4, Qt::ExactMatch ) )
		apply1PushButton->setEnabled(TRUE);
	else
		apply1PushButton->setEnabled(FALSE);

}

void kappinstaller::execApp()
{
	QString app = appsListView->selectedItem()->text(2);
	QString exec = listView->findItem(app, 1, Qt::ExactMatch )->text(5);
	QString terminal = listView->findItem(app, 1, Qt::ExactMatch )->text(8);
	if( terminal == "T" )
		exec = "x-terminal-emulator --noclose -e "+exec;

	KProcess *proc = new KProcess;
	*proc << QStringList::split( " ", exec );
	proc->start();
}


void kappinstaller::applyChanges1()
{
	
	categoriesListView->hide();
	widgetStack->raiseWidget(2);
	installListBox->clear();
	removeListBox->clear();


	QListViewItemIterator it( listView );
	while ( it.current() )
	{
		QString id = it.current()->text(0);
		QString status = it.current()->text(4);
		QString icon = it.current()->text(9);
		if( status == "install" )
		{
			QString name = it.current()->text(1);
			QPixmap ico = myFunctions->getIcon(icon);
			installListBox->insertItem( ico, name);

		}
		if( status == "remove" )
		{
			QString name = it.current()->text(1);
			QPixmap ico = myFunctions->getIcon(icon);
			removeListBox->insertItem( ico, name);
		}
		++it;
	}


}


void kappinstaller::applyChanges2()
{
	changedPackages = "";

	for(uint i = 0; i < installListBox->count(); i++)
	{
		QString name = installListBox->text(i); 
		QString package = listView->findItem(name, 1, Qt::ExactMatch )->text(3);
		changedPackages += "+"+package+" ";
	}
	for(uint i = 0; i < removeListBox->count(); i++)
	{
		QString name = removeListBox->text(i); 
		QString package = listView->findItem(name, 1, Qt::ExactMatch )->text(3);
		changedPackages += "@"+package+" ";
	}

	changedPackages = changedPackages.replace( QRegExp(" $") , "" );
	

	KProcess *proc = new KProcess;
	*proc << "kdesu" << "/usr/lib/apt-get-konsole/apt-get-konsole" << changedPackages;
	proc->start();
	setEnabled(FALSE);
	connect(proc, SIGNAL(processExited(KProcess *)), this, SLOT( back() ));


}


void kappinstaller::cancel()
{
	categoriesListView->show();
	widgetStack->raiseWidget(0);
}

void kappinstaller::back()
{
	setEnabled(TRUE);
	categoriesListView->show();
	widgetStack->raiseWidget(0);

	if( datadir.contains("app-install") )
	{
		//update status
		QListViewItemIterator jt( listView );
		while ( jt.current() )
		{
			QString package = jt.current()->text(3);
			if( QFile::exists( "/usr/share/doc/"+package+"/copyright" ) )
				jt.current()->setText( 4, "installed");
			else	
				jt.current()->setText( 4, "notinstalled");
			++jt;
		}
	}
	else
	{
		importApps();
	}

	
	// update appsListView
	QStringList names;
	QListViewItemIterator it( appsListView );
	while ( it.current() )
	{
		names.append( it.current()->text(2) );
		++it;
	}
	appsListView->clear();

	for(uint i = 0; i < names.count(); i++)
	{

		QString id     = listView->findItem(names[i], 1, Qt::ExactMatch )->text(0);
		QString status = listView->findItem(names[i], 1, Qt::ExactMatch )->text(4);
		QString icon   = listView->findItem(names[i], 1, Qt::ExactMatch )->text(9);

		QPixmap ico = myFunctions->getIcon(icon);

		FancyListViewItem * item = new FancyListViewItem( appsListView, "", QCheckListItem::CheckBox );
		item->setPixmap( 1, ico );
		item->setText(   2, names[i] );
		item->setTristate(FALSE);

		if( status == "installed" )
			item->setOn(TRUE);
		else if( status == "install")
		{
			item->setOn(TRUE);
			for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(green) );
		}
		else if( status == "remove")
			for(uint i = 0; i < 3; i++)
					item->setBackground(i, QColor(red) );
		appsListView->setColumnAlignment( i, Qt::AlignVCenter );
	}
	descriptionTextBrowser->clear();
	apply1PushButton->setEnabled(FALSE);

}


void kappinstaller::showHomepage()
{
	QString app = appsListView->selectedItem()->text(2);
	kapp->invokeBrowser( listView->findItem(app, 1, Qt::ExactMatch )->text(7) );
}



void kappinstaller::disableButtons()
{
	execPushButton->hide();
	homepagePushButton->hide();
}


void kappinstaller::openUrl(const QString& url)
{
	// E.g. /usr/share/sidux-kappinstaller/html/http://www.kde.org
	if( url.mid(26, 4) == "http" )
	{
		descriptionTextBrowser->setSource( appdir+"html/"+ categoriesListView->selectedItems().first()->text(0)+".html");
		kapp->invokeBrowser( url.mid(26) );
	}

}


//------------------------------------------------------------------------------
//-- menu ----------------------------------------------------------------------
//------------------------------------------------------------------------------


void kappinstaller::about()
{
	KAboutApplication* about = new KAboutApplication ( this );
	about->show();
}


void kappinstaller::aboutKDE()
{
	KAboutKDE* about = new KAboutKDE ( this );
	about->show();
}


void kappinstaller::minimize()
{
	this->hide();
   if(!systray->isVisible() )
		systray->show();

}


QStringList kappinstaller::readFile(QString input)
{

	QStringList lines;
	QFile file( input );
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		QString line;
		while ( !stream.atEnd() ) {
				line = stream.readLine(); // line of text excluding '\n'
				lines += line;
		}
		file.close();
	}
	return lines;
}





QString kappinstaller::getDescription(QString package)
{
	QStringList tmp;
	QString description;
	if( QFile::exists( appdir+"descriptions/"+i18n("en")+"/"+package+".txt" ) )
	{
		tmp = readFile( appdir+"descriptions/"+i18n("en")+"/"+package+".txt" );
		description = "<b>"+tmp[0]+"</b><br>";
		for(uint i = 1; i < tmp.count(); i++)
		{
			if(tmp[i] == "." )
				description += "<br>";
			else
				description += tmp[i]+"<br>";
		}
	}
	return description;
}


QStringList kappinstaller::getDescriptionHomepage(QString package)
{

	this->shell->setCommand("apt-cache show "+package);
	this->shell->start(true);
	QString input = this->shell->getBuffer().stripWhiteSpace().replace(" Homepage:", "Homepage:").replace(" Homepage:", "Homepage:");
	QString description = "<b>"+QStringList::split( "\n", input ).grep("Description:")[0].replace("Description: ", "")+"</b><br><br>";
	QStringList inputList = QStringList::split( "\n", input ).grep( QRegExp("^ ") );
	for(uint i = 0; i < inputList.count(); i++)
	{
		if(inputList[i] == "." )
			description += "<br>";
		else
			description += inputList[i]+"<br>";
	}
	QString homepage = QStringList::split( "\n", this->shell->getBuffer().stripWhiteSpace() ).grep("Homepage:")[0].replace("Homepage:", "").replace(" ", "");

	QStringList output;
	output.append(description);
	output.append(homepage);

	return output;
}

QString kappinstaller::getHomepage(QString package)
{

	this->shell->setCommand("apt-cache show "+package);
	this->shell->start(true);
	QString homepage = QStringList::split( "\n", this->shell->getBuffer().stripWhiteSpace() ).grep("Homepage:")[0].replace("Homepage:", "").replace(" ", "");
	return homepage;
}


#include "kappinstaller.moc"
