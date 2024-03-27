
#include "../lib/miniaudio.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qpainter.h"
#include <qnamespace.h>
#include <qpainterpath.h>
#include <qtransform.h>
#include <qwindowdefs.h>
#include <qevent.h>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setMouseTracking(true);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Window, QColor::fromRgb(20, 24, 29));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setWindowTitle("Physics");

    //sphere = new Sphere(Point(50, 50), Point(2.5, 2.5), Color(255, 255, 255), 25);
    sandbox = new Sandbox(this->width(), this->height(), std::chrono::duration<double, std::milli>(16));
    sandbox->addMenu();
    
    sandbox->addStaticBody(Point(sandbox->width / 2, sandbox->height - 50), sandbox->width, 50);

    sandbox->start().detach();
    this->startTimer(16);
}

MainWindow::~MainWindow(){
    delete ui;
}
void MainWindow::wheelEvent(QWheelEvent *event){
    cameraScale += ((float)event->angleDelta().y()) / 1000;
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    int hozScrollSpeed = 20;
    int vertScrollSpeed = 20;
    if(event->key() == Qt::Key_Left) cameraPos.x += hozScrollSpeed;
    if(event->key() == Qt::Key_Right) cameraPos.x -= hozScrollSpeed;
    if(event->key() == Qt::Key_Up) cameraPos.y += vertScrollSpeed;
    if(event->key() == Qt::Key_Down) cameraPos.y -= vertScrollSpeed;

    if(event->key() == Qt::Key_Tab) sandbox->paused = !sandbox->paused;

}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);                         // Make a painter. "this" is the window.
    painter.setRenderHint(QPainter::Antialiasing);  // Turn on antialiasing
    painter.translate(cameraPos.x, cameraPos.y);
    painter.scale(cameraScale.x, cameraScale.y);

    painter.setPen(QPen(Qt::white));
    painter.drawRect(sandbox->position.x, sandbox->position.y, sandbox->width, sandbox->height);

    // really need to fix the spheres eventually
    painter.setPen(QPen(Qt::transparent));
    for(int i = 0; i < sandbox->spheres.size(); i++){
    painter.setBrush(QColor(sandbox->spheres[i]->color));
    painter.drawEllipse(
        sandbox->spheres[i]->position.x,
        sandbox->spheres[i]->position.y,
        sandbox->spheres[i]->radius,
        sandbox->spheres[i]->radius
    );  // Draw ellipse (x, y, width, height)
    }

    for(int i = 0; i < sandbox->shapes.size(); i++){
        Shape *s = sandbox->shapes[i];
        if(Sandbox::DEBUG_VIEW_TRIANGULATIONS){
            painter.setPen(QPen(Qt::green));
            painter.setBrush(Qt::transparent);
            for(int k = 0; k < s->triangles.size(); k++){
                painter.drawPolygon(Util::convertPointVector(s->triangles[k]), s->triangles[k].size());
            }
        }
        if(Sandbox::DEBUG_VIEW_BOUNDING_BOXES){
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::transparent);
            BoundingRect* br = sandbox->shapes[i]->boundingRect;
            painter.drawRect(br->center.x - br->rx, br->center.y - br->ry, br->rx * 2, br->ry * 2);
        }
        if(!Sandbox::DEBUG_NO_FILL){
            painter.setBrush(QColor(s->color));
        }
        painter.setPen(QPen(Qt::white));
        painter.drawPolygon(Util::convertPointVector(s->verts), s->verts.size());
        if(Sandbox::DEBUG_VIEW_VELOCITY){
            if(s->velocity != Point(0.0, 0.0)){
                painter.setPen(QPen(Qt::yellow));
                painter.drawLine(QPointF(s->position), QPointF(s->position + (s->velocity * 10)));
            }
        }
    }
    if(Sandbox::DEBUG_VIEW_CONTACTS){
        painter.setBrush(QBrush(Qt::red));
        for(auto iter = sandbox->collisions.begin(); iter != sandbox->collisions.end(); ++iter){
            for(int i = 0; i < iter->second.contacts.size(); i++){
                painter.setPen(QPen(Qt::red));
                painter.drawEllipse(QPointF(iter->second.contacts[i].position), 3, 3);
                painter.drawLine(QPointF(iter->second.contacts[i].position), QPointF(iter->second.contacts[i].position + (iter->second.contacts[i].normal * 10)));
                painter.setPen(QPen(Qt::blue));
                painter.drawLine(QPointF(iter->second.contacts[i].position), QPointF(iter->second.contacts[i].position + iter->second.contacts[i].relv));
            }
            for(int k = 0; k < iter->second.dClosest.size(); k++){
                painter.setPen(Qt::yellow);
                painter.setBrush(Qt::yellow);
                painter.drawEllipse(QPointF(iter->second.dClosest[k]), 3, 3);
            }
            for(int k = 0; k < iter->second.dSplitEdges.size(); k++){
                painter.setPen(Qt::yellow);
                painter.setBrush(Qt::yellow);
                painter.drawLine(QPointF(iter->second.dSplitEdges[k].a), QPointF(iter->second.dSplitEdges[k].b));
            }
            for(int k = 0; k < iter->second.dTestEdges.size(); k++){
                painter.setPen(Qt::green);
                painter.setBrush(Qt::green);
                painter.drawLine(QPointF(iter->second.dTestEdges[k].a), QPointF(iter->second.dTestEdges[k].b));
            }
            for(int k = 0; k < iter->second.intersections.size(); k++){
                painter.setPen(Qt::blue);
                painter.setBrush(Qt::blue);
                painter.drawEllipse(QPointF(iter->second.intersections[k]), 3, 3);
            }
        }
    }
    // We draw the buttons/UI second because it needs to be on top.
    painter.resetTransform();
    painter.setPen(QPen(Qt::white));
    painter.setBrush(QColor::fromRgb(20, 24, 29));
    QBrush b(palette().window());
    for(auto i = sandbox->buttons.begin(); i != sandbox->buttons.end(); i++){
    painter.fillRect(
        i->first.pos.x,
        i->first.pos.y,
        i->first.width,
        i->first.height,
        b
    );
    painter.drawRect(
        i->first.pos.x,
        i->first.pos.y,
        i->first.width,
        i->first.height
    );
    painter.setFont(QFont("Arial"));
    QRect rect = painter.boundingRect(
        i->first.pos.x, i->first.pos.y,
        i->first.width, i->first.height,
        Qt::AlignHCenter | Qt::AlignVCenter,
        i->first.text.c_str());
        painter.drawText(rect, i->first.text.c_str());
    }

}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    sandbox->tryClickButtons(event->pos());
}

