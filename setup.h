#ifndef SETUP_H
#define SETUP_H
#include <iostream>
#include <vector>
using namespace std;

class Setup {
private:
//    static const vector<string> source;

    string username;  // имя пользователя
    string password;  // пароль для получения казённого ключа
    string board;  // доска
    string thread;  // тред
    string chaos;  // флаг хаоса и тред для постинга
    string potocksCount;  // число потоков
    char logMode;  // режим логов
    string solver, key;  // номер решателя и ключ
    string proxyRepeatsCount;  // число повторов прокси
    string mode;  // режим вайпалки
    string minBan, maxBan;  // наименьший и наибольший номера банов
    string complainBoard;  // доска для жалоб на вайп
    unsigned char complainsCount;  // число ссылок в одной жалобе
    bool complainWithPosts;  // жалобы со ссылками на посты
    string triggerForm;  // режим триггера
    string shrapnelCharge, minPostsCount;  // число тредов для шрапнели
    char mediaKind;  // тип прикреплений
    string mediaGroup, mediasCount;  // подкаталог прикреплений и их число на пост
    char sageMode;  // режим сажи
    unsigned char shackalPower; // уровень шакала
    bool shackalColor, shackalAffine;  // опции шакала
    bool toPNG;  // преобразование пикч в PNG
    vector<string> shrapnelThreads;  // треды для шрапнели при ручном указании

    bool hasBoard, hasThread, hasChaos, hasPotocksCount, hasLogMode, hasSolver, hasKey, hasProxyRepeatsCount, hasMode, hasMinBan, hasMaxBan, \
        hasTriggerForm, hasShrapnelCharge, hasMinPostsCount, hasMediaKind, hasMediaGroup, hasMediasCount, hasSageMode, hasShackalPower, \
        hasComplainBoard, hasComplainsCount;

    static const string DIGITS;
    bool is_digit(const char &c);
    bool is_num(const string &str);
    bool is_pos_num(const string &str);
    bool is_zero(const string &str);

//    string decode(const string &base64DKOTI);
    string comline();

public:
    Setup();
    ~Setup();

    void set_username(const string &username="");
    void set_password(const string &password="");
    void set_board(const string &board="b");
    void set_thread(const string &thread="0");
    void set_chaos(const string &chaos="-1");
    void set_potocksCount(const string &potocksCount="0");
    void set_logMode(const char &logMode='0');
    void set_solver(const string &solver="0");
    void set_key(const string &key);
    void set_proxyRepeatsCount(const string &proxyRepeatsCount="10");
    void set_mode(const string &mode);
    void set_minBan(const string &minBan);
    void set_maxBan(const string &maxBan);
    void set_triggerForm(const string &triggerForm="0");
    void set_shrapnelCharge(const string &shrapnelCharge);
    void set_minPostsCount(const string &minPostsCount="0");
    void set_mediaKind(const char &mediaKind);
    void set_mediaGroup(const string &mediaGroup="");
    void set_mediasCount(const string &mediasCount="1");
    void set_sageMode(const char &sageMode='0');
    void set_shackal_power(const unsigned char &value);
    void set_shackal_color(const bool &status);
    void set_shackal_affine(const bool &status);
    void set_2PNG(const bool &status);
    void set_shrapnelThreads(const vector<string> &shrapnelThreads);
    void set_complain_board(const string &complainBoard="b");
    void set_complains_count(const int &complainsCount=1);
    void set_complains_with_posts(const bool &status);

    void clear();
    vector<string> validate();
    unsigned char complete();
    vector<string> start();
};

#endif // SETUP_H
