#pragma once

#include "vd_view_painter.hpp"







namespace vd {

class size_edit  : public QLineEdit{
    Q_OBJECT
	vd::painter::point_and_QGraphicsItem* find_item;
	QPen oldPen;
	void change_size();
	public:
	size_edit(const QString &contents, QWidget *parent = nullptr):
	QLineEdit(contents, parent),find_item(nullptr), oldPen(nullptr){}
	void set_finditem(vd::painter::point_and_QGraphicsItem* find_item_) {find_item=find_item_;}
	void remove_finditem() ;
	vd::painter::point_and_QGraphicsItem* get_find_item() {return find_item;}
    protected:
    void focusInEvent(QFocusEvent*);
	void focusOutEvent(QFocusEvent *);
	void keyPressEvent(QKeyEvent *event);
		 
};
};