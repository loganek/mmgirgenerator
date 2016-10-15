#ifndef _GIR_GEN_REPOSITORY_LOADER_H_
#define _GIR_GEN_REPOSITORY_LOADER_H_

#include "datastructures.h"
#include <libxml++/libxml++.h>

#include <string>

namespace GirGen {

class RepositoryLoader
{
    xmlpp::DomParser parser;
public:
    static std::map<std::string, std::shared_ptr<NamespaceInfo>> namespaces;

    void load_namespace(const xmlpp::Element *element, const std::shared_ptr<NamespaceInfo> &nspace);

    std::shared_ptr<AliasInfo> load_alias(const xmlpp::Element *element);
    std::shared_ptr<ClassInfo> load_class(const xmlpp::Element *element);
    std::shared_ptr<InterfaceInfo> load_interface(const xmlpp::Element *element);
    std::shared_ptr<ConstantInfo> load_constant(const xmlpp::Element *element);
    std::shared_ptr<EnumInfo> load_enumeration(const xmlpp::Element *element);
    std::shared_ptr<FunctionInfo> load_function(const xmlpp::Element *element);
    std::shared_ptr<SignalInfo> load_signal(const xmlpp::Element *element);
    std::shared_ptr<CallableInfo::ReturnValue> load_return_value(const xmlpp::Node *node);
    std::vector<std::shared_ptr<CallableInfo::ParameterInfo>> load_parameters(const xmlpp::Node *node);

    void load_structure(const xmlpp::Element *element, const std::shared_ptr<StructureInfo> &structure);
    void load_callable(const xmlpp::Element *element, const std::shared_ptr<CallableInfo> &callable);

    std::shared_ptr<TypeInfo> read_type(const xmlpp::Node *parent_node);
    void read_documentation(const xmlpp::Node *parent_node, const std::shared_ptr<IDocumentatable> &documentable);

    static TransferOwnership transfer_ownership_from_string(const std::string &str);
    static EmissionStage emission_stage_from_string(const std::string &str);
    static const xmlpp::Element* to_element(const xmlpp::Node *node);

public:
    virtual ~RepositoryLoader() {}

    void parse_gir_file(const std::string &gir_file);
};

}

#endif // _GIR_GEN_REPOSITORY_LOADER_H_
