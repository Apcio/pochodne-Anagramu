#include "worker.h"

Worker::Worker(QObject *p): QObject(p)
{
    cons = new QTextStream(stdout);
    dictPath = "";
    wordAnagram = "";
}

Worker::~Worker()
{
    delete cons;
}

void Worker::run(int &argc, char **argv)
{
    QString *param = nullptr;
    for(int i = 1; i < argc; ++i){
        if(QString::compare(argv[i], "-dic") == 0) {
            param = &dictPath;
            continue;
        }
        if(QString::compare(argv[i], "-word") == 0) {
            param = &wordAnagram;
            continue;
        }
        if(param != nullptr) {
            *param = argv[i];
            param = nullptr;
            continue;
        }
    }

    if(verifyParams() && readDictionary()) {
        findAnagram();
    }

    emit close();
}

bool Worker::verifyParams()
{
    if(dictPath == "") {
        *cons << "Wprowadź słownik" << endl;
        return false;
    }
    QFileInfo fileInfo(dictPath);
    if(!fileInfo.exists() || !fileInfo.isFile() || !fileInfo.isReadable()) {
        *cons << "Brak dostępu do pliku" << endl;
        return false;
    }

    if(wordAnagram == "") {
        *cons << "Nie wczytano anagramu" << endl;
        return false;
    }

    return true;
}

QString Worker::parseDictWord(QString &w)
{
    QRegularExpression re("[a-zA-Z0-9\\p{L}]+");
    QRegularExpressionMatch match = re.match(&w);

    QStringList result = match.capturedTexts();
    result = result.join("").split("", QString::SkipEmptyParts);
    result.sort(Qt::CaseInsensitive);
    return result.join("");
}

bool Worker::readDictionary()
{
    QFile file(dictPath);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) {
        return false;
    }

    QString value;
    QTextStream stream(&file);

    int i = 0;
    while (!stream.atEnd())
    {
        value = stream.readLine();
        dictionary.insert(parseDictWord(value), value);
        if((++i % 9) == 0) {
            *cons << "\rWczytano " << i << " linii";
            cons->flush();
        }
    }

    return true;
}

void Worker::findAnagram()
{
    QString anagram = parseDictWord(wordAnagram);
    *cons << endl << endl << "Szukam anagramu:" << endl << wordAnagram;
    cons->flush();

    QMap<QString, QString>::iterator iterator;
    int currentLength = anagram.count();

    for(iterator = dictionary.begin(); iterator != dictionary.end(); ++iterator) {
        if( (iterator.key().count() == (currentLength + 1)) && (iterator.key().contains(anagram)) ) {
            *cons << "->" << iterator.value();
            cons->flush();
            anagram = iterator.key();
            currentLength = anagram.count();
            iterator = dictionary.begin();
        }
    }
    *cons << endl;
}
