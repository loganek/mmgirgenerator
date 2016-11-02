#ifndef _GIR_GEN_DATA_STRUCTURES_H_
#define _GIR_GEN_DATA_STRUCTURES_H_

#include <vector>
#include <string>
#include <memory>

namespace GirGen {

enum class TransferOwnership
{
    None,
    Full
};

enum class EmissionStage
{
    First,
    Last,
    Cleanup
};

struct BaseInfo
{
  std::string name;
};

struct IDocumentatable
{
    std::string documentation;
    std::string documentation_deprecated;
};

struct TypeInfo : public BaseInfo
{
    std::string c_type;
    bool is_constant = false;
    virtual ~TypeInfo() {}
};

struct ArrayInfo : public TypeInfo
{
    int length = -1;
    int fixed_size = -1;
    bool is_zero_terminated = false;
    std::shared_ptr<TypeInfo> underlying_type;
};

struct AliasInfo : public BaseInfo, public IDocumentatable
{
    std::string c_type;
    std::shared_ptr<TypeInfo> type;
};

struct ConstantInfo : public BaseInfo, public IDocumentatable
{
    std::string c_type;
    std::string value;
    std::shared_ptr<TypeInfo> type;
};

struct PropertyInfo : public BaseInfo, public IDocumentatable
{
    std::shared_ptr<TypeInfo> type;
    bool writable = false;
    bool readable = true;
    bool construct_only = false;
    TransferOwnership transfer_ownership;
    bool deprecated = false;
};

struct EnumInfo : public BaseInfo, public IDocumentatable
{
    struct MemberInfo : public BaseInfo, public IDocumentatable
    {
        std::string c_identifier;
        std::string value;
    };

    std::string c_type;
    bool is_bitfield = false;
    std::vector<std::shared_ptr<MemberInfo>> members;
};

struct CallableInfo : public BaseInfo, public IDocumentatable
{
    struct ReturnValue : public IDocumentatable
    {
        std::shared_ptr<TypeInfo> type;
    };

    struct ParameterInfo : public BaseInfo, public IDocumentatable
    {
        std::shared_ptr<TypeInfo> type;
        TransferOwnership transfer_ownership;
        bool is_instance_param = false;
    };

    std::shared_ptr<ReturnValue> return_value;
    std::vector<std::shared_ptr<ParameterInfo>> parameters;
    bool throws = false;
};

struct FunctionInfo : public CallableInfo
{
    std::string c_identifier;

    bool is_virtual = false;
    bool is_constructor = false;
};

struct SignalInfo : public CallableInfo
{
    EmissionStage when;
};

struct StructureInfo : public BaseInfo, public IDocumentatable
{
    std::string c_type;
    std::string parent_name;
    bool is_abstract = false;
    std::vector<std::shared_ptr<FunctionInfo>> methods;
    std::vector<std::shared_ptr<FunctionInfo>> functions;
    std::vector<std::shared_ptr<SignalInfo>> glib_signals;
};

struct ClassInfo : public StructureInfo
{
    std::vector<std::string> implemented_interfaces;
    std::vector<std::shared_ptr<PropertyInfo>> properties;
};

struct InterfaceInfo : public StructureInfo
{
    std::vector<std::string> prerequisites;
    std::vector<std::shared_ptr<PropertyInfo>> properties;
};

struct NamespaceInfo : public BaseInfo
{
    std::string c_prefix;
    std::vector<std::shared_ptr<ClassInfo>> classes;
    std::vector<std::shared_ptr<InterfaceInfo>> interfaces;
    std::vector<std::shared_ptr<FunctionInfo>> functions;
    std::vector<std::shared_ptr<AliasInfo>> aliases;
    std::vector<std::shared_ptr<ConstantInfo>> constants;
    std::vector<std::shared_ptr<EnumInfo>> enumerations;
};

}

#endif /* _GIR_GEN_DATA_STRUCTURES_H_ */
