<?php

/**
 * implementation of provision_verify
 */
function drush_provision_boost_provision_verify($url = null) {
  if (PROVISION_CONTEXT_PLATFORM && drush_get_option('boost')) {
    drush_log(dt("configuring platform for boost support"));
    if (!is_dir("cache")) {
      provision_path("mkdir", "cache", TRUE, 
        dt("Created <code>@path</code>"),
        dt("Could not create <code>@path</code>"),
        'DRUSH_PERM_ERROR');
    }
    provision_path("chmod", "cache", 0770, 
      dt("Changed permissions of <code>@path</code> to @confirm"),
      dt("Could not change permissions <code>@path</code> to @confirm"),
      'DRUSH_PERM_ERROR');
    provision_path("chgrp", "cache", drush_get_option('web_group'),
      dt('Change group ownership of settings.php to @confirm'),
      dt('Could not change group ownership of settings.php to @confirm'));
  } elseif (PROVISION_CONTEXT_SITE && drush_get_option('boost')) {
    # nothing to do here (yet?)
    # we still log this because we add config settings to the vhost (below)
    drush_log(dt("configuring site for boost support"));
  }
}

/**
 * Inject the relevant Apache configuration in the site vhost
 */
function provision_boost_provision_apache_vhost_config($data = null) {
  if (($version = drush_get_option('boost')) && is_numeric($version)) {
    return file_get_contents(dirname(__FILE__) . "/boosted1.txt");
  }
}

/**
 * Inject the relevant Apache configuration into the global apache configuration
 */
function provision_boost_provision_apache_dir_config($data = null) {
  if (($version = drush_get_option('boost')) && is_numeric($version)) {
    return file_get_contents(dirname(__FILE__) . "/boosted$version.txt");
  }
}

/**
 * Persist boost settings in the drushrc.php
 */
function drush_provision_boost_post_provision_verify($url = NULL) {
  if (PROVISION_CONTEXT_SITE) {
    drush_set_option('boost', drush_get_option('boost'), 'site');
  } elseif (PROVISION_CONTEXT_PLATFORM) {
    drush_set_option('boost', drush_get_option('boost'), 'drupal');
  }
}
