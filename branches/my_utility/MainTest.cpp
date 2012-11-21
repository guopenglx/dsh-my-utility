#include <iostream>
#include <string>
#include <map>
#include <set>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <my_utility\map.h>
#include <my_utility\set.h>
#include <my_utility\timer.h>
#include <my_utility\delegater.h>
#include <my_utility\utility.h>

const int N = 1000000 ; 

int main()
{
	DSH::map<int,int>  x ;
	std::map<int,int>  xx ;

	DSH::timer mTimer ;

	std::cout << "N = " << N << std::endl;
	std::cout << std::endl;
	std::cout << "Insert " << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		x.insert(std::make_pair(i,i));
	}
	std::cout << "My map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		xx.insert(std::make_pair(i,i));
	}
	std::cout << "STL map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Searching" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		x.find(i);
	}
	std::cout << "My map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		xx.find(i);
	}
	std::cout << "STL map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Traverse" << std::endl;
	mTimer.reset() ;
	for(auto i = x.begin() ; i != x.end() ; ++i );
	std::cout << "My map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(auto i = xx.begin() ; i != xx.end() ; ++i );
	std::cout << "STL map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Access" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i )
		x[i] = i ;
	std::cout << "My map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i )
		xx[i] = i ;
	std::cout << "STL map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Synthesize Testing " << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) {
		auto iy = x.find(i);
		x.erase(iy->first);
		x.insert(std::make_pair(i,i));
	}
	std::cout << "My map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) {
		auto iy = xx.find(i);
		xx.erase(iy->first);
		xx.insert(std::make_pair(i,i));
	}
	std::cout << "STL map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	/// Copy to bak 
	DSH::map<int,int> xb = x ;
	std::map<int,int> xxb = xx ;
	std::cout << std::endl;
	std::cout << "Copy Assignment Testing" << std::endl;
	if( std::equal( xb.begin() , xb.end() , x.begin() ) )
		std::cout << "My Map : Passed" << std::endl;
	else
		std::cout << "My Map : Failed" << std::endl;
	if( std::equal( xxb.begin() , xxb.end() , xx.begin() ) )
		std::cout << "STL Map : Passed" << std::endl;
	else
		std::cout << "STL Map : Failed" << std::endl;

	std::cout << std::endl;
	std::cout << "Erasing" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		x.erase(i);
	}
	std::cout << "My map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		xx.erase(i);
	}
	std::cout << "STL map : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "now My map size : " << x.size() << std::endl;
	std::cout << "now STL map size : " << xx.size() << std::endl;
	

	/*  Set  */

	DSH::set<int>  y ;
	std::set<int>  yy ;
	std::cout << std::endl;
	std::cout << "N = " << N << std::endl;
	std::cout << std::endl;
	std::cout << "Insert " << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		y.insert(i);
	}
	std::cout << "My set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		yy.insert(i);
	}
	std::cout << "STL set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Searching" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		y.find(i);
	}
	std::cout << "My set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		yy.find(i);
	}
	std::cout << "STL set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Traverse" << std::endl;
	mTimer.reset() ;
	for(auto i = y.begin() ; i != y.end() ; ++i );
	std::cout << "My set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(auto i = yy.begin() ; i != yy.end() ; ++i );
	std::cout << "STL set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "Synthesize Testing " << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) {
		auto iy = y.find(i);
		y.erase(*iy);
		y.insert(i);
	}
	std::cout << "My set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) {
		auto iy = yy.find(i);
		yy.erase(*iy);
		yy.insert(i);
	}
	std::cout << "STL set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	/// Copy to bak
	DSH::set<int> yb = y ;
	std::set<int> yyb = yy;
	std::cout << std::endl;
	std::cout << "Copy Assignment Testing" << std::endl;
	
	if( std::equal( yb.begin() , yb.end() , y.begin() ) )
		std::cout << "My Set : Passed" << std::endl;
	else
		std::cout << "My Set : Failed" << std::endl;
	if( std::equal( yyb.begin() , yyb.end() , yy.begin() ) )
		std::cout << "STL Set : Passed" << std::endl;
	else
		std::cout << "STL Set : Failed" << std::endl;

	std::cout << std::endl;
	std::cout << "Erasing" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		y.erase(i);
	}
	std::cout << "My set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i ) {
		yy.erase(i);
	}
	std::cout << "STL set : " << mTimer.elapsed() * 1000 << " ms" << std::endl;

	std::cout << std::endl;
	std::cout << "now My set size : " << x.size() << std::endl;
	std::cout << "now STL set size : " << xx.size() << std::endl;

	// Test Delegate
	std::cout << std::endl;
	std::cout << "Delegater" << std::endl;
	DSH::delegate<int(int,int)> mydelegate;
	mydelegate = [](int x,int y)->int { return x + y ; } ;
	std::cout << mydelegate(1,2) << std::endl;

	// Test SafePtrMgr 
	std::cout << std::endl;
	std::cout << "SafePtrMgr" << std::endl;
	{   // Block Field
		DSH::SafePtrMgr<int> m_ptr(2);
		m_ptr.get()[0] = 1;
		m_ptr.get()[1] = 2;
		std::cout << m_ptr.get()[0] << " " << m_ptr.get()[1] << std::endl;
		 
		DSH::SafePtrMgr<int> m_ptrone(1);
		*m_ptrone.get() = 1 ;
		std::cout << *m_ptrone.get() << std::endl;
	}

	system("pause");
	return 0;
}