<?php

define(MAIL_REDIRECT_ALLOW, 0);
define(MAIL_REDIRECT_BLOCK, 1);
define(MAIL_REDIRECT_REDIRECT, 2);
define(MAIL_REDIRECT_CUSTOM_REDIRECT, 3);
define(MAIL_REDIRECT_DEFAULT_RULE, MAIL_REDIRECT_REDIRECT);


define(MAIL_REDIRECT_OVERRIDE_DISABLED, 0);
define(MAIL_REDIRECT_OVERRIDE_ALLOW, 1);
define(MAIL_REDIRECT_OVERRIDE_BLOCK, 2);
define(MAIL_REDIRECT_OVERRIDE_REDIRECT, 3);

/**
 * @file
 * This contains functions used by cat_profile_mail_alter to redirect mail on staging systems (to
 * avoid unintentional mail bombs for production users)
 *
 * @author Stuart McDonald after Rohin Knight
 */

/**
  * Function to handle allowing mail through.
  */
function _mail_redirect_allow(&$message) {
  if (variable_get('mail_redirect_log_enabled', TRUE)) {
    watchdog('mail redirect', t('"@mailkey" ALLOWED THROUGH TO @to',
      array('@mailkey' => $message['id'], '@to' => $message['to'])));
  }
}

/**
  * Function to handle mail being blocked, simply sets to field to null.
  */
function _mail_redirect_block(&$message) {
  if (variable_get('mail_redirect_log_enabled', TRUE)) {
    watchdog('mail redirect', '"@mailkey" BLOCKED FROM BEING SENT TO @to',
      array('@mailkey' => $message['id'], '@to' => $message['to']));
  }

  $message['to'] = null;
}

/**
  * Function to handle mail being redirected.
  */
function _mail_redirect_redirect(&$message, $redirect_email) {
  if (variable_get('mail_redirect_log_enabled', TRUE)) {
    watchdog('mail redirection', t('"@mailkey" REDIRECTED FROM @to TO @redir_to', array('@mailkey' => $message['id'], '@to' => $message['to'], '@redir_to' => $redirect_email)));
  }

  $message['subject'] = 'Rerouted email: ' . $message['subject'];

  // The rest of this function code was copied from the reroute_email module: http://drupal.org/project/reroute_email
  global $base_url;

  // Format a message to show at the top
  $msg  = "This email was rerouted.\n";
  $msg .= "Web site: @site\n";
  $msg .= "Mail key: @key\n";
  $msg .= "Originally to: <@to>\n";
  $msg .= "-----------------------\n";

  // Prepend to the body of the email
  $message['body'] = t($msg, array('@site' => $base_url, '@to' => $message['to'], '@key' => $message['id'])) . implode('\n', $message['body']);

  $message['to'] = $redirect_email; // finally, we must redirect the email!
}

/**
  * Get the default_email, or use the site email if one can't be found.
  *
  * @return email to redirect to.
  */
function _mail_redirect_get_redirect_email() {
  return variable_get('mail_redirect_default_email', variable_get('site_mail', null));
}
