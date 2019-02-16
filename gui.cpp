#include "gui.h"
#include "ui_gui.h"
#include <cstdio>
#include <stdlib.h>
#include <QLayout>
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QMessageBox>
#include <QDebug>
#include "rapidjson/document.h"

GUI::GUI (QWidget * parent)
    : QMainWindow(parent) , ui(new Ui::GUI) , started(false) , postsCount("0")
{
    ui->setupUi(this);
    fileDialog = new QFileDialog;

    QStringList fonts = QDir(QDir::currentPath()+"/gui/fonts").entryList();
    for (unsigned int i(0); i < fonts.size(); i++)
        QFontDatabase::addApplicationFont(QString(QDir::currentPath()+"/gui/fonts/"+fonts[i]));
    QFontDatabase db;
    for (unsigned short i(0); i < db.families().size(); i++)
        std::cout << db.families()[i].toStdString() << std::endl;

    std::string path;
    std::ifstream theme(QDir::currentPath().toStdString()+"/gui/theme");
    getline(theme, path);
    set_layout(QDir::currentPath().toStdString()+"/gui/"+path);
    getline(theme, path);
    QFile style;
    style.setFileName(QDir::currentPath()+"/gui/"+path.c_str());
    style.open(QFile::ReadOnly);
    QString QSS = style.readAll();
    setStyleSheet("QWidget { border: none; }\n"+QSS);
    getline(theme, path);
    music = new QSound(QDir::currentPath()+"/gui/"+path.c_str());
    getline(theme, path);
    anima = new QMovie(QDir::currentPath()+"/gui/"+path.c_str());
    ui->anima->setMovie(anima);
    getline(theme, path);
    ui->chan->set_chan(path);

    ui->chan->set_blockquote(ui->blockquote);
    ui->chan->uncall();
    install_filter();

    isPidor = false;
    isPlaying = false;
    ui->rollUpButton->hide();
    ui->closeButton->hide();

    loadData();
}

GUI::~GUI () {
    delete ui;
}


