#include <device-common/device_state.h>

DeviceState::DeviceState(QObject *parent)
    : QObject{parent}
{}

DeviceState &DeviceState::instance()
{
    static DeviceState instance;
    return instance;
}

DeviceInfo *DeviceState::deviceInfo() { return &m_deviceInfo; }

DeviceStatus *DeviceState::deviceStatus() { return &m_deviceStatus; }
