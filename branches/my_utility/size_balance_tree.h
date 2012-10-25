#ifndef _DSH_SIZE_BALANCE_TREE_H
#define _DSH_SIZE_BALANCE_TREE_H

/* Debug Flag Field */
//#define _DSH_DEBUG
//#define _DSH_OLD
/* Debug Flag Field */

#include <utility>
#include <functional>
#include <iterator>
#include <cassert>

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 3 / 20                                              *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                    *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # size_balance_tree                                                 *
*                                                                       *
*   # Version 1 :  修改了树结构,根节点为 Nil 节点,顺利实现了            *
*                  逆向迭代器                                           *
*                                                                       *
*   # Version 2 :  将插入和删除操作都改为非递归, 现在尚未发现           *
*                  存在这样的算法的代码.                                *
*                                                                       *
*************************************************************************/

namespace _DSH {

	/* Prefix Declare */
	template< typename _Value_type,typename _Compare,typename _St>  class sbt ;

	template< typename Pointer >  class _Mytree ;

	template< typename _Value_type, typename _St> class sbt_node ;

	/* base node type */
	template< typename _Value_type,
			  typename _St>
	class sbt_node
	{
	public:
		typedef _Value_type value_type ;
		typedef _St         size_type ;
	private:
		typedef sbt_node<value_type , size_type> self ;
	public:
		sbt_node()
			:data(),size(0),
			lch(nullptr),
			rch(nullptr),
			parent(nullptr)
		{}

		sbt_node(const self& other)
			:data(other.data),size(other.size),
			lch(nullptr),
			rch(nullptr),
			parent(nullptr)
		{ }

		sbt_node( self&& other )
			:data(std::move(other.data)),size(std::move(other.size)),
			lch(nullptr),
			rch(nullptr),
			parent(nullptr)
		{ }

		explicit sbt_node(const value_type& _data,
						  const size_type& _size)
			:data(_data),size(_size),
			lch(nullptr),
			rch(nullptr),
			parent(nullptr)
		{ }

		explicit sbt_node( value_type&& _data , size_type&& _size )
			: data(std::move(_data)) , size(std::move(_size)),
			lch(nullptr),
			rch(nullptr),
			parent(nullptr)
		{ }

		~sbt_node()
		{
		}
		self& operator = (const self& other)
		{
			if( std::addressof(other) == this ) return *this ;
			data = other.data ;
			size = other.size ;
			return *this      ;
		}

		self& operator = (self&& other )
		{
			if( std::addressof(other) == this ) return *this ;
			data = std::move(other.data) ;
			size = std::move(other.size) ;
			return *this      ;
		}

	public:
		/* member */
		value_type  data ;
		size_type   size ;

		/* Link */
		self *lch , *rch , *parent ;

	};   // end of sbt_node

	/*  global method of tree */
	template< typename Pointer >
	class _Mytree
	{
	public:
		typedef Pointer  ptr_type;

		static
			bool isNil(const ptr_type& p ,const ptr_type& nil_ptr )
		{
			return p == nullptr || p == nil_ptr ;
		}

		static
			ptr_type& Left( ptr_type& p )
			// p != NULL
		{
			return p->lch ;
		}

		static
			const ptr_type& Left(const ptr_type& p )
			// p != NULL
		{
			return p->lch ;
		}

		static
			ptr_type& Right( ptr_type& p )
			// p != NULL
		{
			return p->rch ;
		}

		static
			const ptr_type& Right(const ptr_type& p )
			// p != NULL
		{
			return p->rch ;
		}

		static
			ptr_type& Parent( ptr_type& p )
			// p != NULL
		{
			return p->parent ;
		}

		static
			const ptr_type& Parent(const ptr_type& p )
			// p != NULL
		{
			return p->parent ;
		}

		static
			ptr_type Left_Most( ptr_type root )
			// root != NULL
		{
			while( ! _Mytree<ptr_type>::isNil(_Mytree<ptr_type>::Left(root),nullptr ))
				root = _Mytree<ptr_type>::Left(root);
			return root ;
		}

