/*
 * edu.cpp
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

#include <klocale.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <kprocess.h>
#include <kaboutapplication.h>
#include <kaboutkde.h>
#include <klistview.h>
#include <klistbox.h>
#include <kmessagebox.h>
#include <kdesktopfile.h>
#include <kdirlister.h>
#include <qdir.h>
#include <kconfigbase.h>

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
#include <qlistbox.h>
#include <qtextbrowser.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qregexp.h>
#include <qheader.h>
#include <qstatusbar.h>



#include <fancylistviewitem.h>
#include <edu.h>


#include "kconfigbackend.h"

edu::edu(QWidget *parent, const char *name, const QStringList &)
:EduDialog(parent, name)
{
	loader = KGlobal::iconLoader();
	iconpath = loader->theme()->dir();
	appdir   = "/usr/share/sidux-edu/";


	statusBar()->hide();

	getCategories();
	importApps();

	execPushButton->hide();
	homepagePushButton->hide();
	examplePushButton->hide();
	widgetStack->raiseWidget(3);
	//widgetStack->raiseWidget(2);

	// setup leftmenu
	categoriesListView->setAlternateBackground( QColor(237, 244, 249) );
	categoriesListView->header()->hide();

	appsListView->header()->hide();

}


//------------------------------------------------------------------------------
//-- load apps and categories at startup ---------------------------------------
//------------------------------------------------------------------------------


void edu::getCategories()
{
	QStringList categories = QDir( appdir+"categories").entryList( QDir::Files );

	QPixmap icon;
	for(uint i =  categories.count(); i > 0; i--)
	{
		KDesktopFile file( appdir+"categories/"+categories[i-1] );
		icon = loader->loadIcon( file.readIcon(), KIcon::Desktop, 36);

		KListViewItem * item = new KListViewItem( categoriesListView, 0 );
		item->setText(  0, file.readName() );
		item->setPixmap(0, icon );
	}

	categoriesListView->setSorting(-1);
	icon = loader->loadIcon( "seminarix_start", KIcon::Desktop, 36);
	KListViewItem * item1 = new KListViewItem( categoriesListView, 0 );
	item1->setText(  0, "Start" );
	item1->setPixmap(0, icon );

}

void edu::importApps()
{
	listView->clear();
	QStringList apps = QDir( appdir+"apps").entryList( QDir::Files );

	QString line;



	for( uint i = 0; i < apps.count(); i++ )
	{

		// create listitem
		QListViewItem * item = new QListViewItem( listView, 0 );

		//id
		QString id = QString(apps[i]).replace(".pmap", "");
		item->setText( 0, id);


		// mpam file
		int found = 0;
		QString desktopPath;
		QString package;
		QString exec;
		QFile file2( appdir+"apps/"+apps[i] );
		file2.open( IO_ReadOnly );
		QTextStream stream2( &file2 );
		while ( !stream2.atEnd() and found < 6 )
		{
			line = stream2.readLine(); // line of text excluding '\n'
			if( line.contains("Name=") )
			{
				item->setText( 1, line.mid(5) );
				found++;
			}
			else if ( line.contains("Categories=") )
			{
				item->setText( 2, line.mid(11) );
				found++;
			}
			else if( line.contains("Package=") )
			{
				package = line.mid(8);
				item->setText( 3, package );
				found++;
			}
			else if ( line.contains("Exec=") )
			{
				exec = line.mid(5);
				found++;
			}
			else if ( line.contains("DesktopPath=") )
			{
				desktopPath = line.mid(12);
				if( desktopPath != "console" and desktopPath != "none")
					found++;
			}
			else if ( line.contains("Description=") )
			{
				item->setText( 6, line.mid(12) );
				found++;
			}
			else if ( line.contains("Homepage=") )
			{
				item->setText( 7, line.mid(9) );
				found++;
			}


		}
		file2.close();

	
		if( desktopPath == "console" )
		{
			item->setText( 8, "T" ); // terminal
			item->setText( 9, id );  // icon
		}
		else if( desktopPath == "none" )
		{
			item->setText( 8, "F" ); // terminal
			item->setText( 9, id );  // icon
		}
		else
		{
			// *.desktop file
			KDesktopFile file3( desktopPath );
			QString exec = QStringList::split( " ", file3.readEntry("Exec") )[0];
			item->setText( 5, exec );
			// terminal
			item->setText( 8, "F" ); // terminal
			item->setText( 9, file3.readIcon() ); // icon
		}

		//status
		if( QFile::exists( "/usr/share/doc/"+package+"/copyright" ) )
			item->setText( 4, "installed");
		else
			item->setText( 4, "notinstalled");

		//hasExample
		QString packageDir = "/usr/share/seminarix-samples/"+QStringList::split( " ", package )[0];
		if( QFile::exists(packageDir) )
			item->setText( 10, "TRUE" );
		else
			item->setText( 10,  "FALSE" );
	}


}



//------------------------------------------------------------------------------
//-- main widget ---------------------------------------------------------------
//------------------------------------------------------------------------------

void edu::showCategoryApps()
{

	if( categoriesListView->selectedItems().count() < 1)
		return;

	widgetStack->raiseWidget(0);

	appsListView->clear();

	QString category = categoriesListView->selectedItems().first()->text(0);

	if( category == "Start")
	{
		widgetStack->raiseWidget(3);
		return;
	}

	//descriptionTextBrowser->clear();
	descriptionTextBrowser->setSource(appdir+"html/"+category+".html");


	QStringList id;
	QStringList names;
	QStringList icons;
	QStringList status;


	QListViewItemIterator it( listView );
	if( comboBox->currentItem() == 1 ) // just installed programms
		while ( it.current() )
		{
			QString status0 = it.current()->text(4);
			if( status0 == "installed" or  status0 == "remove" )
				if( it.current()->text(2).contains(category) )
				{
					id.append(     it.current()->text(0) );
					names.append(  it.current()->text(1) );
					icons.append(  it.current()->text(9) );
					status.append( status0 );
				}
			++it;
		}
	else if( comboBox->currentItem() == 2 )
		while ( it.current() )
		{
			QString status0 = it.current()->text(4);
			if( status0 == "notinstalled" or status0 == "install" )
				if( it.current()->text(2).contains(category) )
				{
					id.append(     it.current()->text(0) );
					names.append(  it.current()->text(1) );
					icons.append(  it.current()->text(9) );
					status.append( status0 );
				}
			++it;
		}
	else
		while ( it.current() )
		{
			if ( it.current()->text(2).contains(category) )
			{
				id.append(     it.current()->text(0) );
				names.append(  it.current()->text(1) );
				status.append( it.current()->text(4) );
				icons.append(  it.current()->text(9) );
			}
			++it;
		}




	for(uint i = 0; i < names.count(); i++)
	{
		QPixmap ico = getIcon(icons[i]);

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

void edu::searchApp()
{

	appsListView->clear();
	descriptionTextBrowser->clear();

	QStringList names;
	QStringList id;
	QStringList status;
	QStringList icons;
	QListViewItemIterator it( listView );
	while ( it.current() ) {
		if ( it.current()->text(1).contains( searchLineEdit->text(), FALSE ) or it.current()->text(6).contains( searchLineEdit->text(), FALSE ) )
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
		QPixmap ico = getIcon(icons[i]);

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



void edu::showApp()
{

	if (!appsListView->selectedItem()) return;
	QString app = appsListView->selectedItem()->text(2);

	// Show description
	QString description =  listView->findItem(app, 1, Qt::ExactMatch )->text(6);
	descriptionTextBrowser->setText( "<b>"+app+"</b><br><br>"+description );

	// Show homepage buttons
	QString homepage = listView->findItem(app, 1, Qt::ExactMatch )->text(7);
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

void edu::changed()
{
	QListViewItemIterator it( appsListView );
	QString tmp;

	for ( ; it.current(); ++it )
	{

		QString name = it.current()->text(2);
		QString status = listView->findItem(name, 1, Qt::ExactMatch )->text(4);
		QString id = listView->findItem(name, 1, Qt::ExactMatch )->text(0);
		QString icon = listView->findItem(name, 1, Qt::ExactMatch )->text(9);
		QPixmap ico = getIcon(icon);

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


void edu::execApp()
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


void edu::showHomepage()
{
	QString app = appsListView->selectedItem()->text(2);
	kapp->invokeBrowser( listView->findItem(app, 1, Qt::ExactMatch )->text(7) );
}

void edu::copyExample()
{
	if(KMessageBox::Yes == KMessageBox::questionYesNo(this, "Sollen die Beispieldateien auf dem Desktop angezeigt werden?")  )
	{
		QString app = appsListView->selectedItem()->text(2);
		QString package = listView->findItem(app, 0, Qt::ExactMatch )->text(3);
		package = QStringList::split( " ", package )[0];
		KProcess *proc = new KProcess;
		*proc << appdir+"sh/copyExample" << package;
		proc->start();
	}

}

void edu::disableButtons()
{
	execPushButton->hide();
	homepagePushButton->hide();
	examplePushButton->hide();
}

void edu::openUrl(const QString& url)
{
	// E.g. /usr/share/sidu x-edu/html/http://www.kde.org
	if( url.mid(26, 4) == "http" )
	{
		descriptionTextBrowser->setSource( appdir+"html/"+ categoriesListView->selectedItems().first()->text(0)+".html");
		kapp->invokeBrowser( url.mid(26) );
	}

}



//------------------------------------------------------------------------------
//-- aplly Changes -------------------------------------------------------------
//------------------------------------------------------------------------------



void edu::applyChanges1()
{
	
	categoriesListView->hide();
	widgetStack->raiseWidget(1);
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
			QPixmap ico;
			if( QFile::exists( iconpath+"32x32/apps/"+icon+".png" ) )
				ico = QPixmap(  iconpath+"32x32/apps/"+icon+".png" );
			else
				ico = QPixmap( appdir+"icons/"+id+".png" );
			installListBox->insertItem( ico, name);

		}
		if( status == "remove" )
		{
			QString name = it.current()->text(1);
			QPixmap ico;
			if( QFile::exists( iconpath+"32x32/apps/"+icon+".png" ) )
				ico = QPixmap(  iconpath+"32x32/apps/"+icon+".png" );
			else
				ico = QPixmap( appdir+"icons/"+id+".png" );
			removeListBox->insertItem( ico, name);
		}
		++it;
	}


}


void edu::applyChanges2()
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
	*proc << "kdesu" << "sidux-edu-applyChanges" << changedPackages;
	proc->start();
	setEnabled(FALSE);
	connect(proc, SIGNAL(processExited(KProcess *)), this, SLOT( back() ));


}


void edu::cancel()
{
	categoriesListView->show();
	widgetStack->raiseWidget(0);
}

void edu::back()
{
	setEnabled(TRUE);
	categoriesListView->show();
	widgetStack->raiseWidget(0);

	//update status
	QStringList tmpList = QStringList::split( " ", changedPackages );
	for( QStringList::Iterator it = tmpList.begin(); it != tmpList.end(); ++it )
	{
		QString package = QString(*it).mid(1);
		if( QFile::exists( "/usr/share/doc/"+package+"/copyright" ) )
		{
			listView->findItem(package, 3, Qt::ExactMatch )->setText(4, "installed");
			execPushButton->show();
		}
		else
		{
			listView->findItem(package, 3, Qt::ExactMatch )->setText(4, "notinstalled");
			execPushButton->hide();
		}
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
		QPixmap ico;
		QString id     = listView->findItem(names[i], 1, Qt::ExactMatch )->text(0);
		QString status = listView->findItem(names[i], 1, Qt::ExactMatch )->text(4);
		QString icon   = listView->findItem(names[i], 1, Qt::ExactMatch )->text(9);


		if( QFile::exists( iconpath+"32x32/apps/"+icon+".png" ) )
			ico = QPixmap(  iconpath+"32x32/apps/"+icon+".png" );
		else
			ico = QPixmap( appdir+"icons/"+id+".png" );

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


//------------------------------------------------------------------------------
//-- menu ----------------------------------------------------------------------
//------------------------------------------------------------------------------

void edu::about()
{
	KAboutApplication* about = new KAboutApplication ( this );
	about->show();
}

void edu::aboutKDE()
{
	KAboutKDE* about = new KAboutKDE ( this );
	about->show();
}

void edu::siduxManual()
{
	KProcess *proc = new KProcess;
	*proc << "x-www-browser" << "/usr/share/sidux-manual/index.html";
	proc->start();
}

void edu::seminarixManual()
{
	KProcess *proc = new KProcess;
	*proc << "x-www-browser" << "/usr/share/seminarix-handbook/index.html";
	proc->start();
}

void edu::homepage()
{
	kapp->invokeBrowser( "http://www.sidux.com/" );
}


QPixmap edu::getIcon(QString icon)
{
	QPixmap ico = loader->loadIcon( icon, KIcon::Desktop, 32, KIcon::DefaultState, 0L, TRUE);
	if (icon.isNull() )
	{
		if( QFile::exists ("/usr/share/app-install/icons/"+icon+".png") )
			ico = QPixmap("/usr/share/app-install/icons/"+icon+".png");
		else if( QFile::exists("/usr/share/app-install/icons/"+icon+".xpm") )
			ico = QPixmap("/usr/share/app-install/icons/"+icon+".xpm");
		else if( QFile::exists("/usr/share/pixmaps/"+icon+".xpm") )
			ico = QPixmap("/usr/share/pixmaps/"+icon+".xpm");
		else if( QFile::exists("/usr/share/pixmaps/"+icon+"-icon.xpm") )
			ico = QPixmap("/usr/share/pixmaps/"+icon+"-icon.xpm");
		else if( QFile::exists("/usr/share/"+icon+"/"+icon+".xpm") )
			ico = QPixmap("/usr/share/"+icon+"/"+icon+".xpm");
		else if( QFile::exists("/usr/share/"+icon+"/pixmaps/"+icon+".xpm") )
			ico = QPixmap("/usr/share/"+icon+"/pixmaps/"+icon+".xpm");
		else if( icon == "wxmaxima")
			ico = QPixmap("/usr/share/wxMaxima/icons/maximaicon.xpm");
		else
			ico = QPixmap( appdir+"images/empty.png" );
	}

	if( ico.height() != 32 and ico.width() != 32 )
	{
		ico = QPixmap( appdir+"images/empty.png" );
	}
	return ico;
}

#include "edu.moc"
