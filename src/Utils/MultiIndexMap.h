#pragma once

#include <unordered_map>
#include <type_traits>

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
            using Arg = Aggregator<std::vector<Key>, Value>;
            using ArgReversed = Aggregator<Value, std::vector<Key>>;

            TypeOfMap outMap;

            MultiIndexMap(Arg config) {
                for (auto map : config) {
                    auto [entityTypes, assetEnum] = map;
                    for (auto entityType : entityTypes) {
                        outMap.insert_or_assign(entityType, assetEnum);
                    }
                }
            }
            MultiIndexMap(ArgReversed config) {
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
        typename IntermediateType::TypeOfMap createMultiIndexMap(typename IntermediateType::Arg Config) {
            return IntermediateType(Config).outMap;
        }
        
        template<
            typename Key,
            typename Value,
            typename IntermediateType = MultiIndexMap<Key, Value>
        >
        typename IntermediateType::TypeOfMap createMultiIndexMap(typename IntermediateType::ArgReversed Config) {
            return IntermediateType(Config).outMap;
        }
    }
}
