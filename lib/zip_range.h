//
// Created by Stephen on 08/05/2019.
//

#ifndef ZIP_ITERATOR_ZIP_RANGE_H
#define ZIP_ITERATOR_ZIP_RANGE_H

#include "zip_iterator.h"

namespace utils {

namespace internal {

template<class ContainerTuple, std::size_t n>
struct BeginIterator {
	using CurrentType = decltype(std::get<n - 1>(std::declval<ContainerTuple>()).cbegin());
	using NextType = typename BeginIterator<ContainerTuple, n - 1>::Ret;
	using Ret = decltype(std::tuple_cat(std::declval<NextType>(), std::declval<std::tuple<CurrentType>>()));

	static Ret begin(ContainerTuple &container) {
		return std::tuple_cat(BeginIterator<ContainerTuple, n - 1>::begin(container),
							  std::make_tuple((std::get<n - 1>(container).cbegin())));
	}
};

template<class ContainerTuple>
struct BeginIterator<ContainerTuple, 1> {
	using Type = decltype(std::get<0>(std::declval<ContainerTuple>()).cbegin());
	using Ret = std::tuple<Type>;

	static Ret begin(ContainerTuple &container) {
		return std::make_tuple(std::get<0>(container).cbegin());
	}
};

template<class ContainerTuple, std::size_t n>
struct EndIterator {
	using CurrentType = decltype(std::get<n - 1>(std::declval<ContainerTuple>()).cend());
	using NextType = typename EndIterator<ContainerTuple, n - 1>::Ret;
	using Ret = decltype(std::tuple_cat(std::declval<NextType>(), std::declval<std::tuple<CurrentType>>()));

	static Ret end(ContainerTuple &container) {
		return std::tuple_cat(EndIterator<ContainerTuple, n - 1>::end(container),
							  std::make_tuple((std::get<n - 1>(container).cend())));
	}
};

template<class ContainerTuple>
struct EndIterator<ContainerTuple, 1> {
	using Type = decltype(std::get<0>(std::declval<ContainerTuple>()).cend());
	using Ret = std::tuple<Type>;

	static Ret end(ContainerTuple &container) {
		return std::make_tuple((std::get<0>(container).cend()));
	}
};

}

template<class ContainerTuple>
class zip_range {

	using IteratorTuple = typename internal::BeginIterator<ContainerTuple, std::tuple_size<ContainerTuple>::value>::Ret;

	utils::zip_iterator<IteratorTuple> m_internal_begin;
	utils::zip_iterator<IteratorTuple> m_internal_end;

public:
	explicit zip_range(ContainerTuple &&containers) {

		m_internal_begin = utils::make_zip_iterator(
				internal::BeginIterator<ContainerTuple, std::tuple_size<ContainerTuple>::value>::begin(containers));
		m_internal_end = utils::make_zip_iterator(
				internal::EndIterator<ContainerTuple, std::tuple_size<ContainerTuple>::value>::end(containers));
	}

	utils::zip_iterator<IteratorTuple> &begin() { return m_internal_begin; }

	utils::zip_iterator<IteratorTuple> &end() { return m_internal_end; }

};

template<class ... ValueContainer>
static zip_range<std::tuple<ValueContainer...>> make_zip_range(ValueContainer &... containers) {
	return zip_range<std::tuple<ValueContainer...>>(std::make_tuple(containers...));
}

}

#endif //ZIP_ITERATOR_ZIP_RANGE_H