void GUI::set_layout (std::string path) {
    const char * members[] = {"GUI",
    "top_banner", "help_button", "peedor_bar_button", "music_button", "open_config_button", "save_config_button", "username_label", "username", "password_label", "password", "roll_up_button", "close_button",

    "main_set", "main_set_header",
    "mode_box", "mode_label", "main_mode", "shrapnel_mode",
    "main_box", "board_label", "board", "thread_label", "thread", "fibers_label", "fibers",
    "shrapnel_box", "shrapnel_board_label", "shrapnel_board", "threads_label", "threads", "min_posts_label", "min_posts", "shrapnel_fibers_label", "shrapnel_fibers",

    "solver_set", "solver_set_header",
    "solver_box", "solver_label", "anticaptcha", "guro-captcha", "x-captcha", "reserved",
    "key_box", "key_label", "key", "server_key",

    "post_set", "post_set_header",
    "repeats_label", "repeats",
    "text_box", "text_label", "no_text", "quoting", "copying", "overload", "pastes", "complain", "random",
    "trigger_box", "trigger_label", "chaining", "random_trigger", "shashlik", "op-post_trigger", "no_trigger",
    "complain_box", "complain_label", "complain_board_label", "complain_board", "links_count_label", "links_count", "with_posts",
    "chaos_box", "chaos_label", "chaos", "random_chaos", "target_label", "target",
    "sage", "sage_box", "sage_always", "sage_from_posts",

    "media_set", "media_set_header",
    "media_box", "media_label", "images", "images_count_label", "images_count", "images_folder_label", "images_folder", "videos", "videos_count_label", "videos_count", "videos_folder_label", "videos_folder", "medias_from_posts", "no_media",
    "random_media_name", "shackal_box", "shackal_label", "shackal_degree_label", "shackal_degree", "colorization", "affine", "to_PNG",

    "stats", "stats_header", "total_posts_label", "total_posts", "total_posts_LCD", "total_bans_label", "total_bans", "total_bans_LCD", "posts_label", "posts", "posts_LCD", "proxies_label", "proxies", "proxies_LCD",

    "Wipe-chan", "blockquote", "back_banner", "anima", "start_button"
    };
    std::string text, toolTip;
    int x, y, width, height, value, digitsCount;
    bool hiding, hidden, disable, disabled, checked, vertical;
    QWidget * widget;  QLabel * label;  QPushButton * button;  QCheckBox * checkBox;  QRadioButton * radioButton;  QLineEdit * edit;  QSpinBox * spin; QComboBox * combo; QLCDNumber * LCD; QSlider * slider;
    char type;
    rapidjson::Document JSON;
    rapidjson::Value member;

    std::ifstream layoutFile(path.c_str());
    layoutFile.seekg(0, std::ios::end);
    size_t size = layoutFile.tellg();
    std::string layout(size, ' ');
    layoutFile.seekg(0);
    layoutFile.read(&layout[0], size);

    if (!JSON.Parse(layout.c_str()).HasParseError()) {
        for (size_t i(0); i < sizeof(members)/sizeof(members[0]); i++) {
            if (JSON.HasMember(members[i]) && JSON[members[i]].IsObject()) {
                member = JSON[members[i]].GetObject();

                if (member.HasMember("x") && member["x"].IsInt()) x = member["x"].GetInt(); else x = 0;
                if (member.HasMember("y") && member["y"].IsInt()) y = member["y"].GetInt(); else y = 0;
                if (member.HasMember("width") && member["width"].IsInt()) width = member["width"].GetInt(); else width = 0;
                if (member.HasMember("height") && member["height"].IsInt()) height = member["height"].GetInt(); else height = 0;
                if (member.HasMember("text") && member["text"].IsString()) text = member["text"].GetString(); else text = "";
                if (member.HasMember("value") && member["value"].IsInt()) value = member["value"].GetInt(); else value = 0;
                if (member.HasMember("tip") && member["tip"].IsString()) toolTip = member["tip"].GetString(); else toolTip = "";
                if (member.HasMember("digits_count") && member["digits_count"].IsInt()) digitsCount = member["digits_count"].GetInt(); else digitsCount = 0;
                if (member.HasMember("checked") && member["checked"].IsBool()) checked = member["checked"].GetBool(); else checked = false;
                if (member.HasMember("vertical") && member["vertical"].IsBool()) vertical = member["vertical"].GetBool(); else vertical = false;
                if (member.HasMember("disable") && member["disable"].IsBool()) disable = member["disable"].GetBool(); else disable = false;
                if (member.HasMember("disabled") && member["disabled"].IsBool()) disabled = member["disabled"].GetBool(); else disabled = false;
                if (member.HasMember("hiding") && member["hiding"].IsBool()) hiding = member["hiding"].GetBool(); else hiding = false;
                if (member.HasMember("hidden") && member["hidden"].IsBool()) hidden = member["hidden"].GetBool(); else hidden = false;

                if (i == 0) {
                    setMinimumSize(width, height);
                    setMaximumSize(width, height);
                    setGeometry(
                        QStyle::alignedRect(
                            Qt::LeftToRight,
                            Qt::AlignCenter,
                            QSize(width, height),
                            qApp->desktop()->availableGeometry()
                        )
                    );
                } else {
                    if (members[i] == "top_banner") { widget = label = ui->topBanner; type = 'l'; }
                    else if (members[i] == "help_button") { widget = button = ui->helpButton; type = 'b'; }
                    else if (members[i] == "peedor_bar_button") { widget = button = ui->peedorBarButton; type = 'b'; }
                    else if (members[i] == "music_button") { widget = button = ui->musicButton; type = 'b'; }
                    else if (members[i] == "open_config_button") { widget = button = ui->openButton; type = 'b'; }
                    else if (members[i] == "save_config_button") { widget = button = ui->saveButton; type = 'b'; }
                    else if (members[i] == "username_label") { widget = label = ui->usernameLabel; type = 'l'; }
                    else if (members[i] == "username") { widget = edit = ui->username; type = 'e'; }
                    else if (members[i] == "password_label") { widget = label = ui->passwordLabel; type = 'l'; }
                    else if (members[i] == "password") { widget = edit = ui->password; type = 'e'; }
                    else if (members[i] == "roll_up_button") { widget = button = ui->rollUpButton; type = 'b'; }
                    else if (members[i] == "close_button") { widget = button = ui->closeButton; type = 'b'; }


                    else if (members[i] == "main_set") { widget = ui->mainSet; type = 'w'; }
                    else if (members[i] == "main_set_header") { widget = label = ui->mainSetHeader; type = 'l'; }

                    else if (members[i] == "mode_box") { widget = ui->modeBox; type = 'w'; }
                    else if (members[i] == "mode_label") { widget = label = ui->modeLabel; type = 'l'; }
                    else if (members[i] == "main_mode") { widget = radioButton = ui->mainMode; type = 'r'; }
                    else if (members[i] == "shrapnel_mode") { widget = radioButton = ui->shrapnelMode; type = 'r'; }

                    else if (members[i] == "main_box") { widget = ui->mainBox; type = 'w'; }
                    else if (members[i] == "board_label") { widget = label = ui->boardLabel; type = 'l'; }
                    else if (members[i] == "board") { widget = combo = ui->board; type = 'm'; }
                    else if (members[i] == "thread_label") { widget = label = ui->threadLabel; type = 'l'; }
                    else if (members[i] == "thread") { widget = edit = ui->thread; type = 'e'; }
                    else if (members[i] == "fibers_label") { widget = label = ui->fibersLabel; type = 'l'; }
                    else if (members[i] == "fibers") { widget = edit = ui->fibers; type = 'e'; }

                    else if (members[i] == "shrapnel_box") { widget = ui->shrapnelBox; type = 'w'; }
                    else if (members[i] == "shrapnel_board_label") { widget = label = ui->shrapnelBoardLabel; type = 'l'; }
                    else if (members[i] == "shrapnel_board") { widget = combo = ui->shrapnelBoard; type = 'm'; }
                    else if (members[i] == "threads_label") { widget = label = ui->threadsLabel; type = 'l'; }
                    else if (members[i] == "threads") { widget = edit = ui->threads; type = 'e'; }
                    else if (members[i] == "min_posts_label") { widget = label = ui->minPostsLabel; type = 'l'; }
                    else if (members[i] == "min_posts") { widget = edit = ui->minPosts; type = 'e'; }
                    else if (members[i] == "shrapnel_fibers_label") { widget = label = ui->shrapnelFibersLabel; type = 'l'; }
                    else if (members[i] == "shrapnel_fibers") { widget = edit = ui->shrapnelFibers; type = 'e'; }


                    else if (members[i] == "solver_set") { widget = ui->solverSet; type = 'w'; }
                    else if (members[i] == "solver_set_header") { widget = label = ui->solverSetHeader; type = 'l'; }

                    else if (members[i] == "solver_box") { widget = ui->solverBox; type = 'w'; }
                    else if (members[i] == "solver_label") { widget = label = ui->solverLabel; type = 'l'; }
                    else if (members[i] == "anticaptcha") { widget = radioButton = ui->anticaptcha; type = 'r'; }
                    else if (members[i] == "guro-captcha") { widget = radioButton = ui->gurocaptcha; type = 'r'; }
                    else if (members[i] == "x-captcha") { widget = radioButton = ui->x_captcha; type = 'r'; }
                    else if (members[i] == "reserved") { widget = radioButton = ui->reserved; type = 'r'; }

                    else if (members[i] == "key_box") { widget = ui->keyBox; type = 'w'; }
                    else if (members[i] == "key_label") { widget = label = ui->keyLabel; type = 'l'; }
                    else if (members[i] == "key") { widget = edit = ui->key; type = 'e'; }
                    else if (members[i] == "server_key") { widget = checkBox = ui->serverKey; type = 'c'; }


                    else if (members[i] == "post_set") { widget = ui->postSet; type = 'w'; }
                    else if (members[i] == "post_set_header") { widget = label = ui->postSetHeader; type = 'l'; }

                    else if (members[i] == "repeats_label") { widget = label = ui->repeatsLabel; type = 'l'; }
                    else if (members[i] == "repeats") { widget = spin = ui->repeats; type = 's'; }

                    else if (members[i] == "text_box") { widget = ui->textBox; type = 'w'; }
                    else if (members[i] == "text_label") { widget = label = ui->textLabel; type = 'l'; }
                    else if (members[i] == "no_text") { widget = radioButton = ui->noText; type = 'r'; }
                    else if (members[i] == "quoting") { widget = radioButton = ui->quoting; type = 'r'; }
                    else if (members[i] == "copying") { widget = radioButton = ui->copying; type = 'r'; }
                    else if (members[i] == "overload") { widget = radioButton = ui->overload; type = 'r'; }
                    else if (members[i] == "pastes") { widget = radioButton = ui->pastes; type = 'r'; }
                    else if (members[i] == "complain") { widget = radioButton = ui->complain; type = 'r'; }
                    else if (members[i] == "random") { widget = radioButton = ui->random; type = 'r'; }

                    else if (members[i] == "trigger_box") { widget = ui->triggerBox; type = 'w'; }
                    else if (members[i] == "trigger_label") { widget = label = ui->triggerLabel; type = 'l'; }
                    else if (members[i] == "chaining") { widget = radioButton = ui->chaining; type = 'r'; }
                    else if (members[i] == "random_trigger") { widget = radioButton = ui->randTrigger; type = 'r'; }
                    else if (members[i] == "shashlik") { widget = radioButton = ui->shashlik; type = 'r'; }
                    else if (members[i] == "op-post_trigger") { widget = radioButton = ui->oppost; type = 'r'; }
                    else if (members[i] == "no_trigger") { widget = radioButton = ui->notrigger; type = 'r'; }

                    else if (members[i] == "complain_box") { widget = ui->complainBox; type = 'w'; }
                    else if (members[i] == "complain_label") { widget = label = ui->complainLabel; type = 'l'; }
                    else if (members[i] == "complain_board_label") { widget = label = ui->complainBoardLabel; type = 'l'; }
                    else if (members[i] == "complain_board") { widget = combo = ui->complainBoard; type = 'm'; }
                    else if (members[i] == "links_count_label") { widget = label = ui->linksCountLabel; type = 'l'; }
                    else if (members[i] == "links_count") { widget = spin = ui->linksCount; type = 's'; }
                    else if (members[i] == "with_posts") { widget = checkBox = ui->withPosts; type = 'c'; }

                    else if (members[i] == "chaos_box") { widget = ui->chaosBox; type = 'w'; }
                    else if (members[i] == "chaos_label") { widget = label = ui->chaosLabel; type = 'l'; }
                    else if (members[i] == "chaos") { widget = checkBox = ui->chaos; type = 'c'; }
                    else if (members[i] == "random_chaos") { widget = checkBox = ui->randChaos; type = 'c'; }
                    else if (members[i] == "target_label") { widget = label = ui->targetLabel; type = 'l'; }
                    else if (members[i] == "target") { widget = edit = ui->target; type = 'e'; }

                    else if (members[i] == "sage") { widget = checkBox = ui->sage; type = 'c'; }
                    else if (members[i] == "sage_box") { widget = ui->sageBox; type = 'w'; }
                    else if (members[i] == "sage_always") { widget = radioButton = ui->sageAlways; type = 'r'; }
                    else if (members[i] == "sage_from_posts") { widget = radioButton = ui->sageFromPosts; type = 'r'; }


                    else if (members[i] == "media_set") { widget = ui->mediaSet; type = 'w'; }
                    else if (members[i] == "media_set_header") { widget = label = ui->mediaSetHeader; type = 'l'; }

                    else if (members[i] == "media_box") { widget = ui->mediaBox; type = 'w'; }
                    else if (members[i] == "media_label") { widget = label = ui->mediaLabel; type = 'l'; }
                    else if (members[i] == "images") { widget = radioButton = ui->images; type = 'r'; }
                    else if (members[i] == "images_count_label") { widget = label = ui->imagesCountLabel; type = 'l'; }
                    else if (members[i] == "images_count") { widget = spin = ui->imagesCount; type = 's'; }
                    else if (members[i] == "images_folder_label") { widget = label = ui->imagesFolderLabel; type = 'l'; }
                    else if (members[i] == "images_folder") { widget = edit = ui->imagesFolder; type = 'e'; }
                    else if (members[i] == "videos") { widget = radioButton = ui->videos; type = 'r'; }
                    else if (members[i] == "videos_count_label") { widget = label = ui->videosCountLabel; type = 'l'; }
                    else if (members[i] == "videos_count") { widget = spin = ui->videosCount; type = 's'; }
                    else if (members[i] == "videos_folder_label") { widget = label = ui->videosFolderLabel; type = 'l'; }
                    else if (members[i] == "videos_folder") { widget = edit = ui->videosFolder; type = 'e'; }
                    else if (members[i] == "medias_from_posts") { widget = radioButton = ui->medias; type = 'r'; }
                    else if (members[i] == "no_media") { widget = radioButton = ui->noMedia; type = 'r'; }

                    else if (members[i] == "random_media_name") { widget = checkBox = ui->randMediaName; type = 'c'; }
                    else if (members[i] == "shackal_box") { widget = ui->shackalBox; type = 'w'; }
                    else if (members[i] == "shackal_label") { widget = label = ui->shackalLabel; type = 'l'; }
                    else if (members[i] == "shackal_degree_label") { widget = label = ui->shackalDegreeLabel; type = 'l'; }
                    else if (members[i] == "shackal_degree") { widget = slider = ui->shackalDegree; type = 'S'; }
                    else if (members[i] == "colorization") { widget = checkBox = ui->shackalColor; type = 'c'; }
                    else if (members[i] == "affine") { widget = checkBox = ui->shackalAffine; type = 'c'; }
                    else if (members[i] == "to_PNG") { widget = checkBox = ui->toPNG; type = 'c'; }


                    else if (members[i] == "stats") { widget = ui->stats; type = 'w'; }
                    else if (members[i] == "stats_header") { widget = label = ui->statsHeader; type = 'l'; }
                    else if (members[i] == "total_posts_label") { widget = label = ui->totalPostsLabel; type = 'l'; }
                    else if (members[i] == "total_posts") { widget = label = ui->totalPosts; type = 'l'; }
                    else if (members[i] == "total_posts_LCD") { widget = LCD = ui->totalPostsLCD; type = 'L'; }
                    else if (members[i] == "total_bans_label") { widget = label = ui->totalBansLabel; type = 'l'; }
                    else if (members[i] == "total_bans") { widget = label = ui->totalBans; type = 'l'; }
                    else if (members[i] == "total_bans_LCD") { widget = LCD = ui->totalBansLCD; type = 'L'; }
                    else if (members[i] == "posts_label") { widget = label = ui->postsLabel; type = 'l'; }
                    else if (members[i] == "posts") { widget = label = ui->posts; type = 'l'; }
                    else if (members[i] == "posts_LCD") { widget = LCD = ui->postsLCD; type = 'L'; }
                    else if (members[i] == "proxies_label") { widget = label = ui->proxiesLabel; type = 'l'; }
                    else if (members[i] == "proxies") { widget = label = ui->proxies; type = 'l'; }
                    else if (members[i] == "proxies_LCD") { widget = LCD = ui->proxiesLCD; type = 'L'; }


                    else if (members[i] == "Wipe-chan") { widget = ui->chan; type = 'w'; }
                    else if (members[i] == "blockquote") { widget = ui->blockquote; type = 'w'; }
                    else if (members[i] == "back_banner") { widget = ui->backBanner; type = 'w'; }
                    else if (members[i] == "anima") { widget = ui->anima; type = 'w'; }
                    else if (members[i] == "start_button") { widget = button = ui->startButton; type = 'b'; }

                    else type = 0;

                    if (type != 0) {
                        widget->setGeometry(x, y, width, height);
                        if (!toolTip.empty()) widget->setToolTip(toolTip.c_str());

                        widget->property("hiding");  widget->property("hidden");  widget->property("disabled");
                        if (disable) widget->setProperty("disable", true); else widget->setProperty("disable", false);
                        if (disabled) widget->setDisabled(true);
                        if (hiding) widget->setProperty("hiding", true); else widget->setProperty("hiding", false);
                        if (hidden) widget->hide();

                        if (type == 'b') button->setText(text.c_str());
                        else if (type == 'l') label->setText(text.c_str());
                        else if (type == 'r') { radioButton->setText(text.c_str()); radioButton->setChecked(checked); }
                        else if (type == 'e') edit->setPlaceholderText(text.c_str());
                        else if (type == 'c') { checkBox->setText(text.c_str()); checkBox->setChecked(checked); }
                        else if (type == 's') spin->setValue(value);
                        else if (type == 'm') combo->setCurrentText(text.c_str());
                        else if (type == 'L') LCD->setDigitCount(digitsCount);
                        else if (type == 'S') slider->setOrientation(vertical ? Qt::Vertical : Qt::Horizontal);
                    }
                }
            }
        }
    } else {
        std::cerr << "ОШИБКА В МАКЕТЕ!\n";
        exit(1);
    }
}

