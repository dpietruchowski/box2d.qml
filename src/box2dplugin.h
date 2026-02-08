#ifndef BOX2DPLUGIN_H
#define BOX2DPLUGIN_H

#include <QQmlExtensionPlugin>

class Box2DPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // BOX2DPLUGIN_H
