#include <algorithm>
#include <random>
#include <vector>
#include "catalog/catalog.h"
#include "catalog/namespace_handle.h"
#include "transaction/transaction_manager.h"
#include "util/test_harness.h"
namespace terrier {

struct TablespaceHandleTests : public TerrierTest {
  void SetUp() override {
    TerrierTest::SetUp();
    txn_manager_ = new transaction::TransactionManager(&buffer_pool_, true, LOGGING_DISABLED);

    catalog_ = new catalog::Catalog(txn_manager_);
  }

  void TearDown() override {
    TerrierTest::TearDown();
    delete catalog_;  // delete catalog first
    delete txn_manager_;
    delete txn_;
  }

  catalog::Catalog *catalog_;
  storage::RecordBufferSegmentPool buffer_pool_{100, 100};

  transaction::TransactionContext *txn_;
  transaction::TransactionManager *txn_manager_;
};

// Tests that we can get the default namespace and get the correct value from the corresponding row in pg_tablespace
// NOLINTNEXTLINE
TEST_F(TablespaceHandleTests, BasicCorrectnessTest) {
  txn_ = txn_manager_->BeginTransaction();
  auto tsp_handle = catalog_->GetTablespaceHandle();
  auto tsp_entry_ptr = tsp_handle.GetTablespaceEntry(txn_, "pg_global");
  EXPECT_STREQ("pg_global", tsp_entry_ptr->GetColumn(1).GetStringValue());
}
}  // namespace terrier
