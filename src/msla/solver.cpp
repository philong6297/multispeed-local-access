//
// Created by longlp on 09/12/2019.
//

#include "solver.hpp"
#include <QtCore>
#include <optional>
#include <queue>
#include <vector>
namespace {
  using msla::ChildrenTrackingTable;
  using msla::CostHandler;
  using msla::Data;
  using msla::LinkId;
  using msla::NodeGraph;
  using msla::NodeId;
  using msla::ParentTrackingTable;
  using std::nullopt;
  using std::optional;
  using std::vector;
  auto initialize_parent_tracking_table(const NodeId root_id, const size_t total_nodes) {
    // ParentTrackingTable table;
    //    for (NodeId node = 0; node < total_nodes; ++node) {
    //      table.emplace(node, root_id);
    //    }
    ParentTrackingTable table(total_nodes, root_id);
    return table;
  }

  auto initialize_children_tracking_table(const NodeId root_id, const size_t total_nodes) {
    //    ChildrenTrackingTable table;
    //    for (NodeId node = 0; node < total_nodes; ++node) {
    //      table.emplace(node, std::set<NodeId>{});
    //      if (node != root_id) {
    //        table[root_id].insert(node);
    //      }
    //    }
    ChildrenTrackingTable table(total_nodes, std::set<NodeId>{});
    for (NodeId node = 0; node < total_nodes; ++node) {
      if (node != root_id) {
        table[root_id].insert(node);
      }
    }
    return table;
  }

  auto get_current_link_type(const Data&                  data,
                             const ChildrenTrackingTable& child_id,
                             const NodeId                 node_id) -> LinkId {
    std::set<NodeId>   visited{};
    std::queue<NodeId> queue;
    queue.push(node_id);
    visited.insert(node_id);
    auto total_weight = 0.0;
    while (not queue.empty()) {
      const auto current_node = queue.front();
      queue.pop();

      total_weight += data.node_weight_table[current_node];
      for (const auto child : child_id[current_node]) {
        if (visited.insert(child).second) {
          queue.push(child);
        }
      }
    }

    for (LinkId link_type_id = 0; link_type_id < size(data.link_value_table); ++link_type_id) {
      if (data.link_value_table[link_type_id] > total_weight) {
        return link_type_id;
      }
    }
    return size(data.link_value_table);
  }

  auto get_excess(const Data& data, const ChildrenTrackingTable& child_id, const NodeId node_id)
      -> double {
    const auto node_link_id = get_current_link_type(data, child_id, node_id);
    return data.link_value_table[node_link_id] - data.node_weight_table[node_id];
  }

  auto get_upgrade(const Data&                  data,
                   const ChildrenTrackingTable& child_id,
                   const CostHandler&           handler,
                   const size_t                 total_links,
                   const NodeId                 source,
                   const NodeId                 target) -> double {
    if (get_excess(data, child_id, target) <
        data.node_weight_table[source] + data.node_weight_table[target]) {
      const auto root                = data.root_id;
      const auto link_target_to_root = get_current_link_type(data, child_id, target);
      if (link_target_to_root < total_links - 1) {
        return handler(data, target, root, link_target_to_root + 1) -
               handler(data, target, root, link_target_to_root);
      }
    }
    return 0;
  }

  auto backtrace_get_node_link_to_root(const ParentTrackingTable& parent,
                                       const NodeId               root_id,
                                       const NodeId               from_this_node) -> NodeId {
    auto             node = from_this_node;
    std::set<NodeId> visited;
    while (parent[node] != root_id) {
      if (not visited.insert(node).second) {
        return root_id;
      }
      node = parent[node];
    }
    return node;
  }

