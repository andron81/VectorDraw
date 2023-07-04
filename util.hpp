#pragma once

namespace util {

static void save_image( graphics_view * p_view ) {

	QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить в...", ".", "Изображения (*.png *.jpg)" );
	if ( filename.isEmpty() ) return;

	canvas * p_canvas = p_view->get_canvas();
	Q_ASSERT( p_canvas );

	QSize  canvas_sz( p_canvas->get_size() );
	QPoint canvas_pt( p_view->mapFromScene( p_canvas->get_top_left() ) );

	QImage file( canvas_sz, QImage::Format_RGB32 );
	QPainter painter( &file );

	painter.setRenderHints( p_view->renderHints() );

	// Renders the source rect, which is in view coordinates, from the scene into target, which is in paint device coordinates, using painter.
	// This function is useful for capturing the contents of the view onto a paint device, such as a QImage (e.g., to take a screenshot), or for printing to QPrinter.
	// ...
	// If target is a null rect, the full dimensions of painter's paint device (e.g., for a QPrinter, the page size) will be used.
	p_view->render( &painter, QRectF() /*target*/, QRect( canvas_pt, canvas_sz ) /*source*/ );

	file.save( filename );
}

} // namespace util
