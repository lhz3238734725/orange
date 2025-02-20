#include "src/config.h"
#include "src/log.h"
#include <yaml-cpp/yaml.h>

using namespace orange;

ConfigVar<int>::ptr g_int_value_config = Config::Lookup("system.port", int(800), "system port");
ConfigVar<float>::ptr g_f_value_config = Config::Lookup("system.port", float(800), "system port");
ConfigVar<float>::ptr g_float_value_config = Config::Lookup("system.float", 1.1111f, "system float");
ConfigVar<char>::ptr g_char_value_config = Config::Lookup("system.char", 'a', "system char");
ConfigVar<std::vector<int>>::ptr g_vec_int_value_config = Config::Lookup("system.vec_int", std::vector<int>{1, 2}, "system vec_int");
ConfigVar<std::list<int>>::ptr g_list_int_value_config = Config::Lookup("system.list_int", std::list<int>{1, 2}, "system vec_int");
ConfigVar<std::set<int>>::ptr g_set_int_value_config = Config::Lookup("system.set_int", std::set<int>{1, 2}, "system set_int");
ConfigVar<std::unordered_set<int>>::ptr g_unordered_set_int_value_config = Config::Lookup("system.unordered_set_int", std::unordered_set<int>{1, 2}, "system unordered_set_int");
ConfigVar<std::map<std::string, int>>::ptr g_map_str_int_value_config = Config::Lookup("system.map_str_int", std::map<std::string, int>{{"1", 1}, {"2", 2}}, "system map_str_int");
ConfigVar<std::unordered_map<std::string, int>>::ptr g_unordered_map_str_int_value_config = Config::Lookup("system.unordered_map_str_int", std::unordered_map<std::string, int>{{"1", 1}, {"2", 2}}, "system unordered_map_str_int");

// yaml-cpp 的简单使用
void print_yaml(const YAML::Node& node, int level){
   if(node.IsScalar()) {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << std::string(level*4, ' ') << node.Scalar();
   } else if(node.IsNull()) {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << std::string(level*4, ' ') << "NULL";
   } else if(node.IsMap()) {
        for(auto it = node.begin(); it != node.end(); ++it){
            ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << std::string(level*4, ' ') << "map-key:" << it->first;
            print_yaml(it->second, level + 1);
        }
   } else if(node.IsSequence()){
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << std::string(level*4, ' ') << "sequence:";
        for(size_t i = 0; i < node.size(); ++i){
            ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << std::string(level*4, ' ') << "sequence[" << i << "]:";
            print_yaml(node[i], level + 1);
        }
   }
}

void tes_yaml(){
    YAML::Node root = YAML::LoadFile("./config/log.yaml");
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "\n" << root;
    print_yaml(root, 0);
}

void test_config() {
#define XX(g_var, prefix, name)   \
    for (auto& i :  g_var->getValue()) \
    {   \
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << #prefix":"#name" " << i;  \
    }

#define XX_PM(g_var, prefix, name)   \
    for (auto& i :  g_var->getValue()) \
    {   \
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << #prefix":"#name" " << i.first << " : " << i.second;   \
    }

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before " << g_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before " << g_float_value_config->toString();
    
    XX(g_vec_int_value_config, before, vec_int);
    XX(g_list_int_value_config, before, list_int);
    XX(g_set_int_value_config, before, set_int);
    XX(g_unordered_set_int_value_config, before, uset_int);
    XX_PM(g_map_str_int_value_config, before, map_str_int);
    XX_PM(g_unordered_map_str_int_value_config, before, umap_str_int);

    YAML::Node root = YAML::LoadFile("./config/log.yaml");
    Config::LoadFromTaml(root);

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after " << g_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after " << g_float_value_config->toString();
    XX(g_vec_int_value_config, after, vec_int);
    XX(g_list_int_value_config, after, list_int);
    XX(g_set_int_value_config, after, set_int);
    XX(g_unordered_set_int_value_config, after, uset_int);
    XX_PM(g_map_str_int_value_config, after, map_str_int);
    XX_PM(g_unordered_map_str_int_value_config, after, umap_str_int);

#undef XX
#undef XX_PM
}

class Person{
public:
    std::string name;
    int age = 0;
    bool sex = 0;
    std::vector<int> ints;

    std::string toString() const{
        std::stringstream ss;
        ss << "{ Person : name=" << name << ", " 
           << "age=" << age << "," 
           << "sex=" << sex << "}";
        return ss.str();
    }
};

namespace orange{
template<>
class LexicalCast<std::string, Person>{
public:
    Person operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        Person p;
        p.name = node["name"].as<std::string>();
        p.age = node["age"].as<int>();
        p.sex = node["sex"].as<bool>();

        return p;
    }
};

template<>
class LexicalCast<Person, std::string>{
public:
    std::string operator()(const Person& p){
        YAML::Node node;
        node["name"] = p.name;
        node["age"] = p.age;
        node["sex"] = p.sex;

        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
}

void test_class(){
    ConfigVar<Person>::ptr g_person = Config::Lookup("class.person", Person(), "class person");
    ConfigVar<std::map<std::string, Person>>::ptr g_person_map = Config::Lookup("class.person_map", std::map<std::string, Person>(), "class person_map");
    ConfigVar<std::map<std::string, std::vector<Person>>>::ptr g_person_map_vec = Config::Lookup("class.person_map_vec", std::map<std::string, std::vector<Person>>(), "class person_map_vec");

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before:" << g_person->getValue().toString();

    for(auto& i : g_person_map->getValue()){
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before:" << i.first <<" " << i.second.toString();
    }
    YAML::Node root = YAML::LoadFile("./config/log.yaml");
    Config::LoadFromTaml(root);

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after:" << g_person->getValue().toString();
    for(auto& i : g_person_map->getValue()){
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after:" << i.first <<" " << i.second.toString();
    }


    for(auto& i : g_person_map_vec->getValue()){
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after:" << i.first <<" ";
        for(auto& it : i.second){
            ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << it.toString();
        }
    }
}

int main(int argc, char** argv){

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_int_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_float_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_float_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_char_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_char_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_vec_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_list_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_set_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_unordered_set_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_map_str_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_unordered_map_str_int_value_config->toString();

    tes_yaml();
    test_config();
    test_class();

    return 0;
}