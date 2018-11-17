#include <org/mlflow/tracking/MlflowClient.h>
#include <chrono>
#include <awslib/AwsFileHandler.h>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <ctime>

using namespace std::chrono;
using namespace mlflow;

// Returns a default client based on the MLFLOW_TRACKING_URI environment variable.
 
long
get_curr_time() {
  auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now());
  long t = now.time_since_epoch().count();
  return t;
}

MlflowClient::MlflowClient() {
  getDefaultTrackingUri();
}

std::string 
MlflowClient::sendPost(std::string path, std::string json) { 
  return httpCaller->post(path, json);
}

Run 
MlflowClient::getRun(std::string runUuid) {
  std::string builder = "runs/get?run_uuid=" + runUuid;
  return mapper->toGetRunResponse(httpCaller->get(builder)).run();
}

/**
 * Creates a new run under the default experiment with no application name.
 * @return RunInfo created by the server
 */
RunInfo 
MlflowClient::createRun() {
  return createRun(DEFAULT_EXPERIMENT_ID);
}

/**
 * Creates a new run under the given experiment with no application name.
 * @return RunInfo created by the server
 */
RunInfo 
MlflowClient::createRun(long experimentId) {
  return createRun(experimentId, "CPP_Application");
}

/**
 * Creates a new run under the given experiment with the given application name.
 * @return RunInfo created by the server
 */
RunInfo 
MlflowClient::createRun(long experimentId, std::string appName) {
  CreateRun* request = new CreateRun();
  request->set_experiment_id(experimentId);
  request->set_source_name(appName);
  request->set_source_type(SourceType::LOCAL);
  request->set_start_time(get_curr_time());
  const char* username = std::getenv("USER"); 
  if (username != NULL) {
    request->set_user_id(username);
  }
  return createRun(request);
}

/**
 * Creates a new run.
 * @return RunInfo created by the server
 */
RunInfo 
MlflowClient::createRun(CreateRun* request) {
  std::string ijson = mapper->toJson(*request);
  std::string ojson = sendPost("runs/create", ijson);
  return mapper->toCreateRunResponse(ojson).run().info();
}

// Logs a parameter against the given run, as a key-value pair.
void 
MlflowClient::logParam(std::string runUuid, std::string key, std::string value) {
  sendPost("runs/log-parameter", mapper->makeLogParam(runUuid, key, value));
}

// Logs a new metric against the given run, as a key-value pair.
void 
MlflowClient::logMetric(std::string runUuid, std::string key, double value) {
  sendPost("runs/log-metric", mapper->makeLogMetric(runUuid, key, value, get_curr_time()));
}

std::string 
getTime() {
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  std::string str(buffer);

  return str;
}

void
MlflowClient::logMessage(std::string runId, std::string message) {
  setTag(runId, "Message", "["+getTime()+"]: "+message);
}

void 
MlflowClient::logAuthor(std::string runId, std::string author) {
  setTag(runId, "Author", author);
}

std::string 
exec(const char* cmd) {
  char buffer[128];
  std::string result = "";
  FILE* pipe = popen(cmd, "r");
  if (!pipe) throw std::runtime_error("popen() failed!");
  try {
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
  } catch (...) {
    pclose(pipe);
    throw;
  }
  pclose(pipe);
  return result;
}

void 
MlflowClient::logDockerId(std::string runId) {
  std::string dockerId = exec(" cat /etc/hostname ");
  setTag(runId, "Docker ID", dockerId);
}

void 
MlflowClient::logDockerImageName(std::string runId) {
  CHECK(std::getenv("FUTURE_CONTAINER_NAME") != NULL) << 
                "Set FUTURE_CONTAINER_NAME as your name of the container when you want to commit.";
  setTag(runId, "Docker Image Name", std::getenv("FUTURE_CONTAINER_NAME"));
}

void
MlflowClient::logArtifact(std::string runId, std::string expId, 
                 std::string localPath, std::string fileName, 
                 std::string bucket, std::string prefix) {
  EcoAws::FileHandler handler("us-east-1");
  const std::string keyname = prefix + "/" + expId + "/" + runId + "/" + fileName;

  int ret = handler.S3PutFile(bucket, keyname, localPath);
  CHECK(ret == 0) << "Upload failed... ";
  
  setTag(runId, "File uploaded", "File: "+localPath+", uploaded into: "+"s3://"+bucket+keyname+" at time: "+ getTime() + "Access link: "+ "https://s3.console.aws.amazon.com/s3/object/" + bucket+keyname );

}

// Logs a new tag against the given run, as a key-value pair.
void 
MlflowClient::setTag(std::string runUuid, std::string key, std::string value) {
  sendPost("runs/set-tag", mapper->makeSetTag(runUuid, key, value));
}

// Sets the status of a run to be FINISHED at the current time.
void 
MlflowClient::setTerminated(std::string runUuid) {
  setTerminated(runUuid, RunStatus::FINISHED);
}

// Sets the status of a run to be completed at the current time.
void 
MlflowClient::setTerminated(std::string runUuid, RunStatus status) {
  setTerminated(runUuid, status, get_curr_time());
}

// Sets the status of a run to be completed at the given endTime.
void 
MlflowClient::setTerminated(std::string runUuid, RunStatus status, long endTime) {
  sendPost("runs/update", mapper->makeUpdateRun(runUuid, status, endTime));
}

std::string 
MlflowClient::getDefaultTrackingUri() {
  const char* defaultTrackingUri = std::getenv("MLFLOW_TRACKING_URI");  
  if (defaultTrackingUri == NULL) {
    LOG(FATAL) << "Default client requires MLFLOW_TRACKING_URI is set. Use fromTrackingUri() instead.";
  }
  return defaultTrackingUri;
}


