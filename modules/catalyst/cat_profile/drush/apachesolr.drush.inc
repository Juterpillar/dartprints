<?php

/**
 * Implementation of hook_debian_postinst().
 */
function apachesolr_debian_postinst() {
    $settings =  '    // Written by Cat Profile on behalf of apachesolr. //' . PHP_EOL;
    $settings .= '    $settings .= \'$conf["apachesolr_path"] = "\' . $debconf->get("apachesolr_path") . \'" . PHP_EOL;\';' . PHP_EOL;
    $settings .= '    $settings .= \'$conf["apachesolr_host"] = "\' . $debconf->get("apachesolr_host") . \'" . PHP_EOL;\';' . PHP_EOL;
    $settings .= '    $settings .= \'$conf["apachesolr_port"] = "\' . $debconf->get("apachesolr_port") . \'" . PHP_EOL;\';' . PHP_EOL;
    return $settings;
}

/**
 * Implementation of hook_config().
 */
function apachesolr_debian_config() {
  $output = '';

  // @see apachesolr_debian_tempaltes().
  $output .= "db_input \$PRIORITY __PACKAGE__/apachesolr_host || true" . PHP_EOL;
  $output .= "db_input \$PRIORITY __PACKAGE__/apachesolr_port || true" . PHP_EOL;
  $output .= "db_input \$PRIORITY __PACKAGE__/apachesolr_path || true" . PHP_EOL;
  $output .= "db_go" . PHP_EOL;
  return $output;
}

/**
 * Implementation of hook_templates().
 */
function apachesolr_debian_templates() {
  $output = '';
  // Add templates for apachesolr.
  $output .= "Template: __PACKAGE__/apachesolr_host" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: " . PHP_EOL;
  $output .= "Description: Solr Host:" . PHP_EOL;
  $output .= " Which server hosts Apache Solr?" . PHP_EOL;
  $output .= " " . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/apachesolr_port" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: 8983" . PHP_EOL;
  $output .= "Description: ApacheSolr port:" . PHP_EOL;
  $output .= " Which port is Solr listening on?" . PHP_EOL;
  $output .= PHP_EOL;
  $output .= "Template: __PACKAGE__/apachesolr_path" . PHP_EOL;
  $output .= "Type: string" . PHP_EOL;
  $output .= "Default: /solr/__PACKAGE__" . PHP_EOL;
  $output .= "Description: Path to solr queryable interface." . PHP_EOL;
  $output .= " This is the path to which queries can be appended to." . PHP_EOL;
  $output .= PHP_EOL;
  return $output;
}
