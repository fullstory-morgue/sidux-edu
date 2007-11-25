/*
 * unique.cpp
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "unique.h"
#include "systray.h"

#include <kcmdlineargs.h>
#include <kdebug.h>

#include <qstring.h>



int Unique::newInstance()
{
	KUniqueApplication::newInstance();
	
	kdDebug() << "newInstance" << endl;
	
	// handle command line arguments
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

	args->clear();

	// exit value for the calling process
	return 0;
}
