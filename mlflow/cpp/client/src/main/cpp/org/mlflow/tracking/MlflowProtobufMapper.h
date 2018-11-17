#include <org/mlflow/api/proto/service.pb.h>
#include <glog/logging.h>

using namespace mlflow;

class MlflowProtobufMapper {
public:
  MlflowProtobufMapper() {};
  std::string toJson(google::protobuf::Message& mb);
  CreateRun_Response toCreateRunResponse(std::string json);
  GetRun_Response toGetRunResponse(std::string json);
  std::string makeUpdateRun(std::string runUuid, RunStatus status, long endTime); 
  std::string makeLogParam(std::string runUuid, std::string key, std::string value);
  std::string makeLogMetric(std::string runUuid, std::string key, double value, long timestamp);
  std::string makeSetTag(std::string runUuid, std::string key, std::string value);
  std::string ConvertMessageToJson(const google::protobuf::Message& poMsg);
  

private:
  std::string print(const google::protobuf::Message& message);
  void merge(std::string json, CreateRun_Response& builder); 
  void merge(std::string json, GetRun_Response& builder); 
  void merge(std::string json, RunInfo& builder); 
};
