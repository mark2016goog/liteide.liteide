#include "projectfile.h"

#include <QFile>
#include <QDebug>
#include <QFileInfo>

ProjectFile::ProjectFile(QObject *parent) :
        QObject(parent)
{
}

QString ProjectFile::projectName() const
{
    return QFileInfo(_filePath).completeBaseName();
}

QString ProjectFile::projectPath() const
{
    return QFileInfo(_filePath).absolutePath();
}

QString ProjectFile::fileName() const
{
    return QFileInfo(_filePath).fileName();
}

QString ProjectFile::filePath() const
{
    return _filePath;
}


bool ProjectFile::openProject(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    if (!file.isOpen())
        return false;

    QString all = file.readAll();
    all.replace(QRegExp("\\\\[\\s]*[\n\r]+[\\s]*")," ");
    QStringList list = all.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    context.clear();
    foreach(QString line, list) {
        QStringList v = line.split(QRegExp("\\+="),QString::SkipEmptyParts);
        if (v.isEmpty()) {
            v = line.split(QRegExp("="),QString::SkipEmptyParts);
            if (v.count() == 2) {
                QStringList v2 = v.at(1).split("",QString::SkipEmptyParts);
                if (!v2.isEmpty())
                    context[v.at(0).trimmed()] = v2;
            }
        } else if (v.count() == 2) {
            QStringList v2 = v.at(1).split(" ",QString::SkipEmptyParts);
            if (!v2.isEmpty())
                context[v.at(0).trimmed()].append(v2);
        }
    }
    file.close();
    _filePath = fileName;
    return !context.isEmpty();
}

void ProjectFile::close()
{
}

QStringList ProjectFile::values(const QString &key)
{
    return context.value(key);
}

QStringList ProjectFile::headerFiles()
{
    return values("HEADERS");
}

QStringList ProjectFile::sourceFiles()
{
    return values("SOURCES");
}