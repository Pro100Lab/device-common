#include <device-common/device_status.h>

DeviceStatus::DeviceStatus(QObject *parent) : QObject(parent) {}

DeviceStatus::DeviceStatus(const DeviceStatus &other) : QObject(nullptr),
    m_description(other.m_description),
    m_properties(other.m_properties)
{}

QString DeviceStatus::description() const { return m_description; }

QMap<QString, QVariant> DeviceStatus::properties() const { return m_properties; }

void DeviceStatus::setDescription(const QString &desc) { m_description = desc; emit descriptionChanged(); }

void DeviceStatus::setProperties(const QMap<QString, QVariant> &props) { m_properties = props; emit propertiesChanged(); }

void DeviceStatus::setProperty(const QString &key, const QVariant &value) {
    m_properties[key] = value;
}

QVariant DeviceStatus::getProperty(const QString &key) const {
    return m_properties.value(key);
}

DeviceStatus DeviceStatus::fromJson(const QString json)
{
    DeviceStatus status;
    try
    {
        QJsonDocument doc = QJsonDocument::fromJson(json.toLocal8Bit());
        QJsonObject object = doc.object();

        for( const QString& key : object.keys() )
        {
            if( key == "description" )
                status.m_description = object.find(key)->toString();
            else
                status.m_properties[ key ] = object.find(key)->toVariant();
        }
    }
    catch( ... )
    {
        qDebug() << "failed DeviceStatus::fromJson";
    }

    return status;
}

QString DeviceStatus::ToJSON() const {
    QJsonObject jsonObj;

    // Добавляем описание
    jsonObj["description"] = m_description;

    // Преобразуем QMap<QString, QVariant> в QJsonObject
    for (auto it = m_properties.constBegin(); it != m_properties.constEnd(); ++it) {
        const QString &key = it.key();
        const QVariant &value = it.value();
        jsonObj.insert(key, QJsonValue::fromVariant(value));
    }

    QJsonDocument doc(jsonObj);
    return doc.toJson(QJsonDocument::Compact);
}
