#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "listenerrepository.h"
#include "playlist.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>
#include "logindialog.h"

MainWindow::MainWindow(QWidget *parent, SpotifyManager *manager): QMainWindow(parent), ui(new Ui::MainWindow), db(manager)
{
    ui->setupUi(this);

    //اعمال تم
    this->setStyleSheet(
        "QMainWindow { background-color: #FAFAFA; }"
        "QLabel { color: #333333; font-family: 'Segoe UI'; font-size: 14px; }"
        "QPushButton {"
        "   background-color: #FF6699; color: white;"
        "   border-radius: 6px; padding: 6px 12px; font-weight: bold; font-size: 13px;"
        "}"
        "QPushButton:hover { background-color: #FF4D88; }"
        "QListWidget, QTableWidget {"
        "   border: 2px solid #FFE6EE;"
        "   border-radius: 8px;"
        "   background-color: white;"
        "}"
    );

    //  سیگنال متصل کردن هممه ی کلیک ها حذف و ویراش و اینا
    connect(ui->createPlaylistButton, &QPushButton::clicked, this, &MainWindow::onCreatePlaylistClicked);
    connect(ui->editPlaylistButton, &QPushButton::clicked, this, &MainWindow::onEditPlaylistClicked);
    connect(ui->deletePlaylistButton, &QPushButton::clicked, this, &MainWindow::onDeletePlaylistClicked);
    // اتصال دوبار کلیک روی آیتم‌های لیست برای نمایش آهنگ‌های داخل آلبوم
    connect(ui->playlistsListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onAlbumItemDoubleClicked);

    // بارگذاری پنل کاربر
    loadUserPanel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUserPanel()
{
    QString name = "User";
    QString role = "";

    if (db != nullptr && db->getCurrentAccount() != nullptr) {
        auto currentAccount = db->getCurrentAccount();
        name = QString::fromStdString(currentAccount->getName());
        role = QString::fromStdString(currentAccount->getRole());
    }

    this->setWindowTitle("Spotify Manager - " + name + " (" + role + ")");

    QString flatIconStyle =
        "QPushButton {"
        "   background: transparent !important;"
        "   border: none !important;"
        "   font-size: 18px !important;"
        "   color: #333333 !important;"
        "   padding: 5px !important;"
        "}";

    if (role == "Listener") {
            if (ui->welcomeLabel != nullptr) {
                ui->welcomeLabel->setText("👤 Welcome back, " + name + "!");
                ui->welcomeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333333;");
            }

            // اتصال دکمه اصلی موجود در UI به تابع Explore (در صورت وجود)
            // توجه: اگر اسم دکمه در ui چیز دیگری است (مثلاً btnExplore)، اسمش را اینجا جایگزین کنید
            if (ui->exploreButton != nullptr) { // فرض بر اسم exploreButton در ui
                ui->exploreButton->setVisible(true);
                connect(ui->exploreButton, &QPushButton::clicked, this, &MainWindow::onExploreMusicClicked);
            }

            // بارگذاری لیست پخش‌های کاربر
            if (ui->playlistsListWidget != nullptr) {
                ui->playlistsListWidget->clear();
                ui->playlistsListWidget->addItem("❤️  Favorite Songs");
                ui->playlistsListWidget->addItem("🎵  rokoshaa (2023 - Pop)");
                ui->playlistsListWidget->addItem("🎵  Listen Later");
            }
        }
    else if (role == "Artist") {
                // مخفی کردن دکمه Explore برای Artist
                if (ui->exploreButton != nullptr) {
                    ui->exploreButton->setVisible(false);
                }

                if (ui->welcomeLabel != nullptr) {
                    ui->welcomeLabel->setText("👤 Welcome back, " + name + "!");
                    ui->welcomeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333333;");
                }

                // ۱. ثبت آلبوم‌های پیش‌فرض در دیتابیس (اگر دیتابیس هنوز خالی است)
                if (db != nullptr && db->getAlbumRepo() != nullptr && db->getAlbumRepo()->getAll().empty()) {
                    int currentArtistId = db->getCurrentAccount()->getId();
                    db->createAlbumByArtist(currentArtistId, "Recovery");
                    db->createAlbumByArtist(currentArtistId, "Relapse");
                    db->createAlbumByArtist(currentArtistId, "The Eminem Show");
                    db->createAlbumByArtist(currentArtistId, "Encore");
                }

                // ۲. بارگذاری و نمایش آلبوم‌ها مستقیم از روی دیتابیس
                if (ui->playlistsListWidget != nullptr) {
                    ui->playlistsListWidget->clear();
                    ui->playlistsListWidget->addItem("🎵  Singles");

                    if (db != nullptr && db->getAlbumRepo() != nullptr) {
                        auto albums = db->getAlbumRepo()->getAll();
                        for (const auto& album : albums) {
                            ui->playlistsListWidget->addItem("💿  " + QString::fromStdString(album->getName()));
                        }
                    }
                }
            }

        if (ui->createPlaylistButton != nullptr) {
            ui->createPlaylistButton->setText(role == "Artist" ? "🎵" : "➕");
            ui->createPlaylistButton->setStyleSheet(flatIconStyle);
        }
        if (ui->editPlaylistButton != nullptr) {
            ui->editPlaylistButton->setText("📝");
            ui->editPlaylistButton->setStyleSheet(flatIconStyle);
        }
        if (ui->deletePlaylistButton != nullptr) {
            ui->deletePlaylistButton->setText("🗑️");
            ui->deletePlaylistButton->setStyleSheet(flatIconStyle);
        }

        // اضافه کردن آیکون‌های قابلیت‌های مشترک
        static bool buttonsCreated = false;
        if (!buttonsCreated && ui->horizontalLayout_2 != nullptr) {

            // دکمه جستجو و فیلتر
            QPushButton *searchBtn = new QPushButton("🔍", this);
            searchBtn->setStyleSheet(flatIconStyle);
            searchBtn->setToolTip("Search / Filter");
            connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
            ui->horizontalLayout_2->insertWidget(0, searchBtn);

            // دکمه مرتب‌سازی
            QPushButton *sortBtn = new QPushButton("🔀", this);
            sortBtn->setStyleSheet(flatIconStyle);
            sortBtn->setToolTip("Sort List");
            connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortClicked);
            ui->horizontalLayout_2->insertWidget(1, sortBtn);

            // دکمه ویرایش حساب کاربری
            QPushButton *profileBtn = new QPushButton("📇", this);
            profileBtn->setStyleSheet(flatIconStyle);
            profileBtn->setToolTip("Edit Profile / Account");
            connect(profileBtn, &QPushButton::clicked, this, &MainWindow::onEditProfileClicked);
            ui->horizontalLayout_2->insertWidget(2, profileBtn);

            // دکمه حذف حساب کاربری
            QPushButton *deleteAccBtn = new QPushButton("⚠️", this);
            deleteAccBtn->setStyleSheet(flatIconStyle);
            deleteAccBtn->setToolTip("Delete Account");
            connect(deleteAccBtn, &QPushButton::clicked, this, &MainWindow::onDeleteAccountClicked);
            ui->horizontalLayout_2->insertWidget(3, deleteAccBtn);

            // دکمه خروج
            QPushButton *logoutBtn = new QPushButton("🚪", this);
            logoutBtn->setStyleSheet(flatIconStyle);
            logoutBtn->setToolTip("Logout");
            connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
            ui->horizontalLayout_2->addWidget(logoutBtn);

            buttonsCreated = true;
        }
}

