// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "google/cloud/spanner/database.h"
#include "google/cloud/testing_util/status_matchers.h"
#include <gmock/gmock.h>
#include <sstream>
#include <string>

namespace google {
namespace cloud {
namespace spanner {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace {

using ::google::cloud::testing_util::StatusIs;

TEST(Database, Basics) {
  Instance in("p1", "i1");
  Database db(in, "d1");
  EXPECT_EQ("d1", db.database_id());
  EXPECT_EQ(in, db.instance());
  EXPECT_EQ("projects/p1/instances/i1/databases/d1", db.FullName());

  auto copy = db;
  EXPECT_EQ(copy, db);
  EXPECT_EQ("d1", copy.database_id());
  EXPECT_EQ(in, copy.instance());
  EXPECT_EQ("projects/p1/instances/i1/databases/d1", copy.FullName());

  auto moved = std::move(copy);
  EXPECT_EQ(moved, db);
  EXPECT_EQ("d1", moved.database_id());
  EXPECT_EQ(in, moved.instance());
  EXPECT_EQ("projects/p1/instances/i1/databases/d1", moved.FullName());

  Instance in2("p2", "i2");
  Database db2(in2, "d2");
  EXPECT_NE(db2, db);
  EXPECT_EQ("d2", db2.database_id());
  EXPECT_EQ(in2, db2.instance());
  EXPECT_EQ("projects/p2/instances/i2/databases/d2", db2.FullName());
}

TEST(Database, OutputStream) {
  Instance in("p1", "i1");
  Database db(in, "d1");
  std::ostringstream os;
  os << db;
  EXPECT_EQ("projects/p1/instances/i1/databases/d1", os.str());
}

TEST(Database, MakeDatabase) {
  auto db = Database(Instance(Project("p1"), "i1"), "d1");
  EXPECT_EQ(db, MakeDatabase(db.FullName()).value());

  for (std::string invalid : {
           "",
           "projects/",
           "projects/p1",
           "projects/p1/instances/",
           "projects/p1/instances/i1",
           "projects/p1/instances/i1/databases",
           "projects/p1/instances/i1/databases/",
           "/projects/p1/instances/i1/databases/d1",
           "projects/p1/instances/i1/databases/d1/",
           "projects/p1/instances/i1/databases/d1/etc",
       }) {
    auto db = MakeDatabase(invalid);
    EXPECT_THAT(db, StatusIs(StatusCode::kInvalidArgument,
                             "Improperly formatted Database: " + invalid));
  }
}

}  // namespace
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace spanner
}  // namespace cloud
}  // namespace google
