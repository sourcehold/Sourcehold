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
                    auto view = registry.view<Selector...>();
                    for(auto entity: view.each()) {
                        this->each(entity);
                    }
                };

                virtual void BasicSystem<Selector...>::each(EachEntityType entity) {
                    Sourcehold::System::Logger::error(Sourcehold::System::ECS) << "Not implemented \"each\"." << std::endl;
                };
            };
        }
    }
}
