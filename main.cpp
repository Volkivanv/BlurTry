#include <QCoreApplication>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

#include <QFileDialog>
//#include <QPixmap>

#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>

#include <QDir>
#include <QPainter>
#include <string>

QImage blurImage(QImage source, int r)
{
    if(source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    //
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(r);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(),QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(),
                 QRectF(0,0,source.width(),source.height()));
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setMinimumSize(600,600);
    auto vBox = new QVBoxLayout(&w);
    auto label = new QLabel;
    auto slider = new QSlider(Qt::Horizontal);
    auto pushButton = new QPushButton;
    vBox->addWidget(label);
    vBox->addWidget(slider);
    vBox->addWidget(pushButton);

    slider->setMinimum(0);
    slider->setMaximum(10);
    pushButton->setText("Load...");

    QString filepath;
    bool loaded = false;

    QObject::connect(pushButton, &QPushButton::clicked,[&filepath,&label,&loaded,&slider](){
        filepath = QFileDialog::getOpenFileName(nullptr,
                                     "Open jpeg image",
                                     "/opt/",
                                     "jpg files (*.jpg , *.JPG)");
       // auto sourceImage = QImage(filepath);
        auto sourceImage = QImage(filepath);
        int rad = slider->value();
        label->setPixmap(QPixmap::fromImage(blurImage(sourceImage, rad)).scaled(
                                 label->width(),
                                 label->height(),
                                 Qt::KeepAspectRatio));
        loaded = true;

    });

    QObject::connect(slider,&QSlider::valueChanged,[&filepath, &label, &loaded](int rad){
      if(loaded){
        auto sourceImage = QImage(filepath);
        label->setPixmap(QPixmap::fromImage(blurImage(sourceImage, rad)).scaled(
                                 label->width(),
                                 label->height(),
                                 Qt::KeepAspectRatio));


      }else{
            QString srad = QString::fromStdString("Please load a image! Blur radius = "+std::to_string(rad));
            label->setText(srad);
      }
    });

    w.show();
    return a.exec();
}
