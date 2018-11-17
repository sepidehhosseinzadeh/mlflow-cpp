#include <string>

// Provides a hostname and optional authentication for talking to an MLflow server.
class MlflowHostCreds {
public:
  // Hostname (e.g., http://localhost:5000) to MLflow server.
  virtual std::string getHost() = 0;
  virtual std::string getUsername() = 0;
  virtual std::string getPassword() = 0;
  // Token to use with Bearer authentication when talking to server.
  // If provided, user/password authentication will be ignored.
  virtual std::string getToken() = 0;
  // If true, we will not verify the server's hostname or TLS certificate.
  // This is useful for certain testing situations, but should never be true in production.
  virtual bool shouldIgnoreTlsVerification() = 0;
};




