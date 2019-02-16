#include "setup.h"
#include <stdlib.h>
#include <unistd.h>

const string Setup::DIGITS = "0123456789";

Setup::Setup ()
    : hasBoard(false) , hasThread(false) , hasChaos(false) , hasPotocksCount(false) , hasLogMode(false) , hasSolver(false) , hasKey(false) , \
      hasProxyRepeatsCount(false) , hasMode(false) , hasMinBan(false) , hasMaxBan(false) , hasTriggerForm(false) , \
      hasShrapnelCharge(false) , hasMinPostsCount(false) , hasMediaKind(false) , hasMediaGroup(false) , \
      hasMediasCount(false) , hasSageMode(false) , hasShackalPower(false) , shackalColor(false) , shackalAffine(false) , \
      toPNG(false) , hasComplainBoard(false) , hasComplainsCount(false) , complainWithPosts(false) , randMediaName(false)
{}

Setup::~Setup () {}



bool Setup::is_digit (const char &c) {
    return DIGITS.find(c) != string::npos;
}

bool Setup::is_num (const string &str) {
    return !str.empty() && \
            ( (str.size() == 1 && is_digit(str[0])) || \
            ((str[0] == '-' || is_digit(str[0])) && is_pos_num(str.substr(1))) );
}

bool Setup::is_pos_num (const string &str) {
    if (str.empty()) return false;
    for (unsigned int pos(0); pos < str.length(); pos++)
        if (!is_digit(str[pos])) return false;
    return true;
}

bool Setup::is_zero (const string &str) {
    return str.size() == 1 && str[0] == '0';
}




void Setup::set_username (const string &username) {
    this->username = username;
}

void Setup::set_password (const string &password) {
    this->password = password;
}

void Setup::set_board (const string &board) {
    if (board.length() != 0) {
        this->board = board;
        hasBoard = true;
    } else
        hasBoard = false;
}
void Setup::set_thread (const string &thread) {
    if (is_pos_num(thread)) {
        this->thread = thread;
        hasThread = true;
    } else
        hasThread = false;
}
void Setup::set_chaos (const string &chaos) {
    if (is_num(chaos)) {
        this->chaos = chaos;
        hasChaos = true;
    } else
        hasChaos = false;
}
void Setup::set_potocksCount (const string &potocksCount) {
    if (is_pos_num(potocksCount)) {
        this->potocksCount = potocksCount;
        hasPotocksCount = true;
    } else
        hasPotocksCount = false;
}
void Setup::set_logMode (const char &logMode) {
    if (DIGITS.find(logMode) != string::npos) {
        this->logMode = logMode;
        hasLogMode = true;
    } else
        hasLogMode = false;
}
void Setup::set_solver (const string &solver) {
    if (is_pos_num(solver)) {
        this->solver = solver;
        hasSolver = true;
    } else
        hasSolver = false;
}
void Setup::set_key (const string &key) {
    if (key.empty() || key.length() == 32) {
        this->key = key;
        hasKey = true;
    } else
        hasKey = false;
}
void Setup::set_proxyRepeatsCount (const string &proxyRepeatsCount) {
    if (is_pos_num(proxyRepeatsCount) && !is_zero(proxyRepeatsCount)) {
        this->proxyRepeatsCount = proxyRepeatsCount;
        hasProxyRepeatsCount = true;
    } else
        hasProxyRepeatsCount = false;
}
void Setup::set_mode (const string &mode) {
    if (is_pos_num(mode)) {
        this->mode = mode;
        hasMode = true;
    } else
        hasMode = false;
}
void Setup::set_minBan (const string &minBan) {
    if (is_pos_num(minBan)) {
        this->minBan = minBan;
        hasMinBan = true;
    } else
        hasMinBan = false;
}
void Setup::set_maxBan (const string &maxBan) {
    if (is_pos_num(maxBan)) {
        this->maxBan = maxBan;
        hasMaxBan = true;
    } else
        hasMaxBan = false;
}
void Setup::set_triggerForm (const string &triggerForm) {
    if (is_pos_num(triggerForm)) {
        this->triggerForm = triggerForm;
        hasTriggerForm = true;
    } else
        hasTriggerForm = false;
}
void Setup::set_shrapnelCharge (const string &shrapnelCharge) {
    if (is_pos_num(shrapnelCharge)) {
        this->shrapnelCharge = shrapnelCharge;
        hasShrapnelCharge = true;
    } else
        hasShrapnelCharge = false;
}
void Setup::set_minPostsCount (const string &minPostsCount) {
    if (is_pos_num(minPostsCount)) {
        this->minPostsCount = minPostsCount;
        hasMinPostsCount = true;
    } else
        hasMinPostsCount = false;
}
void Setup::set_mediaKind (const char &mediaKind) {
    if (DIGITS.find(mediaKind) != string::npos) {
        this->mediaKind = mediaKind;
        hasMediaKind = true;
    } else
        hasMediaKind = false;
}
void Setup::set_mediaGroup (const string &mediaGroup) {
    if (!mediaGroup.empty()) {
        this->mediaGroup = mediaGroup;
        hasMediaGroup = true;
    } else
        hasMediaGroup = false;
}
void Setup::set_mediasCount (const string &mediasCount) {
    if (is_pos_num(mediasCount)) {
        this->mediasCount = mediasCount;
        hasMediasCount = true;
    }
}
void Setup::set_sageMode (const char &sageMode) {
    if (DIGITS.find(sageMode) != string::npos) {
        this->sageMode = sageMode;
        hasSageMode = true;
    } else
        hasSageMode = false;
}
void Setup::set_rand_media_name (const bool &status) {
    randMediaName = status;
}
void Setup::set_shackal_power (const unsigned char &value) {
    shackalPower = value;
}
void Setup::set_shackal_color (const bool &status) {
    shackalColor = status;
}
void Setup::set_shackal_affine (const bool &status) {
    shackalAffine = status;
}
void Setup::set_2PNG (const bool &status) {
    toPNG = status;
}
void Setup::set_shrapnelThreads (const vector<string> &shrapnelThreads) {
    if (!this->shrapnelThreads.empty()) this->shrapnelThreads.clear();
    this->shrapnelThreads.reserve(shrapnelThreads.size());
    this->shrapnelThreads = shrapnelThreads;
}
void Setup::set_complain_board (const string &complainBoard) {
    if (complainBoard.length() != 0) {
        this->complainBoard = board;
        hasComplainBoard = true;
    } else
        hasComplainBoard = false;
}
void Setup::set_complains_count (const int &complainsCount) {
    if (complainsCount > 0 && complainsCount < 51) {
        this->complainsCount = complainsCount;
        hasComplainsCount = true;
    } else
        hasComplainsCount = false;
}
void Setup::set_complains_with_posts (const bool &status) {
    complainWithPosts = status;
}



