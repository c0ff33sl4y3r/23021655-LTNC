#ifndef ObjectGroup_cpp
#define ObjectGroup_cpp
#include "ObjectGroup.hpp"

ObjectGroup::ObjectGroup(json* map){
    for (auto& object : (*map)["objects"]){
        Object* obj = new Object(new json(object));
        objectsById[obj->objectId] = obj;
        // Many object can have the same name
        objectsByName[obj->objectName].push_back(obj);
    }
}

ObjectGroup::~ObjectGroup(){
    for (auto& object : objectsById){
        delete object.second;
    }
    for (auto& V : objectsByName){
        for (auto &object: V.second){
            delete object;
        }
    }
    objectsById.clear();
    objectsByName.clear();
}

#endif