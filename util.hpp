#pragma once

namespace util {

static void save_png( graphics_view * p_view ) {
	// TODO: bring up file dialog

	QSize  canvas_sz( p_view->get_canvas_size() );
	QPoint canvas_pt( p_view->get_canvas_pos() );
	QRect  canvas_rc( canvas_pt, canvas_sz );

	QImage file( canvas_sz, QImage::Format_RGB32 );
	QPainter painter( &file );

	painter.setRenderHints( p_view->renderHints() );

	// Renders the source rect, which is in view coordinates, from the scene into target, which is in paint device coordinates, using painter.
	// This function is useful for capturing the contents of the view onto a paint device, such as a QImage (e.g., to take a screenshot), or for printing to QPrinter.
	// ...
	// If target is a null rect, the full dimensions of painter's paint device (e.g., for a QPrinter, the page size) will be used.
	p_view->render( &painter, QRectF() /*target*/, canvas_rc /*source*/ );

	file.save( "output.png" );
}

} // namespace util
