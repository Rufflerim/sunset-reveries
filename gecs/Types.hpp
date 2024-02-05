//
// Created by gaetz on 02/07/2023.
//

#ifndef GECSS_TYPES_HPP
#define GECSS_TYPES_HPP

#include <bitset>
using std::bitset;

#include <unordered_map>
using std::unordered_map;

#include "Defines.hpp"
#include "Defines.hpp"

namespace gecs {

    /**
     * Max number of components
     */
    constexpr u32 MAX_COMPONENTS = 32;

    /**
     * Mask for storing archetype id in entity id. Not used yet.
     */
    constexpr u64 LAST_32_BITS = 0b0000000000000000000000000000000011111111111111111111111111111111;

    /**
     * Id used for entitied.
     * Maybe should be used for look back to entities. Let's see.
     */
    using Id = u64;

    /**
     * Archetype id is a MAX_COMPONENT size flags collection, telling if specific component exists.
     */
    using ArchetypeId = bitset<MAX_COMPONENTS>;

    struct Position;
    struct Velocity;
    struct Sprite;

    // This list of components is used to create the column
    // type, with a std::variant.
    #define COMPONENTS Position, Velocity, Sprite

    /**
     * Component identifier
     */
    enum class ComponentId {
        Position = 0,
        Velocity = 1,
        Sprite = 2,
    };

    str ComponentIdToString(ComponentId componentId);
    ComponentId IndexToComponentId(i32 index);
    str ArchetypeIdToString(ArchetypeId id);
    u32 ComponentIdToBitValue(ComponentId componentId);

    /**
     * An Archetype stores a specific set of component vectors,
     * corresponding to an entity of specific composition. Each
     * entity data is stored on the same "row", meaning at the same
     * index for each component vector.
     *
     * For practical purpose, it has an Id and a char array name.
     * Maybe the Id should be discarded.
     */
    class Archetype;

    /**
     * For a specific entity, when its data is stored :
     * - In which archetype
     * - At which row
     */
    struct ArchetypeRecord {
        /// Reference to entity's archetype
        Archetype* archetype;
        /// Entity row in archetypes component table
        u64 row;
    };

    /**
     * Structure to hold Component id, Archetype id
     * and column of the component in the archetype.
     * Is meant to be sorted by component id and
     * archetype to order gathered data for requests.
     */
    struct CompArchIdAndCol {
        ComponentId componentId { ComponentId::Position };
        ArchetypeId archId { 0 };
        size_t columnIndex { 0 };
    };

    /**
     * For an entity, structure to hold entity id,
     * archetype id and entity's row in the archetype.
     * Used to filter and sort requests' gqthered data.
     */
    struct IdArchRow {
        Id id {0};
        ArchetypeId archId;
        size_t row {0};
    };


    /**
     * Convert a templated component class to a component id.
     * @tparam T Component's class
     * @return Corresponding component id
     */
    template <class T>
    ComponentId ToComponentId() {
        if constexpr (std::is_same_v<T, Position>) {
            return ComponentId::Position;
        } else if constexpr (std::is_same_v<T, Velocity>) {
            return ComponentId::Velocity;
        } else if constexpr (std::is_same_v<T, Sprite>) {
            return ComponentId::Sprite;
        }
    }

    /**
     * Convert a variadic template array of component classes
     * to a vector of component ids
     * @tparam ComponentTypes Component classes
     * @return Vector of component ids
     */
    template <typename... ComponentTypes>
    std::vector<ComponentId> ToComponentIds() {
        std::vector<ComponentId> ret;

        // Use fold expression to call ToComponentId for each type
        (ret.push_back(ToComponentId<ComponentTypes>()), ...);

        return ret;
    }

    /**
     * Convert an archetype id to a vector of component ids the archetype holds
     * @param archetypeId`The archetype id we want to know which components it contains
     * @return Vector of component ids
     */
    std::vector<ComponentId> ToComponentIds(ArchetypeId archetypeId);

    /**
     * Convert a variadic template array of component classes
     * to the archetype id that correspond to those components
     * @tparam ComponentTypes Component classes
     * @return Archetype id that exactly holds those components
     */
    template <typename... ComponentTypes>
    ArchetypeId ToArchetypeId() {
        std::vector<ComponentId> comps = ToComponentIds<ComponentTypes...>();
        ArchetypeId id;
        for (auto comp : comps) {
            id.set(static_cast<size_t>(comp));
        }
        return id;
    }

    /**
     * Convert a vector of component ids
     * to the archetype id that correspond to those components
     * @param comps Component ids
     * @return Archetype id that exactly holds those components
     */
    ArchetypeId ToArchetypeId(const std::vector<ComponentId>& comps);
}

#endif //GECSS_TYPES_HPP
