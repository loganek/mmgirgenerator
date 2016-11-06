#ifndef _GIR_GEN_DEFS_PRINTER_H_
#define _GIR_GEN_DEFS_PRINTER_H_

#include "namespacecollection.h"

#include <map>

namespace GirGen {
  
class DefsPrinter
{
    std::shared_ptr<NamespaceCollection> nspace_collection;
    std::shared_ptr<NamespaceInfo> nspace;
    std::map<std::string, std::string> namespace_mapping;

    std::string get_c_type_name(const std::shared_ptr<TypeInfo>& type_info) const;

    std::string get_property_type(const std::shared_ptr<TypeInfo>& type_info) const;

    void print_virtual_methods(const std::vector<std::shared_ptr<FunctionInfo>> &functions, const std::string &parent_c_type) const;
    void print_signals(const std::vector<std::shared_ptr<SignalInfo>> &signal_objects, const std::string &parent_c_type) const;
    void print_properties(const std::vector<std::shared_ptr<PropertyInfo>>& properties, const std::string& parent_ctype) const;
    void print_function(const std::shared_ptr<FunctionInfo>& fnc, const std::shared_ptr<StructureInfo>& parent) const;

    void print_callable_parameters(const std::shared_ptr<CallableInfo> &callable, bool force_conts_string = false) const;

public:
    DefsPrinter(const std::string &nspace_name, const std::shared_ptr<NamespaceCollection> &nspace_collection, const std::map<std::string, std::string>& namespace_mapping)
        : nspace_collection(nspace_collection), namespace_mapping(namespace_mapping) {
        nspace = (*nspace_collection)[nspace_name];
    }
    virtual ~DefsPrinter() {}

    void print_virtual_methods() const;
    void print_enums() const;
    void print_signals() const;
    void print_properties() const;
    void print_functions() const;
};

}

#endif /* _GIR_GEN_DEFS_PRINTER_H_ */
