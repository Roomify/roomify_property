<?php

/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 */
function roomify_property_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'property_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'bat_types';
  $view->human_name = 'Types by Property';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a class="button add-type-button ctools-use-modal ctools-modal-roomify-dashboard-modal-style" href="/admin/bat/config/type/wizard/!1/nojs">add types</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_property_bat_type_reference_roomify_property']['id'] = 'reverse_property_bat_type_reference_roomify_property';
  $handler->display->display_options['relationships']['reverse_property_bat_type_reference_roomify_property']['table'] = 'bat_types';
  $handler->display->display_options['relationships']['reverse_property_bat_type_reference_roomify_property']['field'] = 'reverse_property_bat_type_reference_roomify_property';
  $handler->display->display_options['relationships']['reverse_property_bat_type_reference_roomify_property']['required'] = TRUE;
  /* Field: Type: Type ID */
  $handler->display->display_options['fields']['type_id']['id'] = 'type_id';
  $handler->display->display_options['fields']['type_id']['table'] = 'bat_types';
  $handler->display->display_options['fields']['type_id']['field'] = 'type_id';
  $handler->display->display_options['fields']['type_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type_id']['separator'] = '';
  /* Field: Type: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'bat_types';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Bundle';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  /* Field: Type: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'bat_types';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Type: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'bat_types';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Published';
  $handler->display->display_options['fields']['status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['status']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'admin/bat/config/types/manage/[type_id]/edit?destination=admin/bat/config/property/manage/!1/types';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Units';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Units';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'admin/bat/config/types/manage/[type_id]/units?destination=admin/bat/config/property/manage/!1/types';
  /* Contextual filter: Roomify Property: Roomify property ID */
  $handler->display->display_options['arguments']['property_id']['id'] = 'property_id';
  $handler->display->display_options['arguments']['property_id']['table'] = 'roomify_properties';
  $handler->display->display_options['arguments']['property_id']['field'] = 'property_id';
  $handler->display->display_options['arguments']['property_id']['relationship'] = 'reverse_property_bat_type_reference_roomify_property';
  $handler->display->display_options['arguments']['property_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['property_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['property_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['property_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['property_id']['summary_options']['items_per_page'] = '25';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $views['property_types'] = $view;

  $view = new view();
  $view->name = 'property_revisions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'roomify_properties_revision';
  $view->human_name = 'Property revisions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Property revisions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'bypass roomify_property entities access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'revision_id' => 'revision_id',
    'revision_timestamp' => 'revision_timestamp',
    'name_1' => 'name_1',
    'name' => 'name',
    'log' => 'log',
  );
  $handler->display->display_options['style_options']['default'] = 'revision_id';
  $handler->display->display_options['style_options']['info'] = array(
    'revision_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'revision_timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'log' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Property revision: User */
  $handler->display->display_options['relationships']['revision_uid']['id'] = 'revision_uid';
  $handler->display->display_options['relationships']['revision_uid']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['relationships']['revision_uid']['field'] = 'revision_uid';
  /* Field: Property revision: Revision ID */
  $handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
  $handler->display->display_options['fields']['revision_id']['label'] = 'Revision';
  /* Field: Property revision: Revision date */
  $handler->display->display_options['fields']['revision_timestamp']['id'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['fields']['revision_timestamp']['field'] = 'revision_timestamp';
  $handler->display->display_options['fields']['revision_timestamp']['label'] = 'Created on';
  $handler->display->display_options['fields']['revision_timestamp']['date_format'] = 'bat';
  $handler->display->display_options['fields']['revision_timestamp']['second_date_format'] = 'bat_daily';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'revision_uid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Created by';
  /* Field: Property revision: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Property revision: Log message */
  $handler->display->display_options['fields']['log']['id'] = 'log';
  $handler->display->display_options['fields']['log']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['fields']['log']['field'] = 'log';
  /* Field: Property revision: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = '';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  /* Contextual filter: Property revision: Property ID */
  $handler->display->display_options['arguments']['property_id']['id'] = 'property_id';
  $handler->display->display_options['arguments']['property_id']['table'] = 'roomify_properties_revision';
  $handler->display->display_options['arguments']['property_id']['field'] = 'property_id';
  $handler->display->display_options['arguments']['property_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['property_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['property_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['property_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['property_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/bat/config/property/manage/%/revisions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Revisions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $views['property_revisions'] = $view;

  return $views;
}
