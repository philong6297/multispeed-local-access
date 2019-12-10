//
// Created by longlp on 08/12/2019.
//

#ifndef SRC_MSLA_DEFS_HPP_
#define SRC_MSLA_DEFS_HPP_
#include <functional>
#include <map>
#include <set>
#include <vector>
namespace msla {
  using NodeId = size_t;
  using LinkId = size_t;
  // using LinkValueTable        = std::map<LinkId, double>;
  using LinkValueTable = std::vector<double>;
  // using NodeWeightTable       = std::map<NodeId, double>;
  using NodeWeightTable = std::vector<double>;
  // using ParentTrackingTable   = std::map<NodeId, NodeId>;
  using ParentTrackingTable = std::vector<NodeId>;
  // using ChildrenTrackingTable = std::map<NodeId, std::set<NodeId>>;
  using ChildrenTrackingTable = std::vector<std::set<NodeId>>;
  using NodeGraph             = std::map<std::pair<NodeId, NodeId>, LinkId>;
  struct Data {
    LinkValueTable  link_value_table;
    NodeWeightTable node_weight_table;
    NodeId          root_id;
    size_t          limit_links;
  };
  using CostHandler =
      std::function<double(const Data& data, NodeId from, NodeId to, LinkId link_type)>;
}  // namespace msla
#endif  // SRC_MSLA_DEFS_HPP_
