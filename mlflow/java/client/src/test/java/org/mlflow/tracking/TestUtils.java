package org.mlflow.tracking;

import java.util.*;

import org.testng.Assert;

import org.mlflow.api.proto.Service.*;

public class TestUtils {

  final static double EPSILON = 0.0001F;

  static boolean equals(double a, double b) {
    return a == b ? true : Math.abs(a - b) < EPSILON;
  }

  static void assertRunInfo(RunInfo runInfo, long experimentId, String sourceName) {
    Assert.assertEquals(runInfo.getExperimentId(), experimentId);
    Assert.assertEquals(runInfo.getSourceName(), sourceName);
    Assert.assertNotEquals(runInfo.getUserId(), "");
  }

  public static void assertParam(List<Param> params, String key, String value) {
    Assert.assertTrue(params.stream().filter(e -> e.getKey().equals(key) && e.getValue().equals(value)).findFirst().isPresent());
  }

  public static void assertMetric(List<Metric> metrics, String key, double value) {
    Assert.assertTrue(metrics.stream().filter(e -> e.getKey().equals(key) && equals(e.getValue(), value)).findFirst().isPresent());
  }

  public static void assertTag(List<RunTag> tags, String key, String value) {
    Assert.assertTrue(tags.stream().filter(e -> e.getKey().equals(key) && e.getValue().equals(value)).findFirst().isPresent());
  }
  public static java.util.Optional<Experiment> getExperimentByName(List<Experiment> exps, String expName) {
    return exps.stream().filter(e -> e.getName().equals(expName)).findFirst();
  }

  static public String createExperimentName() {
    return "TestExp_" + UUID.randomUUID().toString();
  }
}