void Setup::clear () {
    hasBoard = hasThread = hasChaos = hasPotocksCount = hasLogMode = hasSolver = hasKey = hasProxyRepeatsCount = hasMode = hasMinBan =
    hasMaxBan = hasTriggerForm = hasShrapnelCharge = hasMinPostsCount = hasMediaKind = hasMediaGroup = hasMediasCount =
    hasSageMode =  hasShackalPower = shackalColor = shackalAffine = toPNG = hasComplainBoard = hasComplainsCount = complainWithPosts = false;
}



vector<string> Setup::validate () {
    vector<string> errors;

    if (!hasBoard) errors.push_back("Доска не задана!");
    if (!hasThread) errors.push_back("Цель не задана!");
    if (!hasPotocksCount) errors.push_back("Число потоков не задано!");
    if (!hasSolver) errors.push_back("Решатель не задан!");
    if (key == "" && (username == "" || password == "")) errors.push_back("Для получения казённого ключа необходимо залогиниться!");
    if (!hasProxyRepeatsCount) errors.push_back("Число повторов прокси не задано!");
    if (!hasMode) errors.push_back("Режим вайпалки не задан!");
    if (!hasSageMode) errors.push_back("Режим сажи не задан!");
    if (!hasTriggerForm) errors.push_back("Режим триггера не задан!");
    if (!hasKey) errors.push_back("Неправильно введён ключ!");
    if (mode == "8") {
        if (!hasMinBan) errors.push_back("Не задан нижний номер банов!");
        if (!hasMaxBan) errors.push_back("Не задан верхний номер банов!");
    }
    if (thread == "1" && shrapnelCharge == "0" && shrapnelThreads.size() == 0) errors.push_back("Не заданы треды для шрапнели!");
    if ((mediaKind == '1' || mediaKind == '2') && !hasMediasCount) errors.push_back("Не задано число прикреплений!");
    if (board == "d" && mediasCount != "0") errors.push_back("Прикрепления в /d/!");
    if (thread == "0" && (mode == "3" || mode == "7")) errors.push_back("Этим режимом нельзя вайпать нулевую!");
    if (thread == "0" && triggerForm != "0") errors.push_back("Триггер на нулевой!");
    if (thread == "0" && chaos != "-1") errors.push_back("Нельзя устраивать хаос с нулевой!");
    if (shrapnelCharge == "0" && chaos == "0") errors.push_back("Для полного хаоса требуется шрапнель!");
    if (thread == "0" && sageMode == '2') errors.push_back("Нельзя копировать сажу с нулевой!");
    if (thread == "0" && mediaKind == '3') errors.push_back("Нельзя копировать прикрепления с нулевой!");
    if (mode == "2" && triggerForm == "0" && (mediaKind == '0' || ((mediaKind == '1' || mediaKind == '2') && mediasCount == "0"))) errors.push_back("Ничего не запостится!");

    if (errors.size() == 0) errors.push_back("OK");

    return errors;
}

