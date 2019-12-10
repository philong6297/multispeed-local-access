#ifndef SRC_UI_NODE_HPP_
#define SRC_UI_NODE_HPP_

#include <QGraphicsItem>
#include <vector>
#include "msla/defs.hpp"
class Edge;
class GraphWidget;

class Node final : public QGraphicsItem {
 public:
  explicit Node(msla::NodeId node_id, bool is_root);
  ~Node() override = default;
  Q_DISABLE_COPY_MOVE(Node)
  void           AddEdge(Edge *edge);
  static int32_t GetRadius() { return 60; }

 private:
  friend class Edge;
  [[nodiscard]] QRectF       boundingRect() const override;
  [[nodiscard]] QPainterPath shape() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

 private:
  std::vector<Edge *> edge_list_;
  msla::NodeId        node_id_;
  bool                is_root_;
};

#endif  // SRC_UI_NODE_HPP_
