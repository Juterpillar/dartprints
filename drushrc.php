<?php

/**
 * @file Drush configuration file.
 *
 * This file is apart of the Catalyst distribution
 * of this Drupal install. 
 *
 * DO NOT MANIPULATE THIS FILE
 *
 * This file is maintained from an upstream source.
 * If you wish to add drush configurations to this
 * site install, please consider using
 * ./sites/default/drushrc.php or ./drushrc.local.php
 *
 */

/**
 * @group Drush Debian Packaging
 * @see http://drupal.org/sandbox/fiasco/1087202
 */

/**
 * Set modules that must be enabled before building a debian package.
 */
  $options['dh-make-depends'] = array(
    'cat_profile',
    'environment',
    'archimedes',
    'reroute_email',
  );

/**
 * Make recommendations of modules to install before building a debian package.
 */
  $options['dh-make-recommends'] = array(
    'boost',
  );

/**
 * Identify any modules that will conflict with the build of this package.
 */
  $options['dh-make-conflicts'] = array(
    'drupal_status',
  );

/**
 * @EndGroup Drush Debian Packaging
 */

/**
 * Load local drush config at a global scope relative to
 * this Drupal installation.
 */
if (file_exists(dirname(__FILE__) . '/drushrc.local.php')) {
  include_once dirname(__FILE__) . '/drushrc.local.php';
}
