/*
 * ac.cpp
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
#include <qprogressbar.h>
#include <qsound.h>

#include <ac.h>


ac::ac(const QString &changes, QWidget *parent, const char *name, const QStringList &)
:AcDialog(parent, name)
{

	borderFrame->hide();
	setGeometry( x(), y(), 500, 30);
	borderFrame->setGeometry( 0, 0, 500, 400);
	konsoleFrame->setGeometry( 0, 0, 500, 400);

	appdir   = "/usr/share/kappinstaller/";

	loadKonsole();
	konsoleFrame->installEventFilter( this );

	changes2 = changes;
	printf(changes);

	QStrList run;
	run.append( appdir+"sh/applyChanges" );

	run.append( changes );

	// run command
	terminal()->startProgram( appdir+"sh/applyChanges", run );

	j=0;
	firstdownload = TRUE;

	connect( konsole, SIGNAL( receivedData( const QString& ) ), SLOT( shellExited( const QString&) ) );
	connect( konsole, SIGNAL( destroyed() ), SLOT( close() ) );
}


void ac::details()
{
	if(borderFrame->isShown() )
	{
		borderFrame->hide();
		detailsPushButton->setText( i18n("Show details") );
		setGeometry( x()+4, y()+27, 500, 30);
	}
	else
	{
		borderFrame->show();
		detailsPushButton->setText( i18n("Hide details") );
		setGeometry( x()+4, y()+27, 500, 400);
	}
}


void ac::shellExited(const QString& txt)
{
	if( ! step )
		step = 80/(QStringList::split( " ", changes2 ).count()*2);

	if( txt.contains("No update") )
		step = 100/(QStringList::split( " ", changes2 ).count()*2);
	else if( txt.contains("Update done") )
		j = 20;
	else if( txt.contains("Packages to download: ") )
	{
		int downloads = QStringList::split( " ", txt ).count()-3;
		step = 80/(QStringList::split( " ", changes2 ).count()*2+downloads*8 );
	}
	else if( txt.contains("[Y/n]") or txt.contains( i18n("[Y/n]") ) )
	{
		if( !borderFrame->isShown() )
			details();
	}
	else if( txt.contains("Get") or txt.contains("Fetched") or txt.contains(i18n("Get") ) or txt.contains("Fetched") )
	{
		if( !firstdownload )
			j += 8*step;
		else
			firstdownload = FALSE;
	}
	else if( txt.contains("Removing") or txt.contains( i18n("Removing") ) )
		j += 2*step;
	else if( txt.contains("Unpacking") or txt.contains( i18n("Setting up") ) )
		j += step;
	else if( txt.contains("<Enter>") )
	{
	 	j = 100;
		closePushButton->setEnabled(TRUE);
	}

	progressBar1->setProgress(j);
}



//------------------------------------------------------------------------------
//--- load console -------------------------------------------------------------
//------------------------------------------------------------------------------


void ac::loadKonsole()
{
	KLibFactory* factory = KLibLoader::self()->factory( "libsanekonsolepart" );
	if (!factory)
		factory = KLibLoader::self()->factory( "libkonsolepart" );
	konsole = static_cast<KParts::Part*>( factory->create( konsoleFrame, "konsolepart", "QObject", "KParts::ReadOnlyPart" ) );
	terminal()->setAutoDestroy( true );
	terminal()->setAutoStartShell( false );
	konsole->widget()->setGeometry(0, 0, konsoleFrame->width(), konsoleFrame->height());	
}


bool ac::eventFilter( QObject *o, QEvent *e )
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


#include "ac.moc"
