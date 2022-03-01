#pragma once

#include <algorithm>
#include <exception>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <vector>

template <typename RandomAccessIter, typename Comp = std::ranges::less,
          typename Proj = std::identity>
requires std::random_access_iterator<RandomAccessIter> &&
    std::sortable<RandomAccessIter, Comp, Proj>
auto median_in_place(RandomAccessIter first, RandomAccessIter last, const Comp& comp = {},
                     const Proj& proj = {}) {

  auto size = std::distance(first, last);
  if (size == 0) throw std::domain_error("Can't find median of an empty range.");

  auto middle = first + (size / 2);

  std::ranges::nth_element(first, middle, last, comp, proj);

  if (size % 2 == 1) return std::invoke(proj, *middle);

  auto below_middle = std::ranges::max_element(first, middle, comp, proj);

  using std::midpoint; // allow users to provide their own midpoint
  return midpoint(std::invoke(proj, *below_middle), std::invoke(proj, *middle));
}

template <typename RandomAcccessRange, typename Comp = std::less<>, typename Proj = std::identity>
requires std::ranges::random_access_range<RandomAcccessRange> &&
    std::sortable<std::ranges::iterator_t<RandomAcccessRange>, Comp, Proj>
auto median_in_place(RandomAcccessRange& range, const Comp& comp = {}, const Proj& proj = {}) {
  return median_in_place(std::ranges::begin(range), std::ranges::end(range), comp, proj);
}

template <typename InputIter, typename Comp = std::ranges::less, typename Proj = std::identity>
requires std::input_iterator<InputIter> &&
    std::sortable<typename std::vector<std::iter_value_t<InputIter>>::iterator, Comp, Proj>
auto median(const InputIter first, const InputIter last, const Comp& comp = {}, const Proj& proj = {}) {
  std::vector<std::iter_value_t<InputIter>> input_copy(first, last); // always make a copy
  return median_in_place(input_copy, comp, proj);
}

template <typename InputRange, typename Comp = std::less<>, typename Proj = std::identity>
requires std::ranges::input_range<InputRange> &&
    std::sortable<typename std::vector<std::ranges::range_value_t<InputRange>>::iterator, Comp,
                  Proj>
auto median(const InputRange& range, const Comp& comp = {}, const Proj& proj = {}) {
  return median(std::ranges::begin(range), std::ranges::end(range), comp, proj);
}
