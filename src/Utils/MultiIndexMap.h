#pragma once

#include <unordered_map>

namespace Sourcehold {
namespace Utils {
template <typename Key, typename Value,
          template <typename, typename, typename...> typename Aggregator =
              std::unordered_map>
class MultiIndexMap {
 public:
  using TypeOfMap = Aggregator<Key, Value>;
  using Arg = Aggregator<const std::vector<Key>, Value>;
  using ArgReversed = Aggregator<Value, const std::vector<Key>>;

  TypeOfMap outMap;

  MultiIndexMap(const Arg config) {
    for (auto mapEntry : config) {
      auto [keys, value] = mapEntry;
      for (auto key : keys) {
        insert_or_assign(outMap, key, value);
      }
    }
  }

  MultiIndexMap(const ArgReversed config) {
    // I like using this "reversed" option, because it feels more json-like in
    // code Maybe I should do that in the functional way, but it is not a
    // priority now
    for (auto mapEntry : config) {
      auto [value, keys] = mapEntry;
      for (auto key : keys) {
        insert_or_assign(outMap, key, value);
      }
    }
  }

  constexpr inline void insert_or_assign(TypeOfMap &out, Key key, Value value) {
    out.insert_or_assign(key, value);
  }
};

template <typename Key, typename Value,
          typename IntermediateType = MultiIndexMap<Key, Value>>
constexpr typename IntermediateType::TypeOfMap createMultiIndexMap(
    const typename IntermediateType::Arg Config) {
  return IntermediateType(Config).outMap;
}

template <typename Key, typename Value,
          typename IntermediateType = MultiIndexMap<Key, Value>>
constexpr typename IntermediateType::TypeOfMap createMultiIndexMap(
    const typename IntermediateType::ArgReversed Config) {
  return IntermediateType(Config).outMap;
}
}  // namespace Utils
}  // namespace Sourcehold
