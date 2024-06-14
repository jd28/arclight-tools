#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QRegularExpression>
#include <QWidget>

#include <optional>

class QCompleter;
class QtProperty;
class QtIntPropertyManager;
class QtStringPropertyManager;
class QtAbstractPropertyBrowser;
class QtBoolPropertyManager;
class QtEnumPropertyManager;
class QtGroupPropertyManager;

namespace nw {
struct Resref;
}

class PropertiesView : public QWidget {
    Q_OBJECT
public:
    explicit PropertiesView(QWidget* parent = nullptr);

    QtProperty* addGroup(QString name);
    QtProperty* addPropertyBool(QString name, bool value);
    QtProperty* addPropertyInt(QString name, int value, std::optional<int> min, std::optional<int> max);
    QtProperty* addPropertyEnum(QString name, int value, QStringList enums, QList<QVariant> data = {});
    QtProperty* addPropertyString(QString name, const QString& value, QRegularExpression regex = {}, QCompleter* completer = nullptr);
    QtProperty* addPropertyString(QString name, const std::string& value, QRegularExpression regex = {}, QCompleter* completer = nullptr);
    QtProperty* addPropertyString(QString name, const nw::Resref& value, QRegularExpression regex = {}, QCompleter* completer = nullptr);

    QtBoolPropertyManager* bools() const;
    QtAbstractPropertyBrowser* editor() const;
    QtEnumPropertyManager* enums() const;
    QtIntPropertyManager* ints() const;
    QtGroupPropertyManager* groups() const;
    QtStringPropertyManager* strings() const;

signals:

private:
    QtBoolPropertyManager* bools_;
    QtAbstractPropertyBrowser* editor_;
    QtEnumPropertyManager* enums_;
    QtGroupPropertyManager* groups_;
    QtIntPropertyManager* ints_;
    QtStringPropertyManager* strings_;
};

#endif // PROPERTIESVIEW_H
