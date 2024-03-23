#ifndef Object_cpp
#define Object_cpp
#include "Object.hpp"

Object::Object(json* map){
    objectId = (*map)["id"];
    objectName = (*map)["name"];
    objectType = (*map)["type"];
    objectX = (*map)["x"];
    objectY = (*map)["y"];
    objectWidth = (*map)["width"];
    objectHeight = (*map)["height"];
    objectRotation = (*map)["rotation"];
    objectVisible = (*map)["visible"];
    if ((*map).find("properties") != (*map).end())
        objectProperties = (*map)["properties"];
    else
        objectProperties = json::parse(R"###([])###");
}

Object::~Object(){
}

#endif