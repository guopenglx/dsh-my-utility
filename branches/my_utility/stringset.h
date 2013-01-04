#ifndef DSH_STRINGSET_H
#define DSH_STRINGSET_H

#include <cctype>
#include <utility>
#include <cassert>
#include <algorithm>
#include <memory>
#include <list>

#include <initializer_list>

// Flag
//#define _DSH_OLD
// Flag

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 11 / 29                                             *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                    *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # String Set                                                        *
*                                                                       *
*      # version 1 : 基于字典树的String Set                             *
*                                                                       *
*      # version 2 : 增加字典序导出                                     *
*                    增加新哈希                                         *
*                                                                       *
*      # version 3 : 增加内存优先策略                                   *
*                                                                       *
*************************************************************************/


namespace DSH
{
	namespace TR1
	{
		template<typename charT , bool is_lower>
		struct alpha{
		public:
			typedef charT char_type ;
			inline std::size_t max_value() const {
				return 26 ;
			}

			inline std::size_t operator()(charT _ch) const {
				return _ch - charT('a');
			}
		};

		template<typename charT>
		struct alpha<charT , false> {
		public:
			typedef charT char_type ;
			inline std::size_t max_value() const {
				return 26 ;
			}

			inline std::size_t operator()(charT _ch) const {
				return _ch - charT('A');
			}
		};

		template<typename charT>
		struct number {
		public:
			typedef charT char_type ;
			inline std::size_t max_value() const {
				return 10 ; 
			}

			inline std::size_t operator()(charT _ch) const {
				return _ch - charT('0');
			}
		};

		// 数字字母大小写混合
		template<typename charT>
		struct alpha_number
		{
		public:
			typedef charT char_type ;
			inline std::size_t max_value() const {
				return 10 + 26 + 26 ;
			}

			inline std::size_t operator()(charT _ch)const {
				if( std::isdigit( _ch ) ) return _ch - charT('0');
				if( std::islower( _ch ) ) return _ch - charT('a') + 10 ;
				else return _ch - charT('A') + 36 ;
			}
		};
	}
}

namespace _DSH
{
	template<typename charT , bool is_hash>
	class _trie_node
	{
	public:
		typedef charT             char_type;
		typedef bool              flag_type;
		typedef std::size_t       size_type  ;

		typedef _trie_node<charT ,is_hash> _Self ;
		typedef _Self*                     pointer ;
		typedef _Self*                     child_type ;
		typedef std::allocator<pointer>    alloc_type ;

	public:
		_trie_node()
			:value() , is_end(false) , buf_size(0),child_num(0),is_nil(false) child(nullptr)
		{ }

		_trie_node(char_type _value , flag_type _is_end , std::size_t _buf_size , flag_type _is_nil)
			:value(_value),is_end(_is_end),buf_size(_buf_size),child_num(0),is_nil(_is_nil),child(nullptr)
		{
			try {
				child = this->_get_alloc().allocate( buf_size );
				for(size_type i = 0 ; i < buf_size ; ++i )
					this->_get_alloc().construct( child + i , nullptr );
			} catch(...) {
				throw ;
			}
		}

		~_trie_node() { 
			for(size_type i = 0 ; i < buf_size  ; ++i )
				this->_get_alloc().destroy( child + i );
			this->_get_alloc().deallocate( child , buf_size );
			child = nullptr ;
			parent = nullptr ;
		}

	private:
		//Forbid
		_trie_node(const _Self& other) ;
		_trie_node(_Self&& other) ;
		_Self& operator = (const _Self& other) ;
		_Self& operator = (_Self&& other) ;

	public:

		inline pointer& operator[](size_type key) { 
			assert(child != nullptr);
			return *(child + key) ;
		}

		inline const pointer operator[](size_type key) const {
			assert(child != nullptr);
			return *(child + key);
		}

		inline pointer&  getParent()  { return parent ; }
		inline const pointer getParent() const { return parent ; }

		inline char_type& getValue() { return value ; }
		inline const char_type getValue() const { return value ;}

		inline size_type& getSize() { return buf_size; } 
		inline const size_type getSize() const { return buf_size ; }

		inline size_type& getChildSize() { return child_num ; } 
		inline const size_type getChildSize() const { return child_num ; }

