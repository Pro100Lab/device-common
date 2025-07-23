#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <QObject>
#include <QIcon>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBuffer>


class DeviceInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ Name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QIcon icon READ Icon WRITE setIcon  NOTIFY iconChanged FINAL)

private:
    QString name;
    QIcon icon;

public:
    explicit DeviceInfo(QObject* parent = nullptr);

    DeviceInfo(const DeviceInfo& other);
    DeviceInfo(DeviceInfo&& other);

    DeviceInfo& operator=(const DeviceInfo& other);
    DeviceInfo& operator=(DeviceInfo&& other);

    QString Name() {return name;}
    QIcon Icon() {return icon;}

    void setName(QString newName) { name = newName; }
    void setIcon(QIcon newIcon) {icon = newIcon; }


signals:
    void nameChanged();
    void iconChanged();

public:

    QString toJSON();
    static DeviceInfo fromJSON(QString json);

    /// FIXME: Вынеси меня в отдельный модуль с утилитами
    static QString IconToBase64(const QIcon& icon, const QSize& size = QSize(64, 64));
    static QString IconToBase64WithType(QIcon icon);
    static QIcon Base64ToIcon(const QString& base64);

};

#endif // DEVICE_INFO_H
