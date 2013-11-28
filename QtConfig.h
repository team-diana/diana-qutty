#ifndef QTCONFIG_H
#define QTCONFIG_H

extern "C" {
#include "putty.h"
}
#include <QIODevice>
#include <map>
#include <string>

using namespace std;

class QtConfig : public QObject {

    Q_OBJECT

public:
    map<string, string> ssh_host_keys;
    map<string, Config> config_list;

    QtConfig();
    ~QtConfig();

    bool restoreConfig();
    bool saveConfig();

signals:
    void savedSessionsChanged();

private:
    int readFromXML(QIODevice *device);
    int writeToXML(QIODevice *device);
    bool restoreFromPuttyWinRegistry();
};

// all global config is here
extern QtConfig qutty_config;

#define QUTTY_DEFAULT_CONFIG_SETTINGS "Default Settings"

int initConfigDefaults(Config *cfg);

#endif // QTCONFIG_H
