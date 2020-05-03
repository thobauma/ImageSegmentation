//Copyright Matthias Fauconneau
#include "mainwindow.h"
/**
pseudocode from "An Experimental Comparison of Min-Cut/Max-Flow Algorithms for Energy Minimization in Vision",
Yuri Boykov and Vladimir Kolmogorov, 2002

S={s}, T={t}, A={s,t}, O={} // A: active nodes, O: orphans
loop
 grow S or T to find an augmenting path P from s to t
 if P = ∅ terminate
 augment on P
 adopt orphans

//grow
while A = ∅ {
  pick an active node p ∈ A //FIFO -> breadth-first
  for every neighbor q such that tree cap(p → q) > 0 {
    if TREE(q) = ∅ then add q to search tree as an active node:
    T REE(q) := TREE(p), PARENT (q) := p, A := A ∪ {q}
    if TREE(q) = ∅ and TREE(q) = TREE(p) return P = P AT Hs→t
  }
  remove p from A
}
return P = ∅

//augment
find the bottleneck capacity ∆ on P
update the residual graph by pushing flow ∆ through P
for each edge (p, q) in P that becomes saturated {
  if TREE(p) = TREE(q) = S then set PARENT (q) := ∅ and O := O ∪ {q}
  if TREE(p) = TREE(q) = T then set PARENT (p) := ∅ and O := O ∪ {p}
}

//adopt
while O = ∅ {
  pick an orphan node p ∈ O and remove it from O
  process p
}

//process p
foreach neighbour q {
  if
   - TREE(p) = TREE(q)
   - capacity( q -> p ) > 0
   - q is not orphan (mark node confirmed to be connected to source at a given stage)
   - best "distance to source"
  then
   PARENT(p) = q
} else //no valid parent
  foreach neighbour q with TREE(q) = TREE(p) {
    if capacity( q -> p ) > 0: A << q
    if parent( q ) = p: O << q; parent(q)=0;
  }
  TREE(p) = 0
  A -= p //or just skip free nodes when processing active nodes

**/

int main( int argc, char *argv[] ) {
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Qt");
    QApplication::setApplicationName("Manga");
    MainWindow w; w.show();
    return a.exec();
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setObjectName("mainToolbar");
    status = new QLabel(this);
    toolbar->addWidget(status);
    toolbar->addAction(QIcon::fromTheme("view-refresh"),"Restart",this,SLOT(start()));

    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setRenderHint( QPainter::Antialiasing );
    setCentralWidget(view);
    resize(1280,960);
    QTimer::singleShot(0,this,SLOT(start())); //finish constructing MainWindow before computing
}


bool Node::hasNeighbour( int i ) {
    Q_ASSERT(i>0 && i <= 4);
    const int neighbour[5][2] = { {0,0},{0,-1},{-1,0},{1,0},{0,1} };
    int nx = x+neighbour[i][0];
    int ny = y+neighbour[i][1];
    return ( nx>=0 && ny>=0 && nx<graph->w && ny<graph->h );
}
Node Node::neighbour( int i ) {
    Q_ASSERT(i>0 && i <= 4);
    const int neighbour[5][2] = { {0,0},{0,-1},{-1,0},{1,0},{0,1} };
    return graph->node(x+neighbour[i][0], y+neighbour[i][1]);
}
Node Node::parent() { Q_ASSERT(data->parent>0 && data->parent <= 4); return neighbour(data->parent); }

void Node::setActive() { data->active=true; graph->active << *this; }
void Node::setSource() { data->tree = 1; data->root=true; setActive(); }
void Node::setSink() { data->tree = 2; data->root=true; setActive(); }
void Node::setTree( Node p ) { data->tree=p.data->tree; }
bool Node::isFree() { return data->tree == 0; }
void Node::setFree() { data->tree = 0; }
void Node::setParent( int i ) { Q_ASSERT(i>0 && i <= 4); data->parent = i; }
bool Node::hasParent() { return data->parent != 0; }
void Node::setOrphan() { data->parent = 0; graph->orphan << *this; }
bool Node::isOrphan() { Node t=*this; while( t.hasParent() ) t=t.parent(); return !t.data->root; }

