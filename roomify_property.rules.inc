<?php

/**
 * @file
 */

/**
 * Implements hook_rules_event_info().
 */
function roomify_property_rules_event_info() {
  $default = array(
    'group' => t('Property Revisioning'),
    'variables' => _roomify_property_rules_event_variables(),
  );
  $events = array(
    'roomify_property_revisioning_post_update' => $default + array(
      'label' => t('Property revision has been updated'),
    ),
    'roomify_property_revisioning_pre_publish' => $default + array(
      'label' => t('Property revision is going to be published'),
    ),
    'roomify_property_revisioning_post_publish' => $default + array(
      'label' => t('Property revision has been published'),
    ),
    'roomify_property_revisioning_pre_revert' => $default + array(
      'label' => t('Property is going to be reverted to revision'),
    ),
    'roomify_property_revisioning_post_revert' => $default + array(
      'label' => t('Property has been reverted to revision'),
    ),
    'roomify_property_revisioning_post_unpublish' => $default + array(
      'label' => t('Property has been unpublished'),
    ),
    'roomify_property_revisioning_pre_delete' => $default + array(
      'label' => t('Property revision is going to be deleted'),
    ),
    'roomify_property_revisioning_post_delete' => $default + array(
      'label' => t('Property revision has been deleted'),
    ),
  );
  return $events;
}

/**
 * Returns event variables suitable for use with a type revision event.
 */
function _roomify_property_rules_event_variables() {
  $vars = array(
    'user' => array(
      'type' => 'user',
      'label' => t('acting user'),
      'description' => t('The acting user.'),
      'handler' => 'roomify_property_revisioning_events_argument_global_user',
    ),
    'roomify_property' => array(
      'type' => 'roomify_property',
      'label' => t('target propertu.'),
    ),
    'revision' => array(
      'type' => 'roomify_property',
      'label' => t('current revision of target content.'),
      'description' => t('The current content revision'),
      'handler' => 'roomify_property_revisioning_events_argument_current_revision',
    ),
  );
  return $vars;
}

/**
 * Get global user argument.
 */
function roomify_property_revisioning_events_argument_global_user($arguments, $name, $info) {
  global $user;
  return user_load($user->uid);
}

/**
 * Evaluate revision argument.
 */
function roomify_property_revisioning_events_argument_current_revision($arguments, $name, $info) {
  if (empty($arguments['roomify_property'])) {
    drupal_set_message(t('Revisioning: could not evaluate rule condition -- type variable missing.'), 'warning');
    return FALSE;
  }
  $property = $arguments['roomify_property'];
  $current_vid = $property->current_revision_id;
  if ($property->revision_id != $current_vid) {
    $current = roomify_property_load($property->property_id, $current_vid);
    return $current;
  }
  return $property;
}

/**
 * Implements hook_rules_action_info().
 */
function roomify_property_rules_action_info() {
  $default = array(
    'group' => t('Property Revisioning'),
  );
  return array(
    'roomify_property_revisioning_rules_action_publish_latest' => $default + array(
      'label' => t('Publish the most recent pending revision'),
      'parameter' => array(
        'roomify_property' => array('type' => 'roomify_property', 'label' => t('type')),
      ),
    ),
    'roomify_property_revisioning_rules_action_load_current' => $default + array(
      'label' => t('Load current revision of type'),
      'parameter' => array(
        'roomify_property' => array('type' => 'roomify_property', 'label' => t('type')),
      ),
      'new variables' => array(
        'loaded_current_revision' => array(
          'type' => 'roomify_property',
          'label' => t('Loaded current revision of property'),
          'save' => FALSE,
          'label callback' => 'revisioning_rules_loaded_current_label',
        ),
      ),
    ),
  );
}

/**
 * Action: load current revision of provided property.
 */
function roomify_property_revisioning_rules_action_load_current($property) {
  $current_vid = $property->current_revision_id;
  if ($property->revision_id != $current_vid) {
    $current = roomify_property_load_revision($current_vid);
    return array('loaded_current_revision' => $current);
  }
  return array('loaded_current_revision' => $property);
}

/**
 * Action: publish most recent pending revision.
 */
function roomify_property_revisioning_rules_action_publish_latest($property) {
  // Get the latest pending revision.
  $pending_revisions = roomify_property_get_pending_revisions($property->property_id);
  $latest_pending = array_shift($pending_revisions);
  if ($latest_pending) {
    $property_revision = roomify_property_load_revision($latest_pending->revision_id);

    roomify_property_revisioning_event('pre publish', $property_revision);

    $property_revision->default_revision = TRUE;
    $property_revision->save();

    roomify_property_revisioning_event('post publish', $property_revision);

    return TRUE;
  }
  // If there is no pending revision, take the current revision, provided it is
  // NOT published.
  if (!$property->status) {
    if (!isset($property->is_current)) {
      $property->current_revision_id = roomify_property_get_current_property_revision_id($property->property_id);
      $property->is_current = revisioning_revision_is_current($property);
    }
    if ($property->is_current) {
      roomify_property_revisioning_event('pre publish', $property_revision);

      $property_revision->default_revision = TRUE;
      $property_revision->save();

      roomify_property_revisioning_event('post publish', $property_revision);

      return TRUE;
    }
  }
  return FALSE;
}
