#ifndef DSH_UTILITY_H
#define DSH_UTILITY_H

#include <utility>
#include <memory>

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
*      # version 1 : RAII缓冲区管理                                     *
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
		SafePtrMgr( std::size_t _BlockSize )
			:mBlockSize(_BlockSize) , mAllocator() ,  mBuffer(mAllocator.allocate(mBlockSize))
		{
			//
			for(std::size_t i = 0 ; i < mBlockSize ; ++i )
				mAllocator.construct( mBuffer + i , Ty()) ;
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

	template<typename Ty , typename Alloc , typename DeAlloc>
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
			:m_allocator() , m_deallocator() , mBuffer(nullptr) 
		{
			m_allocator( mBuffer );
		}

		SafeSrcMgr( Self&& other )
			:m_allocator(std::move(other.m_allocator)),
			m_deallocator(std::move(other.m_deallocator)),
			mBuffer(other.mBuffer) 
		{
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
		}

		inline 
			void release() 
		{
			m_deallocator( mBuffer );
		}

		inline 
			Ty* get() const
		{
			return mBuffer ;
		}

	private:

		Alloc   m_allocator ;
		DeAlloc m_deallocator ;
		Ty     *mBuffer ;
	};
}

#endif