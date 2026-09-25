#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <json.hpp>
#include <memory>
#include <random>

struct Player;

struct Monster;

class Stats;

class Object;

class Room;

using vec_uniqueObj = std::vector<std::unique_ptr<Object>>;
using uniqueObj = std::unique_ptr<Object>;
using sharedMons = std::shared_ptr<Monster>;
using vec_sharedMons = std::vector<std::shared_ptr<Monster>>;
using vec_vec_sharedMons = std::vector<std::vector<std::shared_ptr<Monster>>>;
using uniqueRoom = std::unique_ptr<Room>;
using vec_uniqueRoom = std::vector<std::unique_ptr<Room>>;
using uniqueStats = std::unique_ptr<Stats>;
using uniquePlayer = std::unique_ptr<Player>;

class Object{
    std::string name_object{};
    int character{};
public:
    Object(std::string name_object_, int value) : name_object(name_object_), character(value){}
    int get_ch(){
        return character;
    }
    std::string return_name(){
        return name_object;
    }
    virtual ~Object(){}
};

struct HpUpObject : Object{
    HpUpObject(std::string name_object_, int value) : Object(name_object_, value){}
};

struct ArmorUpObject : Object{
    ArmorUpObject(std::string name_object_, int value) : Object(name_object_, value){}
};

struct DamageUpObject : Object{
    DamageUpObject(std::string name_object_, int value) : Object(name_object_, value){}
};

class Stats{
protected:
    int health{};
    int armor{};
    int damage{};
    std::string name{};
public:
    Stats(int health_, int armor_, int damage_) : health(health_), armor(armor_), damage(damage_){}
    int get_hp(){
        return health;
    }
    int get_armor(){
        return armor;
    }
    int get_damage(){
        return damage;
    }
    void set_hp(int value){
        health = value;
    }
    void set_armor(int value){
        armor = value;
    }
    void set_damage(int value){
        damage = value;
    }
    std::string get_name(){
        return name;
    }
    void set_name(std::string _name){
        name = _name;
    }
    void print_stat(){
        std::cout << "HP: " << health << " Armor: " << armor << " Damage: " << damage << std::endl;
    }
    virtual ~Stats(){};
};

struct Monster : Stats{
    Monster(std::string name_): Stats(20, 0, 5){
        name = name_;
    }
    Monster(std::string name_, uniqueStats stat_) : Stats(stat_->get_hp(), stat_->get_armor(), stat_->get_damage()){
        name = name_;
    }
    void print_Monster(){
        std::cout << name << ": ";
        this->print_stat();
    }
};

struct Player: Stats{

    Player(std::string _name) : Stats(100, 0, 10){
        name = _name;
    }

    Player(std::string _name, uniqueStats stat_) : Stats(stat_->get_hp(), stat_->get_armor(), stat_->get_damage()){
        name = _name;
    }

    void print_player(){
        std::cout << "Player: " << name << " ";
        this->print_stat();
    }
};

class Room{

    vec_sharedMons monsters_in_room;
    uniqueObj object;

public:

    Room(vec_sharedMons monsters_in_room_, uniqueObj object_) {
        monsters_in_room = std::move(monsters_in_room_);
        if(object_.get()){
            object = std::move(object_);
        }
    }
    void resize(int size){
        monsters_in_room.resize(size);
    }
    void push(sharedMons sm){
        monsters_in_room.push_back(sm);
    }
    int get_size(){
        return monsters_in_room.size();
    }
    void set_obj(uniqueObj obj){
        object = std::move(obj);
    }
    uniqueObj get_obj(){
        return std::move(object);
    }
    std::shared_ptr<Monster> get_Monster(int index){
        return monsters_in_room[index];
    }
    void delete_Monster(int index){
        monsters_in_room.erase(monsters_in_room.begin() + index);
    }
    void print_stat(){
        for(int i = 0; i < monsters_in_room.size(); i++){
            monsters_in_room[i]->print_Monster();
        }
        if(object.get()){
            std::cout << "Object: " << object->return_name() << std::endl;
        }
    }

};

class Map{
protected:
    vec_uniqueRoom rooms;
    int current_room{};
    vec_uniqueObj inventory;
    uniquePlayer player{};

public:

    Map(vec_uniqueRoom rs, uniquePlayer pl){
        rooms = std::move(rs);
        player = std::move(pl);
        current_room = 0;
    }

    void push_inv(uniqueObj obj){
        inventory.push_back(std::move(obj));
    }

    void up_object(){
        uniqueObj obj = std::move(rooms[current_room]->get_obj());
        if(dynamic_cast<HpUpObject*>(obj.get())){
            player->set_hp(player->get_hp() + obj->get_ch());
        }
        else if(dynamic_cast<ArmorUpObject*>(obj.get())){
            player->set_armor(player->get_armor() + obj->get_ch());
        }
        else if(dynamic_cast<DamageUpObject*>(obj.get())){
            player->set_damage(player->get_damage() + obj->get_ch());
        }
        else {
            std::cout << "Loot isn exist" << std::endl;
            return;
        }
        push_inv(std::move(obj));
        rooms[current_room]->print_stat();
    }

