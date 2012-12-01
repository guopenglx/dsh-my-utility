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
#include <cmath>
#include <fstream>

const int N = 3000000 ;
const double eps = 1e-5 ;

//#define _MY_PC

std::string getRandom( )
{
	int len( (rand() % 10) + 1);
	std::string ret("");
	for(int i = 0 ; i < len ; ++i ) ret.append(1 , char('a') + (rand() % 26)) ;
	return ret ;
}

int main()
{
	std::srand( std::time(0)) ;
	std::vector<std::string> strvec(N);

#ifdef _MY_PC

	{
		std::ifstream testfile("F:\\stringset_data\\phone.txt");
		for(int i = 0 ; i < N ; ++i ) {
			std::getline( testfile , strvec[i] );
		}
		testfile.close();
	}
   
#else

  for(int i = 0 ; i < N; ++i )
      strvec[i] = getRandom();
      
#endif

	DSH::timer mTimer ;
	DSH::stringset<DSH::TR1::number<char>>     x ;
	std::set<std::string>                      y ;

	std::cout << "导入手机号码个数 N = " << N << std::endl; 
//	std::cout << "去掉重复后个数 M = " << strvec.size() << std::endl;

	std::cout << std::endl;
	std::cout << "插入 速度测试 : " << std::endl;
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
	std::cout << "查找 速度测试 : " << std::endl;
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
	std::cout << "字典序输出 : " << std::endl;
	std::vector<std::string> outputx ;
	std::vector<std::string> outputy ;

	double x_time , y_time ; 

	mTimer.reset() ;
	x.lexicographical_copy( std::back_inserter(outputx) );
	x_time = mTimer.elapsed() ;

	mTimer.reset() ;
	std::copy( y.begin() , y.end() , std::back_inserter(outputy) );
	y_time = mTimer.elapsed();

	if( !std::equal( outputx.begin() , outputx.end() , y.begin() ) )
		std::cout << "出现Bug ! " << std::endl;
	else {
		x_time = (fabs(x_time) < eps) ? (0) : (x_time) ;
		y_time = (fabs(y_time) < eps) ? (0) : (y_time) ;
		std::cout << "My StringSet : " << x_time << " 秒" << std::endl ;
		std::cout << "STL StringSet : " << y_time << " 秒" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "删除 速度测试 : " << std::endl;
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
	
	return 0;
}
