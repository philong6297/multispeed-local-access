#ifndef SRC_UI_GRAPH_WIDGET_HPP_
#define SRC_UI_GRAPH_WIDGET_HPP_

#include <QGraphicsView>
#include "msla/defs.hpp"
class Node;

class GraphWidget : public QGraphicsView {
  Q_OBJECT

 public:
  explicit GraphWidget(const QString& title, QWidget* parent = nullptr);
  ~GraphWidget() override = default;
  Q_DISABLE_COPY_MOVE(GraphWidget)

  void SetGraph(const msla::NodeGraph&      graph,
                const std::vector<QPointF>& points,
                msla::NodeId                root_id);

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
};

#endif  // SRC_UI_GRAPH_WIDGET_HPP_
