#include "graph_widget.hpp"
#include "edge.hpp"
#include "node.hpp"

GraphWidget::GraphWidget(const QString &title, QWidget *parent) : QGraphicsView(parent) {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setWindowTitle(title);
  setFixedSize(1000, 1000);

  auto *scene = new QGraphicsScene(this);
  scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  setScene(scene);
  setCacheMode(CacheBackground);
  setViewportUpdateMode(BoundingRectViewportUpdate);
  setRenderHint(QPainter::Antialiasing);
  setTransformationAnchor(AnchorUnderMouse);
}
void GraphWidget::resizeEvent(QResizeEvent *event) {
  const auto radius = Node::GetRadius();
  setSceneRect(-radius, -radius, width() + radius, height() + radius);
  fitInView(-radius,
            -radius,
            width() + radius,
            height() + radius,
            Qt::AspectRatioMode::KeepAspectRatioByExpanding);
  QGraphicsView::resizeEvent(event);
}
void GraphWidget::SetGraph(const msla::NodeGraph &     graph,
                           const std::vector<QPointF> &points,
                           const msla::NodeId          root_id) {
  std::vector<Node *> nodes;

  // setup points coordinate
  for (msla::NodeId node_id = 0; node_id < std::size(points); ++node_id) {
    nodes.emplace_back(new Node{node_id, node_id == root_id});
    nodes[node_id]->setPos(points[node_id]);
    scene()->addItem(nodes[node_id]);
  }

  // setup edge
  for (const auto &link : graph) {
    auto *     node_a  = nodes[link.first.first];
    auto *     node_b  = nodes[link.first.second];
    const auto link_id = link.second;
    scene()->addItem(new Edge(node_a, node_b, link_id));
  }
}
