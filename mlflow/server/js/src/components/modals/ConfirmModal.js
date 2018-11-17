import React, { Component } from 'react';
import PropTypes from 'prop-types';
import ReactModal from 'react-modal';
import { Button, Modal } from 'react-bootstrap';

const modalStyles = {
  content: {
    top: '50%',
    left: '50%',
    right: 'auto',
    bottom: 'auto',
    marginRight: '-50%',
    transform: 'translate(-50%, -50%)',
    padding: 0,
  },
  overlay: {
    backgroundColor: 'rgba(33, 37, 41, .75)',
  }
};

export class ConfirmModal extends Component {
  constructor(props) {
    super(props);
    this.onRequestCloseHandler = this.onRequestCloseHandler.bind(this);
    this.handleSubmitWrapper = this.handleSubmitWrapper.bind(this);
  }

  static propTypes = {
    isOpen: PropTypes.bool.isRequired,
    // Function which returns a promise which resolves when the submission is done.
    handleSubmit: PropTypes.func.isRequired,
    onClose: PropTypes.func.isRequired,
    title: PropTypes.string.isRequired,
    helpText: PropTypes.string.isRequired,
    confirmButtonText: PropTypes.string.isRequired,
  };

  state = {
    isSubmitting: false,
  };

  onRequestCloseHandler() {
    if (!this.state.isSubmitting) {
      this.props.onClose();
    }
  }

  handleSubmitWrapper() {
    this.setState({ isSubmitting: true });
    this.props.handleSubmit().finally(() => {
      this.props.onClose();
      this.setState({ isSubmitting: false });
    });
  }

  render() {
    return (
      <ReactModal
        isOpen={this.props.isOpen}
        style={modalStyles}
        closeTimeoutMS={200}
        appElement={document.body}
        onRequestClose={this.onRequestCloseHandler}
      >
        <Modal.Header>
          <Modal.Title>
            {this.props.title}
          </Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <p style={{ marginBottom: '10px' }}>
            {this.props.helpText}
          </p>
        </Modal.Body>
        <Modal.Footer>
          <Button
            bsStyle="default"
            disabled={this.state.isSubmitting}
            onClick={this.props.onClose}
            className="mlflow-form-button"
          >
            Cancel
          </Button>
          <Button
            bsStyle="primary"
            onClick={this.handleSubmitWrapper}
            disabled={this.state.isSubmitting}
            className="mlflow-save-button mlflow-form-button"
          >
            {this.props.confirmButtonText}
          </Button>
        </Modal.Footer>
      </ReactModal>
    );
  }
}
