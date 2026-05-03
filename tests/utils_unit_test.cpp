#include "jdb/database/SqliteDatabase.hpp"
#include "jdb/database/DataClass.hpp"
#include "jdb/database/Repository.hpp"
#include "jdb/database/ExtendedModel.hpp"

#include <fmt/format.h>
#include <gtest/gtest.h>

#include <iostream>

#include "jdb/utils/Scope.hpp"

using namespace jinject;
using namespace jdb;

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

TEST_F(jDbSuite, PostAndReturn) {
  Scope scope(1);

  struct callback {
    void f() {
      std::cout << "entered !" << std::endl;
    }
  };

  callback c;

  scope.post([&]() {
    c.f();
  });
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  ::testing::AddGlobalTestEnvironment(new Environment{});

  return RUN_ALL_TESTS();
}
