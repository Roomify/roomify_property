<?php

/**
 * @file
 */

/**
 *
 */
class BatActivityStateWidget extends FacetapiWidget {

  /**
   * Execution callback.
   */
  public function execute() {
    $elements = &$this->build[$this->facet['field alias']];
    $elements = drupal_get_form('property_bat_activity_facets_availability', $elements);
  }

  /**
   * Overrides FacetapiWidget::settingsForm().
   */
  public function settingsForm(&$form, &$form_state) {
    parent::settingsForm($form, $form_state);
    $settings = $this->settings->settings;

    unset($form['widget']['widget_settings']['links'][$this->id]['soft_limit']);
    unset($form['widget']['widget_settings']['links'][$this->id]['show_expanded']);
    unset($form['widget']['widget_settings'][$this->id]['facet_more_text']);
  }

}
