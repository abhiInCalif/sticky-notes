#include <iostream>
#include "spacetouch.h"

using std::cout;
using std::cerr;
using std::endl;
using Spacetouch::Interface;
using Spacetouch::Swiperecognizer;
using Spacetouch::Taprecognizer;
using Spacetouch::Longpressrecognizer;
using Spacetouch::Controller;
using Spacetouch::Parser;
using Spacetouch::Pipeline;
using Spacetouch::Listener;
using Spacetouch::Proximityindicator;

class MySwipe : public Swiperecognizer {
public:
    MySwipe(Pipeline *pipeline, int moveToID, Parser *iniConfig):Swiperecognizer(pipeline, moveToID, iniConfig){}
    void onStateBegan(){
        // cout << "Swipe " << direction() << " began" << endl;
    }
    void onStateEnded(){
        // cout << "Swipe " << direction() << " ended" << endl;
    }
    void onStateFailed(){
        // cout << "Swipe " << direction() << " failed" << endl;
    }
};
class MyTap: public Taprecognizer {
public:
    MyTap(Pipeline *pipeline, Parser *iniConfig):Taprecognizer(pipeline, iniConfig){}
    void onStateBegan(){
        double* xyz = pipeline().getAbsoluteXYZ();
        cerr << "Received Tap" << endl;
        cout << "Tap began, x:" + std::to_string(xyz[0]) + "; y:" + std::to_string(xyz[1]) + "; z:" + std::to_string(xyz[2]) << endl;
    }
    void onStateEnded(){
        // cout << "Tap ended" << endl;
    }
    void onStateFailed(){
        // cout << "Tap failed" << endl;
    }
};
class MyLongpress : public Longpressrecognizer {
public:
    MyLongpress(Pipeline *pipeline, Parser *iniConfig):Longpressrecognizer(pipeline, iniConfig){}
    void onStateBegan(){
        // cout << "Longpress began" << endl;
    }
    void onStateEnded(){
        // cout << "Longpress ended" << endl;
    }
    void onStateFailed(){
        // cout << "Longpress failed" << endl;
    }
};
int testController(){
    Parser *iniConfig = new Parser("./config.ini");
    Parser iniConfigTMP("./config.ini");
    //iniConfigTMP = *iniConfig;
    iniConfig->dump("debug.log");
    int BOTTOM = iniConfig->getint("Pipeline:BOTTOM", -1);
    int TOP = iniConfig->getint("Pipeline:TOP", -1);
    int LEFT = iniConfig->getint("Pipeline:LEFT", -1);
    int RIGHT = iniConfig->getint("Pipeline:RIGHT", -1);
    Listener *listener = new Listener();
    Controller *g = new Controller(listener, iniConfig);
    Pipeline pipe = g->pipeline();
    MySwipe *leftSwipe = new MySwipe(&pipe, LEFT, iniConfig);
    g->installGesture(leftSwipe);
    MySwipe *rightSwipe = new MySwipe(&pipe, RIGHT, iniConfig);
    g->installGesture(rightSwipe);
    MySwipe *downSwipe = new MySwipe(&pipe, BOTTOM, iniConfig);
    g->installGesture(downSwipe);
    MySwipe *upSwipe = new MySwipe(&pipe, TOP, iniConfig);
    g->installGesture(upSwipe);
    MyTap *tap = new MyTap(&pipe, iniConfig);
    g->installGesture(tap);
    MyLongpress *longpress = new MyLongpress(&pipe, iniConfig);
    g->installGesture(longpress);

    Proximityindicator *pi = new Proximityindicator(&pipe, iniConfig);
    g->installGesture(pi);

    g->run();

    delete pi;
    delete longpress;
    delete tap;
    delete upSwipe;
    delete downSwipe;
    delete rightSwipe;
    delete leftSwipe;
    delete g;
    delete listener;
    delete iniConfig;
    return 0;
}
