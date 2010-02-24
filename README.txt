Boost support for Aegir
=======================

This Drush module will alter the Aegir platform configurations to add the right
rewrite rules. It will also create the boost directories needed for proper
operation, with proper permissions.

Installation instructions
-------------------------

Drop this directory in Aegir's Drush path (e.g. /var/aegir/.drush).

Then you will need to manually verify the sites you want using the right flags
so that the configuration is enabled.

Platform example
~~~~~~~~~~~~~~~~

cd drupal-6.14 ; drush provision verify --platform_boost=1 --platform_id=12345

The platform ID is important here: it is essential to generate the right
configuration file.

Site example
~~~~~~~~~~~~
cd drupal-6.14 ; drush provision verify --site_boost=1

The argument to site or platform_boost determines which boost rewrite rules
will be included in the Apache configuration. If you are running a multisite
install, you probably want type 2 instead of type 1.

Caveats
-------

Platforms and sites need to be verified before this takes effect.

There is no frontend yet.
