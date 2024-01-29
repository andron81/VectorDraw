#pragma once



namespace vd{
	enum class  tool_e;
	class painter;
}	
namespace vd::items {

enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };
enum loc { up = 0, down = 1, right = 2, left = 3, none = 4 };
struct item_base {
	
	vd::painter* m_painter;
	item_base(vd::painter * m_painter_ );
	virtual int type() const = 0;
	virtual QJsonObject to_JSON() const = 0;
	QPen get_pen(qint8 style , qint8 width);
}; 

class myline final: public item_base,public QGraphicsLineItem {
	public:
enum { Type = 6 };
	QJsonObject to_JSON() const;
	myline(vd::painter *, qreal x1, qreal y1, qreal x2, qreal y2, int style,int width );
	int type() const override { return Type; }
}	;

class text  final: public item_base, public QGraphicsTextItem {
	bool is_select;
public:
	enum { Type = -510 };

	// Enable the use of qgraphicsitem_cast with this item.
	int type() const override { return Type; }

	text(vd::painter *, const QPointF & pos );
	void set_is_select(bool value);
	bool get_is_select();
	void change_size_by( int points );
	void click();
	QJsonObject to_JSON() const ;
	
	void mousePressEvent( QGraphicsSceneMouseEvent  * p_event ); 
}; // class text


class size : public item_base, public QGraphicsItem {
	qint8 mode  ; // 
	QLineF main_line; // for select tool 
	qreal x1, y1; //#1  point  
	qreal x2, y2; //#2 
	qreal x3, y3; //#3
	bool first_point_visible;	
	QPen	pen;
public:

	struct triangle_type {
	qreal x1, y1; //#1  point  
	qreal x2, y2; //#2 point
	qreal x3, y3; //#3
	};

	enum { Type = types::e_size };

	triangle_type  get_coord();
	int type() const override { return -500; }
	void set_first_point_visible(bool value) ;
	size (vd::painter *, qreal _x1, qreal _y1);

	size (vd::painter *, qreal _x1, qreal _y1,
		  qreal _x2, qreal _y2,
		  qreal _x3, qreal _y3,
		  QLineF _main_line, QPen _pen
		  );
	~size();
	QRectF boundingRect() const override;
	
	void setMode(qint8 s); 
	void setBlackColor();
	void setGreenColor(); 
	bool get_first_point_visible();
	
	qint8 getMode();
	void setX2(qreal _x2);
	void setY2(qreal _y2);
	void setX1(qreal _x1);
	void setY1(qreal _y1);
	void setX3(qreal _x3);
	void setY3(qreal _y3);
	

	QLineF get_main_line();

	
	QPointF GetNearXYOBJECT(qreal xnow, qreal ynow);
	
	
	
	void swapPoint();
	void sortPoint();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) override;

	QJsonObject to_JSON() const;
};

}; // namespace vd::items
