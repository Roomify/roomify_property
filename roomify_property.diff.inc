<?php

/**
 * @file
 */

/**
 * Create a comparison for the property between versions 'old_revision_id' and 'new_revision_id'.
 *
 * @param object $property
 *   RoomifyProperty on which to perform comparison
 * @param int $old_revision_id
 *   Version ID of the old revision.
 * @param int $new_revision_id
 *   Version ID of the new revision.
 */
function roomify_property_compare_revisions($property, $old_revision_id, $new_revision_id) {
  module_load_include('inc', 'diff', 'diff.pages');

  // Attaches the CSS.
  $build['#attached'] = diff_build_attachments();

  $state = 'raw_plain';

  $property_revisions = roomify_property_revision_list($property);

  $old_property = roomify_property_load_revision($old_revision_id);
  $new_property = roomify_property_load_revision($new_revision_id);

  $old_account = user_load($property_revisions[$old_revision_id]->revision_uid);
  $new_account = user_load($property_revisions[$new_revision_id]->revision_uid);

  // Generate table header (date, username, log message).
  $old_header = t('!date by !username', array(
    '!date' => l(format_date($old_property->revision_timestamp), "admin/bat/config/property/manage/$property->property_id/revision/$old_property->revision_id/view", array('absolute' => 1)),
    '!username' => theme('username', array('account' => $old_account)),
  ));
  $new_header = t('!date by !username', array(
    '!date' => l(format_date($new_property->revision_timestamp), "admin/bat/config/property/manage/$property->property_id/revision/$new_property->revision_id/view", array('absolute' => 1)),
    '!username' => theme('username', array('account' => $new_account)),
  ));

  $old_log = $old_property->log != '' ? '<p class="revision-log">' . filter_xss($old_property->log) . '</p>' : '';
  $new_log = $new_property->log != '' ? '<p class="revision-log">' . filter_xss($new_property->log) . '</p>' : '';

  // Generate previous diff/next diff links.
  $next_vid = roomify_property_diff_get_next_vid($property_revisions, $new_revision_id);
  if ($next_vid) {
    $next_link = l(t('Next difference >'), 'admin/bat/config/property/manage/' . $property->property_id . '/revisions/view/' . $new_revision_id . '/' . $next_vid, array('absolute' => 1));
  }
  else {
    $next_link = '';
  }
  $prev_vid = roomify_property_diff_get_previous_vid($property_revisions, $old_revision_id);
  if ($prev_vid) {
    $prev_link = l(t('< Previous difference'), 'admin/bat/config/property/manage/' . $property->property_id . '/revisions/view/' . $prev_vid . '/' . $old_revision_id, array('absolute' => 1));
  }
  else {
    $prev_link = '';
  }

  $header = _diff_default_header($old_header, $new_header);
  $rows = array();
  if ($old_log || $new_log) {
    $rows['logs'] = array(
      array(
        'data' => $old_log,
        'colspan' => 2,
      ),
      array(
        'data' => $new_log,
        'colspan' => 2,
      ),
    );
  }
  $rows['navigation'] = array(
    array(
      'data' => $prev_link,
      'class' => array('diff-prevlink'),
      'colspan' => 2,
    ),
    array(
      'data' => $next_link,
      'class' => array('diff-nextlink'),
      'colspan' => 2,
    ),
  );

  $rows = array_merge($rows, roomify_property_diff_body_rows($old_property, $new_property, $state));

  $build['diff_table'] = array(
    '#theme' => 'table__diff__standard',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('diff')),
    '#colgroups' => _diff_default_cols(),
    '#sticky' => FALSE,
  );

  return $build;
}

/**
 * Get the entry in the revisions list after $vid.
 *
 * @param array $property_revisions
 *   Array of property revision IDs in descending order.
 * @param int $vid
 *   Version ID to look for.
 *
 * @return bool|int
 *   Returns FALSE if $vid is the last entry.
 */
function roomify_property_diff_get_next_vid($property_revisions, $vid) {
  $previous = NULL;
  foreach ($property_revisions as $revision) {
    if ($revision->revision_id == $vid) {
      return ($previous ? $previous->revision_id : FALSE);
    }
    $previous = $revision;
  }
  return FALSE;
}

/**
 * Get the entry in the revision list before $vid.
 *
 * @param array $property_revisions
 *   Array of property revision IDs in descending order.
 * @param int $vid
 *   Version ID to look for.
 *
 * @return bool|int
 *   Returns FALSE if $vid is the first entry.
 */
function roomify_property_diff_get_previous_vid($property_revisions, $vid) {
  $previous = NULL;
  foreach ($property_revisions as $revision) {
    if ($previous && $previous->revision_id == $vid) {
      return $revision->revision_id;
    }
    $previous = $revision;
  }
  return FALSE;
}

/**
 * Creates an array of rows which represent the difference between properties.
 *
 * @param object $old_property
 *   RoomifyProperty for comparison which will be displayed on the left side.
 * @param object $new_property
 *   RoomifyProperty for comparison which will be displayed on the right side.
 * @param bool $state
 *   The state to render for the diff.
 */
function roomify_property_diff_body_rows($old_property, $new_property, $state = 'raw') {
  $context = array(
    'states' => array($state),
    'view_mode' => 'diff_standard',
  );
  return diff_entity_body_rows('roomify_property', $old_property, $new_property, $context);
}
