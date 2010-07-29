<?php

/**
 * The Drupal AHAH framework won't execpt [, ], (, ), : or .
 * as characters in input name attributes, therefore, we need
 * to use a seperator that is compliant and doesn't conflict
 * with existing namespaces. For example, a block's unique
 * identifier is the theme, module and delta combine; for
 * features that needs to be one key so wee need to seperate
 * them with a seperator delimiter.
 */
define('CAT_PROFILE_FEATURES_DELIMITER', '----');

/**********
 *  Core  *
 *********/

/**
 * @ingroup url_alias
 * @{
 */

/**
 * Implementation of hook_features_export().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_url_aliases_features_export($data, &$export, $module_name = '') {
  foreach ($data as $url) {
    $export['features']['cat_profile_url_aliases'][$url] = $url;
    list($theme, $module, $delta) = explode(CAT_PROFILE_FEATURES_DELIMITER, $key);
  }
  $export['dependencies'][] = 'path';
}

/**
 * Implementation of hook_features_export_render().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_url_aliases_features_export_render($module_name, $data) {
  module_load_include('inc', 'cat_profile');
  $export = cat_profile_export_table('url_alias', array('src' => $data));
  $code = '  $data = ' . features_var_export($export, '  ') . ';' . PHP_EOL;
  $code .= '  return $data;';
  return array('cat_profile_url_aliases' => $code);
}

/**
 * Implementation of hook_features_revert().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_url_alias_features_revert($module) {
  $data = module_invoke($module, 'cat_profile_url_aliases');
  foreach ($data['url_alias'] as $row) {
    cat_profile_data_revert($data, 'url_alias', array('src' => $row['src']));
  }
}

/**
 * @} End of "ingroup url_alias".
 */

/***********
 * Contrib *
 **********/

/**
 * @ingroup messaging.
 * @{
 */

/**
 * Implementation of hook_features_export_options().
 */
function cat_profile_messaging_features_export_options() {
  $rs = db_query('SELECT type, msgkey, method FROM {messaging_message_parts} ORDER BY type, method, msgkey');
  while ($row = db_fetch_array($rs)) {
    $key = implode(CAT_PROFILE_FEATURES_DELIMITER, $row);
    $options[$key] = $key;
  }
  return $options;
}

/**
 * Implementation of hook_features_export().
 */
function cat_profile_messaging_features_export($data, &$export, $module_name = '') {
  foreach ($data as $preset) {
    $export['features']['cat_profile_messaging'][$preset] = $preset;
  }
}

/**
 * Implementation of hook_features_export_render().
 */
