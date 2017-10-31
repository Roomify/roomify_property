<?php

/**
 * @file
 * RoomifyProperty editing UI.
 */

/**
 * UI controller.
 */
class RoomifyPropertyUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path]['title'] = 'Roomify Properties';

    // Change the add page menu to multiple types of entities.
    $items[$this->path . '/add']['title'] = 'Add a Property';
    $items[$this->path . '/add']['description'] = 'Create a new property.';
    $items[$this->path . '/add']['page callback'] = 'roomify_property_add_page';
    $items[$this->path . '/add']['access callback'] = 'roomify_property_add_access';
    unset($items[$this->path . '/add']['title callback']);

    if (module_exists('diff')) {
      $items[$this->path . '/manage/' . $wildcard . '/revisions/view'] = array(
        'title' => '',
        'page callback' => 'roomify_property_compare_revisions',
        'page arguments' => array($id_count + 1, $id_count + 4, $id_count + 5),
        'access callback' => TRUE,
        'type' => MENU_LOCAL_TASK,
        'file' => 'roomify_property.diff.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module']),
      );
    }

    $items[$this->path . '/manage/' . $wildcard . '/revision/%/view'] = array(
      'title' => 'View Revision',
      'page callback' => 'roomify_property_revision_view',
      'page arguments' => array($id_count + 1, $id_count + 3),
      'access callback' => 'roomify_property_access_revision',
      'access arguments' => array('view', $id_count + 1, $id_count + 3),
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path . '/manage/' . $wildcard . '/revision/%/revert'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array('roomify_property_revision_revert_confirm', $id_count + 1, $id_count + 3),
      'access callback' => 'roomify_property_access_revision',
      'access arguments' => array('revert', $id_count + 1, $id_count + 3),
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    $items[$this->path . '/manage/' . $wildcard . '/revision/%/delete'] = array(
      'page callback' => 'drupal_get_form',
      'page arguments' => array('roomify_property_revision_delete_confirm', $id_count + 1, $id_count + 3),
      'access callback' => 'roomify_property_access_revision',
      'access arguments' => array('delete', $id_count + 1, $id_count + 3),
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    // Add property type specific types menu item.
    $items[$this->path . '/manage/' . $wildcard . '/types'] = array(
      'title callback' => 'roomify_property_types_title_callback',
      'title arguments' => array(5),
      'page callback' => 'roomify_property_types_wrapper',
      'page arguments' => array(5),
      'access callback' => 'roomify_property_manage_types_access',
      'access arguments' => array(5),
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
      'type' => MENU_LOCAL_TASK,
    );

    // Add property type specific calendar menu item.
    $items[$this->path . '/manage/' . $wildcard . '/calendar/availability'] = array(
      'title' => 'Manage availability',
      'page callback' => 'roomify_property_calendar_wrapper',
      'page arguments' => array(5, 'availability'),
      'access callback' => 'roomify_property_access',
      'access arguments' => array('update', 5),
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
      'type' => MENU_LOCAL_TASK,
    );

    // Add property type specific calendar menu item.
    $items[$this->path . '/manage/' . $wildcard . '/calendar/pricing'] = array(
      'title' => 'Manage pricing',
      'page callback' => 'roomify_property_calendar_wrapper',
      'page arguments' => array(5, 'pricing'),
      'access callback' => 'roomify_property_access',
      'access arguments' => array('update', 5),
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
      'type' => MENU_LOCAL_TASK,
    );

    // Add menu items to add each different type of properties.
    foreach (roomify_property_get_types() as $property_type) {
      $items[$this->path . '/add/' . $property_type->type] = array(
        'title' => 'Add @property_type_label property',
        'title arguments' => array('@property_type_label' => $property_type->label),
        'page callback' => 'roomify_property_create_form_wrapper',
        'page arguments' => array($property_type->type),
        'access callback' => 'roomify_property_access',
        'access arguments' => array('create', roomify_property_create(array('type' => $property_type->type, 'uid' => 0))),
        'file' => 'roomify_property.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module']),
      );
    }

    // Menu item for viewing property.
    $items['property/' . $wildcard] = array(
      'title callback' => 'roomify_property_page_title',
      'title arguments' => array(1),
      'page callback' => 'roomify_property_page_view',
      'page arguments' => array(1),
      'access callback' => 'roomify_property_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );

    return $items;
  }

  /**
   * Overrides overviewTableHeaders() defaults.
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = array(
      t('ID'),
      t('Name'),
      t('Bundle'),
    );
    if (!empty($this->entityInfo['exportable'])) {
      $header[] = t('Status');
    }
    // Add operations with the right colspan.
    $header[] = array('data' => t('Operations'), 'colspan' => $this->operationCount());
    return $header;
  }

  /**
   * Overrides overviewTableHeaders() defaults.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $entity_uri = entity_uri($this->entityType, $entity);

    $row[] = $id;

    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType,
      ),
    );

    $property_type = roomify_property_type_load($entity->type);
    $row[] = $property_type->label;

    // Add a row for the exportable status.
    if (!empty($this->entityInfo['exportable'])) {
      $row[] = array(
        'data' => array(
          '#theme' => 'entity_status',
          '#status' => $entity->{$this->statusKey},
        ),
      );
    }
    // In case this is a bundle, we add links to the field ui tabs.
    $field_ui = !empty($this->entityInfo['bundle of']) && entity_type_is_fieldable($this->entityInfo['bundle of']) && module_exists('field_ui');
    // For exportable entities we add an export link.
    $exportable = !empty($this->entityInfo['exportable']);
    // If i18n integration is enabled, add a link to the translate tab.
    $i18n = !empty($this->entityInfo['i18n controller class']);

    // Add operations depending on the status.
    if (entity_has_status($this->entityType, $entity, ENTITY_FIXED)) {
      $row[] = array('data' => l(t('clone'), $this->path . '/manage/' . $id . '/clone'), 'colspan' => $this->operationCount());
    }
    else {
      $row[] = l(t('edit'), $this->path . '/manage/' . $id);

      if ($field_ui) {
        $row[] = l(t('manage fields'), $this->path . '/manage/' . $id . '/fields');
        $row[] = l(t('manage display'), $this->path . '/manage/' . $id . '/display');
      }
      if ($i18n) {
        $row[] = l(t('translate'), $this->path . '/manage/' . $id . '/translate');
      }
      if ($exportable) {
        $row[] = l(t('clone'), $this->path . '/manage/' . $id . '/clone');
      }

      if (empty($this->entityInfo['exportable']) || !entity_has_status($this->entityType, $entity, ENTITY_IN_CODE)) {
        $row[] = l(t('delete'), $this->path . '/manage/' . $id . '/delete', array('query' => drupal_get_destination()));
      }
      elseif (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
        $row[] = l(t('revert'), $this->path . '/manage/' . $id . '/revert', array('query' => drupal_get_destination()));
      }
      else {
        $row[] = '';
      }
    }
    if ($exportable) {
      $row[] = l(t('export'), $this->path . '/manage/' . $id . '/export');
    }
    return $row;
  }

  /**
   * Creates the markup for the add Property Entities page within the class
   * so it can easily be extended/overridden.
   */
  public function addPage() {
    $item = menu_get_item();
    $property_types = roomify_property_get_types();

    if (count($property_types) == 1) {
      $property_type = reset($property_types);
      drupal_goto($this->path . '/add/' . $property_type->type);
    }

    $items = array();
    foreach ($property_types as $property_type) {
      $items[] = array(
        'title' => t('Add @property_type_label property', array('@property_type_label' => $property_type->label)),
        'href' => $this->path . '/add/' . $property_type->type,
        'description' => '',
      );
    }

    return array(
      '#theme' => 'roomify_property_add_list',
      '#content' => $items,
    );
  }

}

