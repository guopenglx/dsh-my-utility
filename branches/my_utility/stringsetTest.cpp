#include "utility.h"
#include "stringset.h"
#include <iostream>
#include <string>
#include <set>
#include <my_utility\timer.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iterator>

const int N = 3000000 ;

std::string getRandom( )
{
	int len( (rand() % 5) + 1);
	std::string ret("");
	for(int i = 0 ; i < len ; ++i ) ret.append(1 , char('a') + (rand() % 26)) ;
	return ret ;
}

int main()
{
	std::srand( std::time(0)) ;
	std::vector<std::string> strvec(N);
	for(int i = 0 ; i < N ; ++i ) 
		strvec[i] = getRandom() ;

	DSH::timer mTimer ;
	DSH::stringset<DSH::TR1::alpha<char,true>> x ;
	std::set<std::string>                      y ;

	std::cout << " N = " << N << std::endl; 

	std::cout << std::endl;
	std::cout << "插入字符串速度测试 : " << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i )
	{
		x.insert( strvec[i].data() );
	}
	std::cout << "My StringSet : " << mTimer.elapsed() << " 秒" << std::endl; 
	mTimer.reset() ;
	for(int i = 0 ; i < N  ;++i )
	{
		y.insert( strvec[i] );
	}
	std::cout << "STL StringSet : " << mTimer.elapsed() << " 秒" << std::endl; 

	std::cout << std::endl;
	std::cout << "查找字符串速度测试 : " << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i )
	{
		x.find( strvec[i].data() );
	}
	std::cout << "My StringSet : " << mTimer.elapsed() << " 秒" << std::endl; 
	mTimer.reset() ;
	for(int i = 0 ; i < N  ;++i )
	{
		y.find( strvec[i] );
	}
	std::cout << "STL StringSet : " << mTimer.elapsed() << " 秒" << std::endl; 

	std::cout << std::endl;
	std::cout << "删除字符串速度测试 : " << std::endl;
	mTimer.reset() ;
	for(int i = 0 ; i < N ; ++i )
	{
		x.erase( strvec[i].data() );
	}
	std::cout << "My StringSet : " << mTimer.elapsed() << " 秒" << std::endl; 
	mTimer.reset() ;
	for(int i = 0 ; i < N  ;++i )
	{
		y.erase( strvec[i] );
	}
	std::cout << "STL StringSet : " << mTimer.elapsed() << " 秒" << std::endl; 
	

	std::cout << std::endl;
	std::cout << "遍历字符串 : " << std::endl;
	std::vector<std::string> outputx ;
	std::vector<std::string> outputy ;

	x.lexicographical_copy( std::back_inserter(outputx) );
	std::copy( y.begin() , y.end() , std::back_inserter(outputy) );

	if( std::equal( outputx.begin() , outputx.end() , y.begin() ) )
		std::cout << "通过" << std::endl;
	else 
		std::cout << "不通过" << std::endl;

	return 0;
}
