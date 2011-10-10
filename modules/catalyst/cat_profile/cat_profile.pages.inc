<?php

/**
 * Callback for healthchecking Catalyst built sites.
 */
function cat_profile_healthcheck() {
  drupal_add_http_header("Pragma", "no-cache");
  drupal_add_http_header("Expires", "0");

  // Build up our list of errors.
  $errors = array();
   
  // Check that the main database is active.
  $account = db_query('SELECT * FROM {users} WHERE uid = 1')->fetchObject();
  if (!$account->uid == 1) {
    $errors[] = 'Master database not responding.';
  }
   
  $account = db_query('SELECT * FROM {users} WHERE uid = 1', array(), array('target' => 'slave'))->fetchObject();
  if (!$account->uid == 1) {
    $errors[] = 'Slave database not responding.';
  }

  $check_modules = array('system');
  drupal_alter('healthcheck_modules', $check_modules);

  foreach ($check_modules as $module) {
    module_load_install($module);
    foreach (module_invoke($module, 'requirements', 'runtime') as $system => $result) {
      if (!isset($result['severity'])) {
        continue;
      }
      if ($result['severity'] == REQUIREMENT_ERROR) {
        $errors[] = t('@service failed: !message', array(
          '@service' => $system,
          '!message' => $result['description'],
        )); 
      }
    }
  }

  $lbe = variable_get('loadbalance_enable_file', '/etc/' . basename(DRUPAL_ROOT) . '/loadbalance_enable');
  if (variable_get('loadbalancing_enabled', FALSE) && !file_exists($lbe)) {
    $errors[] = t('Server out of the load balancer.');
  }

#  // Check that all memcache instances are running on this server.
#  if (isset($conf['cache_inc']) && $conf['memcache_servers']) {
#    foreach ($conf['memcache_servers'] as $address => $bin) {
#      list($ip, $port) = explode(':', $address);
#      if (!memcache_connect($ip, $port)) {
#        $errors[] = 'Memcache bin <em>' . $bin . '</em> at address ' . $address . ' is not available.';
#      }
#    }
#  }
#   
#  // Check that the files directory is operating properly.
#  if ($test = tempnam(variable_get('file_directory_path', conf_path() .'/files'), 'status_check_')) {
#    // Uncomment to check if files are saved in the correct server directory.
#    //if (!strpos($test, '/mnt/nfs') === 0) {
#    //  $errors[] = 'Files are not being saved in the NFS mount under /mnt/nfs.';
#    //}
#    if (!unlink($test)) {
#      $errors[] = 'Could not delete newly create files in the files directory.';
#    }
#  }
#  else {
#    $errors[] = 'Could not create temporary file in the files directory.';
#  }
   
  // Print all errors.
  if ($errors) {
    $errors[] = 'Errors on this server will cause it to be removed from the load balancer.';
    header('HTTP/1.1 500 Internal Server Error');
    print implode("<br />\n", $errors);
  }
  else {
    // Split up this message, to prevent the remote chance of monitoring software
    // reading the source code if mod_php fails and then matching the string.
    print 'Ready';
  }
   
  // Exit immediately, note the shutdown function registered at the top of the file.
  exit();
}