/**
 *
 */
function roomify_property_form($form, &$form_state, $property, $op = 'edit') {
  global $user;
  // Add the breadcrumb for the form's location.
  roomify_property_set_breadcrumb();
  drupal_set_title(t('Edit !property_name', array('!property_name' => $property->name)));

  $property->date = format_date($property->created, 'custom', 'Y-m-d H:i:s O');
  $account = user_load($property->uid);
  $property->author_name = isset($account->name) ? $account->name : '';

  return roomify_property_edit_form($form, $form_state, $property);
}

/**
 * Show output from property type-specific calendar pages under the property admin.
 */
function roomify_property_calendar_wrapper($property, $event_type) {
  $property_type = str_replace('_property', '', $property->type);
  $function = 'roomify_' . $property_type . '_event_ui_calendar_page';

  return(call_user_func($function, $property->property_id, $event_type));
}

/**
 * Show types view on property tab.
 */
function roomify_property_types_wrapper($property) {
  return(views_embed_view('property_types', 'block_1', $property->property_id));
}

/**
 * Form callback wrapper: create a Property.
 *
 * @param $type
 *   The Property type for the property to be created.
 */
function roomify_property_create_form_wrapper($type) {
  global $user;
  // Add the breadcrumb for the form's location.
  roomify_property_set_breadcrumb();

  $property = roomify_property_create(array('type' => $type, 'uid' => $user->uid));
  $property->created = REQUEST_TIME;
  $property->author_name = $user->name;
  $property->status = 1;

  return drupal_get_form('roomify_property_edit_form', $property);
}

