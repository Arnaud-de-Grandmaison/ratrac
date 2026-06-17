|CIUbuntu2404| |CIUbuntu2604| |CIMacOS26|
|BadgeRepoSize| |BadgeContributorsNb| |BadgeLastCommitMaster| |BadgeCommitFrequence| |BadgeMainLanguage| |BadgeLicence|

.. |CIUbuntu2404| image:: https://github.com/Arnaud-de-Grandmaison/ratrac/actions/workflows/ubuntu-24.04.yml/badge.svg
    :alt: Last build status on Ubuntu 24.04
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/actions/workflows/ubuntu-24.04.yml

.. |CIUbuntu2604| image:: https://github.com/Arnaud-de-Grandmaison/ratrac/actions/workflows/ubuntu-26.04.yml/badge.svg
    :alt: Last build status on Ubuntu 26.04
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/actions/workflows/ubuntu-26.04.yml

.. |CIMacOS26| image:: https://github.com/Arnaud-de-Grandmaison/ratrac/actions/workflows/macOS-26.yml/badge.svg
    :alt: Last build status on macOS 26
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/actions/workflows/macOS-26.yml

.. |BadgeRepoSize| image:: https://img.shields.io/github/repo-size/Arnaud-De-Grandmaison/ratrac
    :alt: Github size
    :target: #

.. |BadgeContributorsNb| image:: https://img.shields.io/github/contributors/arnaud-de-grandmaison/ratrac
    :alt: Github number of contributors
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/graphs/contributors

.. |BadgeLastCommitMaster| image:: https://img.shields.io/github/last-commit/arnaud-de-grandmaison/ratrac/main
    :alt: Github last commit day on main branch
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/commits/main

.. |BadgeCommitFrequence| image:: https://img.shields.io/github/commit-activity/m/arnaud-de-grandmaison/ratrac
    :alt: GitHub commit activity
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/graphs/commit-activity

.. |BadgeMainLanguage| image:: https://img.shields.io/github/languages/top/arnaud-de-grandmaison/ratrac
    :alt: Github main language
    :target: #

.. |BadgeLicence| image:: https://img.shields.io/github/license/Arnaud-de-Grandmaison/ratrac
    :alt: Github licence
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/blob/main/LICENSE

===============================================================================
Ratrac : a ray tracer experiment
===============================================================================

Ratrac is our C++ implementation of `The Ray Tracer Challenge <http://raytracerchallenge.com/>`_ excellent book from Jamis Buck.

Build
=====

Ratrac can be built from the command line with:

.. code-block:: bash

  $ mkdir build
  $ cd build
  $ CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON -G Ninja ..
  $ ninja

Test
====

Ratrac testing relies on googletest_. The tests are run n the build directory with:

.. code-block:: bash

  $ cd build
  $ ninja check

Use
===

Enjoy !

.. _googletest: https://github.com/google/googletest
