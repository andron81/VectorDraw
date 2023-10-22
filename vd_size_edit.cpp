#include "vd_size_edit.hpp"
#include "configuration.hpp"

extern cf_::Configuration * cfg;

    void vd::size_edit::focusInEvent(QFocusEvent* e){
		qDebug()<<"focus on my_QLineEdit";

		if (find_item->item){
			
		QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(find_item->item);
		oldPen = findLine->pen();
		findLine->setPen(cfg->getSavedValue().m_focus_line_pen);	
		//findLine->setFocus();
		}	

    QLineEdit::focusInEvent(e);
	}
	
	void vd::size_edit::keyPressEvent(QKeyEvent *e){

		if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {

			if (find_item->item) {	
			QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(find_item->item);	
			QLineF lineTmp=findLine->line();
			float widthf=text().toFloat();
			if (abs(lineTmp.x1()-lineTmp.x2())>0)//hori
			
			lineTmp.setP2(QPointF(widthf+lineTmp.p1().x(),lineTmp.p1().y()));
			
			else 								   //vert
			lineTmp.setP2(QPointF(lineTmp.p1().x(),lineTmp.p1().y()+widthf));
			findLine->setLine(lineTmp);
			}

		}
		QLineEdit::keyPressEvent(e);
	}
	

	
	void vd::size_edit::focusOutEvent(QFocusEvent * e) {
		if (find_item->item) {
			QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(find_item->item);	
			QLineF lineTmp=findLine->line();
			float widthf=text().toFloat();
			if (abs(lineTmp.x1()-lineTmp.x2())>0)//hori
			
			lineTmp.setP2(QPointF(widthf+lineTmp.p1().x(),lineTmp.p1().y()));
			
			else 								   //vert
			lineTmp.setP2(QPointF(lineTmp.p1().x(),lineTmp.p1().y()+widthf));
			findLine->setLine(lineTmp);
			findLine->setPen(oldPen);
			find_item->item=nullptr;			
		}
		setVisible(false);
		cfg->getSavedValue().layout->edit_block_visible(false,nullptr) ;
    QLineEdit::focusOutEvent(e);
	}