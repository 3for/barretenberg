#pragma once

#include "../../../types.hpp"

#include "../../reference_string/reference_string.hpp"
#include "../../waffle_types.hpp"

#include "../widgets/base_widget.hpp"

#include "../../../transcript/manifest.hpp"
#include "../../../transcript/transcript.hpp"
namespace waffle
{
class Verifier
{
  public:
    Verifier(const size_t subgroup_size = 0, const transcript::Manifest &manifest = transcript::Manifest({}), bool has_fourth_wire = false);
    Verifier(Verifier&& other);
    Verifier(const Verifier& other) = delete;
    Verifier& operator=(const Verifier& other) = delete;
    Verifier& operator=(Verifier&& other);

    bool verify_proof(const waffle::plonk_proof& proof);

    ReferenceString reference_string;

    barretenberg::g1::affine_element SIGMA_1;
    barretenberg::g1::affine_element SIGMA_2;
    barretenberg::g1::affine_element SIGMA_3;

    std::vector<std::unique_ptr<VerifierBaseWidget>> verifier_widgets;
    size_t n;

    transcript::Manifest manifest;

    bool __DEBUG_HAS_FOURTH_WIRE;
};
} // namespace waffle