# PhysicsSimulator

## Contributors
Connor Hill
Espen Wold
Harrison Labrecque
Lorenzo Zimmerer
Tyler Chasse
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

# Qt For Webassembly
[QT Documentataion](https://doc.qt.io/qt-6/wasm.html)

First, install "WebAssembly (multi-threaded)" from version 6.6.3 in the Qt maintentance tool

Next, install [emscripten](https://emscripten.org/docs/getting_started/downloads.html) 
  within the terminal/cmd:
  1. Find the Qt directory on your system (for me it was just C:/Qt) and cd that path
  2. ```git clone https://github.com/emscripten-core/emsdk.git``` 
  3. ```cd emsdk``` (enter the emsdk directory within the emscripten repo you just cloned)
  4. ```git pull```
  5. ```./emsdk install 3.1.37``` (replace "./emsdk" with "emsdk.bat" if on windows, this is true for any "./emsdk")
  6. ```./emsdk activate 3.1.37 --permanent```
  7. ```source ./emsdk_env.sh``` (on windows run "emsdk_env.bat" instead)
  8. Mac and Linux only: ```source /path/to/emsdk/emsdk_env.sh``` to add to PATH
  9. ```em++ --version``` to verify installation
     
On Windows and Linux you must have Python intalled

On Mac you must have macOS 10.14 Mojave or above

Now go into Qt creator and open the project
  1. Go to Edit>Preferences>Devices>WebAssembly and provide the path to emscripten/emsdk and apply
 
  ![image](https://github.com/Espen-ChamplainCollege/PhysicsSimulator/assets/97118953/8d33539f-aad8-44eb-9385-0d59cb300908)
  
  2. Go to projects and activate the WebAssembly 6.6.3 kit on the left side (it should be greyed out for you until you click to activate it)
  
  ![image](https://github.com/Espen-ChamplainCollege/PhysicsSimulator/assets/97118953/ea0314e2-9007-4ae9-bd02-2e36018615c5)
  
  3. Click run under that kit and once it builds click the main run button bottom left (for some reason the run under the kit won't actually run it)
  
  4. After that the page should automatically open

![image](https://github.com/Espen-ChamplainCollege/PhysicsSimulator/assets/97118953/bd6e851d-3198-4b60-b458-efbe54992045)

Note: Everything seems to work for now except for custom shapes

If you have any probems just let me know, this was a long and complicated process for me so it's totally possible I'm forgetting or missing a step

