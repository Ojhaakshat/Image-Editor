#pragma once
#include"node.h"
#include<QPushButton>
#include<QFileDialog>
#include<QVBoxLayout>
#include<QLineEdit>
#include<QLabel>
#include<QImageReader>

class ReadNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*h[5];
    QLabel*label[4];
public:
    QString fileName;
    QPushButton*button1;
    QLineEdit*edit[5];
    ReadNodePropertiesWindow(QString title):PropertiesWindow(title){
        button1=new QPushButton("Read Image File",this);
        v1=new QVBoxLayout;
        for(int i=0;i<5;i++) h[i]=new QHBoxLayout;
        for(int i=0;i<5;i++){
            edit[i]=new QLineEdit;
            edit[i]->setDisabled(true);
        }
        for(int i=0;i<4;i++) label[i]=new QLabel;
        label[0]->setText("File Extension:");
        label[1]->setText("File Size(in kB):");
        label[2]->setText("Image Width(in px):");
        label[3]->setText("Image Height(in px):");

        h[0]->addWidget(edit[0]);
        h[0]->addWidget(button1);
        h[1]->addWidget(label[0]);
        h[1]->addWidget(edit[1]);
        h[2]->addWidget(label[1]);
        h[2]->addWidget(edit[2]);
        h[3]->addWidget(label[2]);
        h[3]->addWidget(edit[3]);
        h[4]->addWidget(label[3]);
        h[4]->addWidget(edit[4]);
        for(int i=0;i<5;i++) v1->addLayout(h[i]);
        v1->addStretch();
        setLayout(v1);
    }
};

class readNode:public node{
    Q_OBJECT
    ReadNodePropertiesWindow*win;
private slots:
    void button1Clicked(){
        win->fileName=QFileDialog::getOpenFileName(win,"Read Image File","","Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)");
        QFileInfo info(win->fileName);
        QImageReader info2(win->fileName);
        win->edit[0]->setText(win->fileName);
        if(info2.canRead()){
            win->edit[1]->setText(info.suffix());
            win->edit[2]->setText(QString::number((float)info.size()/1000));
            win->edit[3]->setText(QString::number(info2.size().width()));
            win->edit[4]->setText(QString::number(info2.size().height()));
        }else{
            for(int i=1;i<=4;i++) win->edit[i]->setText("");
        }
        refresh();
    }
public:
    readNode(QGraphicsScene*scene):node(scene,noInput,oneOutput,"readNode"+QString::number(lastIndex++)){
        win=new ReadNodePropertiesWindow(name);
        propW=win;
        connect(win->button1,SIGNAL(clicked()),this,SLOT(button1Clicked()));
    }
    bool imageCalculate(QImage &image){
        bool ans=image.load(win->fileName);
        return ans;
    };
    static int lastIndex;
};

class ViewerNodePropertiesWindow:public PropertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
public:
    ImageGraphicsView*gw;
    QGraphicsScene*scene;
    ViewerNodePropertiesWindow(QString title):PropertiesWindow(title){
        v1=new QVBoxLayout;
        gw=new ImageGraphicsView;
        v1->addWidget(gw);
        setLayout(v1);
        scene=new QGraphicsScene(this);
        gw->setScene(scene);
        resize(800,600);
    }
};


class viewerNode:public node{
    Q_OBJECT
    ViewerNodePropertiesWindow*win;
    QImage image;
protected slots:
    void refresh(){
        imageCalculate(image);
    }
public:
    viewerNode(QGraphicsScene*scene):node(scene,oneInput,noOutput,"viewerNode"+QString::number(lastIndex++)){
        win=new ViewerNodePropertiesWindow(name);
        propW=win;
    }
    bool imageCalculate(QImage &image){
        win->scene->clear();
        if(input1==NULL) return false;
        bool ans=input1->imageCalculate(image);
        if(!ans) return false;
        QGraphicsPixmapItem*pix=win->scene->addPixmap(QPixmap::fromImage(image));
        pix->setPos(-(float)image.width()/2,-(float)image.height()/2);
        QPen pen;
        pen.setColor(Qt::red);
        win->scene->addLine(-(float)image.width()/2,(float)image.height()/2,(float)image.width()/2,(float)image.height()/2,pen);
        win->scene->addLine(-(float)image.width()/2,-(float)image.height()/2,(float)image.width()/2,-(float)image.height()/2,pen);
        win->scene->addLine(-(float)image.width()/2,(float)image.height()/2,-(float)image.width()/2,-(float)image.height()/2,pen);
        win->scene->addLine((float)image.width()/2,(float)image.height()/2,(float)image.width()/2,-(float)image.height()/2,pen);
        QFont font;
        font.setPixelSize(max(image.width(),image.height())/40);
        QGraphicsTextItem*text=win->scene->addText(QString::number(image.width())+"x"+QString::number(image.height()),font);
        text->setDefaultTextColor(Qt::red);
        text->setPos((float)image.width()/2,(float)image.height()/2);
        win->gw->centerOn(0,0);
        return true;
    };
    static int lastIndex;
};
