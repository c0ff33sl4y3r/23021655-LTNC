#ifndef Object_hpp
#define Object_hpp
#include "Config.cpp"

struct Object{
    Object(json* map);
    ~Object();
    int objectId;
    string objectName;
    string objectType;
    int objectX;
    int objectY;
    int objectWidth;
    int objectHeight;
    int objectRotation;
    bool objectVisible;
    json objectProperties;
};

#endif