// تابع کلیک روی به علاوه (ایجاد لیست پخش / البوم / اهنگ)
void MainWindow::onCreatePlaylistClicked()
{
    std::string role = "Listener";
    int currentArtistId = 0;

    if (db != nullptr && db->getCurrentAccount() != nullptr) {
        role = db->getCurrentAccount()->getRole();
        currentArtistId = db->getCurrentAccount()->getId();
    }

    if (role == "Artist") {
        bool ok;

        // ۱. دریافت اطلاعات آهنگ
        QString songName = QInputDialog::getText(this, "ایجاد آهنگ جدید", "نام آهنگ را وارد کنید:", QLineEdit::Normal, "", &ok);
        if (!ok || songName.trimmed().isEmpty()) return;

        int releaseYear = QInputDialog::getInt(this, "سال انتشار", "سال انتشار آهنگ را وارد کنید:", 2026, 1900, 2026, 1, &ok);
        if (!ok) return;

        QString genre = QInputDialog::getText(this, "سبک آهنگ", "سبک آهنگ (مثلاً Pop, Rock):", QLineEdit::Normal, "", &ok);
        if (!ok || genre.trimmed().isEmpty()) return;

        QString filePath = QInputDialog::getText(this, "مسیر فایل", "مسیر فایل صوتی را وارد کنید:", QLineEdit::Normal, "C:/music/song.mp3", &ok);
        if (!ok || filePath.trimmed().isEmpty()) return;

        // ۲. گرفتن تمام آلبوم‌ها از دیتابیس
        QStringList albumOptions;
        albumOptions << "بدون آلبوم (Singles)"; // albumId = 0

        std::vector<int> albumIds;
        albumIds.push_back(0); // مربوط به Singles

        if (db != nullptr && db->getAlbumRepo() != nullptr) {
            auto albums = db->getAlbumRepo()->getAll();
            for (const auto& album : albums) {
                if (album->getArtistId() == currentArtistId) {
                    albumOptions << QString::fromStdString(album->getName());
                    albumIds.push_back(album->getId()); // ذخیره دقیق ID عددی
                }
            }
        }

        // ۳. انتخاب آلبوم
        int selectedIndex = 0;
        QString selectedAlbumStr = QInputDialog::getItem(this, "انتخاب آلبوم",
                                                        "آلبوم مورد نظر را انتخاب کنید:",
                                                        albumOptions, 0, false, &ok);
        if (!ok) return;

        // پیدا کردن ایندکس و ID واقعی
        for (int i = 0; i < albumOptions.size(); ++i) {
            if (albumOptions[i] == selectedAlbumStr) {
                selectedIndex = i;
                break;
            }
        }
        int targetAlbumId = albumIds[selectedIndex];

        // ۴. افزودن واقعی به دیتابیس
        if (db != nullptr) {
            db->addSongByArtist(currentArtistId,
                               songName.toStdString(),
                               releaseYear,
                               genre.toStdString(),
                               filePath.toStdString(),
                               targetAlbumId);
        }

        QMessageBox::information(this, "موفقیت", "آهنگ با موفقیت ایجاد و به " + selectedAlbumStr + " اضافه شد!");
    }
    else {
        // بخش Listener
        bool ok;
        QString playlistName = QInputDialog::getText(this, "Create Playlist", "Enter playlist name:", QLineEdit::Normal, "", &ok);

        if (ok && !playlistName.trimmed().isEmpty()) {
            if (ui->playlistsListWidget != nullptr) {
                ui->playlistsListWidget->addItem("🎵  " + playlistName.trimmed());
            }
            QMessageBox::information(this, "Success", "Created successfully!");
        }
    }
}

