#pragma once

#include <unordered_map>

namespace Sourcehold {
    namespace Utils {
        template<
            typename Key,
            typename Value,
            template <typename, typename, typename...> typename Aggregator = std::unordered_map
        >
        class MultiIndexMap {
            public:
            using TypeOfMap = Aggregator<Key, Value>;
            using Arg = Aggregator<const std::vector<Key>, Value>;
            using ArgReversed = Aggregator<Value, const std::vector<Key>>;

            TypeOfMap outMap;

            MultiIndexMap(const Arg config) {
                for (auto map : config) {
                    auto [entityTypes, assetEnum] = map;
                    for (auto entityType : entityTypes) {
                        outMap.insert_or_assign(entityType, assetEnum);
                    }
                }
            }
            MultiIndexMap(const ArgReversed config) {
                // I like using this "reversed" option, because it feels more json-like in code
                // Maybe I should do that in the functional way, but it is not a priority now
                for (auto map : config) {
                    auto [assetEnum, entityTypes] = map;
                    for (auto entityType : entityTypes) {
                        outMap.insert_or_assign(entityType, assetEnum);
                    }
                }
            }
        };

        template<
            typename Key,
            typename Value,
            typename IntermediateType = MultiIndexMap<Key, Value>
        >
        constexpr typename IntermediateType::TypeOfMap createMultiIndexMap(const typename IntermediateType::Arg Config) {
            return IntermediateType(Config).outMap;
        }
        
        template<
            typename Key,
            typename Value,
            typename IntermediateType = MultiIndexMap<Key, Value>
        >
        constexpr typename IntermediateType::TypeOfMap createMultiIndexMap(const typename IntermediateType::ArgReversed Config) {
            return IntermediateType(Config).outMap;
        }
    }
}