    void move_player(std::string command){
        if(command == "forward"){
            if(current_room < rooms.size()-1){
                ++current_room;
            }
            else {
                std::cout << "This last map" << std::endl;
            }
        }
        else if(command == "back"){
            if(current_room > 0){
               --current_room;
            }
            else {
                std::cout << "This start map" << std::endl;
            }
        }
    }
    void attack(int index_monster){
        index_monster-=1;
        std::shared_ptr<Monster> monster = rooms[current_room]->get_Monster(index_monster);
        monster->set_armor(monster->get_armor() - player->get_damage());
        player->set_armor(player->get_armor() - monster->get_damage());

        if(monster->get_armor() < 0){
            monster->set_armor(-monster->get_armor());
            monster->set_hp(monster->get_hp() - monster->get_armor());
            monster->set_armor(0);
        }
        if(player->get_armor() < 0){
            player->set_armor(-player->get_armor());
            player->set_hp(player->get_hp() - player->get_armor());
            player->set_armor(0);
        }
        if(monster->get_hp() <= 0){
            rooms[current_room]->delete_Monster(index_monster);
        }
    }
    void print(){
        rooms[current_room]->print_stat();
        player->print_player();
    }

};
class Save{
    virtual void save() = 0;
};
class Load{
    virtual void load() = 0;
};

class Save_and_Load_to_JSON : public Map, public Save, public Load{
public:
    Save_and_Load_to_JSON(vec_uniqueRoom rs, uniquePlayer pl): Map(std::move(rs), std::move(pl)){}
    void save()override{
        std::ofstream fs("save.json");
        if(fs.is_open()){
            nlohmann::json data;
            data["Player"]["name"] = player->get_name();
            data["Player"]["room"] = current_room;
            data["Player"]["stats"]["hp"] = player->get_hp();
            data["Player"]["stats"]["armor"] = player->get_armor();
            data["Player"]["stats"]["damage"] = player->get_damage();
            for(int i = 0; i < inventory.size(); ++i){
                if(dynamic_cast<HpUpObject*>(inventory[i].get())){
                    data["Player"]["inventory"][i]["type"] = "HpUpObject";
                }
                else if(dynamic_cast<ArmorUpObject*>(inventory[i].get())){
                    data["Player"]["inventory"][i]["type"] = "ArmorUpObject";
                }
                else if(dynamic_cast<DamageUpObject*>(inventory[i].get())){
                    data["Player"]["inventory"][i]["type"] = "DamageUpObject";
                }
                if(!inventory.empty()){
                    data["Player"]["inventory"][i]["name"] = inventory[i].get()->return_name();
                    data["Player"]["inventory"][i]["character"] = inventory[i].get()->get_ch();
                }
                else {
                    data["Player"]["inventory"] = " ";
                }

            }
            data["Map"]["countroom"] = rooms.size();
            for(int i = 0; i < rooms.size(); ++i){
                for(int j = 0; j < rooms[i]->get_size(); ++j){
                    std::shared_ptr<Monster> monster = rooms[current_room]->get_Monster(j);
                    data["Rooms"][i]["Monsters"][j]["name"] = monster->get_name();
                    data["Rooms"][i]["Monsters"][j]["stats"]["hp"] = monster->get_hp();
                    data["Rooms"][i]["Monsters"][j]["stats"]["armor"] = monster->get_armor();
                    data["Rooms"][i]["Monsters"][j]["stats"]["damage"] = monster->get_damage();
                }
                uniqueObj obj = rooms[i]->get_obj();
                if(obj){
                    if(dynamic_cast<HpUpObject*>(obj.get())){
                        data["Rooms"][i]["Object"]["type"] = "HpUpObject";
                    }
                    else if(dynamic_cast<ArmorUpObject*>(obj.get())){
                        data["Rooms"][i]["Object"]["type"] = "ArmorUpObject";
                    }
                    else if(dynamic_cast<DamageUpObject*>(obj.get())){
                        data["Rooms"][i]["Object"]["type"] = "DamageUpObject";
                    }
                    data["Rooms"][i]["Object"]["name"] = obj->return_name();
                    data["Rooms"][i]["Object"]["character"] = obj->get_ch();
                    rooms[i]->set_obj(std::move(obj));
                }
                else{
                    data["Rooms"][i]["Object"]["type"]="None";
                }

            }
            fs << std::setw(4) << data;
            std::cout << "Data is saved" << std::endl;
        }
        else {
            std::cout << "Save failed" << std::endl;
        }
    }
    void load()override{
        std::ifstream fs("save.json");
        if(fs.is_open()){

            nlohmann::json data = nlohmann::json::parse(fs);

            current_room = data["Player"]["room"];

            player->set_armor(data["Player"]["stats"]["armor"]);
            player->set_damage(data["Player"]["stats"]["damage"]);
            player->set_hp(data["Player"]["stats"]["hp"]);
            inventory.resize(data["Player"]["inventory"].size());
            for(int i = 0; i < data["Player"]["inventory"].size(); ++i){
                if(data["Player"]["inventory"][i]["type"] == "HpUpObject"){
                    inventory[i] = (std::make_unique<HpUpObject>(data["Player"]["inventory"][i]["name"], data["Player"]["inventory"][i]["character"]));
                }
                else if(data["Player"]["inventory"][i]["type"] == "ArmorUpObject"){
                    inventory[i] = (std::make_unique<ArmorUpObject>(data["Player"]["inventory"][i]["name"], data["Player"]["inventory"][i]["character"]));
                }
                else if(data["Player"]["inventory"][i]["type"] == "DamageUpObject"){
                    inventory[i] = (std::make_unique<DamageUpObject>(data["Player"]["inventory"][i]["name"], data["Player"]["inventory"][i]["character"]));
                }
            }
            rooms.resize(data["Map"]["countroom"]);

            for(int i = 0; i < rooms.size(); ++i){
                rooms[i]->resize(data["Rooms"][i]["Monsters"].size());
                for(int j = 0; j < data["Rooms"][i]["Monsters"].size(); ++j){
                    std::shared_ptr<Monster> monster = rooms[current_room]->get_Monster(j);
                    monster->set_name(data["Rooms"][i]["Monsters"][j]["name"]);
                    monster->set_armor(data["Rooms"][i]["Monsters"][j]["stats"]["armor"]);
                    monster->set_hp(data["Rooms"][i]["Monsters"][j]["stats"]["hp"]);
                    monster->set_damage(data["Rooms"][i]["Monsters"][j]["stats"]["damage"]);
                }

                if(data["Rooms"][i]["Object"]["type"] != "None"){
                    uniqueObj obj;
                    if(data["Rooms"][i]["Object"]["type"] == "HpUpObject"){
                        obj = std::make_unique<HpUpObject>(data["Rooms"][i]["Object"]["name"], data["Rooms"][i]["Object"]["character"]);
                    }
                    else if(data["Rooms"][i]["Object"]["type"] == "ArmorUpObject"){
                        obj = std::make_unique<ArmorUpObject>(data["Rooms"][i]["Object"]["name"], data["Rooms"][i]["Object"]["character"]);
                    }
                    else if(data["Rooms"][i]["Object"]["type"] == "DamageUpObject"){
                        obj = std::make_unique<DamageUpObject>(data["Rooms"][i]["Object"]["name"], data["Rooms"][i]["Object"]["character"]);
                    }
                    rooms[i]->set_obj(std::move(obj));
                }
            }
            std::cout << "Save is load" << std::endl;
        }
        else {
            std::cout << "Save isnt exist" << std::endl;
        }
    }
};

