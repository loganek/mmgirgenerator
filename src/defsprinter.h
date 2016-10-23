#ifndef _GIR_GEN_DEFS_PRINTER_H_
#define _GIR_GEN_DEFS_PRINTER_H_

#include "datastructures.h"
#include <map>

namespace GirGen {
  
class DefsPrinter
{
    std::shared_ptr<NamespaceInfo> nspace;
    std::map<std::string, std::string> namespace_mapping;

    void print_virtual_methods_of_structure(const std::shared_ptr<StructureInfo> &structure, const std::string &parent_c_type) const;

public:
    DefsPrinter(const std::shared_ptr<NamespaceInfo> &nspace, const std::map<std::string, std::string>& namespace_mapping)
        : nspace(nspace), namespace_mapping(namespace_mapping) {}
    virtual ~DefsPrinter() {}

    void print_virtual_methods() const;
    void print_enums() const;
};

}

#endif /* _GIR_GEN_DEFS_PRINTER_H_ */
