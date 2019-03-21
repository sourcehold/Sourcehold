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
        
        quint16 width, height, dummy;
        in >> width;
        in >> dummy;
        in >> height;
        in >> dummy;

        img = QImage(width, height, QImage::Format_RGB32);
        img.fill(Qt::black);

        while(!in.atEnd()) {
            quint8 token;
            in >> token;

            quint8 len = (token & 0b11111) + 1;
            quint8 flag = token >> 5;

            switch(flag) {
                case 0b000: {
                    for(quint8 i = 0; i < len; ++i,++x) {
                        qint16 pixelColor;
                        in >> pixelColor;
                        quint8 r,g,b;
                        ReadPixel(pixelColor, r, g, b);
                        img.setPixelColor(x, y, QColor(r, g, b, 255));
                    }
                }break;
                case 0b001: {
                    for(quint8 i = 0; i < len; i++, x++) {
                        img.setPixelColor(x, y, QColor(0, 0, 0, 0));
                    }
                }break;
                case 0b010: {
                    quint16 pixelColor;
                    in >> pixelColor;
                    quint8 r,g,b;
                    ReadPixel(pixelColor, r, g, b);
                    for(quint8 i = 0; i < len; ++i,++x) {
                        img.setPixelColor(x, y, QColor(r, g, b, 255));
                    }
                }break;
                case 0b100: {
                    y++;
                    x = 0;
                }break;
                default: {
                    QMessageBox::information(this, tr("Error"), "Invalid token in tgx!");
                    return;
                }break;
            }
        }

        file.close();
    }else if(suffix == "gm1") {
        struct Entry {
            quint16 width, height, offsetX, offsetY, part, parts;
            quint8 direction, horizOffset, partWidth, color;
            quint32 size, offset;
        };

        quint16 palette[2560];
        quint32 num, type, len, maxWidth = 0, maxHeight = 0, offData = 0;

        in.skipRawData(3*sizeof(quint32));
        in >> num;
        in.skipRawData(1*sizeof(quint32));
        in >> type;
        in.skipRawData(14*sizeof(quint32));
        in >> len;
        in.skipRawData(1*sizeof(quint32));

        in.readRawData((char*)palette, sizeof(palette));

        QVector<Entry> entries(num);

        for(quint32 n = 0; n < num; n++) {
            in.readRawData((char*)&entries[n].offset, sizeof(quint32));
        }

        for(quint32 n = 0; n < num; n++) {
            in.readRawData((char*)&entries[n].size, sizeof(quint32));
        }

        for(quint32 n = 0; n < num; n++) {
            in >> entries[n].width;
            in >> entries[n].height;
            in >> entries[n].offsetX;
            in >> entries[n].offsetY;
            in >> entries[n].part;
            in >> entries[n].parts;
            in >> entries[n].direction;
            in >> entries[n].horizOffset;
            in >> entries[n].partWidth;
            in >> entries[n].color;

            maxWidth = std::max<quint32>(maxWidth, entries[n].width);
            maxHeight = std::max<quint32>(maxHeight, entries[n].height);
        }

        offData =  in.device()->pos();

        img = QImage(maxWidth, maxHeight, QImage::Format_RGB32);
        img.fill(Qt::black);

        for(quint32 index = 0; index < num; index++) {
            in.device()->seek(offData + entries[index].offset);

            switch(type) {
                case 1 : case 4 : case 6: {

                }break;
                case 2: {

                }break;
                case 3: {

                }break;
                case 5 : case 7: {

                }break;
                default: break;
            }
        }

        file.close();
    }else return;

    MdiPixmap *pixmap = new MdiPixmap(img);
    QMdiSubWindow *child = mdiArea->addSubWindow(pixmap);

    child->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    child->layout()->setSizeConstraint(QLayout::SetNoConstraint);
    child->setAttribute(Qt::WA_DeleteOnClose);
    child->setWindowTitle(suffix + ": " + baseName);
    child->show();
}

void MainWindow::ReadPixel(quint16 pixel, quint8 &r, quint8 &g, quint8 &b) {
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