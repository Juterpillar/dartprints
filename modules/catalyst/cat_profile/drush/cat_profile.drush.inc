<?php

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
  return $output;
}

/**
 * Implementation of hook_dh_make_prebuild().
 */
function cat_profile_dh_make_prebuild($dir) {
  $ini_file = $dir. '/info.ini';
  if (is_file($ini_file)) {
    $ini = parse_ini_file($ini_file, TRUE);
  }
  else {
    $ini = array();
  }
  if ($vc_engine = drush_pm_include_version_control(DRUPAL_ROOT)) {
    if ($vc_engine->name() == 'git') {
      $ini['git']['commit'] = $vc_engine->hash();
      $gitconfig = parse_ini_file(DRUPAL_ROOT . '/.git/config', TRUE);
      foreach ($gitconfig as $section => $config) {
        if (!preg_match_all('/remote_(*)/', $section, $matches)) {
          continue;
        }
        $ini['git']['remote_' . $matches[1][0]] = $config['url'];
      }
    }
  }
  if (drush_get_option('wrms', FALSE)) {
    $ini['wrms']['wr'] = drush_get_option('wrms', array());
  }
  cat_profile_write_ini_file($ini, $ini_file, TRUE);
  return array($ini_file => 'info.ini');
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

/**
 * Implementation of hook_debian_install().
 */
function cat_profile_debian_install() {
  return array(
    array('debian/info.ini', 'etc/' . drush_get_option('dh-package')),
  );
}

function cat_profile_write_ini_file($assoc_arr, $path, $has_sections=FALSE) {
    $content = "";

    if ($has_sections) {
      foreach ($assoc_arr as $key => $elem) {
        $content .= "[".$key."]\n";
        foreach ($elem as $key2 => $elem2) {
            if(is_array($elem2)) {
              for($i=0;$i<count($elem2);$i++) {
                $content .= $key2."[] = \"".$elem2[$i]."\"\n";
              }
            }
            else if($elem2=="") $content .= $key2." = \n";
            else $content .= $key2." = \"".$elem2."\"\n";
        }
      }
    }
    else {
      foreach ($assoc_arr as $key=>$elem) {
        if(is_array($elem)) {
          for($i=0;$i<count($elem);$i++) {
            $content .= $key2."[] = \"".$elem[$i]."\"\n";
          }
        }
        else if($elem=="") $content .= $key2." = \n";
        else $content .= $key2." = \"".$elem."\"\n";
      }
    }

    if (!$handle = fopen($path, 'w')) {
        return false;
    }
    if (!fwrite($handle, $content)) {
        return false;
    }
    fclose($handle);
    return true;
}
