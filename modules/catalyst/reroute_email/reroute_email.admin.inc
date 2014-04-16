<?php

/**
 * @file
 * Reroute Email admin configuration functions.
 */

/**
 * Settings form.
 */
function reroute_email_settings() {
  $form[REROUTE_EMAIL_ENABLE] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable rerouting'),
    '#default_value' => variable_get(REROUTE_EMAIL_ENABLE, 0),
    '#description'   => t('Check this box if you want to enable email rerouting. Uncheck to disable rerouting.'),
  );
  $form[REROUTE_EMAIL_TO] = array(
    '#type'          => 'textarea',
    '#title'         => t('Destination email addresses'),
    '#default_value' => reroute_email_destination(),
    '#description'   => t('Provide a space, comma, semicolon, or newline-delimited list of email addresses to reroute email messages to.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );

  $form['whitelist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Whitelists'),
    '#description' => t('Permit delivery to these addresses and/or domains. Mail for non-whitelisted email addresses will be rerouted to the destination addresses above.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );
  $form['whitelist'][REROUTE_EMAIL_WHITELIST] = array(
    '#type'          => 'textarea',
    '#title'         => t('Email address whitelist'),
    '#default_value' => variable_get(REROUTE_EMAIL_WHITELIST, REROUTE_EMAIL_DEFAULT_ADDRESS),
    '#description'   => t('Provide a space, comma, semicolon, or newline-delimited list of email addresses to permit delivery to.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );
  $form['whitelist'][REROUTE_EMAIL_DOMAIN_WHITELIST] = array(
    '#type'          => 'textfield',
    '#title'         => t('Domain whitelist'),
    '#default_value' => variable_get(REROUTE_EMAIL_DOMAIN_WHITELIST, ''),
    '#description'   => t('Provide a space, comma, or semicolon-delimited list of domains to permit delivery to.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );

  $form[REROUTE_EMAIL_ENABLE_MESSAGE] = array(
    '#type' => 'checkbox',
    '#title' => t('Show rerouting description in mail body'),
    '#default_value' => variable_get(REROUTE_EMAIL_ENABLE_MESSAGE, 1),
    '#description' => t('Check this box if you want a message to be inserted into the email body when the mail is being rerouted. Otherwise, SMTP headers will be used to describe the rerouting. If sending rich-text email, leave this unchecked so that the body of the email will not be disturbed.'),
    '#states' => array(
      'visible' => array(':input[name=reroute_email_enable]' => array('checked' => TRUE)),
    ),
  );

  return system_settings_form($form);
}

/**
 * Validation callback for reroute_email_settings() form.
 */
function reroute_email_settings_validate($form, $form_state) {
  if ($form_state['values']['reroute_email_enable'] == TRUE) {
    // Destination addresses.
    $destination = reroute_email_split_string($form_state['values'][REROUTE_EMAIL_TO]);
    foreach ($destination as $address) {
      if (!valid_email_address($address)) {
        form_set_error(REROUTE_EMAIL_TO, t('@address is not a valid email address', array('@address' => $address)));
      }
    }
    // Whitelisted addresses.
    $whitelist = reroute_email_split_string($form_state['values'][REROUTE_EMAIL_WHITELIST]);
    foreach ($whitelist as $address) {
      if (!valid_email_address($address)) {
        form_set_error(REROUTE_EMAIL_WHITELIST, t('@address is not a valid email address', array('@address' => $address)));
      }
    }
    // Whitelisted domains.
    $domain_whitelist = reroute_email_split_string($form_state['values'][REROUTE_EMAIL_DOMAIN_WHITELIST]);
    foreach ($domain_whitelist as $domain) {
      if (!valid_email_address("user@" . $domain)) {
        form_set_error(REROUTE_EMAIL_DOMAIN_WHITELIST, t('@domain is not a valid domain', array('@domain' => $domain)));
      }
    }
  }
}

/**
 * Form for sending test messages.
 */
function reroute_email_test_email_form() {
  return array(
    'addresses' => array(
      '#type' => 'fieldset',
      'to' => array(
        '#type' => 'textfield',
        '#title' => t('To'),
        '#required' => TRUE,
      ),
      'cc' => array(
        '#type' => 'textfield',
        '#title' => t('Cc'),
      ),
      'bcc' => array(
        '#type' => 'textfield',
        '#title' => t('Bcc'),
      ),
    ),
    'subject' => array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => t('Reroute Email Test'),
    ),
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Send email'),
    ),
  );
}

/**
 * Submit handler for email test.
 */
function reroute_email_test_email_form_submit(&$form, &$form_state) {
  $to = $form_state['values']['to'];
  $param_keys = array('cc', 'bcc', 'subject', 'body');
  $params = array_intersect_key($form_state['values'], array_flip($param_keys));
  $message = drupal_mail('reroute_email', 'test_email_form', $to, language_default(), $params);
  if (!empty($message['result'])) {
    drupal_set_message(t("Test email submitted for delivery."));
  }
}
