#pragma once

#include "jdb/database/DataClass.hpp"

namespace jdb {
  template<typename Model, FieldConcept... Fields>
    struct ExtendedModel;

  template<jmixin::StringLiteral Name, PrimaryConcept PrimaryKeys, ForeignConcept ForeignKeys, FieldConcept... Fields, FieldConcept ...NewFields>
  struct ExtendedModel<DataClass<Name, PrimaryKeys, ForeignKeys, Fields...>, NewFields...> : DataClass<Name, PrimaryKeys, ForeignKeys, Fields..., NewFields...> {
      virtual ~ExtendedModel() = default;
    };
}
