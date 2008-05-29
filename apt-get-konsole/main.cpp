/*
 * main.cpp
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

#include <ac.h>

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>



static KCmdLineOptions options[] =
{
	{ "+[CHANGES]", I18N_NOOP( "Document to open" ), 0 },
	KCmdLineLastOption
};

int main(int argc, char **argv)
{
	// specify data for About dialog
	KAboutData* about = new KAboutData("aptgetkonsole", "aptgetkonsole", "");

	about->setProgramLogo( QImage("/usr/share/icons/hicolor/32x32/apps/kappinstaller.png") );
	about->setShortDescription( "aptgetkonsole" );
	about->setLicense(KAboutData::License_GPL_V2);
	about->setHomepage("http://linux.wuertz.org");
	about->setBugAddress("xadras@sidux.com");
	about->setCopyrightStatement("(c) 2008 Fabian Wuertz");
	about->addAuthor("Fabian Würtz (xadras)", "Developer", "xadras@sidux.com", "http://linux.wuertz.org/");


	KCmdLineArgs::init(argc, argv, about);
	KCmdLineArgs::addCmdLineOptions( options );


	KApplication app;
	ac *mainWin = 0;

	// start program
	if (!app.isRestored())
	{
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		QString changes;
		for(int i = 0; i < args->count(); i++) // Counting start at 0!
			changes = QString::fromLocal8Bit(args->arg(i));



		mainWin = new ac( changes );
		app.setMainWidget( mainWin );
		mainWin->show();

		args->clear();
	}

	return app.exec();
}

