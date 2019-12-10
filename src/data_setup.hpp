//
// Created by longlp on 09/12/2019.
//

#ifndef SRC_DATA_SETUP_HPP_
#define SRC_DATA_SETUP_HPP_
#include <QPoint>
#include <optional>
#include "msla/defs.hpp"
namespace data_setup {
  struct DataInitializer {
    ~DataInitializer();
    msla::Data           msla_data{};
    std::vector<QPointF> graph_node_coordinate{};
    msla::CostHandler    cost_handler{};
  };
  [[nodiscard]] auto initialize_a(const std::vector<QPointF>& graph_node_coordinate)
      -> DataInitializer;
  [[nodiscard]] auto initialize_b(const std::vector<QPointF>& graph_node_coordinate)
      -> DataInitializer;
  [[nodiscard]] auto initialize_c(const std::vector<QPointF>& graph_node_coordinate)
      -> DataInitializer;
  [[nodiscard]] auto generate_graph_node_coordinate() -> std::vector<QPointF>;
}  // namespace data_setup
#endif  // SRC_DATA_SETUP_HPP_
