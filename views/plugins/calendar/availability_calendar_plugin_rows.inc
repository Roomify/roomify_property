<?php

/**
 * @file
 */

/**
 *
 */
class AvailabilityCalendarPluginRows extends views_plugin_row {

  /**
   * {@inheritdoc}
   */
  public function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    // Initialize the entity-type used.
    $table_data = views_fetch_data($this->view->base_table);
    $this->entity_type = $table_data['table']['entity type'];
    // Set base table and field information as used by views_plugin_row to
    // select the entity id if used with default query class.
    $info = entity_get_info($this->entity_type);
    if (!empty($info['base table']) && $info['base table'] == $this->view->base_table) {
      $this->base_table = $info['base table'];
      $this->base_field = $info['entity keys']['id'];
    }
  }

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = array(
      'event_type' => array('default' => ''),
    );

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $event_types_options = array();
    $event_types = bat_event_get_types();
    foreach ($event_types as $event_type) {
      $event_types_options[$event_type->type] = $event_type->label;
    }

    $form['event_type'] = array(
      '#type' => 'select',
      '#title' => t('Event type'),
      '#options' => $event_types_options,
      '#required' => TRUE,
      '#default_value' => $this->options['event_type'],
    );
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $property = roomify_property_load($values->property_id);

    $output = '<h2 class="property-title">' . $property->name . '</h2>';

    $types = array();
    foreach ($property->property_bat_type_reference[LANGUAGE_NONE] as $type) {
      $types[] = $type['target_id'];
    }

    $calendar_id = drupal_html_id('availability_calendar_' . $property->property_id);

    $fc_user_settings = array(
      array(
        'unitType' => implode(',', $types),
        'eventType' => $this->options['event_type'],
        'calendar_id' => 'fullcalendar-scheduler',
      ),
    );

    $calendar_settings = array(
      'modal_style' => 'default',
      'calendar_id' => 'fullcalendar-scheduler',
      'user_settings' => array('batCalendar' => $fc_user_settings),
    );

    $output .= theme('bat_fullcalendar', array(
      'calendar_settings' => $calendar_settings,
      'attributes' => array(
        'id' => $calendar_id,
        'class' => array(
          'cal',
          'calendar-set',
          'clearfix',
        ),
      ),
    ));

    return $output;
  }

}
