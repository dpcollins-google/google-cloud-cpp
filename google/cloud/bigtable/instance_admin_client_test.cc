// Copyright 2018 Google Inc.
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

#include "google/cloud/bigtable/instance_admin_client.h"
#include "google/cloud/bigtable/internal/logging_instance_admin_client.h"
#include "google/cloud/testing_util/scoped_environment.h"
#include <gmock/gmock.h>

namespace google {
namespace cloud {
namespace bigtable {
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_BEGIN
namespace {

TEST(InstanceAdminClientTest, Default) {
  auto admin_client = CreateDefaultInstanceAdminClient(
      "test-project", ClientOptions().set_connection_pool_size(1));
  ASSERT_TRUE(admin_client);
  EXPECT_EQ("test-project", admin_client->project());

  auto stub0 = admin_client->Channel();
  EXPECT_TRUE(stub0);

  auto stub1 = admin_client->Channel();
  EXPECT_EQ(stub0.get(), stub1.get());

  admin_client->reset();
  stub1 = admin_client->Channel();
  EXPECT_TRUE(stub1);
  EXPECT_NE(stub0.get(), stub1.get());
}

TEST(InstanceAdminClientTest, MakeClient) {
  auto admin_client = MakeInstanceAdminClient(
      "test-project", Options{}.set<GrpcNumChannelsOption>(1));
  ASSERT_TRUE(admin_client);
  EXPECT_EQ("test-project", admin_client->project());

  auto stub0 = admin_client->Channel();
  EXPECT_TRUE(stub0);

  auto stub1 = admin_client->Channel();
  EXPECT_EQ(stub0.get(), stub1.get());

  admin_client->reset();
  stub1 = admin_client->Channel();
  EXPECT_TRUE(stub1);
  EXPECT_NE(stub0.get(), stub1.get());
}

TEST(InstanceAdminClientTest, Logging) {
  testing_util::ScopedEnvironment env("GOOGLE_CLOUD_CPP_ENABLE_TRACING", "rpc");

  auto admin_client = MakeInstanceAdminClient("test-project");
  ASSERT_TRUE(admin_client);
  ASSERT_TRUE(dynamic_cast<internal::LoggingInstanceAdminClient const*>(
      admin_client.get()))
      << "Should create LoggingInstanceAdminClient";
}

}  // namespace
GOOGLE_CLOUD_CPP_INLINE_NAMESPACE_END
}  // namespace bigtable
}  // namespace cloud
}  // namespace google
