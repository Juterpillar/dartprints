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
  $form['site_information']['site_mail']['#default_value'] = 'no-reply@' . $_SERVER['SERVER_NAME'];

  $form['server_settings']['site_default_country']['#default_value'] = 'NZ';
  $form['server_settings']['#collapsible'] = TRUE;
  $form['server_settings']['#collapsed'] = TRUE;

  $form['update_notifications']['update_status_module']['#default_value'] = array();
  $form['update_notifications']['#collapsible'] = TRUE;
  $form['update_notifications']['#collapsed'] = TRUE;

  if (module_exists('archimedes')) {
    $form['update_notifications']['warning']['#markup'] = t('<strong>It is not recommended to use update notifications as Archimedes can do this for you</strong>');
  }

  // If git is installed, use the developers email address.
  if ($git_bin = trim(`which git`)) {
    if ($mail = trim(`$git_bin config user.email`)) {
      $form['admin_account']['account']['mail']['#default_value'] = $mail;
    }
  }

}