function cat_profile_messaging_features_export_render($module_name = '', $data) {
  foreach ($data as $key) {
    $args = explode(CAT_PROFILE_FEATURES_DELIMITER, $key);
    $message = db_result(db_query("SELECT message FROM {messaging_message_parts} WHERE type = '%s' AND msgkey = '%s' AND method = '%s'", $args));
    $presets[$key] = $message;
  }
  $code = "  \$message_parts = ". features_var_export($presets, '  ') .";\n";
  $code .= '  return $message_parts;';
  return array('cat_profile_messaging' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function cat_profile_messaging_features_revert($module = 'messaging') {
  module_load_include('inc', 'messaging', 'messaging.admin');
  $defaults = module_invoke($module, 'messaging_features_store');
  if (empty($defaults)) {
    return;
  }
  $settings = array();
  foreach ($defaults as $key => $message) {
    list($type, $msgkey, $method) = explode(CAT_PROFILE_FEATURES_DELIMITER, $key);
    $settings[$type][$msgkey][$method] = $message;
  }
  foreach ($settings as $group => $messages) {
    $form_values = array(
      'values' => array(
        'group' => $group,
        'messages' => $messages,
      ),
    );
    messaging_admin_message_form_submit(array(), $form_values);
  }
}

/**
 * @} End of "ingroup messaging"
 */

/**
 * @ingroup workflow
 * @{
 */

/**
 * Implementation of hook_features_export_options().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_workflow_features_export_options()  {
  $workflows = workflow_get_all();
  return $workflows;
}

/**
 * Implementation of hook_features_export().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_workflow_features_export($data, &$export, $module_name = '') {
  foreach ($data as $wid) {
    $workflow = workflow_load($wid);
    $export['features']['cat_profile_workflow'][$wid] = $wid;
    $rs = db_query("SELECT type FROM {workflow_type_map} WHERE wid = %d", $wid);
    while ($type = db_result($rs)) {
      $pipe['variable'][] = 'worflow_' . $type;
    }
  }
  $export['dependencies'][] = 'workflow';
  if (isset($pipe)) {
    return $pipe;
  }
}

/**
 * Implementation of hook_features_export_render().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_workflow_features_export_render($module_name, $data) {
  module_load_include('inc', 'cat_profile');
  $data = cat_profile_export_table('workflows', array('wid' => $data));
  $code = ' $data = ' . features_var_export($data, '  ') . ';' . PHP_EOL;
  $code .= ' return $data;';
  return array('cat_profile_workflow_data' => $code);
}

/**
 * Implementation of hook_features_revert().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_workflow_features_revert($module) {
  $data = module_invoke($module, 'cat_profile_workflow_data');
  $wids = array();
  foreach ($data['workflows'] as $row) {
    $wids[] = $row['wid'];
  }
  cat_profile_data_revert($data, 'workflows', array('wid' => $wids));
}

/**
 * @} End of "ingroup workflow"
 */

/**
 * @ingroup homebox
 * @{
 */

/**
 * Implementation of hook_features_export_options().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_homebox_features_export_options()  {
  $rs = db_query('SELECT * FROM {homebox_pages}');
  while ($row = db_fetch_object($rs)) {
    $homeboxes[$row->pid] = $row->name;
  }
  return $homeboxes;
}

/**
 * Implementation of hook_features_export().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_homebox_features_export($data, &$export, $module_name = '') {
  foreach ($data as $pid) {
    $export['features']['cat_profile_homebox'][$pid] = $pid;
    $pipe['cat_profile_url_aliases'][] = 'homebox/' . $pid;
    $pipe['variable'][] = 'homebox_column_count_' . $pid;
    $pipe['variable'][] = 'homebox_users_use_colors_' . $pid;
    $pipe['variable'][] = 'homebox_color_' . $pid . '_0';
    $pipe['variable'][] = 'homebox_color_' . $pid . '_1';
    $pipe['variable'][] = 'homebox_color_' . $pid . '_2';
    $pipe['variable'][] = 'homebox_color_' . $pid . '_3';
    $pipe['variable'][] = 'homebox_color_' . $pid . '_4';
    $pipe['variable'][] = 'homebox_color_' . $pid . '_5';
  }
  $export['dependencies'][] = 'homebox';
  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_homebox_features_export_render($module_name, $data) {
  module_load_include('inc', 'cat_profile');
  $data = cat_profile_export_table('homebox_pages', array('pid' => $data));
  foreach ($data['homebox_pages'] as &$row) {
    $rs = db_query('SELECT d.region, d.movable, d.status, d.open, d.weight, b.module, b.delta, b.theme FROM {blocks} b INNER JOIN {homebox_default} d on b.bid = d.bid WHERE d.pid = %d ORDER by b.theme, b.module, b.delta', $row['pid']);
    while ($block = db_fetch_array($rs)) {
      $row['blocks'][] = $block;
    }
  }
  $code = ' $data = ' . features_var_export($data, '  ') . ';' . PHP_EOL;
  $code .= ' return $data;';
  return array('cat_profile_homebox' => $code);
}

/**
 * Implementation of hook_features_revert().
 * @author Josh Waihi <josh@catalyst.net.nz>
 */
function cat_profile_homebox_features_revert($module) {
  _block_rehash();
  $data = module_invoke($module, 'cat_profile_homebox');
  $pids = $block_data = $bids = array();
  foreach ($data['homebox_pages'] as &$row) {
    $pids[] = $row['pid'];
    foreach ($row['blocks'] as $block) {
      if ($bid = db_result(db_query("SELECT bid FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $block['module'], $block['delta'], $block['theme']))) {
        unset($block['module']);
        unset($block['delta']);
        unset($block['theme']);
        $block['bid'] = $bid;
        $block['pid'] = $row['pid'];
        $bids[] = $bid;
        $block_data['homebox_default'][] = $block;
      }
    }
    unset($row['blocks']);
  }
  cat_profile_data_revert($data, 'homebox_pages', array('pid' => $pids));
  db_query('DELETE FROM {homebox_default} WHERE pid IN (' . db_placeholders($pids) . ')', $pids);
  cat_profile_data_revert($block_data, 'homebox_default', array('bid' => $bids));
}

/**
 * @} End of "ingroup homebox"
 */

/**
 * implementation of hook_features_export_options()
 * @author Josh Schmidt <joshs@catalyst.net.nz>
 */
function cat_profile_ckeditor_features_export_options() {
  $results = db_query("SELECT name FROM {ckeditor_settings}");
  $settings = array();
  while ($row = db_fetch_object($results)) {
    $settings[$row->name] = $row->name;
  }
  return $settings;
}

/**
 * implementation of hook_features_export()
 * @author Josh Schmidt <joshs@catalyst.net.nz>
 */
function cat_profile_ckeditor_features_export($data, &$export, $module_name = '') {
  foreach ($data as $setting) {
    $export['features']['cat_profile_ckeditor'][$setting] = $setting;
  }
  $export['dependencies'][] = 'ckeditor';
}

/**
 * implementation of hook_features_export_render()
 * @author Josh Schmidt <joshs@catalyst.net.nz>
 */
function cat_profile_ckeditor_features_export_render($module_name, $data) {
  module_load_include('inc', 'cat_profile');
  $data = cat_profile_export_table('ckeditor_settings', array('name' => $data));
  foreach ($data['ckeditor_settings'] as &$value) {
    $value['settings'] = unserialize($value['settings']);
    //List the roles this setting is assigned to
    $result = db_query("SELECT r.name FROM {ckeditor_role} ckr INNER JOIN {role} r ON ckr.rid=r.rid WHERE ckr.name = '%s'", $value['name']);
    $roles = array();
    while ($row = db_fetch_object($result)) {
      $roles[] = $row->name;
    }
    if (!empty($roles)) {
      $value['roles'] = $roles;
    }
  }
  $code = ' $data = ' . features_var_export($data, '  ') . ';' . PHP_EOL;
  $code .= ' return $data;';
  return array('cat_profile_ckeditor' => $code);
}

/**
 * implementation of hook_features_revert()
 * @author Josh Schmidt <joshs@catalyst.net.nz>
 */
function cat_profile_ckeditor_features_revert($module) {
  $data = module_invoke($module, 'cat_profile_ckeditor');
  $names = array();
  $ck_roles = array();
  foreach ($data['ckeditor_settings'] as &$row) {
    if (isset($row['roles'])) {
      $ck_roles[$row['name']] = $row['roles'];
      unset($row['roles']);
    }
    $names[] = $row['name'];
    $row['settings'] = serialize($row['settings']);
  }
  //drupal_set_message('<pre>'.print_r($names, true).print_r($data, true).'</pre>');
  cat_profile_data_revert($data, 'ckeditor_settings', array('name' => $names));

  //Assigning roles to settings
  $result = db_query("SELECT rid,name FROM {role}");
  $drupal_roles = array();
  while ($row = db_fetch_object($result)) {
    $drupal_roles[strtolower($row->name)] = $row->rid;
  }

  foreach ($ck_roles as $setting=>$roles) {
    foreach ($roles as $role) {
      $role = strtolower($role);
      if (isset($drupal_roles[$role])) {
        $rid = $drupal_roles[$role];
        //First check to see if already assigned
        $assigned = db_result(db_query("SELECT COUNT(*) FROM {ckeditor_role} WHERE rid = %d AND name = '%s'", $rid, $setting));
        if (!$assigned) {
          db_query("INSERT INTO {ckeditor_role} VALUES('%s', %d)", $setting, $rid);
        }
      }
    }
  }
}

/**
 * @ingroup ckeditor
 * @{
 */


/**
 * @} End of "ingroup ckeditor"
 */
