

#include "base.hpp"

// TODO: using for all standard types: e.g day type = u8, solution type u64 etc.

std::expected<std::string, std::string>
internals::read(std::filesystem::path name) {

  if (!std::filesystem::exists(name)) {
    return std::unexpected(
        std::format("File '{}' doesn't exist!", name.string()));
  }

  std::ifstream fileInputStream(name);

  if (!fileInputStream.is_open()) {
    return std::unexpected(
        std::format("File '{}' couldn't be opened!", name.string()));
  }

  std::stringstream result;
  result << fileInputStream.rdbuf();
  return result.str();
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first,
           const InputDescriptionGeneratorSingle second) {

  return InputDescriptionGeneratorMultiple{first, second};
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorSingle first,
           const InputDescriptionGeneratorMultiple second) {

  InputDescriptionGeneratorMultiple result{first};

  for (const auto &elem : second.all) {
    result.all.push_back(elem);
  }

  return result;
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first,
           const InputDescriptionGeneratorSingle second) {

  InputDescriptionGeneratorMultiple result{};

  for (const auto &elem : first.all) {
    result.all.push_back(elem);
  }
  result.all.push_back(second);

  return result;
}

InputDescriptionGeneratorMultiple
operator>>(const InputDescriptionGeneratorMultiple first,
           const InputDescriptionGeneratorMultiple second) {
  InputDescriptionGeneratorMultiple result{};

  for (const auto &elem : first.all) {
    result.all.push_back(elem);
  }

  for (const auto &elem : second.all) {
    result.all.push_back(elem);
  }

  return result;
}

InputDescriptionGeneratorSingle Input::SameInput(std::string name) {

  return InputDescriptionGeneratorSingle{
      InputDescriptionGenerateTag::Input1 | InputDescriptionGenerateTag::Input2,
      name};
}

InputDescriptionGeneratorSingle Input::Input(std::string name) {

  return InputDescriptionGeneratorSingle{
      InputDescriptionGenerateTag::InputSingle, name};
}

InputDescriptionGeneratorMultiple Input::SameExample(std::string name,
                                                     std::uint64_t result1,
                                                     std::uint64_t result2) {

  return InputDescriptionGeneratorMultiple{
      InputDescriptionGeneratorSingle{
          InputDescriptionGenerateTag::ExampleSingle, name, result1},
      InputDescriptionGeneratorSingle{
          InputDescriptionGenerateTag::ExampleSingle, name, result2}};
}

InputDescriptionGeneratorSingle Input::ExampleInput(std::string name,
                                                    std::uint64_t result) {

  return InputDescriptionGeneratorSingle{
      InputDescriptionGenerateTag::ExampleSingle, name, result};
}

std::optional<AoCDayStorageType> get_registered_day(std::uint8_t day) {

  if (!internals::global_init::available_days()->contains(day)) {
    return std::nullopt;
  }

  return internals::global_init::available_days()->at(day);
}

std::shared_ptr<std::unordered_map<std::uint8_t, AoCDayStorageType>>
internals::global_init::available_days() {
  static std::shared_ptr<std::unordered_map<std::uint8_t, AoCDayStorageType>>
      temp = std::make_shared<
          std::unordered_map<std::uint8_t, AoCDayStorageType>>();
  return temp;
}
