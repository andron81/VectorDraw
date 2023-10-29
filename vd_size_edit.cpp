#include "vd_size_edit.hpp"
#include "configuration.hpp"

extern cf_::Configuration * cfg;

    void vd::size_edit::focusInEvent(QFocusEvent* e){
		qDebug()<<"focus on my_QLineEdit";

		if (find_item->item){
			
		QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(find_item->item);
		//oldPen = findLine->pen();
		findLine->setPen(cfg->getSavedValue().m_focused_line_pen);	
		//findLine->setFocus();
		}	

    QLineEdit::focusInEvent(e);
	}
	
	void vd::size_edit::remove_finditem(){
		if (find_item && find_item->item) {static_cast<QGraphicsLineItem *>(find_item->item)->setPen(cfg->getSavedValue().m_line_pen);
				find_item->item=nullptr;			
				cfg->getSavedValue().layout->edit_block_visible(false);				
				}
	}
	
	void vd::size_edit::keyPressEvent(QKeyEvent *e){		
		if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) 
			vd::size_edit::change_size();
		QLineEdit::keyPressEvent(e);
	}
	
	void vd::size_edit::change_size(){
		
				if (find_item->item) {
			qDebug()<<"focusOutEvent is good";
			QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(find_item->item);	
			QLineF lineTmp=findLine->line();
			float widthf=text().toFloat();
			if (abs(lineTmp.x1()-lineTmp.x2())>0)//hori
			
			lineTmp.setP2(QPointF(widthf+lineTmp.p1().x(),lineTmp.p1().y()));
			
			else 								   //vert
			lineTmp.setP2(QPointF(lineTmp.p1().x(),lineTmp.p1().y()+widthf));
			findLine->setLine(lineTmp);
			find_item->firstCoord=lineTmp;
			find_item->point= QPointF(lineTmp.p1().x(),lineTmp.p1().y() );
			//findLine->setPen(oldPen);
			//find_item->item=nullptr;			
		}
		//setVisible(false);
		
	}
	
	void vd::size_edit::focusOutEvent(QFocusEvent * e) {
		qDebug()<<"focusOutEvent";
		vd::size_edit::change_size();
		QLineEdit::focusOutEvent(e);
	}