sharedMons random_Monster(int i){
    std::random_device rd;
    // Генератор (Вихрь Мерсенна)
    std::mt19937 gen(rd());
    // Распределение в диапазоне [1, 6]
    std::uniform_int_distribution<> distrib(1, 2);
    if(distrib(gen) == 1){
        return std::make_unique<Monster>("Monster " + std::to_string(i+1));
    }
    std::uniform_int_distribution<> distribstat(10, 45);
    int hp = distribstat(gen);
    int arm = distribstat(gen);
    int damage = distribstat(gen);
    return std::make_shared<Monster>("Monster " + std::to_string(i+1), std::make_unique<Stats>(hp, arm, damage));
}
uniqueObj random_Object(){
    std::random_device rd;
    // Генератор (Вихрь Мерсенна)
    std::mt19937 gen(rd());
    // Распределение в диапазоне [1, 6]
    std::uniform_int_distribution<> distrib(1, 3);
    std::uniform_int_distribution<> distribstat(5, 15);
    if(distrib(gen) == 1){
        return std::make_unique<HpUpObject>("Flask", distribstat(gen));
    }
    else if(distrib(gen) == 1){
        return std::make_unique<ArmorUpObject>("Helm", distribstat(gen));
    }
    else{
        return std::make_unique<DamageUpObject>("Sword", distribstat(gen));
    }
}

int main(){
    uniquePlayer player = std::make_unique<Player>("Maxim");
    vec_vec_sharedMons Monsters;
    for(int i = 0; i < 5; ++i){
        vec_sharedMons vec;
        for(int j = 0; j < 3; ++j){
            vec.push_back(random_Monster(j));
        }
        Monsters.push_back(std::move(vec));
    }
    vec_uniqueRoom rooms;
    for(int j = 0; j < Monsters.size(); ++j){
        rooms.push_back(std::make_unique<Room>(std::move(Monsters[j]), random_Object()));
    }

    std::cout << "Game start\n";
    Save_and_Load_to_JSON map(std::move(rooms), std::move(player));
    std::string str{};
    int index{};
    do{
        map.print();
        std::cout << "Enter command: " << std::endl;
        getline(std::cin, str);
        std::stringstream ss(str);
        ss >> str;
        if(str == "Move"){
            ss >> str;
            map.move_player(str);
        }
        else if(str == "Attack"){
            ss >> index;
            map.attack(index);
        }
        else if(str == "Get"){
            map.up_object();
        }
        else if(str == "Save"){
            map.save();
        }

        else if(str == "Load"){
            map.load();
        }

    }while(str != "Exit");


}