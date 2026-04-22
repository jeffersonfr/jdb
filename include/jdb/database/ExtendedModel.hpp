#pragma once

#include "jdb/database/DataClass.hpp"

namespace jdb {
  template<typename Model, typename... Fields>
    struct ExtendedModel;

  template<jmixin::StringLiteral Name, PrimaryConcept PrimaryKeys, ForeignConcept ForeignKeys, FieldConcept... Fields, FieldConcept ...NewFields>
  struct ExtendedModel<DataClass<Name, PrimaryKeys, ForeignKeys, Fields...>, NewFields...> : DataClass<Name, PrimaryKeys, ForeignKeys, Fields..., NewFields...> {
      ~ExtendedModel() override = default;
    };

  /*
  template<jmixin::StringLiteral Name, PrimaryConcept PrimaryKeys, ForeignConcept ForeignKeys, FieldConcept... Fields, jmixin::StringLiteral Name2, PrimaryConcept PrimaryKeys2, ForeignConcept ForeignKeys2, FieldConcept ...NewFields2>
  struct ExtendedModel<DataClass<Name, PrimaryKeys, ForeignKeys, Fields...>, DataClass<Name2, PrimaryKeys2, ForeignKeys2, NewFields2...>> : DataClass<Name, PrimaryKeys, ForeignKeys, Fields..., NewFields2...> {
    ~ExtendedModel() override = default;
  };

  template<jmixin::StringLiteral Name, PrimaryConcept PrimaryKeys, ForeignConcept ForeignKeys, FieldConcept... Fields, jmixin::StringLiteral ...NamesExt, FieldConcept ...NewFieldsExt>
  struct ExtendedModel<DataClass<Name, PrimaryKeys, ForeignKeys, Fields...>, DataClass<NamesExt, NoPrimary, NoForeign, NewFieldsExt>...> : DataClass<Name, PrimaryKeys, ForeignKeys, Fields..., NewFieldsExt...> {
    ~ExtendedModel() override = default;
  };
  */

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2,
    jmixin::StringLiteral Name3, FieldConcept ...Fields3>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>,
    DataClass<Name3, NoPrimary, NoForeign, Fields3...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2..., Fields3...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2,
    jmixin::StringLiteral Name3, FieldConcept ...Fields3,
    jmixin::StringLiteral Name4, FieldConcept ...Fields4>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>,
    DataClass<Name3, NoPrimary, NoForeign, Fields3...>,
    DataClass<Name4, NoPrimary, NoForeign, Fields4...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2..., Fields3..., Fields4...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2,
    jmixin::StringLiteral Name3, FieldConcept ...Fields3,
    jmixin::StringLiteral Name4, FieldConcept ...Fields4,
    jmixin::StringLiteral Name5, FieldConcept ...Fields5>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>,
    DataClass<Name3, NoPrimary, NoForeign, Fields3...>,
    DataClass<Name4, NoPrimary, NoForeign, Fields4...>,
    DataClass<Name5, NoPrimary, NoForeign, Fields5...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2..., Fields3..., Fields4..., Fields5...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2,
    jmixin::StringLiteral Name3, FieldConcept ...Fields3,
    jmixin::StringLiteral Name4, FieldConcept ...Fields4,
    jmixin::StringLiteral Name5, FieldConcept ...Fields5,
    jmixin::StringLiteral Name6, FieldConcept ...Fields6>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>,
    DataClass<Name3, NoPrimary, NoForeign, Fields3...>,
    DataClass<Name4, NoPrimary, NoForeign, Fields4...>,
    DataClass<Name5, NoPrimary, NoForeign, Fields5...>,
    DataClass<Name6, NoPrimary, NoForeign, Fields6...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2..., Fields3..., Fields4..., Fields5..., Fields6...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2,
    jmixin::StringLiteral Name3, FieldConcept ...Fields3,
    jmixin::StringLiteral Name4, FieldConcept ...Fields4,
    jmixin::StringLiteral Name5, FieldConcept ...Fields5,
    jmixin::StringLiteral Name6, FieldConcept ...Fields6,
    jmixin::StringLiteral Name7, FieldConcept ...Fields7>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>,
    DataClass<Name3, NoPrimary, NoForeign, Fields3...>,
    DataClass<Name4, NoPrimary, NoForeign, Fields4...>,
    DataClass<Name5, NoPrimary, NoForeign, Fields5...>,
    DataClass<Name6, NoPrimary, NoForeign, Fields6...>,
    DataClass<Name7, NoPrimary, NoForeign, Fields7...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2..., Fields3..., Fields4..., Fields5..., Fields6..., Fields7...> {
    ~ExtendedModel() override = default;
  };

  template<
    jmixin::StringLiteral Name0, PrimaryConcept PrimaryKeys0, ForeignConcept ForeignKeys0, FieldConcept... Fields0,
    jmixin::StringLiteral Name1, FieldConcept ...Fields1,
    jmixin::StringLiteral Name2, FieldConcept ...Fields2,
    jmixin::StringLiteral Name3, FieldConcept ...Fields3,
    jmixin::StringLiteral Name4, FieldConcept ...Fields4,
    jmixin::StringLiteral Name5, FieldConcept ...Fields5,
    jmixin::StringLiteral Name6, FieldConcept ...Fields6,
    jmixin::StringLiteral Name7, FieldConcept ...Fields7,
    jmixin::StringLiteral Name8, FieldConcept ...Fields8>
  struct ExtendedModel<
    DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0...>,
    DataClass<Name1, NoPrimary, NoForeign, Fields1...>,
    DataClass<Name2, NoPrimary, NoForeign, Fields2...>,
    DataClass<Name3, NoPrimary, NoForeign, Fields3...>,
    DataClass<Name4, NoPrimary, NoForeign, Fields4...>,
    DataClass<Name5, NoPrimary, NoForeign, Fields5...>,
    DataClass<Name6, NoPrimary, NoForeign, Fields6...>,
    DataClass<Name7, NoPrimary, NoForeign, Fields7...>,
    DataClass<Name8, NoPrimary, NoForeign, Fields8...>
  > : DataClass<Name0, PrimaryKeys0, ForeignKeys0, Fields0..., Fields1..., Fields2..., Fields3..., Fields4..., Fields5..., Fields6..., Fields7..., Fields8...> {
    ~ExtendedModel() override = default;
  };
}
