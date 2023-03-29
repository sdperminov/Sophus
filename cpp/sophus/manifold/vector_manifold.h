// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "sophus/common/common.h"
#include "sophus/concepts/manifold.h"
#include "sophus/linalg/vector_space.h"

namespace sophus {

template <class TScalar, int kDim>
struct VectorManifold {
  using Scalar = TScalar;
  static int constexpr kDof = kDim;
  static int constexpr kNumParams = kDim;

  using Params = Eigen::Vector<TScalar, kDim>;
  using Tangent = Params;

  VectorManifold() { v.setZero(); }

  VectorManifold(Params const& v) : v(v) {}

  auto oplus(Tangent const& a) const -> VectorManifold {
    return VectorManifold(this->v + a);
  }

  auto ominus(VectorManifold const& other) const -> Tangent {
    return this->v - other.params();
  }

  static auto fromParams(Params const& params) -> VectorManifold {
    return VectorManifold(params);
  }

  auto setParams(Params const& params) -> void { v = params; }

  auto params() const -> Params { return v; }

  auto ptr() const -> Scalar const* { return v.data(); }

  auto unsafeMutPtr() -> Scalar* { return v.data(); }

  static auto tangentExamples() -> std::vector<Tangent> {
    return pointExamples<Scalar, kDim>();
  }

  template <concepts::Range TSequenceContainer>
  static auto average(TSequenceContainer const& range)
      -> std::optional<VectorManifold> {
    size_t const len = std::distance(std::begin(range), std::end(range));
    SOPHUS_ASSERT_GE(len, 0);

    Params params = Params::Zero();
    for (auto const& m : range) {
      params += m.params();
    }
    return VectorManifold(params / len);
  }

  Params v;
};

}  // namespace sophus