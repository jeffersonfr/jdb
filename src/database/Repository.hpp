#pragma once

#include "database/Database.hpp"
#include "database/CompoundModel.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "jinject/jinject.h"

#include <fmt/format.h>

template<typename T>
struct Repository {
  using Model = T;

  explicit Repository(std::shared_ptr<Database> db) : mDb{std::move(db)} {
  }

  std::shared_ptr<Database> get_database() { return mDb; }

  template<StringLiteral Extras, std::size_t Limit = 100>
  std::vector<Model> select(auto... values) const {
    std::vector<Model> items;
    std::ostringstream o;

    o << "SELECT * from " << Model::get_name() << " "
      << fmt::vformat(Extras.to_string(), fmt::make_format_args(values...));

    mDb->query_string(o.str(), [&](std::vector<std::string> const &columns,
                                   std::vector<Data> const &values) {
      Model item;

      if (items.size() >= Limit) {
        return false;
      }

      for (int i = 0; i < static_cast<int>(columns.size()); i++) {
        std::string const &column = columns[i];

        item[column] = values[i];
      }

      items.emplace_back(item);

      return true;
    });

    return items;
  }

  std::vector<Model> load_all() const { return select<"ORDER BY ROWID">(); }

  template<StringLiteral... Fields>
  int64_t count_by(auto... values) const {
    std::ostringstream o;
    int64_t result = 0L;

    o << "SELECT COUNT (*) from " << Model::get_name() << " WHERE ";

    for_each_where<0, Fields...>(o, values...);

    mDb->query_string(o.str(), [&](std::vector<std::string> const &columns,
                                   std::vector<Data> const &values) {
      result = values[0].get_int().value();

      return false;
    });

    return result;
  }

  template<StringLiteral... Fields>
  std::vector<Model> load_by(auto... values) const {
    std::vector<Model> items;
    std::ostringstream o;

    o << "SELECT * from " << Model::get_name() << " WHERE ";

    for_each_where<0, Fields...>(o, values...);

    o << " ORDER BY ROWID";

    mDb->query_string(o.str(), [&](std::vector<std::string> const &columns,
                                   std::vector<Data> const &values) {
      Model item;

      for (int i = 0; i < static_cast<int>(columns.size()); i++) {
        std::string const &column = columns[i];

        item[column] = values[i];
      }

      items.emplace_back(item);

      return true;
    });

    return items;
  }

  template<StringLiteral... Fields>
  std::optional<Model> first_by(auto... values) const {
    std::vector<Model> items;
    std::ostringstream o;

    if constexpr (sizeof...(values) == 0) {
      o << "SELECT * from " << Model::get_name() << " ORDER BY ";
    } else {
      o << "SELECT * from " << Model::get_name() << " WHERE ";

      for_each_where<0, Fields...>(o, values...);

      o << " ORDER BY ROWID, ";
    }

    for_each_order<0, Fields...>(o);

    o << " ASC LIMIT 1";

    mDb->query_string(o.str(), [&](std::vector<std::string> const &columns,
                                   std::vector<Data> const &values) {
      Model item;

      for (int i = 0; i < (int)columns.size(); i++) {
        std::string const &column = columns[i];

        item[column] = values[i];
      }

      items.emplace_back(item);

      return false;
    });

    if (items.empty()) {
      return {};
    }

    return {items[0]};
  }

  template<StringLiteral... Fields>
  std::optional<Model> last_by(auto... values) const {
    std::vector<Model> items;
    std::ostringstream o;

    if constexpr (sizeof...(values) == 0) {
      o << "SELECT * from " << Model::get_name() << " ORDER BY ";
    } else {
      o << "SELECT * from " << Model::get_name() << " WHERE ";

      for_each_where<0, Fields...>(o, values...);

      o << " ORDER BY ROWID, ";
    }

    for_each_order<0, Fields...>(o);

    o << " DESC LIMIT 1";

    mDb->query_string(o.str(), [&](std::vector<std::string> const &columns,
                                   std::vector<Data> const &values) {
      Model item;

      for (int i = 0; i < (int)columns.size(); i++) {
        std::string const &column = columns[i];

        item[column] = values[i];
      }

      items.emplace_back(item);

      return false;
    });

    if (items.empty()) {
      return {};
    }

    return {items[0]};
  }

  /*
    Find a unique registry using the primary key's ids. Is there is more than
    one registry, the method returns the first result of the list;
  */
  std::optional<Model> find(auto... values) {
    auto result = find_expanded(typename Model::Keys{}, values...);

    if (!result.empty()) {
      return {result[0]};
    }

    return {};
  }

  [[nodiscard]] std::expected<Model, std::runtime_error> save(Model const &item) const {
    try {
      return mDb->insert(item);
    } catch (std::runtime_error &e) {
      return std::unexpected{e};
    }
  }

  void save_all(std::vector<Model> const &items) const {
    mDb->transaction([&](Database &db) {
      for (auto const &item: items) {
        save(item);
      }
    });
  }

  std::optional<std::string> update(Model const &item) const {
    try {
      mDb->update(item);

      return {};
    } catch (std::runtime_error &e) {
      return e.what();
    }
  }

  std::optional<std::string> remove(Model const &model) const {
    try {
      mDb->remove(model);
    } catch (std::runtime_error &e) {
      return e.what();
    }

    return {};
  }

  std::optional<std::string> remove_all(std::vector<Model> const &items) const {
    try {
      mDb->transaction([&](Database &db) {
        for (auto const &item: items) {
          remove(item);
        }
      });
    } catch (std::runtime_error &e) {
      return e.what();
    }

    return {};
  }

