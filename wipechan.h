#ifndef WIPECHAN_H
#define WIPECHAN_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QDir>
#include <iostream>
#include <fstream>
#include <map>

class Wipechan : public QFrame {
public:
    enum Request { MEET = 0, HELLO, START, FINISH, ERROR, CRASH, TIP };

private:
    struct {
        std::vector<std::string> meet;
        std::vector<std::string> hello[5];
        std::vector<std::string> start[5];
        std::vector<std::string> finish[5];
        std::vector<std::string> error[5];
        std::vector<std::string> crash[5];
        std::map<std::string, std::string> tips;
    } quoteBank;

    QPlainTextEdit * blockquote;
    std::string * quote;
    std::string dir;
    std::string defaultQuote;

    bool called;
    unsigned int rnd;

    void set_height(const unsigned short &num=0);
    void set_quote(const std::string &quote, std::vector<std::string> * bank);
    void set_tips(const std::string &tips, std::map<std::string, std::string> * bank);
    void set_quote_bank();
    void add_quote_part(const std::vector<std::string> &bank, const std::vector<std::string> &params, const std::string &username="");
    void add_quote_part(const std::string &str, const std::vector<std::string> &params, const std::string &username="");
    void set_emotion(const Request &request, const long &relation=5000);

    void meet(const std::string &username);
    void hello(const std::string &username, const long &relation);
    void start(const std::string &username, const long &relation);
    void finish(const std::vector<std::string> &results, const std::string &username, const long &relation);
    void error(const std::vector<std::string> &errors, const std::string &username, const long &relation);
    void crash(const std::string &reason, const std::string &username, const long &relation);
    void tip(const std::vector<std::string> &params, const std::string &username);


public:
    Wipechan();
    Wipechan(QWidget * centralWidget);
    Wipechan(QWidget * centralWidget, QPlainTextEdit * blockquote);
    void set_blockquote(QPlainTextEdit * blockquote);
    void set_chan(const std::string &dir);

    void call(const Request &request, const std::vector<std::string> &params={}, const std::string &username="Аноним", const long &relation=5000);
    void uncall();
    bool is_called();
};

#endif // WIPECHAN_H
