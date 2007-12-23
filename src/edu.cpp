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

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
#include <qlistbox.h>
#include <qtextbrowser.h>
#include <qcombobox.h>
#include <qgroupbox.h>

#include <klistview.h>
#include <klistbox.h>
#include <kmessagebox.h>

#include <qheader.h>
#include <qstatusbar.h>
#include <kaboutapplication.h>
#include <kaboutkde.h>


#include <kdesktopfile.h>
#include <kdirlister.h>
#include <qdir.h>
#include <kconfigbase.h>

#include "edu.h"


#include "kconfigbackend.h"

edu::edu(QWidget *parent, const char *name, const QStringList &)
:EduDialog(parent, name)
{
	loader = KGlobal::iconLoader();
	load();
}


void edu::load()
{
	statusBar()->hide();

	getCategories();
	getAllApps();

	loadKonsole();
	konsoleFrame->installEventFilter( this );
	execPushButton->hide();
	homepagePushButton->hide();
	additionalGroupBox->hide();
	widgetStack->raiseWidget(3);
	//widgetStack->raiseWidget(2);

	// setup leftmenu
	categoriesListView->setAlternateBackground( QColor(237, 244, 249) );
	categoriesListView->header()->hide();


}


//------------------------------------------------------------------------------
//-- load apps and categories at startup ---------------------------------------
//------------------------------------------------------------------------------