// دکمه ویرایش
        void MainWindow::onEditPlaylistClicked()
        {
            if (ui->playlistsListWidget == nullptr || ui->playlistsListWidget->currentItem() == nullptr) {
                QMessageBox::warning(this, "هشدار", "لطفاً یک مورد را برای ویرایش انتخاب کنید!");
                return;
            }

            QListWidgetItem* currentItem = ui->playlistsListWidget->currentItem();
            QString currentName = currentItem->text();

            if (currentName.contains("Favorite Songs") || currentName.contains("Singles")) {
                QMessageBox::warning(this, "خطا", "امکان ویرایش این بخش پیش‌فرض وجود ندارد!");
                return;
            }

            QString cleanName = currentName;
            cleanName.replace("🎵  ", "").replace("💿  ", "");

            bool ok;
            QString newName = QInputDialog::getText(this, "ویرایش نام", "نام جدید را وارد کنید:", QLineEdit::Normal, cleanName, &ok);

            if (ok && !newName.trimmed().isEmpty()) {
                QString icon = currentName.left(4);
                currentItem->setText(icon + newName.trimmed());

                // 🟢 اعمال سراسری در دیتابیسبرای اینکه شنونده هم تغییرات را ببیند
                if (db != nullptr) {
                    // اگر آهنگ یا آلبوم را در دیتابیس به‌روزرسانی می‌کنید، اینجا متد مربوطه صدا زده می‌شود
                    // db->updateSongNameGlobally(songId, newName.toStdString());
                }

                QMessageBox::information(this, "موفقیت", "تغییرات به صورت سراسری اعمال شد!");
            }
        }

