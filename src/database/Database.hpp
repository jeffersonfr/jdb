#pragma once

#include "database/DataClass.hpp"
#include "database/Migration.hpp"

#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include <fmt/format.h>

struct Database {
  virtual ~Database() = default;

  virtual int64_t
    query_string(std::string const &sql,
                 std::function<bool(std::vector<std::string> const &,
                                    std::vector<Data> const &)>
                 callback) = 0;

  virtual void transaction(std::function<void(Database &)> callback) = 0;

  virtual int64_t get_last_rowid() = 0;

  template<typename Model, StringLiteral... Fields>
  std::optional<Model> find_by_rowid(int64_t rowId) {
    std::optional<Model> item;
    std::ostringstream o;

    o << "SELECT * from " << Model::get_name() << " WHERE ROWID = " << rowId;

    query_string(o.str(), [&](std::vector<std::string> const &columns,
                              std::vector<Data> const &values) {
      Model model;

      for (int i = 0; i < static_cast<int>(columns.size()); i++) {
        std::string const &column = columns[i];

        model[column] = values[i];
      }

      item = model;

      return false;
    });

    return item;
  }

  template<typename Field>
  bool default_with_null_value(auto const &value) {
    bool hasValue = true;

    value.get_value(
      overloaded{
        [&](std::nullptr_t arg) { hasValue = false; }, [](auto arg) {
        }
      });

    if (!Field::is_nullable() and !hasValue and
        Field::get_default().has_value()) {
      return true;
    }

    return false;
  }

  template<typename Model>
  Model insert(Model const &model) {
    std::ostringstream o;
    int first = 0;

    o << "INSERT INTO " << Model::get_name() << " (";

    model.get_fields([&]<typename Field>() {
      auto const &value = model[Field::get_name()];

      if (default_with_null_value<Field>(value)) {
        return;
      }

      if (first++) {
        o << ", ";
      }

      o << Field::get_name();
    });

    o << ") VALUES (";

    first = 0;

    model.get_fields([&]<typename Field>() {
      auto const &value = model[Field::get_name()];

      if (default_with_null_value<Field>(value)) {
        return;
      }

      if (first++) {
        o << ", ";
      }

      value.get_value(overloaded{
        [&](std::nullptr_t arg) {
          if (!Field::is_nullable() and Field::get_default().has_value()) {
            return;
          }
          if (!Field::is_nullable() and
              Field::get_type() != FieldType::Serial) {
            throw std::runtime_error(
              fmt::format("unable to insert '{}', field '{}' is not null",
                          Model::get_name(), Field::get_name()));
          }
          o << "null";
        },
        [&](bool arg) {
          if (Field::get_type() != FieldType::Bool and
              Field::get_type() != FieldType::Int) {
            throw std::runtime_error(
              fmt::format("unable to insert '{}', field '{}' is not "
                          "convertible to boolean",
                          Model::get_name(), Field::get_name()));
          }
          o << (arg ? "true" : "false");
        },
        [&](int64_t arg) {
          if (Field::get_type() != FieldType::Int and
              Field::get_type() != FieldType::Serial and
              Field::get_type() != FieldType::Timestamp and
              Field::get_type() != FieldType::Bool) {
            throw std::runtime_error(
              fmt::format("unable to insert '{}', field '{}' is not "
                          "convertible to integer",
                          Model::get_name(), Field::get_name()));
          }
          o << arg;
        },
        [&](double arg) {
          if (Field::get_type() != FieldType::Decimal) {
            throw std::runtime_error(
              fmt::format("unable to insert '{}', field '{}' is not "
                          "convertible to double",
                          Model::get_name(), Field::get_name()));
          }
          o << arg;
        },
        [&](std::string arg) {
          if (Field::get_type() != FieldType::Text and
              Field::get_type() != FieldType::Timestamp) {
            throw std::runtime_error(fmt::format(
              "unable to insert '{}', field '{}' is not a text value",
              Model::get_name(), Field::get_name()));
          }
          o << std::quoted(arg);
        }
      });
    });

    o << ");";

    query_string(o.str(), [](auto...) { return false; });

    int64_t lastRowId = get_last_rowid();
    auto result = find_by_rowid<Model>(lastRowId);

    if (!result.has_value()) {
      throw std::runtime_error("unable to recover model sequence");
    }

    return result.value();
  }

