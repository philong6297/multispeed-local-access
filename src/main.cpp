#include "ui/graph_widget.hpp"

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QTime>
#include <QtCore>
#include "data_setup.hpp"
#include "msla/solver.hpp"
namespace {}  // namespace
int main(int argc, char** argv) {
  [[maybe_unused]] QApplication app(argc, argv);

  const auto generated_graph = data_setup::generate_graph_node_coordinate();

  // generate graph
  GraphWidget     widget_init("Khởi Tạo", nullptr);
  msla::NodeGraph init_graph;
  widget_init.SetGraph(init_graph, generated_graph, 9);
  widget_init.show();

  // a and 0
  GraphWidget widget_a("Kết Quả Câu A", nullptr);
  GraphWidget widget_0("Kết Quả Trạng Thái 0", nullptr);
  const auto  initializer_a = data_setup::initialize_a(generated_graph);
  msla::solve(
      initializer_a.msla_data,
      initializer_a.cost_handler,
      [&initializer_a, &widget_0](const msla::NodeGraph& graph) {
        widget_0.SetGraph(
            graph, initializer_a.graph_node_coordinate, initializer_a.msla_data.root_id);
      },
      [&initializer_a, &widget_a](const msla::NodeGraph& graph) {
        widget_a.SetGraph(
            graph, initializer_a.graph_node_coordinate, initializer_a.msla_data.root_id);
      });
  widget_0.show();
  widget_a.show();

  // b
  GraphWidget widget_b("Kết Quả Câu B", nullptr);
  const auto  initializer_b = data_setup::initialize_b(generated_graph);
  msla::solve(
      initializer_b.msla_data,
      initializer_b.cost_handler,
      []([[maybe_unused]] const msla::NodeGraph& graph) {},
      [&initializer_b, &widget_b](const msla::NodeGraph& graph) {
        widget_b.SetGraph(
            graph, initializer_b.graph_node_coordinate, initializer_b.msla_data.root_id);
      });
  widget_b.show();

  GraphWidget widget_c("Kết quả Câu C", nullptr);
  const auto  initializer_c = data_setup::initialize_c(generated_graph);
  msla::solve(
      initializer_c.msla_data,
      initializer_c.cost_handler,
      []([[maybe_unused]] const msla::NodeGraph& graph) {},
      [&initializer_c, &widget_c](const msla::NodeGraph& graph) {
        widget_c.SetGraph(
            graph, initializer_c.graph_node_coordinate, initializer_c.msla_data.root_id);
      });
  widget_c.show();

  return QApplication::exec();
}
