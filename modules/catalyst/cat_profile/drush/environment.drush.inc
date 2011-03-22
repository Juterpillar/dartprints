<?php

/**
 * Implementation of hook_debian_postinst().
 */
function environment_debian_postinst() {
  $code = '';
  if (module_exists('environment')) {
    $settings .= "    \$conf['environment'] = '\"' . \$debconf->get('environment') . '\"' . PHP_EOL;" . PHP_EOL;
    return $settings;
  }
}

/**
 * Implementation of hook_config().
 */
function environment_debian_config() {
  $output = '';
  if (module_exists('environment')) {
    $output .= "db_input \$PRIORITY __PACKAGE__/environment || true" . PHP_EOL;
    $output .= "db_go" . PHP_EOL;
  }
  return $output;
}

/**
 * Implementation of hook_templates().
 */
function environment_debian_templates() {
  $output = '';
  // Add templates for environment.
  if (module_exists('environment')) {
    $choices = array();
    foreach (environment_load() as $key => $env) {
      $choices[] = $key;
    }
    $output .= "Template: __PACKAGE__/environment" . PHP_EOL;
    $output .= "Type: select" . PHP_EOL;
    $output .= "Choices: " . implode(', ', $choices) . PHP_EOL;
    $output .= "Default: production" . PHP_EOL;
    $output .= "Description: What environment is this install?" . PHP_EOL;
    $output .= " Please choice one of " . count($choices) . " environments:" . PHP_EOL;
    $output .= PHP_EOL;
  }
  return $output;
}
