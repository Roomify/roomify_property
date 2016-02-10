<?php


/**
 * Implements hook_views_data_alter().
 */
function roomify_property_views_data_alter(&$data) {
  $data['roomify_properties']['edit_unit'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the property.'),
      'handler' => 'roomify_property_handler_edit_link_field',
    ),
  );
  $data['roomify_properties']['delete_unit'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the property.'),
      'handler' => 'roomify_property_handler_delete_link_field',
    ),
  );
}
