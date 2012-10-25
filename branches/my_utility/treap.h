#ifndef _DSH_TREAP_H
#define _DSH_TREAP_H

#include <functional>
#include <iterator>
#include <cstdlib>
#include <cstddef>
#include <ctime>

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 3 / 16                                              *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                     *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # treap 类模版                                                      *
*                                                                       *
*                                                                       *
*************************************************************************/

namespace _DSH{

	/*  treap_node begin */
	template< class _Kt,
		      class _Vt >
	class treap_node
	{
	public:
		typedef _Kt                     node_key_type   ;
		typedef _Vt                     node_value_type ;
		typedef treap_node< _Kt , _Vt > node_self     ;


		treap_node()
			:parent(nullptr),
			lch(nullptr),
			rch(nullptr),
			key(),
			value(),
			priority(),
			count()
		{
		}

		treap_node( const node_self& node )
			:parent(node.parent),
			lch(node.lch),
			rch(node.rch),
			key(node.key),
			value(node.value),
			priority(node.priority),
			count(node.count)
		{
		}

		treap_node( node_self&& node )
			:parent(node.parent),
			lch(node.lch),
			rch(node.rch),
			key(node.key),
			value(node.value),
			priority(node.priority),
			count(node.count)
		{
		}

		~treap_node()
		{
			if( parent || lch || rch )
				Delete_node();
		}

		node_self& operator = ( const node_self& node )
		{
			parent = node.parent;
			lch    = node.lch   ;
			rch    = node.rch   ;
			key    = node.key   ;
			value  = node.value ;

			return *this ;
		}

		node_self& operator = ( node_self&& node )
		{
			parent = node.parent;
			lch    = node.lch   ;
			rch    = node.rch   ;
			key    = node.key   ;
			value  = node.value ;

			return *this ;
		}

		void Delete_node()
		{
			parent = lch = rch = nullptr ;
		}

		node_self * parent ;
		node_self * lch ;
		node_self * rch ;

		_Kt         key ;
		_Vt         value ;
		size_t      priority ;
		size_t      count    ;
	};

	/*  _tree_class     */
	template< typename Pointer >
	class _tree_class
	{
	protected:
		static bool isNull(Pointer pCur)
		{
			return pCur == nullptr ;
		}

		static Pointer _Left_most(Pointer pCur)
		{
			while( !isNull(pCur->lch) ) pCur = pCur->lch ;
			return pCur ;
		}

		static Pointer _Right_most(Pointer pCur)
		{
			while( !isNull(pCur->rch) ) pCur = pCur->rch ;
			return pCur ;
		}
	};

	/*  Treap Iterator  */
	template< class MyNode >
	class treap_iterator
		:_tree_class< MyNode* >
	{
	public:
		/*
		*   Member Type:
		*       value_type
		*       pointer
		*       reference
		*/
		typedef treap_iterator< MyNode >  self ;
		typedef MyNode                         node_type ;
		typedef typename MyNode::node_key_type value_type ;
		typedef value_type&               reference ;
		typedef value_type*               pointer   ;

		typedef size_t                    difference_type   ;
		typedef std::forward_iterator_tag iterator_category ;

		/*  Constructor  */
		treap_iterator()
			:p_node(0)
		{
		}

		treap_iterator(  MyNode *ptr )
			: p_node( ptr )
		{
		}

		treap_iterator( const self& iter )
			: p_node( iter.p_node )
		{
		}

		~treap_iterator()
		{
		}

		self& operator = ( const self& iter )
		{
			p_node = iter.p_node ;
			return *this ;
		}

		/*  deReference  operator  */
		reference operator * () const
		{
			return p_node->key ;
		}
		/*  Reference    operator  */
		pointer   operator -> () const
		{
			return std::addressof(**this);
		}

		/*  Pre Increment operator */
		self& operator ++()
		{
			Operator_Increment( this->p_node );
			return *this;
		}

		/*  Post Increment operator */
		self  operator ++(int)
		{
			self _temp = *this;
			++(*this) ;
			return _temp;
		}

		bool operator == (const self& other)
		{
			return p_node == other.p_node ;
		}

		bool operator != (const self& other)
		{
			return !(p_node == other.p_node) ;
		}
	private:
		/*
		*   pCurrent = pCurrent->next
		*   Post Order Traverse
		*/
		void Operator_Increment( node_type* & pCurrent )
		{
			if( isNull( pCurrent ) )
				;
			else if( !isNull(pCurrent->rch) )
			{
				pCurrent = _Left_most( pCurrent->rch );
			}
			else
			{
				node_type* pNode ;
				while(
					!isNull( pNode = pCurrent->parent )
					&& ( pCurrent == pNode->rch )
					)
				{
					pCurrent = pNode ;
				}
				pCurrent = pNode ;
			}
		}

		/*  Member ptr  */
		node_type * p_node ;
	};

