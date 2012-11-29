#ifndef DSH_UTILITY_H
#define DSH_UTILITY_H

#include <utility>
#include <memory>
#include <functional>

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 10 / 24                                             *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                    *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # Utility Library                                                   *
*                                                                       *
*      # version 1 : RAII   资源管理                                    *
*************************************************************************/


namespace DSH
{
	/* Safe Pointer Manager */

	template<typename Ty , typename Alloc = std::allocator<Ty>>
	class SafePtrMgr
	{
	public:
		typedef typename SafePtrMgr<Ty , Alloc> Self ;
	private:
		/* Forbid  Copy Constructor  */
		SafePtrMgr( const Self& other) {} ;

		/* Forbid  Copy Assignment  */
		Self& operator = ( const Self& other ) {} ;

	public:

		SafePtrMgr()
			:mBlockSize(0) , mAllocator() , mBuffer( nullptr )
		{
			//
		}

		// Initial pointer by BlockSize 
		// ptr = new Ty[BlockSize] 
		SafePtrMgr( std::size_t _BlockSize , const Ty& _Value = Ty() )
			:mBlockSize(_BlockSize) , mAllocator() ,  mBuffer(mAllocator.allocate(mBlockSize))
		{
			//
			if( mBuffer != nullptr ){
				for(std::size_t i = 0 ; i < mBlockSize ; ++i )
					mAllocator.construct( mBuffer + i , _Value ) ;
			}
		}

		SafePtrMgr( Self&& other ) 
			:mBlockSize(other.mBlockSize) , mAllocator(std::move(other.mAllocator)) ,
			mBuffer( other.mBuffer )
		{
			//
			other.mBuffer = nullptr ;
		}

		~SafePtrMgr()
		{
			if( mBuffer )
				release() ;
		}

	public:

		Self& operator = ( Self&& other )
		{
			if( this == std::addressof(other) ) return *this ;
			else
			{
				release() ;
				
				mBlockSize  = other.mBlockSize ;
				mAllocator  = other.mAllocator ;
				mBuffer     = other.mBuffer ;
				other.mBuffer = nullptr ;

				return *this ;
			}
		}

		inline 
			void release()
		{
			for(std::size_t i = 0 ; i < mBlockSize ; ++i )
				mAllocator.destroy( mBuffer + i );
			mAllocator.deallocate( mBuffer , mBlockSize );
			mBuffer = nullptr ;
		}

		inline 
			Ty * get() 
		{
			return mBuffer ;
		}

		inline
			const Ty * get() const
		{
			return mBuffer ;
		}

		inline 
			operator bool () const
		{
			return mBuffer != nullptr ;
		}
	private:
		/* Member Var */
		std::size_t mBlockSize ;
		Alloc       mAllocator ;
		Ty         *mBuffer ;
	};

	//
	//  param : Ty -> Type of handle 
	//          Alloc -> Type of Function "allocator" 
	//                   # opertator()(Ty *_buffer) : allocate and stored in buffer,nullptr if failed
	//
	//          DeAlloc -> Type of Function "deallocator"
	//                   # operator()( Ty *_buffer) : deallocate the _buffer 
	//
	template<typename Ty ,
		     typename Alloc = std::function<Ty*(void)>,
			 typename DeAlloc = std::function<void(Ty*)>>
	class SafeSrcMgr
	{
	public:
		typedef typename SafeSrcMgr<Ty , Alloc , DeAlloc> Self ;
	private:
		/* Forbid  Copy Constructor  */
		SafeSrcMgr(const Self& other) ;

		/* Forbid  Copy Assignment  */
		Self& operator = (const Self& other) ;

	public:
		SafeSrcMgr( Alloc _allocator , DeAlloc _deallocator) 
			: mBuffer(nullptr) 
		{
			m_allocator = _allocator ;
			m_deallocator = _deallocator ;

			mBuffer = m_allocator();
		}

		SafeSrcMgr( Self&& other )
			:mBuffer(other.mBuffer) 
		{
			m_allocator = std::move(other.m_allocator);
			m_deallocator = std::move(other.m_deallocator);
			//
			other.mBuffer = nullptr ;
		}

		~SafeSrcMgr()
		{
			if( mBuffer ) 
				release() ;
		}
	public:

		Self& operator = (Self&& other)
		{
			if( this == std::addressof(other) ) return *this ;

			release() ;
			m_allocator   = std::move(other.m_allocator);
			m_deallocator = std::move(other.m_deallocator);
			mBuffer       = std::move(other.mBuffer);
			other.mBuffer = nullptr ;

			return *this ;
		}

		inline 
			void release() 
		{
			m_deallocator( mBuffer );
		}

		inline 
			Ty* get() 
		{
			return mBuffer ;
		}

		inline 
			const Ty* get() const
		{
			return mBuffer ;
		}

		inline 
			operator bool () const
		{
			return mBuffer != nullptr ;
		}

	private:

		Alloc   m_allocator ;
		DeAlloc m_deallocator ;
		Ty     *mBuffer ;
	};
}

#endif