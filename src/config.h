#ifndef __ORANGE_CONFIG_H__
#define __ORANGE_CONFIG_H__

#include "log.h"
#include <memory>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <functional>
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
    virtual std::string getTypeName() = 0;
protected:
    std::string m_name;
    std::string m_description;
};

/**
 * @brief 类型转换模板类（F 原类型 T 目标类型） 这个只支持简单类型与std::string的转换
 */
template<class F, class T>
class LexicalCast{
public:
    T operator()(const F& v){
        return boost::lexical_cast<T>(v);
    }
};

/**
 * @brief 类型转换模板偏特化(std::string 转换成 std::vector<T>)
 */
template<class T>
class LexicalCast<std::string, std::vector<T>>{
public:
    std::vector<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);

        std::vector<T> vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }

        return vec;
    }
};

/**
 * @brief 类型转换模板偏特化(std::vector<T> 转换成 std::string)
 */
template<class F>
class LexicalCast<std::vector<F>, std::string>{
public:
    std::string operator()(const std::vector<F>& v) {
        YAML::Node node(YAML::NodeType::Sequence);

        for(auto& i : v){
            node.push_back(YAML::Node(LexicalCast<F, std::string>()(i)));
        }

        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板偏特化(std::string 转换成 std::list<T>)
 */
template<class T>
class LexicalCast<std::string, std::list<T>>{
public:
    std::list<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);

        std::list<T> list;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            list.emplace_back(LexicalCast<std::string, T>()(ss.str()));
        }

        return list;
    }
};

/**
 * @brief 类型转换模板偏特化(std::list<T> 转换成 std::string)
 */
template<class F>
class LexicalCast<std::list<F>, std::string>{
public:
    std::string operator()(const std::list<F>& v){
        YAML::Node node(YAML::NodeType::Sequence);
        
        for(auto& i : v){
            node.push_back(YAML::Node(LexicalCast<F, std::string>()(i)));
        }

        std::stringstream ss;
        ss << node;

        return ss.str();
    }
};

/**
 * @brief 类型模板偏特化(std::string 转换成 std::set<T>)
 */
template<class T>
class LexicalCast<std::string, std::set<T>>{
public:
    std::set<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);

        std::set<T> set;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            set.insert(LexicalCast<std::string, T>()(ss.str()));
        }

        return set;
    }
};

/**
 * @brief 类型模板偏特化(std::set<T> 转换成 std::string)
 */
template<class F>
class LexicalCast<std::set<F>, std::string>{
public:
    std::string operator()(const std::set<F>& v){
        YAML::Node node(YAML::NodeType::Sequence);

        for(auto& i : v){
            node.push_back(YAML::Node(LexicalCast<F, std::string>()(i)));
        }

        std::stringstream ss;
        ss << node;

        return ss.str();
    }
};

/**
 * @brief 类型模板偏特化(std::string 转换成 std::unordered_set<T>)
 */
template<class T>
class LexicalCast<std::string, std::unordered_set<T>>{
public:
    std::unordered_set<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);

        std::unordered_set<T> set;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            set.insert(LexicalCast<std::string, T>()(ss.str()));
        }

        return set;
    }
};

/**
 * @brief 类型模板偏特化(std::unordered_set<T> 转换成 std::string)
 */
template<class F>
class LexicalCast<std::unordered_set<F>, std::string>{
public:
    std::string operator()(const std::unordered_set<F>& v){
        YAML::Node node(YAML::NodeType::Sequence);

        for(auto& i : v){
            node.push_back(YAML::Node(LexicalCast<F, std::string>()(i)));
        }

        std::stringstream ss;
        ss << node;

        return ss.str();
    }
};

/**
 * @brief 类型模板偏特化(std::string 转换成 std::map<std::string, T>)
 */
template<class T>
class LexicalCast<std::string, std::map<std::string, T>>{
public:
    std::map<std::string, T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);

        std::map<std::string, T> map;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); ++it){
            ss.str("");
            ss << it->second;
            map.insert(std::make_pair<std::string, T>(std::string(it->first.Scalar()), LexicalCast<std::string, T>()(ss.str())));
        }

        return map;
    }
};

/**
 * @brief 类型模板偏特化(std::map<std::string, T> 转换成 std::string)
 */
template<class F>
class LexicalCast<std::map<std::string, F>, std::string>{
public:
    std::string operator()(const std::map<std::string, F>& v){
        YAML::Node node(YAML::NodeType::Map);

        for(auto& i : v){
            node[i.first] = YAML::Load(LexicalCast<F, std::string>()(i.second));
        }

        std::stringstream ss;
        ss << node;

        return ss.str();
    }
};

/**
 * @brief 类型模板偏特化(std::string 转换成 std::unordered_map<std::string, T>)
 */
