import React, { Component } from 'react';
import PropTypes from 'prop-types';
import ShowArtifactImageView from './ShowArtifactImageView';
import { getExtension, IMAGE_EXTENSIONS, TEXT_EXTENSIONS } from '../../utils/FileUtils';
import ShowArtifactTextView from './ShowArtifactTextView';
import previewIcon from '../../static/preview-icon.png';
import './ShowArtifactPage.css';

class ShowArtifactPage extends Component {
  static propTypes = {
    runUuid: PropTypes.string.isRequired,
    // If path is not defined don't render anything
    path: PropTypes.string,
  };

  render() {
    if (this.props.path) {
      if (IMAGE_EXTENSIONS.has(getExtension(this.props.path))) {
        return <ShowArtifactImageView runUuid={this.props.runUuid} path={this.props.path}/>;
      } else if (TEXT_EXTENSIONS.has(getExtension(this.props.path))) {
        return <ShowArtifactTextView runUuid={this.props.runUuid} path={this.props.path}/>;
      }
    }
    return (
      <div className="select-preview-outer-container">
        <div className="select-preview-container">
          <div className="select-preview-image-container">
            <img className="select-preview-image" alt="Preview icon." src={previewIcon}/>
          </div>
          <div className="select-preview-text">
            <span className="select-preview-header">Select a file to preview</span>
            <span className="select-preview-supported-formats">
              Supported formats: image and text files
            </span>
          </div>
        </div>
      </div>
    );
  }
}

export const getSrc = (path, runUuid) => {
  const basePath = "get-artifact";
  return `${basePath}?path=${encodeURIComponent(path)}&run_uuid=${encodeURIComponent(runUuid)}`;
};


export default ShowArtifactPage;