		inline flag_type& getEnd() { return is_end; }
		inline const flag_type getEnd() const { return is_end ; }

		inline flag_type& getNil() { return is_nil ; }
		inline const flag_type getNil() const { return is_nil ;}

	private:
		alloc_type _get_alloc() const { return alloc_type() ; } 

	private:
		char_type  value ;
		flag_type  is_end ;
		flag_type  is_nil ;
		size_type  buf_size; 
		size_type  child_num ;
		child_type *child ;
		pointer    parent ;
	};

	template<typename charT>
	class _trie_node<charT , false>
	{
	public:
		typedef charT             char_type;
		typedef bool              flag_type;
		typedef std::size_t       size_type  ;

		typedef _trie_node<charT ,false>   _Self ;
		typedef _Self*                     pointer ;
		typedef std::allocator<pointer>    alloc_type ;

		typedef std::list<std::pair<size_type,pointer>>       child_type ;
		typedef typename child_type::iterator                 iterator ;

	public:
		_trie_node()
			:value() , is_end(false) , buf_size(0),child_num(0),is_nil(false) child()
		{ }

		_trie_node(char_type _value , flag_type _is_end , std::size_t _buf_size , flag_type _is_nil)
			:value(_value),is_end(_is_end),buf_size(_buf_size),child_num(0),is_nil(_is_nil),child()
		{
//			try {
//				child = this->_get_alloc().allocate( buf_size );
//				for(size_type i = 0 ; i < buf_size ; ++i )
//					this->_get_alloc().construct( child + i , nullptr );
//			} catch(...) {
//				throw ;
//			}


		}

		~_trie_node() { 
//			for(size_type i = 0 ; i < buf_size  ; ++i )
//				this->_get_alloc().destroy( child + i );
//			this->_get_alloc().deallocate( child , buf_size );
//			child = nullptr ;

			parent = nullptr ;
		}

	private:
		//Forbid
		_trie_node(const _Self& other) ;
		_trie_node(_Self&& other) ;
		_Self& operator = (const _Self& other) ;
		_Self& operator = (_Self&& other) ;

	public:
		typename child_type::iterator& child_begin() {
			return child.begin();
		}

		const typename child_type::iterator child_begin() const {
			return child.cbegin() ;
		}

		typename child_type::iterator& child_end() {
			return child.end();
		}

		const typename child_type::iterator child_end() const {
			return child.cend();
		}

		inline pointer& operator[](size_type key) { 
//			assert(child != nullptr);
			typedef typename child_type::iterator iter_type ;

			iter_type pCur(std::begin(child));
			for( ; pCur != std::end(child) ; ++pCur)
				if( pCur->first == key ) return pCur->second ;

			child.push_back( std::make_pair( key , nullptr ) ) ;
			return (*(child.rbegin())).second ;
		}

		inline const pointer operator[](size_type key) const {
//			assert(child != nullptr);
			typedef typename child_type::iterator iter_type ;

			for(iter_type pCur( std::begin(child) ) ; pCur != std::end(child) ; ++pCur)
				if( pCur->first == key ) return pCur->second ;

			return nullptr ;
		}

		inline pointer&  getParent()  { return parent ; }
		inline const pointer getParent() const { return parent ; }

		inline char_type& getValue() { return value ; }
		inline const char_type getValue() const { return value ;}

		inline size_type& getSize() { return buf_size; } 
		inline const size_type getSize() const { return buf_size ; }

		inline size_type& getChildSize() { return child_num ; } 
		inline const size_type getChildSize() const { return child_num ; }

		inline flag_type& getEnd() { return is_end; }
		inline const flag_type getEnd() const { return is_end ; }

		inline flag_type& getNil() { return is_nil ; }
		inline const flag_type getNil() const { return is_nil ;}

	private:
		alloc_type _get_alloc() const { return alloc_type() ; } 

	private:
		char_type  value ;
		flag_type  is_end ;
		flag_type  is_nil ;
		size_type  child_num ;
		size_type  buf_size; 

//		pointer    *child ;

		child_type child ;
		pointer    parent ;
	};

	



	template<typename node_type , bool is_hash>
	class _trie_helper
	{
	public:
		const static std::size_t MAX_LEN = 256 ;

