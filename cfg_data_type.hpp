#pragma once

namespace vd {
	class layout;
} 

class MainWindow;

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
	qint8 mode; //0 - normal program start mode , 1 - start program from 1c
	QString hs; //canvas size
};	

namespace vd::util {
} // namespace vd::util
