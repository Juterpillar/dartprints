<?php

include_once dirname(__FILE__) . '/apachesolr.drush.inc';
include_once dirname(__FILE__) . '/civicrm.drush.inc';
include_once dirname(__FILE__) . '/environment.drush.inc';

/**
 * Default path to Memcache caching plugin.
 */
define('MEMCACHE_PLUGIN', realpath(drupal_get_path('module', 'memcache')));

/**
 * Determine if the memcache cache plugin is available.
 */
define('MEMCACHE_AVAILABLE', is_dir(MEMCACHE_PLUGIN));

/**
 * Implementation of hook_drush_command().
 */
function cat_profile_drush_command() {
  $items['loadbalance-enable'] = array(
    'description' => "Enable load balancing if environment is ok.",
    'examples' => array(
      'drush lbe' => 'Sets a file used to indicate to the load balancer that the web server may serve pages.',
    ),
    'aliases' => array('lbe'),
  );
  $items['loadbalance-disable'] = array(
    'description' => "Disable load balancing if environment is ok.",
    'examples' => array(
      'drush lbe' => 'Removes a file used to indicate to the load balancer that the web server may serve pages.',
    ),
    'aliases' => array('lbd'),
  );
  $items['core-check'] = array(
    'description' => "Check the core version of Drupal is the latest",
    'drupal dependencies' => array('system', 'update')
  );
  return $items;
}

/**
 * Enabled load balancing capability.
 */
function drush_cat_profile_loadbalance_enable() {
  $filepath = variable_get('loadbalance_enable_file', '/etc/' . basename(DRUPAL_ROOT) . '/loadbalance_enable');
  if (file_exists($filepath)) {
    return drush_log("Loadbalancing already enabled", 'status');
  }
  if (!is_writeable(dirname($filepath))) {
    return drush_set_error("Cannot write to file path: $filepath. Load balanceing cannot be enabled.");
  }
  file_put_contents($filepath, "Remove this file to disable loadbalancing\n");
  drush_log("Server set to enable load balancing. Please check " . url('healthcheck', array('absolute' => TRUE)) . " is responding without errors", 'success');
}

/**
 * Disabled load balancing capability.
 */
function drush_cat_profile_loadbalance_disable() {
  $filepath = variable_get('loadbalance_enable_file', '/etc/' . basename(DRUPAL_ROOT) . '/loadbalance_enable');
  if (!file_exists($filepath)) {
    return drush_log("Loadbalancing already disabled", 'status');
  }
  if (!unlink($filepath)) {
    return drush_set_error("Cannot unlink file path: $filepath. Load balanceing cannot be disabled.");
  }
  drush_log("Load balancing disabled", 'success');
}

/**
 * Check to see if core is the latest version
 */
function drush_cat_profile_core_check() {

  // Needed to call out to the Drupal.org version API
  drush_include_engine('update_info', 'drupal', NULL, DRUSH_BASE_PATH . '/commands/pm/update_info');

  // Just checking the core project
  $projects = array(
    'drupal' => array(
      'label' => 'Drupal',
      'version' => VERSION,
      'type' => 'core',
      'extensions' => array(),
    )
  );

  // Call the API
  $update_info = pm_get_project_info($projects);
  // Latest version number, likely 7.xx
  $latest_release = array_shift($update_info['drupal']['releases']);
  $latest_version = $latest_release['version'];

  if (version_compare(VERSION, $latest_version, '<')) {
    return drush_set_error('Drupal core is version ' . VERSION . ' but ' . $latest_version . ' is available');
  }
  else {
    return drush_log('Drupal core is running latest ' . VERSION . ' version', 'ok');
  }

}

/**
 * Write code.
 */
function cat_profile_write_code($code, $indent = 4) {
  $code = explode(PHP_EOL, $code);
  $indent = str_pad('', $indent, ' ');
  $contents = $indent . '$code = "<?php" . PHP_EOL;' . PHP_EOL;
  foreach ($code as $line) {
    $contents .= $indent . '$code .= \'' . str_replace("'",  "\'", $line) . '\' . PHP_EOL;' . PHP_EOL;
  }
  return $contents;
}

/**
 * Implementation of hook_postinst().
 *
 * Add custom actions to post install of debian package.
 * @see drush dh-make-drupal
 */
