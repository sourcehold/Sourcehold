#pragma once

#include <entt/entt.hpp>

#include "System/Logger.h"

namespace Sourcehold {
    namespace ECS {
        namespace System {
            template<typename ...Selector>
            class BasicSystem {
                public:
                using EachEntityType = std::tuple<entt::entity, Selector...>;

                // TODO - I was getting weird errors when bodies of those methods were
                // defined in .cpp -> maybe figure out later why (probably typing error)

                void run(entt::registry &registry) {
                    this->beforeRun(); // prepare state for update
                    auto view = registry.view<Selector...>();
                    for(EachEntityType entity: view.each()) {
                        this->each(registry, entity);
                    }
                };

                virtual void beforeRun() = 0;
                virtual void each(entt::registry &registry, EachEntityType entity) = 0;
            };
        }
    }
}
