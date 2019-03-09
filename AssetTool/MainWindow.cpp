#include <MainWindow.hh>

MdiPixmap::MdiPixmap(QImage &imgData) : QLabel() {
    _pixmap = QPixmap();
    _pixmap.convertFromImage(imgData, Qt::ColorOnly);
    setPixmap(_pixmap);
    _layout = new QHBoxLayout();
    _layout->setSizeConstraint(QLayout::SetNoConstraint);
    setLayout(_layout);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    setMinimumSize(_pixmap.width(), _pixmap.height());
}

void MdiPixmap::resizeEvent(QResizeEvent *event) {
    int width = event->size().width();
    int height = event->size().height();
    setPixmap(_pixmap.scaled(width,height));
    QLabel::resizeEvent(event);
}

MainWindow::MainWindow() : mdiArea(new QMdiArea) {
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    QHBoxLayout *mdiLayout = new QHBoxLayout();
    mdiLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mdiArea->setLayout(mdiLayout);

    createActions();
    createMenus();

    setWindowTitle(tr("Sourcehold asset manager"));
    setMinimumSize(100, 100);
    showMaximized();
}

void MainWindow::createActions() {
    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an asset file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save an asset file"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("About this app"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::loadAsset(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open asset"), file.errorString());
        return;
    }

    QFileInfo info(file);
    const QString suffix = info.completeSuffix();
    const QString baseName = info.completeBaseName();

    QImage img;
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    if(suffix == "tgx") {
        uint32_t x = 0, y = 0;

        qint16 width, height, dummy;
        in >> width;
        in >> dummy;
        in >> height;
        in >> dummy;

        img = QImage(width, height, QImage::Format_RGB32);
        img.fill(Qt::black);

        while(!file.atEnd()) {
            qint8 token;
            in >> token;

            qint8 len = (token & 0b11111) + 1;
            qint8 flag = token >> 5;

            switch(flag) {
                case 0b000: {
                    for(qint8 i = 0; i < len; ++i,++x) {
                        qint16 pixelColor;
                        in >> pixelColor;
                        qint8 r,g,b;
                        ReadPixel(pixelColor, r, g, b);
                        img.setPixelColor(x, y, QColor(r, g, b, 255));
                    }
                }break;
                case 0b001: {
                    for(qint8 i = 0; i < len; i++, x++) {
                        img.setPixelColor(x, y, QColor(0, 0, 0, 0));
                    }
                }break;
                case 0b010: {
                    qint16 pixelColor;
                    in >> pixelColor;
                    qint8 r,g,b;
                    ReadPixel(pixelColor, r, g, b);
                    for(qint8 i = 0; i < len; ++i,++x) {
                        img.setPixelColor(x, y, QColor(r, g, b, 255));
                    }
                }break;
                case 0b100: {
                    y++;
                    x = 0;
                }break;
                default: break;
            }
        }

        file.close();
    }else if(suffix == "gm1") {
        file.close();
    }else return;

    MdiPixmap *pixmap = new MdiPixmap(img);

    QMdiSubWindow *child = new QMdiSubWindow(mdiArea);

    child->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    child->layout()->setSizeConstraint(QLayout::SetNoConstraint);
    child->layout()->addWidget(pixmap);
    child->setAttribute(Qt::WA_DeleteOnClose);
    child->setWindowTitle(baseName);
    child->show();
}

void MainWindow::ReadPixel(qint16 pixel, qint8 &r, qint8 &g, qint8 &b) {
    r = ((pixel >> 10) & 0b11111) << 3;
    g = (((pixel >> 5) & 0b11111) | ((pixel >> 8) & 0b11)) << 3;
    b = (pixel & 0b11111) << 3;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open asset"), "",
            tr("Stronghold assets (*.tgx *.gm1)"));
            
    if (!fileName.isEmpty()) {
        loadAsset(fileName);
    }
}

void MainWindow::save() {

}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Menu"), 
        tr("COPYRIGHT (C) 2019 Julian Offenhäuser (metalvoidzz@gmail.com)"));
}