// Set up Sounds

const char* SOUND_FILE_PATH = "../PhysicsSimulator/boop.mp3";

static void playback_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_uint32 framesRead = ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    // Make sure we play the whole sound
    if (framesRead < frameCount) {
        ma_uint32 totalSizeInBytes = frameCount * ma_get_bytes_per_frame(pDecoder->outputFormat, pDecoder->outputChannels);
        ma_uint32 bytesRead = framesRead * ma_get_bytes_per_frame(pDecoder->outputFormat, pDecoder->outputChannels);
        memset((unsigned char*)pOutput + bytesRead, 0, totalSizeInBytes - bytesRead);

        ma_device_stop(pDevice);
    }
    (void)pInput;
}

static bool playSound(const char* filePath) {
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file(filePath, NULL, &decoder);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize decoder.\n");
        return false;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format    = decoder.outputFormat;
    deviceConfig.playback.channels  = decoder.outputChannels;
    deviceConfig.sampleRate         = decoder.outputSampleRate;
    deviceConfig.dataCallback       = playback_callback;
    deviceConfig.pUserData          = &decoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        ma_decoder_uninit(&decoder);
        return false;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return false;
    }

    // Wait for the sound to finish playing. 
    /*while (ma_device_is_started(&device)) {
        ma_sleep(100);
    }*/

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return true;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if ((sandbox->checkMouseClick) && (sandbox->userShapePoints.size() < sandbox->userShapeVerts)) {
        Point mousePos = QWidget::mapFromGlobal(QCursor::pos());
        sandbox->userShapePoints.push_back(mousePos);
        playSound(SOUND_FILE_PATH);
    }
}

void MainWindow::timerEvent(QTimerEvent *event) { repaint(); }
