#include <iostream>
#include <algorithm>

#include <QtCore>
#include <QVector>
#include <QLabel>
#include <QVBoxLayout>
#include <QAction>
#include <QDebug>
#include <QApplication>
#include <QCloseEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenuBar>
#include <QPixmap>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QPushButton>
#include <QMenu>

class MdiPixmap : public QLabel
{
    Q_OBJECT

    public:
        MdiPixmap(QImage &imgData);
        virtual ~MdiPixmap() = default;

        void resizeEvent(QResizeEvent *event);
    protected:
        QPixmap _pixmap;
        QHBoxLayout* _layout;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        virtual ~MainWindow() = default;
    protected:
        void createActions();
        void createMenus();
        void loadAsset(const QString &fileName);
        void ReadPixel(quint16 pixel, quint8 &r, quint8 &g, quint8 &b);

        void closeEvent(QCloseEvent *event) override;
    private slots:
        void open();
        void save();
        void about();
    private:
        QMdiArea *mdiArea;
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *helpMenu;
        QAction *openAction;
        QAction *saveAction;
        QAction *exitAction;
        QAction *aboutAction;
};