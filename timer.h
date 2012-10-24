#ifndef _DSH_TIMER_H_INCLUDED
#define _DSH_TIMER_H_INCLUDED

#include <chrono>


/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2012 / 7 / 15                                              *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                    *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # Timer                                                             *
*                                                                       *
*                                                                       *
*************************************************************************/

namespace DSH {
	class timer
	{
	public:
		timer()
			:mStart(std::chrono::system_clock::now())
		{
			//
		}

		~timer()
		{
			//
		}

	public:
		void reset()
		{
			mStart = std::chrono::system_clock::now();
		}

		// Seconds
		double elapsed() 
		{
			mNow = std::chrono::system_clock::now();
			return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(mNow - mStart).count())
				/1000000 ;
		}

	private:
		std::chrono::time_point<std::chrono::system_clock> mStart ;
		std::chrono::time_point<std::chrono::system_clock> mNow ;
	};
}
#endif // TIMER_H_INCLUDED
