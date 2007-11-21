/*
 * main.cpp
 *
 * Copyright (c) 2007 Fabian Wuertz
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


#include "edu.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>


static KCmdLineOptions options[] =
{
	//{ "+[URL]", I18N_NOOP( "Document to open" ), 0 },
	KCmdLineLastOption
};

int main(int argc, char **argv)
{
	// specify data for About dialog
	KAboutData* about = new KAboutData("edu", "sidux-seminarix", "");

	about->setProgramLogo( QImage("/usr/share/icons/hicolor/32x32/apps/seminarix.png") );
	about->setShortDescription( "sidux-seminarix" );
	about->setLicense(KAboutData::License_GPL_V2);
	about->setHomepage("http://sidux.com");
	about->setBugAddress("xadras@sidux.com");
	about->setCopyrightStatement("(c) 2007 sidux-edu Team");

	about->addAuthor("Fabian Würtz (xadras)", "Entwickler des Programmstarters", "xadras@sidux.com", "http://sidux.wuertz.org/");
	about->addAuthor("BlueShadow", "Paketzusammenstellung und Dokumentation" );
	about->addAuthor("Thomas Kross (captagon)", "Paketzusammenstellung und Dokumentation" );
	about->addAuthor("Dinko Sabo (cobra)", "Paketzusammenstellung und Artwork", "cobra@sidux.com");
	about->addAuthor("hendrikL", "Paketzusammenstellung und Dokumentation" );
	about->addAuthor("Roland Engert (RoEn)", "Paketzusammenstellung und Dokumentation" );

	
	about->addCredit("Stefan Lippers-Hollmann (slh)", "Paketierung" );

	KCmdLineArgs::init(argc, argv, about);
	KCmdLineArgs::addCmdLineOptions( options );
	KApplication app;
	edu *mainWin = 0;

	if (app.isRestored())
	{
		//RESTORE(edu_main);
	}
	else
	{
		// no session.. just start up normally
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

		/// @todo do something with the command line args here

		mainWin = new edu( );
		app.setMainWidget( mainWin );
		mainWin->show();

		args->clear();
	}

	// mainWin has WDestructiveClose flag by default, so it will delete itself.
	return app.exec();
}

