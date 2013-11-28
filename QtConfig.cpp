#define QUTTY_XMLIZE_STRUCTS

#include "QtConfig.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <stdio.h>
#ifdef _WINDOWS
extern "C" {
#include "WINDOWS\STORAGE.H"
}
#endif

#ifdef _LINUX
#define _snprintf(buf, size, format, ...) sprintf(buf, format, __VA_ARGS__) 
#endif
QtConfig::QtConfig()
{
}

QtConfig::~QtConfig() {
    ;
}

int QtConfig::readFromXML(QIODevice *device)
{
#ifdef _WINDOWS
    QXmlStreamReader xml;
    int i;
    char *tmpbuf;

    xml.setDevice(device);
    if (!xml.readNextStartElement() || xml.name() != "qutty" ||
            xml.attributes().value("version") != "1.0") {
        QMessageBox::warning(NULL, QObject::tr("Qutty Configuration"),
                             QObject::tr("Invalid xml file"));
        return false;
    }
    while (xml.readNextStartElement()) {
        if (xml.name() == "config" && xml.attributes().value("version") == "1.0") {
            Config cfg;
            memset(&cfg, 0, sizeof(Config));
            while (xml.readNextStartElement()) {
                if (xml.name() == "dataelement") {
                    QStringRef tmptype = xml.attributes().value("datatype");
                    QStringRef tmpname = xml.attributes().value("dataname");
                    QStringRef tmpqstr = xml.attributes().value("datavalue");
                    QByteArray tmpbarr = tmpqstr.toLocal8Bit();
                    tmpbuf = tmpbarr.data();
#define int(a) if (tmptype=="int"  && tmpname==#a) sscanf(tmpbuf, "%d", &cfg.a);
#define Filename(a) if (tmptype=="Filename"  && tmpname==#a) \
            sscanf(tmpbuf, "%s", cfg.a.path);
#define FontSpec(a) if (tmptype=="FontSpec"  && tmpname==#a) { \
            i = sscanf(tmpbuf, "%d %d %d ", \
                    &cfg.a.isbold, &cfg.a.height, &cfg.a.charset); \
            if (i==3) { \
                char *tmp_fontspec = tmpbuf; \
                /* skip the three spaces to get to fontname */ \
                if (tmp_fontspec) tmp_fontspec = strchr(tmp_fontspec, ' '); \
                if (tmp_fontspec) tmp_fontspec = strchr(tmp_fontspec+1, ' '); \
                if (tmp_fontspec) tmp_fontspec = strchr(tmp_fontspec+1, ' '); \
                if (tmp_fontspec) strncpy(cfg.a.name, tmp_fontspec+1, sizeof(cfg.a.name)); \
            }}
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_int(name, arr) \
            if (tmpname==#name) \
                for(i=0; i<arr; i++) \
                    sscanf(tmpbuf+i*9, "%08X ", &cfg.name[i]);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_short(name, arr) \
            QUTTY_SERIALIZE_ELEMENT_ARRAY_int(name, arr)
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_char(name, arr) \
            if (tmpname==#name) { \
                strncpy(cfg.name, tmpbuf, sizeof(cfg.name)); \
            }
#define QUTTY_SERIALIZE_ELEMENT_ARRAY(t, n, a) QUTTY_SERIALIZE_ELEMENT_ARRAY_##t(n, a);

            QUTTY_SERIALIZE_STRUCT_CONFIG_ELEMENT_LIST

            if (tmptype == "unsigned char" && tmpname == "colours")
                for(i=0; i<22; i++) {
                    int a, b, c;
                    sscanf(tmpbuf+i*9, "%X %X %X ", &a, &b, &c);
                    cfg.colours[i][0] = (uchar)a;
                    cfg.colours[i][1] = (uchar)b;
                    cfg.colours[i][2] = (uchar)c;
                }
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY_short
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY_int
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY_char
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY
#undef int
#undef Filename
#undef FontSpec
                    xml.skipCurrentElement();
                } else {
                    xml.skipCurrentElement();
                }
            }
            config_list[cfg.config_name] = cfg;
        } else if (xml.name() == "sshhostkeys" && xml.attributes().value("version") == "1.0") {
            while (xml.readNextStartElement()) {
                if (xml.name() == "entry") {
                    QStringRef key = xml.attributes().value("datakey");
                    QStringRef val = xml.attributes().value("datavalue");
                    QByteArray key_barr = key.toLocal8Bit();
                    QByteArray val_barr = val.toLocal8Bit();
                    ssh_host_keys[key_barr.constData()] = val_barr.constData();
                    xml.skipCurrentElement();
                } else {
                    xml.skipCurrentElement();
                }
            }
        } else {
            xml.skipCurrentElement();
        }
    }
#endif
    return 0;
}

