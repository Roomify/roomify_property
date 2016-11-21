<?php

/**
 * @file
 * RoomifyPropertyType editing UI.
 */

/**
 * UI controller.
 */
class RoomifyPropertyTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    foreach ($items as &$item) {
      unset($item['access callback']);
      $item['access arguments'] = array('administer roomify_property_type entities');
    }

    return $items;
  }

}

/**
 * Geneproperties the property type editing form.
 */
function roomify_property_type_form($form, &$form_state, $property_type, $op = 'edit') {
  if ($op == 'clone') {
    $property_type->label .= ' (cloned)';
    $property_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('property type name'),
    '#type' => 'textfield',
    '#default_value' => $property_type->label,
    '#description' => t('The human-readable name of this property type.'),
    '#required' => TRUE,
    '#size' => 30,
    '#weight' => -100,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($property_type->type) ? $property_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'roomify_property_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this property type. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => -99,
  );

  if ($op == 'edit') {
    $form['type']['#disabled'] = TRUE;
  }

  // Add the field related form elements.
  $form_state['roomify_property_type'] = $property_type;
  field_attach_form('roomify_property_type', $property_type, $form, $form_state);

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  $form['publishing_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publishing options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );

  $form['publishing_options']['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new revision'),
    '#default_value' => (isset($type_bundle->data['revision'])) ? $type_bundle->data['revision'] : 0,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save property type'),
    '#weight' => 100,
    '#submit' => array('roomify_property_type_form_submit'),
  );

  $form['#validate'][] = 'roomify_property_type_form_validate';

  return $form;
}

/**
 * Form API validate callback for the property type form.
 */
function roomify_property_type_form_validate(&$form, &$form_state) {
  // Notify field widgets to validate their data.
  entity_form_field_validate('roomify_property_type', $form, $form_state);
}

/**
 * Form API submit callback for the property type form.
 */
function roomify_property_type_form_submit(&$form, &$form_state) {
  $property_type = entity_ui_controller('roomify_property_type')->entityFormSubmitBuildEntity($form, $form_state);

  $property_type->data['revision'] = $form_state['values']['revision'];

  // Save and go back.
  $property_type->save();

  $form_state['property_type'] = $property_type;

  $form_state['redirect'] = 'admin/bat/config/property-types';
}
