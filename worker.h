/*!
 * Worker class - is used for read program parameters and verify them.
 * After veryfing program parameters, it start to read dictionary from selected file to memory
 * and save it to map variable called dictionary, where key is sorted word, and value is word from file.
 *
 * Algorithm search anagram by iterating over map dictionary and searching word N+1 and checking the word is containing all chars from previous word.
 *
 * @cons - variable holds pointer to console output
 * @dictPath - variable holds absolute path to dictionary file, path is readed from program parameter called -slw
 * @wordAnagram - variable holds words passed by program parameter called -word
 * @dictionary - holds words readed from dictionary file, it contain key as sorted word and value as original word
 */

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QTextStream>
#include <QString>
#include <QPair>
#include <QFileInfo>
#include <QMap>
#include <QMapIterator>
#include <QRegularExpression>

class Worker: public QObject
{
    Q_OBJECT

private:
    QTextStream *cons;
    QString dictPath;
    QString wordAnagram;
    QMap<QString, QString> dictionary;

    bool verifyParams();
    bool readDictionary();
    QString parseDictWord(QString &w);

    void findAnagram();

public:
    Worker(QObject *p = nullptr);
    ~Worker();

public slots:
    void run(int &argc, char **argv);

signals:
    void close();
};

#endif // WORKER_H
