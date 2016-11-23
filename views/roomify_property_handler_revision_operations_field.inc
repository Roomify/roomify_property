<?php

/**
 * @file
 * This field handler aggregates operations that can be done on a type
 * under a single field providing a more flexible way to present them in a view.
 */

/**
 *
 */
class roomify_property_handler_revision_operations_field extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function construct() {
    parent::construct();

    $this->additional_fields['property_id'] = 'property_id';
    $this->additional_fields['revision_id'] = 'revision_id';
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $property_id = $this->get_value($values, 'property_id');
    $revision_id = $this->get_value($values, 'revision_id');

    $property = roomify_property_load_revision($revision_id);
    if (entity_revision_is_default('roomify_property', $property)) {
      return '<b>' . t('This is the published revision.') . '</b>';
    }
    else {
      $links = array();

      if (roomify_property_access_revision('view', $property, $revision_id)) {
        $links[] = array(
          'href'  => 'admin/bat/config/property/manage/' . $property_id . '/revision/' . $revision_id . '/view',
          'title' => t('View'),
        );
      }

      if (roomify_property_access_revision('revert', $property, $revision_id)) {
        $links[] = array(
          'href'  => 'admin/bat/config/property/manage/' . $property_id . '/revision/' . $revision_id . '/revert',
          'title' => t('Revert'),
        );
      }

      if (roomify_property_access_revision('delete', $property, $revision_id)) {
        $links[] = array(
          'href'  => 'admin/bat/config/property/manage/' . $property_id . '/revision/' . $revision_id . '/delete',
          'title' => t('Delete'),
        );
      }

      if (!empty($links)) {
        return theme('links', array(
          'links' => $links,
          'attributes' => array(
            'class' => array(
              'links',
              'inline',
              'operations',
            ),
          ),
        ));
      }
    }
  }

}
