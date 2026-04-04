#include "jdb/database/SqliteDatabase.hpp"
#include "jdb/database/DataClass.hpp"
#include "jdb/database/Repository.hpp"
#include "jdb/database/ExtendedModel.hpp"

#include <fmt/format.h>
#include <gtest/gtest.h>

#include <iostream>

using namespace jinject;
using namespace jdb;

using UserModel = DataClass<"user", Primary<"id">, NoForeign,
  Field<"id", FieldType::Serial, false>,
  Field<"name", FieldType::Int, false>,
  Field<"address", FieldType::Int, false>,
  Field<"description", FieldType::Text, false>>;

using LoginModel = DataClass<"login", Primary<"id">, NoForeign,
  Field<"user_id", FieldType::Serial, false>,
  ConstrainedField<1, "pass", FieldType::Int, false>>;

using CompoundUser = CompoundModel<UserModel, LoginModel>;

using UserModelRepository = Repository<UserModel>;
using LoginModelRepository = Repository<LoginModel>;

using ExtendedUserModel = ExtendedModel<UserModel,
  Field<"new_field", FieldType::Text, false>
>;

struct Environment : public ::testing::Environment {
  Environment() = default;

  void SetUp() override {
  }

  void TearDown() override {
  }
};

struct jDbSuite : public ::testing::Test {
  jDbSuite() = default;

  void SetUp() override {
  }

  void TearDown() override {
  }
};

namespace jdb {
  void model_from_data(UserModel &model, std::string const &data) {
    model["description"] = data;
  }

  void model_to_data(UserModel const &model, std::string &data) {
    data = model["description"].get_text().value();
  }
}

TEST_F(jDbSuite, SimpleMigration) {
  using MyDatabase = SqliteDatabase<UserModel>;

  auto db = std::make_shared<MyDatabase>("sistema.db");

  db->add_migration(Migration{
    1, [](Database &db) {
      // do nothing ...
    }
  })
  .build();
}

TEST_F(jDbSuite, SimpleConvertion) {
  UserModel model;
  std::string data;

  model.from("Jeff Ferr");
  model.to(data);

  ASSERT_EQ(data, "Jeff Ferr");
}

TEST_F(jDbSuite, DumpModel) {
  UserModel user;

  user["id"] = 1;
  user["name"] = "Jeff Ferr";
  user["address"] = "First District";
  user["description"] = "Some description";

  LoginModel login;

  login["user_id"] = 1;
  login["pass"] = "12345678";

  CompoundUser compound{user, login};

  // no constrained
  ASSERT_EQ(user.to_string(), "{'id':1, 'name':'Jeff Ferr', 'address':'First District', 'description':'Some description'}");
  ASSERT_EQ(login.to_string(), "{'user_id':1, 'pass':'12345678'}");
  ASSERT_EQ(compound.to_string(), "{'user': {'id':1, 'name':'Jeff Ferr', 'address':'First District', 'description':'Some description'}, 'login': {'user_id':1, 'pass':'12345678'}}");

  // constrained with RestrictLevel = 0
  ASSERT_EQ(login.restrict<0>().to_string(), "{'user_id':1}");
  ASSERT_EQ(compound.restrict<0>().to_string(), "{'user': {'id':1, 'name':'Jeff Ferr', 'address':'First District', 'description':'Some description'}, 'login': {'user_id':1}}");

  // constrained with RestrictLevel = 1
  ASSERT_EQ(login.restrict<1>().to_string(), "{'user_id':1, 'pass':'12345678'}");
  ASSERT_EQ(compound.restrict<1>().to_string(), "{'user': {'id':1, 'name':'Jeff Ferr', 'address':'First District', 'description':'Some description'}, 'login': {'user_id':1, 'pass':'12345678'}}");
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  ::testing::AddGlobalTestEnvironment(new Environment{});

  return RUN_ALL_TESTS();
}
