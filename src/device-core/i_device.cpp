#include <device-common/device-core/i_device.h>

namespace {
QString DEFAULT_ICON_BASE_64 = "";
}

IDevice::IDevice(QObject *parent)
    :
    QObject{parent}
{}

IDevice::IDevice(QString name, QString descriptor, QString address, QObject *parent)
    : m_deviceName( name ),
    m_deviceDescriptor( descriptor ),
    m_deviceAddress( address ),
    QObject( parent )
{
}

IDevice::IDevice(const IDevice &device)
    : m_deviceName( device.deviceName() ),
    m_deviceDescriptor( device.deviceDescriptor() ),
    m_deviceAddress( device.deviceAddress() ),
    m_deviceIcon( device.deviceIcon() ),
    m_deviceStatus( device.status() ),
    QObject( device.parent() )
{

}

IDevice::IDevice(IDevice &&device)
{
    if(this != &device)
    {
        m_deviceAddress = device.m_deviceAddress;
        m_deviceDescriptor = device.m_deviceDescriptor;
        m_deviceIcon = device.m_deviceIcon;
        m_deviceName = device.m_deviceName;
        m_deviceStatus = device.m_deviceStatus;

        device.m_deviceStatus = DeviceStatus();
        device.m_deviceAddress = "";
        device.m_deviceName = "";
        device.m_deviceIcon = QIcon();
        device.m_deviceDescriptor = "";
    }
}

IDevice &IDevice::operator=(IDevice &&device)
{
    if(this != &device)
    {
        m_deviceAddress = device.m_deviceAddress;
        m_deviceDescriptor = device.m_deviceDescriptor;
        m_deviceIcon = device.m_deviceIcon;
        m_deviceName = device.m_deviceName;
        m_deviceStatus = device.m_deviceStatus;

        device.m_deviceStatus = DeviceStatus();
        device.m_deviceAddress = "";
        device.m_deviceName = "";
        device.m_deviceIcon = QIcon();
        device.m_deviceDescriptor = "";
    }

    return *this;
}

IDevice &IDevice::operator=(const IDevice& device)
{
    m_deviceAddress = device.m_deviceAddress;
    m_deviceDescriptor = device.m_deviceDescriptor;
    m_deviceIcon = device.m_deviceIcon;
    m_deviceName = device.m_deviceName;
    m_deviceStatus = device.m_deviceStatus;

    return *this;
}
