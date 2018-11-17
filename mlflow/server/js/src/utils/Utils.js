import dateFormat from 'dateformat';
import React from 'react';
import notebookSvg from '../static/notebook.svg';
import emptySvg from '../static/empty.svg';
import laptopSvg from '../static/laptop.svg';
import projectSvg from '../static/project.svg';

class Utils {
  /**
   * Merge a runs parameters / metrics.
   * @param runsUuids - A list of Run UUIDs.
   * @param keyValueList - A list of objects. One object for each run.
   * @retuns A key to a map of (runUuid -> value)
   */
  static mergeRuns(runUuids, keyValueList) {
    const ret = {};
    keyValueList.forEach((keyValueObj, i) => {
      const curRunUuid = runUuids[i];
      Object.keys(keyValueObj).forEach((key) => {
        const cur = ret[key] || {};
        ret[key] = {
          ...cur,
          [curRunUuid]: keyValueObj[key]
        };
      });
    });
    return ret;
  }

  static runNameTag = 'mlflow.runName';

  static formatMetric(value) {
    if (Math.abs(value) < 10) {
      return (Math.round(value * 1000) / 1000).toString();
    } else if (Math.abs(value) < 100) {
      return (Math.round(value * 100) / 100).toString();
    } else {
      return (Math.round(value * 10) / 10).toString();
    }
  }

  /**
   * We need to cast all of the timestamps back to numbers since keys of JS objects are auto casted
   * to strings.
   *
   * @param metrics - List of { timestamp: "1", [run1.uuid]: 7, ... }
   * @returns Same list but all of the timestamps casted to numbers.
   */
  static convertTimestampToInt(metrics) {
    return metrics.map((metric) => {
      return {
        ...metric,
        timestamp: Number.parseFloat(metric.timestamp),
      };
    });
  }

  /**
   * Format timestamps from millisecond epoch time.
   */
  static formatTimestamp(timestamp) {
    const d = new Date(0);
    d.setUTCMilliseconds(timestamp);
    return dateFormat(d, "yyyy-mm-dd HH:MM:ss");
  }

  /**
   * Format a duration given in milliseconds.
   *
   * @param duration in milliseconds
   */
  static formatDuration(duration) {
    if (duration < 500) {
      return duration + "ms";
    } else if (duration < 1000 * 60) {
      return (duration / 1000).toFixed(1) + "s";
    } else if (duration < 1000 * 60 * 60) {
      return (duration / 1000 / 60).toFixed(1) + "min";
    } else if (duration < 1000 * 60 * 60 * 24) {
      return (duration / 1000 / 60 / 60).toFixed(1) + "h";
    } else {
      return (duration / 1000 / 60 / 60 / 24).toFixed(1) + "d";
    }
  }

  static formatUser(userId) {
    return userId.replace(/@.*/, "");
  }

  static baseName(path) {
    const pieces = path.split("/");
    return pieces[pieces.length - 1];
  }

  static dropExtension(path) {
    return path.replace(/(.*[^/])\.[^/.]+$/, "$1");
  }

  static getGitHubRegex() {
    return /[@/]github.com[:/]([^/.]+)\/([^/#]+)#?(.*)/;
  }

  /**
   * Renders the source name and entry point into an HTML element. Used for display.
   * @param run MlflowMessages.RunInfo
   * @param tags Object containing tag key value pairs.
   */
  static renderSource(run, tags) {
    let res = Utils.formatSource(run);
    if (run.source_type === "PROJECT") {
      const match = run.source_name.match(Utils.getGitHubRegex());
      if (match) {
        let url = "https://github.com/" + match[1] + "/" + match[2].replace(/.git/, '');
        if (match[3]) {
          url = url + "/tree/master/" + match[3];
        }
        res = <a href={url}>{res}</a>;
      }
      return res;
    } else if (run.source_type === "NOTEBOOK") {
      const notebookIdTag = 'mlflow.databricks.notebookID';
      const webappUrlTag = 'mlflow.databricks.webappURL';
      const notebookId = tags && tags[notebookIdTag] && tags[notebookIdTag].value;
      const webappUrl = tags && tags[webappUrlTag] && tags[webappUrlTag].value;
      if (notebookId && webappUrl) {
        res = (<a title={run.source_name} href={`${webappUrl}/#notebook/${notebookId}`}>
          {Utils.baseName(run.source_name)}
        </a>);
      }
      return res;
    } else {
      return res;
    }
  }

  /**
   * Returns an svg with some styling applied.
   */
  static renderSourceTypeIcon(sourceType) {
    const imageStyle = {
      height: '20px',
      position: 'relative',
      top: '-1px',
      right: '3px',
    };
    if (sourceType === "NOTEBOOK") {
      return <img title="Notebook" style={imageStyle} src={notebookSvg} />;
    } else if (sourceType === "LOCAL") {
      return <img title="Local Source" style={imageStyle} src={laptopSvg} />;
    } else if (sourceType === "PROJECT") {
      return <img title="Project" style={imageStyle} src={projectSvg} />;
    }
    return <img style={imageStyle} src={emptySvg} />;
  }

  /**
   * Renders the source name and entry point into a string. Used for sorting.
   * @param run MlflowMessages.RunInfo
   */
  static formatSource(run) {
    if (run.source_type === "PROJECT") {
      let res = Utils.dropExtension(Utils.baseName(run.source_name));
      if (run.entry_point_name && run.entry_point_name !== "main") {
        res += ":" + run.entry_point_name;
      }
      return res;
    } else {
      return Utils.baseName(run.source_name);
    }
  }

  /**
   * Renders the run name into a string.
   * @param runTags Object of tag name to MlflowMessages.RunTag instance
   */
  static getRunDisplayName(runTags, runUuid) {
    return Utils.getRunName(runTags) || "Run " + runUuid;
  }

  static getRunName(runTags) {
    const runNameTag = runTags[Utils.runNameTag];
    if (runNameTag) {
      return runNameTag.value;
    }
    return "";
  }

  static renderVersion(run, shortVersion = true) {
    if (run.source_version) {
      const versionString = shortVersion ? run.source_version.substring(0, 6) : run.source_version;
      if (run.source_type === "PROJECT") {
        const match = run.source_name.match(Utils.getGitHubRegex());
        if (match) {
          const url = ("https://github.com/" + match[1] + "/" + match[2].replace(/.git/, '') +
                     "/tree/" + run.source_version) + "/" + match[3];
          return <a href={url}>{versionString}</a>;
        }
        return versionString;
      } else {
        return versionString;
      }
    }
    return null;
  }

  static getErrorMessageFromXhr(xhr) {
    const { status } = xhr;
    if (status === 0) {
      return 'Request failed to send. Check your internet connection';
    }
    if (status >= 400 && status < 500) {
      if (xhr.responseJSON && xhr.responseJSON.message) {
        return xhr.responseJSON.message;
      }
      if (xhr.responseText) {
        return xhr.responseText;
      }
    }
    if (status >= 500) {
      return `Request Failed: ${xhr.statusText}`;
    }
    return 'Unknown Error';
  }

  static pluralize(word, quantity) {
    if (quantity > 1) {
      return word + 's';
    } else {
      return word;
    }
  }
}

export default Utils;
