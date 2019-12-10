//
// Created by longlp on 09/12/2019.
//

#ifndef SRC_MSLA_SOLVER_HPP_
#define SRC_MSLA_SOLVER_HPP_
#include <optional>
#include "defs.hpp"
namespace msla {
  auto solve(const Data&                                        data,
             const CostHandler&                                 cost_handler,
             const std::function<void(const NodeGraph& graph)>& state_0_callback,
             const std::function<void(const NodeGraph& graph)>& result_callback) -> void;
}  // namespace msla

#endif  // SRC_MSLA_SOLVER_HPP_