		static inline 
			bool canBeDestroy( node_type *ptr )
		{
			//			node_type **end = &((*ptr)[buf_size]) ;
			//			for( node_type **begin =  &((*ptr)[0]) ; begin != end; ++begin )
			//				if( (*begin) != nullptr ) return false ;
			//			return true ;

			return ptr->getChildSize() == 0 ;
		}

#ifdef _DSH_OLD
		template<typename FwdIter>
		static inline
			FwdIter _lexicographical_traverse(node_type *root , FwdIter dResult ) 
		{
			typedef typename node_type::size_type size_type ;

			if( root == nullptr ) return dResult;

			if( root->getEnd() && !root->getNil() )
				//  one string
			{
				_export(root , dResult);
			}

			if( root->getChildSize() == 0 ) return dResult;

			node_type **begin = &((*root)[0]);
			node_type **end = &((*root)[root->getSize()]);

			while( begin != end )
			{
				dResult = _lexicographical_traverse( *begin++ , dResult );
			}

			return dResult ;
		}
#else
		template<typename FwdIter>
		static inline
			FwdIter _lexicographical_traverse( node_type *root , FwdIter dResult ) 
		{

			typedef typename node_type::size_type size_type ;

			if( root == nullptr || canBeDestroy(root) ) return dResult;

			size_type  _Top(0) ;
			node_type *_Stack[MAX_LEN] = { nullptr };

			_Stack[_Top++] = root ;
			// Traverse
			
			while( _Top )
			{
				node_type *pTmp = _Stack[--_Top];

				assert( pTmp != nullptr );

				if( pTmp->getEnd() && !pTmp->getNil() )
					_export( pTmp , dResult );

				if( pTmp->getChildSize() == 0 ) continue ;

				node_type **begin =  &((*pTmp)[0]) ;
				node_type **end   =  &((*pTmp)[pTmp->getSize()]) ;

				for( --end ; end >= begin ; --end )
				{
					if( *end != nullptr )
					{
						assert( _Top <= MAX_LEN );
						_Stack[_Top++] = *end ;
					}
				}
			}

			return dResult ;
		}
#endif
		template<typename FwdIter>
		static inline void _export( node_type *pCur , FwdIter& dResult )
		{
			typedef typename node_type::char_type char_type ;
//			typedef typename node_type::trait_type trait_type ;

			std::basic_string<char_type> _Tmp("");

			const node_type *Cur( pCur ) ;
			while( !Cur->getNil() )
			{
				_Tmp += Cur->getValue() ;
				Cur = Cur->getParent() ;
			}
			std::reverse( std::begin( _Tmp ) , std::end( _Tmp ) );

			*dResult++ = std::move(_Tmp) ;
		}
	};


	template<typename node_type>
	class _trie_helper<node_type,false>
	{
	public:
		const static std::size_t MAX_LEN = 256 ;

		static inline 
			bool canBeDestroy( node_type *ptr )
		{
			//			node_type **end = &((*ptr)[buf_size]) ;
			//			for( node_type **begin =  &((*ptr)[0]) ; begin != end; ++begin )
			//				if( (*begin) != nullptr ) return false ;
			//			return true ;

			return ptr->getChildSize() == 0 ;
		}

		template<typename FwdIter>
		static inline
			FwdIter _lexicographical_traverse( node_type *root , FwdIter dResult ) 
		{

			typedef typename node_type::size_type size_type ;

			if( root == nullptr || canBeDestroy(root) ) return dResult;

			size_type  _Top(0) ;
			node_type *_Stack[MAX_LEN] = { nullptr };

			_Stack[_Top++] = root ;
			// Traverse
			
			while( _Top )
			{
				node_type *pTmp = _Stack[--_Top];

				assert( pTmp != nullptr );

				if( pTmp->getEnd() && !pTmp->getNil() )
					_export( pTmp , dResult );

				if( pTmp->getChildSize() == 0 ) continue ;

				typename node_type::iterator begin( pTmp->child_begin());
				typename node_type::iterator end( pTmp->child_end() );

				for( ; begin != end ; ++begin )
				{
					if( begin->second != nullptr )
						_Stack[_Top++] = begin->second ;
				}
			}

			return dResult ;
		}

