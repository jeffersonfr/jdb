#pragma once

#include "database/DataClass.hpp"

template<typename... Models>
struct CompoundModel : public Models... {
  CompoundModel() = default;

  CompoundModel(Models const &... models) { ((set<Models>(models)), ...); }

  template<typename T>
  void set(T const &model) { (T &) (*this) = model; }

  template<typename T>
  T &get() const { return (T &) (*this); }

  template<typename T>
  Data &get(std::string const &id) const {
    return ((T &) (*this))[id];
  }

  friend std::ostream &operator<<(std::ostream &out,
                                  CompoundModel const &value) {
    for_each<0, Models...>(out, value);

    return out;
  }

private:
  template<std::size_t Index, typename Arg, typename... Args>
  static constexpr void for_each(std::ostream &out,
                                 CompoundModel const &value) {
    if constexpr (Index > 0) {
      out << ", ";
    }

    out << std::quoted(Arg::get_name(), '\'') << ": " << value.get<Arg>();

    if constexpr (sizeof...(Args) > 0) {
      return for_each<Index + 1, Args...>(out, value);
    }
  }

  template<std::size_t Index>
  static constexpr void for_each(std::ostream &out) {
  }
};

template<typename... Models>
struct fmt::formatter<CompoundModel<Models...> > : fmt::ostream_formatter {
};

namespace jinject {
  template<typename ...Models>
  struct introspection<CompoundModel<Models...> > {
    static std::string to_string() {
      std::string compoundModels;

      for_each<1, Models...>(compoundModels);

      return fmt::format("CompoundModel<{}>", compoundModels);
    }

  private:
    template<std::size_t Index>
    static void for_each(std::string &compoundModels) {
    }

    template<std::size_t Index, typename Arg, typename ...Args>
    static void for_each(std::string &compoundModels) {
      if (Index > 1) {
        compoundModels = compoundModels + ", ";
      }

      compoundModels = compoundModels + Arg::get_name();

      for_each<Index + 1, Args...>(compoundModels);
    }
  };
} // namespace jinject
