/*
 * edu.cpp
 *
 * Copyright (c) 2007 Fabian Wuertz
 * edu is based on knxcc_kmain from Andreas Loible
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


#include <kgenericfactory.h>

#include <klocale.h>
#include <kiconloader.h>
#include <kapp.h>
#include <kprocess.h>

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qwidgetstack.h>
#include <qlistbox.h>
#include <qtextbrowser.h>
#include <qcombobox.h>

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

#include "edu.h"

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
	addtionalPushButton->hide();
	widgetStack->raiseWidget(3);

	// setup leftmenu
	categoriesListView->setAlternateBackground( QColor(237, 244, 249) );
	categoriesListView->header()->hide();


}


//------------------------------------------------------------------------------
//-- load apps and categories at startup ---------------------------------------
//------------------------------------------------------------------------------


void edu::getAllApps()
{
	listView->clear();
	QStringList apps = QDir( "/usr/share/sidux-edu/apps").entryList( QDir::Files );


	QStringList seminarixApps;
    QFile file( "/usr/share/sidux-edu/list/seminarix.list" );
    if ( file.open( IO_ReadOnly ) ) {
        QTextStream stream( &file );
        QString line;
        int i = 1;
        while ( !stream.atEnd() ) {
            line = stream.readLine(); // line of text excluding '\n'
            printf( "%3d: %s\n", i++, line.latin1() );
            seminarixApps += line;
        }
        file.close();
    }


	for( QStringList::Iterator it = apps.begin(); it != apps.end(); ++it )
	{
		KDesktopFile file( "/usr/share/sidux-edu/apps/"+*it );
		//app.append( file.readComment() );

		QListViewItem * item = new QListViewItem( listView, 0 );
		//item->setPixmap(0, statusImg);
		item->setText( 0, file.readName() );
		item->setText( 1, file.readIcon() );
		item->setText( 2, file.readEntry("Exec") );
		item->setText( 3, file.readEntry("Package") );
		item->setText( 4, file.readEntry("Description") );
		item->setText( 5, file.readEntry("Homepage") );
		item->setText( 6, file.readEntry("Categories") );
		if( file.tryExec() )
			item->setText( 7, "TRUE" );
		else
			item->setText( 7, "FALSE" );
		if( seminarixApps.contains( file.readEntry("Package") ) )
			item->setText( 8, "TRUE" );
		else
			item->setText( 8, "FALSE" );
	}


}

void edu::getCategories()
{
	QStringList categories = QDir( "/usr/share/sidux-edu/categories").entryList( QDir::Files );


	QPixmap icon;
	for( QStringList::Iterator it = categories.begin(); it != categories.end(); ++it )
	{
		KDesktopFile file( "/usr/share/sidux-edu/categories/"+*it );
		icon = loader->loadIcon( file.readIcon(), KIcon::Desktop, 36);

		KListViewItem * item = new KListViewItem( categoriesListView, 0 );
		item->setText(  0, file.readName() );
		item->setPixmap(0, icon );
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
	descriptionTextBrowser->clear();

	QString category = "Seminarix-"+categoriesListView->selectedItems().first()->text(0);

	QStringList names;
	QStringList icons;
	QListViewItemIterator it( listView );
	if( comboBox->currentItem() == 0 )
		while ( it.current() )
		{
			if ( it.current()->text(6).contains(category) and it.current()->text(7) == "TRUE" )
			{
				names.append( it.current()->text(0) );
				icons.append( it.current()->text(1) );
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
			}
			++it;
		}

	QPixmap icon;
	for(uint i = 0; i < names.count(); i++)
	{
		icon = loader->loadIcon( icons[i], KIcon::Desktop, 36);
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
	QListViewItemIterator it( listView );
	while ( it.current() ) {
		if ( it.current()->text(0).contains( searchLineEdit->text(), FALSE ) or it.current()->text(4).contains( searchLineEdit->text(), FALSE ) )
		{
			names.append( it.current()->text(0) );
			icons.append( it.current()->text(1) );
		}
		++it;
	}

	QPixmap icon;
	for(uint i = 0; i < names.count(); i++)
	{
		icon = loader->loadIcon( icons[i], KIcon::Desktop, 36);
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
			run.append( "siduxEduInstallApp" );
			run.append( package );
	
			// run command
			terminal()->startProgram( "siduxEduInstallApp", run );
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
		addtionalPushButton->hide();
	else
		addtionalPushButton->show();
}

void edu::disableButtons()
{
	execPushButton->hide();
	homepagePushButton->hide();
	addtionalPushButton->hide();
}


void edu::additionalPackage()
{
	KMessageBox::information(this, "Zusaetzliches Paket" );
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
	kapp->invokeBrowser( "/usr/share/sidux-manual/index.html" );
}

void edu::seminarixManual()
{
	kapp->invokeBrowser( "/usr/share/seminarix-handbuch/de/index.html" );
}

void edu::homepage()
{
	kapp->invokeBrowser( "http://www.sidux.com/" );
}

#include "edu.moc"
