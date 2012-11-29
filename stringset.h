#ifndef DSH_STRINGSET_H
#define DSH_STRINGSET_H

#include <cctype>
#include <utility>
#include <cassert>
#include <algorithm>
#include <memory>

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
*      # version 2 : 增加字典序导出                                     *
*                    增加新哈希                                         *
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

	template<typename charT , typename boolT>
	class _trie_node
	{
	public:
		typedef charT             char_type;
		typedef boolT             flag_type;
		typedef std::size_t       size_type  ;

		typedef _trie_node<charT,boolT> _Self;
		typedef _Self*                  pointer ;
		typedef std::allocator<pointer> alloc_type ;

	public:
		_trie_node()
			:value() , is_end(false) , buf_size(0) ,is_nil(false) child(nullptr)
		{ }

		_trie_node(char_type _value , flag_type _is_end , std::size_t _buf_size , flag_type _is_nil)
			:value(_value),is_end(_is_end),buf_size(_buf_size),is_nil(_is_nil),child(nullptr)
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
		pointer    *child ;
		pointer    parent ;
	};

	template<typename charT,typename charHash,typename charTraits = std::char_traits<charT>>
	class _trie
	{
	public:
		typedef _trie<charT,charHash,charTraits> _Self;
		typedef charT                       char_type ;
		typedef _trie_node<char_type ,bool> node_type ;
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

			if( !canBeDestroy(pCur) )
			// Can Not Destroy this node 
			{
				pCur->getEnd() = false ;

				return true ;
			}

			// Can destroy this node  
			node_type *pFather( pCur->getParent() );
			(*pFather)[ hasher(pCur->getValue()) ] = nullptr ;

			this->_get_alloc().destroy( pCur );
			this->_get_alloc().deallocate(pCur ,1) ;
			pCur = pFather ;

			// Trace Back Delete 
			while( !pCur->getNil() ) {
				pFather = pCur->getParent() ;

				if( !pCur->getEnd() && canBeDestroy(pCur) ) {
					(*pFather)[ hasher(pCur->getValue()) ] = nullptr ;

					this->_get_alloc().destroy( pCur );
					this->_get_alloc().deallocate(pCur , 1);

					pCur = pFather ;
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
			_lexicographical_traverse( mRoot , dResult );
			return dResult ;
		}

	private:
		template<typename FwdIter>
		inline
			void _lexicographical_traverse(node_type*root , FwdIter& dResult ) const
		{
			if( root == nullptr ) return ;
			
			if( root->getEnd() && !root->getNil() )
				//  one string
			{
				std::basic_string<char_type,trait_type> _Tmp("");

				const node_type *Cur( root ) ;
				while( !Cur->getNil() )
				{
					_Tmp += Cur->getValue() ;
				}
				std::reverse( std::begin( _Tmp ) , std::end( _Tmp ) );

				*dResult++ = std::move(_Tmp) ;
			}

			node_type **begin = &((*root)[0]);
			node_type **end = &((*root)[buf_size]);

			while( begin != end )
			{
				_lexicographical_traverse( *begin++ , dResult );
			}
		}

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

		inline bool canBeDestroy( node_type *ptr ) const
		{
			node_type **end = &((*ptr)[buf_size]) ;
			for( node_type **begin =  &((*ptr)[0]) ; begin != end; ++begin )
				if( (*begin) != nullptr ) return false ;
			return true ;
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
	template<typename charHash ,
		     typename charTraits = std::char_traits<typename charHash::char_type>>
	class stringset {
	public:
		typedef charHash hash_type ;
		typedef charTraits trait_type ;
		typedef typename hash_type::char_type char_type ;

		typedef stringset<hash_type,trait_type> self ;

	private:
		typedef _DSH::_trie<char_type,hash_type,trait_type>  container_type ;

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
		bool insert( const char_type* _string ) 
		{
			if( m_trie.insert( _string ) )
			{
				++m_size ;
				return true ;
			}
			return false ;
		}

		bool erase( const char_type* _string )
		{
			if( m_trie.erase( _string ) )
			{
				--m_size ;
				return true ;
			}
			return false ;
		}

		bool find( const char_type* _string ) const
		{
			return m_trie.find( _string );
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