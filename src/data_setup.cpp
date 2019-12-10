//
// Created by longlp on 09/12/2019.
//
#include "data_setup.hpp"
#include <QLine>
#include <QRandomGenerator>
namespace {
  using msla::LinkId;
  using msla::NodeId;
}  // namespace
auto data_setup::initialize_a(const std::vector<QPointF>& graph_node_coordinate)
    -> DataInitializer {
  DataInitializer initializer{};

  const auto total_nodes = size(graph_node_coordinate);

  // setup weight
  const auto choose_node_weight = [](const NodeId node_id) -> double {
    if (node_id == 0 or node_id == 11 or node_id == 39) {
      return 6;
    }
    if (node_id == 16 or node_id == 24 or node_id == 48) {
      return 2;
    }
    if (node_id == 3 or node_id == 22 or node_id == 44) {
      return 10;
    }
    return 1;
  };
  for (NodeId node_id = 0; node_id < total_nodes; ++node_id) {
    initializer.msla_data.node_weight_table.emplace_back(choose_node_weight(node_id));
  }

  // setup links
  initializer.msla_data.link_value_table = {3, 7, 15};

  // setup root
  initializer.msla_data.root_id = 9;

  // setup limit links
  initializer.msla_data.limit_links = std::numeric_limits<size_t>::max() - 1;
  // setup cost handler
  initializer.cost_handler = [&initializer]([[maybe_unused]] const msla::Data& data,
                                            const NodeId                       from,
                                            const NodeId                       to,
                                            const LinkId link_type) -> double {
    const auto length =
        QLineF{initializer.graph_node_coordinate[from], initializer.graph_node_coordinate[to]}
            .length();

    switch (link_type) {
      case 0:
        return std::round(0.2 * length);
      case 1:
        return std::round(0.4 * length);
      case 2:
        return std::round(0.6 * length);
      default:
        return -1;
    }
  };

  // setup point coordinate
  initializer.graph_node_coordinate = graph_node_coordinate;
  return initializer;
}

auto data_setup::initialize_b(const std::vector<QPointF>& graph_node_coordinate)
    -> DataInitializer {
  auto initializer                       = initialize_a(graph_node_coordinate);
  initializer.msla_data.link_value_table = {3, 7, 15, 25};
  initializer.cost_handler               = [&initializer]([[maybe_unused]] const msla::Data& data,
                                            const NodeId                       from,
                                            const NodeId                       to,
                                            const LinkId link_type) -> double {
    const auto length =
        QLineF{initializer.graph_node_coordinate[from], initializer.graph_node_coordinate[to]}
            .length();

    switch (link_type) {
      case 0:
        return std::round(0.2 * length);
      case 1:
        return std::round(0.4 * length);
      case 2:
        return std::round(0.6 * length);
      case 3:
        return std::round(0.7 * length);
      default:
        return -1;
    }
  };
  return initializer;
}

auto data_setup::initialize_c(const std::vector<QPointF>& graph_node_coordinate)
    -> DataInitializer {
  auto initializer                  = initialize_b(graph_node_coordinate);
  initializer.msla_data.limit_links = 5;
  return initializer;
}

auto data_setup::generate_graph_node_coordinate() -> std::vector<QPointF> {
  std::vector<QPointF> graph;
  graph.resize(50);
  std::generate(begin(graph), end(graph), []() -> QPointF {
    const QPointF result = {QRandomGenerator::system()->bounded(0, 1001),
                            QRandomGenerator::system()->bounded(0, 1001)};
    return result;
  });
  return graph;
}

data_setup::DataInitializer::~DataInitializer() {
  msla_data.link_value_table.clear();
  msla_data.node_weight_table.clear();
  graph_node_coordinate.clear();
}
