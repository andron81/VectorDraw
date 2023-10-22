#pragma once

#include "vd_view_painter.hpp"





namespace vd {

class size_edit  : public QLineEdit{
    Q_OBJECT
	vd::painter::point_and_QGraphicsItem* find_item;
	QPen oldPen;
	public:
	size_edit(const QString &contents, QWidget *parent = nullptr):
	QLineEdit(contents, parent),find_item(nullptr), oldPen(nullptr){}
	void set_finditem(vd::painter::point_and_QGraphicsItem* find_item_) {find_item=find_item_;}
    protected:
    void focusInEvent(QFocusEvent*);
	void focusOutEvent(QFocusEvent *);
	void keyPressEvent(QKeyEvent *event);
		 
};
}