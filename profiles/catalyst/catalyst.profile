<?php
/**
 * @file
 * Enables modules and site configuration for a catalyst site installation.
 */

// Send user straight to catalyst profile installation. This ensures that
// everything gets installed as per requirements.
if (!isset($_GET['profile']) && $_SERVER['SCRIPT_NAME'] == (base_path() . 'install.php')) {
  header('Location: ' . base_path() . 'install.php?profile=catalyst&locale=en');
}

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function catalyst_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}


/**
 * Implements hook_form_alter().
 */
function catalyst_form_install_select_profile_form_alter() {
}
