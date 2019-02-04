#include "wipechan.h"
#include <cstdlib>

Wipechan::Wipechan () : QFrame() , called(false) {
    quote = new std::string;
    blockquote = new QPlainTextEdit;
}

Wipechan::Wipechan (QWidget * centralWidget) : QFrame(centralWidget) , called(false) {
    quote = new std::string;
    blockquote = new QPlainTextEdit;
}

Wipechan::Wipechan (QWidget * centralWidget, QPlainTextEdit * blockquote) : QFrame(centralWidget) , called(false) {
    quote = new std::string;
    this->blockquote = blockquote;
}

void Wipechan::set_blockquote (QPlainTextEdit * blockquote) {
    this->blockquote = blockquote;
    blockquote->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    set_quote_bank();
}

void Wipechan::set_chan (const std::string &dir) {
    this->dir = dir;
}

void Wipechan::set_quote (const std::string &quote, std::vector<std::string> * bank) {
    std::string row;
    std::ifstream file(QDir::currentPath().toStdString()+"/gui/"+dir+"/quote-bank/"+quote);
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file, row);
            bank->push_back(row);
        }
        bank->pop_back();
        file.close();
    }
}

void Wipechan::set_tips (const std::string &tips, std::map<std::string, std::string> * bank) {
    std::string row;
    unsigned long pos, npos;

    std::ifstream file(QDir::currentPath().toStdString()+"/gui/"+dir+"/quote-bank/"+tips);
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file, row);
            if (!row.empty() && (pos = row.find('\t')) != -1) {
                npos = pos;
                while (row[npos] == '\t') npos++;
                bank->insert(std::pair<std::string, std::string>(row.substr(0, pos), row.substr(npos, row.length())));
            }
        }
        file.close();
        defaultQuote = (*bank)["DEFAULT"];
    }
}

void Wipechan::set_quote_bank () {
    rand();

    set_quote("meet", &quoteBank.meet);

    set_quote("hello/0", &quoteBank.hello[0]);
    set_quote("hello/1", &quoteBank.hello[1]);
    set_quote("hello/2", &quoteBank.hello[2]);
    set_quote("hello/3", &quoteBank.hello[3]);
    set_quote("hello/4", &quoteBank.hello[4]);

    set_quote("start/0", &quoteBank.start[0]);
    set_quote("start/1", &quoteBank.start[1]);
    set_quote("start/2", &quoteBank.start[2]);
    set_quote("start/3", &quoteBank.start[3]);
    set_quote("start/4", &quoteBank.start[4]);

    set_quote("finish/0", &quoteBank.finish[0]);
    set_quote("finish/1", &quoteBank.finish[1]);
    set_quote("finish/2", &quoteBank.finish[2]);
    set_quote("finish/3", &quoteBank.finish[3]);
    set_quote("finish/4", &quoteBank.finish[4]);

    set_quote("error/0", &quoteBank.error[0]);
    set_quote("error/1", &quoteBank.error[1]);
    set_quote("error/2", &quoteBank.error[2]);
    set_quote("error/3", &quoteBank.error[3]);
    set_quote("error/4", &quoteBank.error[4]);

    set_quote("crash/0", &quoteBank.crash[0]);
    set_quote("crash/1", &quoteBank.crash[1]);
    set_quote("crash/2", &quoteBank.crash[2]);
    set_quote("crash/3", &quoteBank.crash[3]);
    set_quote("crash/4", &quoteBank.crash[4]);

    set_tips("tips", &quoteBank.tips);
}



bool Wipechan::is_called () {
    return called;
}

void Wipechan::uncall () {
    hide();
    called = false;
}

void Wipechan::set_height (const unsigned short &num) {
    blockquote->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    unsigned short rowsCount;

    if (num != 0) rowsCount = num;
    else rowsCount = blockquote->document()->size().height();

    if (rowsCount == 0) rowsCount = 1;
    else if (rowsCount < 10) rowsCount++;
    blockquote->resize(blockquote->size().width(), rowsCount * 1.15 * blockquote->font().pixelSize() + 10);
    if (blockquote->geometry().height() > geometry().height() - blockquote->geometry().y()) {
        blockquote->resize(blockquote->size().width(), geometry().height() - blockquote->geometry().y());
        blockquote->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }

    blockquote->moveCursor(QTextCursor::Start);
    blockquote->ensureCursorVisible();
}

void Wipechan::add_quote_part (const std::vector<std::string> &bank, const std::vector<std::string> &params, const std::string &username) {
    rnd = rand() % bank.size();
    add_quote_part(bank[rnd], params, username);
}

void Wipechan::add_quote_part (const std::string &str, const std::vector<std::string> &params, const std::string &username) {
    std::string macro;
    unsigned long npos(0), pos;

    for (pos = str.find("__"); pos != -1; pos = str.find("__", npos)) {
        quote->append(str.substr(npos, pos-npos));
        npos = str.find("__", pos+2);
        npos += 2;
        macro = str.substr(pos, npos-pos);

        if (macro == "__USERNAME__") quote->append(username);
        else if (macro == "__POSTS__") quote->append(params[1]);
        else if (macro == "__BANS__") quote->append(params[2]);
        else if (macro == "__TOTAL_POSTS__") quote->append(params[3]);
    }
    quote->append(str.substr(npos, str.length()));
}



