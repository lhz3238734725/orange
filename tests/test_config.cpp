#include "src/config.h"
#include "src/log.h"
#include <yaml-cpp/yaml.h>

using namespace orange;

ConfigVar<int>::ptr g_int_value_config = Config::Lookup("system.port", int(800), "system port");
ConfigVar<float>::ptr g_float_value_config = Config::Lookup("system.float", 1.1111f, "system float");
ConfigVar<char>::ptr g_char_value_config = Config::Lookup("system.char", 'a', "system char");

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
    YAML::Node root = YAML::LoadFile("./config/log.yaml");
    Config::LoadFromTaml(root);
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after " << g_int_value_config->toString();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "after " << g_float_value_config->toString();
}

int main(int argc, char** argv){

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_int_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_int_value_config->toString();

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_float_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_float_value_config->toString();

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_char_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_char_value_config->toString();

    tes_yaml();

    test_config();

    return 0;
}