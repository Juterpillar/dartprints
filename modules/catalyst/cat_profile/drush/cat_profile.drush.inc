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
  return $output;
}
