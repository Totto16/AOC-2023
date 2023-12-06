

#include "base.hpp"

helpers::expected<std::string, std::string> internals::read(std::filesystem::path name) {

    if (!std::filesystem::exists(name)) {
        return helpers::unexpected(std::format("File '{}' doesn't exist!", name.string()));
    }

    std::ifstream fileInputStream(name);

    if (!fileInputStream.is_open()) {
        return helpers::unexpected(std::format("File '{}' couldn't be opened!", name.string()));
    }

    std::stringstream result;
    result << fileInputStream.rdbuf();
    return result.str();
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first, const InputDescriptionGeneratorSingle second) {

    return InputDescriptionGeneratorMultiple{ first, second };
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first, const InputDescriptionGeneratorMultiple second) {

    InputDescriptionGeneratorMultiple result{ first };

    for (const auto& elem : second.all) {
        result.all.push_back(elem);
    }

    return result;
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first, const InputDescriptionGeneratorSingle second) {

    InputDescriptionGeneratorMultiple result{};

    for (const auto& elem : first.all) {
        result.all.push_back(elem);
    }
    result.all.push_back(second);

    return result;
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first, const InputDescriptionGeneratorMultiple second) {
    InputDescriptionGeneratorMultiple result{};

    for (const auto& elem : first.all) {
        result.all.push_back(elem);
    }

    for (const auto& elem : second.all) {
        result.all.push_back(elem);
    }

    return result;
}

InputDescriptionGeneratorMultiple Input::SameInput(std::string name) {

    return InputDescriptionGeneratorMultiple{
        InputDescriptionGeneratorSingle{InputDescriptionGenerateTag::InputTag, name},
        InputDescriptionGeneratorSingle{InputDescriptionGenerateTag::InputTag, name}
    };
}

InputDescriptionGeneratorSingle Input::Input(std::string name) {

    return InputDescriptionGeneratorSingle{ InputDescriptionGenerateTag::InputTag, name };
}

InputDescriptionGeneratorMultiple Input::SameExample(std::string name, ResultType result1, ResultType result2) {

    return InputDescriptionGeneratorMultiple{
        InputDescriptionGeneratorSingle{InputDescriptionGenerateTag::ExampleTag, name, result1},
        InputDescriptionGeneratorSingle{InputDescriptionGenerateTag::ExampleTag, name, result2}
    };
}

InputDescriptionGeneratorSingle Input::ExampleInput(std::string name, ResultType result) {

    return InputDescriptionGeneratorSingle{ InputDescriptionGenerateTag::ExampleTag, name, result };
}

std::optional<AoCDayStorageType> get_registered_day(DayType day) {

    if (!internals::global_init::available_days()->contains(day)) {
        return std::nullopt;
    }

    return internals::global_init::available_days()->at(day);
}

std::vector<DayType> get_available_days() {

    std::vector<DayType> keys{};
    keys.reserve(internals::global_init::available_days()->size());

    for (const auto& [key, _] : *internals::global_init::available_days()) {
        keys.push_back(key);
    }

    return keys;
}

std::shared_ptr<std::unordered_map<DayType, AoCDayStorageType>> internals::global_init::available_days() {
    static std::shared_ptr<std::unordered_map<DayType, AoCDayStorageType>> temp =
            std::make_shared<std::unordered_map<DayType, AoCDayStorageType>>();
    return temp;
}
