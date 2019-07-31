/*
 * (C) Copyright 2017 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef FV3JEDI_INCREMENT_INCREMENT_H_
#define FV3JEDI_INCREMENT_INCREMENT_H_

#include <ostream>
#include <string>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "fv3jedi/Geometry/Geometry.h"
#include "fv3jedi/Increment/Increment.interface.h"
#include "fv3jedi/State/State.h"
#include "oops/base/GeneralizedDepartures.h"
#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/dot_product.h"
#include "oops/util/Duration.h"
#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"

namespace eckit {
  class Configuration;
}

namespace ufo {
  class GeoVaLs;
  class Locations;
}

namespace oops {
  class Variables;
  class UnstructuredGrid;
}

namespace fv3jedi {
  class ModelBiasIncrement;
  class ErrorCovariance;
  class State;
  class GetValuesTrajMatrix;

// FV3JEDI increment

// -----------------------------------------------------------------------------

class Increment : public oops::GeneralizedDepartures,
                        public util::Printable,
                        private util::ObjectCounter<Increment> {
 public:
  static const std::string classname() {return "fv3jedi::Increment";}

/// Constructor, destructor
  Increment(const Geometry &, const oops::Variables &,
                   const util::DateTime &);
  Increment(const Geometry &, const Increment &);
  Increment(const Increment &, const bool);
  Increment(const Increment &);
  virtual ~Increment();

/// Basic operators
  void diff(const State &, const State &);
  void zero();
  void zero(const util::DateTime &);
  Increment & operator =(const Increment &);
  Increment & operator+=(const Increment &);
  Increment & operator-=(const Increment &);
  Increment & operator*=(const double &);
  void axpy(const double &, const Increment &, const bool check = true);
  double dot_product_with(const Increment &) const;
  void schur_product_with(const Increment &);
  void random();
  void dirac(const eckit::Configuration &);

/// Get increment values at observation locations
  void getValuesTL(const ufo::Locations &, const oops::Variables &,
                   ufo::GeoVaLs &, const GetValuesTrajMatrix &) const;
  void getValuesAD(const ufo::Locations &, const oops::Variables &,
                   const ufo::GeoVaLs &, const GetValuesTrajMatrix &);

/// Unstructured grid
  void ug_coord(oops::UnstructuredGrid &) const;
  void field_to_ug(oops::UnstructuredGrid &, const int &) const;
  void field_from_ug(const oops::UnstructuredGrid &, const int &);

/// I/O and diagnostics
  void read(const eckit::Configuration &);
  void write(const eckit::Configuration &) const;
  double norm() const;

  void updateTime(const util::Duration & dt) {time_ += dt;}

/// Other
  void accumul(const double &, const State &);
  void jnormgrad(const State &, const eckit::Configuration &);

// Utilities
  boost::shared_ptr<const Geometry> geometry() const {return geom_;}

  const util::DateTime & time() const {return time_;}
  util::DateTime & time() {return time_;}
  const util::DateTime & validTime() const {return time_;}
  util::DateTime & validTime() {return time_;}

  int & toFortran() {return keyInc_;}
  const int & toFortran() const {return keyInc_;}

// Private methods and variables
 private:
  void print(std::ostream &) const;
  F90inc keyInc_;
  boost::shared_ptr<const Geometry> geom_;
  oops::Variables vars_;
  util::DateTime time_;
};
// -----------------------------------------------------------------------------

}  // namespace fv3jedi

#endif  // FV3JEDI_INCREMENT_INCREMENT_H_