#ifndef __ORANGE_CONFIG_H__
#define __ORANGE_CONFIG_H__

#include "log.h"
#include <memory>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>

namespace orange{

/**
 * @brief 配置项基类
 */
class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name, const std::string& description = "")
        : m_name(name)
        , m_description(description) {
        // 将字符串转成小写
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }
    ~ConfigVarBase() = default;

    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;
protected:
    std::string m_name;
    std::string m_description;
};

/**
 * @brief 具体配置项
 * @details T 配置项类型
 */
template<class T>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string& name, const T& default_value, const std::string& description = "") 
        : ConfigVarBase(name, description)
        , m_val(default_value) {

    }

    std::string toString() override{
        try{
            return boost::lexical_cast<std::string>(m_val);
        }catch(std::exception& e){
            // typeid(XX).name() c++用于获取某个变量的数据类型
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "ConfigVar::toString exception " << e.what() 
                << " convert: " << typeid(m_val).name() << " to string";
            return "";
        }
    }

    bool fromString(const std::string& val) override{
        try{
            m_val = boost::lexical_cast<T>(val);
        }catch(std::exception& e){
            // typeid(XX).name() c++用于获取某个变量的数据类型
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "ConfigVar::fromString exception " << e.what() 
                << " convert: " << " string to" << typeid(m_val).name();
        }
        return true;
    }

    const T getValue() const { return m_val; }
    void setValue(const T& val) { m_val = val; }
private:
    T m_val;
};

/**
 * @brief 配置项管理类
*/
class Config {
public:
    typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name, const T& default_value, const std::string& description = ""){
        auto tmp = Lookup<T>(name);
        if(tmp){
            ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "Lookup name=" << name << "exists";
            return tmp;
        }

        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos){
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "Lookup name=" << name << "invalid";
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        m_datas[name] = v;
        return v;
    } 
    
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name){
        auto it = m_datas.find(name);
        if(it == m_datas.end()){
            return nullptr;
        }

        // dynamic_pointer_cast 用于智能指针之间的动态类型转换
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }
    
    /**
     * @brief 将yaml节点中的数据读入配置管理器中的配置项
     * @param[in] root YANL节点
     */
    static void LoadFromTaml(const YAML::Node& root);

    /**
     * @brief 从配置管理器中查找key为name的配置项
     * @param[in] name 配置项名称
     * @return 配置项 未找到返回 nullptr
     */
    static ConfigVarBase::ptr LookupBase(const std::string& name);

private:
    static ConfigVarMap m_datas;
};

}

#endif