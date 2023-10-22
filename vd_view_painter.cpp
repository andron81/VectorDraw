#include "vd_items.hpp"
#include "vd_view_painter.hpp"
#include "configuration.hpp"


extern cf_::Configuration * cfg;
void vd::painter::edit_tools_on() {cfg->getSavedValue().layout->edit_block_visible(true,&findItem) ;}
void vd::painter::testy(){}
