#pragma once
#include<QGraphicsView>
#include<QWheelEvent>
#include"node.h"
#include<QGraphicsSceneContextMenuEvent>
#include<QMenu>

class GraphicsView:public QGraphicsView{
    void wheelEvent(QWheelEvent *event){
        int delta=event->angleDelta().y();
        float factor=1.1;
        if(delta>0) scale(factor,factor);
        if(delta<0)scale(1/factor,1/factor);
    }
public:
    GraphicsView(QWidget*parent=NULL):QGraphicsView(parent){
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setRenderHint(QPainter::Antialiasing);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setBackgroundBrush(QBrush(QColor(38,38,38)));
    }
};

class canvas:public QGraphicsItem{
    QGraphicsScene*scene;
    GraphicsView*gw;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr){
        QPen pen(QColor(52,52,52));
        painter->setPen(pen);
        for(int y=-10000;y<=10000;y+=25){
            if(y%200==0){
                pen.setColor(QColor(22,22,22));
                painter->setPen(pen);
            }
            painter->drawLine(-10000,y,10000,y);
            if(y%200==0){
                pen.setColor(QColor(52,52,52));
                painter->setPen(pen);
            }
        }
        for(int x=-10000;x<=10000;x+=25){
            if(x%200==0){
                pen.setColor(QColor(22,22,22));
                painter->setPen(pen);
            }
            painter->drawLine(x,-10000,x,10000);
            if(x%200==0){
                pen.setColor(QColor(52,52,52));
                painter->setPen(pen);
            }
        }
        setZValue(-2);
    }
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
        node*&n=node::connection.first.first;
        QString&type=node::connection.first.second;
        QGraphicsPathItem*&path=node::connection.second;
        if(n!=NULL){
            scene->removeItem(path);
            delete path;
            path=NULL;
            if(type=="ci1") n->getCI1()->setState(hoverExit);
            else if(type=="ci2") n->getCI2()->setState(hoverExit);
            else if(n->getOutput()->empty()) n->getCO()->setState(hoverExit);
            type="";
            n=NULL;
        }

        QMenu menu;
        QAction*centerView=menu.addAction("Center View");
        QAction*resetZoom=menu.addAction("Reset Zoom");
        QAction*newNode=menu.addAction("New Node");
        QAction*current=menu.exec(event->screenPos());
        if(current==centerView){
            gw->centerOn(0,0);
        }
        if(current==resetZoom){
            gw->resetTransform();
        }
        if(current==newNode){
            node*n=new node(scene);
            n->setPos(event->scenePos());
        }
    };
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event){
        pair<node*,QString>&p=node::connection.first;
        node*&n=p.first;
        QString&type=p.second;
        QGraphicsPathItem*&gpath=node::connection.second;
        if(n!=NULL){
            QPainterPath path;
            if(n->getIType()==oneInput){
                if(type=="ci1"){
                    path.moveTo(n->x()+n->getWidth()/2,n->y()+n->getCI1()->getHeight());
                    path.cubicTo(n->x()+n->getWidth()/2,n->y()+n->getCI1()->getHeight()-25,
                                 event->scenePos().x(),event->scenePos().y()+25,event->scenePos().x(),event->scenePos().y());
                }
            }else if(n->getIType()==twoInput){
                if(type=="ci1"){
                    path.moveTo(n->x()+n->getWidth()/4,n->y()+n->getCI1()->getHeight());
                    path.cubicTo(n->x()+n->getWidth()/4,n->y()+n->getCI1()->getHeight()-25,
                                 event->scenePos().x(),event->scenePos().y()+25,event->scenePos().x(),event->scenePos().y());
                }else if(type=="ci2"){
                    path.moveTo(n->x()+3*n->getWidth()/4,n->y()+n->getCI1()->getHeight());
                    path.cubicTo(n->x()+3*n->getWidth()/4,n->y()+n->getCI1()->getHeight()-25,
                                 event->scenePos().x(),event->scenePos().y()+25,event->scenePos().x(),event->scenePos().y());
                }
            }
            if(n->getOType()==oneOutput){
                if(type=="co"){
                    path.moveTo(n->x()+n->getWidth()/2,n->y()+n->getHeight()-n->getCO()->getHeight());
                    path.cubicTo(n->x()+n->getWidth()/2,n->y()+n->getHeight()-n->getCO()->getHeight()+25,
                                 event->scenePos().x(),event->scenePos().y()-25,event->scenePos().x(),event->scenePos().y());
                }
            }
            gpath->setPath(path);
        }
        QGraphicsItem::hoverMoveEvent(event);
    };
public:
    canvas(QGraphicsScene*scene,GraphicsView*gw):scene(scene),gw(gw){
        setCursor(Qt::ArrowCursor);
        scene->addItem(this);
        setAcceptHoverEvents(true);
    }
    QRectF boundingRect() const{
        return QRectF(-10000,-10000,20000,20000);
    };
};