/**
 * Geneproperties the property editing form.
 */
function roomify_property_edit_form($form, &$form_state, $property) {
  global $user;

  if ($property->status == 0) {
    $form['unpublished'] = array(
      '#markup' => '<div style="color: #FF4500; font-size: 20px; float: right;">' . t('Property suspended') . '</div>',
      '#weight' => -100,
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($property->name) ? $property->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -99,
  );

  // Add the field related form elements.
  $form_state['roomify_property'] = $property;

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  if (module_exists('path')) {
    $path = array();

    $uri = $property->uri();

    if (!empty($property->property_id)) {
      $conditions = array('source' => $uri['path']);
      $langcode = entity_language('roomify_property', $property);

      if ($langcode != LANGUAGE_NONE) {
        $conditions['language'] = $langcode;
      }

      $path = path_load($conditions);
      if ($path === FALSE) {
        $path = array();
      }
    }

    $path += array(
      'pid' => NULL,
      'source' => isset($property->property_id) ? $uri['path'] : NULL,
      'alias' => '',
      'language' => isset($langcode) ? $langcode : LANGUAGE_NONE,
    );

    $form['path'] = array(
      '#type' => 'fieldset',
      '#title' => t('URL path settings'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($path['alias']),
      '#group' => 'additional_settings',
      '#attributes' => array(
        'class' => array('path-form'),
      ),
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'path') . '/path.js'),
      ),
      '#access' => user_access('create url aliases') || user_access('administer url aliases'),
      '#weight' => 30,
      '#tree' => TRUE,
      '#element_validate' => array('path_form_element_validate'),
    );

    $form['path']['alias'] = array(
      '#type' => 'textfield',
      '#title' => t('URL alias'),
      '#default_value' => $path['alias'],
      '#maxlength' => 255,
      '#description' => t('Optionally specify an alternative URL by which this property can be accessed. Use a relative path and don\'t add a trailing slash or the URL alias won\'t work.'),
    );

    // Add in the fixed values.
    foreach (array('pid', 'source', 'language') as $fixed) {
      $form['path'][$fixed] = array(
        '#type' => 'value',
        '#value' => $path[$fixed],
      );
    }
  }

  field_attach_form('roomify_property', $property, $form, $form_state, entity_language('roomify_property', $property));

  // Hide type reference field in favor of 'Rooms' tab.
  $form['property_bat_type_reference']['#access'] = FALSE;

  // Type author information for administrators.
  $form['author'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('bypass roomify_property entities access'),
    '#title' => t('Authoring information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('type-form-author'),
    ),
    '#attached' => array(
      'js' => array(
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 90,
  );

  $form['type'] = array(
    '#type' => 'value',
    '#value' => $property->type,
  );

  $form['author']['author_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => !empty($property->author_name) ? $property->author_name : '',
    '#weight' => -1,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );
  $form['author']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored on'),
    '#maxlength' => 25,
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array('%time' => !empty($property->date) ? date_format(date_create($property->date), 'Y-m-d H:i:s O') : format_date($property->created, 'custom', 'Y-m-d H:i:s O'), '%timezone' => !empty($property->date) ? date_format(date_create($property->date), 'O') : format_date($property->created, 'custom', 'O'))),
    '#default_value' => !empty($property->date) ? $property->date : '',
  );

  $form['revisions'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('bypass roomify_property entities access'),
    '#title' => t('Revision information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#attributes' => array(
      'class' => array('type-form-revisions'),
    ),
    '#weight' => 95,
  );

  if (module_exists('revisioning')) {
    $form['revisions']['log'] = array(
      '#type' => 'textarea',
      '#title' => !empty($property->property_id) ? t('Update log message') : t('Creation log message'),
      '#rows' => 4,
      '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful history of changes to this property.'),
    );

    $options = array();
    if (!empty($property->property_id)) {
      $options[REVISIONING_NO_REVISION] = t('Modify current revision, no moderation');
    }
    $options[REVISIONING_NEW_REVISION_NO_MODERATION] = t('Create new revision, no moderation');
    $options[REVISIONING_NEW_REVISION_WITH_MODERATION] = t('Create new revision and moderate');

    $form['revisions']['revision_operation'] = array(
      '#title' => t('Revision creation and moderation options'),
      '#description' => t('Moderation means that the new revision is not publicly visible until approved by someone with the appropriate permissions.'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => isset($property->property_id)
        ? REVISIONING_NEW_REVISION_WITH_MODERATION
        : REVISIONING_NEW_REVISION_NO_MODERATION,
    );

    if (variable_get('revisioning_no_moderation_by_default', FALSE)) {
      $form['revisions']['revision_operation']['#default_value'] = REVISIONING_NEW_REVISION_NO_MODERATION;
    }

    if (!empty($property->property_id)) {
      $revision_count = db_query("SELECT COUNT(revision_id) FROM {roomify_properties_revision} WHERE property_id = :property_id AND revision_id > :revision_id",
        array(
          ':revision_id' => $property->revision_id,
          ':property_id' => $property->property_id,
        )
      )->fetchField();

      if ($revision_count == 1) {
        drupal_set_message(t('Please note there is one revision more recent than the one you are about to edit.'), 'warning');
      }
      elseif ($revision_count > 1) {
        drupal_set_message(t('Please note there are @count revisions more recent than the one you are about to edit.',
          array('@count' => $revision_count)), 'warning');
      }
    }
  }
  else {
    if (!empty($property->property_id)) {
      $property_type = roomify_property_type_load($property->type);

      $form['revisions']['revision'] = array(
        '#type' => 'checkbox',
        '#title' => t('Create new revision on update'),
        '#description' => t('If an update log message is entered, a revision will be created even if this is unchecked.'),
        '#default_value' => (isset($property_type->data['revision'])) ? $property_type->data['revision'] : 0,
      );
    }
    $form['revisions']['log'] = array(
      '#type' => 'textarea',
      '#title' => !empty($property->property_id) ? t('Update log message') : t('Creation log message'),
      '#rows' => 4,
      '#description' => t('Provide an explanation of the changes you are making. This will provide a meaningful history of changes to this property.'),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
  );
  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Property'),
    '#submit' => $submit + array('roomify_property_edit_form_submit'),
  );

  if (!(isset($property->is_new) && $property->is_new)) {
    if (user_access('unpublish any property') || (user_access('unpublish own property') && $property->uid == $user->uid)) {
      if ($property->status) {
        $form['actions']['suspend'] = array(
          '#type' => 'submit',
          '#value' => t('Suspend Property'),
          '#suffix' => l(t('Cancel'), 'admin/bat/config/property'),
          '#submit' => $submit + array('roomify_property_form_submit_suspend'),
        );
      }
      else {
        $form['actions']['activate'] = array(
          '#type' => 'submit',
          '#value' => t('Activate Property'),
          '#suffix' => l(t('Cancel'), 'admin/bat/config/property'),
          '#submit' => $submit + array('roomify_property_form_submit_activate'),
        );
      }
    }
    else {
      $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/bat/config/property');
    }
  }

  if (!empty($property->name) && roomify_property_access('delete', $property)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Property'),
      '#submit' => $submit + array('roomify_property_form_submit_delete'),
      '#attributes' => array('style' => 'float: right;'),
    );
  }

  $form['#validate'][] = 'roomify_property_edit_form_validate';

  return $form;
}

