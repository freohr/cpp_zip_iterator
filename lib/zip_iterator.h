//
// Created by guest on 02/05/19.
//

#ifndef ZIP_ITERATOR_ZIP_ITERATOR_H
#define ZIP_ITERATOR_ZIP_ITERATOR_H

#include <cstddef>
#include <tuple>
#include <functional>

namespace utils {
namespace internal {
template<class Tuple, std::size_t n>
struct Dereference {

	using CurrentVal = decltype(*(std::get<n - 1>(std::declval<Tuple>())));
	using NextRet = typename Dereference<Tuple, n - 1>::Ret;
	using Ret = decltype(std::tuple_cat(std::declval<NextRet>(), std::declval<std::tuple<CurrentVal>>()));

	static Ret dereference(Tuple &t) {
		return std::tuple_cat(Dereference<Tuple, n - 1>::dereference(t),
							  std::make_tuple<std::reference_wrapper<typename std::remove_reference<CurrentVal>::type>>(
									  *(std::get<n - 1>(t))));
	}
};

template<class Tuple>
struct Dereference<Tuple, 1> {

	using Val = decltype(*(std::get<0>(std::declval<Tuple>())));
	using Ret = std::tuple<Val>;

	static Ret dereference(Tuple &t) {
		return std::make_tuple<std::reference_wrapper<typename std::remove_reference<Val>::type>>(
				*(std::get<0>(t)));
	}
};

template<class Tuple, std::size_t n>
struct Increment {
	static void increment(Tuple &t) {
		Increment<Tuple, n - 1>::increment(t);
		++(std::get<n - 1>(t));
	}
};

template<class Tuple>
struct Increment<Tuple, 1> {
	static void increment(Tuple &t) {
		++(std::get<0>(t));
	}
};
}

template<typename IteratorTuple>
class zip_iterator {
public:
	zip_iterator() = default;

	explicit zip_iterator(const IteratorTuple &iteratorTuple) : m_iterator_tuple(iteratorTuple) {}

	explicit zip_iterator(IteratorTuple &&iteratorTuple) : m_iterator_tuple(iteratorTuple) {}

	typename internal::Dereference<
			IteratorTuple,
			std::tuple_size<IteratorTuple>::value>::Ret operator*() {
		return internal::Dereference<
				IteratorTuple,
				std::tuple_size<IteratorTuple>::value>
		::dereference(m_iterator_tuple);
	}

	zip_iterator &operator++() {
		internal::Increment<
				IteratorTuple,
				std::tuple_size<IteratorTuple>::value>
		::increment(m_iterator_tuple);

		return *this;
	}

	zip_iterator operator++(int) {
		auto copy = *this;
		++this;

		return copy;
	}

	bool operator==(const zip_iterator<IteratorTuple> &other) {
		return m_iterator_tuple == other.m_iterator_tuple;
	}

	bool operator!=(const zip_iterator<IteratorTuple> &other) {
		return m_iterator_tuple != other.m_iterator_tuple;
	}

private:
	IteratorTuple m_iterator_tuple;
};

template<class ... Iterators>
static zip_iterator<std::tuple<Iterators...>> make_zip_iterator(Iterators &&... iterators) {
	return zip_iterator<std::tuple<Iterators...>>(std::make_tuple(iterators...));
}

template<class IteratorTuple>
static zip_iterator<IteratorTuple> make_zip_iterator(IteratorTuple&& tuple) {
	return zip_iterator<IteratorTuple>(tuple);
}
}


#endif //ZIP_ITERATOR_ZIP_ITERATOR_H
