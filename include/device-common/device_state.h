#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

#include <QObject>
#include <QIcon>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QBuffer>
#include <device-common/device_info.h>
#include <device-common/device_status.h>


class DeviceState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DeviceInfo* deviceInfo READ deviceInfo  NOTIFY deviceInfoChanged FINAL)
    Q_PROPERTY(DeviceStatus* deviceStatus READ deviceStatus NOTIFY deviceStatusChanged FINAL)

public:
    DeviceState(const DeviceState&) = delete;
    DeviceState& operator=(const DeviceState&) = delete;

    static DeviceState& instance();

    DeviceInfo* deviceInfo();
    DeviceStatus* deviceStatus();

private:
    DeviceInfo m_deviceInfo;
    DeviceStatus m_deviceStatus;

signals:
    void deviceInfoChanged();
    void deviceStatusChanged();

private:
    explicit DeviceState(QObject *parent = nullptr);
};

#endif // DEVICE_STATE_H