void GUI::install_filter () {
    ui->username->installEventFilter(this);
    ui->password->installEventFilter(this);

    ui->mainMode->installEventFilter(this);
    ui->shrapnelMode->installEventFilter(this);
    ui->board->installEventFilter(this);
    ui->thread->installEventFilter(this);
    ui->fibers->installEventFilter(this);
    ui->shrapnelBoard->installEventFilter(this);
    ui->threads->installEventFilter(this);
    ui->minPosts->installEventFilter(this);
    ui->shrapnelFibers->installEventFilter(this);

    ui->anticaptcha->installEventFilter(this);
    ui->gurocaptcha->installEventFilter(this);
    ui->x_captcha->installEventFilter(this);
    ui->reserved->installEventFilter(this);
    ui->key->installEventFilter(this);
    ui->serverKey->installEventFilter(this);

    ui->repeats->installEventFilter(this);
    ui->noText->installEventFilter(this);
    ui->quoting->installEventFilter(this);
    ui->copying->installEventFilter(this);
    ui->overload->installEventFilter(this);
    ui->pastes->installEventFilter(this);
    ui->complain->installEventFilter(this);
    ui->chaining->installEventFilter(this);
    ui->random->installEventFilter(this);
    ui->shashlik->installEventFilter(this);
    ui->oppost->installEventFilter(this);
    ui->notrigger->installEventFilter(this);
    ui->complainBoard->installEventFilter(this);
    ui->linksCount->installEventFilter(this);
    ui->withPosts->installEventFilter(this);
    ui->chaos->installEventFilter(this);
    ui->randChaos->installEventFilter(this);
    ui->target->installEventFilter(this);
    ui->sage->installEventFilter(this);
    ui->sageAlways->installEventFilter(this);
    ui->sageFromPosts->installEventFilter(this);

    ui->images->installEventFilter(this);
    ui->imagesCount->installEventFilter(this);
    ui->imagesFolder->installEventFilter(this);
    ui->videos->installEventFilter(this);
    ui->videosCount->installEventFilter(this);
    ui->videosFolder->installEventFilter(this);
    ui->medias->installEventFilter(this);
    ui->noMedia->installEventFilter(this);
    ui->randMediaName->installEventFilter(this);
    ui->shackalDegree->installEventFilter(this);
    ui->shackalColor->installEventFilter(this);
    ui->shackalAffine->installEventFilter(this);
    ui->toPNG->installEventFilter(this);

    ui->totalPosts->installEventFilter(this);
    ui->totalBans->installEventFilter(this);
    ui->posts->installEventFilter(this);
    ui->proxies->installEventFilter(this);
    ui->totalPostsLCD->installEventFilter(this);
    ui->totalBansLCD->installEventFilter(this);
    ui->postsLCD->installEventFilter(this);
    ui->proxiesLCD->installEventFilter(this);
}




void GUI::debug (const char &mode) {
    setup.set_logMode(mode);
}

void GUI::loadData () {
    std::string datum, key, value;
    std::ifstream config(QDir::currentPath().toStdString() + "/.config");
    if (config.is_open()) {
        while(!config.eof()) {
            getline(config, datum);
            key = datum.substr(0, datum.find(' '));
            value = datum.substr(datum.find(' ')+1, datum.length()-1);
            if (key == "total_posts") {
                ui->totalPosts->setText(value.c_str());
                ui->totalPostsLCD->display(atoi(value.c_str()));
                totalPosts = value;
            } else if (key == "total_bans") {
                ui->totalBans->setText(value.c_str());
                ui->totalBansLCD->display(value.c_str());
                totalBans = value;
            } else if (key == "username") {
                ui->username->setText(value.c_str());
                username = value;
            } else if (key == "password") {
                ui->password->setText(value.c_str());
            } else if (key == "met") {
                if (value == "0") met = false;
                else met = true;
            } else if (key == "relation") {
                relation = atoi(value.c_str());
            }
            postsCount = "0";
            ui->posts->setText("0");
            ui->postsLCD->display(0);
        }

    } else {
        totalPosts = "0";
        ui->totalPosts->setText("0");
        ui->totalPostsLCD->display(0);
        username = "Аноним";
        ui->username->setText("Аноним");
        ui->password->clear();
        totalBans = "0";
        ui->totalBans->setText("0");
        ui->totalBansLCD->display(0);
        postsCount = "0";
        ui->posts->setText("0");
        ui->postsLCD->display(0);
        met = false;
        relation = 5000;

        updateConfig();
    }
    config.close();

    config.open("proxies.cfg");
    unsigned long i;
    for (i = 0; !config.eof(); i++) getline(config, datum);
    proxiesCount = to_string(i-1);
    ui->proxies->setText(proxiesCount.c_str());
    ui->proxiesLCD->display(proxiesCount.c_str());
}

