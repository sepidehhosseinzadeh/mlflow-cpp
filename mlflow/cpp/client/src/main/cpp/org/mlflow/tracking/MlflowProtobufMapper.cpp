#include <google/protobuf/util/json_util.h>
#include <org/mlflow/tracking/MlflowProtobufMapper.h>
#include <org/mlflow/api/proto/service.pb.h> 
#include <google/protobuf/text_format.h>
#include <utils/json_util.h>
#include <json/writer.h>

using namespace mlflow;

std::string 
MlflowProtobufMapper::toJson(google::protobuf::Message& mb) {
  return print(mb);
}

CreateRun_Response 
MlflowProtobufMapper::toCreateRunResponse(std::string json) {
  CreateRun_Response builder;
  merge(json, *builder.mutable_run()->mutable_info());
  return builder;
}

GetRun_Response 
MlflowProtobufMapper::toGetRunResponse(std::string json) {
  GetRun_Response builder;
  merge(json, *builder.mutable_run()->mutable_info());
  return builder;
}

std::string 
MlflowProtobufMapper::makeUpdateRun(std::string runUuid, 
                RunStatus status, long endTime) {
  UpdateRun builder;
  builder.set_run_uuid(runUuid);
  builder.set_status(status);
  builder.set_end_time(endTime);
  return print(builder);
}

std::string 
MlflowProtobufMapper::makeLogParam(std::string runUuid, 
                std::string key, std::string value) {
  LogParam builder;
  builder.set_run_uuid(runUuid);
  builder.set_key(key);
  builder.set_value(value);
  return print(builder);
}

std::string 
MlflowProtobufMapper::makeLogMetric(std::string runUuid, 
                std::string key, double value, long timestamp) {
  LogMetric builder;
  builder.set_run_uuid(runUuid);
  builder.set_key(key);
  builder.set_value(value);
  builder.set_timestamp(timestamp);
  return print(builder);
}

std::string 
MlflowProtobufMapper::makeSetTag(std::string runUuid, 
                std::string key, std::string value) {
  SetTag builder;
  builder.set_run_uuid(runUuid);
  builder.set_key(key);
  builder.set_value(value);
  return print(builder);
}

std::string 
MlflowProtobufMapper::print(const google::protobuf::Message& message) {
    return ConvertMessageToJson(message);
}

std::string
postprocess(Json::Value value) {
  Json::FastWriter fastWriter;
  std::string str = fastWriter.write(value);
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
  str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
  str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
  str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
  return str;
}

void 
MlflowProtobufMapper::merge(std::string json, RunInfo& msg) {
  std::string json_main = json.substr(json.find_first_of("{"), json.find_last_of("}")-json.find_first_of("{"));
  Json::Value json_value;
  ginger_ex::util::JsonUtil::readJsonFromString(json_main, &json_value);

  std::string run_uuid, experiment_id, name, source_type, source_name, user_id, status, start_time, artifact_uri, lifecycle_stage;
  run_uuid = postprocess(json_value["run"]["info"]["run_uuid"]);
  experiment_id = postprocess(json_value["run"]["info"]["experiment_id"]);
  name = postprocess(json_value["run"]["info"]["name"]);
  source_type = postprocess(json_value["run"]["info"]["source_type"]);
  source_name = postprocess(json_value["run"]["info"]["source_name"]);
  user_id = postprocess(json_value["run"]["info"]["user_id"]);
  status = postprocess(json_value["run"]["info"]["status"]);
  start_time = postprocess(json_value["run"]["info"]["start_time"]);
  artifact_uri = postprocess(json_value["run"]["info"]["artifact_uri"]);
  lifecycle_stage = postprocess(json_value["run"]["info"]["lifecycle_stage"]);
  msg.set_run_uuid(run_uuid);
  msg.set_experiment_id(atol(experiment_id.c_str()));
  msg.set_name(name);
  //msg.set_source_type(source_type);
  msg.set_source_name(source_name);
  msg.set_user_id(user_id);
  //msg.set_status(status);
  msg.set_start_time(atol(start_time.c_str()));
  msg.set_artifact_uri(artifact_uri);
  msg.set_lifecycle_stage(lifecycle_stage);
}

void 
MlflowProtobufMapper::merge(std::string json, 
                CreateRun_Response& msg) {
  google::protobuf::TextFormat::Parser parser;
  parser.MergeFromString(json, &msg);
}

void 
MlflowProtobufMapper::merge(std::string json, 
                GetRun_Response& msg) {
  google::protobuf::TextFormat::Parser parser;
  parser.AllowPartialMessage(true);
  parser.MergeFromString(json, &msg);

}

std::string 
MlflowProtobufMapper::ConvertMessageToJson(const google::protobuf::Message& poMsg) {
  std::string strMsg;
  google::protobuf::util::JsonOptions stOpt;
  stOpt.always_print_enums_as_ints = true;
  stOpt.always_print_primitive_fields = true;
  stOpt.preserve_proto_field_names = true;
  google::protobuf::util::MessageToJsonString(poMsg, &strMsg, stOpt);
  return strMsg;
}


