#include <org/mlflow/tracking/creds/BasicMlflowHostCreds.h>
#include <glog/logging.h>
#include <curl/curl.h>
#include <iomanip>

class MlflowHttpCaller {
private:
  //MlflowHostCredsProvider* hostCredsProvider;
  BasicMlflowHostCreds* hostCredsProvider;
  const std::string BASE_API_PATH = "api/2.0/preview/mlflow";
  void fillRequestSettings(std::string path, CURL* curl);
  std::string encodeForUrlQuery(std::string s);

public:
  std::string post(std::string path, std::string json);    
  std::string get(std::string path); 
};