	/*  treap begin  */
	template< class _Kt,
		      class _Vt,
			  class Compare = std::less<_Kt> >
	class treap
		:_tree_class< treap_node< _Kt , _Vt >* >
	{
	public:
		typedef _Kt      key_type  ;
		typedef _Vt      value_type;
		typedef Compare  comp_type ;
		typedef size_t   size_type ;
		typedef size_t   priority_type ;

		typedef treap_node< _Kt , _Vt >  node_type ;
		typedef treap_node< _Kt , _Vt >* node_ptr  ;

		typedef treap< _Kt , _Vt , Compare > self  ;

		typedef self* pointer   ;
		typedef self& reference ;
		typedef const reference const_reference ;

		/* Iterator Type of Treap */
		typedef treap_iterator< node_type > iterator ;


		/*  Constructor  */
		treap()
			:root_ptr(nullptr),comp(),step(0)
		{
			srand( static_cast<unsigned int>(std::time(0)) );
		}
		treap( const_reference other )
		{
			std::srand( static_cast<unsigned int>(std::time(0)) ) ;
			_assign_copy( other.root_ptr );
		}
		treap( self&& other )
			:root_ptr(other.root_ptr),
			comp(other.comp),
			step(other.step)
		{
			std::srand( static_cast<unsigned int>(std::time(0)) );
		}
		~treap()
		{
			if( root_ptr )
				Delete_Tree();
		}
		reference operator = ( const_reference other )
		{
			Delete_Tree() ;
			comp     = other.comp     ;
			step     = other.step     ;
			_assign_copy( other.root_ptr );
		}
		reference operator = ( self&& other )
		{
			root_ptr = other.root_ptr ;
			comp     = other.comp     ;
			step     = other.step     ;
			return *this ;
		}


		/*  Size of the treap */
		size_type size() const
		{
			return GetCount( root_ptr );
		}

		/*  Is Empty  */
		bool empty() const
		{
			return ( GetCount( root_ptr ) == 0 );
		}

		/*  Clear the Treap  */
		void clear()
		{
			Delete_Tree();
		}

		/*  Insert (key,value) in Treap  */
		std::pair<iterator,bool>
		insert(const key_type& m_key , const value_type& m_value )
		{
//DEBUG
			step = 0 ;
//
			return insert_node( root_ptr , m_key , m_value );
		}

		void erase ( const key_type& m_key )
		{
// DEBUG
			step = 0 ;
//
			delete_node( root_ptr , m_key );
		}

		/* Find the key in  Treap */
		iterator search( const key_type& m_key )
		{
			return iterator( _search( m_key ) );
		}

		/* Find the k-th of Treap */
		iterator rank( const size_type k )
		{
			node_ptr index = find_rank( k );
			return iterator( index );
		}

		/*  Find the Max Member  */
		iterator maximum ()
		{
			return iterator(_Right_most( this->root_ptr ));
		}

		/*   Find the Min Member  */
		iterator minimum ()
		{
			return iterator(_Left_most( this->root_ptr ));
		}

		iterator begin()
		{
			return iterator(_Left_most( this->root_ptr ));
		}

		const iterator cbegin() const
		{
			return iterator(_Left_most( this->root_ptr ) );
		}

		iterator end()
		{
			return iterator(nullptr) ;
		}

		const iterator cend() const
		{
			return iterator(nullptr);
		}

		/* DEBUG  */
		size_type debug() const
		{
			return step ;
		}

		template< typename F >
		void InOrder(const F& f)
		{
			_InOrder( this->root_ptr , f );
		}

		template< typename F >
		void _InOrder(node_ptr mRoot , const F& f)
		{
			if( mRoot )
			{
				_InOrder(mRoot->lch,f);
				f(mRoot) ;
				_InOrder(mRoot->rch,f);
			}
		}

	private:
		/* Right Rot */
		node_ptr right_rot( node_ptr mRoot )
		{
			node_ptr temp = mRoot->lch ;

			mRoot->lch = temp->rch ;
			if( mRoot->lch ) mRoot->lch->parent = mRoot ;

			temp->rch  = mRoot ;
			temp->parent = mRoot->parent ;

			if( temp->parent && temp->parent->lch == mRoot )
				temp->parent->lch = temp ;
			else if( temp->parent && temp->parent->rch == mRoot )
				temp->parent->rch = temp ;

			mRoot->parent = temp ;

			temp->count  = GetCount(mRoot) ;
			mRoot->count = GetCount(mRoot->lch) + GetCount(mRoot->rch) + 1 ;

			return temp;
		}

		/* Left Rot */
		node_ptr left_rot( node_ptr mRoot )
		{
			node_ptr temp = mRoot->rch ;

			mRoot->rch = temp->lch ;
			if( mRoot->rch ) mRoot->rch->parent = mRoot ;

			temp->lch = mRoot ;
			temp->parent = mRoot->parent ;

			if( temp->parent && temp->parent->lch == mRoot )
				temp->parent->lch = temp ;
			else if( temp->parent && temp->parent->rch == mRoot )
				temp->parent->rch = temp ;

			mRoot->parent = temp ;

			temp->count  = GetCount(mRoot);
			mRoot->count = GetCount(mRoot->lch) + GetCount(mRoot->rch) + 1 ;

			return temp ;
		}

		/* Insert treap_node in Treap */
		std::pair< iterator , bool >
		insert_node( node_ptr & mRoot ,
			              const key_type& m_key ,
			              const value_type& m_value )
		{
//DEBUG
			++ step ;
//
			std::pair< iterator , bool > Ret ;

			if( !mRoot )
			{
				mRoot = new treap_node< key_type , value_type > ;
				mRoot->key   = m_key ;
				mRoot->value = m_value ;

				mRoot->lch = nullptr ;
				mRoot->rch = nullptr ;

				mRoot->priority = std::rand() ;
				mRoot->count = 1 ;

				/// RetValue
				Ret.first  = iterator( mRoot );
				Ret.second = true ;
			}
			else if( comp( m_key , mRoot->key ) )
			{
				/* m_key < mRoot->key */
				Ret = insert_node( mRoot->lch , m_key , m_value );
				mRoot->lch->parent = mRoot ;

				if( mRoot->lch->priority < mRoot->priority )
					mRoot = right_rot( mRoot );
			}
			else if( comp( mRoot->key , m_key ) )
			{
				/* m_key > mRoot->key */
				Ret = insert_node( mRoot->rch , m_key , m_value );
				mRoot->rch->parent = mRoot ;

				if( mRoot->rch->priority < mRoot->priority )
					mRoot = left_rot( mRoot );
			}
			else
			{
				/* m_key == mRoot->key */
				Ret.first  = iterator(mRoot) ;
				Ret.second = false  ;
			}

			/* update the Size of mRoot */
			mRoot->count = GetCount( mRoot->lch ) + GetCount( mRoot->rch ) + 1 ;

			return Ret ;
		}

		/* delete the treap_node in Treap */
		void delete_node( node_ptr& mRoot ,
			              const key_type& m_key )
		{
// DEBUG
			++ step ;
//
			/* mRoot != NULL */
			if( mRoot )
			{
				if( comp( mRoot->key , m_key ) )
				{
					/* mRoot->key < m_key */
					delete_node( mRoot->rch , m_key );
				}
				else if( comp( m_key , mRoot->key ) )
				{
					/* mRoot->key > m_key */
					delete_node( mRoot->lch , m_key );
				}
				else
				{
					/* key == m_key */

					if( !mRoot->lch && !mRoot->rch )
					{
						/* mRoot is the leaf node in Teap */
						if( mRoot->parent && mRoot->parent->lch == mRoot)
						{
						    mRoot->parent->lch = nullptr;
						}
						else if( mRoot->parent && mRoot->parent->rch == mRoot )
						{
						    mRoot->parent->rch = nullptr ;
						}

						if( mRoot )
						{
						    mRoot->Delete_node();
						    delete mRoot ;
						    mRoot = nullptr ;
						}
					}
					else if( !mRoot->lch && mRoot->rch )
					{
						/* mRoot has right child not left child */
						node_ptr pTemp = mRoot ;
						mRoot = pTemp->rch ;

						if( pTemp->parent && pTemp->parent->lch == pTemp )
						{
							mRoot->parent = pTemp->parent;
							mRoot->parent->lch = mRoot   ;
						}
						else if( pTemp->parent && pTemp->parent->rch == pTemp )
						{
							mRoot->parent = pTemp->parent;
							mRoot->parent->rch = mRoot   ;
						}
						else
						{
							// pTemp->parent == NULL
							mRoot->parent = nullptr ;
						}
						
						if( pTemp )
						{
							pTemp->Delete_node();
							delete pTemp ;
							pTemp = nullptr ;
						}
					}
					else if( !mRoot->rch && mRoot->lch )
					{
						/* mRoot has left child not right child */
						node_ptr pTemp = mRoot ;
						mRoot = pTemp->lch ;

						if( pTemp->parent && pTemp->parent->lch == pTemp )
						{
							mRoot->parent = pTemp->parent;
							mRoot->parent->lch = mRoot   ;
						}
						else if( pTemp->parent && pTemp->parent->rch == pTemp )
						{
							mRoot->parent = pTemp->parent;
							mRoot->parent->rch = mRoot   ;
						}
						
						if( pTemp )
						{
							pTemp->Delete_node();
							delete pTemp ;
							pTemp = nullptr ;
						}
					}
					else
					{
						/* mRoot has both left and right child */
						if( mRoot->lch->priority < mRoot->rch->priority )
						{
							mRoot = right_rot( mRoot ) ;
						}
						else
						{
							mRoot = left_rot( mRoot );
						}
						delete_node( mRoot , m_key );
					}
				}

				/* if mRoot is deleted */
				if( !mRoot ) return ;

				mRoot->count = GetCount( mRoot->lch ) + GetCount( mRoot->rch ) + 1 ;
			}
		}

		/* DesConstruct All Node in Recursion */
		void _Delete_Tree( node_ptr& mRoot )
		{
			if( mRoot )
			{
				_Delete_Tree( mRoot->lch ) ;
				_Delete_Tree( mRoot->rch ) ;
				mRoot->Delete_node();
				delete mRoot ;
				mRoot = nullptr ;
			}
		}
		void Delete_Tree()
		{
			_Delete_Tree( root_ptr ) ;
		}

		/*  Get the Size of Current */
		size_type GetCount( node_ptr current ) const
		{
			if( !current ) return 0 ;
			else return current->count ;
		}

		/*
		*    Find the k-th member of Treap
		*    k = [1 to n] : n = treap.size()
		*    T(n) = log(n)
		*/
		node_ptr find_rank( const size_type k )
		{
			/*  Out Of Range  */
			if( (k < 1) || (k > GetCount( root_ptr )) )
			{
				return nullptr;
			}

			node_ptr  pTemp = root_ptr ;
			size_type index = k ;
			size_type cur   = GetCount( pTemp->lch ) + 1;

			while( cur != index )
			{
				if( cur > index )
				{
					/*  index-th is in left tree of cur  */
					pTemp = pTemp->lch ;
					cur   = GetCount( pTemp->lch ) + 1 ;
				}
				else
				{
					/*  index-th is in right tree of cur */
					index = GetCount( pTemp ) - cur    ;
					pTemp = pTemp->rch                 ;
					cur   = GetCount( pTemp->lch ) + 1 ;
				}
			}

			return pTemp ;
		}

		/*   Find key in Treap */
		node_ptr _search( const key_type& m_key )
		{
			node_ptr current(root_ptr);
// DEBUG
			step = 0 ;
			while( current )
			{
				++ step ;
				if( comp( m_key , current->key ) )
				{
					current = current->lch ;
				}
				else if( comp( current->key , m_key ) )
				{
					current = current->rch ;
				}
				else
				{
					break ;
				}
			}
			return current ;
		}

		void _assign_copy(const node_ptr _other )
		{
			_assign_recur( this->root_ptr , _other );
		}

		void _assign_recur( node_ptr& lhs ,const node_ptr rhs )
		{
			if( !isNull(rhs) )
			{
				lhs = new node_type ;
				lhs->count = rhs->count ;
				lhs->key   = rhs->key   ;
				lhs->priority = rhs->priority ;
				lhs->value = rhs->value ;

				if( !isNull( rhs->lch ) )
				{
					_assign_recur( lhs->lch , rhs );
					lhs->lch->parent = lhs ;
				}

				if( !isNull( rhs->rch ) )
				{
					_assign_recur( lhs->rch , rhs ) ;
					lhs->rch->parent = lhs ;
				}
			}
		}

    private:
		/* Member Var */
		node_ptr  root_ptr ;
		comp_type comp     ;


		/*  DEBUG  */
		size_type step ;
	};
}

#endif
