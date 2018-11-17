#include <org/mlflow/tracking/creds/MlflowHostCredsProvider.h>

// A static hostname and optional credentials to talk to an MLflow server.
class BasicMlflowHostCreds : 
        public MlflowHostCreds, public MlflowHostCredsProvider {
private:
  std::string host = "http://127.0.0.1:5000", username = "", password = "", token = "";
  bool shouldIgnoreTlsVerification_;

public:

  BasicMlflowHostCreds(std::string host) {
    this->host = host;
  }

  BasicMlflowHostCreds(std::string host, std::string username, std::string password) {
    this->host = host;
    this->username = username;
    this->password = password;
  }

  BasicMlflowHostCreds(std::string host, std::string token) {
    this->host = host;
    this->token = token;
  }

  BasicMlflowHostCreds(
      std::string host,
      std::string username,
      std::string password,
      std::string token,
      bool ignoreTlsVerification) {
    this->host = host;
    this->username = username;
    this->password = password;
    this->token = token;
    this->shouldIgnoreTlsVerification_ = ignoreTlsVerification;
  }

  std::string getHost() {
    return host;
  }

  std::string getUsername() {
    return username;
  }

  std::string getPassword() {
    return password;
  }

  std::string getToken() {
    return token;
  }

  bool shouldIgnoreTlsVerification() {
    return shouldIgnoreTlsVerification_;
  }

  MlflowHostCreds* getHostCreds() {
    return this;
  }

  void refresh() {
    // no-op
  }

};
