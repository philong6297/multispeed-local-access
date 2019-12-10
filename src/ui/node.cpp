#include "node.hpp"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include "edge.hpp"
#include "graph_widget.hpp"

Node::Node(const msla::NodeId node_id, const bool is_root) :
    QGraphicsItem(nullptr), edge_list_{}, node_id_{node_id}, is_root_{is_root} {
  setFlag(ItemSendsGeometryChanges);
  setCacheMode(DeviceCoordinateCache);
  setZValue(-1);
}

void Node::AddEdge(Edge *edge) {
  edge_list_.push_back(edge);
  edge->Adjust();
}

QRectF Node::boundingRect() const {
  return {-60, -60, 60, 60};
}

QPainterPath Node::shape() const {
  QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

void Node::paint(QPainter *painter,
                 [[maybe_unused]] const QStyleOptionGraphicsItem * /*option*/,
                 [[maybe_unused]] QWidget * /*widget*/) {
  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::darkGray);
  // painter->drawEllipse(-7, -7, 20, 20);

  QRadialGradient gradient(-3, -3, 10);

  gradient.setColorAt(1, is_root_ ? Qt::red : Qt::yellow);
  painter->setBrush(gradient);
  painter->setPen(QPen(Qt::black, 1.5));

  const auto center_point = boundingRect().center();
  const auto rx           = boundingRect().height() / 2;
  const auto ry           = boundingRect().width() / 2;

  painter->drawEllipse(center_point, rx, ry);
  QFont font = painter->font();
  font.setBold(true);
  font.setPixelSize(20);
  painter->setFont(font);
  const auto rectangle = boundingRect();
  painter->drawText(rectangle, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(node_id_ + 1));
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (change == ItemPositionHasChanged) {
    for (auto *edge : edge_list_) {
      edge->Adjust();
    }
  }
  return QGraphicsItem::itemChange(change, value);
}
