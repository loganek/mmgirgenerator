#include "datastructures.h"

#include <unordered_map>

namespace GirGen {

Direction direction_from_string(const std::string &str)
{
    static std::unordered_map<std::string, Direction> dir_map {
        {"in", Direction::In}, {"out", Direction::Out}, {"inout", Direction::InOut}};

    return dir_map.at(str);
}

TransferOwnership transfer_ownership_from_string(const std::string &str)
{
    static std::unordered_map<std::string, TransferOwnership> dir_map {
        {"full", TransferOwnership::Full}, {"none", TransferOwnership::None}, {"container", TransferOwnership::Container}, {"floating", TransferOwnership::None},};

    return dir_map.at(str);
}

EmissionStage emission_stage_from_string(const std::string &str)
{
    static std::unordered_map<std::string, EmissionStage> dir_map {
        {"first", EmissionStage::First}, {"last", EmissionStage::Last}, {"cleanup", EmissionStage::Cleanup}};

    return dir_map.at(str);
}


}
