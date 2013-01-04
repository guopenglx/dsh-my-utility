#ifndef _DSH_SET_H
#define _DSH_SET_H

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
*   # set                                                               *
*                                                                       *
*                                                                       *
*************************************************************************/


namespace DSH
{
	template<typename _Kt,
			 typename _Compare = std::less<_Kt>>
	class set
	{
	public:
		typedef _Kt        key_type ;
		typedef _Kt        value_type ;
		typedef _Compare   key_comp ;

	private:
		typedef _DSH::sbt<value_type , key_comp , size_t> _tree_type ;
	public:
		typedef typename _tree_type::size_type size_type ;
		typedef typename _tree_type::iterator  iterator  ;
		typedef typename _tree_type::reverse_iterator reverse_iterator ;

		const static size_type npos ;
	public:
		set()
			:_M_()
		{
		}

		set(std::initializer_list<value_type> _list)
			:_M_()
		{
			for(auto _pCur = _list.begin() ;
				     _pCur != _list.end() ;
					 ++_pCur )
			{
				_M_.insert(*_pCur) ;
			}
		}

		set(const set& other)
			:_M_(other._M_)
		{
		}

		set(set&& other)
			:_M_(std::move(other._M_))
		{
		}

		~set()
		{
			_M_.~sbt() ;
		}
		set& operator = (const set& other)
		{
			if( this == std::addressof(other) ) return *this;

			_M_ = other._M_ ;
			return *this ;
		}

		set& operator = (set&& other)
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
			_M_.erase(key);
		}

		void erase( key_type&& key )
		{
			_M_.erase(std::move(key));
		}

		/*  Lookup  */
		iterator find( const key_type& key)
		{
			return _M_.find(key);
		}

		iterator find( key_type&& key)
		{
			return _M_.find(std::move(key));
		}

		size_type rank( const key_type& key )
		{
			return _M_.rank(key);
		}

		size_type rank( key_type&& key )
		{
			return _M_.rank(std::move(key));
		}

		iterator select( size_type index )
		{
			return _M_.rank(index);
		}

		/* Debug */


	private:
		/*  Member Var  */
		_tree_type _M_ ;
	};

		/// Static Non_Postion
	template<typename _Kt,
			 typename _Compare>
	const typename set<_Kt, _Compare >::size_type
		           set<_Kt, _Compare >::npos =
				   (typename set<_Kt, _Compare >::size_type)(-1);
}

#endif
