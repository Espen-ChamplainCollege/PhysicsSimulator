# PhysicsSimulator

## Contributors

Espen Wold
Harrison Labrecuqe
## Getting started

You should have [Qt](https://www.qt.io/download-qt-installer-oss) installed already.

When installing Qt, you should also install Qt Creator. It should be an option in the Qt installer.

Clone this repo `git clone https://github.com/Espen-ChamplainCollege/PhysicsSimulator`

Move to the dev branch `git checkout dev`

Create a branch to work on `git checkout -b yourBranchName`

Now if you open Qt Creator, and go to open project, you can open the file `CMakeLists.txt` to load the project in Qt Creator
(There is no "Qt project file", it just lives in the `CMakeLists.txt` file.)

At this point you can work on the project like usual. Remember to `git add (files)` to add your changes anda `git commit -m "message"` to commit them.

## Program Structure

Like most C++ programs, `main.cpp` is the entry point of the application.

```
  QApplication a(argc, argv);   // <- Create a QApplication object. This object is basically the container for our application.
  MainWindow w;                 // <- Create a MainWindow object. This is self explanatory, it's just the window.
  w.show();                     // <- Show the window. The window starts out hidden, so we have to call this to show it.
  return a.exec();              // <- This line pretty much hands the process over to the application, which wont return the process until we exit the app.
```

`mainwindow.h` is the header file for our MainWindow class.

```
QT_BEGIN_NAMESPACE;                     // <- I do not know what this line and the end namespace line do, but I do know they are macros. Just ignore them for now.
namespace Ui { class MainWindow; }      // <- In the Ui namespace, we declare the class MainWindow.
QT_END_NAMESPACE;

class MainWindow : public QMainWindow { Q_OBJECT    // <- Now we define the class MainWindow. public QMainWindow is saying that it is derived from QMainWindow. Q_OBJECT is another macro.

public:
  MainWindow(QWidget *parent = nullptr);            // <- Constructor, nothing special
  ~MainWindow();                                    // <- Destructor, nothing special

private:
  Ui::MainWindow *ui;                               // <- A pointer to the window, nothing special
};
```

`mainwindow.cpp` is the source file for MainWindow.

```
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {     // <- Constructor definition
  ui->setupUi(this);                                // <- We call this to setup the UI.
}

MainWindow::~MainWindow(){                          // <- Destructor definition
  delete ui;                                        // <- Delete the UI.
}
```
