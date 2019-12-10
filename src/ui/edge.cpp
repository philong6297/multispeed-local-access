#include "edge.hpp"
#include <QPainter>
#include <cmath>
#include "node.hpp"
namespace {
  constexpr auto kArrowSize = 10;

  auto get_edge_offset(const QLineF &line, const QRectF &ellipse) -> QPointF {
    if (const auto length = line.length(); length > 0.0) {
      const auto rx = ellipse.width() / 2 - 1;
      const auto ry = ellipse.height() / 2 - 1;
      return {(line.dx() * rx) / length, (line.dy() * ry) / length};
    }
    return {0, 0};
  }
}  // namespace

Edge::Edge(Node *source_node, Node *dest_node, const msla::LinkId link_id) :
    source_node_{source_node}, dest_node_{dest_node}, link_id_(link_id) {
  source_node->AddEdge(this);
  dest_node_->AddEdge(this);
  Adjust();
}

void Edge::Adjust() {
  if (source_node_ == nullptr or dest_node_ == nullptr) {
    return;
  }

  prepareGeometryChange();
  const auto   source_rect = source_node_->boundingRect();
  const auto   dest_rect   = dest_node_->boundingRect();
  const QLineF line(mapFromItem(source_node_, source_rect.center()),
                    mapFromItem(dest_node_, dest_rect.center()));
  dest_point_   = line.p2() - get_edge_offset(line, dest_rect);
  source_point_ = line.p1() + get_edge_offset(line, source_rect);
}

QRectF Edge::boundingRect() const {
  if (source_node_ == nullptr or dest_node_ == nullptr) {
    return {};
  }

  return QRectF(source_point_,
                QSizeF(dest_point_.x() - source_point_.x(), dest_point_.y() - source_point_.y()));
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  if (source_node_ == nullptr or dest_node_ == nullptr) {
    return;
  }

  QLineF line(source_point_, dest_point_);

  if (static_cast<int32_t>(line.length()) == 0) {
    return;
  }

  // Draw the line itself
  constexpr auto pen_width = 4;
  QPen           pen;
  pen.setWidthF(pen_width);
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::RoundJoin);
  pen.setStyle(Qt::SolidLine);
  const auto color = [](const msla::LinkId link_id) -> QColor {
    switch (link_id) {
      case 0:
        return Qt::green;
      case 1:
        return Qt::blue;
      case 2:
        return QColor(46, 43, 95);
      case 3:
        return QColor(139, 0, 255);
      default:
        return Qt::black;
    }
  }(link_id_);
  pen.setColor(color);
  painter->setPen(pen);
  painter->drawLine(line);
}
