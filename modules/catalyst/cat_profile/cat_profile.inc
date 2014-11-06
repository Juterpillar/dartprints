<?php

/**
 * Helper functions for update hooks
 */

/**
 * Helper function remove a given field from an instance.
 *
 * @param string $entity_type
 *   The entity type name
 * @param string $bundle_name
 *   The bundle name
 * @param string $field_name
 *   The name of the field to remove
 *
 * @return boolean
 *   False upon failure
 */
function _cat_profile_delete_field($entity_type, $bundle_name, $field_name) {
  // Check whether the field exists at all.
  if ($field = field_info_field($field_name)) {
    if ($instance = field_info_instance($entity_type, $field_name, $bundle_name)) {
      field_delete_instance($instance);
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Helper function for enabling modules.
 *
 * @param text $modulenames
 *   The name of the module, either an array of text or just text.
 */
function _cat_profile_enable_module($modulenames) {
  $modules = is_array($modulenames) ? $modulenames : array($modulenames);
  $success = module_enable($modules);
  if ($success == FALSE) {
    throw new DrupalUpdateException(implode(',', $modules) . ' module could not be enabled.');
  }
}

/**
 * Helper function for disabling modules.
 *
 * @param text $modulename
 *   The name of the module. Either an array of text or just text
 */
function _cat_profile_disable_module($module) {
  $modules = is_array($modulename) ? $modulename : array($modulename);
  if (module_exists($module)) {
    module_disable(array($module));
    if (module_exists($module)) {
      throw new DrupalUpdateException("{$module} module could not be disabled");
    }
    if (!drupal_uninstall_modules(array($module))) {
      throw new DrupalUpdateException("{$module} module could not be uninstalled");
    }
  }
}

/**
 * Helper function for deleting vocabularies.
 *
 * @param text $vocabulary_name
 *   The name of the vocabulary to delete
 * @param boolean $surpress
 *   If true, the error will be surpressed
 */
function _cat_profile_delete_vocabulary($vocabulary_name, $surpress = FALSE) {
  $vocabs = taxonomy_vocabulary_get_names();

  if (!array_key_exists($vocabulary_name, $vocabs)) {
    if ($surpress) {
      // Don't show the error.
      return;
    }
    throw new DrupalUpdateException('Unable to find vocabulary: ' . $vocabulary_name);
  }

  // Find the vocab and delete it.
  $vocab = $vocabs[$vocabulary_name];
  taxonomy_vocabulary_delete($vocab->vid);
}