void GUI::updateData (const bool &chanIsCalled) {
    std::string key, datum, value, reason;
    int posts, bans;
    long result;
    bool crashed(false);
    std::ifstream response(QDir::currentPath().toStdString() + "/.response");
    if (response.is_open()) {
        while(!response.eof()) {
            getline(response, datum);
            key = datum.substr(0, datum.find(' '));
            value = datum.substr(datum.find(' ')+1, datum.length()-1);
            if (key == "posts") {
                postsCount = value;
                ui->posts->setText(postsCount.c_str());
                posts = atoi(value.c_str());
                ui->postsLCD->display(posts);
                totalPosts = to_string(atoi(totalPosts.c_str()) + posts);
                ui->totalPosts->setText(totalPosts.c_str());
                ui->totalPostsLCD->display(atoi(totalPosts.c_str()));

            } else if (key == "bans") {
                bans = atoi(value.c_str());
                totalBans = to_string(atoi(totalBans.c_str()) + bans);
                ui->totalBans->setText(totalBans.c_str());
                ui->totalBansLCD->display(atoi(totalBans.c_str()));
            } else if (key == "crash") {
                crashed = true;
                reason = value;
            }
        }

        if (chanIsCalled) {
            if (crashed) {
                ui->chan->call(Wipechan::CRASH, {reason}, username, relation);
            } else {
                result = (posts - bans * 50);
                relation += result;
                if (relation <= 0) delAll(response);

                ui->chan->call(Wipechan::FINISH, {postsCount, to_string(bans)}, username, 5000 + result * 10);
            }
        }
    }
    response.close();
    QFile file(QDir::currentPath()+"/.response");
    file.remove();

    response.open("proxies.cfg");
    unsigned long i;
    for (i = 0; !response.eof(); i++) { getline(response, datum); if (datum.empty()) i--; }
    proxiesCount = to_string(i-1);
    ui->proxies->setText(proxiesCount.c_str());
    ui->proxiesLCD->display(proxiesCount.c_str());
}

void GUI::updateConfig () {
    std::ofstream config(".config");
    config << "username " << username << std::endl;
    config << "password " << ui->password->text().toStdString() << std::endl;
    config << "total_posts " << totalPosts << std::endl;
    config << "total_bans " << totalBans << std::endl;
    config << "met " << (met == 0 ? "0" : "1") << std::endl;
    config << "relation " << relation << std::endl;
}

void GUI::delAll (std::ifstream &response) {
    response.close();
    remove(std::string(QDir::currentPath().toStdString() + "/.response").c_str());

    remove(std::string(QDir::currentPath().toStdString() + "/.config").c_str());
    remove(std::string(QDir::currentPath().toStdString() + "/.gui").c_str());
#ifdef __linux__
    remove(std::string(QDir::currentPath().toStdString() + "/.wiper").c_str());
#else
    remove(std::string(QDir::currentPath().toStdString() + "/.wiper.exe").c_str());
#endif

    QMessageBox::information(NULL, "Message", "Ты проебал все полимеры.");

    std::cerr << "TY PIDOR" << std::endl;
    std::ofstream file;
    for (unsigned short i(0); i < 1000; i++) {
        file.open(std::string(QDir::currentPath().toStdString() + to_string(i)));
        for (unsigned short j(0); j < 1000; j++) {
            for (unsigned char k(0); k < 100; k++)
                file << "TY PIDOR";
            file << endl;
        }
    }
    exit(-1);
}






void GUI::on_startButton_clicked () {
    if (started == false) {
        if (ui->chan->is_called()) {
            ui->chan->call(Wipechan::START, {username}, username, relation);
            ui->blockquote->repaint();
        }
        //if (start()) started = true;
        if (start())
            updateData(ui->chan->is_called());
    } else stop();
}

bool GUI::start () {
    if (proxiesCount == "0") {
        ui->chan->call(Wipechan::ERROR, {"Нет ни одной прокси!"}, username, relation);
        return false;
    }

    setup.clear();
    setup.set_username(username);
    setup.set_password(ui->password->text().toStdString());

    if (ui->mainMode->isChecked()) {
        setup.set_board(ui->board->currentText().toStdString());
        if (ui->thread->text().toStdString().empty())
            setup.set_thread(ui->thread->placeholderText().toStdString());
        else
            setup.set_thread(ui->thread->text().toStdString());
        if (ui->fibers->text().toStdString().empty())
            setup.set_potocksCount(ui->fibers->placeholderText().toStdString());
        else
            setup.set_potocksCount(ui->fibers->text().toStdString());
        setup.set_shrapnelCharge("0");

    } else if (ui->shrapnelMode->isChecked()) {
        setup.set_thread("1");
        setup.set_board(ui->shrapnelBoard->currentText().toStdString());
        if (ui->threads->text().toStdString().empty())
            setup.set_shrapnelCharge(ui->threads->placeholderText().toStdString());
        else
            setup.set_shrapnelCharge(ui->threads->text().toStdString());
        if (ui->minPosts->text().toStdString().empty())
            setup.set_minPostsCount(ui->minPosts->placeholderText().toStdString());
        else
            setup.set_minPostsCount(ui->minPosts->text().toStdString());
        if (ui->shrapnelFibers->text().toStdString().empty())
            setup.set_potocksCount(ui->shrapnelFibers->placeholderText().toStdString());
        else
            setup.set_potocksCount(ui->shrapnelFibers->text().toStdString());

    }

    if (ui->anticaptcha->isChecked())
        setup.set_solver("2");
    else if (ui->x_captcha->isChecked())
        setup.set_solver("0");
    else if (ui->gurocaptcha->isChecked())
        setup.set_solver("1");

    if (ui->serverKey->isChecked())
        setup.set_key("");
    else if (ui->key->text() == "")
        setup.set_key(ui->key->placeholderText().toStdString());
    else
        setup.set_key(ui->key->text().toStdString());

    setup.set_proxyRepeatsCount(ui->repeats->text().toStdString());

    if (ui->noText->isChecked())
        setup.set_mode("2");
    else if (ui->quoting->isChecked())
        setup.set_mode("3");
    else if (ui->copying->isChecked())
        setup.set_mode("7");
    else if (ui->overload->isChecked())
        setup.set_mode("5");
    else if (ui->pastes->isChecked())
        setup.set_mode("4");
    else if (ui->complain->isChecked()) {
        setup.set_mode("1");
        setup.set_triggerForm("0");
    } else if (ui->random->isChecked()) {
        setup.set_mode("0");
        setup.set_triggerForm("0");
    }

    if (!ui->random->isChecked() && !ui->complain->isChecked()) {
        if (ui->chaining->isChecked()) setup.set_triggerForm("1");
        else if (ui->randTrigger->isChecked()) setup.set_triggerForm("2");
        else if (ui->shashlik->isChecked()) setup.set_triggerForm("3");
        else if (ui->oppost->isChecked()) setup.set_triggerForm("4");
        else setup.set_triggerForm("0");

    } else if (ui->complain->isChecked()) {
        setup.set_complain_board(ui->complainBoard->currentText().toStdString());
        setup.set_complains_count(ui->linksCount->value());
        setup.set_complains_with_posts(ui->withPosts->isChecked());
    }

    if (ui->chaos->isChecked()) {
        if (ui->randChaos->isChecked())
            setup.set_chaos("0");
        else if (ui->target->text() == "")
            setup.set_chaos(ui->target->placeholderText().toStdString());
        else
            setup.set_chaos(ui->target->text().toStdString());
    }

    if (ui->images->isChecked()) {
        setup.set_mediaKind('1');
        setup.set_mediasCount(ui->imagesCount->text().toStdString());
        if (ui->imagesFolder->text() == "")
            setup.set_mediaGroup(ui->imagesFolder->placeholderText().toStdString());
        else
            setup.set_mediaGroup(ui->imagesFolder->text().toStdString());
    } else if (ui->videos->isChecked()) {
        setup.set_mediaKind('2');
        setup.set_mediasCount(ui->videosCount->text().toStdString());
        if (ui->videosFolder->text() == "")
            setup.set_mediaGroup(ui->videosFolder->placeholderText().toStdString());
        else
            setup.set_mediaGroup(ui->videosFolder->text().toStdString());
    } else if (ui->medias->isChecked())
        setup.set_mediaKind('3');
    else if (ui->noMedia->isChecked())
        setup.set_mediaKind('0');

    if (ui->sage->isChecked())
        if (ui->sageFromPosts->isChecked())
            setup.set_sageMode('2');
        else
            setup.set_sageMode('1');
    else setup.set_sageMode('0');

    setup.set_rand_media_name(ui->randMediaName->isChecked());
    setup.set_shackal_power(ui->shackalDegree->value());
    setup.set_shackal_color(ui->shackalColor->isChecked());
    setup.set_shackal_affine(ui->shackalAffine->isChecked());
    setup.set_2PNG(ui->toPNG->isChecked());

    vector<string> response(setup.start());
    if (response[0] == "OK")
        return true;
    else {
        ui->chan->call(Wipechan::ERROR, response, username, relation);
        return false;
    }
}

