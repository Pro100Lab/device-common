#ifndef I_DEVICE_H
#define I_DEVICE_H


#include <QObject>
#include <QIcon>
#include <QMap>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QQuickPaintedItem>

#include <device-common/device_info.h>
#include <device-common/device_status.h>

enum DeviceConnectionState{
    CONNECTED = 0,
    DISCONNECTED = 1,
    INOPERABLE = 2
};


class IconItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
public:
    void paint(QPainter *painter) override;

    QIcon icon() const;
    void setIcon(const QIcon &icon);

private:
    QIcon m_icon;
};

class IDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ deviceName NOTIFY deviceNameChanged FINAL)
    Q_PROPERTY(QString descriptor READ deviceDescriptor NOTIFY deviceDescriptorChanged FINAL)
    Q_PROPERTY(QIcon icon READ deviceIcon NOTIFY deviceIconChanged FINAL)
    Q_PROPERTY(QString address READ deviceAddress NOTIFY deviceAddressChanged FINAL)
    Q_PROPERTY(DeviceStatus status READ status NOTIFY deviceStatusChanged FINAL)
    Q_PROPERTY(QString iconBase64 READ iconBase64 NOTIFY deviceIconChanged FINAL)

public:
    explicit IDevice(QObject *parent=nullptr);
    explicit IDevice(QString name, QString descriptor, QString address, QObject *parent = nullptr);
    explicit IDevice(const IDevice& device);
    explicit IDevice(IDevice&& device);

    IDevice& operator=(IDevice&& device);
    IDevice& operator=(const IDevice& device);

    virtual QString deviceName() const { return m_deviceName; }
    virtual QString deviceDescriptor() const { return m_deviceDescriptor; }
    virtual QIcon deviceIcon() const { return m_deviceIcon; }
    virtual QString deviceAddress() const { return m_deviceAddress; }
    virtual DeviceStatus status() const { return m_deviceStatus; }
    virtual QString iconBase64() const { return DeviceInfo::IconToBase64( m_deviceIcon ); }

    void initialize(QString name, QIcon icon)
    {
        setName(name);
        setIcon(icon);
    }

    void initialize(DeviceInfo di)
    {
        if(!di.Name().isEmpty())
            setName(di.Name());
        if(!di.Icon().isNull())
            setIcon(di.Icon());
    }

    virtual void setName(QString name){
        m_deviceName = name;
        emit deviceNameChanged();
    }

    virtual void setDescriptor(QString descriptor) {
        m_deviceDescriptor = descriptor;
        emit deviceDescriptorChanged();
    }

    virtual void setIcon(QIcon icon) {
        m_deviceIcon = icon;
        emit deviceIconChanged();
    }

    virtual void setDescriptionStatus(const QString& description) {
        m_deviceStatus.setDescription(description);
        emit deviceStatusChanged();
    }

    virtual void setDescriptionProperty(const QString& name, QVariant value)
    {
        m_deviceStatus.setProperty(name, value);
        emit deviceStatusChanged();
    }

    virtual void setStatus(DeviceStatus status)
    {
        m_deviceStatus = status;
        emit deviceStatusChanged();
    }

signals:
    void deviceNameChanged();
    void deviceDescriptorChanged();
    void deviceIconChanged();
    void deviceAddressChanged();
    void deviceStatusChanged();

private:
    QString m_deviceAddress;
    QString m_deviceName;
    QString m_deviceDescriptor;
    QIcon m_deviceIcon;
    DeviceStatus m_deviceStatus;
};


#endif // I_DEVICE_H
