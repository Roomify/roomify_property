<?php

/**
 * @file
 */

/**
 *
 */
class AvailabilityCalendarPluginOneRow extends views_plugin_style {

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->view->query->add_field($this->view->base_table, $this->view->base_field);
    parent::query();
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = array(
      'event_type' => array('default' => ''),
    );

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
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
  public function render() {
    global $theme;

    $types = array();

    $grouping_ids = array();

    foreach ($this->view->result as $row) {
      $property = roomify_property_load($row->property_id);

      $grouping_ids[$property->property_id] = array();

      foreach ($property->property_bat_type_reference[LANGUAGE_NONE] as $type) {
        if (!in_array($type['target_id'], $types)) {
          $types[] = $type['target_id'];
        }
        $grouping_ids[$property->property_id][] = $type['target_id'];
      }

      if (empty($grouping_ids[$property->property_id])) {
        unset($grouping_ids[$property->property_id]);
      }
      else {
        $grouping_ids[$property->property_id] = $property->property_id . ':' . implode(',', $grouping_ids[$property->property_id]);
      }
    }

    $event_type = bat_event_type_load($this->options['event_type']);

    $calendar_id = drupal_html_id('availability_calendar');

    $fc_user_settings = array(
      array(
        'unitType' => implode(',', $types),
        'eventType' => $this->options['event_type'],
        'calendar_id' => 'fullcalendar-scheduler',
        'grouping_entity_type' => 'roomify_property',
        'grouping_ids' => implode(';', $grouping_ids),
        'collapse_childrens' => 1,
        'calendarHeight' => 'parent',
        'eventGranularity' => $event_type->event_granularity,
        'themeSystem' => ($theme == 'roomify_travel') ? 'bootstrap3' : 'standard',
        'editable' => ($theme == 'roomify_travel') ? FALSE : TRUE,
        'selectable' => ($theme == 'roomify_travel') ? FALSE : TRUE,
        'enableModal' => ($theme == 'roomify_travel') ? FALSE : TRUE,
        'slotWidth' => 55,
      ),
    );

    $calendar_settings = array(
      'modal_style' => 'default',
      'calendar_id' => 'fullcalendar-scheduler',
      'user_settings' => array('batCalendar' => $fc_user_settings),
    );

    $output = theme('bat_fullcalendar', array(
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
