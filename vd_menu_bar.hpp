#pragma once

namespace vd {

class menu_bar {
	QMainWindow *	m_main_window;

public:
	menu_bar( QMainWindow * p_main_window ) : m_main_window( p_main_window ) { Q_ASSERT( m_main_window ); }

#if 0
	QMenu * add_menu( const QString & name ) {
		return m_main_window->menuBar()->addMenu( name );
	}

	template <typename QObjectT, typename SlotT>
	void add_menu_item( QMenu * p_menu, const QString & name, QObjectT * p_obj, SlotT && slot ) {
		QAction * p_act = new QAction( name, m_main_window );
		QObject::connect( p_act, &QAction::triggered, p_obj, std::forward<SlotT>( slot ) );
		p_menu->addAction( p_act );
		//return p_menu;
	}
#endif

	template <typename QObjectT, std::size_t... Is, typename Tuple>
	constexpr void add_impl( QObjectT * p_obj, QMenu * p_menu, std::index_sequence <Is...>, Tuple t ) {
		([&]{
			const char * name = std::get<Is * 2    >( std::move( t ) );
			auto         slot = std::get<Is * 2 + 1>( std::move( t ) );
			if ( !name ) {
				p_menu->addSeparator();
			} else {
				QAction * p_act = new QAction( name, m_main_window );
				QObject::connect( p_act, &QAction::triggered, p_obj, slot );
				p_menu->addAction( p_act );
			}
		}(), ... );
	}

	template <typename QObjectT, typename... Args>
	constexpr void add( QObjectT * p_obj, const QString & name, Args ...args ) {
		static_assert( sizeof...( args ) % 2 == 0 ); // Number of arguments must be even
		add_impl( p_obj, m_main_window->menuBar()->addMenu( name ),
			std::make_index_sequence<sizeof...( Args ) / 2>{},
			std::tuple( std::move( args )... ) );
	}
}; // class menu_bar

} // namespace vd
