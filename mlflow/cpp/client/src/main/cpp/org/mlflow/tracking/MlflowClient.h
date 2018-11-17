#include <org/mlflow/api/proto/service.pb.h>
#include <org/mlflow/tracking/MlflowProtobufMapper.h>
#include <org/mlflow/tracking/MlflowHttpCaller.h>
#include <glog/logging.h>

using namespace mlflow;

class MlflowClient {
public:
  MlflowClient();
  //MlflowClient(std::string trackingUri);
  std::string sendPost(std::string path, std::string json);
  RunInfo createRun();
  RunInfo createRun(long experimentId);
  RunInfo createRun(long experimentId, std::string appName);
  RunInfo createRun(CreateRun* request); 
  Run getRun(std::string runUuid);
  void logParam(std::string runUuid, std::string key, std::string value);
  void logMetric(std::string runUuid, std::string key, double value);

  void logMessage(std::string runId, std::string message);
  void logAuthor(std::string runId, std::string author);
  void logDockerId(std::string runId);
  void logDockerImageName(std::string runId);
  void logArtifact(std::string runId, std::string expId, 
                   std::string localPath, std::string fileName,
                   std::string bucket, std::string prefix);
  void setTag(std::string runUuid, std::string key, std::string value);
  void setTerminated(std::string runUuid);
  void setTerminated(std::string runUuid, RunStatus status);
  void setTerminated(std::string runUuid, RunStatus status, long endTime);

private:
  const long DEFAULT_EXPERIMENT_ID = 0;
  MlflowProtobufMapper* mapper = new MlflowProtobufMapper();
  MlflowHttpCaller* httpCaller = new MlflowHttpCaller();
  std::string getDefaultTrackingUri();
};
