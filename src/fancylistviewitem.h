#ifndef FANCYLISTVIEWITEM_H
#define FANCYLISTVIEWITEM_H

#include <qpainter.h>
#include <qvaluevector.h>
#include <qlistview.h>

class FancyListViewItem;
 
class FancyListViewItem : public QCheckListItem
{
	public:
		FancyListViewItem(QListView *parent, const QString &label1, Type tt = RadioButtonController)
			: QCheckListItem(parent, label1, tt)
		{}
	
		FancyListViewItem(QListViewItem *parent, const QString &label1, Type tt = RadioButtonController)
			: QCheckListItem(parent, label1, tt)
		{}
	
		void paintCell(QPainter *painter, const QColorGroup &cg, int column, int width, int align);
		int width(const QFontMetrics &fm, const QListView *lv, int column) const;
	
		QFont font(uint column) const;
		void setFont(uint column, const QFont &font);
		QColor background(uint column) const;
		void setBackground(uint column, const QColor &color);
	
	private:
		QValueVector<QFont> fonts;
		QValueVector<QColor> backgrounds;
};

#endif