		template<typename FwdIter>
		static inline void _export( node_type *pCur , FwdIter& dResult )
		{
			typedef typename node_type::char_type char_type ;
//			typedef typename node_type::trait_type trait_type ;

			std::basic_string<char_type> _Tmp("");

			const node_type *Cur( pCur ) ;
			while( !Cur->getNil() )
			{
				_Tmp += Cur->getValue() ;
				Cur = Cur->getParent() ;
			}
			std::reverse( std::begin( _Tmp ) , std::end( _Tmp ) );

			*dResult++ = std::move(_Tmp) ;
		}
	};




	template<typename charT,typename charHash,bool is_hash = true , typename charTraits = std::char_traits<charT>>
	class _trie
	{
	public:
		typedef _trie<charT,charHash,is_hash,charTraits> _Self;
		typedef charT                          char_type ;
		typedef _trie_node<char_type,is_hash>  node_type ;
		typedef charHash                    hash_type ;
		typedef std::allocator<node_type>  alloc_type ;
		typedef std::size_t                size_type  ;
		typedef charTraits                 trait_type ;

	public:
		_trie()
			:buf_size(),mRoot(nullptr),hasher() 
		{
			try {
				buf_size = hasher.max_value() ;

				//  construct root node
				mRoot = this->_get_alloc().allocate(1);
				this->_get_alloc().construct( mRoot ,
					char_type(),
					false ,
					buf_size,
					true );
			} catch(...) {
				throw;
			}
		} 

		_trie(_Self&& other)
			:buf_size(other.buf_size),mRoot(other.mRoot),hasher(std::move(other.hasher))
		{
			other.mRoot = nullptr ;
		}

		_trie(const _Self& other)
			:buf_size(other.buf_size),mRoot(nullptr),hasher(other.hasher)
		{
			_copy( mRoot , other.mRoot) ;
		}

		~_trie() 
		{
			_tidy( mRoot );
		}

		_Self& operator = (_Self&& other)
		{
			buf_size = other.buf_size ;
			mRoot = other.mRoot ;
			hasher = std::move(other.hasher) ;
			other.mRoot = nullptr ;

			return *this ;
		}

		_Self& operator = (const _Self& other)
		{
			if( this == std::addressof(other) ) return *this;

			_copy( mRoot , other.mRoot );
			return *this ;
		}

	public:
		inline 
			bool find( const char_type* _string ) const
		{
			assert( _string != nullptr );

			node_type *pCur( mRoot );

			while( *_string != char_type() ) {
				pCur = (*pCur)[ hasher( *_string ) ] ;

				if( pCur == nullptr )
					return false ;

				++_string ;
			}

			if( pCur->getEnd() || pCur->getNil() ) return true ;
			else return false ;
		}

		inline 
			bool insert( const char_type* _string ) 
		{
			assert( _string != nullptr ) ;

			node_type *pCur(mRoot);

			while( *_string != char_type() ) {
				size_type index( hasher(*_string) );
				node_type *tmp( (*pCur)[index] );

				if( tmp == nullptr )
				{
					try {
						(*pCur)[index] = this->_get_alloc().allocate(1);
						this->_get_alloc().construct( (*pCur)[index] ,
							*_string,
							false ,
							buf_size ,
							false );

						// Set Parent 
						((*pCur)[index])->getParent() = pCur ;

						// Child Size Increase
						++pCur->getChildSize() ;
					} catch(...) { throw ; }
				}

				// Move to Next Node
				pCur = (*pCur)[index] ;
				// Move to Next Char
				++_string ;
			}

			if( pCur->getNil() || pCur->getEnd() )
				return false ;

			pCur->getEnd() = true ;

			return true;
		}

