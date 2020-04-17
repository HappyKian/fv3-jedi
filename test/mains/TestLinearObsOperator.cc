/*
 * (C) Copyright 2017 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "fv3jedi/Utilities/Traits.h"
#include "fv3jedi/Run/Run.h"
#include "test/interface/LinearObsOperator.h"

int main(int argc,  char ** argv) {
  fv3jedi::Run run(argc, argv);
  test::LinearObsOperator<fv3jedi::Traits> tests;
  return run.execute(tests);
}