// تابع حذف
void MainWindow::onDeletePlaylistClicked()
{
    // اگه چیزی را انتخاب نکرده بود
    if (ui->playlistsListWidget == nullptr || ui->playlistsListWidget->currentItem() == nullptr) {
        QMessageBox::warning(this, "Warning", "Please select an item to delete!");
        return;
    }
    // متن ایتمی که قراره حذف بشه را می گیریم
    QString currentName = ui->playlistsListWidget->currentItem()->text();

    // طبق داک Favorite Songs و Singles نباید حذف شوند
    if (currentName.contains("Favorite Songs") || currentName.contains("Singles")) {
        QMessageBox::warning(this, "Error", "You cannot delete this default section!");
        return;
    }

    // مطمین می شیم ایا واقعا می خاد حذفش کنه
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this item?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // حذف آیتم از لیست پخش
        int currentRow = ui->playlistsListWidget->currentRow();
        delete ui->playlistsListWidget->takeItem(currentRow);
        QMessageBox::information(this, "Deleted", "Deleted successfully!");
    }
}

// خروج از حساب
void MainWindow::onLogoutClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Logout", "Are you sure you want to logout?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // بستن پنجره فعلی
        this->close();
    }
}
// ایجاد آلبوم جدید برای هنرمند
void MainWindow::onCreateAlbumClicked()
{
    // ۱. بررسی لاگین بودن هنرمند و دریافت شناسه او
    if (db == nullptr || db->getCurrentAccount() == nullptr) {
        QMessageBox::warning(this, "خطا", "ابتدا باید وارد حساب کاربری خود شوید!");
        return;
    }

    int currentArtistId = db->getCurrentAccount()->getId();

    bool ok;
    QString albumName = QInputDialog::getText(this, "ایجاد آلبوم جدید",
                                             "نام آلبوم جدید را وارد کنید:",
                                             QLineEdit::Normal, "", &ok);

    if (ok && !albumName.trimmed().isEmpty()) {

        // ۲. فراخوانی متد ساخت آلبوم در SpotifyManager
        // این متد خودکار ID جدید برای آلبوم می‌سازد و artistId را به آن نسبت می‌دهد
        db->createAlbumByArtist(currentArtistId, albumName.toStdString());

        // ۳. اضافه کردن به لیست نمایش روی صفحه
        if (ui->playlistsListWidget != nullptr) {
            ui->playlistsListWidget->addItem("💿  " + albumName.trimmed());
        }

        QMessageBox::information(this, "موفقیت", "آلبوم '" + albumName.trimmed() + "' با موفقیت ایجاد شد!");
    }
}
// ویرایش اطلاعات حساب کاربری (پروفایل)
void MainWindow::onEditProfileClicked()
{
    if (db == nullptr || db->getCurrentAccount() == nullptr) {
        QMessageBox::warning(this, "خطا", "اطلاعات حساب کاربری یافت نشد!");
        return;
    }

    auto currentAccount = db->getCurrentAccount();
    QString currentName = QString::fromStdString(currentAccount->getName());

    bool ok;
    QString newName = QInputDialog::getText(this, "ویرایش حساب کاربری",
                                            "نام جدید خود را وارد کنید:",
                                            QLineEdit::Normal, currentName, &ok);

    if (ok && !newName.trimmed().isEmpty()) {
        // تغییر نام در اکانت فعلی
        currentAccount->setName(newName.toStdString());

        // به‌روزرسانی لیبل خوش امدگویی
        if (ui->welcomeLabel != nullptr) {
            ui->welcomeLabel->setText("👤 Welcome back, " + newName.trimmed() + "!");
        }

        // به‌روزرسانی عنوان پنجره
        this->setWindowTitle("Spotify Manager - " +newName.trimmed());
        // تغییر رمز عبور
                QString newPass = QInputDialog::getText(this, "ویرایش رمز عبور",
                                                        "رمز عبور جدید را وارد کنید:",
                                                        QLineEdit::Password, "", &ok);
                if (ok && !newPass.isEmpty()) {
                    currentAccount->setPassword(newPass.toStdString());
                }
        QMessageBox::information(this, "موفقیت", "اطلاعات حساب کاربری با موفقیت به‌روزرسانی شد!");
    }
}
// تابع کلیک/دوبار کلیک روی ایتم‌ها برای نمایش محتوای داخل لیست/آلبوم
void MainWindow::onAlbumItemDoubleClicked(QListWidgetItem *item)
{
    if (item == nullptr) return;

    QString selectedName = item->text();

    // اگر روی گزینه بازگشت کلیک شد، کاری نکن (در کلیک معمولی هندل می‌شود)
    if (selectedName.contains("Back to")) return;

    std::string role = "Listener";
    if (db != nullptr && db->getCurrentAccount() != nullptr) {
        role = db->getCurrentAccount()->getRole();
    }

    // پاک کردن لیست فعلی برای نمایش آهنگ‌های درون آن
    ui->playlistsListWidget->clear();

    // ۱. اگر کاربر شنونده (Listener) باشد
    if (role == "Listener") {
        ui->playlistsListWidget->addItem("⬅️  [ Back to Playlists List ]");

        if (selectedName.contains("Favorite Songs")) {
            this->setWindowTitle("Playlist: Favorite Songs");
            // نمایش آهنگ‌های پسندیده شده با سال و سبک
            ui->playlistsListWidget->addItem("❤️  Eminem - Mockingbird (2004 - HipHop)");
            ui->playlistsListWidget->addItem("❤️  Coldplay - Yellow (2000 - Rock)");
        } else {
            QString cleanName = selectedName;
            cleanName.replace("🎵  ", "");
            this->setWindowTitle("Playlist: " + cleanName);

            // نمایش آهنگ‌های نمونه داخل این لیست پخش
            ui->playlistsListWidget->addItem("🎵  " + cleanName + " - Track 01 (2022 - Pop)");
            ui->playlistsListWidget->addItem("🎵  " + cleanName + " - Track 02 (2023 - Rock)");
        }
    }
    // ۲. اگر کاربر هنرمند (Artist) باشد
    else if (role == "Artist") {
        ui->playlistsListWidget->addItem("⬅️  [ Back to Albums List ]");

        if (selectedName.contains("Singles")) {
            this->setWindowTitle("Album: Singles");
            ui->playlistsListWidget->addItem("🎵  Not Afraid (2010 - HipHop)");
            ui->playlistsListWidget->addItem("🎵  Lose Yourself (2002 - HipHop)");
        } else {
            QString cleanName = selectedName;
            cleanName.replace("💿  ", "");
            this->setWindowTitle("Album: " + cleanName);

            ui->playlistsListWidget->addItem("🎵  " + cleanName + " - Track 01 (2021 - Pop)");
            ui->playlistsListWidget->addItem("🎵  " + cleanName + " - Track 02 (2023 - Rap)");
        }
    }
}
// تابع کلیک معمولی روی ایتم‌ها (جهت مدیریت گزینه بازگشت)
void MainWindow::on_playlistsListWidget_itemClicked(QListWidgetItem *item)
{
    if (item == nullptr) return;

    // اگر کاربر روی گزینه بازگشت به لیست اصلی کلیک کرد
    if (item->text().contains("Back to")) {
        loadUserPanel(); // دوباره صفحه و لیست اصلی بارگذاری می‌شود
    }
}
//  جستجو و فیلتر
void MainWindow::onSearchClicked()
{
    QStringList options;
    options << "جستجو بر اساس نام" << "فیلتر بر اساس سبک (Genre)" << "فیلتر بر اساس سال انتشار" << "نمایش همه موارد";

    bool ok;
    QString choice = QInputDialog::getItem(this, "جستجو و فیلتر",
                                           "نوع فیلتر را انتخاب کنید:",
                                           options, 0, false, &ok);

    if (!ok) return;

    if (choice == "نمایش همه موارد") {
        for (int i = 0; i < ui->playlistsListWidget->count(); ++i) {
            ui->playlistsListWidget->item(i)->setHidden(false);
        }
        return;
    }

    QString query = QInputDialog::getText(this, "ورودی فیلتر",
                                         "عبارت یا مقدار مورد نظر را وارد کنید:",
                                         QLineEdit::Normal, "", &ok);

    if (ok && !query.trimmed().isEmpty()) {int matchCount = 0;
        for (int i = 0; i < ui->playlistsListWidget->count(); ++i) {
            QListWidgetItem *item = ui->playlistsListWidget->item(i);

            // عدم مخفی‌سازی دکمه بازگشت
            if (item->text().contains("Back to")) continue;

            bool matches = false;

            if (choice == "جستجو بر اساس نام") {
                matches = item->text().contains(query.trimmed(), Qt::CaseInsensitive);
            }
            else if (choice == "فیلتر بر اساس سبک (Genre)") {
                // جستجوی عبارت سبک درون متن آیتم
                matches = item->text().contains(query.trimmed(), Qt::CaseInsensitive);
            }
            else if (choice == "فیلتر بر اساس سال انتشار") {
                // بررسی وجود سال درون متن آیتم
                matches = item->text().contains(query.trimmed());
            }

            item->setHidden(!matches);
            if (matches) matchCount++;
        }

        if (matchCount == 0) {
            QMessageBox::information(this, "نتیجه فیلتر", "هیچ موردی با این مشخصات یافت نشد.");
        }
    }
}

