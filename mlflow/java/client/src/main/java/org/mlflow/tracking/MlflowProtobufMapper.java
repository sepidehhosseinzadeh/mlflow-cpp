package org.mlflow.tracking;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.MessageOrBuilder;
import com.google.protobuf.util.JsonFormat;

import org.mlflow.api.proto.Service.*;

class MlflowProtobufMapper {

  String makeCreateExperimentRequest(String expName) {
    CreateExperiment.Builder builder = CreateExperiment.newBuilder();
    builder.setName(expName);
    return print(builder);
  }

  String makeLogParam(String runUuid, String key, String value) {
    LogParam.Builder builder = LogParam.newBuilder();
    builder.setRunUuid(runUuid);
    builder.setKey(key);
    builder.setValue(value);
    return print(builder);
  }

  String makeLogMetric(String runUuid, String key, double value, long timestamp) {
    LogMetric.Builder builder = LogMetric.newBuilder();
    builder.setRunUuid(runUuid);
    builder.setKey(key);
    builder.setValue(value);
    builder.setTimestamp(timestamp);
    return print(builder);
  }

  String makeSetTag(String runUuid, String key, String value) {
    SetTag.Builder builder = SetTag.newBuilder();
    builder.setRunUuid(runUuid);
    builder.setKey(key);
    builder.setValue(value);
    return print(builder);
  }

  String makeUpdateRun(String runUuid, RunStatus status, long endTime) {
    UpdateRun.Builder builder = UpdateRun.newBuilder();
    builder.setRunUuid(runUuid);
    builder.setStatus(status);
    builder.setEndTime(endTime);
    return print(builder);
  }

  String makeDeleteRun(String runUuid) {
    DeleteRun.Builder builder = DeleteRun.newBuilder();
    builder.setRunId(runUuid);
    return print(builder);
  }

  String makeRestoreRun(String runUuid) {
    RestoreRun.Builder builder = RestoreRun.newBuilder();
    builder.setRunId(runUuid);
    return print(builder);
  }

  String toJson(MessageOrBuilder mb) {
    return print(mb);
  }

  GetExperiment.Response toGetExperimentResponse(String json) {
    GetExperiment.Response.Builder builder = GetExperiment.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  ListExperiments.Response toListExperimentsResponse(String json) {
    ListExperiments.Response.Builder builder = ListExperiments.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  CreateExperiment.Response toCreateExperimentResponse(String json) {
    CreateExperiment.Response.Builder builder = CreateExperiment.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  GetRun.Response toGetRunResponse(String json) {
    GetRun.Response.Builder builder = GetRun.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  CreateRun.Response toCreateRunResponse(String json) {
    CreateRun.Response.Builder builder = CreateRun.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  GetMetric.Response toGetMetricResponse(String json) {
    GetMetric.Response.Builder builder = GetMetric.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  GetMetricHistory.Response toGetMetricHistoryResponse(String json) {
    GetMetricHistory.Response.Builder builder = GetMetricHistory.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  GetParam.Response toGeParamResponse(String json) {
    GetParam.Response.Builder builder = GetParam.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  ListArtifacts.Response toListArtifactsResponse(String json) {
    ListArtifacts.Response.Builder builder = ListArtifacts.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  SearchRuns.Response toSearchRunsResponse(String json) {
    SearchRuns.Response.Builder builder = SearchRuns.Response.newBuilder();
    merge(json, builder);
    return builder.build();
  }

  private String print(MessageOrBuilder message) {
    try {
      return JsonFormat.printer().preservingProtoFieldNames().print(message);
    } catch (InvalidProtocolBufferException e) {
      throw new MlflowClientException("Failed to serialize message " + message, e);
    }
  }

  private void merge(String json, com.google.protobuf.Message.Builder builder) {
    try {
      JsonFormat.parser().ignoringUnknownFields().merge(json, builder);
    } catch (InvalidProtocolBufferException e) {
      throw new MlflowClientException("Failed to serialize json " + json + " into " + builder, e);
    }
  }
}
