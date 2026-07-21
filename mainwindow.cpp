#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "listenerrepository.h"
#include "playlist.h"
#include <QInputDialog>
#include <QLineEdit>

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
    QString role = "Listener";

    // اگر دیتابیس وصل بود و کاربری واقعاً لاگین کرده بود
    if (db != nullptr && db->getCurrentAccount() != nullptr) {
        //یک اشاره گر به اکانت کاربری که تازه لاگین کرده می زنیم
        auto currentAccount = db->getCurrentAccount();
        // تبدیل stringبه qstring
        name = QString::fromStdString(currentAccount->getName());
        role = QString::fromStdString(currentAccount->getRole());
    }

    // بالا صفحه تیتر پنجره اصلی
    this->setWindowTitle("Spotify Manager - " + name);

    if (role == "Listener") {
        // تغییر متن و استایل هدر بالای صفحه
        if (ui->welcomeLabel != nullptr)
        {
            ui->welcomeLabel->setText("👤 Welcome back, " + name + "!");//متن لیبل خوشامدی گویی
            ui->welcomeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333333;");//فونت لیبل خوش امد گویی
        }
        // پر کردن لیست‌پ پخش وسط صفحه
        if (ui->playlistsListWidget != nullptr) {
            ui->playlistsListWidget->clear();// می گیم اگه لیست پخشی وجود داشت همشو پاک کن تا دوباره بنویسیم
            ui->playlistsListWidget->setStyleSheet(//ست کردن لیست پخش
                "QListWidget {"
                "   border: 2px solid #FFE6EE;"
                "   border-radius: 10px;"
                "   padding: 5px;"
                "}"
                "QListWidget::item {"
                "   padding: 12px;"
                "   font-size: 14px;"
                "   font-weight: bold;"
                "}"
            );
// اضافه کردن این ایکون ها به لیست پخش
            ui->playlistsListWidget->addItem("❤️  Favorite Songs");
            ui->playlistsListWidget->addItem("🎵  rokoshaa");
            ui->playlistsListWidget->addItem("🎵  Listen Later");
        }
        // استایل متنی می سازیم با فونت 18 و حذف کادر و ...
        QString flatIconStyle =
            "QPushButton {"
            "   background: transparent !important;"
            "   border: none !important;"
            "   font-size: 18px !important;"
            "   color: #333333 !important;"
            "   padding: 5px !important;"
            "}";

        // اگه دکمه ساخت لیست وجود داشت متنش به ایموجی ➕ تغییر بده و استایل اش را شیشه ای کن
        if (ui->createPlaylistButton != nullptr) {
            ui->createPlaylistButton->setText("➕");
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

        //  دکمه خروج
        QPushButton *logoutBtn = new QPushButton("🚪", this);
        logoutBtn->setStyleSheet(flatIconStyle);
// وصل می کنیمonLogoutClicked دکمه خروج را به تابع
        connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

        if (ui->horizontalLayout_2 != nullptr) {
            ui->horizontalLayout_2->addWidget(logoutBtn);//اضافه کردن دکمه خروج به پایین سمت راست
        }
    }
}
// تابع کلیک روی به علاوه
void MainWindow::onCreatePlaylistClicked()
{
    bool ok;
    //یک پنجره کوچک ورودی (QInputDialog) باز می‌کنیم که از کاربر نام لیست پخش را می‌گیریم
    QString playlistName = QInputDialog::getText(this, "Create Playlist","Enter playlist name:", QLineEdit::Normal,"", &ok);
// اگه دکمه اوکی را کاربر زد و اسمش خالی نبود
    if (ok && !playlistName.isEmpty()) {
        // اضافه کردن مستقیم به لیست‌ویجت برای تست ظاهر و کارکرد دکمه
        if (ui->playlistsListWidget != nullptr) {
            ui->playlistsListWidget->addItem("🎵  " + playlistName);
        }
        // پیغام موفقیت امیز
        QMessageBox::information(this, "Success", "Playlist '" + playlistName + "' created successfully!");
    }
}
// دکمه ویرایش
void MainWindow::onEditPlaylistClicked()
{
    //اگه کاربر هیچ البومی را انتخاب نکرده بود یا هیچ لیستی وجود نداشت
    if (ui->playlistsListWidget == nullptr || ui->playlistsListWidget->currentItem() == nullptr) {
        QMessageBox::warning(this, "Warning", "Please select a playlist to edit!");// هشدار زرد رنگ بده
        return;
    }
    // یه اشاره گر به ایتم انتخابی کاربر می زنیم
    QListWidgetItem* currentItem = ui->playlistsListWidget->currentItem();
    // متنش را استخراج می کنیم
    QString currentName = currentItem->text();
// اگه جزو فیوریت هامون باشه اجازه ویرایش نداریم
    if (currentName.contains("Favorite Songs")) {
        QMessageBox::warning(this, "Error", "You cannot edit the Favorite Songs playlist!");
        return;
    }
    // پاک کردن ایکون اهنگ برای نمایش نام ان
    QString cleanName = currentName.replace("🎵  ", "");

    bool ok;
    // یه کادر باز می کنیم و اسم قبلی را پیش فرض توش می زاریم و از کاربر می خوایم اسم جدید را بگه
    QString newName = QInputDialog::getText(this, "Edit Playlist", "Enter new name:",QLineEdit::Normal, cleanName, &ok);

    if (ok && !newName.isEmpty()) {
        currentItem->setText("🎵  " + newName);// ایکون موسیقی را به تهش می چسبونیم
        QMessageBox::information(this, "Success", "Playlist renamed successfully!");
    }
}
// تابع حذف
void MainWindow::onDeletePlaylistClicked()
{
    // اگه چیزی را انتخاب نکرده بود
    if (ui->playlistsListWidget == nullptr||ui->playlistsListWidget->currentItem() == nullptr) {
        QMessageBox::warning(this, "Warning", "Please select a playlist to delete!");
        return;
    }
    // متن ایتمی که قراره حذف بشه را می گیریم
    QString currentName = ui->playlistsListWidget->currentItem()->text();

    if (currentName.contains("Favorite Songs")) {
        QMessageBox::warning(this, "Error", "You cannot delete the Favorite Songs playlist!");
        return;
    }
// مطمین می شیم ایا واقعا می خاد حذفش کنه
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete this playlist?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // حذف آیتم از لیست پخش
        int currentRow = ui->playlistsListWidget->currentRow();
        delete ui->playlistsListWidget->takeItem(currentRow);
        QMessageBox::information(this, "Deleted", "Playlist deleted successfully!");
    }
}
//خروج از حساب
void MainWindow::onLogoutClicked()
{
    //مطمینی می خای بری
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Logout", "Are you sure you want to logout?",QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "Logged Out", "You have logged out successfully.");
        this->close();
    }
}
