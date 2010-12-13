<?php

/**
 * Implementation of hook_drush_command().
 */
function nagios_nrpe_drush_command() {
  $items['nagios-nrpe-list'] = array(
    'description' => 'List all available nagios commands for NRPE',
    'arguments' => array(),
    'aliases' => array('nrpe-ls'),
  );
  $items['nagios-nrpe-check'] = array(
    'description' => 'Run a nagios command.',
    'arguments' => array('check'),
    'aliases' => array('nrpe'),
  );
  return $items;
}

/**
 * Drush command callback: nagios-nrpe-list.
 */
function drush_nagios_nrpe_list() {
  $commands = array_merge(module_invoke_all('nagios_nrpe_checks'), drush_command_invoke_all('nagios_nrpe_checks'));
  foreach ($commands as $cmd => $desc) {
    $rows[] = array($cmd, $desc);
  }
  return drush_print_table($rows);
}

/**
 * Drush command callback: nagios-nrpe-check.
 */
function drush_nagios_nrpe_check($check) {
  foreach (module_implements('nagios_nrpe_checks') as $module) {
    if (in_array($check, array_keys(module_invoke($module, 'nagios_nrpe_checks')))) {
      return nagios_nrpe_check($module, $check);
    }
  }
  foreach (drush_command_implements('nagios_nrpe_checks') as $command) {
    $function = $command . '_nagios_nrpe_checks';
    if (in_array($check, array_keys($function()))) {
      return nagios_nrpe_check($command, $check);
    }
  }
  drush_set_error('No such nagios nrpe check exists: ' . $check);
}

/**
 * Helper function for drush_nagios_nrpe_check().
 */
function nagios_nrpe_check($module, $check) {
  $function = $module . '_nagios_check_' . $check;
  if (function_exists($function)) {
    $nagios_data = $function();
  }
  else {
    $function = $module . '_nagios_check';
    $nagios_data = $function($check);
  }
  $key = key($nagios_data);
  $status = nagios_status();
  $message = $key . ' ' . $status[$nagios_data[$key]['status']] . ' - ' . $nagios_data[$key]['text'];
  if ($nagios_data[$key]['type'] == 'perf') {
    $message .= '|' . $key . '=' . $nagios_data[$key]['text'];
  }
  // We want to exit with the correct exit status so NRPE knows the outcome
  // of our drush nrpe check so set DRUSH_EXECUTION_COMPLETED to true.
  drush_set_context('DRUSH_EXECUTION_COMPLETED', TRUE);
  drush_print($message);
  exit((int) $nagios_data[$key]['status']);
}
