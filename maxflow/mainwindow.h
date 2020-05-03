//Copyright Matthias Fauconneau
#pragma once

#include <sys/mman.h>

#include <cmath>
#include <algorithm>

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QTimer>

#include <QVector3D>

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QSlider>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPainterPath>

typedef unsigned int uint;
typedef unsigned char ubyte;

struct Node;
struct NodePrivate;
struct Graph;

struct NodePrivate {
    float capacity[2]; //right,down ( up=upNode.down, left=leftNode.right )
    int tree; //0: free, 1: source tree, 2: sink tree
    int parent; //0: orphan 1..8: direction
    bool root; //source or sink
    bool active; //for visualization
};

struct Node { //cursor on a graph node
public:
    bool hasNeighbour( int i );
    Node neighbour(int);
    Node parent();

    void setActive();

    void setTree( Node);
    bool isFree();
    void setFree();
    void setSource();
    void setSink();

    void setParent(int);
    bool hasParent();
    void setOrphan();
    bool isOrphan();

//private:
    Graph* graph;
    int x,y;
    NodePrivate* data;
};

struct Graph {
    Graph( QImage* image );
    Node node(int x, int y);
    void rasterCircle(int x0, int y0, int radius );
public:
    int w,h;
    NodePrivate* nodes;
    QList<Node> active,orphan;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

public slots:
    void start();
    void grow();
    void augment();
    void adopt();
    void draw();
private:
    Node path[2];
    Graph* graph;

    QLabel* status;
    QImage* image;
    QGraphicsScene* scene;
    QGraphicsView* view;
};