int QtConfig::writeToXML(QIODevice *device)
{
    QXmlStreamWriter xml;
    char tmpbuf[10240];
    int tmplen;
    xml.setDevice(device);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE qutty>");
    xml.writeStartElement("qutty");
    xml.writeAttribute("version", "1.0");
    for(map<string, Config>::iterator  it=config_list.begin();
        it != config_list.end(); it++) {
        int i;
        Config *cfg = &(it->second);

        xml.writeStartElement("config");
        xml.writeAttribute("version", "1.0");

#define XMLIZE(type, name, value) \
    xml.writeStartElement("dataelement"); \
    xml.writeAttribute("datatype", type); \
    xml.writeAttribute("dataname", name); \
    xml.writeAttribute("datavalue", value); \
    xml.writeEndElement();

#ifdef _WINDOWS
#define int(a) _snprintf(tmpbuf, sizeof(tmpbuf), "%d", cfg->a); XMLIZE("int", #a, tmpbuf);
#define Filename(a) \
        _snprintf(tmpbuf, sizeof(tmpbuf), "%s", cfg->a.path); \
        XMLIZE("Filename", #a, tmpbuf);
#define FontSpec(a) \
        _snprintf(tmpbuf, sizeof(tmpbuf), "%d %d %d %s", \
                cfg->a.isbold, cfg->a.height, cfg->a.charset, cfg->a.name); \
        XMLIZE("FontSpec", #a, tmpbuf);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_int(type, name, arr) \
        tmplen = 0; \
        for(i=0; i<arr; i++) \
            tmplen += _snprintf(tmpbuf+tmplen, sizeof(tmpbuf)-tmplen, \
                                    "%08X ", cfg->name[i]); \
        XMLIZE(#type, #name, tmpbuf);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_short(type, name, arr) \
        QUTTY_SERIALIZE_ELEMENT_ARRAY_int(type, name, arr);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_char(type, name, arr) \
        _snprintf(tmpbuf, sizeof(tmpbuf), "%s", cfg->name); \
        XMLIZE(#type, #name, tmpbuf);
#endif
#define LINUX
#ifdef _LINUX
#define int(a) sprintf(tmpbuf, "%d", cfg->a); XMLIZE("int", #a, tmpbuf);
#define Filename(a) \
        sprintf(tmpbuf, "%s", cfg->a.path); \
        XMLIZE("Filename", #a, tmpbuf);
#define FontSpec(a) \
        sprintf(tmpbuf, "%d %d %d %s", \
                cfg->a.isbold, cfg->a.height, cfg->a.charset, cfg->a.name); \
        XMLIZE("FontSpec", #a, tmpbuf);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_int(type, name, arr) \
        tmplen = 0; \
        for(i=0; i<arr; i++) \
            tmplen += sprintf(tmpbuf+tmplen, \
                                    "%08X ", cfg->name[i]); \
        XMLIZE(#type, #name, tmpbuf);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_short(type, name, arr) \
        QUTTY_SERIALIZE_ELEMENT_ARRAY_int(type, name, arr);
#define QUTTY_SERIALIZE_ELEMENT_ARRAY_char(type, name, arr) \
        sprintf(tmpbuf, "%s", cfg->name); \
        XMLIZE(#type, #name, tmpbuf);

#endif

#ifdef _WINDOWS 
#define QUTTY_SERIALIZE_ELEMENT_ARRAY(t, n, a) QUTTY_SERIALIZE_ELEMENT_ARRAY_##t(t, n, a);

        QUTTY_SERIALIZE_STRUCT_CONFIG_ELEMENT_LIST;

        for(i=0, tmplen=0; i<22; i++)
            tmplen += _snprintf(tmpbuf+tmplen, sizeof(tmpbuf)-tmplen, "%02X %02X %02X ",
                                cfg->colours[i][0], cfg->colours[i][1], cfg->colours[i][2]);
        XMLIZE("unsigned char", "colours", tmpbuf);
#endif
#ifdef _LINUX
#define QUTTY_SERIALIZE_ELEMENT_ARRAY(t, n, a) QUTTY_SERIALIZE_ELEMENT_ARRAY_##t(t, n, a);
        QUTTY_SERIALIZE_STRUCT_CONFIG_ELEMENT_LIST;

        for(i=0, tmplen=0; i<22; i++)
            tmplen += sprintf(tmpbuf+tmplen, "%02X %02X %02X ",
                                cfg->colours[i][0], cfg->colours[i][1], cfg->colours[i][2]);
        XMLIZE("unsigned char", "colours", tmpbuf);
#endif


#undef QUTTY_SERIALIZE_ELEMENT_ARRAY_short
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY_int
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY_char
#undef QUTTY_SERIALIZE_ELEMENT_ARRAY
#undef int
#undef Filename
#undef FontSpec
        xml.writeEndElement();
    }

    xml.writeStartElement("sshhostkeys");
    xml.writeAttribute("version", "1.0");
    for(map<string, string>::iterator  it=ssh_host_keys.begin();
        it != ssh_host_keys.end(); it++) {
        xml.writeStartElement("entry");
        xml.writeAttribute("datakey", it->first.c_str()); \
        xml.writeAttribute("datavalue", it->second.c_str()); \
        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeEndDocument();
    return 0;
}

bool QtConfig::restoreConfig()
{
    config_list.clear();
    QFile file(QDir::home().filePath("qutty.xml"));

    if (!file.exists()) {
        restoreFromPuttyWinRegistry();
    }

    if (!file.exists()) {
        Config cfg;
        initConfigDefaults(&cfg);
        strcpy(cfg.config_name, QUTTY_DEFAULT_CONFIG_SETTINGS);
        qutty_config.config_list[QUTTY_DEFAULT_CONFIG_SETTINGS] = cfg;
        saveConfig();
    }
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(NULL, QObject::tr("Qutty Configuration"),
                             QObject::tr("Cannot read file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    readFromXML(&file);
    emit savedSessionsChanged();
    return true;
}

/*
 * Windows only: Try loading the sessions stored in registry by PUTTY
 */
bool QtConfig::restoreFromPuttyWinRegistry()
{
#ifdef _WINDOWS
    bool rc = true;
    struct sesslist savedSess;
    void *sesskey;
    Config cfg;

    get_sesslist(&savedSess, TRUE);
    qDebug() << "putty nsessions " << savedSess.nsessions;
    for (int i=0; i<savedSess.nsessions; i++) {
        memset(&cfg, 0, sizeof(cfg));

        sesskey = open_settings_r(savedSess.sessions[i]);
        load_open_settings(sesskey, &cfg);
        close_settings_r(sesskey);

        strncpy(cfg.config_name, savedSess.sessions[i], sizeof(cfg.config_name));
        this->config_list[cfg.config_name] = cfg;

        qDebug() << "putty session " << i << " name " << savedSess.sessions[i]
                 << " host " << cfg.host << " port " << cfg.port;
    }

    // load ssh hostkey list from registry
    void *handle = enum_sshhostkey_start();
    uchar hostkey[512], hostkey_val[2048];
    if (handle) {
        while (enum_sshhostkey_next(handle, hostkey, sizeof(hostkey),
                                    hostkey_val, sizeof(hostkey_val))) {
            qutty_config.ssh_host_keys[string((char*)hostkey)] = string((char*)hostkey_val);
        }
        enum_sshhostkey_finish(handle);
    }

    if (savedSess.nsessions > 0) {
        rc = this->saveConfig();
        if (rc) {
            QMessageBox::information(NULL, QObject::tr("Qutty first-time Configuration"),
                         QObject::tr("Automatically loaded %1 saved sessions from PuTTY")
                             .arg(savedSess.nsessions-1));
        } else {
            QMessageBox::warning(NULL, QObject::tr("Qutty first-time Configuration"),
                         QObject::tr("Failed to save %1 saved sessions from PuTTY")
                             .arg(savedSess.nsessions-1));
        }
    }

    get_sesslist(&savedSess, FALSE);    /* free */

    return rc;
#endif
}

bool QtConfig::saveConfig()
{
    QFile file(QDir::home().filePath("qutty.xml"));
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(NULL, QObject::tr("Qutty Configuration"),
                             QObject::tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    writeToXML(&file);
    return true;
}
