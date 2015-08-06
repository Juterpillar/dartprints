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

  // Warn about the integrated "Poor man's cron" in drupal_page_footer().
  // @see catalyst_defaults_requirements()
  $form['catalyst_defaults_automated_cron_warning'] = array(
    '#title' => t('Warn when automated cron is enabled'),
    '#description' => t("Add a warning to the status report about the 'automated cron' mechanism, which is enabled in Drupal by default, but undesirable for Catalyst sites in general.")
    . '<br/>' . t("Details: <code>drupal_page_footer()</code> runs <code>system_run_automated_cron()</code> based on the <code>cron_safe_threshold</code> variable. This means that the full weight of a cron run is added to whichever requests happen to trigger it, potentially causing poor response times and unexpected memory usage spikes, and perhaps exceeding the memory limits if the request in question happened to be a heavy one to begin with.")
    . '<br/>' . t("This is never desirable if a genuine cron task has been configured to run the Drupal cron hooks."),
    '#default_value' => variable_get('catalyst_defaults_automated_cron_warning', TRUE),
    '#type' => 'checkbox',
  );

  return system_settings_form($form);
}