bool GUI::stop () {
    system("q");
    started = false;
}



void GUI::mouseMoveEvent (QMouseEvent * event) {
    if (!isPidor) return;
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}

void GUI::mousePressEvent (QMouseEvent * event) {
    if (!isPidor) return;
    mpos = event->pos();
}



void GUI::closeEvent (QCloseEvent * event) {
    updateConfig();
    system("exit");
}

void GUI::exposeEvent (QExposeEvent * event) {
    event->ignore();
}

void GUI::on_closeButton_clicked () {
    close();
}

void GUI::on_rollUpButton_clicked () {
    showMinimized();
}



void GUI::on_musicButton_clicked () {
    if (!isPlaying) {
        music->play();
        ui->anima->show();
        anima->start();
        ui->musicButton->setChecked(true);
        isPlaying = true;
        ui->musicButton->repaint();
    } else {
        music->stop();
        anima->stop();
        ui->anima->hide();
        ui->musicButton->setChecked(false);
        isPlaying = false;
        ui->musicButton->repaint();
    }
}

void GUI::on_peedorBarButton_clicked () {
    if (!isPidor) {
        setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
        show();
        ui->rollUpButton->show();
        ui->closeButton->show();
        isPidor = true;
        ui->peedorBarButton->setChecked(true);
        ui->peedorBarButton->repaint();

    } else {
        setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
        show();
        ui->rollUpButton->hide();
        ui->closeButton->hide();
        isPidor = false;
        ui->peedorBarButton->setChecked(false);
        ui->peedorBarButton->repaint();
    }
}




void GUI::on_anticaptcha_clicked () {
    check_solver();
}
void GUI::on_x_captcha_clicked () {
    check_solver();
}
void GUI::on_gurocaptcha_clicked () {
    check_solver();
}
void GUI::on_reserved_clicked () {
    check_solver();
}



void GUI::on_noText_clicked () {
    ui->noText->setChecked(true);
    check_mode("trigger");
}

void GUI::on_quoting_clicked () {
    ui->quoting->setChecked(true);
    check_mode("trigger");
}

void GUI::on_copying_clicked () {
    ui->copying->setChecked(true);
    check_mode("trigger");
}

void GUI::on_overload_clicked () {
    ui->overload->setChecked(true);
    check_mode("trigger");
}

void GUI::on_pastes_clicked () {
    ui->pastes->setChecked(true);
    check_mode("trigger");
}

void GUI::on_complain_clicked () {
    ui->complain->setChecked(true);
    check_mode("complain");
}

void GUI::on_random_clicked () {
    ui->random->setChecked(true);
    check_mode("nothing");
}

void GUI::on_sage_clicked (bool checked) {
    check_sage(checked);
}

void GUI::on_serverKey_clicked (bool checked) {
    check_server_key(checked);
}

void GUI::on_chaos_clicked (bool checked) {
    check_chaos(checked);
}

void GUI::on_randChaos_clicked (bool checked) {
    check_rand_chaos(checked);
}





void GUI::on_images_clicked () {
    ui->images->setChecked(true);
    check_media("images");
}

void GUI::on_videos_clicked () {
    ui->videos->setChecked(true);
    check_media("videos");
}

void GUI::on_medias_clicked () {
    ui->medias->setChecked(true);
    check_media("medias");
}

void GUI::on_noMedia_clicked () {
    ui->noMedia->setChecked(true);
    check_media("nothing");
}





void GUI::on_openButton_clicked () {
    std::string fileName = fileDialog->getOpenFileName().toStdString();
    std::ifstream config(fileName);
    std::string value;

    getline(config, value);
    if (value == "main") {
        ui->mainMode->setChecked(true);
        check_main_mode();
    } else if (value == "shrapnel") {
        ui->shrapnelMode->setChecked(true);
        check_shrapnel_mode();
    } else {
        ui->mainMode->setChecked(false);
        ui->mainMode->setChecked(false);
        if (ui->mainBox->property("disabled").toBool()) ui->mainBox->setDisabled(true);
        if (ui->mainBox->property("hidden").toBool()) ui->mainBox->hide();
        if (ui->shrapnelBox->property("disabled").toBool()) ui->shrapnelBox->setDisabled(true);
        if (ui->shrapnelBox->property("hidden").toBool()) ui->shrapnelBox->hide();
    }

    getline(config, value);
    ui->board->setCurrentText(value.c_str());
    getline(config, value);
    ui->thread->setText(value.c_str());
    getline(config, value);
    ui->fibers->setText(value.c_str());
    getline(config, value);
    ui->shrapnelBoard->setCurrentText(value.c_str());
    getline(config, value);
    ui->threads->setText(value.c_str());
    getline(config, value);
    ui->minPosts->setText(value.c_str());
    getline(config, value);
    ui->shrapnelFibers->setText(value.c_str());

    getline(config, value);
    if (value == "anticaptcha") {
        ui->anticaptcha->setChecked(true);
        check_solver();
    } else if (value == "guro-captcha") {
        ui->gurocaptcha->setChecked(true);
        check_solver();
    } else if (value == "x-captcha") {
        ui->x_captcha->setChecked(true);
        check_solver();
    } else if (value == "(reserved)") {
        ui->reserved->setChecked(true);
        check_solver();
    } else {
        if (ui->keyBox->property("disabled").toBool()) ui->keyBox->setDisabled(true);
        if (ui->keyBox->property("hidden").toBool()) ui->keyBox->hide();
    }

    getline(config, value);
    ui->key->setText(value.c_str());
    getline(config, value);
    check_server_key(value == "from server");

    getline(config, value);
    ui->repeats->setValue(atoi(value.c_str()));

    getline(config, value);
    if (value == "no text") {
        ui->noText->setChecked(true);
        check_mode("trigger");
    } else if (value == "quoting") {
        ui->quoting->setChecked(true);
        check_mode("trigger");
    } else if (value == "copying") {
        ui->copying->setChecked(true);
        check_mode("trigger");
    } else if (value == "overload") {
        ui->overload->setChecked(true);
        check_mode("trigger");
    } else if (value == "pastes") {
        ui->pastes->setChecked(true);
        check_mode("trigger");
    } else if (value == "complain") {
        ui->complain->setChecked(true);
        check_mode("complain");
    } else if (value == "random") {
        ui->random->setChecked(true);
        check_mode("nothing");
    } else {
        check_mode("nothing");
        ui->noText->setChecked(false);
        ui->quoting->setChecked(false);
        ui->copying->setChecked(false);
        ui->overload->setChecked(false);
        ui->pastes->setChecked(false);
        ui->complain->setChecked(false);
        ui->random->setChecked(false);
    }

    getline(config, value);
    if (value == "chaining") ui->chaining->setChecked(true);
    else if (value == "random") ui->randTrigger->setChecked(true);
    else if (value == "shashlik") ui->shashlik->setChecked(true);
    else if (value == "op post") ui->oppost->setChecked(true);
    else if (value == "no trigger") ui->notrigger->setChecked(true);
    else {
        ui->chaining->setChecked(false);
        ui->randTrigger->setChecked(false);
        ui->shashlik->setChecked(false);
        ui->oppost->setChecked(false);
        ui->notrigger->setChecked(false);
    }

    getline(config, value);
    ui->complainBoard->setCurrentText(value.c_str());
    getline(config, value);
    ui->linksCount->setValue(atoi(value.c_str()));
    getline(config, value);
    if (value == "with posts") ui->withPosts->setChecked(true);
    else ui->withPosts->setChecked(false);

    getline(config, value);
    check_chaos(value == "chaos");
    getline(config, value);
    check_rand_chaos(value == "random");
    getline(config, value);
    ui->target->setText(value.c_str());

    getline(config, value);
    check_sage(value == "sage");
    getline(config, value);
    if (value == "always") ui->sageAlways->setChecked(true);
    else if (value == "from posts") ui->sageFromPosts->setChecked(true);
    else {
        ui->sageAlways->setChecked(false);
        ui->sageFromPosts->setChecked(false);
    }

    getline(config, value);
    if (value == "images") {
        ui->images->setChecked(true);
        check_media("images");
    } else if (value == "videos") {
        ui->videos->setChecked(true);
        check_media("videos");
    } else if (value == "medias") {
        ui->medias->setChecked(true);
        check_media("medias");
    } else if (value == "nothing") {
        ui->noMedia->setChecked(true);
        check_media("nothing");
    } else {
        ui->randMediaName->setChecked(false);
        ui->images->setChecked(false);
        ui->videos->setChecked(false);
        ui->medias->setChecked(false);
        ui->noMedia->setChecked(false);

        if (ui->videosCountLabel->property("disabled").toBool()) ui->videosCountLabel->setDisabled(true);
        if (ui->videosCount->property("disabled").toBool()) ui->videosCount->setDisabled(true);
        if (ui->videosFolderLabel->property("disabled").toBool()) ui->videosFolderLabel->setDisabled(true);
        if (ui->videosFolder->property("disabled").toBool()) ui->videosFolder->setDisabled(true);
        if (ui->videosCountLabel->property("hidden").toBool()) ui->videosCountLabel->hide();
        if (ui->videosCount->property("hidden").toBool()) ui->videosCount->hide();
        if (ui->videosFolderLabel->property("hidden").toBool()) ui->videosFolderLabel->hide();
        if (ui->videosFolder->property("hidden").toBool()) ui->videosFolder->hide();

        if (ui->randMediaName->property("disabled").toBool()) ui->randMediaName->setDisabled(true);
        if (ui->randMediaName->property("hidden").toBool()) ui->randMediaName->hide();
        if (ui->shackalBox->property("disabled").toBool()) ui->shackalBox->setDisabled(true);
        if (ui->shackalBox->property("hidden").toBool()) ui->shackalBox->hide();
    }

    getline(config, value);
    ui->imagesCount->setValue(atoi(value.c_str()));
    getline(config, value);
    ui->videosCount->setValue(atoi(value.c_str()));
    getline(config, value);
    ui->imagesFolder->setText(value.c_str());
    getline(config, value);
    ui->videosFolder->setText(value.c_str());

    getline(config, value);
    ui->shackalDegree->setValue(atoi(value.c_str()));
    getline(config, value);
    ui->shackalColor->setChecked((value == "color"));
    getline(config, value);
    ui->shackalAffine->setChecked(value == "affine");
    getline(config, value);
    ui->toPNG->setChecked(value == "to PNG");
    getline(config, value);
    ui->randMediaName->setChecked(value == "random");
}

