//
// Created by gaetz on 07/01/24.
//

#include "QueryManager.hpp"
#include "World.hpp"

namespace gecs {

    void QueryManager::Store(gecs::ArchetypeId key, void* q) {
        /// TODO: what about deleting this pointer?
        queries.emplace(key, q);
    }

    bool QueryManager::HasQuery(ArchetypeId archId) {
        return queries.contains(archId);
    }

    using ComponentArchetypes = unordered_map<ArchetypeId, size_t>;

    vector<IdArchRow> QueryManager::GetEntitiesWithArchsRows() {
        vector<IdArchRow> entitiesWithArchRows;
        const auto& entities = World::Instance().GetEntities();
        entitiesWithArchRows.reserve(entities.size());
        for (const auto &entity: entities) {
            entitiesWithArchRows.push_back(
                    IdArchRow{entity.first, entity.second.archetype->archetypeId, entity.second.row});
        }
        return entitiesWithArchRows;
    }

    vector<CompArchIdAndCol> QueryManager::GetComponentsWithArchsCols(vector<ComponentId>&& componentIds, ArchetypeId pattern) {
        vector<CompArchIdAndCol> res;
        World& world = World::Instance();
        const auto& archetypes = world.GetArchetypes();
        const auto& components = world.GetComponents();
        for (const auto& c : components) {
            if (std::find(componentIds.begin(), componentIds.end(), c.first) == componentIds.end()) continue;
            ComponentArchetypes archs = c.second;
            for (const auto archIds : archs) {
                // We check the archetype contain the minimal components
                // We also exclude empty archetypes
                if ((archIds.first & pattern) == pattern && archetypes.at(archIds.first).GetRowCount() > 0) {
                    res.push_back(CompArchIdAndCol { c.first, archIds.first, archIds.second });
                }
            }
        }

        std::sort(res.begin(), res.end(), [=](CompArchIdAndCol a, CompArchIdAndCol b) {
            return a.componentId < b.componentId ||
                   (a.componentId == b.componentId && a.archId.to_ulong() < b.archId.to_ulong());
        });

        return res;
    }

    Column& QueryManager::GetColumn(ArchetypeId archId, size_t column) {
        return (World::Instance().GetArchetypes())[archId].components[column];
    }

    vector<CompArchIdAndCol> QueryManager::FilterForOneComponent(const vector<CompArchIdAndCol>& filterMaterial, ComponentId componentId) {
        vector<CompArchIdAndCol> compFilter;
        for (const auto& compArchCol: filterMaterial) {
            if (compArchCol.componentId == componentId) {
                compFilter.emplace_back(compArchCol);
            }
        }
        return compFilter;
    }

    void QueryManager::DestroyEntities(const vector<Id> &toDelete) {
        World& world = World::Instance();
        for(Id id : toDelete) {
            world.DestroyEntity(id);
        }
    }

    unordered_map<ArchetypeId, Archetype> &QueryManager::GetWorldArchetypes() {
        return World::Instance().GetArchetypes();
    }

    vector<vector<std::pair<ArchetypeId, size_t>>> QueryManager::GetArchetypeAndColumnIndices(const vector<CompArchIdAndCol> &compArchCols) {
        vector<vector<std::pair<ArchetypeId, size_t>>> ret;
        ComponentId currentCompId = compArchCols[0].componentId;
        vector<std::pair<ArchetypeId, size_t>> currentArchsAndCols { {compArchCols[0].archId, compArchCols[0].columnIndex} };
        for (u32 i = 1; i < compArchCols.size(); ++i) {
            if (currentCompId != compArchCols[i].componentId) {
                ret.emplace_back(currentArchsAndCols);
                currentArchsAndCols.clear();
                currentArchsAndCols.emplace_back(compArchCols[i].archId, compArchCols[i].columnIndex);
                currentCompId = compArchCols[i].componentId;
                continue;
            }
            currentArchsAndCols.emplace_back(compArchCols[i].archId, compArchCols[i].columnIndex);
        }
        ret.emplace_back(currentArchsAndCols);
        return ret;
    }

    vector<vector<size_t>> QueryManager::GetDataStartIndices(const vector<CompArchIdAndCol> &compArchCols) {
        const auto& archetypeRegistry = World::Instance().GetArchetypesConst();
        ComponentId currentCompId = compArchCols[0].componentId;
        vector<vector<size_t>> starts {};
        vector<size_t> currentStarts { 0 };
        size_t accumulator { 0 };
        // We create a vector of start indices for each component.
        // We count the number of elements for the current archetype
        // and add it to the accumulator, so that we can know the start index
        // for a specific archetype for the same component.
        for (u32 i = 1; i < compArchCols.size(); ++i) {
            if (currentCompId != compArchCols[i].componentId) {
                starts.emplace_back(currentStarts);
                currentStarts.clear();
                currentStarts.push_back(0);
                accumulator = 0;
                currentCompId = compArchCols[i].componentId;
                continue;
            }
            const auto count = archetypeRegistry.at(compArchCols[i].archId).components[compArchCols[i].columnIndex].Count();
            accumulator += count;
            currentStarts.push_back(accumulator);
        }
        starts.emplace_back(currentStarts);
        return starts;
    }

    void QueryManager::RemoveComponentFromWorld(Id EntityId, ComponentId componentId) {
        World::Instance().RemoveComponent(EntityId, componentId);
    }
}