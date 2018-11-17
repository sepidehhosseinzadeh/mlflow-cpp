#include <org/mlflow/tracking/MlflowHttpCaller.h>

static size_t 
WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string 
MlflowHttpCaller::encodeForUrlQuery(std::string s) {
  static const char* allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~!$&'()*+,;=:@/?";
  std::string::size_type idx = s.find_first_not_of(allowed);
  while (idx != std::string::npos) {
    std::ostringstream oss;
    oss << '%' << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << (int)s[idx];
    std::string encoded = oss.str();
    s.replace(idx, 1, encoded);
    idx = s.find_first_not_of(allowed, idx+encoded.length());
  }
  return s;
}

std::string 
MlflowHttpCaller::post(std::string path, std::string json) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  //LOG(ERROR) << ("Sending POST " + path + ": " + json);
  curl_global_init(CURL_GLOBAL_ALL);  
  curl = curl_easy_init();
  fillRequestSettings(path, curl);
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_POST, true);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    CHECK(res == CURLE_OK) << "curl_easy_perform() failed: " + 
                               std::string(curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  curl_global_cleanup(); 
  LOG(FATAL) << "error...";
}

std::string 
MlflowHttpCaller::get(std::string path) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  //LOG(ERROR) << ("Sending GET " + path);
  curl_global_init(CURL_GLOBAL_ALL);  
  curl = curl_easy_init();

  //fillRequestSettings(path, curl);
  BasicMlflowHostCreds* hostCreds = new BasicMlflowHostCreds(std::getenv("MLFLOW_TRACKING_URI"));
  //MlflowHostCreds* hostCreds = hostCredsProvider->getHostCreds();
  std::string uri = hostCreds->getHost() + "/" + BASE_API_PATH + "/" + path;
  std::string username = hostCreds->getUsername();
  std::string password = hostCreds->getPassword();
  std::string token = hostCreds->getToken();
  if (username != "" && password != "") {
    uri.append(encodeForUrlQuery("?username=" + username));
    uri.append(encodeForUrlQuery("&password=" + password));
  }
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, true);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, true); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
    res = curl_easy_perform(curl);
    CHECK(res == CURLE_OK) << "curl_easy_perform() failed: " + 
                                   std::string(curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  curl_global_cleanup(); 
  LOG(FATAL) << "error...";
}

void
MlflowHttpCaller::fillRequestSettings(std::string path, CURL *curl) {
//  MlflowHostCreds* hostCreds = hostCredsProvider->getHostCreds();
  BasicMlflowHostCreds* hostCreds = new BasicMlflowHostCreds(std::getenv("MLFLOW_TRACKING_URI"));
  //createHttpClientIfNecessary(hostCreds->shouldIgnoreTlsVerification());
  std::string uri = hostCreds->getHost() + "/" + BASE_API_PATH + "/" + path; 
  std::string username = hostCreds->getUsername();
  std::string password = hostCreds->getPassword();
  std::string token = hostCreds->getToken();
  struct curl_slist *headerlist = NULL;
  if (username != "" && password != "") {
    // TODO
    std::string authHeader = encodeForUrlQuery(username + ":" + password);
    headerlist = curl_slist_append(headerlist, ("Authorization, Basic " + authHeader).c_str());
  } else if (token != "") {
    headerlist = curl_slist_append(headerlist, ("Authorization, Bearer " + token).c_str());
  }
  headerlist = curl_slist_append(headerlist, "Content-Type:application/json;charset=UTF-8");
  curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
  curl_easy_setopt(curl, CURLOPT_HEADER, true);
  curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
}


