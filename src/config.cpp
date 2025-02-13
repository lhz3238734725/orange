#include "config.h"

namespace orange {
    Config::ConfigVarMap Config::m_datas;

    ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
        auto it = m_datas.find(name);
        return it == m_datas.end() ? nullptr : it->second;
    }

    // "A.B", 10
    // A:
    //    B: 10
    //    C: str
    static void ListAllMember(const std::string& prefix, const YAML::Node& node, std::list<std::pair<std::string, const YAML::Node>>& output){
        
        if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._0123456789") != std::string::npos){
            ORANGE_LOG_ERROR(ORANGE_LOG_ROOT()) << "Config invaild name: " << prefix << " : " << node;
            return;
        }

        output.emplace_back(std::make_pair(prefix, node));

        if(node.IsMap()){
            for (auto it = node.begin(); it != node.end(); ++it)
            {
                ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, output);
            }
            
        }
    }

    void Config::LoadFromTaml(const YAML::Node& root){
        std::list<std::pair<std::string, const YAML::Node>> all_nodes;
        ListAllMember("", root, all_nodes);

        for (auto& i : all_nodes)
        {
            if(i.first.empty()) continue;

            std::transform(i.first.begin(), i.first.end(), i.first.begin(), ::tolower);
            ConfigVarBase::ptr var = Config::LookupBase(i.first);

            if(var){
                if(i.second.IsScalar()){
                    var->fromString(i.second.Scalar());
                }else{
                    std::stringstream ss;
                    ss << i.second;
                    var->fromString(ss.str());
                }
            }
        }
        
    }
}