		static
			ptr_type Right_Most( ptr_type root )
			// root != NULL
		{
			while( ! _Mytree<ptr_type>::isNil(_Mytree<ptr_type>::Right(root),nullptr ))
				root = _Mytree<ptr_type>::Right(root);
			return root;
		}
	};

	template< typename MyNode >
	class sbt_iterator
	{
		template< typename _Value_type,typename _Compare,typename _St>
		friend class sbt ;
	private:
		typedef MyNode                     node_type ;
		typedef sbt_iterator<MyNode>       self ;
		typedef _DSH::_Mytree<node_type*>  _tree ;
	public:
		typedef std::bidirectional_iterator_tag  iterator_category ;
		typedef typename MyNode::size_type       difference_type ;
		typedef typename MyNode::value_type      value_type;
		typedef typename MyNode::value_type&     reference ;
		typedef typename MyNode::value_type*     pointer   ;

	public:
		sbt_iterator()
			:pNode(nullptr),
			nilNode(nullptr)
		{ }

		sbt_iterator(const self& other)
			:pNode(other.pNode),nilNode(other.nilNode)
		{ }

		self& operator = (const self& other)
		{
			if( std::addressof(other) == this ) return *this;
			pNode = other.pNode ;
			nilNode = other.nilNode ;
			return *this ;
		}
	private:

		explicit sbt_iterator(node_type *_pNode , node_type *_nilNode)
			:pNode(_pNode),
			nilNode(_nilNode)
		{
		}

	public:

		inline
		reference operator* ()
		{
			return pNode->data ;
		}

		inline
		pointer operator-> ()
		{
			return std::addressof(**this) ;
		}

		self& operator++ ()
		{
			if( _tree::isNil(pNode,nilNode) )
				;  // end(), do nothing
			else if( ! _tree::isNil(_tree::Right(pNode) , nullptr ) )
				pNode = _tree::Left_Most( _tree::Right(pNode) ) ;
			else
			{
				node_type *pParent(pNode);
				while( ! _tree::isNil( pParent = _tree::Parent(pNode) , nilNode)
					&& (pNode == _tree::Right(pParent)) )
				{
					pNode = pParent ;
				}
				pNode = pParent ;
			}
			if( pNode == nullptr )
				pNode = nilNode ;
			return *this;
		}

		self operator++ (int)
		{
			self temp(*this);
			++(*this);
			return temp;
		}

		self& operator-- ()
		{
			if( _tree::isNil(pNode, nilNode) )
				pNode = _tree::Right_Most( pNode ) ;
			else if( ! _tree::isNil(_tree::Left(pNode) , nullptr ) )
				pNode = _tree::Right_Most(_tree::Left(pNode));
			else
			{
				node_type *pParent(pNode);
				while( ! _tree::isNil( pParent = _tree::Parent(pNode), nilNode )
					&& (pNode == _tree::Left(pParent)) )
				{
					pNode = pParent ;
				}
				pNode = pParent ;
			}
			if( pNode == nullptr )
				pNode = nilNode ;
			return *this ;
		}

		self operator-- (int)
		{
			self temp(*this);
			--(*this);
			return temp ;
		}

		inline
		bool operator == (const self& rhs) const
		{
			return pNode == rhs.pNode && nilNode == rhs.nilNode ;
		}

		inline
		bool operator != (const self& rhs ) const
		{
			return !(*this == rhs) ;
		}

		/* Debug */
	private:
		node_type *get()
		{
		    return pNode ;
		}

	private:
		node_type *pNode ;
		node_type *nilNode ;
	};


	template< typename _Value_type,
			  typename _Compare = std::less<_Value_type>,
		      typename _St = size_t >
	class sbt
	{

	public:
		typedef _Compare                                     comp_type ;
		typedef _DSH::sbt_node<_Value_type , _St>            node_type ;
		typedef typename node_type::value_type               value_type ;
		typedef typename node_type::size_type                size_type ;
		typedef _DSH::sbt_iterator<node_type>                iterator ;
		typedef std::reverse_iterator<iterator>              reverse_iterator ;
		const static size_type  npos ;    /// Bad_Rank

