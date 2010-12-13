<?php

function theme_mailkey_table($element) {
  foreach ($element['mailkeys'] as $rec_id => $rec_fieldset) {
    if (is_numeric($rec_id)) {
      $columns = array();
      foreach($rec_fieldset as $fld_name => $fld_setup) {
        if($fld_name[0] != '#') {
          $columns[] = theme($fld_setup['#type'], $fld_setup);
        }
      }
      $rows[] = $columns;
    }
  }

  $header = array(t('Mailkey'), t('Rule'), t('E-mail address'), t('Remove'));
  $output = theme('table', $header, $rows, array('style' => 'width: 100%'));


  // Add javascript settings - we will use these when creating new mailkey rules
  $js_settings = array();
  $js_settings['new_mailkey_next_row_even'] = (count($rows) % 2 == 1);

  foreach ($element['add_mailkey'] as $field_name => $fieldset) {
    $js_settings[$field_name . '_field'] = theme($fieldset['#type'], $fieldset);
  }

  drupal_add_js(array('mail_redirect' => $js_settings), 'setting');

  return $output;
}