  template<StringLiteral... Fields>
  void remove_by(Data values...) const {
    auto items = load_by<Fields...>(values);

    for (auto const &item: items) {
      remove(item);
    }
  }

private:
  std::shared_ptr<Database> mDb;

  template<StringLiteral... Keys>
  auto find_expanded(Primary<Keys...> primaryKeys, auto... values) {
    return load_by<Keys...>(values...);
  }

  template<std::size_t Index, StringLiteral Field, StringLiteral... Fields>
  void for_each_where(std::ostream &out, Data value, auto... values) const {
    if (Index != 0) {
      out << " AND ";
    }

    value.get_value(overloaded{
      [&](std::nullptr_t arg) { out << "(" << Field.to_string() << " IS NULL)"; },
      [&](bool arg) {
        out << "(" << Field.to_string() << " = " << (arg ? "true" : "false")
          << ")";
      },
      [&](int64_t arg) {
        out << "(" << Field.to_string() << " = " << arg << ")";
      },
      [&](double arg) {
        out << "(" << Field.to_string() << " = " << arg << ")";
      },
      [&](std::string arg) {
        out << "(" << Field.to_string() << " LIKE '%" << arg << "%')";
      }
    });

    if constexpr (sizeof...(Fields) > 0) {
      for_each_where<Index + 1, Fields...>(values...);
    }
  }

  template<std::size_t Index, StringLiteral Field, StringLiteral... Fields>
  void for_each_order(std::ostream &out) const {
    if (Index != 0) {
      out << ", ";
    }

    out << Field.to_string();

    if constexpr (sizeof...(Fields) > 0) {
      for_each_order<Index + 1, Fields...>();
    }
  }
};

template<typename... Models>
struct Repository<CompoundModel<Models...> > {
  using Model = CompoundModel<Models...>;

  explicit Repository(std::shared_ptr<Database> db = jinject::get{}) : mDb{std::move(db)} {
  }

  std::shared_ptr<Database> get_database() { return mDb; }

  std::vector<Model> load_all() const {
    std::vector<Model> items;
    std::ostringstream o;

    o << "SELECT ";

    for_each_model<0, Models...>(o);

    o << " FROM ";

    for_each_join<0, Models...>(o);

    std::ostringstream where;

    for_each_where<0, Models...>(where);

    if (!where.str().empty()) {
      o << " WHERE " << where.str();
    }

    mDb->query_string(o.str(), [&](std::vector<std::string> const &columns,
                                   std::vector<Data> const &values) {
      Model item;

      for_each_fill_model<0, Models...>(item, columns.begin(), values.begin());

      items.emplace_back(item);

      return true;
    });

    return items;
  }

  std::optional<std::string> update(Model const &item) {
    try {
      mDb->transaction([&](Database &db) { update_internal(item); });
    } catch (std::runtime_error &e) {
      return e.what();
    }

    return {};
  }

  std::optional<std::string> update_all(std::vector<Model> const &items) const {
    try {
      mDb->transaction([&](Database &db) {
        for (auto const &item: items) {
          update_internal(item);
        }
      });
    } catch (std::runtime_error &e) {
      return e.what();
    }

    return {};
  }

private:
  std::shared_ptr<Database> mDb;

  template<std::size_t Index, typename TModel, typename... TModels>
  void for_each_fill_model(Model &item, auto itColumn, auto itValue) const {
    int count{};

    TModel::get_fields([&]<typename Field>() { count++; });

    for (auto i = 0; i < count; i++) {
      item.template get<TModel>(*itColumn++) = *itValue++;
    }

    if constexpr (sizeof...(TModels) > 0) {
      for_each_fill_model<Index + 1, TModels...>(item, itColumn, itValue);
    }
  }

  template<std::size_t Index, typename TModel, typename... TModels>
  void for_each_model(std::ostream &out) const {
    if (Index != 0) {
      out << ", ";
    }

    out << TModel::get_name() << ".*";

    if constexpr (sizeof...(TModels) > 0) {
      for_each_model<Index + 1, TModels...>(out);
    }
  }

  template<std::size_t Index, typename TModel, typename... TModels>
  void for_each_join(std::ostream &out) const {
    if (Index != 0) {
      out << " INNER JOIN ";
    }

    out << TModel::get_name();

    if constexpr (sizeof...(TModels) > 0) {
      for_each_join<Index + 1, TModels...>(out);
    }
  }

  template<std::size_t Index, typename TModel, typename... TModels>
  void for_each_where(std::ostream &out) const {
    int indexRefers = 0;

    TModel::get_refers([&]<typename FKey>() {
      if (Index != 0 or indexRefers++ != 0) {
        out << " AND ";
      }

      out << TModel::get_name() << "." << FKey::get_name() << " = "
        << FKey::Model::get_name() << ".";

      FKey::Model::get_keys(
        [&]<typename Field>() { out << Field::get_name(); });
    });

    if constexpr (sizeof...(TModels) > 0) {
      if (TModel::Refers::get_size() > 0) {
        for_each_where<Index + 1, TModels...>(out);
      } else {
        for_each_where<Index, TModels...>(out);
      }
    }
  }

  template<std::size_t Index, typename TModel, typename... TModels>
  void for_each_update(Model const &item) const {
    std::unique_ptr<Repository<TModel> > repository = jinject::get{};

    repository->update(item.template get<TModel>());

    if constexpr (sizeof...(TModels) > 0) {
      for_each_update<Index + 1, TModels...>(item);
    }
  }

  void update_internal(Model const &model) {
    for_each_update<0, Models...>(model);
  }
};

namespace jinject {
  template<typename T>
  struct introspection<Repository<T> > {
    static std::string to_string() {
      return fmt::format("Repository<{}>", introspection<T>::to_string());
    }
  };
}