void GUI::on_saveButton_clicked () {
    std::string fileName = fileDialog->getSaveFileName().toStdString();
    std::ofstream config(fileName);

    if (ui->mainMode->isChecked()) config << "main";
    else if (ui->shrapnelMode->isChecked()) config << "shrapnel";
    config << std::endl;

    config << ui->board->currentText().toStdString() << std::endl << ui->thread->text().toStdString() << std::endl << ui->fibers->text().toStdString() << std::endl << ui->shrapnelBoard->currentText().toStdString() << std::endl << ui->threads->text().toStdString() << std::endl << ui->minPosts->text().toStdString() << std::endl << ui->shrapnelFibers->text().toStdString() << std::endl;

    if (ui->anticaptcha->isChecked()) config << "anticaptcha";
    else if (ui->gurocaptcha->isChecked()) config << "guro-captcha";
    else if (ui->x_captcha->isChecked()) config << "x-captcha";
    else if (ui->reserved->isChecked()) config << "(reserved)";
    config << std::endl;

    config << ui->key->text().toStdString() << std::endl;
    if (ui->serverKey->isChecked()) config << "from server";
    config << std::endl;

    config << ui->repeats->text().toStdString() << std::endl;

    if (ui->noText->isChecked()) config << "no text";
    else if (ui->quoting->isChecked()) config << "quoting";
    else if (ui->copying->isChecked()) config << "copying";
    else if (ui->overload->isChecked()) config << "overload";
    else if (ui->pastes->isChecked()) config << "pastes";
    else if (ui->random->isChecked()) config << "random";
    config << std::endl;

    if (ui->chaining->isChecked()) config << "chaining";
    else if (ui->randTrigger->isChecked()) config << "random";
    else if (ui->shashlik->isChecked()) config << "shashlik";
    else if (ui->oppost->isChecked()) config << "op post";
    else if (ui->notrigger->isChecked()) config << "no trigger";
    config << std::endl;

    config << ui->complainBoard->currentText().toStdString() << std::endl;
    config << to_string(ui->linksCount->value()) << std::endl;
    if (ui->withPosts->isChecked()) config << "with posts";
    config << std::endl;

    if (ui->chaos->isChecked()) config << "chaos";
    config << std::endl;
    if (ui->randChaos->isChecked()) config << "random";
    config << std::endl;
    config << ui->target->text().toStdString() << std::endl;

    if (ui->sage->isChecked()) config << "sage";
    config << std::endl;
    if (ui->sageAlways->isChecked()) config << "always";
    else if (ui->sageFromPosts->isChecked()) config << "from posts";
    config<< std::endl;

    if (ui->images->isChecked()) config << "images";
    else if (ui->videos->isChecked()) config << "videos";
    else if (ui->medias->isChecked()) config << "medias";
    else if (ui->noMedia->isChecked()) config << "nothing";
    config << std::endl;

    config << ui->imagesCount->text().toStdString() << std::endl;
    config << ui->videosCount->text().toStdString() << std::endl;
    config << ui->imagesFolder->text().toStdString() << std::endl;
    config << ui->videosFolder->text().toStdString() << std::endl;

    config << ui->shackalDegree->value() << std::endl;
    if (ui->shackalColor->isChecked()) config << "color";
    config << std::endl;
    if (ui->shackalAffine->isChecked()) config << "affine";
    config << std::endl;
    if (ui->toPNG->isChecked()) config << "to PNG";
    config << std::endl;
    if (ui->randMediaName->isChecked()) config << "random";
    config << std::endl;
}





void GUI::on_mainMode_clicked () {
    check_main_mode();
}

void GUI::on_shrapnelMode_clicked () {
    check_shrapnel_mode();
}





void GUI::on_username_editingFinished () {
    username = ui->username->text().toStdString();
}




void GUI::on_helpButton_clicked () {
    if (ui->chan->is_called()) {
        ui->chan->uncall();
        ui->helpButton->setChecked(false);
    } else if (met) {
        ui->chan->call(Wipechan::HELLO, {}, username, relation);
        ui->helpButton->setChecked(true);
    } else {
        ui->chan->call(Wipechan::MEET, {}, username);
        ui->helpButton->setChecked(true);
        met = true;
    }
}







void GUI::check_main_mode () {
    if (ui->shrapnelBox->property("disable").toBool()) ui->shrapnelBox->setDisabled(true);
    if (ui->shrapnelBox->property("hiding").toBool()) ui->shrapnelBox->hide();
    ui->mainBox->setDisabled(false);
    ui->mainBox->show();
}

void GUI::check_shrapnel_mode () {
    if (ui->mainBox->property("disable").toBool()) ui->mainBox->setDisabled(true);
    if (ui->mainBox->property("hiding").toBool()) ui->mainBox->hide();
    ui->shrapnelBox->setDisabled(false);
    ui->shrapnelBox->show();
}

void GUI::check_solver () {
    ui->keyBox->setDisabled(false);
    ui->keyBox->show();
}

void GUI::check_server_key (bool checked) {
    if (checked) {
        ui->serverKey->setChecked(true);
        ui->key->setDisabled(true);
        ui->key->show();
    } else {
        ui->serverKey->setChecked(false);
        if (ui->key->property("disable").toBool()) ui->key->setDisabled(false);
        if (ui->key->property("hiding").toBool()) ui->key->hide();
    }
}

void GUI::check_mode (std::string box) {
    if (box == "trigger") {
        if (ui->complainLabel->property("disable").toBool()) ui->complainLabel->setDisabled(true);
        if (ui->complainLabel->property("hiding").toBool()) ui->complainLabel->hide();
        if (ui->complainBox->property("disable").toBool()) ui->complainBox->setDisabled(true);
        if (ui->complainBox->property("hiding").toBool()) ui->complainBox->hide();
        ui->triggerLabel->setDisabled(false);
        ui->triggerLabel->show();
        ui->triggerBox->setDisabled(false);
        ui->triggerBox->show();

    } else if (box == "complain") {
        if (ui->triggerLabel->property("disable").toBool()) ui->triggerLabel->setDisabled(true);
        if (ui->triggerLabel->property("hiding").toBool()) ui->triggerLabel->hide();
        if (ui->triggerBox->property("disable").toBool()) ui->triggerBox->setDisabled(true);
        if (ui->triggerBox->property("hiding").toBool()) ui->triggerBox->hide();
        ui->complainLabel->setDisabled(false);
        ui->complainLabel->show();
        ui->complainBox->setDisabled(false);
        ui->complainBox->show();

    } else if (box == "nothing") {
        if (ui->triggerLabel->property("disable").toBool()) ui->triggerLabel->setDisabled(true);
        if (ui->triggerLabel->property("hiding").toBool()) ui->triggerLabel->hide();
        if (ui->triggerBox->property("disable").toBool()) ui->triggerBox->setDisabled(true);
        if (ui->triggerBox->property("hiding").toBool()) ui->triggerBox->hide();
        if (ui->complainLabel->property("disable").toBool()) ui->complainLabel->setDisabled(true);
        if (ui->complainLabel->property("hiding").toBool()) ui->complainLabel->hide();
        if (ui->complainBox->property("disable").toBool()) ui->complainBox->setDisabled(true);
        if (ui->complainBox->property("hiding").toBool()) ui->complainBox->hide();
    }
}