function cat_profile_debian_postinst() {
  $output = '';
  // If the Print PDF module is using wkhtmltopdf, there are two different
  // binaries, one for 64-bit and one for 32bit. We should symlink the correct
  // version based on the server environment.
  if (module_exists('print_pdf')) {
    $wkhtmltopdf = drupal_get_path('module', 'print') . '/lib/wkhtmltopdf';
    if (file_exists($wkhtmltopdf . '-i386') && file_exists($wkhtmltopdf . '-amd64')) {
      $output .= '$wkhtmltopdf = WEBROOT . "/' . $wkhtmltopdf . '";' . PHP_EOL;
      $output .= 'if (!is_link($wkhtmltopdf)) {' . PHP_EOL;
      $output .= '  deb_shell_exec("uname -m");' . PHP_EOL;
      $output .= '  $machine = trim(implode("", deb_shell_exec_output()));' . PHP_EOL;
      $output .= '  if ($machine == "x86_64") {' . PHP_EOL;
      $output .= '    symlink($wkhtmltopdf . "-amd64", $wkhtmltopdf);' . PHP_EOL;
      $output .= '  }' . PHP_EOL;
      $output .= '  else {' . PHP_EOL;
      $output .= '    symlink($wkhtmltopdf . "-i386", $wkhtmltopdf);' . PHP_EOL;
      $output .= '  }' . PHP_EOL;
      $output .= '}' . PHP_EOL;
    }
  }
  if (module_exists('boost')) {
    $output .= '    $cache_link = WEBROOT . "/cache";' . PHP_EOL;
    $output .= '    if (!is_link($cache_link)) {' . PHP_EOL;
    $output .= '      symlink("/var/lib/sitedata/" . PACKAGE_NAME . "/cache", $cache_link);' . PHP_EOL;
    $output .= '    }' . PHP_EOL;
  }

  $output .= <<<CODE
         \$settings .= "\\\$conf['archimedes_server_email'] = 'archimedes@catalyst.net.nz';" . PHP_EOL;
         //\$settings .= "\\\$conf['archimedes_server_location'] = 'archimedes.catalyst.net.nz';" . PHP_EOL;
         \$settings .= "\\\$conf['archimedes_send_method'] = 'sendXML'; // Alternately postXML" . PHP_EOL;
         \$settings .= "\\\$conf['archimedes_server_key'] = '-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDsqgmiokPIN1nYeUQyMgC/bBUo
3Kba9Swn4Srm+as8nuZZaN1f7kO6yeNFNGF/2s1aWD0dF/7YIJ8kkdy46rodXcQM
7BouZ6owmFYFyvgisbSVOp+4WkdoBg3bwo3yl3jC7ObDtG8tLd5kJv1rkHOcf1vF
8k1s+drkUS5erM+jdQIDAQAB
-----END PUBLIC KEY-----';" . PHP_EOL;

CODE;

  $output .= '$settings .= \'$conf["archimedes_site_environment"] = "\' . $debconf->get("environment") . \'";\' . PHP_EOL;' . PHP_EOL;

  if (MEMCACHE_AVAILABLE) {
    $code = array(
    '     if (trim($debconf->get("memcache_hosts")) != "") {',
    '        deb_shell_exec("find %s -name memcache.info", WEBROOT);',
    '        $files = deb_shell_exec_output();',
    '        if (!empty($files[0])) {',
    '           $hosts = trim($debconf->get("memcache_hosts"));',
    '           foreach (explode(" ", $hosts) as $host) {',
    '             $host = trim($host);',
    '             $settings .= "\$conf[\'memcache_servers\'][\'$host\'] = \'default\';" . PHP_EOL;',
    '           }',
    '           $memcache_dir = ltrim(str_replace(WEBROOT, \'\', dirname($files[0])), \'/\');',
    '           if ($debconf->get("memcache_cache", FALSE)) {',
    '             $settings .= "\$conf[\'cache_default_class\'] = \'MemCacheDrupal\';" . PHP_EOL;',
    '             $settings .= "\$conf[\'cache_default_class\'] = \'MemCacheDrupal\';" . PHP_EOL;',
    '             $settings .= "\$conf[\'cache_default_class\'] = \'MemCacheDrupal\';" . PHP_EOL;',
    '             $settings .= "\$conf[\'cache_backends\'][] = \'{$memcache_dir}/memcache.inc\';" . PHP_EOL;',
    '             $settings .= "\$conf[\'memcache_bins\'][\'cache\'] = \'default\';" . PHP_EOL;',
    '           }',
    // As at 6/12/2012 memcache session handling is not yet ported to drupal 7.
    // @ see http://drupal.org/node/1131468
    // The session_inc conf will break drupal 7.
    //'           if ($debconf->get("memcache_sessions", FALSE)) {',
    //'             $settings .= "\$conf[\'session_inc\'] = \'{$memcache_dir}/memcache-session.inc\';" . PHP_EOL;',
    //'           }',
    '        }',
    '     }',
    );
    $output .= implode(PHP_EOL, $code);
  }

  return $output;
}

/**
 * Implementation of hook_templates().
 */