	private:
		typedef _DSH::_Mytree<node_type*>    _tree ;
		typedef sbt<value_type,comp_type>    self  ;

	public:
		sbt()
			:root_ptr(nullptr),nil_ptr(new node_type()),_comp()
		{
		}

		sbt(self&& other)
			:root_ptr(other.root_ptr),nil_ptr(other.nil_ptr),_comp(std::move(other._comp))
		{
			other.root_ptr = nullptr;
			other.nil_ptr  = nullptr;
		}

		sbt(const self& other)
			:root_ptr(nullptr),nil_ptr(new node_type()),_comp(other._comp)
		{
			this->_assign_copy_constructor(other.root_ptr);
			this->nil_ptr->lch = this->root_ptr ;
			this->nil_ptr->rch = this->root_ptr ;
		}

		~sbt()
		{
			this->_Delete_Tree() ;
			if( nil_ptr ) {
				delete nil_ptr ;
				nil_ptr = nullptr ;
			}
		}

		self& operator = (const self& other)
		{
			if( std::addressof(other) == this ) return *this;
			this->_comp = other._comp ;
			this->_assign_copy(other.root_ptr);
			this->nil_ptr->lch = this->root_ptr ;
			this->nil_ptr->rch = this->root_ptr ;
		}

		self& operator = (self&& other)
		{
			this->root_ptr = other.root_ptr ;
			this->nil_ptr  = other.nil_ptr ;
			this->_comp    = std::move(other._comp);
			other.root_ptr = nullptr;
			other.nil_ptr  = nullptr;
		}

	public:
		std::pair<iterator,bool>
		insert( const value_type& _other )
		{
			auto ret = this->_insert(this->root_ptr , _other );
			this->nil_ptr->lch = ret.first.get();
			this->nil_ptr->rch = nil_ptr->lch ;
			return ret ;
		}

		void erase( const value_type& key )
		{
			_delete(this->root_ptr , key );
			this->nil_ptr->lch = this->root_ptr ;
			this->nil_ptr->rch = nil_ptr->lch ;
		}

		void clear()
		{
			this->_Delete_Tree();
			this->nil_ptr->lch = nullptr ;
			this->nil_ptr->rch = nil_ptr->lch ;
		}

		size_type size() const
		{
			return this->_get_size(this->root_ptr);
		}

		bool empty() const
		{
			return size() == 0 ;
		}

		iterator find(const value_type& key)
		{
			return iterator(this->_find_key(key) , this->nil_ptr );
		}

		iterator find(value_type&& key)
		{
			return iterator(this->_find_key(std::move(key)), this->nil_ptr);
		}

		iterator select(size_type index)
		{
			return iterator(this->_select(this->root_ptr , index) , this->nil_ptr );
		}

		size_type rank(const value_type& key)
		{
		    return this->_rank(this->root_ptr ,key);
		}

		size_type rank(value_type&& key)
		{
			return this->_rank(this->root_ptr, std::move(key));
		}

		iterator begin()
		{
			return iterator(_tree::Left_Most(this->root_ptr) , this->nil_ptr );
		}

