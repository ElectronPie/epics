#pragma once

#if !(defined __cplusplus) || __cplusplus < 201103L
#error EPICS operator_in.hpp requires C++ standard version 11 or higher to work properly!
#endif

#include <algorithm>

namespace eps
{
	class __op_in_helper {};

	template<typename T>
	struct __op_in_lval;

	template<typename T>
	auto operator*(T&& val, __op_in_helper&&) ->
	typename std::enable_if<
		std::is_rvalue_reference<decltype(val)>::value,
		__op_in_lval<T&&>
	>::type
	{ return __op_in_lval<T&&>{std::forward<T>(val)}; }

	template<typename T>
	auto operator*(T&& val, __op_in_helper&&) ->
	typename std::enable_if<
		!std::is_rvalue_reference<decltype(val)>::value,
		__op_in_lval<const T&>
	>::type
	{ return __op_in_lval<const T&>{std::forward<T>(val)}; }

	template<typename T>
	class __op_in_lval
	{
	public:
		T v;
		__op_in_lval(T&& val): v{std::forward<T>(val)} {}
	};

	template<typename T, typename C>
	auto operator*(__op_in_lval<T>&& l, C&& c) ->
	typename std::enable_if<
		std::is_rvalue_reference<decltype(c)>::value,
		const typename std::remove_reference<T>::type&
	>::type
	{
		static typename std::remove_reference<T>::type t{};
		auto match = std::find(std::begin(c), std::end(c), std::forward<T>(l.v));
		if(match == std::end(c))
			return t;
		return *match;
	}

	template<typename T, typename C>
	auto operator*(__op_in_lval<T>&& l, C&& c) ->
	typename std::enable_if<
		!std::is_rvalue_reference<decltype(c)>::value &&
		!std::is_const<
			typename std::remove_pointer<
				typename std::iterator_traits<
					decltype(std::begin(std::declval<C&>()))
				>::pointer
			>::type
		>::value,
		T
	>::type
	{
		static typename std::remove_reference<T>::type t{};
		auto match = std::find(std::begin(c), std::end(c), std::forward<T>(l.v));
		if(match == std::end(c))
			return std::forward<T>(t);
		return std::forward<T>(*match);
	}

	template<typename T, typename C>
	auto operator*(__op_in_lval<T>&& l, C&& c) ->
	typename std::enable_if<
		!std::is_rvalue_reference<decltype(c)>::value &&
		std::is_const<
			typename std::remove_pointer<
				typename std::iterator_traits<
					decltype(std::begin(std::declval<C&>()))
				>::pointer
			>::type
		>::value,
		const typename std::remove_reference<T>::type&
	>::type
	{
		static const typename std::remove_reference<T>::type t;
		auto match = std::find(std::begin(c), std::end(c), std::forward<T>(l.v));
		if(match == std::end(c))
			return t;
		return *match;
	}
}

#define in * eps::__op_in_helper{} *
