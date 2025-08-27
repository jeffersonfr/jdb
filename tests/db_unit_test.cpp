#include "database/SqliteDatabase.hpp"
#include "database/DataClass.hpp"
#include "database/Repository.hpp"

#include <fmt/format.h>
#include <gtest/gtest.h>

#include <iostream>

using namespace jinject;

using DumpModel = DataClass<"dump_model", Primary<"id">, NoForeign,
  Field<"id", FieldType::Serial, false>,
  Field<"imei", FieldType::Int, false>,
  Field<"efetivo", FieldType::Int, false>,
  Field<"descricao", FieldType::Text, false>>;

using DumpModelRepository = Repository<DumpModel>;

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

TEST_F(jDbSuite, SimpleMigration) {
  	using MyDatabase = SqliteDatabase<DumpModel>;

	auto db = std::make_shared<MyDatabase>("sistema.db");

    db->add_migration(Migration{
      1, [](Database &db) {
        // do nothing ...
      }
    })
    .build();
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  ::testing::AddGlobalTestEnvironment(new Environment{});

  return RUN_ALL_TESTS();
}
