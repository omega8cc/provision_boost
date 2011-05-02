<?php

/**
 * implementation of provision_verify
 */
function drush_provision_boost_provision_verify($url = null) {
  // TODO: support platform?
  if (d()->type == 'site') {
    $config = new provisionConfig_drushrc_site(d()->name);

    if (isset($config->data['packages']['modules']['boost']) && ($config->data['packages']['modules']['boost']['status'] == 1)) {
      drush_log(dt("configuring site for boost support"));

      $cache_path = d()->root . '/cache';

      if (!is_dir($cache_path)) {
        provision_file()->mkdir($cache_path . '/normal/' . $url)
          ->succeed('Created <code>@path</code>')
          ->fail('Could not create <code>@path</code>')
          ->status();
      }

      provision_file()->chmod($cache_path, 0770, TRUE)
        ->succeed('Changed permissions of <code>@path</code> to @perm')
        ->fail('Could not change permissions <code>@path</code> to @perm')
        ->status();

      provision_file()->chgrp($cache_path, d('@server_master')->web_group, TRUE)
        ->succeed('Change group ownership of settings.php to @path')
        ->fail('Could not change group ownership of settings.php to @path')
        ->status();
    }
  }
}

/**
 * Inject the relevant Apache configuration in the site vhost
 */
function provision_boost_provision_apache_vhost_config($data = null) {
  $config = new provisionConfig_drushrc_site(d()->name);

  if (isset($config->data['packages']['modules']['boost']) && ($config->data['packages']['modules']['boost']['status'] == 1)) {
    module_load_include('inc', 'boost', 'boost.admin');

    $host = variable_get('boost_server_name_http_host', '%{SERVER_NAME}');
    $root = variable_get('boost_document_root', '%{DOCUMENT_ROOT}');

    $htaccess = boost_admin_generate_htaccess($host, $root);

    // return "RewriteEngine on\n" . $htaccess . "\n";
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
  if (d()->type == 'site') {
    /* drush_set_option('boost', drush_get_option('boost'), 'site');
    $config = new provisionConfig_drushrc_site(d()->name);
    drush_log(dt("config = " . print_r($config, 1)));
    $config->data['boost'] = 1;
    $config->write();
    */

    $config = new provisionConfig_drushrc_site(d()->name);

    if (isset($config->data['packages']['modules']['boost']) && ($config->data['packages']['modules']['boost']['status'] == 1)) {
      boost_cache_clear_all();
      drush_log(dt('boost: Flushed ALL files from static page cache.'));
    }
  } elseif (d()->type == 'platform') {
    drush_set_option('boost', drush_get_option('boost'), 'drupal');
  }
}
