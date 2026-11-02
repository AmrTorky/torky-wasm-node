#ifndef TORKY_WASM_EDGE_NODE_HPP
#define TORKY_WASM_EDGE_NODE_HPP

#include <cmath>
#include <cstdint>
#include <algorithm>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

namespace torky::wasm::node {

    inline constexpr double L_A_NM            = 0.36787944117144233; // e^-1 nm
    inline constexpr double EPSILON_NM2       = 0.016;
    inline constexpr double V_FINITE_ZERO_EV  = 11.384;

    #pragma pack(push, 1)
    struct TorkyCellDtype {
        uint8_t  gov_gate;     // TSG Gate Register (0x01 - 0x04)
        int16_t  phase_q16;    // Phase Q16 Fixed-Point
        int16_t  field_q16;    // Field Q16 Fixed-Point
        uint32_t syndrome_u32; // SHA-256 Merkle Syndrome
    };
    #pragma pack(pop)

    static_assert(sizeof(TorkyCellDtype) == 9, "CRITICAL: TorkyCellDtype must be precisely 9 bytes!");

    class WasmEdgeNodeEngine {
    public:
        // In-browser continuum calculation routine with zero NaN loss
        static inline double evaluate_edge_cell(double r_nm, uint8_t gate_signal) {
            if (gate_signal < 0x01 || gate_signal > 0x04) return 0.0;
            double potential = 1.0 / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
            return std::isnan(potential) ? V_FINITE_ZERO_EV : potential;
        }

        static inline double evaluate_v_finite(double r_nm) {
            return 1.0 / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
        }
    };

    class TsgGateController {
    public:
        static inline bool validate_authorization(uint8_t gate_request, bool bit_tampered) {
            if (bit_tampered) return false;
            return (gate_request >= 0x01 && gate_request <= 0x04);
        }
    };

} // namespace torky::wasm::node

#ifdef __EMSCRIPTEN__
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    double torky_wasm_evaluate(double r_nm, uint8_t gate) {
        return torky::wasm::node::WasmEdgeNodeEngine::evaluate_edge_cell(r_nm, gate);
    }
}
#endif

#endif // TORKY_WASM_EDGE_NODE_HPP
