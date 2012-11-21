#ifndef _DSH_DELEGATER_H
#define _DSH_DELEGATER_H


/* Flag Field */
//#define _GCC_COMPILER
/* Flag Field */


#include <functional>
#include <algorithm>
#include <list>
#include <iterator>
#include <memory>

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 8 / 17                                              *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                   *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # delegater                                                         *
*                                                                       *
*                                                                       *
*************************************************************************/

namespace _DSH{

	/// Impl
	template< typename... _Parameters >
	class delegate_IMPL ;

	template< typename R , typename C , typename... Args >
	class global_class ;

	template<typename R  , typename... Args >
	class delegate_IMPL< R(Args...) >
	{
	public:

		typedef delegate_IMPL<R(Args...)>  _Self ;
		typedef delegate_IMPL<R(Args...)>& _Ref_Self;
		typedef delegate_IMPL<R(Args...)>* _Ptr_Self;

		typedef std::function< R(Args...)>  _Fun_type;
		typedef std::list< _Fun_type >        _Con_type ;

		delegate_IMPL()
			:pList(new _Con_type)
		{
		}

		delegate_IMPL(const _Ref_Self __other)
			:pList(new _Con_type)
		{
			if( this == std::addressof(__other)) return ;

			std::for_each(std::begin(*__other.pList) ,
				std::end(*__other.pList),
				[this](const typename _Self::_Fun_type& x)->void
			{
				this->_push_back(x);
			}
			) ;
		}

		delegate_IMPL(_Self&& __other)
		{
			pList = std::move(__other.pList) ;
			__other.pList = nullptr ;
		}

		~delegate_IMPL()
		{
			this->_clear();
		}

		_Ref_Self operator = (const _Ref_Self __other)
		{
			if( this == std::addressof(__other)) return *this;

			std::for_each(std::begin(*__other.pList) ,
				std::end(*__other.pList),
				[this](const typename _Self::_Fun_type& x)->void
			{
				this->_push_back(x);
			}
			) ;

			return *this ;
		}

		_Ref_Self operator = (_Self&& __other)
		{
			pList = std::move(__other.pList) ;
			__other.pList = nullptr ;
			return *this;
		}

	public:

		/// Method of _Self
		inline
			const _Ref_Self operator = (const _Fun_type& _f)
		{
			this->_clear();
			this->_push_back(_f);
			return *this;
		}

		inline
			const _Ref_Self operator = (_Fun_type&& _f)
		{
			this->_clear();
			this->_push_back(std::move(_f)) ;
			return *this;
		}

		inline
			void operator += (const _Fun_type& _f)
		{
			this->_push_back(_f);
		}

		inline
			void operator += (_Fun_type&& _f)
		{
			this->_push_back(std::move(_f)) ;
		}

		inline
			void operator -= (size_t Pos)
		{
			this->_erase(Pos);
		}

		/// Read-Value Reference
		R operator () ( Args&& ... args )
		{
			return (R)(global_class<R,_Con_type,Args...>::_M_INVOKE(
				*pList,
				std::forward<Args>(args)...)
				);
		}

		/// Locate-Value Reference
		R operator() (const Args&... args)
		{
			return (R)(global_class<R,_Con_type,Args...>::_M_INVOKE(
				*pList ,
				args... )
				);
		}

	private:
		/// Impl of _Self
		std::unique_ptr<_Con_type> pList ;

	public:
		///   _Con_type's Impl Of Push_Back
		inline
			void _push_back(const _Fun_type& x)
		{
			pList->push_back(x);
		}
		inline
			void _push_back(_Fun_type&& x)
		{
			pList->push_back(std::move(x));
		}
		///  _Con_type's Impl Of clear
		inline
			void _clear()
		{
			pList->clear();
		}

		/// _Con_type's Impl Of erase Index
		inline
			void _erase( size_t _pos )
		{
			auto _begin = std::begin( *pList ) ;
			std::advance( _begin , _pos - 1 );
			pList->erase( _begin ) ;
		}
	};

}  /// _DSH

