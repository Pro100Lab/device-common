#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H

#include <QObject>
#include <QIcon>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBuffer>

class DeviceStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QMap<QString, QVariant> properties READ properties WRITE setProperties NOTIFY propertiesChanged )

signals:
    void descriptionChanged();
    void propertiesChanged();

public:
    explicit DeviceStatus(QObject*parent=nullptr);;

    DeviceStatus(const DeviceStatus& other);

    DeviceStatus& operator=(const DeviceStatus& other) = delete;

    Q_INVOKABLE QString description() const;
    Q_INVOKABLE QMap<QString, QVariant> properties() const;
    Q_INVOKABLE QVariant getProperty(const QString &key) const;

    Q_INVOKABLE void setDescription(const QString &desc);
    Q_INVOKABLE void setProperties(const QMap<QString, QVariant> &props);
    Q_INVOKABLE void setProperty(const QString &key, const QVariant &value);


    QString ToJSON() const;
    static DeviceStatus fromJson(const QString json);

private:
    QString m_description;
    QMap<QString, QVariant> m_properties;
};

#endif // DEVICE_STATUS_H
