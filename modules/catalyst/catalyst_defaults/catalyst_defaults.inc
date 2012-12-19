<?php
/**
 * @file
 * On demand libraries for catalyst defaults.
 */

/**
 * Form callback: admin settings form.
 */
function catalyst_defaults_settings_form($form, &$form_state) {
  $form['catalyst_defaults_maintenance_wr'] = array(
    '#title' => t('WRMS maintenance work request'),
    '#description' => t('This should be the number of the WR that will be used to administer Drupal maintenance by Catalyst to this site. You must have %perm to be able to configure this settings', array(
      '%perm' => t('Administer software updates'),
    )),
    '#default_value' => variable_get('catalyst_defaults_maintenance_wr', ''),
    '#element_validate' => array('element_validate_number'),
    '#field_prefix' => t('WR#'),
    '#type' => 'textfield',
    '#disabled' => !user_access('administer software updates'),
  );

  return system_settings_form($form);
}
