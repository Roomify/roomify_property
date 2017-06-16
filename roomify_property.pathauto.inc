<?php

/**
 * @file
 * Hook implementations for the Pathauto module.
 */

/**
 * Implements hook_path_alias_types().
 *
 * Here we return what the original path for a property is so Pathauto can delete
 * them in one go.
 */
function roomify_property_path_alias_types() {
  $alias_types['listing/'] = t('Roomify Property');
  return $alias_types;
}

/**
 * Implements hook_pathauto().
 */
function roomify_property_pathauto($op) {
  if ($op != 'settings') {
    return;
  }

  if (!module_exists('entity_token')) {
    return;
  }

  $settings = array(
    'module' => 'roomify_property',
    'token_type' => 'roomify_property',
    'groupheader' => t('Roomify Property paths'),
    'patterndescr' => t('Default path pattern (applies to all property types with blank patterns below)'),
    'patterndefault' => 'listing/[roomify_property:name]',
    'batch_update_callback' => 'roomify_property_pathauto_bulk_update_batch_process',
    'batch_file' => drupal_get_path('module', 'roomify_property') . '/roomify_property.pathauto.inc',
  );

  $languages = array();
  if (module_exists('locale')) {
    $languages = array(LANGUAGE_NONE => t('language neutral')) + locale_language_list('name');
  }

  foreach (roomify_property_type_labels() as $name => $label) {
    if (count($languages)) {
      $settings['patternitems'][$name] = t('Default path pattern for @property_type (applies to all @property_type groups with blank patterns below)', array('@property_type' => $label));

      foreach ($languages as $lang_code => $lang_name) {
        $settings['patternitems'][$name . '_' . $lang_code] = t('Pattern for all @language @property_type paths', array('@property_type' => $label, '@language' => $lang_name));
      }
    }
    else {
      $settings['patternitems'][$name] = t('Pattern for all @property_type paths', array('@property_type' => $label));
    }
  }

  return (object) $settings;
}

/**
 * Batch processing callback; Generate aliases for properties.
 */
function roomify_property_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('groups', 'g');
  $query->leftJoin('url_alias', 'ua', "CONCAT('group/', g.gid) = ua.source");
  $query->addField('g', 'gid');
  $query->isNull('ua.source');
  $query->condition('g.gid', $context['sandbox']['current'], '>');
  $query->orderBy('g.gid');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'roomify_property');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no properties to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $gids = $query->execute()->fetchCol();

  roomify_property_pathauto_update_alias_multiple($gids, 'bulkupdate');
  $context['sandbox']['count'] += count($gids);
  $context['sandbox']['current'] = max($gids);
  $context['message'] = t('Updated alias for property @pid.', array('@pid' => end($pids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}

/**
 * Update the URL aliases for multiple properties.
 *
 * @param array $pids
 *   An array of property IDs.
 * @param string $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param array $options
 *   (optional) An array of additional options.
 */
function roomify_property_pathauto_update_alias_multiple(array $pids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $properties = roomify_property_load_multiple($pids);
  foreach ($properties as $property) {
    roomify_property_pathauto_update_alias($property, $op);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($pids), 'Updated URL alias for 1 property.', 'Updated URL aliases for @count properties.'));
  }
}

/**
 * Creates or updates entity alias.
 *
 * @param RoomifyProperty $property
 *   The propert the alias is being created/updated for.
 * @param string $op
 *   Operation being performed on the alias ('insert', 'update' or 'bulkupdate').
 * @param array $language
 *   Additional parameter to set the language of the path.
 *
 * @return string
 *   The alias that was created.
 */
function roomify_property_pathauto_update_alias(RoomifyProperty $property, $op, $language = NULL) {
  // Skip processing if the user has disabled pathauto for the property.
  if (isset($property->path['pathauto']) && empty($property->path['pathauto'])) {
    return;
  }

  // Make sure the language is set.
  if (!isset($language)) {
    $language = entity_language('roomify_property', $property);

    // To preserve some backwards compatibility, entity_language() returns NULL
    // instead of LANGUAGE_NONE when no language is found. Pathauto expects
    // LANGUAGE_NONE, though, so we convert it.
    if (!isset($language)) {
      $language = LANGUAGE_NONE;
    }
  }

  // Skip processing if the entity has no pattern.
  if (!pathauto_pattern_load_by_entity('roomify_property', $property->type, $language)) {
    return;
  }

  module_load_include('inc', 'pathauto');
  $uri = entity_uri('roomify_property', $property);
  return pathauto_create_alias('roomify_property', $op, $uri['path'], array('roomify_property' => $property), $property->type, $language);
}