function cat_profile_debian_templates() {
  $output = '';
  // Add templates for memcache sessions.
  $output .= "Template: __PACKAGE__/memcache_hosts" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: " . PHP_EOL;
  $output .= "Description: Memcache hosts:" . PHP_EOL;
  $output .= " If you wish to use memcache, list the hosts seperated by spaces in" . PHP_EOL;
  $output .= " format hostname:port. eg. localhost:11211" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/memcache_cache" . PHP_EOL;
  $output .= "Type: boolean" . PHP_EOL;
  $output .= "Default: true" . PHP_EOL;
  $output .= "Description: Use Memcache for Drupal cache:" . PHP_EOL;
  $output .= " Do you want to use Memcache as the caching mechinism for Drupal?" . PHP_EOL;
  // As at 6/12/2012 memcache session handling is not yet ported to drupal 7.
  // @ see http://drupal.org/node/1131468
  // The session_inc conf will break drupal 7.
  //$output .= PHP_EOL;
  //$output .= "Template: __PACKAGE__/memcache_sessions" . PHP_EOL;
  //$output .= "Type: boolean" . PHP_EOL;
  //$output .= "Default: true" . PHP_EOL;
  //$output .= "Description: Memcache Sessions:" . PHP_EOL;
  //$output .= " Do you want Memcache to handle Drupal sessions?" . PHP_EOL;
  return $output;
}

/**
 * Implementation of hook_config().
 */
function cat_profile_debian_config() {
  $output = "db_input \$PRIORITY __PACKAGE__/memcache_hosts || true" . PHP_EOL;
  $output .= "db_go" . PHP_EOL;
  $output .= "db_get __PACKAGE__/memcache_hosts" . PHP_EOL;
  $output .= "RET=\${RET// /}" . PHP_EOL;
  $output .= 'if [ "$RET" == "" ]; then' . PHP_EOL;
  $output .= "  db_set __PACKAGE__/memcache_cache false" . PHP_EOL;
  //$output .= "  db_set __PACKAGE__/memcache_sessions false" . PHP_EOL;
  $output .= "else" . PHP_EOL;
  $output .= "   db_input \$PRIORITY __PACKAGE__/memcache_cache || true" . PHP_EOL;
  // As at 6/12/2012 memcache session handling is not yet ported to drupal 7.
  // @ see http://drupal.org/node/1131468
  // The session_inc conf will break drupal 7.
  //$output .= "   db_input \$PRIORITY __PACKAGE__/memcache_sessions || true" . PHP_EOL;
  $output .= "fi" . PHP_EOL;
  $output .= "db_go" . PHP_EOL;
  return $output;
}

/**
 * Implementation of hook_debian_create().
 */
function nagios_nrpe_debian_create() {
  // Nagios Support.
  // Generate a Nagios nrpe.cfg format PHP template.
  $checks = (array) module_invoke_all('nagios_nrpe_checks');
  $project = drush_get_option('dh-project');
  foreach ((array) module_invoke_all('nagios_nrpe_checks') as $check => $desc) {
    $commands[$project . '_' . $check] = '/usr/bin/drush -r /var/www/__PACKAGE__ nrpe ' . $check;
  }
  $file = '';
  foreach ($commands as $name => $cmd) {
    $file .= "command[$name]=$cmd\n";
  }
  $filename = '/tmp/' . drush_get_option('dh-package') . '.nrpe.cfg';
  file_put_contents($filename, $file);
  return array($filename => 'nrpe.cfg');
}

/**
 * Implementation of hook_debian_postinst().
 */
function nagios_nrpe_debian_postinst() {
  // Cannot use debian conventions like the dirs file to do this as we're not
  // sure if the Nagios NRPE server is enabled or not.
  $code  = '    // If this server uses Nagios NRPE then add the NRPE checks.' . PHP_EOL;
  $code .= '    if (deb_shell_exec("dpkg -l | grep \'ii  nagios-nrpe-server\'")) {' . PHP_EOL;
  $code .= '      $nagios_cfg = "/etc/nagios/nrpe.d/" . PACKAGE_NAME;' . PHP_EOL;
  $code .= '      if ((is_dir("/etc/nagios/nrpe.d") || mkdir("/etc/nagios/nrpe.d")) && (!is_link($nagios_cfg) && !is_file($nagios_cfg))) {' . PHP_EOL;
  $code .= '        symlink("/etc/" . PACKAGE_NAME . "/nrpe.cfg", $nagios_cfg);' . PHP_EOL;
  $code .= '      }' . PHP_EOL;
  $code .= '    }' . PHP_EOL;

  return $code;
}

/**
 * Implementation of hook_debian_install().
 */
function nagios_nrpe_debian_install() {
  return array(
    array('debian/nrpe.cfg', 'etc/' . drush_get_option('dh-package')),
  );
}
