#pragma once

#include <string>
#include <entt/entt.hpp>

#include "System/Logger.h"
#include "ECS/Component/Component.h"
namespace Sourcehold {
namespace ECS {
namespace System {
template <typename... Selector>
class BasicSystem {
 public:
  using EachEntityType = std::tuple<entt::entity, Selector...>;

  void tick(entt::registry &registry) {
    prepareTick();  // prepare state for update
    auto view = registry.view<Selector...>();
    for (EachEntityType entity : view.each()) {
      tickEntity(registry, entity);
    }
  };

  virtual void prepareTick() = 0;
  virtual void tickEntity(entt::registry &registry, EachEntityType &entity) = 0;
};
}  // namespace System
}  // namespace ECS
}  // namespace Sourcehold