namespace DSH {

    template<typename R  , typename... Args >
	class delegate ;

	template<typename R , typename... Args>
	class delegate< R(Args...) >
	{
	public:
		typedef delegate< R(Args...) >       Self;
		typedef delegate< R(Args...) >&      Ref_Self;
		typedef delegate< R(Args...) >*      Ptr_Self;

		typedef _DSH::delegate_IMPL< R(Args...) >  _IMPL_TYPE;
		typedef typename _IMPL_TYPE::_Fun_type Fun_type;
		typedef typename _IMPL_TYPE::_Con_type Con_type;

		delegate()
			:_M_IMPL()
		{
		}

		delegate( const Ref_Self __other )
			:_M_IMPL(__other._M_IMPL)
		{
		}
		delegate( Self&& __other )
			:_M_IMPL(std::move(__other._M_IMPL))
		{
		}
		~delegate( )
		{
		}

		Ref_Self operator = (const Ref_Self __other)
		{
			if( std::addressof(__other) == this ) return *this ;
			_M_IMPL = __other._M_IMPL;
			return *this;
		}

		Ref_Self operator = (Self&& __other)
		{
			if( std::addressof(__other) == this ) return *this ;
			_M_IMPL = std::move(__other._M_IMPL);
			return *this;
		}

	public:
		/// Method
		const Ref_Self operator = (const Fun_type& _f)
		{
			_M_IMPL = _f ;
			return *this;
		}

		const Ref_Self operator = (Fun_type&& _f)
		{
			_M_IMPL = std::move(_f);
			return *this;
		}

		void operator += (const Fun_type& _f )
		{
			_M_IMPL += _f ;
		}
		void operator += (Fun_type&& _f)
		{
			_M_IMPL += std::move(_f) ;
		}
		void operator -= (size_t pos )
		{
			_M_IMPL -= pos ;
		}


		/// Read-value Reference
		R operator () ( Args&& ... args)
		{
			return (_M_IMPL( std::forward<Args>(args)...));
		}


		/// Locate-value Reference
		R operator () (const Args&... args)
		{
			return (_M_IMPL( args ... ) );
		}

	private:
		_IMPL_TYPE _M_IMPL;
	};

} /// DSH

namespace _DSH {
	template< typename R , typename C , typename... Args >
	class global_class
	{
		friend class delegate_IMPL<R(Args...)> ;
	private:
		static R _M_INVOKE(const C& c , Args&& ... args)
		{
			R RetValue = R() ;
			for(auto first = std::begin(c);
				first != std::end(c);
				++first)
			{
				if( first == std::begin(c) ) {
					RetValue = (*first)( std::forward<Args>(args) ... ) ;
				} else {
					(*first)( std::forward<Args>(args) ... ) ;
				}
			}
			return RetValue;
		}

		static R _M_INVOKE(const C& c,const Args& ... args)
		{
			R RetValue = R() ;
			for(auto first = std::begin(c);
				first != std::end(c);
				++first)
			{
				if( first == std::begin(c) ) {
					RetValue = (*first)( args ... ) ;
				} else {
					(*first)( args ... ) ;
				}
			}
			return RetValue;
		}
	};

	template< typename C , typename... Args >
	class global_class< void , C , Args... >
	{
	public:
		friend class delegate_IMPL<void(Args...)> ;
	private:
		static void _M_INVOKE(const C& c , Args&& ... args)
		{
			for(auto first = std::begin(c);
				first != std::end(c);
				++first)
			{
				(*first)( std::forward<Args>(args) ... ) ;
			}
		}

		static void _M_INVOKE(const C& c ,const Args& ... args)
		{
			for(auto first = std::begin(c);
				first != std::end(c);
				++first)
			{
				(*first)( std::forward<Args>(args) ... ) ;
			}
		}
	};

}   /// _DSH

#endif // DELEGATE_H_INCLUDED
