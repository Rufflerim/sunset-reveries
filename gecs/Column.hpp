//
// Created by Gaëtan Blaise-Cazalet on 22/01/2024.
//

#ifndef GECS_COLUMN_HPP
#define GECS_COLUMN_HPP

#include "Types.hpp"
#include "Components.hpp"
namespace gecs {

    struct Position;
    struct Velocity;
    struct Sprite;

    /**
     * A column is a vector of a specific component type.
     * It is used to store all the data of a specific component
     * for all the entities of a specific archetype.
     */
    class Column {
    private:
        /**
         * Size of the data type
         */
        u32 dataSize;

        /**
         * Stored component id
         */
        ComponentId componentId;

        /**
         * Data vector
         */
        vector<std::variant<COMPONENTS>> data;

    public:
        /**
         * Init the column and reserve memory for a specific number of elements.
         * @tparam T Type of component
         * @param numberOfElements Multiplier for data to reserve
         */
        template<class T>
        void Init(size_t numberOfElements = 100) {
            data.reserve(numberOfElements);
            dataSize = sizeof(T);
            componentId = ToComponentId<T>();
        }

        /**
         * Get a row from the data vector.
         * @tparam T Component typeent
         * @param row Row, corresponding to an entity
         * @return Entity's component data reference
         */
        template<class T>
        T& GetRow(size_t row) {
            return std::get<T>(data[row]);
        }

        /**
         * Get a row from the data vector, no modification.
         * @tparam T Component type
         * @param row Row, corresponding to an entity
         * @return Entity's component data const reference
         */
        template<class T>
        const T& GetRowConst(size_t row) const {
            return std::get<T>(data[row]);
        }

        /**
         * Replace the data vector with a new one.
         * @tparam T Component type
         * @param newData New data vector
         */
        template<class T>
        void ReplaceData(const vector<T>&& newData) {
            GASSERT_DEBUG(data.size() == newData.size())
            data.assign(newData.begin(), newData.end());
        }

        /**
         * Return the number of elements in the data vector.
         * @return Column size
         */
        [[nodiscard]] size_t Count() const  { return data.size(); }

        [[nodiscard]] ComponentId GetComponentId() const { return componentId; }


        /**
         * Add a new element to the data vector.
         * @tparam T Component type
         * @param element New element
         * @return New element's row
         */
        template<class T>
        u64 AddElement(T element) {
            data.push_back(std::move(element));
            return data.size() - 1;
        };

        /**
         * Remove an element from the data vector,
         * by swapping it with the last element then popping it.
         * This should e the onlyway to remove an element from
         * the data vector.
         * @param row Row to remove
         */
        void RemoveElementBySwapping(u64 row) {
            auto last = data.end() - 1;
            auto removedElement = data.begin() + static_cast<i64>(row);
            std::iter_swap(removedElement, last);
            data.pop_back();
        };

        /* Functions used for logging purpose */

        [[nodiscard]] const Position& GetPos(size_t row) const;
        [[nodiscard]] const Velocity& GetVelocity(size_t row) const;
        [[nodiscard]] const Sprite& GetSprite(size_t row) const;
    };

} // gecs

#endif //GAEMI_00_COLUMN_HPP
