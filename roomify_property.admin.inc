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
    $items[$this->path]['title'] = 'Roomify Properties';

    // Change the add page menu to multiple types of entities.
    $items[$this->path . '/add']['title'] = 'Add a Property';
    $items[$this->path . '/add']['description'] = 'Create a new property.';
    $items[$this->path . '/add']['page callback'] = 'roomify_property_add_page';
    $items[$this->path . '/add']['access callback'] = 'roomify_property_add_access';
    unset($items[$this->path . '/add']['title callback']);

    // Add property type specific calendar menu item.
    $items[$this->path . '/manage/%roomify_property/calendar/availability'] = array(
      'title' => 'Manage availability',
      'page callback' => 'roomify_property_calendar_wrapper',
      'page arguments' => array(5, 'availability'),
      'access callback' => 'roomify_property_add_access',
      'file' => 'roomify_property.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
      'type' => MENU_LOCAL_TASK,
    );

    // Add property type specific calendar menu item.
    $items[$this->path . '/manage/%roomify_property/calendar/pricing'] = array(
      'title' => 'Manage pricing',
      'page callback' => 'roomify_property_calendar_wrapper',
      'page arguments' => array(5, 'pricing'),
      'access callback' => 'roomify_property_add_access',
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

    return $items;
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
  drupal_set_title(t('Edit @property_name', array('@property_name' => $property->name)));

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
  field_attach_form('roomify_property', $property, $form, $form_state, isset($property->language)? $property->language : NULL);
  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Type author information for administrators.
  $form['author'] = array(
    '#type' => 'fieldset',
    '#access' => user_access('bypass bat_type entities access'),
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
  if (!empty($property->name) && roomify_property_access('delete', $property)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Property'),
      '#suffix' => l(t('Cancel'), 'admin/bat/config/property'),
      '#submit' => $submit + array('roomify_property_form_submit_delete'),
      '#weight' => 45,
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
function roomify_property_edit_form_submit(&$form, &$form_state) {
  $property = entity_ui_controller('roomify_property')->entityFormSubmitBuildEntity($form, $form_state);

  $property->created = !empty($property->date) ? strtotime($property->date) : REQUEST_TIME;
  $property->changed = time();

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
function roomify_property_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/bat/config/property/manage/' . $form_state['roomify_property']->property_id . '/delete';
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
