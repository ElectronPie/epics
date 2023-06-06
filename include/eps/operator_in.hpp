#pragma once

#if !(defined __cplusplus) || __cplusplus < 201103L
#error EPICS operator_in.hpp requires C++ standard version 11 or higher to work properly!
#endif

#include <algorithm>

#include "aux_type_traits.hpp"

namespace eps
{
	struct __op_in_helper {};

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
	struct __op_in_lval
	{
		T v;
		__op_in_lval(T&& val): v{std::forward<T>(val)} {}
	};

	template<typename T, typename C>
	auto operator*(__op_in_lval<T>&& l, C&& c) ->
	decltype(operator_in(std::forward<T>(l.v), std::forward<C>(c)))
	{
		return operator_in(std::forward<T>(l.v), std::forward<C>(c));
	}
}

namespace eps
{
	template<typename T, typename C>
	auto operator_in(T&& t, C&& c) ->
	typename std::enable_if<
		std::is_rvalue_reference<decltype(c)>::value ||
		(!std::is_rvalue_reference<decltype(c)>::value &&
		eps::is_const_container<C>::value),
		typename eps::element_of<decltype(c)>::type&&
	>::type
	{
		static typename std::remove_reference<decltype(t)>::type def{};
		auto match = std::find(std::begin(std::forward<C>(c)), std::end(std::forward<C>(c)), std::forward<T>(t));
		if(match == std::end(std::forward<C>(c)))
			return def;
		return std::forward<eps::element_of_t<decltype(c)>(*match);
	}

	template<typename T, typename C>
	auto operator_in(T&& t, C&& c) ->
	typename std::enable_if<
		!std::is_rvalue_reference<decltype(c)>::value &&
		!eps::is_const_container<C>::value,
		typename eps::element_of<decltype(c)>
	>::type
	{
		static typename std::remove_reference<eps::element_of_t<decltype(c)>>::type def{};
		auto match = std::find(std::begin(c), std::end(c), t);
		if(match == std::end(c))
			return def;
		return *match;
	}
}

#define in * eps::__op_in_helper{} *
