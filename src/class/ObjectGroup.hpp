#ifndef ObjectGroup_hpp
#define ObjectGroup_hpp
#include "Config.cpp"
#include "Object.cpp"

struct ObjectGroup{
    ObjectGroup(json* map);
    ~ObjectGroup();
    unordered_map <int, Object*> objectsById;
    unordered_map <string, vector <Object*>> objectsByName;
};

#endif