#include "configuration.hpp"
#include "vd_size_edit.hpp"


extern cf_::Configuration * cfg;

    void vd::size_edit::focusInEvent(QFocusEvent* e){
		//nothing yet here


    QLineEdit::focusInEvent(e);
	}
	
	void vd::size_edit::remove_finditem(){
		
		if (find_item && find_item->item) {
			if (find_item->item->type()==6) {
			static_cast<vd::items::myline *>(find_item->item)->setPen(cfg->getSavedValue().m_line_pen);
				find_item->item=nullptr;			
				cfg->getSavedValue().layout->edit_block_visible(false);				
			}
			else 
			
			if (find_item->item->type()==-500) {
			static_cast<vd::items::size *>(find_item->item)->setBlackColor();
				find_item->item=nullptr;			
				cfg->getSavedValue().layout->edit_block_visible(false);				
			}
			else 
			
			if (find_item->item->type()==-510) {
			
				find_item->item=nullptr;			
				cfg->getSavedValue().layout->edit_block_visible(false);				
			}
		
		}
	}
	
	void vd::size_edit::keyPressEvent(QKeyEvent *e){		
		if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) 
			vd::size_edit::change_size();
		QLineEdit::keyPressEvent(e);
	}
	
	void vd::size_edit::change_size(){
		
				if (find_item->item) {
				
			if (find_item->item->type()==6){
				float widthf=text().toFloat();
				vd::items::myline * findLine = static_cast<vd::items::myline *>(find_item->item);				
				QLineF lineTmp=findLine->line();
				
				if (abs(lineTmp.x1()-lineTmp.x2())>0)//hori
				
				lineTmp.setP2(QPointF(widthf+lineTmp.p1().x(),lineTmp.p1().y()));
			
				else 								   //vert
				lineTmp.setP2(QPointF(lineTmp.p1().x(),lineTmp.p1().y()+widthf));
				findLine->setLine(lineTmp);
				find_item->firstCoord=lineTmp;
				find_item->point= QPointF(lineTmp.p1().x(),lineTmp.p1().y() );			
				} else
			if (find_item->item->type()==-500){	
				float widthf=text().toFloat();
				vd::items::size * findSize = static_cast<vd::items::size *>(find_item->item);				
					vd::items::size::triangle_type size_coord = findSize->get_coord();
					QLineF main_line = findSize->get_main_line();
					if (main_line.x1()==main_line.x2()) //vert
					{
						findSize->setY2(size_coord.y1 + widthf)	;
						findSize->update();
						
					}
					else 
					if (main_line.y1()==main_line.y2()) //hori
					{
						
						findSize->setX2(size_coord.x1 + widthf)	;
						findSize->update();
					}

					

						
				} else
				if (find_item->item->type()==-510){		
				vd::items::text * txt = static_cast<vd::items::text *>(find_item->item);
				txt->setPlainText(text())				;
				

					

						
				}				
			}
		
	}
	
	void vd::size_edit::focusOutEvent(QFocusEvent * e) {
		vd::size_edit::change_size();
		QLineEdit::focusOutEvent(e);
	}