float& capacity( Node p, Node q ) {
    if( p.x < q.x ) return p.data->capacity[0];
    if( p.y < q.y ) return p.data->capacity[1];
    if( p.x > q.x ) return q.data->capacity[0];
    if( p.y > q.y ) return q.data->capacity[1];
}
bool sameTree( Node p, Node q ) { return p.data->tree==q.data->tree; }

void Graph::rasterCircle(int x0, int y0, int radius ) { //from Wikipedia
#define setPixel(x,y) node(x,y).setSource();
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    setPixel(x0, y0 + radius);
    setPixel(x0, y0 - radius);
    setPixel(x0 + radius, y0);
    setPixel(x0 - radius, y0);

    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        setPixel(x0 + x, y0 + y);
        setPixel(x0 - x, y0 + y);
        setPixel(x0 + x, y0 - y);
        setPixel(x0 - x, y0 - y);
        setPixel(x0 + y, y0 + x);
        setPixel(x0 - y, y0 + x);
        setPixel(x0 + y, y0 - x);
        setPixel(x0 - y, y0 - x);
    }
#undef setPixel
}

Graph::Graph(QImage* image) {
    w=image->width();
    h=image->height();
    nodes = new NodePrivate[w*h];
    for(int y=0;y<h;y++) for(int x=0;x<w;x++) {
        NodePrivate* data = &nodes[y*w+x];
        memset(data,0,sizeof(NodePrivate));
        if(x<w-1) data->capacity[0] = qMax(0.0, 1.0-qAbs( qGray( image->pixel(x,y) ) - qGray( image->pixel(x+1,y) ) )/128.0);
        if(y<h-1) data->capacity[1] = qMax(0.0, 1.0-qAbs( qGray( image->pixel(x,y) ) - qGray( image->pixel(x,y+1) ) )/128.0);
    }
    //set source at center //need some radius to avoid a minimal cut around source
      rasterCircle(w/2,h/2,4);
    //set sink at border
    for(int x=0;x<w;x++) {
        node(x,0).setSink();
        node(x,h-1).setSink();
    }
    for(int y=0;y<h;y++) {
        node(0,y).setSink();
        node(w-1,y).setSink();
    }
}

Node Graph::node(int x, int y) {
    Q_ASSERT( x>=0 && y>=0 && x<w && y<h );
    Node n = { this,x,y, &nodes[ y*w+x ] };
    return n;
}

void MainWindow::start() {
    image = new QImage("easy.png");
    graph = new Graph(image);
    draw();
    QTimer::singleShot(1000,this,SLOT(grow()));
}
void MainWindow::grow() {
    status->setText(QString("%1 actives").arg(graph->active.count()));
    //grow S or T to find an augmenting path P from s to t
    while( graph->active.count() ) {
        Node p = graph->active.first(); //FIFO -> breadth-first
        if( !p.isFree() ) for( int i=1; i<=4; i++ ) { //for every neighbor q
            if( !p.hasNeighbour(i) ) continue;
            Node q = p.neighbour(i);
            if( capacity(p,q) <= 0 ) continue;
            if( q.isFree() ) { q.setTree(p), q.setParent( 5-i ); q.setActive(); }
            else if( !sameTree(q,p) ) { path[0]=p; path[1]=q; goto augmentPath; }
        }
        graph->active.removeFirst();
        p.data->active=false;
    } /*else*/ status->setText(QString("Done")); draw(); return;
    augmentPath:
    //draw();
    QTimer::singleShot(0,this,SLOT(augment()));
}

