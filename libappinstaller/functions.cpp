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





#include <functions.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qpixmap.h>
#include <qimage.h>


Functions::Functions()
{
	loader = KGlobal::iconLoader();
	iconpath = loader->theme()->dir();
}


QPixmap Functions::getIcon(QString iconname)
{
	iconname = iconname.replace(".png", "").replace(".xpm", "");

	QPixmap ico( iconpath+"32x32/apps/"+iconname+".png");
	//if (ico.isNull() )
	//	ico = QPixmap( loader->loadIcon( iconname, KIcon::Desktop, 32, KIcon::DefaultState, 0L, TRUE) );

	if (ico.isNull() )
	{
		if( QFile::exists ( "/usr/share/app-install/+icons/"+iconname+".png" ) )
			ico = QImage( "/usr/share/app-install/icons/"+iconname+".png" ).smoothScale( 32, 32, QImage::ScaleMin );
		else if( QFile::exists( "/usr/share/app-install/icons/"+iconname+".xpm" ) )
			ico = QImage( "/usr/share/app-install/icons/"+iconname+".xpm" ).smoothScale( 32, 32, QImage::ScaleMin );
		else
			ico = loader->loadIcon( iconname, KIcon::Desktop, 32, KIcon::DefaultState, 0L, TRUE);

		if (ico.isNull() )
		{
			if( QFile::exists("/usr/share/pixmaps/"+iconname+".xpm") )
				ico = QImage("/usr/share/pixmaps/"+iconname+".xpm").smoothScale( 32, 32, QImage::ScaleMin );
			else if( QFile::exists("/usr/share/pixmaps/"+iconname+"-icon.xpm") )
				ico = QImage("/usr/share/pixmaps/"+iconname+"-icon.xpm").smoothScale( 32, 32, QImage::ScaleMin );
			else if( QFile::exists("/usr/share/"+iconname+"/"+iconname+".xpm") )
				ico = QImage("/usr/share/"+iconname+"/"+iconname+".xpm").smoothScale( 32, 32, QImage::ScaleMin );
			else if( QFile::exists("/usr/share/"+iconname+"/pixmaps/"+iconname+".xpm") )
				ico = QImage("/usr/share/"+iconname+"/pixmaps/"+iconname+".xpm").smoothScale( 32, 32, QImage::ScaleMin );
			else
			{
				if( QFile::exists("/usr/share/sidux-edu/icons/empty.png") )
					ico = QPixmap( "/usr/share/sidux-edu/icons/empty.png" );
				else
					ico = QPixmap( "/usr/share/kappinstaller/images/empty.png" );
			}
		}

	}

	return ico;
}