void edu::getCategories()
{
	QStringList categories = QDir( "/usr/share/sidux-edu/categories").entryList( QDir::Files );

	QPixmap icon;
	for(uint i =  categories.count(); i > 0; i--)
	{
		KDesktopFile file( "/usr/share/sidux-edu/categories/"+categories[i-1] );
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

void edu::getAllApps()
{
	listView->clear();
	QStringList apps = QDir( "/usr/share/sidux-edu/apps").entryList( QDir::Files );

	QString line;


	// get basic seminarix apps
	QStringList seminarixApps;
	QFile file1( "/usr/share/sidux-edu/list/seminarix.list" );
	file1.open( IO_ReadOnly );
	QTextStream stream1( &file1 );
	while ( !stream1.atEnd() )
	{
		line = stream1.readLine(); // line of text excluding '\n'
		seminarixApps += line;
	}
	file1.close();



	for( QStringList::Iterator it = apps.begin(); it != apps.end(); ++it )
	{

		// create listitem
		QListViewItem * item = new QListViewItem( listView, 0 );

		// mpam file
		int found = 0;
		QString desktopPath;
		QString package;
		QString exec;
		QFile file2( "/usr/share/sidux-edu/apps/"+*it );
		file2.open( IO_ReadOnly );
		QTextStream stream2( &file2 );
		while ( !stream2.atEnd() and found < 6 )
		{
			line = stream2.readLine(); // line of text excluding '\n'
			if( line.contains("Name=") )
			{
				item->setText( 0, line.mid(5) );
				found++;
			}
			else if( line.contains("Package=") )
			{
				package = line.mid(8);
				item->setText( 3, package );
				found++;
			}
			else if ( line.contains("DesktopPath=") )
			{
				desktopPath = line.mid(12);
				if( desktopPath != "console" and desktopPath != "none")
					found++;
			}
			else if ( line.contains("Categories=") )
			{
				item->setText( 6, line.mid(11) );
				found++;
			}
			else if ( line.contains("Homepage=") )
			{
				item->setText( 5, line.mid(9) );
				found++;
			}
			else if ( line.contains("Description=") )
			{
				item->setText( 4, line.mid(12) );
				found++;
			}
			else if ( line.contains("Exec=") )
			{
				exec = line.mid(5);
				found++;
			}
		}
		file2.close();

		if( seminarixApps.contains( package ) )
			item->setText( 8, "TRUE" );
		else
			item->setText( 8, "FALSE" );


	
		if( desktopPath == "console" )
		{
			item->setText( 2, "x-terminal-emulator --noclose -e "+exec );
			if( QFile::exists(exec) )
				item->setText( 7, "TRUE" );
			else
				item->setText( 7, "FALSE" );
		}
		else if( desktopPath == "none" )
		{
			item->setText( 2, exec );
			if( QFile::exists(exec) )
				item->setText( 7, "TRUE" );
			else
				item->setText( 7, "FALSE" );
		}
		else
		{
			// *.desktop file
			KDesktopFile file3( desktopPath );
			item->setText( 1, file3.readIcon() );
			QString exec = QStringList::split( " ", file3.readEntry("Exec") )[0];
			item->setText( 2, exec );
			if( file3.readEntry("Exec") != "" )
				item->setText( 7, "TRUE" );
			else
				item->setText( 7, "FALSE" );
		}

	}


}



//------------------------------------------------------------------------------
//-- main widget ---------------------------------------------------------------
//------------------------------------------------------------------------------

void edu::getApps()
{

	if( categoriesListView->selectedItems().count() < 1)
		return;

	widgetStack->raiseWidget(0);

	appsListBox->clear();

	QString category = categoriesListView->selectedItems().first()->text(0);

	if( category == "Start")
	{
		widgetStack->raiseWidget(3);
		return;
	}

	//descriptionTextBrowser->clear();
	descriptionTextBrowser->setSource("/usr/share/sidux-edu/html/"+category+".html");


	QStringList names;
	QStringList icons;
	QStringList packages;
	QListViewItemIterator it( listView );
	if( comboBox->currentItem() == 0 )
		while ( it.current() )
		{
			if ( it.current()->text(6).contains(category) and it.current()->text(7) == "TRUE" )
			{
				names.append( it.current()->text(0) );
				icons.append( it.current()->text(1) );
				packages.append( it.current()->text(3) );
			}
			++it;
		}
	else if( comboBox->currentItem() == 2 )
		while ( it.current() )
		{
			if ( it.current()->text(6).contains(category) and it.current()->text(7) == "FALSE" )
			{
				names.append( it.current()->text(0) );
				icons.append( it.current()->text(1) );
				packages.append( it.current()->text(3) );
			}
			++it;
		}
	else
		while ( it.current() )
		{
			if ( it.current()->text(6).contains(category) )
			{
				names.append( it.current()->text(0) );
				icons.append( it.current()->text(1) );
				packages.append( it.current()->text(3) );
			}
			++it;
		}

	QPixmap icon;
	for(uint i = 0; i < names.count(); i++)
	{
		icon = loader->loadIcon( icons[i], KIcon::Desktop, 32, KIcon::DefaultState, 0L, TRUE);
		if (icon.isNull() )
		{
			if( QFile::exists("/usr/share/pixmaps/"+packages[i]+".xpm") )
				icon = QPixmap("/usr/share/pixmaps/"+packages[i]+".xpm");
			else
				icon = loader->loadIcon( "seminarix_empty", KIcon::Desktop, 32);
		}
		appsListBox->insertItem( icon, names[i]);
	}

	disableButtons();
}

void edu::searchApp()
{

	widgetStack->raiseWidget(0);

	appsListBox->clear();
	descriptionTextBrowser->clear();

	QStringList names;
	QStringList icons;
	QStringList packages;
	QListViewItemIterator it( listView );
	while ( it.current() ) {
		if ( it.current()->text(0).contains( searchLineEdit->text(), FALSE ) or it.current()->text(4).contains( searchLineEdit->text(), FALSE ) )
		{
			names.append( it.current()->text(0) );
			icons.append( it.current()->text(1) );
			packages.append( it.current()->text(3) );
		}
		++it;
	}

	QPixmap icon;
	for(uint i = 0; i < names.count(); i++)
	{
		icon = loader->loadIcon( icons[i], KIcon::Desktop, 32, KIcon::DefaultState, 0L, TRUE);
		if (icon.isNull() )
		{
			if( QFile::exists("/usr/share/pixmaps/"+packages[i]+".xpm") )
				icon = QPixmap("/usr/share/pixmaps/"+packages[i]+".xpm");
			else
				icon = loader->loadIcon( "seminarix_empty", KIcon::Desktop, 32);
		}
		appsListBox->insertItem( icon, names[i]);
	}

	disableButtons();
}


void edu::getDescription()
{
	QString app = appsListBox->currentText();
	QString description = listView->findItem(app, 0, Qt::ExactMatch )->text(4);
	descriptionTextBrowser->setText( "<b>"+app+"</b><br><br>"+description );

	enableButtons();
}

void edu::execApp()
{
	QString app = appsListBox->currentText();
	QString exec = listView->findItem(app, 0, Qt::ExactMatch )->text(2);
	QString package = listView->findItem(app, 0, Qt::ExactMatch )->text(3);
	QString isInstalled = listView->findItem(app, 0, Qt::ExactMatch )->text(7);


	if( isInstalled == "TRUE")
	{
		KProcess *proc = new KProcess;
		*proc << QStringList::split( " ", exec );
		proc->start();	
	}
	else
	{
		if(KMessageBox::Yes == KMessageBox::questionYesNo(this, "Das Programm "+appsListBox->currentText()+" ist nicht installiert! Moechten sie es aus dem Internet herunterladen und installieren?")  )
		{
			categoriesListView->hide();
			searchLineEdit->hide();
			widgetStack->raiseWidget(1);
			QStrList run;
			run.append( "/usr/share/sidux-edu/sh/appinstaller" );
			run.append( package );
	
			// run command
			terminal()->startProgram( "/usr/share/sidux-edu/sh/appinstaller", run );
			connect( konsole, SIGNAL(destroyed()), SLOT( back() ) );
		}
	}

}

void edu::back()
{
	categoriesListView->show();
	searchLineEdit->show();
	widgetStack->raiseWidget(0);
	loadKonsole();
	konsoleFrame->installEventFilter( this );
	getAllApps();
	getApps();
}


void edu::showHomepage()
{
	QString app = appsListBox->currentText();
	QString homepage = listView->findItem(app, 0, Qt::ExactMatch )->text(5);
	kapp->invokeBrowser( homepage );
}


void edu::enableButtons()
{
	QString app = appsListBox->currentText();
	QString package = listView->findItem(app, 0, Qt::ExactMatch )->text(3);
	QString isInstalled = listView->findItem(app, 0, Qt::ExactMatch )->text(7);

	if( isInstalled == "FALSE" )
		execPushButton->setText("Programm installieren");
	else
		execPushButton->setText("Programm starten");

	execPushButton->show();

	QString homepage = listView->findItem(app, 0, Qt::ExactMatch )->text(5);
	if( homepage != "")
		homepagePushButton->show();
	else
		homepagePushButton->hide();


	QString isSeminarix = listView->findItem(app, 0, Qt::ExactMatch )->text(8);
	if( isSeminarix == "TRUE" )
		additionalGroupBox->hide();
	else
		additionalGroupBox->show();
}

void edu::disableButtons()
{
	execPushButton->hide();
	homepagePushButton->hide();
	additionalGroupBox->hide();
}

void edu::openUrl(const QString& url)
{
	// E.g. /usr/share/sidu x-edu/html/http://www.kde.org
	if( url.mid(26, 4) == "http" )
	{
		descriptionTextBrowser->setSource("/usr/share/sidux-edu/html/"+ categoriesListView->selectedItems().first()->text(0)+".html");
		kapp->invokeBrowser( url.mid(26) );
	}

}




//------------------------------------------------------------------------------
//--- load console -------------------------------------------------------------
//------------------------------------------------------------------------------




void edu::loadKonsole()
{
	KLibFactory* factory = KLibLoader::self()->factory( "libsanekonsolepart" );
	if (!factory)
		factory = KLibLoader::self()->factory( "libkonsolepart" );
	konsole = static_cast<KParts::Part*>( factory->create( konsoleFrame, "konsolepart", "QObject", "KParts::ReadOnlyPart" ) );
	terminal()->setAutoDestroy( true );
	terminal()->setAutoStartShell( false );
	konsole->widget()->setGeometry(0, 0, konsoleFrame->width(), konsoleFrame->height());	
}

bool edu::eventFilter( QObject *o, QEvent *e )
{
	// This function makes the console emulator expanding
	if ( e->type() == QEvent::Resize )
	{
		QResizeEvent *re = dynamic_cast< QResizeEvent * >( e );
		if ( !re ) return false;
		konsole->widget()->setGeometry( 0, 0, re->size().width(), re->size().height() );
	}
	return false;
};


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

#include "edu.moc"
