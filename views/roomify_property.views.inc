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


  $data['roomify_properties_revision']['table']['entity type'] = 'roomify_property';
  $data['roomify_properties_revision']['table']['group']  = t('Property revision');

  $data['roomify_properties_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Property revision'),
    'help' => t('Property revision is a history of changes to a property.'),
    'defaults' => array(
      'field' => 'name',
    ),
  );

  $data['roomify_properties_revision']['table']['join'] = array(
    'roomify_properties' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    )
  );

  $data['roomify_properties_revision']['table']['default_relationship'] = array(
    'roomify_properties' => array(
      'table' => 'roomify_properties',
      'field' => 'revision_id',
    ),
  );

  $data['roomify_properties_revision']['property_id'] = array(
    'title' => t('Property ID'),
    'help' => t('The unique internal identifier of the property.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['roomify_properties_revision']['revision_id'] = array(
    'title' => t('Revision ID'),
    'help' => t('The revision ID of the property revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'roomify_properties',
      'base field' => 'revision_id',
      'title' => t('Property'),
      'label' => t('Latest property revision'),
    ),
  );

  $data['roomify_properties_revision']['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the property revision used for administrative display.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['roomify_properties_revision']['revision_uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a property revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'revision_uid',
      'field_name' => 'revision_uid',
      'label' => t('Revision user'),
    ),
  );

  $data['roomify_properties_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['roomify_properties_revision']['revision_timestamp'] = array(
    'title' => t('Revision date'),
    'help' => t('The date the property revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['roomify_properties_revision']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this property revision.'),
      'handler' => 'roomify_property_handler_revision_operations_field',
    ),
  );
}
