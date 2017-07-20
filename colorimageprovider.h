#include <QQuickImageProvider>
#include <qqrencode.h>
#include <QDebug>
#include <QUrl>
#include <QScreen>

class ColorImageProvider : public QQuickImageProvider
{
public:
    ColorImageProvider(float ratio=4.0)
        : QQuickImageProvider(QQuickImageProvider::Pixmap),
          _ratio(ratio)
    {
        //_width = qMin(qApp->primaryScreen()->size().width()/(ratio * qApp->devicePixelRatio()), qApp->primaryScreen()->size().height()/(ratio * qApp->devicePixelRatio()));
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        //qDebug() << "newid" << id;
        QUrl copy(id);

        QString convertId = QUrl::fromPercentEncoding(id.toUtf8());

        QQREncode encoder;
        encoder.encode(convertId);

        if (qApp->primaryScreen()->primaryOrientation() == Qt::LandscapeOrientation || qApp->primaryScreen()->primaryOrientation() == Qt::InvertedLandscapeOrientation) {
            _width = qMin(qApp->primaryScreen()->size().width()/(_ratio * qApp->devicePixelRatio()), qApp->primaryScreen()->size().height()/(_ratio * qApp->devicePixelRatio()));
        } else {
            _width = qMax(qApp->primaryScreen()->size().width()/(_ratio * qApp->devicePixelRatio()), qApp->primaryScreen()->size().height()/(_ratio * qApp->devicePixelRatio()));
        }

        return QPixmap::fromImage(encoder.toQImage(_width));
    }

private:
    int _width;
    float _ratio;
};
