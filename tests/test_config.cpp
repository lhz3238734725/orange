#include "src/config.h"
#include "src/log.h"
#include <yaml-cpp/yaml.h>

using namespace orange;

ConfigVar<int>::ptr g_int_value_config = Config::Lookup("system.port", int(800), "system port");
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
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before " << g_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before " << g_float_value_config->toString();
    for (auto& i :  g_vec_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before vec_int: " << i;
    }
    for (auto& i :  g_list_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before list_int: " << i;
    }
    for (auto& i :  g_set_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before set_int: " << i;
    }
    for (auto& i :  g_unordered_set_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before set_int: " << i;
    }
    for (auto& i :  g_map_str_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before map_str_int: " << i.first << " : " << i.second ;
    }
    for (auto& i :  g_unordered_map_str_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "before unordered_map_str_int: " << i.first << " : " << i.second ;
    }

    YAML::Node root = YAML::LoadFile("./config/log.yaml");
    Config::LoadFromTaml(root);
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after " << g_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after " << g_float_value_config->toString();
    for (auto& i :  g_vec_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after vec_int: " << i;
    }
    for (auto& i :  g_list_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after list_int: " << i;
    }
    for (auto& i :  g_set_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after set_int: " << i;
    }
    for (auto& i :  g_unordered_set_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after set_int: " << i;
    }
    for (auto& i :  g_map_str_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after map_str_int: " << i.first << " : " << i.second ;
    }
    for (auto& i :  g_unordered_map_str_int_value_config->getValue())
    {
        ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after unordered_map_str_int: " << i.first << " : " << i.second ;
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

    return 0;
}