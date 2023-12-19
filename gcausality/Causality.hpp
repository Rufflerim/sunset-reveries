//
// Created by gaetz on 16/09/2023.
//

#ifndef GCAUSALITY_CAUSALITY_HPP
#define GCAUSALITY_CAUSALITY_HPP

#include "Defines.hpp"
#include "Law.hpp"
#include "WorldStateManager.hpp"

namespace gcausality {

    class Causality {
    public:
        Causality() = default;

        void AddLaw(const str& archetypeName, i32 order);
        WorldState Tick(f32 dt);

        static Causality& Instance() {
            static Causality instance;
            return instance;
        }

    private:
        vector<Law> laws;
        WorldStateManager stateManager;
    };

}

// Reflexion sur le temps, qui peut être seulement le calcul d'un état à un autre

#endif //GCAUSALITY_CAUSALITY_HPP
