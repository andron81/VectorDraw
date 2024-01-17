#include "configuration.hpp"

extern cf_::Configuration * cfg;

	void vd::items::text::mousePressEvent( QGraphicsSceneMouseEvent  * p_event ) {
		vd::tool_e tool = cfg->getSavedValue().layout->get_view()->get_m_painter().get_tool_e();
		if (tool == vd::tool_e::edit)
		is_select=true;
		
		
		;}