define('CIVICRM_UF', 'Drupal');
define('CIVICRM_UF_DSN', 'mysql://<?php print $username; ?>:<?php print $password; ?>@<?php print $host; ?>/<?php print $database; ?>?new_link=true' );
define('CIVICRM_DSN', 'mysql://<?php print $civicrm_user; ?>:<?php print $civicrm_pass; ?>@<?php print $civicrm_host; ?>/<?php print $civicrm_name; ?>?new_link=true' );
global $civicrm_root;
$civicrm_root = '<?php print WEBROOT; ?>/sites/all/modules/civicrm';
define('CIVICRM_TEMPLATE_COMPILEDIR', '<?php print WEBROOT; ?>/sites/default/files/civicrm/templates_c/' );
define('CIVICRM_UF_BASEURL', 'http://<?php print $servername; ?>/' );
define('CIVICRM_SITE_KEY', null );
define('CIVICRM_DOMAIN_ID', 1 );
define('CIVICRM_DOMAIN_GROUP_ID', null );
define('CIVICRM_DOMAIN_ORG_ID', null );
$include_path = '.'        . PATH_SEPARATOR .
                $civicrm_root . PATH_SEPARATOR . 
                $civicrm_root . DIRECTORY_SEPARATOR . 'packages' . PATH_SEPARATOR .
                get_include_path( );
set_include_path( $include_path );
if ( function_exists( 'variable_get' ) && variable_get('clean_url', '0') != '0' ) {
    define( 'CIVICRM_CLEANURL', 1 );
} else {
    define( 'CIVICRM_CLEANURL', 0 );
}
ini_set('auto_detect_line_endings', '1');
$memLimitString = trim(ini_get('memory_limit'));
$memLimitUnit   = strtolower(substr($memLimitString, -1));
$memLimit       = (int) $memLimitString;
switch ($memLimitUnit) {
    case 'g': $memLimit *= 1024;
    case 'm': $memLimit *= 1024;
    case 'k': $memLimit *= 1024;
}
if ($memLimit >= 0 and $memLimit < 67108864) {
    ini_set('memory_limit', '64M');
}
