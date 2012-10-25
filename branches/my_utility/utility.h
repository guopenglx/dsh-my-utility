#ifndef DSH_UTILITY_H
#define DSH_UTILITY_H

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

		SafePtrMgr( std::size_t _BlockSize )
			:mBlockSize(_BlockSize) , mAllocator() ,  mBuffer(mAllocator.allocate(mBlockSize))
		{
			//
		}

		SafePtrMgr( Self&& other ) 
			:mBlockSize(other.mBlockSize) , mAllocator(std::move(other.mAllocator)) ,
			mBuffer( other.mBuffer )
		{
			//
		}

		~SafePtrMgr()
		{
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
			}
		}

		inline 
			void release()
		{
			mAllocator.deallocate( mBuffer , mBlockSize );
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

		Alloc mAllocator ;

		Ty *mBuffer ;
	};
}

#endif