void Wipechan::call (const Request &request, const std::vector<std::string> &params, const std::string &username, const long &relation) {
    quote->clear();

    if (request == MEET) meet(username);
    else if (request == HELLO) hello(username, relation);
    else if (request == START) start(username, relation);
    else if (request == FINISH) finish(params, username, relation);
    else if (request == ERROR) error(params, username, relation);
    else if (request == CRASH) crash(params[0], username, relation);
    else if (request == TIP) tip(params, username);
    else quote->append("\nWait... WHAT!???~~");

    blockquote->setPlainText(quote->c_str());
    set_height(request == MEET ? 26 : 0);
    set_emotion(request, relation);
    show();
    called = true;
}

void Wipechan::meet (const std::string &username) {
    add_quote_part(quoteBank.meet, {}, username);
}

void Wipechan::hello (const std::string &username, const long &relation) {
    if (relation < 1000) add_quote_part(quoteBank.hello[0], {}, username);
    else if (relation < 4000) add_quote_part(quoteBank.hello[1], {}, username);
    else if (relation < 7000) add_quote_part(quoteBank.hello[2], {}, username);
    else if (relation < 10000) add_quote_part(quoteBank.hello[3], {}, username);
    else add_quote_part(quoteBank.hello[4], {}, username);
}

void Wipechan::start (const std::string &username, const long &relation) {
    if (relation < 1000) add_quote_part(quoteBank.start[0], {}, username);
    else if (relation < 4000) add_quote_part(quoteBank.start[1], {}, username);
    else if (relation < 7000) add_quote_part(quoteBank.start[2], {}, username);
    else if (relation < 10000) add_quote_part(quoteBank.start[3], {}, username);
    else add_quote_part(quoteBank.start[4], {}, username);
}

void Wipechan::finish (const std::vector<std::string> &results, const std::string &username, const long &relation) {
    if (results[0] == "0" && results[1] == "0") {
        quote->append("Ты ничего не запостил, "+username);
        return;
    }

    if (relation < 1000) add_quote_part(quoteBank.finish[0], results, username);
    else if (relation < 4000) add_quote_part(quoteBank.finish[1], results, username);
    else if (relation < 7000) add_quote_part(quoteBank.finish[2], results, username);
    else if (relation < 10000) add_quote_part(quoteBank.finish[3], results, username);
    else add_quote_part(quoteBank.finish[4], results, username);
}

void Wipechan::error (const std::vector<std::string> &errors, const std::string &username, const long &relation) {
    rnd = rand() % quoteBank.error[1].size();

    if (relation < 1000) add_quote_part(quoteBank.error[0], {}, username);
    else if (relation < 4000) add_quote_part(quoteBank.error[1], {}, username);
    else if (relation < 7000) add_quote_part(quoteBank.error[2], {}, username);
    else if (relation < 10000) add_quote_part(quoteBank.error[3], {}, username);
    else add_quote_part(quoteBank.error[4], {}, username);
    quote->append("\n");

    for (unsigned int i(0); i < errors.size(); i++)
        quote->append(errors[i] + "\n");
}

void Wipechan::crash (const std::string &reason, const std::string &username, const long &relation) {
    if (relation < 1000) add_quote_part(quoteBank.crash[0], {}, username);
    else if (relation < 4000) add_quote_part(quoteBank.crash[1], {}, username);
    else if (relation < 7000) add_quote_part(quoteBank.crash[2], {}, username);
    else if (relation < 10000) add_quote_part(quoteBank.crash[3], {}, username);
    else add_quote_part(quoteBank.crash[4], {}, username);
    quote->append("\n");
    quote->append(reason);
}

void Wipechan::tip (const std::vector<std::string> &params, const std::string &username) {
    if (quoteBank.tips.find(params[0]) == quoteBank.tips.cend() || quoteBank.tips[params[0]] == "")
        quote->append("\""+params[0]+"\". "+defaultQuote);
    else
        add_quote_part(quoteBank.tips[params[0]], params, username);
}



void Wipechan::set_emotion (const Request &request, const long &relation) {
    switch (request) {
    case MEET: {
        setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/smiling.png); }");
        break;
    }
    case HELLO: {
        if (relation < 1000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/angry.png); }");
        else if (relation < 4000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/sad.png); }");
        else if (relation < 7000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/smiling.png); }");
        else if (relation < 10000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/open_mouth.png); }");
        else setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/kawaii.png); }");
        break;
    }
    case START: {
        setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/catty.png); }");
        break;
    }
    case FINISH: {
        if (relation < 1000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/angry.png); }");
        else if (relation < 4000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/sad.png); }");
        else if (relation < 7000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/smiling.png); }");
        else if (relation < 10000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/open_mouth.png); }");
        else setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/kawaii.png); }");
        break;
    }
    case ERROR: {
        if (relation < 5000) setStyleSheet(styleSheet() + " background-image: url(gui/"+QString(dir.c_str())+"/images/angry.png); }");
        else setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/catty.png); }");
        break;
    }
    case CRASH: {
        if (relation < 5000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/angry.png); }");
        else setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/catty.png); }");
        break;
    }
    case TIP: {
        if (relation < 4000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/sad.png); }");
        else if (relation < 5000) setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/smiling.png); }");
        else setStyleSheet(styleSheet() + "\n#chan { background-image: url(gui/"+QString(dir.c_str())+"/images/open_mouth.png); }");
    }
    }
}