		iterator end()
		{
			return iterator(this->nil_ptr , this->nil_ptr );
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
		/* Debug */

#ifdef _DSH_DEBUG
		size_type get_size(iterator pRoot) const
		{
			return this->_get_size(pRoot.get());
		}
#endif
	private:
		/*  Debug  */

		size_type _get_size(node_type *pRoot) const
		{
			if( _tree::isNil( pRoot , this->nil_ptr ) ) return 0;
			else return pRoot->size ;
		}

		node_type *left_rot(node_type *pCur)
		{
			/// pCur->rch != NULL

			/// pTemp = pCur->rch
			node_type *pTemp(_tree::Right(pCur));
			/// pCur->rch = pTemp->lch
			_tree::Right(pCur) = _tree::Left(pTemp) ;

			/// pCur->rch != NULL
			if( ! _tree::isNil(_tree::Right(pCur) , nullptr ) )
				/// pCur->rch->parent = pCur ;
				_tree::Parent(_tree::Right(pCur)) = pCur ;

			/// pTemp->lch = pCur ;
			_tree::Left(pTemp) = pCur ;

			/// pTemp->parent = pCur->parent ;
			_tree::Parent(pTemp) = _tree::Parent(pCur) ;

			/// pCur->parent = pTemp ;
			_tree::Parent(pCur) = pTemp ;

			pTemp->size = pCur->size ;
			pCur->size  = this->_get_size(_tree::Left(pCur))
				        + this->_get_size(_tree::Right(pCur))
						+ 1 ;

			return pTemp ;
		}

		node_type *right_rot(node_type *pCur)
		{
			/// pCur->lch != NULL

			/// pTemp = pCur->lch ;
			node_type *pTemp(_tree::Left(pCur));
			/// pCur->lch = pTemp->right
			_tree::Left(pCur) = _tree::Right(pTemp) ;

			/// pCur->lch != NULL
			if( ! _tree::isNil(_tree::Left(pCur) , nullptr) )
				/// pCur->lch->parent = pCur ;
				_tree::Parent(_tree::Left(pCur)) = pCur ;

			/// pTemp->rch = pCur ;
			_tree::Right(pTemp) = pCur ;

			/// pTemp->parent = pCur->parent ;
			_tree::Parent(pTemp) = _tree::Parent(pCur) ;

			/// pCur->parent = pTemp ;
			_tree::Parent(pCur) = pTemp ;

			pTemp->size = pCur->size ;
			pCur->size  = this->_get_size(_tree::Left(pCur))
				        + this->_get_size(_tree::Right(pCur))
						+ 1 ;

			return pTemp ;
		}

		node_type *_maintain( node_type *mRoot , bool flag )
		{
			if( _tree::isNil( mRoot , nullptr ) )
				return mRoot ;
			if( !flag )
				// Left Child
			{
				if( !_tree::isNil(_tree::Left(mRoot), nullptr)
				 && (this->_get_size(_tree::Left(_tree::Left(mRoot)))
					  > this->_get_size(_tree::Right(mRoot)))
				  )
				{
					mRoot = this->right_rot(mRoot);
				}
				else if(! _tree::isNil(_tree::Left(mRoot), nullptr)
					&& (this->_get_size(_tree::Right(_tree::Left(mRoot)))
						 > this->_get_size(_tree::Right(mRoot)))
					   )
				{
					_tree::Left(mRoot) = this->left_rot(_tree::Left(mRoot));
					mRoot              = this->right_rot(mRoot);
				}
				else
				{
					return mRoot ;
				}
			}
			else
				// Right Child
			{
				if( !_tree::isNil(_tree::Right(mRoot), nullptr)
				 && (this->_get_size(_tree::Right(_tree::Right(mRoot)))
					  > this->_get_size(_tree::Left(mRoot)))
				  )
				{
					mRoot = this->left_rot(mRoot);
				}
				else if(! _tree::isNil(_tree::Right(mRoot), nullptr)
					&& (this->_get_size(_tree::Left(_tree::Right(mRoot)))
						 > this->_get_size(_tree::Left(mRoot)))
						 )
				{
					_tree::Right(mRoot) = this->right_rot(_tree::Right(mRoot));
					mRoot               = this->left_rot(mRoot);
				}
				else
				{
					return mRoot ;
				}
			}
			_tree::Left(mRoot) = this->_maintain(_tree::Left(mRoot) , false);
			_tree::Right(mRoot) = this->_maintain(_tree::Right(mRoot) , true );

			mRoot = this->_maintain(mRoot , false );
			mRoot = this->_maintain(mRoot , true  );

			return mRoot ;
		}

#ifdef _DSH_OLD
		std::pair<iterator,bool>
		_insert( node_type *&mRoot ,const value_type& _data )
		{
			std::pair<iterator,bool> RetValue ;

			if( _tree::isNil(mRoot,nullptr) )
			{
				mRoot = new node_type  ;
				mRoot->data = _data    ;
				mRoot->size = 1        ;

				RetValue.first  = iterator(mRoot,this->nil_ptr);
				RetValue.second = true ;
			}
			else if( _comp(_data , mRoot->data) )
			{
				// _data < mRoot->data
				RetValue = _insert(_tree::Left(mRoot),_data);
				if(! _tree::isNil( _tree::Left(mRoot),nullptr ) )
					_tree::Parent(_tree::Left(mRoot)) = mRoot ;
				if( RetValue.second ) {
					mRoot = this->_maintain(mRoot, false );
					++mRoot->size ;
				}
			}
			else if( _comp(mRoot->data,_data) )
			{
				// _data > mRoot->data
				RetValue = _insert(_tree::Right(mRoot),_data);
				if(! _tree::isNil( _tree::Right(mRoot),nullptr ) )
					_tree::Parent(_tree::Right(mRoot)) = mRoot ;
				if( RetValue.second ) {
					mRoot = this->_maintain(mRoot, true );
					++mRoot->size ;
				}
			}
			else
			{
				// _data == mRoot->data
				RetValue.first  = iterator(mRoot,this->nil_ptr);
				RetValue.second = false ;
			}
			return RetValue ;
		}
#else
		std::pair<iterator,bool>
		_insert( node_type *&mRoot , const value_type& _data )
		{
			node_type *pCur(mRoot) ;
			std::pair<iterator,bool> RetValue ;

			// Empty Tree
			if( _tree::isNil(mRoot, nullptr) )
			{
				mRoot = new node_type ;
				mRoot->data = _data   ;
				mRoot->size = 1       ;
				RetValue.first = iterator(mRoot,this->nil_ptr);
				RetValue.second = true ;

				return RetValue ;
			}

			bool New_Node(false) ;
			node_type *pNext(nullptr) ;
			// Find the key
			while( ! New_Node )
			{
				if( _comp( _data , pCur->data ) )
					// _data < pCur->data
				{
					if(! _tree::isNil(_tree::Left(pCur), nullptr) )
					{
						pCur = _tree::Left(pCur) ;
					}
					else
					{
						// Constructor New Node
						pNext = new node_type ;
						pNext->data      = _data ;
						pNext->size      = 1 ;

						// Link pNext to pCur
						_tree::Left(pCur) = pNext ;
						_tree::Parent(pNext) = pCur ;

						RetValue.first   = iterator(pNext , this->nil_ptr );
						RetValue.second  = true ;

						New_Node = true ;
					}
				}
				else if( _comp( pCur->data , _data ) )
					// pCur->data < _data
				{
					if(! _tree::isNil(_tree::Right(pCur) , nullptr) )
					{
						pCur = _tree::Right(pCur) ;
					}
					else
					{
						// Constructor New Node
						pNext = new node_type ;
						pNext->data      = _data ;
						pNext->size      = 1 ;

						// Link pNext to pCur
						_tree::Right(pCur) = pNext ;
						_tree::Parent(pNext) = pCur ;

						RetValue.first   = iterator(pNext , this->nil_ptr );
						RetValue.second  = true ;

						New_Node = true ;
					}
				}
				else
					// _data == pCur->data
				{
					RetValue.first  = iterator(pCur,this->nil_ptr ) ;
					RetValue.second = false ;

					return RetValue ;
				}
			}

			// Trace Back
			while(! _tree::isNil( _tree::Parent(pCur) , nullptr ))
			{
				++pCur->size ;

				node_type *pParent = _tree::Parent(pCur) ;

				if( pNext == _tree::Left(pCur) )
				{
					if( pCur == _tree::Left(pParent) )
					{
						_tree::Left(pParent) = this->_maintain(pCur,false);
						pNext = _tree::Left(pParent);
					}
					else if( pCur == _tree::Right(pParent) )
					{
						_tree::Right(pParent) = this->_maintain(pCur,false);
						pNext = _tree::Right(pParent);
					}
				}
				else if( pNext == _tree::Right(pCur) )
				{
					if( pCur == _tree::Left(pParent) )
					{
						_tree::Left(pParent) = this->_maintain(pCur,true);
						pNext = _tree::Left(pParent);
					}
					else if( pCur == _tree::Right(pParent) )
					{
						_tree::Right(pParent) = this->_maintain(pCur,true);
						pNext = _tree::Right(pParent);
					}
				}
				pCur  = pParent ;
			}

			++mRoot->size ;

			mRoot = this->_maintain(pCur,false);
			mRoot = this->_maintain(pCur,true );

			return RetValue ;
		}
#endif

#ifdef _DSH_OLD

		bool _delete( node_type *&mRoot , const value_type& _data )
		{
			if( ! _tree::isNil(mRoot,nullptr) )
			{
				if( _comp( _data , mRoot->data ) )
					// _data < mRoot->data
				{
					if( _delete( _tree::Left(mRoot) , _data ) )
					{
						--mRoot->size ;
						mRoot = this->_maintain(mRoot,true) ;

						return true ;
					}
				}
				else if( _comp( mRoot->data , _data ) )
				{
					if( _delete( _tree::Right(mRoot) , _data ) )
					{
						--mRoot->size ;
						mRoot = this->_maintain(mRoot ,false) ;

						return true ;
					}
				}
				else
					// _data == mRoot->data
				{
					if(  _tree::isNil(_tree::Left(mRoot),nullptr)
					  && _tree::isNil(_tree::Right(mRoot),nullptr) )
					  // Leaf
					{
						delete mRoot ;
						mRoot = nullptr ;

						return true;
					}
					else if( _tree::isNil(_tree::Left(mRoot),nullptr) )
						// Not left child
					{
						node_type *pRight = _tree::Right(mRoot);
						_tree::Parent(pRight) = _tree::Parent(mRoot) ;
						delete mRoot ;
						mRoot = pRight ;

						return true ;
					}
					else if( _tree::isNil(_tree::Right(mRoot),nullptr) )
						// Not right child
					{
						node_type *pLeft = _tree::Left(mRoot);
						_tree::Parent(pLeft) = _tree::Parent(mRoot);
						delete mRoot ;
						mRoot = pLeft ;

						return true ;
					}
					else
						// Both left and right child
					{
						node_type *pRightNext = _tree::Right(mRoot);
						pRightNext = _tree::Left_Most(pRightNext) ;

						// Swap the data
						std::swap( mRoot->data , pRightNext->data );

                        this->_delete( _tree::Right(mRoot) , _data ) ;

						--mRoot->size ;
						mRoot = this->_maintain( mRoot , false );

						return true ;
					}
				}
			}
			return false;
		}
#else
		void _delete( node_type *&mRoot , const value_type& _data )
		{
			if( _tree::isNil(mRoot, nullptr) ) return ;

			node_type *pCur(mRoot);
			node_type *pTraceParent(_tree::Parent(pCur)) ;
			bool is_delete(false) ;
			while( ! is_delete )
			{
				if( _comp( _data , pCur->data ) )
					// _data < pCur->data
				{
					if(_tree::isNil(_tree::Left(pCur), nullptr)) return ;
					else pCur = _tree::Left(pCur) ;
				}
				else if( _comp(pCur->data , _data ) )
					// _data > pCur->data
				{
					if(_tree::isNil(_tree::Right(pCur), nullptr)) return ;
					else pCur = _tree::Right(pCur) ;
				}
				else
					// _data == pCur->data
				{
					if( _tree::isNil(_tree::Left(pCur), nullptr)
					 && _tree::isNil(_tree::Right(pCur), nullptr) )
					 // Leaf
					{
						node_type *pParent(_tree::Parent(pCur)) ;
						if(!_tree::isNil(pParent, nullptr)) {
							if( pCur == _tree::Left(pParent) )
								_tree::Left(pParent) = nullptr ;
							else if( pCur == _tree::Right(pParent) )
								_tree::Right(pParent) = nullptr ;
						}
						pTraceParent = pParent ;
						delete pCur ;
						pCur = nullptr ;
						is_delete = true ;
					}
					else if( _tree::isNil(_tree::Left(pCur), nullptr) )
						// Only Right child
					{
						node_type *pParent(_tree::Parent(pCur));
						if(!_tree::isNil(pParent, nullptr)) {
							if( pCur == _tree::Left(pParent) )
								_tree::Left(pParent) = _tree::Right(pCur) ;
							else if( pCur == _tree::Right(pParent) )
								_tree::Right(pParent) = _tree::Right(pCur) ;
						}
						_tree::Parent(_tree::Right(pCur)) = pParent ;
						pTraceParent = pParent ;
						delete pCur ;
						pCur = nullptr ;
						is_delete = true ;
					}
					else if( _tree::isNil(_tree::Right(pCur), nullptr) )
						// Only Left child
					{
						node_type *pParent(_tree::Parent(pCur));
						if(!_tree::isNil(pParent, nullptr)) {
							if( pCur == _tree::Left(pParent) )
								_tree::Left(pParent) = _tree::Left(pCur) ;
							else if( pCur == _tree::Right(pParent) )
								_tree::Right(pParent) = _tree::Left(pCur) ;
						}
						_tree::Parent(_tree::Left(pCur)) = pParent ;
						pTraceParent = pParent ;
						delete pCur ;
						pCur = nullptr ;
						is_delete = true ;
					}
					else
						// Both Left and Right child
					{
						node_type *pNextRight(_tree::Left_Most(_tree::Right(pCur)));
						std::swap( pCur->data , pNextRight->data );
						node_type *pParent(_tree::Parent(pNextRight));

						if( pParent == pCur )
						{
							_tree::Right(pParent) = _tree::Right(pNextRight) ;
							if( ! _tree::isNil(_tree::Right(pNextRight), nullptr) )
								_tree::Parent(_tree::Right(pNextRight)) = pParent ;

							delete pNextRight ;
							pNextRight = nullptr ;
							--pParent->size;
						}
						else
						{
							_tree::Left(pParent) = _tree::Right(pNextRight) ;
							if( ! _tree::isNil(_tree::Right(pNextRight), nullptr) )
								_tree::Parent(_tree::Right(pNextRight)) = pParent ;

							delete pNextRight ;
							pNextRight = nullptr ;

							node_type *pTempParent(nullptr);
							while( pParent != pCur )
							{
								--pParent->size ;
								pTempParent = _tree::Parent(pParent) ;

								if( ! _tree::isNil(pTempParent, nullptr)
									&& pParent == _tree::Left(pTempParent) )
								{
									_tree::Left(pTempParent) = this->_maintain(_tree::Left(pTempParent),
										                                       true);
								}
								else if( ! _tree::isNil(pTempParent, nullptr)
									&& pParent == _tree::Right(pTempParent) )
								{
									_tree::Right(pTempParent) = this->_maintain(_tree::Right(pTempParent),
											                                    true);
								}
								pParent = pTempParent ;
							}
							--pParent->size ;
						}

						// pParent == pCur
						pTraceParent = _tree::Parent(pCur) ;
						is_delete = true ;
					}
				}
			}

			/// Trace Back
			bool remove_in_right(true);
			while(! _tree::isNil(pTraceParent, nullptr) )
			{
				--pTraceParent->size ;

				if( pCur == _tree::Left(pTraceParent) )
				{
					_tree::Left(pTraceParent) = this->_maintain( _tree::Left(pTraceParent),
						                                         !remove_in_right );
					remove_in_right = false ;
				}
				else if( pCur == _tree::Right(pTraceParent) )
				{
					_tree::Right(pTraceParent) = this->_maintain( _tree::Right(pTraceParent),
						                                         !remove_in_right );
					remove_in_right = true ;
				}

				pCur         = pTraceParent ;
				pTraceParent = _tree::Parent(pTraceParent) ;
			}

			mRoot = this->_maintain(pCur , !remove_in_right );
		}
#endif

		node_type *_find_key(const value_type& _data)
		{
			node_type *pCur(this->root_ptr);

			while( ! _tree::isNil(pCur, nullptr) )
			{
				if( _comp( _data , pCur->data ) )
					pCur = _tree::Left(pCur);
				else if( _comp( pCur->data , _data ) )
					pCur = _tree::Right(pCur);
				else
					return pCur;
			}
			return this->nil_ptr ;
		}

		node_type *_find_key(value_type&& _data)
		{
			node_type *pCur(this->root_ptr);

			while( ! _tree::isNil(pCur, nullptr) )
			{
				if( _comp( _data , pCur->data ) )
					pCur = _tree::Left(pCur);
				else if( _comp( pCur->data , _data ) )
					pCur = _tree::Right(pCur);
				else
					return pCur;
			}
			return this->nil_ptr ;
		}

		/// index = [0 , size() - 1]
		node_type *_select(node_type *mRoot , size_type index)
		{
			size_type Cur(index + 1) ;

			assert( (index < mRoot->size ) );

			while( Cur != this->_get_size(_tree::Left(mRoot)) + 1 )
			{
				if( Cur > this->_get_size(_tree::Left(mRoot)) )
				{
					Cur -= (this->_get_size(_tree::Left(mRoot)) + 1) ;
					mRoot = _tree::Right(mRoot) ;
				}
				else
				{
					mRoot = _tree::Left(mRoot) ;
				}
			}

			return mRoot ;
		}

		size_type _rank(node_type *mRoot , const value_type& _data)
		{
		    size_type result(0) ;

		    while( ! _tree::isNil(mRoot, nullptr)
                && (_comp( _data , mRoot->data )
                    || _comp( mRoot->data , _data )) )
            {
                if( _comp( _data , mRoot->data ) )
                {
                    mRoot = _tree::Left(mRoot);
                }
                else
                {
                    result += this->_get_size(_tree::Left(mRoot)) + 1;
                    mRoot = _tree::Right(mRoot);
                }
            }
            if( _tree::isNil(mRoot) ) return this->npos;
            else return result + this->_get_size(_tree::Left(mRoot));
		}

		size_type _rank(node_type *mRoot , value_type&& _data )
		{
		    size_type result(0) ;

		    while( ! _tree::isNil(mRoot)
                && (_comp( _data , mRoot->data )
                    || _comp( mRoot->data , _data )) )
            {
                if( _comp( _data , mRoot->data ) )
                {
                    mRoot = _tree::Left(mRoot);
                }
                else
                {
                    result += this->_get_size(_tree::Left(mRoot)) + 1;
                    mRoot = _tree::Right(mRoot);
                }
            }
            if( _tree::isNil(mRoot) ) return this->npos;
            else return result + this->_get_size(_tree::Left(mRoot));
		}

		void _Delete_Tree()
		{
			this->root_ptr = _delete_tree(this->root_ptr);
		}

		node_type *_delete_tree(node_type *root)
		{
			if(root)
			{
				_tree::Right(root) = _delete_tree(_tree::Right(root));
				_tree::Left(root)  = _delete_tree(_tree::Left(root)) ;
				delete root ;
				root = nullptr ;
			}
			return root ;
		}

		void _assign_copy_constructor(node_type* _other )
		{
			this->root_ptr = this->_assign_recursion( this->root_ptr , _other );
		}

		void _assign_copy(node_type* _other )
		{
			this->_Delete_Tree();

			this->root_ptr = this->_assign_recursion( this->root_ptr , _other );
		}

		node_type *_assign_recursion( node_type *_where ,node_type* _other )
		{
			if( ! _tree::isNil( _other , nullptr ) )
			{
				_where = new node_type(*_other);

				_tree::Right(_where) = this->_assign_recursion(_tree::Right(_where),
					                                           _tree::Right(_other));
				if(! _tree::isNil(_tree::Right(_where),nullptr) )
					_tree::Parent(_tree::Right(_where)) = _where;

				_tree::Left(_where)  = this->_assign_recursion(_tree::Left(_where),
					                                           _tree::Left(_other));
				if(! _tree::isNil(_tree::Left(_where),nullptr) )
					_tree::Parent(_tree::Left(_where)) = _where;
			}
			return _where ;
		}

	private:
		node_type *root_ptr;
		node_type *nil_ptr ;
		comp_type _comp;
	};


	/// Static Non_Postion
	template< typename _Value_type,
			  typename _Compare,
	          typename _St>
	const typename sbt<_Value_type , _Compare , _St>::size_type
		           sbt<_Value_type , _Compare , _St>::npos =
				   (typename sbt<_Value_type , _Compare , _St>::size_type)(-1);


}  /// end of _DSH

#endif