  template<typename Model>
  void update(Model const &model) {
    std::ostringstream o;
    int first = 0;

    o << "UPDATE " << Model::get_name() << " SET ";

    model.get_fields([&]<typename Field>() {
      auto const &value = model[Field::get_name()];

      if (default_with_null_value<Field>(value)) {
        return;
      }

      if (first++) {
        o << ", ";
      }

      o << Field::get_name() << " = ";

      value.get_value(overloaded{
        [&](std::nullptr_t arg) {
          if (!Field::is_nullable() and
              Field::get_type() != FieldType::Serial) {
            throw std::runtime_error(
              fmt::format("unable to update '{}', field '{}' is not null",
                          Model::get_name(), Field::get_name()));
          }
          o << "null";
        },
        [&](bool arg) {
          if (Field::get_type() != FieldType::Bool and
              Field::get_type() != FieldType::Int) {
            throw std::runtime_error(
              fmt::format("unable to update '{}', field '{}' is not "
                          "convertible to boolean",
                          Model::get_name(), Field::get_name()));
          }
          o << (arg ? "true" : "false");
        },
        [&](int64_t arg) {
          if (Field::get_type() != FieldType::Int and
              Field::get_type() != FieldType::Serial and
              Field::get_type() != FieldType::Timestamp and
              Field::get_type() != FieldType::Bool) {
            throw std::runtime_error(
              fmt::format("unable to update '{}', field '{}' is not "
                          "convertible to integer",
                          Model::get_name(), Field::get_name()));
          }
          o << arg;
        },
        [&](double arg) {
          if (Field::get_type() != FieldType::Decimal) {
            throw std::runtime_error(
              fmt::format("unable to update '{}', field '{}' is not "
                          "convertible to double",
                          Model::get_name(), Field::get_name()));
          }
          o << arg;
        },
        [&](std::string arg) {
          if (Field::get_type() != FieldType::Text and
              Field::get_type() != FieldType::Timestamp) {
            throw std::runtime_error(fmt::format(
              "unable to update '{}', field '{}' is not a text value",
              Model::get_name(), Field::get_name()));
          }
          o << std::quoted(arg);
        }
      });
    });

    get_where_from_primary_keys<Model>(o, model);

    o << ";";
    query_string(o.str(), [](auto...) { return false; });
  }

  template<typename Model>
  bool remove(Model const &model) {
    std::ostringstream o;
    // bool first = true;

    o << "DELETE FROM " << Model::get_name();

    get_where_from_primary_keys<Model>(o, model);

    o << ";";

    query_string(o.str(), [](auto...) { return false; });

    return true;
  }

  virtual Database &add_migration(Migration migration) = 0;

private:
  template<typename Model>
  void get_where_from_primary_keys(std::ostream &out, Model const &model) {
    std::ostringstream o;
    bool first = true;

    out << " WHERE ";

    model.get_keys([&]<typename Field>() {
      if (!first) {
        out << " AND ";
      }

      first = false;

      auto const &value = model[Field::get_name()];

      value.get_value(overloaded{
        [&](std::nullptr_t arg) {
          out << "(" << Field::get_name() << " IS NULL)";
        },
        [&](bool arg) {
          out << "(" << Field::get_name() << " = " << (arg ? "1" : "0")
            << ")";
        },
        [&](int64_t arg) {
          out << "(" << Field::get_name() << " = " << arg << ")";
        },
        [&](double arg) {
          out << "(" << Field::get_name() << " = " << arg << ")";
        },
        [&](std::string arg) {
          out << "(" << Field::get_name() << " LIKE '%" << arg << "%')";
        }
      });
    });
  }
};

template<typename Model, StringLiteral... Fields>
struct InsertValue {
  explicit InsertValue(Database &db) : mDb{db} {
  }

  ~InsertValue() {
    for (auto item: mInsertions) {
      mDb.transaction([=](Database &db) {
        item();
      });
    }
  }

  InsertValue &values(auto... params) {
    Model model;

    ((model[Fields.to_string()] = params), ...);

    mInsertions.emplace_back([=, &mDb = mDb]() { mDb.insert(model); });

    return *this;
  }

private:
  Database &mDb;
  std::vector<std::function<void()> > mInsertions;
};

template<typename Model, StringLiteral... Fields>
auto insert(Database &db) {
  return InsertValue<Model, Fields...>{db};
}

template<typename Model, StringLiteral... Fields>
struct UpdateValue {
  explicit UpdateValue(Database &db) : mDb{db} {
  }

  ~UpdateValue() {
    for (auto item: mUpdates) {
      mDb.transaction([=](Database &db) {
        item();
      });
    }
  }

  UpdateValue &values(auto... params) {
    Model model;

    ((model[Fields.to_string()] = params), ...);

    mUpdates.emplace_back([=, &mDb = mDb]() { mDb.update(model); });

    return *this;
  }

private:
  Database &mDb;
  std::vector<std::function<void()> > mUpdates;
};

template<typename Model, StringLiteral... Fields>
auto update(Database &db) {
  return UpdateValue<Model, Fields...>{db};
}

template<typename Model, StringLiteral... Fields>
struct RemoveValue {
  explicit RemoveValue(Database &db) : mDb{db} {
  }

  ~RemoveValue() {
    for (auto item: mUpdates) {
      mDb.transaction([=](Database &db) {
        item();
      });
    }
  }

  RemoveValue &values(auto... params) {
    Model model;

    ((model[Fields.to_string()] = params), ...);

    mUpdates.emplace_back([=, &mDb = mDb]() { mDb.remove(model); });

    return *this;
  }

private:
  Database &mDb;
  std::vector<std::function<void()> > mUpdates;
};

template<typename Model, StringLiteral... Fields>
auto remove(Database &db) {
  return RemoveValue<Model, Fields...>{db};
}
