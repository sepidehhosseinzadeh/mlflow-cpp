#!/usr/bin/env python
# -*- coding: utf-8 -*-
import codecs
import filecmp
import hashlib
import os
import shutil
import pytest
import six
import tarfile

from mlflow.utils import file_utils
from mlflow.utils.file_utils import get_parent_dir
from tests.projects.utils import TEST_PROJECT_DIR

from tests.helper_functions import random_int, random_file


def test_yaml_read_and_write(tmpdir):
    temp_dir = str(tmpdir)
    yaml_file = random_file("yaml")
    long_value = long(1) if six.PY2 else 1  # pylint: disable=undefined-variable
    data = {"a": random_int(), "B": random_int(), "text_value": u"中文",
            "long_value": long_value, "int_value": 32, "text_value_2": u"hi"}
    file_utils.write_yaml(temp_dir, yaml_file, data)
    read_data = file_utils.read_yaml(temp_dir, yaml_file)
    assert data == read_data
    yaml_path = file_utils.build_path(temp_dir, yaml_file)
    with codecs.open(yaml_path, encoding="utf-8") as handle:
        contents = handle.read()
    assert "!!python" not in contents
    # Check that UTF-8 strings are written properly to the file (rather than as ASCII
    # representations of their byte sequences).
    assert u"中文" in contents


def test_mkdir(tmpdir):
    temp_dir = str(tmpdir)
    new_dir_name = "mkdir_test_%d" % random_int()
    file_utils.mkdir(temp_dir, new_dir_name)
    assert os.listdir(temp_dir) == [new_dir_name]

    with pytest.raises(OSError):
        file_utils.mkdir("/   bad directory @ name ", "ouch")


def test_make_tarfile(tmpdir):
    # Tar a local project
    tarfile0 = str(tmpdir.join("first-tarfile"))
    file_utils.make_tarfile(
        output_filename=tarfile0, source_dir=TEST_PROJECT_DIR, archive_name="some-archive")
    # Copy local project into a temp dir
    dst_dir = str(tmpdir.join("project-directory"))
    shutil.copytree(TEST_PROJECT_DIR, dst_dir)
    # Tar the copied project
    tarfile1 = str(tmpdir.join("second-tarfile"))
    file_utils.make_tarfile(
        output_filename=tarfile1, source_dir=dst_dir, archive_name="some-archive")
    # Compare the archives & explicitly verify their SHA256 hashes match (i.e. that
    # changes in file modification timestamps don't affect the archive contents)
    assert filecmp.cmp(tarfile0, tarfile1, shallow=False)
    with open(tarfile0, 'rb') as first_tar, open(tarfile1, 'rb') as second_tar:
        assert hashlib.sha256(first_tar.read()).hexdigest()\
               == hashlib.sha256(second_tar.read()).hexdigest()
    # Extract the TAR and check that its contents match the original directory
    extract_dir = str(tmpdir.join("extracted-tar"))
    os.makedirs(extract_dir)
    with tarfile.open(tarfile0, 'r:gz') as handle:
        handle.extractall(path=extract_dir)
    dir_comparison = filecmp.dircmp(os.path.join(extract_dir, "some-archive"),
                                    TEST_PROJECT_DIR)
    assert len(dir_comparison.left_only) == 0
    assert len(dir_comparison.right_only) == 0
    assert len(dir_comparison.diff_files) == 0
    assert len(dir_comparison.funny_files) == 0


def test_get_parent_dir(tmpdir):
    child_dir = tmpdir.join('dir').mkdir()
    assert str(tmpdir) == get_parent_dir(str(child_dir))
