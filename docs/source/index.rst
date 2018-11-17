MLflow Documentation
====================

MLflow is an open source platform for managing the end-to-end machine learning lifecycle.
It tackles three primary functions:

* Tracking experiments to record and compare parameters and results (:ref:`tracking`).
* Packaging ML code in a reusable, reproducible form in order to share with other data
  scientists or transfer to production (:ref:`projects`).
* Managing and deploying models from a variety of ML libraries to a variety of model serving and
  inference platforms (:ref:`models`).

MLflow is library-agnostic. You can use it with any machine learning library, and in any
programming language, since all functions are accessible through a :ref:`rest-api`
and :ref:`CLI<cli>`. For convenience, the project also includes a :ref:`python-api`.

Get started using the :ref:`quickstart` or by reading about the :ref:`key concepts<concepts>`.

.. toctree::
    :maxdepth: 1

    quickstart
    tutorial
    concepts
    tracking
    projects
    models
    cli
    python_api/index
    R-api
    java_api/index
    rest-api

.. warning::

    The current version of MLflow is an alpha release. This means that APIs and storage formats
    are subject to breaking change.
