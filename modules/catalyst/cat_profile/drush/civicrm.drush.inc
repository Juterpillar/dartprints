<?php

/**
 * Implementation of hook_debian_create().
 */
function civicrm_debian_create() {
  if (module_exists('civicrm')) {
    return array(
      dirname(__FILE__) . '/civicrm.settings.php' => 'civicrm.settings.tpl.php',
    );
  }
  return array();
}

/**
 * Implementation of hook_debian_install().
 */
function civicrm_debian_install() {
  return array(
    array('debian/civicrm.settings.tpl.php', 'etc/' . drush_get_option('dh-package')),
  );
}

/**
 * Implementation of hook_debian_postinst().
 */
function civicrm_debian_postinst() {
  $code = '';
  $settings =  '    // Written by Cat Profile on behalf of civicrm. //' . PHP_EOL;
  $settings .= '    define("CIVICRM_SETTINGS", WEBROOT . "/sites/default/civicrm.settings.php");' . PHP_EOL;
  $settings .= '    $civicrm_conf = $databases["default"]["default"];' . PHP_EOL;
  $settings .= '    $civicrm_conf["civicrm_user"] = $debconf->get("civicrm_user", PACKAGE_NAME);' . PHP_EOL;
  $settings .= '    $civicrm_conf["civicrm_pass"] = $debconf->get("civicrm_pass");' . PHP_EOL;
  $settings .= '    $civicrm_conf["civicrm_host"] = $debconf->get("civicrm_host");' . PHP_EOL;
  $settings .= '    $civicrm_conf["civicrm_name"] = $debconf->get("civicrm_database", PACKAGE_NAME);' . PHP_EOL;
  $settings .= '    $civicrm_conf["servername"] = $debconf->get("servername");' . PHP_EOL;
  $settings .= '    $civicrm_file = "<?php" . PHP_EOL . deb_template("/etc/" . PACKAGE_NAME . "/civicrm.settings.tpl.php", $civicrm_conf);' . PHP_EOL;
  $settings .= "    file_put_contents(CIVICRM_SETTINGS, \$civicrm_file);" . PHP_EOL;
  $settings .= '    $databases["civicrm"]["default"] = array(' . PHP_EOL;
  $settings .= '      "driver"   => $debconf->get("db_server", "mysql"),' . PHP_EOL;
  $settings .= '      "username" => $debconf->get("civicrm_user", PACKAGE_NAME),' . PHP_EOL;
  $settings .= '      "password" => $debconf->get("civicrm_pass"),' . PHP_EOL;
  $settings .= '      "host"     => $debconf->get("civicrm_host", "localhost"),' . PHP_EOL;
  $settings .= '      "port"     => $debconf->get("civicrm_port"),' . PHP_EOL;
  $settings .= '      "database" => $debconf->get("civicrm_database", PACKAGE_NAME),' . PHP_EOL;
  $settings .= '      /*"prefix"   => $debconf->get("civicrm_db_prefix"),//*/' . PHP_EOL;
  $settings .= '    );' . PHP_EOL;
  return $settings;
}

/**
 * Implementation of hook_config().
 */
function civicrm_debian_config() {
  $output = '';

  // Civicrm requires an addtional settings php file.
  // We must create config and templates for it.
  // @see civicrm_debian_tempaltes().
  $output .= "db_input \$PRIORITY __PACKAGE__/civicrm_host || true" . PHP_EOL;
  $output .= "db_input \$PRIORITY __PACKAGE__/civicrm_port || true" . PHP_EOL;
  $output .= "db_input \$PRIORITY __PACKAGE__/civicrm_user || true" . PHP_EOL;
  $output .= "db_input \$PRIORITY __PACKAGE__/civicrm_pass || true" . PHP_EOL;
  $output .= "db_input \$PRIORITY __PACKAGE__/civicrm_database || true" . PHP_EOL;
  $output .= "db_get __PACKAGE__/db_server" . PHP_EOL;
  $output .= 'if [ "$RET" == "pgsql" ]; then' . PHP_EOL;
  $output .= "  db_input high __PACKAGE__/civicrm_notice || true" . PHP_EOL;
  $output .= "  db_set __PACKAGE__/db_server mysql" . PHP_EOL;
  $output .= "fi" . PHP_EOL;
  $output .= "db_go" . PHP_EOL;
  return $output;
}

/**
 * Implementation of hook_templates().
 */
function civicrm_debian_templates() {
  $output = '';
  // Add templates for civicrm.
  $output .= "Template: __PACKAGE__/civicrm_host" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: " . PHP_EOL;
  $output .= "Description: CiviCRM Database host:" . PHP_EOL;
  $output .= " Which database host should be used for this civicrm site?" . PHP_EOL;
  $output .= " You can leave this blank to connect via UNIX sockets" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/civicrm_port" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default:" . PHP_EOL;
  $output .= "Description: CiviCRM Database port:" . PHP_EOL;
  $output .= " Which database port should be used for this civicrm site?" . PHP_EOL;
  $output .= " You can leave this blank for the default" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/civicrm_user" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: internetnz" . PHP_EOL;
  $output .= "Description: CiviCRM Database username" . PHP_EOL;
  $output .= " Which username should be used to connect to the civicrm database?" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/civicrm_pass" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default:" . PHP_EOL;
  $output .= "Description: CiviCRM Database password:" . PHP_EOL;
  $output .= " Which password should be used to connect to the civicrm database? - leave blank for none" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/civicrm_database" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: internetnz_civicrm" . PHP_EOL;
  $output .= "Description: CiviCRM Database name" . PHP_EOL;
  $output .= " Which database should civicrm be connected to?" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/civicrm_notice" . PHP_EOL;
  $output .= "Type: note" . PHP_EOL;
  $output .= "Description: This drupal package contains CiviCRM. The database server will be manually set to MySQL " . PHP_EOL;
  $output .= " because CiviCRM requires MySQL." . PHP_EOL;
  return $output;
}
