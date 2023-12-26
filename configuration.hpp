#pragma once
#include "vd_layout.hpp"
#include "VectorDraw.hpp"

namespace cf_ {
struct cfg_data_type{
	vd::layout* layout;
	MainWindow* m_window;
	qint8 pen_width;
	QPen m_line_pen;
	QPen m_focused_line_pen;	
	QPen m_size_pen;
	QPen m_focused_size_pen;
	QColor selected_color;
	QColor default_color;
	

};	
class Configuration
{
    static Configuration *_instance;
    cfg_data_type  _savedValue ;

public:

    static Configuration* Instance();
	
	
    cfg_data_type& getSavedValue();
	void setValue(cfg_data_type data) ;
private:
    Configuration();
};
};

