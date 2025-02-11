#include "src/config.h"
#include "src/log.h"

using namespace orange;

ConfigVar<int>::ptr g_int_value_config = Config::Lookup("system.port", int(800), "system port");
ConfigVar<float>::ptr g_float_value_config = Config::Lookup("system.float", 1.1111f, "system float");
ConfigVar<char>::ptr g_char_value_config = Config::Lookup("system.char", 'a', "system char");

int main(int argc, char** argv){

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_int_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_int_value_config->toString();

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_float_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_float_value_config->toString();

    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_char_value_config->getValue();
    ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << g_char_value_config->toString();

    return 0;
}