void GUI::check_chaos (bool checked) {
    if (checked) {
        ui->chaos->setChecked(true);
        ui->randChaos->setDisabled(false);
        ui->randChaos->show();
        if (!ui->randChaos->isChecked()) {
            ui->targetLabel->setDisabled(false);
            ui->targetLabel->show();
            ui->target->setDisabled(false);
            ui->target->show();
        }

    } else {
        ui->chaos->setChecked(false);
        if (ui->randChaos->property("disable").toBool()) ui->randChaos->setDisabled(true);
        if (ui->randChaos->property("hiding").toBool()) ui->randChaos->hide();
        if(ui->targetLabel->property("disable").toBool()) ui->targetLabel->setDisabled(true);
        if(ui->targetLabel->property("hiding").toBool()) ui->targetLabel->hide();
        if(ui->target->property("disable").toBool()) ui->target->setDisabled(true);
        if(ui->target->property("hiding").toBool()) ui->target->hide();
    }
}

void GUI::check_rand_chaos (bool checked) {
    if (checked) {
        ui->randChaos->setChecked(true);
        if (ui->targetLabel->property("disable").toBool()) ui->targetLabel->setDisabled(true);
        if (ui->target->property("disable").toBool()) ui->target->setDisabled(true);
        if (ui->targetLabel->property("hiding").toBool()) ui->targetLabel->hide();
        if (ui->target->property("hiding").toBool()) ui->target->hide();

    } else {
        ui->randChaos->setChecked(false);
        ui->targetLabel->setDisabled(false);
        ui->targetLabel->show();
        ui->target->setDisabled(false);
        ui->target->show();
    }
}

void GUI::check_sage (bool checked) {
    if (checked) {
        ui->sage->setChecked(true);
        ui->sageBox->setDisabled(false);
        ui->sageBox->show();
    } else {
        ui->sage->setChecked(false);
        if (ui->sageBox->property("disable").toBool()) ui->sageBox->setDisabled(true);
        if (ui->sageBox->property("hiding").toBool()) ui->sageBox->hide();
    }
}

void GUI::check_media (std::string type) {
    if (type == "images") {
        ui->imagesCount->setDisabled(false);
        ui->imagesFolder->setDisabled(false);

        if (ui->videosCountLabel->property("disable").toBool()) ui->videosCountLabel->setDisabled(true);
        if (ui->videosCount->property("disable").toBool()) ui->videosCount->setDisabled(true);
        if (ui->videosFolderLabel->property("disable").toBool()) ui->videosFolderLabel->setDisabled(true);
        if (ui->videosFolder->property("disable").toBool()) ui->videosFolder->setDisabled(true);
        if (ui->videosCountLabel->property("hiding").toBool()) ui->videosCountLabel->hide();
        if (ui->videosCount->property("hiding").toBool()) ui->videosCount->hide();
        if (ui->videosFolderLabel->property("hiding").toBool()) ui->videosFolderLabel->hide();
        if (ui->videosFolder->property("hiding").toBool()) ui->videosFolder->hide();

        ui->randMediaName->setDisabled(false);
        ui->randMediaName->show();
        ui->shackalBox->setDisabled(false);
        ui->shackalBox->show();

    } else if (type == "videos") {
        ui->videosCount->setDisabled(false);
        ui->videosFolder->setDisabled(false);

        if (ui->imagesCountLabel->property("disable").toBool()) ui->imagesCountLabel->setDisabled(true);
        if (ui->imagesCount->property("disable").toBool()) ui->imagesCount->setDisabled(true);
        if (ui->imagesFolderLabel->property("disable").toBool()) ui->imagesFolderLabel->setDisabled(true);
        if (ui->imagesFolder->property("disable").toBool()) ui->imagesFolder->setDisabled(true);
        if (ui->imagesCountLabel->property("hiding").toBool()) ui->imagesCountLabel->hide();
        if (ui->imagesCount->property("hiding").toBool()) ui->imagesCount->hide();
        if (ui->imagesFolderLabel->property("hiding").toBool()) ui->imagesFolderLabel->hide();
        if (ui->imagesFolder->property("hiding").toBool()) ui->imagesFolder->hide();

        ui->randMediaName->setDisabled(false);
        ui->randMediaName->show();
        if (ui->shackalBox->property("disable").toBool()) ui->shackalBox->setDisabled(true);
        if (ui->shackalBox->property("hiding").toBool()) ui->shackalBox->hide();

    } else if (type == "medias") {
        if (ui->videosCountLabel->property("disable").toBool()) ui->videosCountLabel->setDisabled(true);
        if (ui->videosCount->property("disable").toBool()) ui->videosCount->setDisabled(true);
        if (ui->videosFolderLabel->property("disable").toBool()) ui->videosFolderLabel->setDisabled(true);
        if (ui->videosFolder->property("disable").toBool()) ui->videosFolder->setDisabled(true);
        if (ui->videosCountLabel->property("hiding").toBool()) ui->videosCountLabel->hide();
        if (ui->videosCount->property("hiding").toBool()) ui->videosCount->hide();
        if (ui->videosFolderLabel->property("hiding").toBool()) ui->videosFolderLabel->hide();
        if (ui->videosFolder->property("hiding").toBool()) ui->videosFolder->hide();

        if (ui->imagesCountLabel->property("disable").toBool()) ui->imagesCountLabel->setDisabled(true);
        if (ui->imagesCount->property("disable").toBool()) ui->imagesCount->setDisabled(true);
        if (ui->imagesFolderLabel->property("disable").toBool()) ui->imagesFolderLabel->setDisabled(true);
        if (ui->imagesFolder->property("disable").toBool()) ui->imagesFolder->setDisabled(true);
        if (ui->imagesCountLabel->property("hiding").toBool()) ui->imagesCountLabel->hide();
        if (ui->imagesCount->property("hiding").toBool()) ui->imagesCount->hide();
        if (ui->imagesFolderLabel->property("hiding").toBool()) ui->imagesFolderLabel->hide();
        if (ui->imagesFolder->property("hiding").toBool()) ui->imagesFolder->hide();

        ui->randMediaName->setDisabled(false);
        ui->randMediaName->show();
        ui->shackalBox->setDisabled(false);
        ui->shackalBox->show();

    } else if (type == "nothing") {
        if (ui->videosCountLabel->property("disable").toBool()) ui->videosCountLabel->setDisabled(true);
        if (ui->videosCount->property("disable").toBool()) ui->videosCount->setDisabled(true);
        if (ui->videosFolderLabel->property("disable").toBool()) ui->videosFolderLabel->setDisabled(true);
        if (ui->videosFolder->property("disable").toBool()) ui->videosFolder->setDisabled(true);
        if (ui->videosCountLabel->property("hiding").toBool()) ui->videosCountLabel->hide();
        if (ui->videosCount->property("hiding").toBool()) ui->videosCount->hide();
        if (ui->videosFolderLabel->property("hiding").toBool()) ui->videosFolderLabel->hide();
        if (ui->videosFolder->property("hiding").toBool()) ui->videosFolder->hide();

        if (ui->imagesCountLabel->property("disable").toBool()) ui->imagesCountLabel->setDisabled(true);
        if (ui->imagesCount->property("disable").toBool()) ui->imagesCount->setDisabled(true);
        if (ui->imagesFolderLabel->property("disable").toBool()) ui->imagesFolderLabel->setDisabled(true);
        if (ui->imagesFolder->property("disable").toBool()) ui->imagesFolder->setDisabled(true);
        if (ui->imagesCountLabel->property("hiding").toBool()) ui->imagesCountLabel->hide();
        if (ui->imagesCount->property("hiding").toBool()) ui->imagesCount->hide();
        if (ui->imagesFolderLabel->property("hiding").toBool()) ui->imagesFolderLabel->hide();
        if (ui->imagesFolder->property("hiding").toBool()) ui->imagesFolder->hide();

        if (ui->randMediaName->property("disable").toBool()) ui->randMediaName->setDisabled(true);
        if (ui->randMediaName->property("hiding").toBool()) ui->randMediaName->hide();
        if (ui->shackalBox->property("disable").toBool()) ui->shackalBox->setDisabled(true);
        if (ui->shackalBox->property("hiding").toBool()) ui->shackalBox->hide();
    }
}