void MainWindow::augment() {
    //find the bottleneck capacity on the path
    float bottleneck=capacity( path[0], path[1] );
    for( int i=0; i<2; i++ ) {
        Node t = path[i];
        while( t.hasParent() ) { bottleneck = qMin( bottleneck, capacity( t, t.parent() ) ); t=t.parent(); }
    }
    //update the residual graph by pushing flow ∆ through P
    capacity( path[0], path[1] ) -= bottleneck;
    for( int i=0; i<2; i++ ) {
        Node t = path[i];
        while( t.hasParent() ) {
            Node p = t.parent();
            capacity( t, p ) -= bottleneck;
            if( capacity( t, p ) <= 0 ) t.setOrphan();
            t=p;
        }
    }
    status->setText(QString("%1 orphans").arg(graph->orphan.count()));
    //draw();
    QTimer::singleShot(0,this,SLOT(adopt()));
}

void MainWindow::adopt() {
    //adopt orphans
    while( graph->orphan.count() ) {
        Node p = graph->orphan.takeFirst();
        for( int i=1; i<=4; i++ ) { //for every neighbor q
            if( !p.hasNeighbour(i) ) continue;
            Node q = p.neighbour(i);
            if( sameTree(p,q) && capacity(q,p) > 0 && !q.isOrphan() ) { //+ best "distance to source"
                p.setParent( i ); goto adopted;
            }
        }
        /*else*/ {
            for( int i=1; i<=4; i++ ) { //for every neighbor q
                if( !p.hasNeighbour(i) ) continue;
                Node q = p.neighbour(i);
                if( sameTree(p,q) ) {
                    if( capacity(q,p) > 0 ) q.setActive();
                    if( q.data->parent == 5-i ) q.setOrphan();
                }
            }
            p.setFree();
        }
        adopted:
        ;
    }
    draw();
    QTimer::singleShot(0,this,SLOT(grow()));
}

float clip( float c ) { return qMin( 255.0f, qMax( 0.0f, c ) ); }
void MainWindow::draw() {
    scene->clear();
    for(int y=0;y<graph->h;y++) for(int x=0;x<graph->w;x++) {
        NodePrivate* data = graph->node(x,y).data;
        const QColor nodeColor[3] = { QColor(Qt::black), QColor(Qt::red), QColor(Qt::blue) };
        QPen pen(nodeColor[data->tree]);
        if( !data->active ) pen.setStyle(Qt::DotLine);
        scene->addEllipse(x,y,0.5,0.5,pen);
        scene->addEllipse(x+0.125,y+0.125,0.25,0.25,QPen(),QBrush(image->pixel(x,y),Qt::SolidPattern));
        if( data->capacity[0] > 0 ) {
            ubyte a = clip( data->capacity[0]*255 );
            QPen pen(QColor(0,0,0,a));
            pen.setWidthF(data->capacity[0]/32.0);
            scene->addLine(x+0.5,y+0.25,x+1,y+0.25,pen);
        }
        if( data->capacity[1] > 0 ) {
            ubyte a = clip( data->capacity[1]*255 );
            QPen pen(QColor(0,0,0,a));
            pen.setWidthF(data->capacity[1]/32.0);
            scene->addLine(x+0.25,y+0.5,x+0.25,y+1,pen);
        }
        if( data->parent > 0 ) {
            Q_ASSERT( data->parent <= 4 );
            QColor color = nodeColor[data->tree]; color.setAlpha(128);
            QPen pen(color);
            pen.setWidthF(1/16.0);
            if( data->parent == 1 ) scene->addLine(x+0.25,y,x+0.25,y-0.5,pen)->setZValue(-1); //top
            if( data->parent == 2 ) scene->addLine(x,y+0.25,x-0.5,y+0.25,pen)->setZValue(-1); //left
            if( data->parent == 3 ) scene->addLine(x+0.5,y+0.25,x+1,y+0.25,pen)->setZValue(-1); //right
            if( data->parent == 4 ) scene->addLine(x+0.25,y+0.5,x+0.25,y+1,pen)->setZValue(-1); //down
        }
    }
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio );
}