/**
 * Form API validate callback for the event form.
 */
function roomify_property_edit_form_validate(&$form, &$form_state) {
  // Notify field widgets to validate their data.
  entity_form_field_validate('roomify_property', $form, $form_state);
}

/**
 * Form API submit callback for the property form.
 */
function roomify_property_edit_form_submit($form, &$form_state) {
  $property = entity_ui_controller('roomify_property')->entityFormSubmitBuildEntity($form, $form_state);

  $property->created = !empty($property->date) ? strtotime($property->date) : REQUEST_TIME;
  $property->changed = time();

  if (module_exists('revisioning')) {
    if (isset($property->revision_operation)) {
      $property->revision = ($property->revision_operation > REVISIONING_NO_REVISION);
      if ($property->revision_operation == REVISIONING_NEW_REVISION_WITH_MODERATION) {
        $property->default_revision = FALSE;
      }
    }
  }
  else {
    // Trigger a new revision if the checkbox was enabled or a log message supplied.
    if (!empty($form_state['values']['revision']) ||
        !empty($form['change_history']['revision']['#default_value']) ||
        !empty($form_state['values']['log'])) {
      $property->revision = TRUE;
      $property->log = $form_state['values']['log'];
    }
  }

  if (isset($property->author_name)) {
    if ($account = user_load_by_name($property->author_name)) {
      $property->uid = $account->uid;
    }
    else {
      $property->uid = 0;
    }
  }

  $property->save();
  drupal_set_message(t('Property @name saved', array('@name' => $property->name)));

  $form_state['redirect'] = 'admin/bat/config/property';
}

