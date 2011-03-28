<?php

/**
 * @file
 * Drush integration for Cron Queue API.
 */

/**
 * Implementation of hook_drush_help().
 */
function cronqueue_drush_help($section) {
  switch ($section) {
    case 'drush:cronqueue':
      return dt('Provision cron runs to beanstalkd');
  }
}

/**
 * Implementation of hook_queue_drush_command().
 */
function cronqueue_drush_command() {
  $items['cronqueue'] = array(
    'callback' => 'cronqueue_provision',
    'description' => 'Provision a cron queue.',
  );
  return $items;
}

/**
 * Implementation of hook_postinst().
 *
 * Add custom actions to post install of debian package.
 * @see drush dh-make-drupal
 */
function cronqueue_debian_postinst() {
  return implode(PHP_EOL, array(
    '   // Schedule cron tasks rather than run cron',
    '   $cron_conf["cmd"] = str_replace("cron >>", "cronqueue >>", $cron_conf["cmd"]);',
    '   $cron_conf["minutes"] = "0-59/2";',
  ));
}
