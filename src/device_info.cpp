#include <device-common/device_info.h>


DeviceInfo::DeviceInfo(QObject *parent) : QObject(parent) {}

DeviceInfo::DeviceInfo(const DeviceInfo &other) : name(other.name), icon(other.icon) {}

DeviceInfo::DeviceInfo(DeviceInfo &&other) {
    if(this!=&other)
    {
        name = other.name;
        icon = other.icon;

        other.name = "";
        other.icon = QIcon();
    }
}

DeviceInfo &DeviceInfo::operator=(const DeviceInfo &other)
{
    name = other.name;
    icon = other.icon;
    return *this;
}

QString DeviceInfo::toJSON() {
    QJsonObject object;
    if(!name.isEmpty())
        object["name"] = name;

    if(!icon.isNull())
        object["icon"] = IconToBase64(icon);

    return QJsonDocument{ object }.toJson();
}

QString DeviceInfo::IconToBase64(const QIcon &icon, const QSize &size) {
    if (icon.isNull()) return QString();

    QPixmap pixmap = icon.pixmap(size);
    if (pixmap.isNull()) return QString();

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    if (!buffer.open(QIODevice::WriteOnly)) return QString();
    if (!pixmap.save(&buffer, "PNG")) { // Используем PNG формат
        buffer.close();
        return QString();
    }
    buffer.close();

    return QString("data:image/png;base64," + byteArray.toBase64());
}

QString DeviceInfo::IconToBase64WithType(QIcon icon) {
    return QString("data:image/png;base64,%1").arg(IconToBase64(icon));
}

QIcon DeviceInfo::Base64ToIcon(const QString &base64) {
    if (base64.isEmpty()) return QIcon();

    // Удаляем префикс data URI если есть
    QString cleanBase64 = base64;
    if (cleanBase64.startsWith("data:image/png;base64,")) {
        cleanBase64 = cleanBase64.remove(0, 22);
    }

    QByteArray byteArray = QByteArray::fromBase64(cleanBase64.toLatin1());
    if (byteArray.isEmpty()) return QIcon();

    QPixmap pixmap;
    if (!pixmap.loadFromData(byteArray, "PNG")) {
        return QIcon();
    }

    return QIcon(pixmap);
}

DeviceInfo DeviceInfo::fromJSON(QString json) {
    QJsonDocument doc = QJsonDocument::fromJson(json.toLocal8Bit());
    QJsonObject obj = doc.object();

    DeviceInfo di;

    if( obj.contains("name") )
        di.name = obj["name"].toString();

    if( obj.contains("icon") )
        di.icon = Base64ToIcon(obj["icon"].toString());

    return di;
}

DeviceInfo &DeviceInfo::operator=(DeviceInfo &&other)
{
    if(this!=&other)
    {
        name = other.name;
        icon = other.icon;

        other.name = "";
        other.icon = QIcon();
    }

    return *this;
}