/**
 * Form API submit callback for the delete button.
 */
function roomify_property_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $form_state['redirect'] = array('admin/bat/config/property/manage/' . $form_state['roomify_property']->property_id . '/delete', array('query' => $destination));
}

/**
 * Suspend Property.
 */
function roomify_property_form_submit_suspend($form, &$form_state) {
  $form_state['roomify_property']->status = 0;
  $form_state['roomify_property']->save();
}

/**
 * Activate Property.
 */
function roomify_property_form_submit_activate($form, &$form_state) {
  $form_state['roomify_property']->status = 1;
  $form_state['roomify_property']->save();
}

/**
 * Page to add Property.
 */
function roomify_property_add_page() {
  $controller = entity_ui_controller('roomify_property');
  return $controller->addPage();
}

/**
 * Displays the list of available property types for property creation.
 *
 * @ingroup themeable
 */
function theme_roomify_property_add_list($variables) {
  $content = $variables['content'];

  $output = '';
  if ($content) {
    $output = '<dl class="property-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer property types')) {
      $output = '<p>' . t('Properties cannot be added because you have not created any property types yet. Go to the <a href="@create-property-type">property type creation page</a> to add a new property type.', array('@create-property-type' => url('admin/bat/config/property-types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No property types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Sets the breadcrumb for administrative BAT pages.
 */
function roomify_property_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('BAT'), 'admin/bat'),
    l(t('Configuration'), 'admin/bat/config'),
    l(t('Roomify Properties'), 'admin/bat/config/property'),
  );

  drupal_set_breadcrumb($breadcrumb);
}

/**
 * @param $property
 * @param $revision
 */
function roomify_property_revision_view($property, $revision) {
  drupal_set_title(t('Revision of %title from %date', array('%title' => $property->name, '%date' => format_date($property->revision_timestamp))), PASS_THROUGH);

  return entity_view('roomify_property', array(roomify_property_load_revision($revision)));
}

/**
 * @param $property
 * @param $revision
 */
function roomify_property_revision_revert_confirm($form, $form_state, $property, $revision) {
  $property_revision = roomify_property_load_revision($revision);
  $form['#property_revision'] = $property_revision;

  return confirm_form($form, t('Are you sure you want to revert to the revision from %revision-date?', array('%revision-date' => format_date($property_revision->revision_timestamp))), 'admin/bat/config/property/manage/' . $type_revision->property_id . '/revisions', '', t('Revert'), t('Cancel'));
}

/**
 * Form submission handler for roomify_property_revision_revert_confirm().
 */
function roomify_property_revision_revert_confirm_submit($form, &$form_state) {
  $property_revision = $form['#property_revision'];

  $property_revision->revision = TRUE;
  $property_revision->default_revision = TRUE;
  $property_revision->save();

  $form_state['redirect'] = 'admin/bat/config/property/manage/' . $property_revision->property_id . '/revisions';
}

/**
 * @param $property
 * @param $revision
 */
function roomify_property_revision_delete_confirm($form, $form_state, $property, $revision) {
  $property_revision = roomify_property_load_revision($revision);
  $form['#property_revision'] = $property_revision;

  return confirm_form($form, t('Are you sure you want to delete the revision from %revision-date?', array('%revision-date' => format_date($property_revision->revision_timestamp))), 'admin/bat/config/types/manage/' . $property_revision->property_id . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for roomify_property_revision_delete_confirm().
 */
function roomify_property_revision_delete_confirm_submit($form, &$form_state) {
  $property_revision = $form['#property_revision'];
  entity_revision_delete('roomify_property', $property_revision->revision_id);

  $form_state['redirect'] = 'admin/bat/config/property/manage/' . $property_revision->property_id . '/revisions';
}