  auto get_node_graph(const Data&                  data,
                      const ParentTrackingTable&   parent,
                      const ChildrenTrackingTable& child,
                      const size_t                 total_nodes,
                      const NodeId                 root_id) {
    NodeGraph graph{};
    for (NodeId node = 0; node < total_nodes; ++node) {
      if (node != root_id) {
        graph.emplace(std::make_pair(node, parent[node]), get_current_link_type(data, child, node));
      }
    }
    return graph;
  }
}  // namespace
auto msla::solve(const msla::Data&                                  data,
                 const msla::CostHandler&                           cost_handler,
                 const std::function<void(const NodeGraph& graph)>& state_0_callback,
                 const std::function<void(const NodeGraph& graph)>& result_callback) -> void {
  const auto total_nodes  = data.node_weight_table.size();
  const auto total_links  = data.link_value_table.size();
  const auto root_id      = data.root_id;
  const auto limit_links  = data.limit_links;
  auto       parent_table = initialize_parent_tracking_table(root_id, total_nodes);
  auto       child_table  = initialize_children_tracking_table(root_id, total_nodes);
  auto       level_table  = std::vector<NodeId>(total_nodes, 1);
  level_table[root_id]    = 0;
  auto link_count         = std::vector<size_t>(total_nodes, 0);
  link_count[root_id]     = total_nodes - 1;
  vector<vector<bool>> should_calculate_tradeoff(total_nodes, vector<bool>(total_nodes, true));

  auto i = 0;
  state_0_callback(get_node_graph(data, parent_table, child_table, total_nodes, root_id));
  for (auto should_continue = true; should_continue;) {
    qDebug() << "iteration " << i++;
    should_continue = false;
    for (NodeId node_x = 0; node_x < total_nodes; ++node_x) {
      if (node_x == root_id or link_count[node_x] > limit_links) {
        continue;
      }
      optional<NodeId> new_x_parent = nullopt;
      optional<double> min_tradeoff = nullopt;
      for (NodeId node_y = 0; node_y < total_nodes; ++node_y) {
        // qDebug() << "-- at node x " << node_x << "node y " << node_y;
        if (node_y == root_id or node_y == node_x or
            not should_calculate_tradeoff[node_x][node_y] or
            level_table[node_y] > level_table[node_x] or link_count[node_y] > limit_links) {
          continue;
        }

        const auto node_fx = backtrace_get_node_link_to_root(parent_table, root_id, node_x);
        if (node_fx == root_id) {
          should_calculate_tradeoff[node_x][node_y] = false;
          continue;
        }
        //        qDebug() << "reached 1";
        const auto link_fx = get_current_link_type(data, child_table, node_fx);
        //        qDebug() << "reached 2";
        const auto cost_x_y_link_fx = cost_handler(data, node_x, node_y, link_fx);
        //        qDebug() << "reached 3";
        const auto cost_fx_root_link_fx = cost_handler(data, node_fx, root_id, link_fx);
        //        qDebug() << "reached 4";
        
        const auto upgrade_x_y =
            get_upgrade(data, child_table, cost_handler, total_links, node_x, node_y);
        //        qDebug() << "reached 5";
        const auto tradeoff = cost_x_y_link_fx - cost_fx_root_link_fx + upgrade_x_y;
        if (tradeoff < 0.0) {
          should_continue = true;
          if (not min_tradeoff.has_value() or min_tradeoff > tradeoff) {
            min_tradeoff = tradeoff;
            new_x_parent = node_y;
          }
        }
        else {
          should_calculate_tradeoff[node_x][node_y] = false;
          should_calculate_tradeoff[node_y][node_x] = false;
        }
      }
      if (new_x_parent.has_value()) {
        const auto old_parent = parent_table[node_x];
        const auto new_parent = new_x_parent.value();

        child_table[old_parent].erase(node_x);
        child_table[new_parent].insert(node_x);
        parent_table[node_x]                          = new_parent;
        level_table[node_x]                           = level_table[new_parent] + 1;
        should_calculate_tradeoff[new_parent][node_x] = false;
        should_calculate_tradeoff[node_x][new_parent] = false;
        link_count[old_parent]--;
        link_count[new_parent]++;

        qDebug() << "---- change parent " << node_x << " from " << old_parent << " to "
                 << new_parent;
      }
      else {
        qDebug() << "---- no change parent" << node_x;
      }
    }
  }

  result_callback(get_node_graph(data, parent_table, child_table, total_nodes, root_id));
}
