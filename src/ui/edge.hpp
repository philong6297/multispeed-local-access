#ifndef SRC_UI_EDGE_HPP_
#define SRC_UI_EDGE_HPP_

#include <QGraphicsItem>
#include "msla/defs.hpp"
class Node;

class Edge final : public QGraphicsItem {
 public:
  explicit Edge(Node *source_node, Node *dest_node, msla::LinkId link_id);
  ~Edge() override = default;
  Q_DISABLE_COPY_MOVE(Edge)

  void Adjust();

 private:
  [[nodiscard]] QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

 private:
  Node *       source_node_ = nullptr;
  Node *       dest_node_   = nullptr;
  QPointF      source_point_{};
  QPointF      dest_point_{};
  msla::LinkId link_id_;
};

#endif  // SRC_UI_EDGE_HPP_
