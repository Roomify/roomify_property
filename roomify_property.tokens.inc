<?php

/**
 * Implements hook_token_info().
 */
function roomify_property_token_info() {
  $type = array(
    'name' => t('Roomify Property', array(), array('context' => 'a roomify property')),
    'description' => t('Tokens related to roomify property.'),
    'needs-data' => 'roomify-property',
  );

  return array(
    'types' => array('roomify_property' => $type),
    'tokens' => array('roomify_property' => array()),
  );
}
