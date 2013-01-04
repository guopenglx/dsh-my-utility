#ifndef _DSH_MAP_H
#define _DSH_MAP_H

/* Flag Field */
//#define _GCC_COMPILER
/* Flag Field */

#include <initializer_list>
#include "bits\size_balance_tree.h"

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 8 / 17                                              *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                     *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # map                                                               *
*                                                                       *
*                                                                       *
*************************************************************************/

namespace DSH
{
	template<typename _Kt,
		     typename _Vt,
			 typename _Compare = std::less<_Kt>>
	class map
	{
	public:
		typedef _Kt        key_type ;
		typedef _Vt        mapped_type ;
		typedef _Compare   key_comp ;

		typedef std::pair<key_type,mapped_type> value_type ;

		class value_compare
		{
			key_comp   _comp_ ;
		public:
			inline bool operator() (const value_type& lhs, const value_type& rhs) const
			{ return _comp_(lhs.first,rhs.first) ; }
		} ;
	private:
		typedef _DSH::sbt<value_type , value_compare , size_t> _tree_type ;
	public:
		typedef typename _tree_type::size_type size_type ;
		typedef typename _tree_type::iterator  iterator  ;
		typedef typename _tree_type::reverse_iterator reverse_iterator ;
		const static size_type npos ;
	public:
		map()
			:_M_()
		{
		}

		map(std::initializer_list<value_type> _list)
			:_M_()
		{
			for(auto _pCur = _list.begin() ;
				     _pCur != _list.end() ;
					 ++_pCur )
			{
				_M_.insert(*_pCur) ;
			}
		}
		map(const map& other)
			:_M_(other._M_)
		{
		}

		map(map&& other)
			:_M_(std::move(other._M_))
		{
		}

		~map()
		{
			_M_.~sbt() ;
		}
		map& operator = (const map& other)
		{
			if( this == std::addressof(other) ) return *this;

			_M_ = other._M_ ;
			return *this ;
		}

		map& operator = (map&& other)
		{
			if( this == std::addressof(other) ) return *this;

			_M_ = std::move(other._M_) ;
			return *this ;
		}


	public:

		/*  Iterator */
		iterator begin()
		{
			return _M_.begin();
		}

		iterator end()
		{
			return _M_.end() ;
		}

		reverse_iterator rbegin()
		{
			return _M_.rbegin() ;
		}

		reverse_iterator rend()
		{
			return _M_.rend() ;
		}

		/*  Capacity */
		size_type size() const
		{
			return _M_.size() ;
		}

		bool empty() const
		{
			return _M_.empty() ;
		}

		/*  Modifiers */
		void clear()
		{
			_M_.clear() ;
		}

		std::pair<iterator,bool>
		insert( const value_type& value )
		{
			return _M_.insert(value);
		}

		std::pair<iterator,bool>
		insert( value_type&& value )
		{
			return _M_.insert(std::move(value));
		}

		void erase( const key_type& key )
		{
			_M_.erase(value_type(key,mapped_type()));
		}

		void erase( key_type&& key )
		{
			_M_.erase(value_type(std::move(key),mapped_type()));
		}

		/*  Lookup  */
		iterator find( const key_type& key)
		{
			return _M_.find(value_type(key,mapped_type()));
		}

		iterator find( key_type&& key)
		{
			return _M_.find(value_type(std::move(key),mapped_type()));
		}

		size_type rank( const key_type& key )
		{
			return _M_.rank(value_type(key,mapped_type()));
		}

		size_type rank( key_type&& key )
		{
			return _M_.rank(value_type(std::move(key),mapped_type()));
		}

		iterator select( size_type index )
		{
			return _M_.rank(index);
		}

		/*  Element access  */
		mapped_type& operator [] ( const key_type& key )
		{
			return (*(_M_.insert(value_type(key,mapped_type())).first)).second;
		}

		/* Debug */

	private:
		/*  Member Var  */
		_tree_type _M_ ;
	};

		/// Static Non_Postion
	template<typename _Kt,
		     typename _Vt,
			 typename _Compare>
	const typename map<_Kt , _Vt , _Compare >::size_type
		           map<_Kt , _Vt , _Compare >::npos =
				   (typename map<_Kt , _Vt , _Compare >::size_type)(-1);
}

#endif
