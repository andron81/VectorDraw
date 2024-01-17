#pragma once
#include "vd_size_edit.hpp"
#include "vd_view.hpp"
#include <QVector>


	
namespace vd {



// All classes that contain signals or slots must mention Q_OBJECT at the top of their declaration. They must also derive (directly or indirectly) from QObject.
class layout : public QObject {
	Q_OBJECT

	QMainWindow *		m_parent;
	QLineEdit *			m_edit_width;
	QLineEdit *			m_edit_height;
	vd::size_edit *	 	m_edit_width_item;
	QVector<QPushButton *> not_toggled_buttons;
	QGraphicsScene *	m_scene;
	vd::view *			m_view;
	QLabel* size_element_label;	
	QSize get_size() const {
		return {
			m_edit_width ->text().toInt(),
			m_edit_height->text().toInt() };
	}

public:
	layout( QMainWindow * p_parent);	
	void set_size(QSize sz) const {
		m_view->set_canvas_size( sz );
	}
	void edit_block_visible(bool visible, vd::painter::point_and_QGraphicsItem* find_item_=nullptr){
		m_edit_width_item->set_finditem(find_item_);
		
		if (find_item_ && find_item_->item)	{
		switch (find_item_->item->type()) {
			case 6 : {
			vd::items::myline * findobject = static_cast<vd::items::myline *>(find_item_->item);	
			if (abs(findobject->line().x1()-findobject->line().x2())>0)
			m_edit_width_item->setText(QString::number(abs(findobject->line().x1()-findobject->line().x2())));
			else
			m_edit_width_item->setText(QString::number(abs(findobject->line().y1()-findobject->line().y2())));
				 } break;
			case -500 : {				
			vd::items::size * findobject = static_cast<vd::items::size *>(find_item_->item);	
			QLineF main_line = findobject->get_main_line();
			if (abs(main_line.x1()-main_line.x2())>0)
			m_edit_width_item->setText(QString::number(abs(main_line.x1()-main_line.x2())));
			else
			m_edit_width_item->setText(QString::number(abs(main_line.y1()-main_line.y2())));
				 } break;
			case -510 : {				
			vd::items::text * findobject = static_cast<vd::items::text *>(find_item_->item);	

			m_edit_width_item->setText(findobject->toPlainText());
				 } break;		
				 }
			}
			size_element_label->setVisible(visible);
			m_edit_width_item->setVisible(visible);
			for (auto iter=not_toggled_buttons.begin(); iter!=not_toggled_buttons.end(); iter++)
				(*iter)->setEnabled(visible);			
	}
	vd::size_edit  *	 get_m_edit_width_item() {return m_edit_width_item;}
	

	const QGraphicsScene * get_scene() const { return m_scene; }
	QGraphicsScene *       get_scene()       { return m_scene; }

	const vd::view *       get_view()  const { return m_view;  }
	vd::view *             get_view()        { return m_view;  }

private slots:
	void size_changed() {		
		m_view->set_canvas_size( get_size() );
	}
}; // class layout

}; // namespace vd
