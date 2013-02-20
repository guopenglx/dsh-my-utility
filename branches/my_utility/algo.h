#pragma once

#include <utility>

/************************************************************************
*                                                                       *
*   Author : DSH                                                        *
*   Lang   : C++                                                        *
*   Date   : 2013 / 2 / 20                                              *
*                                                                       *
*                                                                       *
*   CopyRight @ 董诗浩 ( DongShiHao ) / 理学院数学系                    *
*               in East China University Of Science and Techinology     *
*                                                                       *
*   # Functional Algorithm Based on Range                               *
*                                                                       *
*                                                                       *
*************************************************************************/

#define DSH_GEN_OPERATOR_OR( rhs_type ) \
template<typename Container , typename OneFunc> \
auto operator | (const Container& c , rhs_type<OneFunc> f) -> Container  \
{  \
	return f | c;  \
}

#define DSH_GEN_OPERATOR_OR2( rhs_type ) \
template<typename Container , typename OneFunc , typename TwoFunc> \
auto operator | (const Container& c, rhs_type<OneFunc,TwoFunc> f) -> Container \
{ \
	return f | c ; \
}

namespace _DSH {
	template<typename UnaryFunc>
	class transformed_holder{
	public:
		explicit transformed_holder( UnaryFunc&& _f )
			:f(_f) { }

		template<typename Container>
		Container operator | (const Container &c) {
			Container ret(c) ;

			auto _First( ret.begin() );
			auto _Last( ret.end() );

			for(auto i(_First) ; i != _Last ; ++i ) {
				auto tmp = std::move( f( std::move( *i ) ) );
				*i = std::move(tmp);
			}

			return ret ;
		}

	private:
		UnaryFunc f ;
	};

	template<typename UnaryFunc>
	class reversed_holder{
	public:
		template<typename Container>
		Container operator | (const Container &c) {
			Container ret(c) ;

			auto _First( ret.begin() );
			auto _Last( ret.end() );

			for (; _First != _Last && _First != --_Last; ++_First) {
				std::iter_swap(_First, _Last);
			}

			return ret ;
		}
	};

	template<typename Type>
	class replace_holder{
	public:
		explicit replace_holder(const Type& lhs , const Type& rhs)
			: oldValue(lhs) , newValue(rhs)  { }

		template<typename Container>
		Container operator | (const Container& c) {
			Container ret(c);

			auto _First( ret.begin() );
			auto _Last( ret.end() );

			for(auto i(_First) ; i != _Last ; ++i ) {
				if( *i == oldValue )
					*i = newValue ;
			}

			return ret ;
		}
	private:
		Type oldValue , newValue ;
	};

	template<typename UnaryFunc>
	class called_holder1 {
	public:
		explicit called_holder1( UnaryFunc&& _f )
			: f(_f) { } 

		template<typename Container>
		Container operator | (const Container &c) {
		
			auto _First( c.begin() );
			auto _Last ( c.end() );

			for(auto i(_First) ; i != _Last ; ++i ){
				f(*i);
			}

			return c ;
		}

	private:
		UnaryFunc f;
	};

	template<typename UnaryFunc,typename EndFunc>
	class called_holder2 {
	public:
		explicit called_holder2( UnaryFunc&& _f , EndFunc&& _endf )
			: f(_f) , endf(_endf) { } 

		template<typename Container>
		Container operator | (const Container &c) {
		
			auto _First( c.begin() );
			auto _Last ( c.end() );

			for(auto i(_First) ; i != _Last ; ++i ){
				f(*i);
			}

			endf();
			return c ;
		}
	private:
		UnaryFunc f;
		EndFunc   endf ;
	};
}

namespace DSH {
	// transformed
	template<typename UnaryFunc>
	inline auto transformed( UnaryFunc&& _f ) -> _DSH::transformed_holder<UnaryFunc>
	{
		return _DSH::transformed_holder<UnaryFunc>(std::forward<UnaryFunc>(_f)) ;
	}

	// called : One Function
	template<typename UnaryFunc>
	inline auto called( UnaryFunc&& _f ) -> _DSH::called_holder1<UnaryFunc>
	{
		return _DSH::called_holder1<UnaryFunc>(std::forward<UnaryFunc>(_f)) ;
	}

	// called : Two Function
	template<typename UnaryFunc , typename EndFunc>
	inline auto called( UnaryFunc&& _f , EndFunc&& _endf) -> _DSH::called_holder2<UnaryFunc,EndFunc>
	{
		return _DSH::called_holder2<UnaryFunc,EndFunc>(std::forward<UnaryFunc>(_f),
			std::forward<EndFunc>(_endf)); ;
	}

	// replaced
	template<typename Type>
	inline auto replaced(const Type& lhs , const Type& rhs) -> _DSH::replace_holder<Type>
	{
		return _DSH::replace_holder<Type>(lhs , rhs);
	}

	// reversed
	const _DSH::reversed_holder<char> reversed = _DSH::reversed_holder<char>();
}

// Add operator | interface here
DSH_GEN_OPERATOR_OR(_DSH::transformed_holder)
DSH_GEN_OPERATOR_OR(_DSH::reversed_holder)
DSH_GEN_OPERATOR_OR(_DSH::replace_holder)
DSH_GEN_OPERATOR_OR(_DSH::called_holder1)
DSH_GEN_OPERATOR_OR2(_DSH::called_holder2)