		inline 
			bool erase( const char_type* _string )
		{
			assert( _string != nullptr );

			node_type *pCur( mRoot );

			while( *_string != char_type() ) {
				pCur = (*pCur)[ hasher( *_string ) ] ;

				if( pCur == nullptr )
					return false ;

				++_string ;
			}

			if( !pCur->getEnd() && !pCur->getNil() )
				return false ;

			if( pCur->getNil() ) return true ;

			if( !_DSH::_trie_helper<node_type,is_hash>::canBeDestroy(pCur) )
				// Can Not Destroy this node 
			{
				pCur->getEnd() = false ;

				return true ;
			}

			// Can destroy this node  
			node_type *pFather( pCur->getParent() );
			(*pFather)[ hasher(pCur->getValue()) ] = nullptr ;

			// Decrease Child Size
			--pFather->getChildSize() ;

			this->_get_alloc().destroy( pCur );
			this->_get_alloc().deallocate(pCur ,1) ;
			pCur = pFather ;

			// Trace Back Delete 
			while( !pCur->getNil() ) {
				pFather = pCur->getParent() ;

				if( !pCur->getEnd() && _DSH::_trie_helper<node_type,is_hash>::canBeDestroy(pCur) ) {
					(*pFather)[ hasher(pCur->getValue()) ] = nullptr ;

					this->_get_alloc().destroy( pCur );
					this->_get_alloc().deallocate( pCur , 1);

					pCur = pFather ;

					if( !pCur->getNil() )
						--pCur->getChildSize();
					else
						break ;
				}
				else
				{
					break; 
				}
			}

			// Successfully erase
			return true ;
		}

		template<typename FwdIter>
		inline 
			FwdIter lexicographical_copy( FwdIter dResult ) const
			// *dResult shall has operator +=() 
		{
			return _DSH::_trie_helper<node_type,is_hash>::_lexicographical_traverse( mRoot , dResult );
		}

	private:

		void _copy( node_type *&myRoot ,const node_type *otherRoot ) 
		{
			if( otherRoot != nullptr )
			{
				try {
					myRoot = this->_get_alloc().allocate(1);
					this->_get_alloc().construct( myRoot ,
						otherRoot->getValue(),
						otherRoot->getEnd(),
						otherRoot->getSize(),
						otherRoot->getNil()
						);

					mRoot->getChildSize() = otherRoot->getChildSize() ;

					// Copy the sub-tree
					for(size_type i = 0 ; i < buf_size ; ++i)
					{
						_copy( (*myRoot)[i] , (*otherRoot)[i] );

						if( (*myRoot)[i] != nullptr )
							(*myRoot)[i]->getParent() = myRoot ;
					}

				} catch(...) {
					throw ;
				}
			}
		}

		void _tidy( node_type *&myRoot )
		{
			if(myRoot != nullptr)
			{
				for(size_type i = 0 ; i < buf_size ; ++i )
				{
					_tidy( (*myRoot)[i] );
				}

				this->_get_alloc().destroy( myRoot );
				this->_get_alloc().deallocate( myRoot , 1 );
				myRoot = nullptr ;
			}
		}

		alloc_type _get_alloc() const { return alloc_type(); }
	private:
		size_type buf_size ;
		node_type *mRoot ;
		hash_type hasher ;

	};
}

namespace DSH
{
	// Speed Priority
	template<typename charHash , bool speedPriority = true>
	class stringset {
	public:
		typedef charHash hash_type ;
		typedef std::char_traits<typename charHash::char_type> trait_type ;
		typedef typename hash_type::char_type char_type ;

		typedef stringset<hash_type,speedPriority> self ;

	private:
		typedef _DSH::_trie<char_type,hash_type,speedPriority,trait_type>  container_type ;

	public:
		stringset()
			:m_size(0),m_trie() 
		{ }

		stringset( const self& other )
			:m_size(other.m_size) , m_trie(other.m_trie)
		{ }

		stringset(self&& other)
			:m_size(other.m_size) , m_trie(std::move(other.m_trie))
		{ }

		stringset( std::initializer_list<std::basic_string<char_type,trait_type>> _list )
			:m_size( _list.size() )
		{
			for( auto Cur( std::begin(_list) ) ; Cur != std::end(_list) ; ++Cur )
				this->insert( *Cur );
		}

		~stringset()
		{ // 
		}

		self& operator = (const self& other)
		{
			if( this == std::addressof(other) ) return *this ;
			m_size = other.m_size ;
			m_trie = other.m_trie ;

			return *this ;
		}