template<class T>
class LexicalCast<std::string, std::unordered_map<std::string, T>>{
public:
    std::unordered_map<std::string, T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);

        std::unordered_map<std::string, T> map;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); ++it){
            ss.str("");
            ss << it->second;
            map.insert(std::make_pair<std::string, T>(std::string(it->first.Scalar()), LexicalCast<std::string, T>()(ss.str())));
        }

        return map;
    }
};

/**
 * @brief 类型模板偏特化(std::unordered_map<std::string, T> 转换成 std::string)
 */
template<class F>
class LexicalCast<std::unordered_map<std::string, F>, std::string>{
public:
    std::string operator()(const std::unordered_map<std::string, F>& v){
        YAML::Node node(YAML::NodeType::Map);

        for(auto& i : v){
            node[i.first] = YAML::Load(LexicalCast<F, std::string>()(i.second));
        }

        std::stringstream ss;
        ss << node;

        return ss.str();
    }
};

/**
 * @brief 具体配置项
 * @details T 配置项类型
 *          FromStr 从std::string转换成T类型的仿函数
 *          ToStr   从T转换成std::string类型的仿函数
 */
template<class T, class FromStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T& old_value, const T& new_value)> on_change_cb;

    ConfigVar(const std::string& name, const T& default_value, const std::string& description = "") 
        : ConfigVarBase(name, description)
        , m_val(default_value) {

    }

    /**
     * @brief 将 T 类型数据转换成 std::string
     */
    std::string toString() override{
        try{
            return ToStr()(m_val);
        }catch(std::exception& e){
            // typeid(XX).name() c++用于获取某个变量的数据类型
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "ConfigVar::toString exception " << e.what() 
                << " convert: " << typeid(m_val).name() << " to string";
            return "";
        }
    }

    /**
     * @brief 将 std::string 转换成 T 类型数据
     */
    bool fromString(const std::string& val) override{
        try{
            setValue(FromStr()(val)); 
        }catch(std::exception& e){
            // typeid(XX).name() c++用于获取某个变量的数据类型
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "ConfigVar::fromString exception " << e.what() 
                << " convert: " << " string to " << typeid(m_val).name();
        }
        return true;
    }

    /**
     * @brief 获取 T 的类型
     */
    std::string getTypeName() override{ return typeid(m_val).name();}

    /**
     * @brief 获取配置项的值
     */
    const T getValue() const { return m_val; }

    /**
     * @brief 设置配置项的值，如果不相同通知回调事件
     */
    void setValue(const T& val) {

        if(m_val == val) return;
        for(auto& i : m_cbs){
            i.second(m_val, val);
        } 
        m_val = val;
    }
    
    /**
     * @brief 添加监听回调，key值从1开始逐步递增
     */
    uint64_t addListener(on_change_cb cb){
        static uint64_t s_fun_id = 0;
        ++s_fun_id;
        m_cbs[s_fun_id] = cb;
        return s_fun_id;
    }

    /**
     * @brief 删除监听回调
     */
    void delListener(uint64_t key){
        m_cbs.erase(key);
    }

    /**
     * @brief 获取监听回调
     */
    on_change_cb getListener(uint64_t key){

        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it;
    }

    /**
     * @brief 清空监听回调
     */
    void clearListener() {
        m_cbs.clear();
    }

private:
    T m_val;
    // 变更回调数组，key值要求唯一，一般用hash
    std::map<uint64_t, on_change_cb> m_cbs;
};

/**
 * @brief 配置项管理类
*/
class Config {
public:
    typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    /**
     * @brief 查找配置项，如果已存在，返回已存在的配置项；不存在则添加
     */
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name, const T& default_value, const std::string& description = ""){

        // 解决相同key，类型不同不报错的情况
        auto it = m_datas.find(name);
        if(it != m_datas.end()){
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
            if(tmp) {
                ORANGE_LOG_INFO(ORANGE_LOG_ROOT()) << "Lookup name=" << name << "exists";
                return tmp;
            }else {
                ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                    << typeid(T).name() << " real_type=" <<  it->second->getTypeName() << " " << it->second->toString();
                return nullptr;
            }
        }

        if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos){
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "Lookup name=" << name << "invalid";
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        m_datas[name] = v;
        return v;
    } 
    
    /**
     * @brief 查找配置项，如果已存在，返回已存在的配置项；不存在返回nullptr
     */
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
     * @param[in] root YAML节点
     */
    static void LoadFromTaml(const YAML::Node& root);

    /**
     * @brief 从配置管理器中查找key为name的配置项
     * @param[in] name 配置项名称
     * @return 配置项 未找到返回nullptr
     */
    static ConfigVarBase::ptr LookupBase(const std::string& name);

private:
    static ConfigVarMap m_datas;
};

}

#endif