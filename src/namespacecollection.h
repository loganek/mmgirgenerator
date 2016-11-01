#ifndef _GIR_GEN_NAMESPACE_COLLECTION_H_
#define _GIR_GEN_NAMESPACE_COLLECTION_H_

#include "datastructures.h"

#include <map>

namespace GirGen {

class NamespaceCollection
{
    using NamespaceMap = std::map<std::string, std::shared_ptr<NamespaceInfo>>;

    NamespaceMap namespaces;

    template<typename T>
    std::shared_ptr<T> find_any_type(std::string type_name, std::string namespace_name, std::vector<std::shared_ptr<T>> NamespaceInfo::*collection) const;

public:
    std::shared_ptr<StructureInfo> find_structure_by_name(const std::string &structure_name, const std::string &default_namespace) const;
    std::shared_ptr<EnumInfo> find_enum_by_name(const std::string &enum_name, const std::string &default_namespace) const;
    std::shared_ptr<AliasInfo> find_alias_by_name(const std::string &alias_name, const std::string &default_namespace) const;

    std::shared_ptr<NamespaceInfo>& operator[](const std::string &nspace_name);

    NamespaceMap::iterator begin() { return namespaces.begin(); }
    NamespaceMap::iterator end() { return namespaces.end(); }
    NamespaceMap::iterator find(const NamespaceMap::key_type& key) { return namespaces.find(key); }
};

}

#endif /* _GIR_GEN_NAMESPACE_COLLECTION_H_ */
