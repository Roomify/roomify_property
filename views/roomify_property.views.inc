<?php

/**
 * @file
 */

/**
 * Implements hook_views_data_alter().
 */
function roomify_property_views_data_alter(&$data) {

  // Expose the created and changed timestamps.
  $data['roomify_properties']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the property was created.'),
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

  $data['roomify_properties']['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['roomify_properties']['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['roomify_properties']['created_timestamp_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['roomify_properties']['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['roomify_properties']['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['roomify_properties']['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $data['roomify_properties']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the property was last updated.'),
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

  $data['roomify_properties']['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $data['roomify_properties']['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $data['roomify_properties']['changed_timestamp_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $data['roomify_properties']['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $data['roomify_properties']['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $data['roomify_properties']['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

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
  $data['roomify_properties_revision']['table']['group'] = t('Property revision');

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
    ),
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
