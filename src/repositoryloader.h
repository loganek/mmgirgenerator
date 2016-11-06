#ifndef _GIR_GEN_REPOSITORY_LOADER_H_
#define _GIR_GEN_REPOSITORY_LOADER_H_

#include "datastructures.h"
#include "namespacecollection.h"
#include <libxml++/libxml++.h>

#include <string>

namespace GirGen {

class RepositoryLoader
{
private:
    xmlpp::DomParser parser;

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
    std::shared_ptr<PropertyInfo> load_property(const xmlpp::Element *element);

    void load_structure(const xmlpp::Element *element, const std::shared_ptr<StructureInfo> &structure);
    void load_callable(const xmlpp::Element *element, const std::shared_ptr<CallableInfo> &callable);

    std::shared_ptr<TypeInfo> read_type(const xmlpp::Node *parent_node);
    void read_documentation(const xmlpp::Node *parent_node, const std::shared_ptr<IDocumentatable> &documentable);

    static TransferOwnership read_transfer_ownership_attribute(const xmlpp::Element *element, TransferOwnership default_value = DEFAULT_TRANSFER_OWNERSHIP_VALUE);
    static Direction read_direction_attribute(const xmlpp::Element *element, Direction default_value = DEFAULT_DIRECTION_VALUE);
    static EmissionStage read_emission_stage_attribute(const xmlpp::Element *element);
    static const xmlpp::Element* to_element(const xmlpp::Node *node);

public:
    virtual ~RepositoryLoader() {}

    void parse_gir_file(const std::string &gir_file, const std::shared_ptr<NamespaceCollection>& nspace_collection);
};

}

#endif /* _GIR_GEN_REPOSITORY_LOADER_H_ */