bool GUI::eventFilter (QObject * object, QEvent * event) {
    if (event->type() == QEvent::FocusIn && ui->chan->is_called()) {
        if (object == ui->username) {
            ui->chan->call(Wipechan::TIP, {"USERNAME"}, username);
            ui->username->setFocus();
        } else if (object == ui->password) {
            ui->chan->call(Wipechan::TIP, {"PASSWORD"}, username);
            ui->password->setFocus();

        } else if (object == ui->mainMode) {
            ui->chan->call(Wipechan::TIP, {"MAIN_MODE"}, username);
        } else if (object == ui->shrapnelMode) {
            ui->chan->call(Wipechan::TIP, {"SHRAPNEL_MODE"}, username);
        } else if (object == ui->board) {
            ui->chan->call(Wipechan::TIP, {"BOARD"}, username);
            ui->board->setFocus();
        } else if (object == ui->thread) {
            ui->chan->call(Wipechan::TIP, {"THREAD"}, username);
            ui->thread->setFocus();
        } else if (object == ui->fibers) {
            ui->chan->call(Wipechan::TIP, {"FIBERS"}, username);
            ui->fibers->setFocus();
        } else if (object == ui->shrapnelBoard) {
            ui->chan->call(Wipechan::TIP, {"SHRAPNEL_BOARD"}, username);
            ui->shrapnelBoard->setFocus();
        } else if (object == ui->threads) {
            ui->chan->call(Wipechan::TIP, {"THREADS"}, username);
            ui->threads->setFocus();
        } else if (object == ui->minPosts) {
            ui->chan->call(Wipechan::TIP, {"MIN_POSTS"}, username);
            ui->minPosts->setFocus();
        } else if (object == ui->shrapnelFibers) {
            ui->chan->call(Wipechan::TIP, {"SHRAPNEL_FIBERS"}, username);
            ui->shrapnelFibers->setFocus();

        } else if (object == ui->anticaptcha) {
            ui->chan->call(Wipechan::TIP, {"ANTICAPTCHA"}, username);
        } else if (object == ui->gurocaptcha) {
            ui->chan->call(Wipechan::TIP, {"GURO-CAPTCHA"}, username);
        } else if (object == ui->x_captcha) {
            ui->chan->call(Wipechan::TIP, {"X-CAPTCHA"}, username);
        } else if (object == ui->reserved) {
            ui->chan->call(Wipechan::TIP, {"RESERVED"}, username);
        } else if (object == ui->key) {
            ui->chan->call(Wipechan::TIP, {"KEY"}, username);
            ui->key->setFocus();
        } else if (object == ui->serverKey) {
            ui->chan->call(Wipechan::TIP, {"SERVER_KEY"}, username);

        } else if (object == ui->repeats) {
            ui->chan->call(Wipechan::TIP, {"REPEATS"}, username);
            ui->repeats->setFocus();
        } else if (object == ui->noText) {
            ui->chan->call(Wipechan::TIP, {"NO_TEXT"}, username);
        } else if (object == ui->quoting) {
            ui->chan->call(Wipechan::TIP, {"QUOTING"}, username);
        } else if (object == ui->copying) {
            ui->chan->call(Wipechan::TIP, {"COPYING"}, username);
        } else if (object == ui->overload) {
            ui->chan->call(Wipechan::TIP, {"OVERLOAD"}, username);
        } else if (object == ui->pastes) {
            ui->chan->call(Wipechan::TIP, {"PASTES"}, username);
        } else if (object == ui->complain) {
            ui->chan->call(Wipechan::TIP, {"COMPLAIN"}, username);
        } else if (object == ui->chaining) {
            ui->chan->call(Wipechan::TIP, {"CHAINING"}, username);
        } else if (object == ui->randTrigger) {
            ui->chan->call(Wipechan::TIP, {"RANDOM_TRIGGER"}, username);
        } else if (object == ui->shashlik) {
            ui->chan->call(Wipechan::TIP, {"SHASHLIK"}, username);
        } else if (object == ui->oppost) {
            ui->chan->call(Wipechan::TIP, {"OP-POST_TRIGGER"}, username);
        } else if (object == ui->notrigger) {
            ui->chan->call(Wipechan::TIP, {"NO_TRIGGER"}, username);
        } else if (object == ui->complainBoard) {
            ui->chan->call(Wipechan::TIP, {"COMPLAIN_BOARD"}, username);
            ui->complainBoard->setFocus();
        } else if (object == ui->linksCount) {
            ui->chan->call(Wipechan::TIP, {"LINKS_COUNT"}, username);
        } else if (object == ui->withPosts) {
            ui->chan->call(Wipechan::TIP, {"WITH_POSTS"}, username);
        } else if (object == ui->chaos) {
            ui->chan->call(Wipechan::TIP, {"CHAOS"}, username);
        } else if (object == ui->randChaos) {
            ui->chan->call(Wipechan::TIP, {"RANDOM_CHAOS"}, username);
        } else if (object == ui->target) {
            ui->chan->call(Wipechan::TIP, {"TARGET"}, username);
            ui->target->setFocus();
        } else if (object == ui->sage) {
            ui->chan->call(Wipechan::TIP, {"SAGE"}, username);
        } else if (object == ui->sageAlways) {
            ui->chan->call(Wipechan::TIP, {"SAGE_ALWAYS"}, username);
        } else if (object == ui->sageFromPosts) {
            ui->chan->call(Wipechan::TIP, {"SAGE_FROM_POSTS"}, username);

        } else if (object == ui->images) {
            ui->chan->call(Wipechan::TIP, {"IMAGES"}, username);
        } else if (object == ui->imagesCount) {
            ui->chan->call(Wipechan::TIP, {"IMAGES_COUNT"}, username);
        } else if (object == ui->imagesFolder) {
            ui->chan->call(Wipechan::TIP, {"IMAGES_FOLDER"}, username);
            ui->imagesFolder->setFocus();
        } else if (object == ui->videos) {
            ui->chan->call(Wipechan::TIP, {"VIDEOS"}, username);
        } else if (object == ui->videosCount) {
            ui->chan->call(Wipechan::TIP, {"VIDEOS_COUNT"}, username);
        } else if (object == ui->videosFolder) {
            ui->chan->call(Wipechan::TIP, {"VIDEOS_FOLDER"}, username);
            ui->videosFolder->setFocus();
        } else if (object == ui->medias) {
            ui->chan->call(Wipechan::TIP, {"MEDIAS_FROM_POSTS"}, username);
        } else if (object == ui->noMedia) {
            ui->chan->call(Wipechan::TIP, {"NO_MEDIA"}, username);
        } else if (object == ui->randMediaName) {
            ui->chan->call(Wipechan::TIP, {"RANDOM_MEDIA_NAME"}, username);
        } else if (object == ui->shackalDegree) {
            ui->chan->call(Wipechan::TIP, {"SHACKAL_DEGREE"}, username);
        } else if (object == ui->shackalColor) {
            ui->chan->call(Wipechan::TIP, {"COLORIZATION"}, username);
        } else if (object == ui->shackalAffine) {
            ui->chan->call(Wipechan::TIP, {"AFFINE"}, username);
        } else if (object == ui->toPNG) {
            ui->chan->call(Wipechan::TIP, {"TO_PNG"}, username);

        } else if (object == ui->totalPostsLabel) {
            ui->chan->call(Wipechan::TIP, {"TOTAL_POSTS"}, username);
        } else if (object == ui->totalPosts) {
            ui->chan->call(Wipechan::TIP, {"TOTAL_POSTS"}, username);
        } else if (object == ui->totalPostsLCD) {
            ui->chan->call(Wipechan::TIP, {"TOTAL_POSTS"}, username);
        } else if (object == ui->totalBansLabel) {
            ui->chan->call(Wipechan::TIP, {"TOTAL_BANS"}, username);
        } else if (object == ui->totalBans) {
            ui->chan->call(Wipechan::TIP, {"TOTAL_BANS"}, username);
        } else if (object == ui->totalBansLCD) {
            ui->chan->call(Wipechan::TIP, {"TOTAL_BANS"}, username);
        } else if (object == ui->postsLabel) {
            ui->chan->call(Wipechan::TIP, {"POSTS"}, username);
        } else if (object == ui->posts) {
            ui->chan->call(Wipechan::TIP, {"POSTS"}, username);
        } else if (object == ui->postsLCD) {
            ui->chan->call(Wipechan::TIP, {"POSTS"}, username);
        } else if (object == ui->proxiesLabel) {
            ui->chan->call(Wipechan::TIP, {"PROXIES"}, username);
        } else if (object == ui->proxies) {
            ui->chan->call(Wipechan::TIP, {"PROXIES"}, username);
        } else if (object == ui->proxiesLCD) {
            ui->chan->call(Wipechan::TIP, {"PROXIES"}, username);
        }

        return true;
    }
    return false;
}
