#pragma once

namespace vd::items {

class text final : public QGraphicsTextItem {
public:
	text( const QPointF & pos ) {
		setPos( pos );
		setFont( QFont( "Times", 72/*, QFont::Bold*/ ) );
		setTextInteractionFlags( Qt::TextSelectableByKeyboard | Qt::TextEditable );
		setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
		//p_text->setToolTip( "Test text tooltip" );
		setPlainText( "Текст" );
	}

	void change_size_by( int points ) {
		QFont f = font();
		f.setPointSize( f.pointSize() + points );
		setFont( f );
	}
}; // class text

} // namespace vd::items
