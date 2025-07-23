#ifndef DEVICESTATE_H
#define DEVICESTATE_H

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
    QString Name() {return name;}
    QIcon Icon() {return icon;}

    void setName(QString newName) { name = newName; }
    void setIcon(QIcon newIcon) {icon = newIcon; }

    explicit DeviceInfo(QObject* parent = nullptr) : QObject(parent) {}
    DeviceInfo(const DeviceInfo& other) : name(other.name), icon(other.icon) {}
    DeviceInfo(DeviceInfo&& other) {
        if(this!=&other)
        {
            name = other.name;
            icon = other.icon;

            other.name = "";
            other.icon = QIcon();
        }
    }

    DeviceInfo& operator=(DeviceInfo&& other)
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

    DeviceInfo& operator=(const DeviceInfo& other)
    {
        name = other.name;
        icon = other.icon;
        return *this;
    }


signals:
    void nameChanged();
    void iconChanged();

public:

    QString toJSON() {
        QJsonObject object;
        if(!name.isEmpty())
            object["name"] = name;

        if(!icon.isNull())
            object["icon"] = IconToBase64(icon);

        return QJsonDocument{ object }.toJson();
    }

    static QString IconToBase64(const QIcon& icon, const QSize& size = QSize(64, 64)) {
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

    static QString IconToBase64WithType(QIcon icon) {
        return QString("data:image/png;base64,%1").arg(IconToBase64(icon));
    }

    static QIcon Base64ToIcon(const QString& base64) {
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

    static DeviceInfo fromJSON(QString json) {
        QJsonDocument doc = QJsonDocument::fromJson(json.toLocal8Bit());
        QJsonObject obj = doc.object();

        DeviceInfo di;

        if( obj.contains("name") )
            di.name = obj["name"].toString();

        if( obj.contains("icon") )
        {
            di.icon = Base64ToIcon(obj["icon"].toString());
        }

        return di;
    }
};


class DeviceStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QMap<QString, QVariant> properties READ properties WRITE setProperties NOTIFY propertiesChanged )

signals:
    void descriptionChanged();
    void propertiesChanged();

public:
    explicit DeviceStatus(QObject*parent=nullptr) : QObject(parent) {};
    // Удаляем конструктор копирования и оператор присваивания
    DeviceStatus(const DeviceStatus& other) : m_description( other.m_description ), m_properties( other.m_properties ) {}
    DeviceStatus& operator=(const DeviceStatus& other) { m_description = other.m_description; m_properties = other.m_properties; return *this;};

    QString description() const { return m_description; }
    void setDescription(const QString &desc) { m_description = desc; emit descriptionChanged(); }

    QMap<QString, QVariant> properties() const { return m_properties; }
    void setProperties(const QMap<QString, QVariant> &props) { m_properties = props; emit propertiesChanged(); }

    // Добавляем значение в properties
    Q_INVOKABLE void setProperty(const QString &key, const QVariant &value) {
        m_properties[key] = value;
    }

    // Получаем значение из properties
    Q_INVOKABLE QVariant getProperty(const QString &key) const {
        return m_properties.value(key);
    }

    static DeviceStatus fromJson(const QString json)
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

    QString ToJSON() const {
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

private:
    QString m_description;
    QMap<QString, QVariant> m_properties;
};


class DeviceState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DeviceInfo* deviceInfo READ deviceInfo  NOTIFY deviceInfoChanged FINAL)
    Q_PROPERTY(DeviceStatus* deviceStatus READ deviceStatus NOTIFY deviceStatusChanged FINAL)
public:
    // Удаляем конструктор копирования и оператор присваивания
    DeviceState(const DeviceState&) = delete;
    DeviceState& operator=(const DeviceState&) = delete;

    // Статический метод для получения экземпляра синглтона
    static DeviceState& instance()
    {
        static DeviceState instance;
        return instance;
    }

    DeviceInfo* deviceInfo() { return &m_deviceInfo; }
    DeviceStatus* deviceStatus() { return &m_deviceStatus; }

private:
    DeviceInfo m_deviceInfo;
    DeviceStatus m_deviceStatus;
signals:
    void deviceInfoChanged();
    void deviceStatusChanged();

         // Ваши сигналы здесь

private:
    // Приватный конструктор
    explicit DeviceState(QObject *parent = nullptr);
};

#endif // DEVICESTATE_H