		self& operator = (self&& other)
		{
			if( this == std::addressof(other)) return *this;
			m_size = other.m_size ;
			m_trie = std::move(other.m_trie);

			return *this ;
		}
	public:

//		bool insert( const char_type* _string ) 
//		{
//			if( m_trie.insert( _string ) )
//			{
//				++m_size ;
//				return true ;
//			}
//			return false ;
//		}

		bool insert( const std::basic_string<char_type,trait_type>& _string )
		{
			if( m_trie.insert( _string.data() ) )
			{
				++m_size;
				return true;
			}
			return false;
		}

//		bool erase( const char_type* _string )
//		{
//			if( m_trie.erase( _string ) )
//			{
//				--m_size ;
//				return true ;
//			}
//			return false ;
//		}

		bool erase( const std::basic_string<char_type,trait_type>& _string )
		{
			if( m_trie.erase( _string.data() ) )
			{
				--m_size ;
				return true ;
			}
			return false ;
		}

//		bool find( const char_type* _string ) const
//		{
//			return m_trie.find( _string );
//		}

		bool find( const std::basic_string<char_type,trait_type>& _string ) const
		{
			return m_trie.find( _string.data() );
		}

		inline std::size_t size() const 
		{
			return m_size ;
		}

		inline bool empty() const 
		{
			return m_size == 0 ;
		}

		template<typename FwdIter>
		inline 
			FwdIter lexicographical_copy( FwdIter dResult ) const
		{
			return m_trie.lexicographical_copy( dResult );
		}

	private:
		std::size_t    m_size ;
		container_type m_trie ;
	};

	// Memory Priority 
	template<typename charHash>
	class stringset<charHash , false> 
	{
	public:
		typedef charHash hash_type ;
		typedef std::char_traits<typename charHash::char_type> trait_type ;
		typedef typename hash_type::char_type char_type ;

		typedef stringset<hash_type,false> self ;

	private:
		typedef _DSH::_trie<char_type,hash_type,false,trait_type>  container_type ;

	public:
		stringset()
			:m_size(0),m_trie() 
		{ }

		stringset( const self& other )
			:m_size(other.m_size) , m_trie(other.m_trie)
		{ }

		stringset(self&& other)
			:m_size(other.m_size) , m_trie(std::move(other.m_trie))
		{ }
		
		stringset(std::initializer_list<std::basic_string<char_type,trait_type>> _list)
			:m_size(_list.size())
		{
			for(auto Cur(std::begin(_list)) ; Cur != std::end(_list) ; ++Cur )
				this->insert( *Cur );
		}

		~stringset()
		{ // 
		}

		self& operator = (const self& other)
		{
			if( this == std::addressof(other) ) return *this ;
			m_size = other.m_size ;
			m_trie = other.m_trie ;

			return *this ;
		}

		self& operator = (self&& other)
		{
			if( this == std::addressof(other)) return *this;
			m_size = other.m_size ;
			m_trie = std::move(other.m_trie);

			return *this ;
		}
	public:

//		bool insert( const char_type* _string ) 
//		{
//			if( m_trie.insert( _string ) )
//			{
//				++m_size ;
//				return true ;
//			}
//			return false ;
//		}

		bool insert( const std::basic_string<char_type,trait_type>& _string )
		{
			if( m_trie.insert( _string.data() ) )
			{
				++m_size;
				return true;
			}
			return false;
		}

//		bool erase( const char_type* _string )
//		{
//			if( m_trie.erase( _string ) )
//			{
//				--m_size ;
//				return true ;
//			}
//			return false ;
//		}

		bool erase( const std::basic_string<char_type,trait_type>& _string )
		{
			if( m_trie.erase( _string.data() ) )
			{
				--m_size ;
				return true ;
			}
			return false ;
		}

//		bool find( const char_type* _string ) const
//		{
//			return m_trie.find( _string );
//		}

		bool find( const std::basic_string<char_type,trait_type>& _string ) const
		{
			return m_trie.find( _string.data() );
		}

		inline std::size_t size() const 
		{
			return m_size ;
		}

		inline bool empty() const 
		{
			return m_size == 0 ;
		}

		template<typename FwdIter>
		inline 
			FwdIter lexicographical_copy( FwdIter dResult ) const
		{
			return m_trie.lexicographical_copy( dResult );
		}

	private:
		std::size_t    m_size ;
		container_type m_trie ;
	};
}

#endif