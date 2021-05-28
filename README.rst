|BadgeLicence| |BadgeBuild| |BadgeRepoSize| |BadgeContributorsNb| |BadgeLastCommitMaster| |BadgeCommitFrequence| |BadgeMainLanguage|

.. |BadgeLicence| image:: https://img.shields.io/github/license/Arnaud-de-Grandmaison/ratrac
    :alt: Github licence
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/blob/main/LICENSE
    
.. |BadgeBuild| image:: https://github.com/Arnaud-de-Grandmaison/ratrac/workflows/C/C++%20CI/badge.svg
    :alt: Github last build status
    :target: https://github.com/Arnaud-de-Grandmaison/ratrac/actions

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

===============================================================================
Ratrac:a ray tracer experiment
===============================================================================



Build
=====

Ratrac can be built from the command line with:

.. code-block:: bash

  $ mkdir build
  $ cd build
  $ cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON -G Ninja ..
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

