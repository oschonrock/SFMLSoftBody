#pragma once
#include <algorithm>
#include <exception>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <vector>

template <typename InputIter, typename Comp = std::ranges::less, typename Proj = std::identity>
requires std::input_iterator<InputIter> &&
    std::sortable<typename std::vector<std::iter_value_t<InputIter>>::iterator, Comp, Proj>
auto median(InputIter first, InputIter last, Comp comp = {}, Proj proj = {}) {

  std::vector<std::iter_value_t<InputIter>> tmp(first, last); // always make a copy

  if (tmp.empty()) throw std::domain_error("Can't find median of an empty range.");

  auto tmp_middle = tmp.begin() + (tmp.size() / 2);

  std::ranges::nth_element(tmp.begin(), tmp_middle, tmp.end(), comp, proj);

  if (tmp.size() % 2 == 1) return std::invoke(proj, *tmp_middle);

  auto below_tmp_middle = std::ranges::max_element(tmp.begin(), tmp_middle, comp, proj);

  return std::midpoint(std::invoke(proj, *tmp_middle), std::invoke(proj, *below_tmp_middle));
}

template <typename InputRange, typename Comp = std::less<>, typename Proj = std::identity>
requires std::ranges::input_range<InputRange> &&
    std::sortable<typename std::vector<std::ranges::range_value_t<InputRange>>::iterator, Comp,
                  Proj>
auto median(InputRange range, Comp comp = {}, Proj proj = {}) {
  return median(range.begin(), range.end(), comp, proj);
}
