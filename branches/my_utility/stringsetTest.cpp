#include <my_utility\stringset.h>
#include <my_utility\utility.h>
#include <my_utility\timer.h>
#include <my_utility\set.h>

#include <iostream>
#include <set>
#include <string>
#include <random> 

const int N = 2000000 ;
const int minLen = 3;
const int maxLen = 5;

template<typename charHash>
class RandomString {
public:
	RandomString( std::size_t _min , std::size_t _max  )
		: bMin(_min) , bMax( _max ) , hasher(),rd(),gen(rd()),strLenGen(bMin,bMax)
		,chrValGen( 1 , hasher.max_value() )  // 1 to 26
	{ }
public:
	inline std::string getString() 
	{
		std::size_t strLen(strLenGen(gen));
		std::string result(strLen , ' ') ;

		for(std::size_t i = 0 ; i != strLen ; ++i )
			result.at(i) = 'a' + (chrValGen(gen) - 1) ;

		return std::move( result );
	}
private:
	std::size_t bMin , bMax ;
	charHash    hasher ;

	std::random_device rd ;
	std::mt19937 gen ;
	std::uniform_int_distribution<> strLenGen ;
	std::uniform_int_distribution<> chrValGen ;
};

int main()
{
	// 测试的两种容器
	DSH::stringset<DSH::TR1::alpha<char,true>> x ;
	std::set<std::basic_string<char>>          y ;

	// 测试辅助工具
	DSH::timer mTimer ;
	std::vector<std::basic_string<char>> strArray( N ) ;

	// 生成随机字符串
	RandomString<DSH::TR1::alpha<char,true>> randStr(minLen , maxLen) ;
	for(int i = 0 ; i < N ; ++i )
		strArray.at(i) = std::move(randStr.getString());

	////////////////////////////////////////////////////////
	//   测试开始
	////////////////////////////////////////////////////////  

	std::cout << "N = " << N << std::endl;

	// 插入测试
	std::cout << std::endl;
	std::cout << "Insert : " << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) 
		x.insert( strArray[i] );
	std::cout << "My String Set  : " << mTimer.elapsed() << " s" << std::endl;

	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) 
		y.insert( strArray[i] );
	std::cout << "STL String Set : " << mTimer.elapsed() << " s" << std::endl;

	// 查询测试
	std::cout << std::endl;
	std::cout << "Search : " << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) 
		x.find( strArray[i] );
	std::cout << "My String Set  : " << mTimer.elapsed() << " s" << std::endl;

	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) 
		y.find( strArray[i] );
	std::cout << "STL String Set : " << mTimer.elapsed() << " s" << std::endl;

	// 删除测试
	std::cout << std::endl;
	std::cout << "Delete : " << std::endl;
	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) 
		x.erase( strArray[i] );
	std::cout << "My String Set  : " << mTimer.elapsed() << " s" << std::endl;

	mTimer.reset();
	for(int i = 0 ; i < N ; ++i ) 
		y.erase( strArray[i] );
	std::cout << "STL String Set : " << mTimer.elapsed() << " s" << std::endl;

	return 0;
} 