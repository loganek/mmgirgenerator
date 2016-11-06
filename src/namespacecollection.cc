#include "namespacecollection.h"

#include <algorithm>

namespace GirGen {

std::shared_ptr<NamespaceInfo>& NamespaceCollection::operator [](const std::string& nspace_name)
{
    return namespaces[nspace_name];
}

template<typename T>
std::shared_ptr<T> NamespaceCollection::find_any_type(std::string type_name, std::string namespace_name, std::vector<std::shared_ptr<T>> NamespaceInfo::*collection) const
{
    auto dot_pos = type_name.find('.');
    if (dot_pos != std::string::npos)
    {
        namespace_name = type_name.substr(0, dot_pos);
        type_name = type_name.substr(dot_pos + 1);
    }

    if (namespaces.find(namespace_name) != namespaces.end())
    {
        std::shared_ptr<NamespaceInfo> nspace = namespaces.at(namespace_name);

        auto it = std::find_if((*nspace.*collection).begin(), (*nspace.*collection).end(), [type_name](const auto &i) {
            return i->name == type_name;
        });
        if (it != (*nspace.*collection).end())
        {
            return *it;
        }
    }

    return std::shared_ptr<T>();
}

std::shared_ptr<StructureInfo> NamespaceCollection::find_structure_by_name(const std::string &structure_name, const std::string &default_namespace) const
{
    std::shared_ptr<StructureInfo> ret;

    if (ret = find_any_type(structure_name, default_namespace, &NamespaceInfo::classes))
    {
        return ret;
    }

    if (ret = find_any_type(structure_name, default_namespace, &NamespaceInfo::interfaces))
    {
        return ret;
    }

    if (ret = find_any_type(structure_name, default_namespace, &NamespaceInfo::unions))
    {
        return ret;
    }

    return ret;
}

std::shared_ptr<EnumInfo> NamespaceCollection::find_enum_by_name(const std::string &enum_name, const std::string &default_namespace) const
{
    return find_any_type<EnumInfo>(enum_name, default_namespace, &NamespaceInfo::enumerations);
}

std::shared_ptr<AliasInfo> NamespaceCollection::find_alias_by_name(const std::string &alias_name, const std::string &default_namespace) const
{
    return find_any_type<AliasInfo>(alias_name, default_namespace, &NamespaceInfo::aliases);
}


}
