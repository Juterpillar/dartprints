<?php

/**
 * Enterprise builds only use drupal settings and a base vhost.
 */
$aliases['enterprise11'] = array(
  'dh-api' => 2,
  'cfgmgr-services' => 'conf.settings.drupal,conf.vhost.nginx',
  'dh-version-suffix' => 'enterprise11',
);

/**
 * Smaller builds contain all the configuration into a single build.
 */
$aliases['diy11'] = array(
  'dh-api' => 2,
  'cfgmgr-services' => 'conf.settings.drupal,conf.vhost.nginx,conf.upstream.nginx,conf.pool.php-fpm,conf.crond.cron,conf',
  'dh-version-suffix' => 'diy11',
);
