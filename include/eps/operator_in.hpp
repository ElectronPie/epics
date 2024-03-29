#pragma once

#if !(defined __cplusplus) || __cplusplus < 201103L
#error EPICS operator_in.hpp requires C++ standard version 11 or higher to work properly!
#endif

#include <algorithm>

#include "aux_type_traits.hpp"

namespace eps
{
	template<typename T, typename C>
	auto operator_in(T&& t, C&& c) ->
	std::pair<
		typename eps::element_of<C>::type,
		bool
	>
	{
		/*typename std::remove_reference<
			typename eps::element_of<decltype(c)>::type
		>::type static def;*/
		auto match = std::find(
			std::begin(std::forward<C>(c)),
			std::end  (std::forward<C>(c)),
			std::forward<T>(t)
		);
		return {
			(match == std::end(std::forward<C>(c)))?
				typename eps::element_of<C>::type{}:
				*match,
			match != std::end(std::forward<C>(c))
		};
	}
}

namespace eps
{
	struct __op_in_helper {};

	template<typename T>
	struct __op_in_lval
	{
		T val;
		__op_in_lval(T _val): val{std::forward<T>(_val)} {}
	};

	template<typename T>
	__op_in_lval<const T&> operator*(const T& val, __op_in_helper)
	{ return __op_in_lval<const T&>(val); }

	template<typename T>
	auto operator*(T&& val, __op_in_helper) ->
	typename std::enable_if<
		!std::is_const<
			typename std::remove_reference<T>::type
		>::value &&
		std::is_rvalue_reference<T>::value,
		__op_in_lval<T&&>
	>::type
	{ return __op_in_lval<T&&>(std::move(val)); }

	template<typename T, typename C>
	auto operator*(__op_in_lval<T> l, C&& c) ->
	decltype(operator_in(std::forward<T>(l.val), std::forward<C>(c)))
	{ return operator_in(std::forward<T>(l.val), std::forward<C>(c)); }
}

#define in * eps::__op_in_helper{} *
