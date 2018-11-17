#include <org/mlflow/tracking/creds/MlflowHostCreds.h>

// Provides a dynamic, refreshable set of MlflowHostCreds.
class MlflowHostCredsProvider {
public:
  // Returns a valid MlflowHostCreds. This may be cached.
  virtual MlflowHostCreds* getHostCreds() = 0;
  // Refreshes the underlying credentials. May be a no-op.
  virtual void refresh() = 0;

};
