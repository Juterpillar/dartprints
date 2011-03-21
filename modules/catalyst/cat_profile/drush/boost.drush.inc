<?php

/**
 * Implementation of hook_debian_postinst().
 */
function boost_debian_postinst() {
  module_load_include('inc', 'boost', 'boost.admin');
  $htaccess = boost_admin_generate_htaccess(variable_get('boost_server_name_http_host', '%{SERVER_NAME}'), variable_get('boost_document_root', '%{DOCUMENT_ROOT}'));
  $code = '    if (APACHE_ENABLED && !NGINX_ENABLED) {' . PHP_EOL;
  foreach (explode(PHP_EOL, $htaccess) as $line) {
    $code .= '      $apache_conf["directory_custom"] .= "' . str_replace('"', '\"', $line) . '" . PHP_EOL;' . PHP_EOL;
  }
  $code .= '    }' . PHP_EOL;
  $code .= '    if (APACHE_ENABLED) {' . PHP_EOL;
  $code .= '      deb_shell_exec("a2enmod headers");' . PHP_EOL;
  $code .= '    }' . PHP_EOL;
  $code .= '    if (!@readlink(WEBROOT . "/cache")) {' . PHP_EOL;
  $code .= '      if (!symlink("/var/lib/sitedata/" . PACKAGE_NAME . "/cache", WEBROOT . "/cache")) {' . PHP_EOL;
  $code .= '        echo "ERROR: Boost directory cannot be symlinked from /var/lib/sitedata/" . PACKAGE_NAME . "/cache to " . WEBROOT . "/cache" . PHP_EOL;' . PHP_EOL;
  $code .= '        die(1);' . PHP_EOL;
  $code .= '      }' . PHP_EOL;
  $code .= '    }' . PHP_EOL;
  $code .= '    chown(WEBROOT . "/cache", WEBUSER);' . PHP_EOL;
  $code .= '    chmod(WEBROOT . "/cache", 0755);' . PHP_EOL;
  return $code;
}