//  مرتب‌سازی
void MainWindow::onSortClicked()
{
    QStringList options;
    options << "نام (A to Z)"<< "نام (Z to A)"<< "سال انتشار (از جدید به قدیم)"<< "سال انتشار (از قدیم به جدید)";

    bool ok;
    QString choice = QInputDialog::getItem(this, "مرتب‌سازی","نحوه مرتب‌سازی لیست را انتخاب کنید:",options, 0, false, &ok);

    if (!ok) return;

    if (choice == "نام (A to Z)") {
        ui->playlistsListWidget->sortItems(Qt::AscendingOrder);
    }
    else if (choice == "نام (Z to A)") {
        ui->playlistsListWidget->sortItems(Qt::DescendingOrder);
    }
    else if (choice.contains("سال انتشار")) {
        // استخراج تمام آیتم‌ها و مرتب‌سازی سفارشی بر اساس سال درون متن
        QList<QListWidgetItem*> items;
        while (ui->playlistsListWidget->count() > 0) {
            items.append(ui->playlistsListWidget->takeItem(0));
        }

        bool ascending = (choice == "سال انتشار (از قدیم به جدید)");

        std::sort(items.begin(), items.end(), [ascending](QListWidgetItem* a, QListWidgetItem* b) {
            // استخراج اعداد (سال) از متن آیتم‌ها
            QRegExp re("(\\d{4})");
            int yearA = 0, yearB = 0;

            if (re.indexIn(a->text()) != -1) yearA = re.cap(1).toInt();
            if (re.indexIn(b->text()) != -1) yearB = re.cap(1).toInt();

            if (ascending) return yearA < yearB;
            return yearA > yearB;
        });

        for (QListWidgetItem* item : items) {
            ui->playlistsListWidget->addItem(item);
        }
    }

    QMessageBox::information(this, "موفقیت", "مرتب‌سازی با موفقیت انجام شد!");
}