unsigned char Setup::complete () {
    unsigned char autoCompCount(0);

    if (!hasThread) set_thread("0");
    else autoCompCount++;
    if (!hasPotocksCount) set_potocksCount("0");
    else autoCompCount++;
    if (!hasLogMode) set_logMode('0');
    else autoCompCount++;
    if (!hasProxyRepeatsCount) set_proxyRepeatsCount("50");
    else autoCompCount++;
    if (!hasTriggerForm) set_triggerForm("0");
    else autoCompCount++;
    if (!hasChaos) set_chaos("-1");
    else autoCompCount++;
    if (!hasShrapnelCharge) set_shrapnelCharge("0");
    else autoCompCount++;
    if (!hasMinPostsCount) set_minPostsCount("0");
    else autoCompCount++;
    if (!hasMediasCount) {
        if (board == "d") set_mediasCount("0");
        else set_mediasCount("1");
    }
    else autoCompCount++;
    if (!hasSageMode) set_sageMode('0');
    else autoCompCount++;
    if (!hasComplainBoard) set_complain_board("b");
    else autoCompCount++;
    if (!hasComplainsCount) set_complains_count(10);
    else autoCompCount++;

    return autoCompCount;
}

vector<string> Setup::start () {
    complete();
    vector<string> errors = validate();
    if (errors[0] == "OK") {
        string command(comline());
        system(command.c_str());
    }
    return errors;
}



string Setup::comline () {
#ifdef __linux__
    string command("python3 engine/main.py ");
#else
    string command("cmd /u /k python engine/main.py ");
#endif

    command += ("-u \"" + username + "\" ");
    command += ("--password \"" + password + "\" ");

    command += ("-b " + board + " -t " + thread + " -c " + chaos + " -p " + potocksCount + " -d " + logMode);
    command += logMode;

    command += (" -s " + solver + " -k ");
    if (!hasKey || key == "") command += "0 -r ";
    else command += (key + " -r ");
    command += (proxyRepeatsCount + " -m " + mode + " ");

    if (hasComplainBoard) command += ("-cb " + complainBoard + " ");
    else if (hasMinBan) command += ("--minBan " + minBan + " ");
    else command += "--minBan -1 ";
    if (hasComplainsCount) command += ("-l " + to_string(complainsCount) + " ");
    else if (hasMaxBan) command += ("--maxBan " + maxBan + " ");
    else command += "--maxBan -1 ";

    if (mode == "1") {
        if (complainWithPosts) command += "-w 1 ";
        else command += "-w 0 ";
    } else {
        if (!hasTriggerForm) command += "-T -1 ";
        else command += ("-T " + triggerForm + " ");
    }

    if (!hasShrapnelCharge) command += "-sh 0 ";
    else command += ("-sh " + shrapnelCharge + " ");
    if (!hasMinPostsCount) command += "-mp -1 -M ";
    else command += ("-mp " + minPostsCount + " -M ");

    if (!hasMediaKind) command += "0";
    else command += mediaKind;
    command += " ";
    if (!hasMediaGroup) command += "-g . ";
    else command += ("-g " + mediaGroup + " ");
    if (!hasMediasCount) command += "-mc 1 ";
    else command += ("-mc " + mediasCount + " -S ");

    command += sageMode;

    command += " -rn ";
    if (randMediaName) command += "1";
    else command += "0";

    command += (" -SH " + to_string(shackalPower) + " ");
    if (shackalColor) command += "-C 1 ";
    else command += "-C 0 ";
    if (shackalAffine) command += "-a 1 ";
    else command += "-a 0 ";
    if (toPNG) command += "-P 1 ";
    else command += "-P 0 ";

    if (hasShrapnelCharge && !hasMinPostsCount)
        for (unsigned int i(0); i < shrapnelThreads.size(); i++)
            command += (" " + shrapnelThreads[i]);

///    cout << command << endl << endl << endl << endl;
    return command;
}