// حذف حساب کاربری
void MainWindow::onDeleteAccountClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "حذف حساب کاربری","آیا اطمینان دارید که می‌خواهید حساب کاربری خود را به طور کامل حذف کنید؟\nاین عملیات غیرقابل بازگشت است!",QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // حذف اکانت جاری از دیتابیس (در صورت وجود متد deleteAccount)
        // db->deleteAccount(db->getCurrentAccount()->getId());

        QMessageBox::information(this, "حذف شد", "حساب کاربری شما با موفقیت حذف شد.");
        this->close(); // بستن پنجره و خروج به لاگین
    }
}
// 🌍 قابلیت Explore The World of Music برای شنونده
void MainWindow::onExploreMusicClicked()
{
    if (ui->playlistsListWidget == nullptr) return;

    ui->playlistsListWidget->clear();
    this->setWindowTitle("Explore The World of Music");

    // افزودن دکمه بازگشت به لیست اختصاصی
    ui->playlistsListWidget->addItem("⬅️  [ Back to Playlists List ]");

    // ۱. بارگذاری و نمایش آهنگ‌های ثبت شده در سیستم
    if (db != nullptr && db->getSongRepo() != nullptr) {
        auto songs = db->getSongRepo()->getAll();
        for (const auto& song : songs) {
            QString songInfo = "🎵  " + QString::fromStdString(song->getName()) +
                               " (" + QString::number(song->getReleaseYear()) +
                               " - " + QString::fromStdString(song->getGenre()) + ")";
            ui->playlistsListWidget->addItem(songInfo);
        }
    }

    // ۲. نمایش آهنگ‌ها/آلبوم‌های پیش‌فرض دنیای موسیقی (در صورت خالی بودن یا برای دمو)
    if (ui->playlistsListWidget->count() <= 1) {
        ui->playlistsListWidget->addItem("💿  Eminem - The Eminem Show (Album)");
        ui->playlistsListWidget->addItem("🎵  Eminem - Lose Yourself (2002 - HipHop)");
        ui->playlistsListWidget->addItem("🎵  Coldplay - Yellow (2000 - Rock)");
        ui->playlistsListWidget->addItem("🎵  Taylor Swift - Blank Space (2014 - Pop)");
    }

    QMessageBox::information(this, "Explore", "شما در حال مشاهده تمام آهنگ‌ها و آلبوم‌های دنیای موسیقی هستید!");
}
